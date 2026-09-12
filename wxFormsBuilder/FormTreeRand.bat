set name=../source/forms/form_trees_rand
Powershell.exe -executionpolicy remotesigned -File wxrep.ps1 %name%.cpp "forms.cpp" "FormTreeRand"
Powershell.exe -executionpolicy remotesigned -File wxrep.ps1 %name%.h "forms.h" "FormTreeRand"



