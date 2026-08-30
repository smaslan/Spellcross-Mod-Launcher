#pragma once

#include <filesystem>
#include <vector>
#include <string>
#include <map>
#include <memory>
#include "fs_archive.h"
#include "spell_font.h"

class SpellSaveResearch{
public:
    static const std::map<int,std::string> c_groups;
    static const std::map<int,std::string> c_flags;

    enum Group : int
    {
        NONE,
        GLOBAL,
        RACES,
        UPGRADE,
        TECH
    };
    enum Flags : int
    {        
        None,
        Special,
        UpgradeItem,
        UnitType,
        Info,        
        NewUnit,        
    };
    
    std::vector<uint8_t> raw;
    std::wstring name;
    Flags flags;
    Group group;
    int cost;
    int time;
    int level;
    int data_id;
    int state;
    int available;
    std::vector<int> or_connections;

    std::string GetGroupName()
    {
        if(c_groups.find(group) != c_groups.end())
            return(c_groups.at(group));
        return("");
    };
    std::string GetFlagName()
    {
        if(c_flags.find(group) != c_flags.end())
            return(c_flags.at(flags));
        return("");
    };
    int SetGroup(std::string name)
    {
        auto it = std::find_if(c_groups.begin(),c_groups.end(),[name](const auto& item) {return(item.second == name);});
        if(it == c_groups.end())
            return(1);
        group = (Group)it->first;
        return(0);
    }
    int SetFlags(std::string name)
    {
        auto it = std::find_if(c_flags.begin(),c_flags.end(),[name](const auto& item) {return(item.second == name);});
        if(it == c_flags.end())
            return(1);
        flags = (Flags)it->first;
        return(0);
    }
};

class SpellSaveUpgrade {
public:
    static const std::map<int,std::string> c_flags;
    
    enum UpgradeClass : int
    {
        NONE,
        ENGINE,
        WEAPON,
        ARMOR
    };

    std::vector<uint8_t> raw;
    std::wstring name;
    int sight;
    int attack;
    int attack_pt;
    int move;
    int defence;
    int range;
    std::vector<int> suitable_types;
    UpgradeClass type;
    int upg_time;
    int upg_price;
    int state;

    std::string GetUpgradeClassStr()
    {
        if(c_flags.find(type) != c_flags.end())
            return(c_flags.at(type));
        return("");
    }
    int SetFlags(std::string name)
    {
        auto it = std::find_if(c_flags.begin(),c_flags.end(),[name](const auto& item) {return(item.second == name);});
        if(it == c_flags.end())
            return(1);
        type = (UpgradeClass)it->first;
        return(0);
    }
   
};

class SpellSaveUnits {
public:
    std::vector<uint8_t> raw;
    std::wstring name;
    int unit_type_id;
    int xp;
    int xp_level;
    int flags;
    int hp;
    int hp_max;
    int hierarch_pos;
    int upg_armor;
    int upg_weapon;
    int upg_engine;
    
    int upg_timeout;
    int upg_unit_type;
    int action_timeout;

    bool is_empty() { return(flags == 0);};
    bool is_permanent() {return(!!(flags & 0x01));};
    bool is_reinforce() { return(!!(flags & 0x02));};
    void SetReinforce(bool is_reinforcement)
    {
        flags = (flags & ~0x03) | 1 << ((int)!!is_reinforcement);
    };
    void clear()
    {
        raw.assign(66,0);
        name = L"<empty slot>";
        unit_type_id = 0;
        xp = 0;
        xp_level = 0;
        flags = 0;
        hp = 0;
        hp_max = 0;
        hierarch_pos = -1;
        upg_armor = -1;
        upg_weapon = -1;
        upg_engine = -1;
        upg_timeout = 0;
        upg_unit_type = 0;
        action_timeout = 0;
    };
};

class SpellSaveCommanders {
public:
    std::vector<uint8_t> raw;
    std::wstring name;    
    int rank;
    int battles;
    int unit_id;
    int flags;
    int x38;
    int valid;

    std::vector<std::wstring>* m_rank_names;

    int is_empty() {return(!valid);};
    int command_level() {return(flags >> 7);}; // id of hierarchy level {0, 1, 2}
    int command_pos() {return(flags & 0x0F);}; // id of position in hierarchy table
    bool is_placed() {return{flags != 0x0000 && flags != 0xFFFF};}; // is placed in hierarchy table?
    void clear()
    {
        raw.assign(44,0);
        name = L"<empty slot>";
        rank = 0;
        battles = 0;
        unit_id = -1;
        flags = 0x0000;
        x38 = 0;
        valid = 0;
    }
    // place commander to hierarchy slot
    void place(int level=-1, int pos=-1)
    {
        if(level < 0 || pos < 0)
        {
            flags = 0;
            unit_id = -1;
        }
        else
            flags = (pos & 0x0F) | (0x40 << level);
    };
    // get rank name string (if exist)
    std::wstring rank_name()
    {
        if(!m_rank_names)
            return(L"");
        if(rank >= m_rank_names->size())
            return(L"");
        return(m_rank_names->at(rank));
    }
    // get full name with rank
    std::wstring full_name()
    {
        if(is_empty())
            return(name);
        auto rank_str = rank_name();
        if(!rank_str.empty())
            rank_str += L" ";
        return(rank_str + name);
    }

};

class SpellSaveEvents
{
public:
    int time;
    int flags;
};

class SpellSaveTerritories
{
public:
    static const std::map<int,std::string> c_states;
    
    std::string mus_name;
    std::string dta_name;
    std::string def_name;
    int remain_money;
    int money_per_round;
    int remain_time;
    int freq_random_attacks_a;
    int freq_random_attacks_b;
    int flags;    
    bool valid;
    uint32_t ssd_flags;
    std::vector<uint8_t> raw;
    int x_center;
    int y_center;
    
    bool isAliance() {return(flags == 0x0F);};
    bool isAccessible() { return(flags == 0x0D);};
    bool isEnemy() { return(flags == 0x00 || flags == 0x0D || flags == 0x0C || flags == 0x10 || flags == 0x1D);};
    std::string GetStateString()
    {
        auto fid = c_states.find(flags);
        if(fid == c_states.end())
            return("Unknown");
        return(fid->second);
    }        
};


class SpellSaveBigmap
{
public:
    int level;
    int terr_count;
    int final_terr;
    int x_size;
    int y_size;
    std::vector<uint8_t> image;
    std::vector<uint8_t> pal;
    std::vector<uint8_t> terr_mask;
    std::vector<uint8_t> terr_hatch;

    std::vector<SpellSaveTerritories> terr;
};

class SpellSaveLevel
{
public:
    int money;
    int money_research;
    int round;
    
    int xp;
    int rank;

    int stat_kill_light;
    int stat_kill_armor;
    int stat_kill_air;
    int stat_kill_light_tot;
    int stat_kill_armor_tot;
    int stat_kill_air_tot;

    int stat_loss_light;
    int stat_loss_armor;
    int stat_loss_air;
    int stat_loss_com;
    int stat_loss_light_tot;
    int stat_loss_armor_tot;
    int stat_loss_air_tot;
    int stat_loss_com_tot;

    std::string level_music;
    std::vector<int> attack_units;
    std::vector<int> attack_spec_units;
    int attack_flags_non_spec;
    int attack_flags_total;
    int attack_flags_xp_level;
    int attack_flags_xp_level2;
    int attack_flags_xp_f_attack_a;
    int attack_flags_xp_f_attack_b;

    int difficulty;
};

class SpellUnitRec;
class SpellUnits;

class SpellSaveBigMap{
private:
    std::shared_ptr<FSarchive> m_common_fs;
    std::vector<std::wstring> m_rank_names;
    std::vector<std::wstring> m_unit_names;
    std::vector<std::wstring> m_commander_names;
    std::shared_ptr<SpellUnits> m_jednotky_def;

    static int c_default_id;

public:
    std::filesystem::path m_path;
    bool is_loaded;

    std::vector<uint8_t> raw;
    std::vector<SpellSaveResearch> research;
    std::vector<SpellSaveUpgrade> upgrade;
    std::vector<SpellSaveUnits> units;
    std::vector<SpellSaveCommanders> commanders;
    std::vector<SpellSaveEvents> events;
    SpellSaveBigmap bigmap;
    SpellSaveLevel level;
    

    SpellSaveBigMap();
    int Load(std::filesystem::path path, std::filesystem::path common_fs_path="");
    int Save(std::filesystem::path path);
    int SortUnits(bool remove_gaps,bool separate,bool by_types,bool by_names);
    int SwapUnits(int id_a,int id_b);
    int ResetUnitName(int unit_id=-1,bool also_reinforces=false);
    int HealUnits();
    int FixUnit(int uid,bool force_xp_level_update=false);
    int AddUnit(int &uid=c_default_id);
    int RemUnit(int uid);
    int SetUnitReinforcement(int uid,bool is_reinforcement);
    int RemCommander(int cid);
    int AddCommander(int& cid);

    int SyncUpgrades();
    int SyncResearch();
    int SyncLevel();

    bool wasUnitEncountered(SpellUnitRec *unit);

    std::map<int,std::wstring> GetUpgradeList(SpellSaveUpgrade::UpgradeClass type);
    std::map<int,std::wstring> GetUnitTypeList(bool add_empty=false,bool with_id=false);
    std::map<int,std::wstring> GetUnitNames(bool with_id=false);
    std::map<int,std::wstring> GetRanksList();
};

class SpellSave{
public:

    class Save{
    public:
        std::string dir_name;
        std::filesystem::path dir_path;
        std::wstring name;
        std::string date;
        bool is_empty;
        SpellSaveBigMap m_bigmap;
    };
    typedef std::vector<Save> Saves;
    

    static int LoadSave(std::filesystem::path dir,Save &save,bool allow_empty=true);
    static int LoadSaves(std::filesystem::path dir, Saves &saves,bool skip_missing=true);
    static bool CheckSaves(std::filesystem::path dir);
};

bool DecodeCLK(const std::vector<uint8_t>& clkBytes,int& outW,int& outH,std::vector<uint8_t>& values);