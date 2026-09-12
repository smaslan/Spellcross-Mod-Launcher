//=============================================================================
// Spellcross map/game scripts randomizer stuff (units, trees, ...).
// 
// This code is part of Spellcross Map Editor project.
// (c) 2026, Stanislav Maslan, s.maslan@seznam.cz
// url: https://github.com/smaslan/spellcross-map-edit
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================
#pragma once

#include <vector>
#include <string>
#include <filesystem>
#include "SimpleIni.h"

#include "spell_def.h"

class SpellUnits;
class Terrain;
class FSarchive;



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
    int Validate(SpellUnits* units,bool fix=true);
    void Normalize();
    int Randomize(int src_unit_id,int& uid);
    
};

class UnitRandomizerSetup{
public:
    int xp_min;
    int xp_max;
    bool randomize_xp;
    bool apply_tough_def;
    bool apply_static;
    bool apply_events;
    bool apply_level_army;
    bool apply_level_rand_attack_units;
    bool apply_level_rand_spec_units;
    bool override_explicit_rule;
    bool override_off_rule;
    bool filter_by_save_research;

    std::filesystem::path m_path;

    UnitRandomizerGlobRule rules_light;
    UnitRandomizerGlobRule rules_armor;
    UnitRandomizerGlobRule rules_air;

    bool canOverrideLocalRule();
    int Validate(SpellUnits* units,bool fix=true);
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
    static int RandomizeLevel(std::string& def,SpellUnits* units,UnitRandomizerSetup* glob_rules);
};










class SpellTreeTool {
public:
    int id;
    std::string name;
    std::vector<std::string> sprites;
    SpellTreeTool(int id,std::string name,std::vector<std::string>& sprites) { this->id = id; this->name = name; this->sprites = sprites; };
};

class SpellTreeToolset {
public:
    std::string m_name;
    std::string m_terrain_name;
    std::vector<SpellTreeTool> m_tools;

    int LoadInfo(std::filesystem::path info_path,std::string terr_name="");
};

class SpellTreeRandomizerItem {
public:
    std::string name;
    double probab;
    int id;
    SpellTreeRandomizerItem(std::string name,double probab=0.0) { this->name = name; this->probab = probab; };
};

class SpellTreeRandomizerRule {
private:
    std::vector<double> m_pdf;

public:
    std::string name;
    std::vector<SpellTreeRandomizerItem> src_trees;
    std::vector<SpellTreeRandomizerItem> rand_trees;
    
    int FilterTrees(std::vector<std::string>& list);
    SpellTreeRandomizerItem* GetSrcTree(std::string name);
    SpellTreeRandomizerItem* GetRandTree(std::string name);    
    int UpdateSrcTree(std::string name,bool state);
    int UpdateRandTree(std::string name,bool state);
    int SetEqualProb();
    int SetRandomProb();
    int ClearProb();
    int FixProb(std::string ref_name="");
    int PrepareRng();
    SpellTreeRandomizerItem *GetRandomTree();
};

class SpellTreeRandomizerTerrain {
public:
    std::string name;
    std::shared_ptr<Terrain> terr;
    std::vector<SpellTreeRandomizerRule> rules;
    std::filesystem::path m_path;
    SpellTreeToolset map_toolset;

    std::vector<std::string> m_sprite_names;
    std::vector<SpellTreeRandomizerRule*> m_sprite_rules;
    int MakeSpriteRules(std::vector<std::string> &names);
    int GetRandomTreeID(int id);

    SpellTreeRandomizerTerrain();
    SpellTreeRandomizerRule* GetRule(std::string name);
    SpellTreeRandomizerRule* AddRule(std::string name="");
    int RemoveRule(std::string name);
    int FilterTrees(std::vector<std::string>& list);
    std::vector<std::string> GetUsedTrees(SpellTreeRandomizerRule* ref_rule);
};



class SpellTreeRandomizerRules {
public:
    std::vector<SpellTreeRandomizerTerrain> terrains;
    
    void Clear();
    SpellTreeRandomizerTerrain* AddTerrain(SpellTreeRandomizerTerrain *terrain);
    SpellTreeRandomizerTerrain* AddTerrain(std::string terrain_name);
    int RemoveTerrain(std::string terrain_name);
    SpellTreeRandomizerTerrain* GetTerrain(std::string terrain_name);
    int LoadPreset(std::filesystem::path info_path,std::string& terrain_name,bool add_new=false);
    int StorePreset(std::filesystem::path info_path,std::string terrain_name);
        
    int LoadINIpresets(CSimpleIniA *ini,std::string section,std::string key);
    int SaveINIpresets(CSimpleIniA* ini,std::string section,std::string key);
};

class SpellTreeRandomizer{
public:
    std::string m_last_error;
    SpellTreeRandomizerRules m_rules;

    int PrepareRules(SpellTreeRandomizerRules &rules, std::vector<std::shared_ptr<FSarchive>> terrain_fs_archives);
    int RandomizeMapDTA(std::vector<uint8_t> &dta,std::string dta_name="");
};