set name=../source/forms/form_dialog
Powershell.exe -executionpolicy remotesigned -File wxrep.ps1 %name%.cpp "forms.cpp" "FormDialog"
Powershell.exe -executionpolicy remotesigned -File wxrep.ps1 %name%.h "forms.h" "FormDialog"



