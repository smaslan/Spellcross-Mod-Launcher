set name=../source/forms/form_patch
Powershell.exe -executionpolicy remotesigned -File wxrep.ps1 %name%.cpp "forms.cpp" "FormPatch"
Powershell.exe -executionpolicy remotesigned -File wxrep.ps1 %name%.h "forms.h" "FormPatch"



