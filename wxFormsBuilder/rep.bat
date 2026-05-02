Powershell.exe -executionpolicy remotesigned -File wxrep.ps1 "../source/main.cpp" "forms.cpp" "FormMain"
Powershell.exe -executionpolicy remotesigned -File wxrep.ps1 "../source/main.h" "forms.h" "FormMain"

Powershell.exe -executionpolicy remotesigned -File wxrep.ps1 "../source/forms/form_about.cpp" "forms.cpp" "FormAbout"
Powershell.exe -executionpolicy remotesigned -File wxrep.ps1 "../source/forms/form_about.h" "forms.h" "FormAbout"

Powershell.exe -executionpolicy remotesigned -File wxrep.ps1 "../source/forms/form_edit.cpp" "forms.cpp" "FormEdit"
Powershell.exe -executionpolicy remotesigned -File wxrep.ps1 "../source/forms/form_edit.h" "forms.h" "FormEdit"

Powershell.exe -executionpolicy remotesigned -File wxrep.ps1 "../source/forms/form_save_backup.cpp" "forms.cpp" "FormSaveBack"
Powershell.exe -executionpolicy remotesigned -File wxrep.ps1 "../source/forms/form_save_backup.h" "forms.h" "FormSaveBack"



