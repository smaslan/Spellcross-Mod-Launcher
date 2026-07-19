//=============================================================================
// Spellcross map units randomizer stuff.
// 
// This code is part of Spellcross Map Editor project.
// (c) 2026, Stanislav Maslan, s.maslan@seznam.cz
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================
#pragma once

#include <vector>

#include "spell_def.h"

class SpellUnits;

class UnitRandomizerRule{    
public:
    int id;
    int ref_unit;
    std::vector<int> rand_units;
};

class UnitRandomizer{
public:
    std::string last_error;
    std::vector<UnitRandomizerRule> rules;
        
    UnitRandomizer();
    void Clear();
    int AddRule(SpellDefCmd* cmd, SpellUnits* units);
    int AddRule(SpellUnits* units,int &new_pos);    
    int RemoveRule(int pos);
    int AddMirrorRules(int pos, int& new_pos);
    int CheckRules();
    int SortRules(int old_pos=-1);
    std::string MakeRulesDEF(std::string indent="");
    UnitRandomizerRule *GetRule(int type_id);

    // map randomizer
    static int RandomizeMap(std::string &def, SpellUnits *units, std::string &error);
};


