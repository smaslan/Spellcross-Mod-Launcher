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

class UnitRandomizerGlobRule{
public:
    std::vector<int> src_unit_list;
    std::vector<int> unit_list;
    std::vector<double> probab_list;

    bool isSrcUnit(int uid);
    bool isTargetUnit(int uid);
    int Randomize(int& uid);
};

class UnitRandomizerSetup{
public:
    int xp_min;
    int xp_max;
    bool randomize_xp;
    bool apply_tough_def;
    bool apply_static;
    bool apply_events;
    bool override_explicit_rule;
    bool override_off_rule;

    std::filesystem::path m_path;

    UnitRandomizerGlobRule rules_light;
    UnitRandomizerGlobRule rules_armor;
    UnitRandomizerGlobRule rules_air;

    bool canOverrideLocalRule();
    int Randomize(SpellUnits *units, int src_unit_id, int &unit);
    int LoadIni(std::filesystem::path path,SpellUnits* units=NULL);
    int SaveIni(std::filesystem::path path,SpellUnits* units=NULL);
};

class UnitRandomizer{
public:    
    static std::string m_last_error;

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
    static int RandomizeMap(std::string &def, SpellUnits *units, UnitRandomizerSetup *glob_rules=NULL);
};


