set name=../source/forms/form_units_rand
Powershell.exe -executionpolicy remotesigned -File wxrep.ps1 %name%.cpp "forms.cpp" "FormUnitRand"
Powershell.exe -executionpolicy remotesigned -File wxrep.ps1 %name%.h "forms.h" "FormUnitRand"



