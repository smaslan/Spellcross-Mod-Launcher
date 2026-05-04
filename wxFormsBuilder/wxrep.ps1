#params: target_source_file_path wxforms_source_path class_name

Function replace-tagged-section{
    param ($data, $key, $insert, $notes)
    
    $start_tag = ("<{0}>" -f $key)
    $end_tag = ("<\/{0}>" -f $key)
        
    $before = $data | Select-String $start_tag -Context 100000,0
    $after = $data | Select-String $end_tag -Context 0,100000
        
    $found = $before.Line -match '([\s]*)(.*%key%)'.replace('%key%',$start_tag)
    if(!$found) {
        Write-Error 'Error parsing source data!' 
    }
    $indent = $matches[1]
    $before_line = $matches[1] + $matches[2] + " - " + $notes
    
    $found = $after.Line -match '(.*%key%)'.replace('%key%',$end_tag)
    if(!$found) {
        Write-Error 'Error parsing source data!' 
    }
    $after_line = $matches[1] + " - " + $notes
    
    $lines = $insert.Split(
        @("`r`n", "`r", "`n"), 
        [StringSplitOptions]::None)
        
    $found =  $lines[0] -match '([\s]*)'
    if(!$found) {
        Write-Error 'Error parsing insert data!' 
    }
    $rem_indent = $matches[1]
    
    $insert = ""
    foreach($line in $lines){        
        $insert = $insert + $indent + ($line -replace $rem_indent,"") + "`r`n"
    }
    
    return ($before.Context.PreContext + $before_line + $insert + $after_line + $after.Context.PostContext) 
}



$ErrorActionPreference = "Stop"
if($args.Count -ne 3)
{
    Write-Error 'Missing parameters! Usage: wxrep.ps1 target_source_file_path wxforms_source_path class_name' 
}
$target_source_file_path = $args[0]
$wxforms_source_path = $args[1]
$class_name = $args[2]

$note = ("Section auto-inserted from '{0}' class '{1}' on {2:yyyy-MM-dd} {2:HH:mm:ss}" -f $wxforms_source_path,$class_name,(Get-Date))

$file = Get-Content $target_source_file_path -Encoding UTF8

if($wxforms_source_path -match '\.h$')
{
    # header file mode:

    $head = Get-Content $wxforms_source_path -Encoding UTF8
    $head = [system.String]::Join("`r`n",($head | select-string -Pattern '#include'))
     
    $file = replace-tagged-section $file 'wxFormsBuilder-include' $head $note
    
    $head = Get-Content $wxforms_source_path -Raw -Encoding UTF8
    $found = $head -match '(?s).*class\s+%CLASS%.+?\n(\s*enum.*?)\s+public:'.replace('%CLASS%',$class_name)
    if(!$found) {
        Write-Error 'class not found in wxform_path!' 
    }
    $data = $matches[1]
    
     
    
    $file = replace-tagged-section $file 'wxFormsBuilder' $data $note
    
}
else
{
    # cpp file mode:
    
    $data = Get-Content $wxforms_source_path -Raw -Encoding UTF8
    $found = $data -match '(?s).*%CLASS%::%CLASS%.*?\n.*?\{.*?\n(.*?)\}'.replace('%CLASS%',$class_name)
    if(!$found) {
        Write-Error 'class not found in wxform_path!' 
    }
    $data = $matches[1]
         
    $file = replace-tagged-section $file 'wxFormsBuilder' $data $note
}

$file | Out-File -Encoding UTF8 -FilePath $target_source_file_path

