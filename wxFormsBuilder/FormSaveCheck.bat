set name=../source/forms/form_save_check
Powershell.exe -executionpolicy remotesigned -File wxrep.ps1 %name%.cpp "forms.cpp" "FormSaveCheck"
Powershell.exe -executionpolicy remotesigned -File wxrep.ps1 %name%.h "forms.h" "FormSaveCheck"



