#include "SpellSaves.h"
#include "other.h"
#include "LZ_spell.h"
#include "fs_archive.h"
#include "spell_def.h"
#include <format>


// load single spellcross save directory
int SpellSave::LoadSave(std::filesystem::path dir,Save &save, bool allow_empty)
{
    if(!std::filesystem::exists(dir))
        return(1);

    save.dir_path = dir;
    save.dir_name = dir.filename().string();
    save.is_empty = true;
    save.date = "";

    // try load name
    auto path = dir / "NAME.SAV";
    std::string name;
    auto err = loadstr(path,name);
    save.name = char2wstringCP895(name.c_str());
    if(err && !allow_empty)
        return(1);
    if(!err)
    {
        save.is_empty = false;        
        auto time = std::filesystem::last_write_time(path);
        save.date = get_time_str(time);
    }

    return(0);
}

// load spellcross saves directories (whole SAVE folder)
int SpellSave::LoadSaves(std::filesystem::path dir,Saves& saves, bool skip_missing)
{
    std::vector<std::string> names;
    for(int k = 0; k <= 8; k++)
        names.push_back(string_format("SAVE%04d",k));
    names.push_back("WORKDIR");

    saves.clear();
    for(auto &name: names)
    {
        auto path = dir / name;
        Save save;        
        auto err = LoadSave(path, save);
        if(err && !skip_missing)
        {
            saves.clear();
            return(1);
        }
        if(err)
            continue;
        saves.push_back(save);
    }

    return(0);
}

// check if saves dir contain at least one save slot (even empty one)
bool SpellSave::CheckSaves(std::filesystem::path dir)
{
    std::vector<std::string> names;
    for(int k = 0; k <= 8; k++)
        names.push_back(string_format("SAVE%04d",k));
    names.push_back("WORKDIR");

    for(auto& name: names)
    {
        auto path = dir / name;
        if(std::filesystem::exists(path))
            return(true);
    }
    return(false);
}





const std::map<int,std::string> SpellSaveTerritories::c_states = {
    {0x00,"Enemy"},
    {0x0F,"Player"},
    {0x0D,"Accessible"},
    {0x10,"Locked"},
    {0x1F,"Unlocked"},
    {0x1D,"Was unlocked (bug)"},
    {0x0C,"Was accessible"}};

const std::map<int,std::string> SpellSaveResearch::c_groups ={
    {SpellSaveResearch::Group::NONE,"None"},
    {SpellSaveResearch::Group::GLOBAL,"Global"},
    {SpellSaveResearch::Group::RACES,"Races"},
    {SpellSaveResearch::Group::UPGRADE,"Upgrades"},
    {SpellSaveResearch::Group::TECH,"Technologies"}};

const std::map<int,std::string> SpellSaveResearch::c_flags ={
    {SpellSaveResearch::Flags::Info,"Info"},
    {SpellSaveResearch::Flags::NewUnit,"NewUnit"},
    {SpellSaveResearch::Flags::Special,"Special"},
    {SpellSaveResearch::Flags::UnitType,"UnitType"},
    {SpellSaveResearch::Flags::UpgradeItem,"UpgradeItem"}};

const std::map<int,std::string> SpellSaveUpgrade::c_flags = {
    {UpgradeClass::NONE,"None"},
    {UpgradeClass::ENGINE,"Engine"},
    {UpgradeClass::WEAPON,"Weapon"},
    {UpgradeClass::ARMOR,"Armor"}};

SpellSaveBigMap::SpellSaveBigMap() :
    m_common_fs((FSarchive*)NULL)
{   
}


/*#define rd32sp(rd32sp_ptr) (*((int32_t*)rd32sp_ptr)++)
#define rd32s(rd32s_ptr) (*((int32_t*)rd32sp_ptr))
#define rd16sp(rd16sp_ptr) ((*((int16_t*)rd16sp_ptr)++))
#define rd16s(rd16s_ptr) ((*((int16_t*)rd16s_ptr)))*/

// reset unit(s) names to default
int SpellSaveBigMap::ResetUnitName(int unit_id, bool also_reinforces)
{
    std::vector<int> id;
    if(unit_id >= (int)units.size())
        return(1);
    else if(unit_id < 0)
    {
        for(int k = 0; k < units.size(); k++)
            if(also_reinforces || units[k].is_permanent())
                id.push_back(k);
    }
    else
        id.push_back(unit_id);

    for(auto &unit_id: id)
    {
        auto &unit = units[unit_id];        
        if(unit.is_empty() || unit.unit_type_id < 0 || unit.unit_type_id >= m_unit_names.size())
            continue;
        unit.name = m_unit_names[unit.unit_type_id];
    }
    return(0);
}

// get list of upgrades of given type
std::map<int,std::wstring> SpellSaveBigMap::GetUpgradeList(SpellSaveUpgrade::UpgradeClass type)
{
    std::map<int,std::wstring> list;
    list.insert({-1, L"<none>"});
    for(auto &upg: upgrade)
    {
        if(upg.type != type)
            continue;
        list.insert({&upg - upgrade.data(),upg.name});
    }
    return(list);
}
// get list of unit types
std::map<int,std::wstring> SpellSaveBigMap::GetUnitTypeList(bool add_empty,bool with_id)
{
    std::map<int,std::wstring> list;
    if(add_empty)
        list.insert({-1, L"<none>"});
    for(auto& unit: m_unit_names)
    {
        auto id = &unit - m_unit_names.data();
        if(with_id)
            list.insert({id,wstring_format(L"#%02d: %ls",id,unit.c_str())});            
        else
            list.insert({id,unit});            
    }
    return(list);
}

// get list of unit name
std::map<int,std::wstring> SpellSaveBigMap::GetUnitNames(bool with_id)
{
    std::map<int,std::wstring> list;
    for(auto &unit: m_unit_names)
    {
        auto id = &unit - m_unit_names.data();
        if(with_id)
            list.insert({id,wstring_format(L"#%02d: %ls",id,unit.c_str())});
        else
            list.insert({id,unit});
            
    }
    return(list);
}

// get list of rank strings
std::map<int,std::wstring> SpellSaveBigMap::GetRanksList()
{
    std::map<int,std::wstring> list;
    for(auto &rank: m_rank_names)
        list.insert({&rank - m_rank_names.data(), rank});
    return(list);
}

// get string of up to max len
std::string get_save_str(uint8_t* data,uint8_t *data_end,int max)
{
    std::string str;
    for(int k = 0; k < max; k++)
    {
        if(data >= data_end)
            return(str);
        if(!*data)
            break;
        str.append(1,(char)*data);
        data++;
    }
    return(str);
}

// store string of up to max len
int store_save_str(uint8_t* data,uint8_t* data_end,int len,std::string str)
{
   if(len <= 0)
       return(1);
   if(data + len >= data_end) 
       return(1);
   if(str.length() > len)
       return(1);
   memset(data,'\0',len);
   memcpy(data,str.c_str(),str.size());
   return(0);
}

// read array of int16 with elements count given by first uint16
std::vector<int> get_save_array_i16(uint8_t* data,uint8_t* data_end)
{
    int count = *(uint16_t*)data; data += sizeof(uint16_t);
    std::vector<int> list;
    for(int k = 0; k < count; k++)
    {
        if(data >= data_end - sizeof(int16_t) + 1)
            return(list);
        list.push_back(*(uint16_t*)data);
        data += sizeof(uint16_t);
    }
    return(list);
}

// store array of int16 with elements count given by first uint16
int store_save_array_i16(uint8_t* data,uint8_t* data_end,std::vector<int> &list,int max_len)
{
    if(data + sizeof(uint16_t) >= data_end)
        return(1);
    if(list.size() > max_len)
        return(1);
    *(uint16_t*)data = list.size(); data += sizeof(uint16_t);
    for(auto &val: list)
    {
        if(data + sizeof(int16_t) >= data_end)
            return(1);
        *(uint16_t*)data = val; data += sizeof(uint16_t);
    }
    return(0);
}

// load big_map.sav session
int SpellSaveBigMap::Load(std::filesystem::path path, std::filesystem::path common_fs_path)
{
    m_path.clear();
    raw.clear();
    research.clear();
    upgrade.clear();
    units.clear();
    commanders.clear();
    bigmap.image.clear();
    bigmap.terr_hatch.clear();
    bigmap.terr_mask.clear();
    bigmap.pal.clear();
    bigmap.terr.clear();


    // try load common.fs    
    m_common_fs.reset();
    //m_common_fs = NULL;
    if(!common_fs_path.empty() && std::filesystem::exists(common_fs_path))
    {
        try{
            m_common_fs = std::make_unique<FSarchive>(common_fs_path.wstring());
        }catch(const std::runtime_error& error) {            
            return(1);
        }        
    }
    if(m_common_fs)
    {
        // parse rank strings
        m_rank_names.clear();
        auto rank_str = m_common_fs->GetFileStr("HODNOSTI.CZ");
        if(rank_str.empty())
            rank_str = m_common_fs->GetFileStr("HODNOSTI.ENG");
        if(rank_str.empty())
            return(1);
        auto lines = get_text_lines(rank_str);
        bool is_short = false;
        for(auto &line: lines)
        {
            auto toks = regexp_get(line, "^(?!;)(.+)");
            if(!toks.empty())
            {                
                if(is_short)
                    m_rank_names.push_back(char2wstringCP895(toks[0].c_str()));
                is_short ^= true;
            }
        }

        // parse unit names
        auto units_str = m_common_fs->GetFileStr("UNITS.CZ");
        if(units_str.empty())
            units_str = m_common_fs->GetFileStr("UNITS.ENG");
        if(units_str.empty())
            return(1);
        m_unit_names.clear();
        auto unit_names = get_text_lines(units_str);
        for(auto &unit: unit_names)
            m_unit_names.push_back(char2wstringCP895(unit.c_str()));

        // parse commander names
        auto c_names_str = m_common_fs->GetFileStr("C_NAMES.DEF");
        if(c_names_str.empty())
            return(1);
        m_commander_names.clear();
        auto c_names = get_text_lines(c_names_str);
        for(auto &name: c_names)
            m_commander_names.push_back(char2wstringCP895(name.c_str()));

    }        
    
    // load BIG_MAP
    std::vector<uint8_t> data;
    if(loaddata(path,data))
        return(1);
    m_path = path;

    // decompress
    LZWexpand lzw(100000);
    raw = lzw.Decode(data);
    if(raw.empty())
        return(1);

    //int min_size = 8 + 47*200 + 35*142 + 2*90 + 48*66 + 14*44;
    int min_size = 21165;
    uint8_t *ptr = raw.data();
    if(raw.size() < min_size)
        return(1);
    uint8_t *pend = ptr + raw.size();

    // research entries count (max 200)
    int res_count = *(uint32_t*)&ptr[0];
    ptr += 8;
    if(res_count > 200)
        return(1);

    // load research entries
    for(int k = 0; k < res_count; k++)
    {
        auto &res = research.emplace_back();
        res.raw.resize(47);
        memcpy(res.raw.data(),ptr,47);

        // read name, convert to unicode
        res.name = char2wstringCP895(trim_whites(get_save_str(&ptr[15],pend,32),true).c_str());
        
        // linked data id
        res.data_id = *(int16_t*)&ptr[7];

        // other stuff
        res.flags = (SpellSaveResearch::Flags)ptr[3];
        res.group = (SpellSaveResearch::Group)ptr[2];
        res.cost = *(int16_t*)&ptr[9];
        res.time = *(int16_t*)&ptr[5];
        res.level = ptr[4];        
        res.state = *(int16_t*)&ptr[0];
        res.available = 0;

        ptr += 47;
    }

    // load upgrade entries
    ptr = &raw[0x24C0];
    for(int k = 0; k < 36; k++)
    {                        
        // read name, convert to unicode
        auto name_len = strnlen((const char*)&ptr[0],30);
        if(!name_len)
            break;
        if(std::all_of(&ptr[30],&ptr[141],[](uint8_t i) { return i==0; }))
            break;
        if(ptr[0 + name_len])
            return(1);        

        auto& upg = upgrade.emplace_back();
        upg.raw.resize(142);
        memcpy(upg.raw.data(),ptr,142);

        upg.name = char2wstringCP895(trim_whites(get_save_str(&ptr[0],pend,30),true).c_str());

        upg.attack = *(int16_t*)&ptr[30];
        upg.attack_pt = *(int16_t*)&ptr[32];
        upg.defence = *(int16_t*)&ptr[34];
        upg.range = *(int16_t*)&ptr[36];
        upg.sight = *(int16_t*)&ptr[38]; // just guess
        upg.move = *(int16_t*)&ptr[40];

        upg.state = *(int16_t*)&ptr[44];
        upg.upg_price = *(int16_t*)&ptr[46];
        upg.upg_time = *(int16_t*)&ptr[48];
        
        // upgrade type class id
        upg.type = (SpellSaveUpgrade::UpgradeClass)*(int16_t*)&ptr[42];
        if(upg.GetUpgradeClassStr().empty())
            return(1);

        // list of suitable unit ids
        int types_count = *(int16_t*)&ptr[50];
        for(int m = 0; m < types_count; m++)
            upg.suitable_types.push_back(ptr[52 + m]);

        ptr += 142;
    }

    // unit available flags (this is independent of research flags for whatever reason)
    ptr = &raw[0x38B8];
    for(int k = 0; k < 90; k++)
    {
        int flag = *(int16_t*)ptr;
        for(auto &res: research)
        {
            if(res.group != SpellSaveResearch::Group::RACES)
                continue;
            if(res.flags != SpellSaveResearch::Flags::UnitType && res.flags != SpellSaveResearch::Flags::NewUnit)
                continue;
            if(res.data_id != k)
                continue;
            res.available = flag;
            break;
        }

        ptr += sizeof(int16_t);
    }

    // list of player units
    ptr = &raw[0x396C];
    for(int k = 0; k < 48; k++)
    {        
        auto& unit = units.emplace_back();
        unit.name = L"<empty slot>";
        unit.flags = ptr[38];        
        unit.upg_engine = -1;
        unit.upg_armor = -1;
        unit.upg_weapon = -1;

        unit.raw.resize(66);
        memcpy(unit.raw.data(), ptr, 66);

        // read name, convert to unicode
        auto name_len = strnlen((const char*)&ptr[0],30);        
        if(!name_len || ptr[0 + name_len] || !unit.flags)
        {
            ptr += 66;
            continue;
        }
        
        unit.name = char2wstringCP895((char*)&ptr[0]);

        unit.unit_type_id = ptr[30];
        unit.xp = *(int32_t*)&ptr[32];
        unit.xp_level = ptr[36];        
        unit.hp = ptr[42];
        unit.hp_max = ptr[43];
        unit.hierarch_pos = *(int8_t*)&ptr[46];
        unit.upg_engine = *(int8_t*)&ptr[48] - 1;
        unit.upg_armor = *(int8_t*)&ptr[50] - 1;
        unit.upg_weapon = *(int8_t*)&ptr[52] - 1;        
        unit.action_timeout = ptr[39];

        unit.upg_timeout = *(int8_t*)&ptr[44];
        unit.upg_unit_type = *(int8_t*)&ptr[62] - 1;
             

        ptr += 66;
    }

    // list of player commanders
    std::vector<std::wstring> used_com_names;
    ptr = &raw[0x45CC];
    for(int k = 0; k < 14; k++)
    {
        auto& com = commanders.emplace_back();
        com.name = L"<empty slot>";
        com.m_rank_names = &m_rank_names;
        com.valid = false;

        com.raw.resize(44);
        memcpy(com.raw.data(),ptr,44);

        com.rank = ptr[34];
        com.battles = *(int16_t*)&ptr[36];
        com.unit_id = *(int8_t*)&ptr[42];
        com.flags = *(uint16_t*)&ptr[40];
        com.x38 = ptr[38];

        // read name, convert to unicode
        auto name_len = strnlen((const char*)&ptr[0],30);
        if(name_len && !ptr[0 + name_len])
        {
            com.name = char2wstringCP895((char*)&ptr[0]);
            if(com.flags)
                used_com_names.push_back(com.name);
        }
        else if(com.flags & com.flags != 0xFFFF)
        {
            // bugged name - generate blank
            com.name = L"<>";
        }
        else
        {
            ptr += 44;
            continue;
        }
        
        com.valid = true;

        ptr += 44;
    }
    // assigned screwed up names
    for(auto &com: commanders)
    {
        if(com.is_empty())
            continue;
        if(com.name != L"<>")
            continue;
        for(auto &name: m_commander_names)
        {
            if(std::find(used_com_names.begin(), used_com_names.end(), name) != used_com_names.end())
                continue;
            // found not used name
            com.name = name;
            used_com_names.push_back(com.name);
        }
    }


    ptr = &raw[0x4834];
    
    // game level (should be 1 - 10)
    bigmap.level = ptr[0];    
    
    // final battle territory index
    bigmap.final_terr = ptr[3];

    // load bigmap territories
    std::vector<bool> used_territories(128,false);    
    if(m_common_fs && bigmap.level >= 1)
    {
        auto bigmap_img_name = string_format("LEVEL_%02d.LZ",bigmap.level);
        auto bm_lz = m_common_fs->GetFileData(bigmap_img_name.c_str());
        if(!bm_lz->size())
            return(1);
        bigmap.image = lzw.Decode(*bm_lz);

        auto bigmap_pal_name = string_format("LEVEL_%02d.PAL",bigmap.level);
        auto bm_pal = m_common_fs->GetFileData(bigmap_pal_name.c_str());
        if(bm_pal->size() != 64*3)
            return(1);
        bigmap.pal.assign(3*256,0);
        memcpy(bigmap.pal.data() + 128*3,bm_pal->data(),64*3);

        auto bigmap_clk_name = string_format("LEVEL_%02d.CLK",bigmap.level);
        auto clk_data = m_common_fs->GetFileData(bigmap_clk_name.c_str());
        if(clk_data->empty())
            return(1);

        // decode CLK territory mask
        DecodeCLK(*clk_data,bigmap.x_size,bigmap.y_size,bigmap.terr_mask);
        if(bigmap.terr_mask.size() != bigmap.image.size())
            return(1);

        // detect available territory masks
        auto clk_mask = bigmap.terr_mask;
        std::sort(clk_mask.begin(),clk_mask.end());
        auto un_end = std::unique(clk_mask.begin(),clk_mask.end());        
        bigmap.terr_count = 0;
        for(auto it = clk_mask.begin(); it < un_end; it++)
            if(*it > 0 && *it < 128)
            {
                used_territories[*it - 1] = true;
                bigmap.terr_count = *it;
            }

        // generate territory hatches
        bigmap.terr_hatch.assign(bigmap.x_size*bigmap.y_size,0);
        for(int col = 1; col <= bigmap.terr_count; col++)
        {
            for(int x = 0; x < 2*bigmap.x_size; x += 7)
            {
                for(int s = 0; s < 2; s++)
                {
                    int x1 = x + s;
                    int y1 = 0;
                    int x2 = x1 - (bigmap.y_size - 1);
                    int y2 = (bigmap.y_size - 1);
                    plot_line_mask(bigmap.terr_hatch.data(),bigmap.terr_hatch.data() + bigmap.x_size*bigmap.y_size,0,0,bigmap.x_size,col,x1,y1,x2,y2,bigmap.terr_mask.data(),col);
                }
            }
            for(int y = 0; y < bigmap.y_size; y++)
            {
                for(int dir = +1; dir >= -1; dir -= 2)
                {
                    int edge = 0;
                    for(int x = 0; x < bigmap.x_size; x++)
                    {
                        int pos = ((dir<0)?(bigmap.x_size-1):0) + x*dir + y*bigmap.x_size;
                        if(edge || bigmap.terr_mask[pos] == col)
                            edge++;
                        if(edge)
                            bigmap.terr_hatch[pos] = col;
                        if(edge > 1)
                            break;
                    }
                }
            }
            for(int x = 0; x < bigmap.x_size; x++)
            {
                for(int dir = +1; dir >= -1; dir -= 2)
                {
                    int edge = 0;
                    for(int y = 0; y < bigmap.y_size; y++)
                    {
                        int pos = ((dir<0)?((bigmap.y_size-1)*bigmap.x_size):0) + x + y*dir*bigmap.x_size;
                        if(edge || bigmap.terr_mask[pos] == col)
                            edge++;
                        if(edge)
                            bigmap.terr_hatch[pos] = col;
                        if(edge > 1)
                            break;
                    }
                }
            }
        }
    }

    // level music
    level.level_music = get_save_str(&raw[0x4838],pend,13);
    
    // counter attack normal units: AttackUnits()
    level.attack_units = get_save_array_i16(&raw[0x4845],pend);
    
    // counter attack special units: AttackSpecialUnits()
    level.attack_spec_units = get_save_array_i16(&raw[0x486F],pend);

    // counter attack flags: AttackFlags()
    level.attack_flags_non_spec = *(int16_t*)&raw[0x4899 + 0];
    level.attack_flags_total = *(int16_t*)&raw[0x4899 + 2];
    level.attack_flags_xp_level = *(int16_t*)&raw[0x4899 + 4];
    level.attack_flags_xp_level2 = *(int16_t*)&raw[0x4899 + 6];
    level.attack_flags_xp_f_attack_a = *(int16_t*)&raw[0x4899 + 8];
    level.attack_flags_xp_f_attack_b = *(int16_t*)&raw[0x4899 + 10];


    // round
    level.round = *(int16_t*)&raw[0x4835];
    // player money
    level.money = *(int32_t*)&raw[0x50F9];
    // money to research
    level.money_research = *(int32_t*)&raw[0x50FD];

    level.xp = *(int32_t*)&raw[0x50EF];
    level.rank = *(int8_t*)&raw[0x50ED];

    level.stat_kill_light_tot = *(int32_t*)&raw[0x5101];
    level.stat_kill_armor_tot = *(int32_t*)&raw[0x5105];
    level.stat_kill_air_tot = *(int32_t*)&raw[0x5109];

    level.stat_loss_light_tot = *(int32_t*)&raw[0x510D];
    level.stat_loss_armor_tot = *(int32_t*)&raw[0x5111];
    level.stat_loss_air_tot = *(int32_t*)&raw[0x5115];
    level.stat_loss_com_tot = *(int32_t*)&raw[0x5119];

    level.stat_kill_light = *(int32_t*)&raw[0x511D];
    level.stat_kill_armor = *(int32_t*)&raw[0x5121];
    level.stat_kill_air = *(int32_t*)&raw[0x5125];

    level.stat_loss_light = *(int32_t*)&raw[0x5129];
    level.stat_loss_armor = *(int32_t*)&raw[0x512D];
    level.stat_loss_air = *(int32_t*)&raw[0x5131];
    level.stat_loss_com = *(int32_t*)&raw[0x5135];
    
    ptr = &raw[0x48E3];
    for(int k = 0; k < bigmap.terr_count; k++)
    {
        auto& terr = bigmap.terr.emplace_back();
        terr.raw.resize(56);
        memcpy(terr.raw.data(),ptr,56);

        terr.valid = used_territories[k];
        terr.flags = ptr[0];

        auto name_len = strnlen((const char*)&ptr[1],13);
        terr.mus_name.resize(name_len);
        memcpy(terr.mus_name.data(),&ptr[1],name_len);

        name_len = strnlen((const char*)&ptr[14],13);
        terr.def_name.resize(name_len);
        memcpy(terr.def_name.data(),&ptr[14],name_len);

        name_len = strnlen((const char*)&ptr[27],13);
        terr.dta_name.resize(name_len);
        memcpy(terr.dta_name.data(),&ptr[27],name_len);

        terr.remain_money = *(int16_t*)&ptr[0x2C];
        terr.money_per_round = *(int16_t*)&ptr[0x2E];

        terr.remain_time = *(int32_t*)&ptr[0x28];

        terr.freq_random_attacks_a = *(int16_t*)&ptr[0x30];
        terr.freq_random_attacks_b = *(int16_t*)&ptr[0x32];

        terr.ssd_flags = *(uint32_t*)&ptr[0x34];

        // calculate center of each territory
        int x_mean = 0;
        int y_mean = 0;
        int c_mean = 0;
        for(int y = 0; y < bigmap.y_size; y++)
            for(int x = 0; x < bigmap.x_size; x++)
                if(bigmap.terr_mask[x + y*bigmap.x_size] == k + 1)
                {
                    x_mean += x;
                    y_mean += y;
                    c_mean++;
                }
        terr.x_center = x_mean / c_mean;
        terr.y_center = y_mean / c_mean;

        ptr += 56;
    }

    return(0);
}

// save big_map.sav session
int SpellSaveBigMap::Save(std::filesystem::path path)
{    
    // store research stuff
    if(research.size() > 200)
        return(1);    
    auto ptr = &raw[0x0000];
    auto pend = raw.data() + raw.size();
    // items count
    *(uint32_t*)&ptr[0] = research.size();
    ptr += 8;
    // clear records
    memset(ptr,0x00,200*47);

    for(auto &res: research)
    {
        // start with raw original data
        memcpy(ptr,res.raw.data(),res.raw.size());

        // put name
        memset(&ptr[15],0,32);
        if(res.name.length() > 31)
            return(1);
        auto name = wstring2stringCP895(res.name);
        memcpy(&ptr[15],name.c_str(),name.length());
        
        // linked data id
        *(int16_t*)&ptr[7] = res.data_id;

        // other stuff
        ptr[3] = res.flags;
        ptr[2] = res.group;
        *(int16_t*)&ptr[9] = res.cost;
        *(int16_t*)&ptr[5] = res.time;
        ptr[4] = res.level;
        *(int16_t*)&ptr[0] = res.state;

        ptr += 47;
    }

    // load upgrade entries
    ptr = &raw[0x24C0];
    for(auto &upg: upgrade)
    {
        // start with raw original data
        memcpy(ptr,upg.raw.data(),upg.raw.size());

        // put name
        memset(&ptr[0],0,30);
        if(upg.name.length() > 29)
            return(1);
        auto name = wstring2stringCP895(upg.name);
        memcpy(&ptr[0],name.c_str(),name.length());


        *(int16_t*)&ptr[30] = upg.attack;
        *(int16_t*)&ptr[32] = upg.attack_pt;
        *(int16_t*)&ptr[34] = upg.defence;
        *(int16_t*)&ptr[36] = upg.range;
        *(int16_t*)&ptr[38] = upg.sight; // just guess
        *(int16_t*)&ptr[40] = upg.move;

        *(int16_t*)&ptr[44] = upg.state;
        *(int16_t*)&ptr[46] = upg.upg_price;
        *(int16_t*)&ptr[48] = upg.upg_time;

        // upgrade type class id
        *(int16_t*)&ptr[42] = upg.type;

        // list of suitable unit ids
        if(upg.suitable_types.size() > 90)
            return(1);
        *(int16_t*)&ptr[50] = upg.suitable_types.size();
        for(int m = 0; m < upg.suitable_types.size(); m++)
        {
            if(upg.suitable_types[m] > 90 || upg.suitable_types[m] < 0)
                return(1);
            ptr[52 + m] = upg.suitable_types[m];
        }

        ptr += 142;
    }

    // unit available flags (this is independent of research flags for whatever reason)
    ptr = &raw[0x38B8];
    for(int k = 0; k < 90; k++)
    {        
        for(auto& res: research)
        {
            if(res.group != SpellSaveResearch::Group::RACES)
                continue;
            if(res.flags != SpellSaveResearch::Flags::UnitType && res.flags != SpellSaveResearch::Flags::NewUnit)
                continue;
            if(res.data_id != k)
                continue;
            *(int16_t*)ptr = res.available;
            break;
        }
        ptr += sizeof(int16_t);
    }
    
    
    // list of player units
    ptr = &raw[0x396C];
    for(auto &unit: units)
    {
        if(unit.is_empty())
        {
            memset(ptr, 0, 66);
            ptr += 66;
            continue;
        }
        
        // start with raw original data
        memcpy(ptr,unit.raw.data(),unit.raw.size());

        // put name
        memset(ptr,0,30);
        if(unit.name.length() > 29)
            return(1);
        auto name = wstring2stringCP895(unit.name);
        memcpy(ptr,name.c_str(),name.length());
        
        ptr[38] = unit.flags;                
        ptr[30] = unit.unit_type_id;
        *(int32_t*)&ptr[32] = unit.xp;
        ptr[36] = unit.xp_level;
        ptr[42] = unit.hp;
        ptr[43] = unit.hp_max;
        *(int8_t*)&ptr[46] = unit.hierarch_pos;
        *(int8_t*)&ptr[48] = unit.upg_engine + 1;
        *(int8_t*)&ptr[50] = unit.upg_armor + 1;
        *(int8_t*)&ptr[52] = unit.upg_weapon + 1;
        ptr[39] = unit.action_timeout;
        *(int8_t*)&ptr[44] = unit.upg_timeout;
        *(int8_t*)&ptr[62] = unit.upg_unit_type + 1;

        ptr += 66;
    }

    // list of player commanders
    ptr = &raw[0x45CC];
    for(auto &com: commanders)
    {
        if(com.is_empty())
        {
            memset(ptr, 0, 44);
            *(int8_t*)&ptr[42] = -1;
            ptr += 44;
            continue;
        }
        
        // start with raw original data
        memcpy(ptr,com.raw.data(),com.raw.size());
        
        // put name
        memset(ptr,0,30);
        if(com.name.length() > 29)
            return(1);
        auto name = wstring2stringCP895(com.name);
        memcpy(ptr,name.c_str(),name.length());
        
        ptr[34] = com.rank;
        *(int16_t*)&ptr[36] = com.battles;
        *(int8_t*)&ptr[42] = com.unit_id;
        *(uint16_t*)&ptr[40] = com.flags;
        ptr[38] = com.x38;

        ptr += 44;
    }

    // store territories
    ptr = &raw[0x48E3];
    for(int k = 0; k < bigmap.terr_count; k++)
    {
        auto& terr = bigmap.terr[k];

        // start with raw original data
        memcpy(ptr,terr.raw.data(),terr.raw.size());
                
        ptr[0] = terr.flags;

        // put music name
        memset(&ptr[1],0,13);
        if(terr.mus_name.length() > 13)
            return(1);
        memcpy(&ptr[1],terr.mus_name.c_str(),terr.mus_name.length());

        // put def name
        memset(&ptr[14],0,13);
        if(terr.def_name.length() > 13)
            return(1);
        memcpy(&ptr[14],terr.def_name.c_str(),terr.def_name.length());

        // put dta name
        memset(&ptr[27],0,13);
        if(terr.dta_name.length() > 13)
            return(1);
        memcpy(&ptr[27],terr.dta_name.c_str(),terr.dta_name.length());           

        *(int16_t*)&ptr[0x2C] = terr.remain_money;
        *(int16_t*)&ptr[0x2E] = terr.money_per_round;

        *(int32_t*)&ptr[0x28] = terr.remain_time;

        *(int16_t*)&ptr[0x30] = terr.freq_random_attacks_a;
        *(int16_t*)&ptr[0x32] = terr.freq_random_attacks_b;

        *(uint32_t*)&ptr[0x34] = terr.ssd_flags;

        ptr += 56;
    }


    // game level (should be 1 - 10)
    raw[0x4834] = bigmap.level;
    // last territory index?
    raw[0x4837] = bigmap.final_terr;


    // level music
    store_save_str(&raw[0x4838],pend,13,level.level_music);
    
    // counter attack normal units: AttackUnits()
    store_save_array_i16(&raw[0x4845],pend,level.attack_units,20);

    // counter attack special units: AttackSpecialUnits()
    store_save_array_i16(&raw[0x486F],pend,level.attack_spec_units,20);

    // counter attack flags: AttackFlags()
    *(int16_t*)&raw[0x4899 + 0] = level.attack_flags_non_spec;
    *(int16_t*)&raw[0x4899 + 2] = level.attack_flags_total;
    *(int16_t*)&raw[0x4899 + 4] = level.attack_flags_xp_level;
    *(int16_t*)&raw[0x4899 + 6] = level.attack_flags_xp_level2;
    *(int16_t*)&raw[0x4899 + 8] = level.attack_flags_xp_f_attack_a;
    *(int16_t*)&raw[0x4899 + 10] = level.attack_flags_xp_f_attack_b;

    // level stuff
    *(int16_t*)&raw[0x4835] = level.round;
    *(int32_t*)&raw[0x50F9] = level.money;
    *(int32_t*)&raw[0x50FD] = level.money_research;
    *(int32_t*)&raw[0x50EF] = level.xp;
    *(int8_t*)&raw[0x50ED] = level.rank;
    *(int32_t*)&raw[0x5101] = level.stat_kill_light_tot;
    *(int32_t*)&raw[0x5105] = level.stat_kill_armor_tot;
    *(int32_t*)&raw[0x5109] = level.stat_kill_air_tot;
    *(int32_t*)&raw[0x510D] = level.stat_loss_light_tot;
    *(int32_t*)&raw[0x5111] = level.stat_loss_armor_tot;
    *(int32_t*)&raw[0x5115] = level.stat_loss_air_tot;
    *(int32_t*)&raw[0x5119] = level.stat_loss_com_tot;
    *(int32_t*)&raw[0x511D] = level.stat_kill_light;
    *(int32_t*)&raw[0x5121] = level.stat_kill_armor;
    *(int32_t*)&raw[0x5125] = level.stat_kill_air;
    *(int32_t*)&raw[0x5129] = level.stat_loss_light;
    *(int32_t*)&raw[0x512D] = level.stat_loss_armor;
    *(int32_t*)&raw[0x5131] = level.stat_loss_air;
    *(int32_t*)&raw[0x5135] = level.stat_loss_com;


    // try encode
    std::vector<uint8_t> sav;
    try{
        LZspell lzw(raw.data(),raw.size(),sav);
    }catch(const std::runtime_error& error) {
        return(1);
    }
    
    // use original path?
    if(path.empty())
        path = m_path;

    // try save
    if(savedata(path,sav))
        return(1);

    return(0);
}


// sort units list
bool ComparePairs(std::pair<int,std::wstring>& a,std::pair<int,std::wstring>& b)
{
    return(_wcsicmp(a.second.c_str(),b.second.c_str()) < 0);
}
int SpellSaveBigMap::SortUnits(bool remove_gaps,bool separate,bool by_types,bool by_names)
{
    if(remove_gaps)
    {
        // remove empty slots
        auto temp = units;
        units.clear();
        std::vector<bool> used_com(commanders.size(),false);
        for(auto &unit: temp)
            if(!unit.is_empty())
            {
                units.push_back(unit);
                int old_id = &unit - temp.data();
                for(auto &com: commanders)
                    if(!used_com[&com - commanders.data()] && com.unit_id == old_id)
                    {
                        com.unit_id = units.size() - 1;
                        used_com[&com - commanders.data()] = true;
                    }
            }
        for(auto& unit: temp)
            if(unit.is_empty())
                units.push_back(unit);
    }

    if(separate)
    {
        // separete permanent units from reinforcements
        auto temp = units;
        units.clear();
        std::vector<bool> used_com(commanders.size(),false);
        for(auto& unit: temp)
            if(unit.is_permanent())
            {
                units.push_back(unit);
                int old_id = &unit - temp.data();
                for(auto& com: commanders)
                    if(!used_com[&com - commanders.data()] && com.unit_id == old_id)
                    {
                        com.unit_id = units.size() - 1;
                        used_com[&com - commanders.data()] = true;
                    }
            }
        for(auto& unit: temp)
            if(unit.is_reinforce())
                units.push_back(unit);
        for(auto& unit: temp)
            if(unit.is_empty())
                units.push_back(unit);
    }

    if(by_types)
    {
        // sort by unit types
        auto temp = units;        
        units.clear();

        std::vector<bool> used(temp.size(), false);
        std::vector<bool> used_com(commanders.size(),false);
        for(int unit_type = 0; unit_type < 90; unit_type++)
        {
            for(int uid = 0; uid < temp.size(); uid++)
            {
                auto &unit = temp[uid];
                if(!unit.is_empty() && !used[uid] && unit.unit_type_id == unit_type)
                {
                    units.push_back(unit);
                    used[uid] = true;
                    for(auto& com: commanders)
                        if(!used_com[&com - commanders.data()] && com.unit_id == uid)
                        {
                            com.unit_id = units.size() - 1;
                            used_com[&com - commanders.data()] = true;
                        }
                }
            }
        }
        for(auto& unit: temp)
            if(unit.is_empty())
                units.push_back(unit);
    }

    if(by_names)
    {
        // sort by unit names
        
        // sort to get order indices first
        std::vector<std::pair<int,std::wstring>> list;
        for(int uid = 0; uid < units.size(); uid++)
            if(!units[uid].is_empty())
                list.push_back({uid, units[uid].name});
        std::sort(list.begin(), list.end(), ComparePairs);

        // reorder
        auto temp = units;
        units.clear();
        std::vector<bool> used_com(commanders.size(),false);
        for(int uid = 0; uid < list.size(); uid++)
        {            
            auto& unit = temp[list[uid].first];
            units.push_back(unit);            
            for(auto& com: commanders)
                if(!used_com[&com - commanders.data()] && com.unit_id == list[uid].first)
                {
                    com.unit_id = units.size() - 1;
                    used_com[&com - commanders.data()] = true;
                }
        }                      
        for(auto& unit: temp)
            if(unit.is_empty())
                units.push_back(unit);
    }

    return(0);
}
int SpellSaveBigMap::SwapUnits(int id_a, int id_b)
{
    if(id_a < 0 || id_a >= units.size() || id_b < 0 || id_b >= units.size())
        return(1);

    if(!units[id_a].is_empty())
    {
        for(auto& com: commanders)
            if(com.unit_id == id_a)
                com.unit_id = 1000 + id_b;
    }
    if(!units[id_b].is_empty())
    {
        for(auto& com: commanders)
            if(com.unit_id == id_b)
                com.unit_id = 1000 + id_a;
    }
    std::swap(units[id_a],units[id_b]);
    for(auto& com: commanders)
        if(com.unit_id >= 1000)
            com.unit_id -= 1000;
    return(0);
}
// heal all units
int SpellSaveBigMap::HealUnits()
{
    for(auto &unit: units)
        unit.hp = unit.hp_max;
    return(0);
}


// synchronize upgrade items
int SpellSaveBigMap::SyncUpgrades()
{
    if(!m_common_fs)
        return(1);

    // get def file
    auto defstr = m_common_fs->GetFile("UPGRADES.DEF");
    if(defstr.empty())
        return(1);

    // get research names
    auto namestr = m_common_fs->GetFile("UPGRADES.CZ");
    if(namestr.empty())
        namestr = m_common_fs->GetFile("UPGRADES.ENG");
    if(namestr.empty())
        return(1);
    auto names = get_text_lines(namestr,true);
    for(auto& name: names)
        name = trim_whites(name,true);

    // parse it to local list
    std::vector<SpellSaveUpgrade> common_upg;
    SpellDEF def(defstr);
    for(int item_id = 0; item_id < 36; item_id++)
    {
        auto label = string_format("Upgrade(%d)",item_id);
        std::unique_ptr<SpellDefSection> section(def.GetSection(label));
        if(!section)
            break;
        auto params = section->GetData();

        SpellSaveUpgrade upg;
        for(auto& par: params)
        {
            if(par->parameters.empty())
                return(1);
            if(par->name == "Move")
            {
                int val;
                if(str2int(par->parameters[0],val))
                    return(1);
                upg.move = val;
            }
            else if(par->name == "Attack")
            {
                int val;
                if(str2int(par->parameters[0],val))
                    return(1);
                upg.attack = val;
            }
            else if(par->name == "AttackPT")
            {
                int val;
                if(str2int(par->parameters[0],val,0))
                    return(1);
                upg.attack_pt = val;
            }
            else if(par->name == "Sight")
            {
                int val;
                if(str2int(par->parameters[0],val))
                    return(1);
                upg.sight = val;
            }
            else if(par->name == "Defence")
            {
                int val;
                if(str2int(par->parameters[0],val))
                    return(1);
                upg.defence = val;
            }
            else if(par->name == "Range")
            {
                int val;
                if(str2int(par->parameters[0],val))
                    return(1);
                upg.range = val;
            }
            else if(par->name == "UpgradeTime")
            {
                int val;
                if(str2int(par->parameters[0],val,0))
                    return(1);
                upg.upg_time = val;
            }
            else if(par->name == "UpgradePrice")
            {
                int val;
                if(str2int(par->parameters[0],val,0))
                    return(1);
                upg.upg_price = val;
            }
            else if(par->name == "Flags")
            {                
                if(upg.SetFlags(par->parameters[0]))
                    return(1);
            }            
            else if(par->name == "SuitableTypes")
            {
                for(auto& pp: par->parameters)
                {
                    int val;
                    if(str2int(pp,val,0,89))
                        return(1);
                    upg.suitable_types.push_back(val);
                }
            }
        }

        // assign name
        if(item_id >= names.size())
            return(1);
        upg.name = char2wstringCP895(names[item_id].c_str());

        // generate empty raw record
        upg.raw.assign(142,0);

        common_upg.push_back(upg);
    }
    
    // copy states of existing upgrade to the common.fs upgrade list
    for(auto& upg: upgrade)
    {
        for(auto& cupg: common_upg)
            if(upg.name == cupg.name)
            {
                cupg.raw = upg.raw;
                cupg.state = upg.state;
                break;
            }
    }

    // reling unit upgrades to new upgrade items
    auto com_units = units;
    for(int k = 0; k < units.size(); k++)
    {
        auto& unit = units[k];
        auto& com_unit = com_units[k];
        
        if(com_unit.upg_armor >= 0)
        {
            auto name = upgrade[com_unit.upg_armor].name;
            unit.upg_armor = -1;
            for(auto &upg: common_upg)
                if(upg.name == name)
                    unit.upg_armor = &upg - common_upg.data();
        }
        if(com_unit.upg_engine >= 0)
        {
            auto name = upgrade[com_unit.upg_engine].name;
            unit.upg_engine = -1;
            for(auto& upg: common_upg)
                if(upg.name == name)
                    unit.upg_engine = &upg - common_upg.data();
        }
        if(com_unit.upg_weapon >= 0)
        {
            auto name = upgrade[com_unit.upg_weapon].name;
            unit.upg_weapon = -1;
            for(auto& upg: common_upg)
                if(upg.name == name)
                    unit.upg_weapon = &upg - common_upg.data();
        }
    }

    // reling research items to new upgrade list
    for(auto &res: research)
    {
        if(res.group != SpellSaveResearch::Group::UPGRADE)
            continue;
        if(res.data_id < 0 || res.data_id >= upgrade.size())
            return(1);
        auto name = upgrade[res.data_id].name;
        for(auto& upg: common_upg)
            if(upg.name == name)
                res.data_id = &upg - common_upg.data();
    }
    
    // override upgrades research
    upgrade = common_upg;

    return(0);
}


// synchronize research items
int SpellSaveBigMap::SyncResearch()
{
    if(!m_common_fs)
        return(1);
    
    // get def file
    auto defstr = m_common_fs->GetFile("RESEARCH.DEF");
    if(defstr.empty())
        return(1);

    // get research names
    auto namestr = m_common_fs->GetFile("RESEARCH.CZ");
    if(namestr.empty())
        namestr = m_common_fs->GetFile("RESEARCH.ENG");
    if(namestr.empty())
        return(1);
    auto names = get_text_lines(namestr,true);
    for(auto &name: names)
        name = trim_whites(name,true);

    // parse it to local list
    std::vector<SpellSaveResearch> common_res;
    SpellDEF def(defstr);
    for(int item_id = 0; item_id < 200; item_id++)
    {
        auto label = string_format("Item(%d)",item_id);
        std::unique_ptr<SpellDefSection> section(def.GetSection(label));
        if(!section)
            break;
        auto params = section->GetData();

        SpellSaveResearch res;
        for(auto &par: params)
        {
            if(par->parameters.empty())
                return(1);
            if(par->name == "Flags")
            {
                if(res.SetFlags(par->parameters[0]))
                    return(1);
            }
            else if(par->name == "Group")
            {
                if(res.SetGroup(par->parameters[0]))
                    return(1);
            }
            else if(par->name == "Data")
            {
                int val;
                if(str2int(par->parameters[0],val,0))
                    return(1);
                res.data_id = val;
            }
            else if(par->name == "Time")
            {
                int val;
                if(str2int(par->parameters[0],val,0))
                    return(1);
                res.time = val;
            }
            else if(par->name == "Level")
            {
                int val;
                if(str2int(par->parameters[0],val,0,10))
                    return(1);
                res.level = val;
            }
            else if(par->name == "UpgradePrice")
            {
                int val;
                if(str2int(par->parameters[0],val,0))
                    return(1);
                res.cost = val;
            }
            else if(par->name == "ORconnections")
            {
                for(auto &pp: par->parameters)
                {
                    int val;
                    if(str2int(pp,val,0,199))
                        return(1);
                    if(val == item_id)
                        return(1);
                    res.or_connections.push_back(val);
                }                
            }            
        }

        // assign name
        if(item_id >= names.size())
            return(1);
        res.name = char2wstringCP895(names[item_id].c_str());

        // generate empty raw record
        res.raw.assign(47,0);

        common_res.push_back(res);
    }

    // copy states of existing research to the common.fs research list
    for(auto &res: research)
    {
        for(auto &cres: common_res)
            if(res.name == cres.name)
            {
                cres.raw = res.raw;
                cres.state = res.state;
                cres.available = res.available;
                break;
            }
    }
    
    // override save research
    research = common_res;

    return(0);
}

// synchronize level setup with current common.fs level DEF file
int SpellSaveBigMap::SyncLevel()
{
    if(!m_common_fs)
        return(1);

    // try get current level def file
    auto def_name = string_format("LEVEL_%02d.DEF",bigmap.level);
    auto defstr = m_common_fs->GetFile(def_name);
    if(defstr.empty())
        return(1);
    
    // parse commands
    SpellDEF def(defstr);
    std::unique_ptr<SpellDefSection> section(def.GetSection("LevelInit"));
    if(!section)
        return(1);
    for(auto &cmd: section->GetData())
    {
        if(cmd->name == "LevelMusic")
        {
            // level music name
            if(cmd->parameters.size() != 1)
                return(1);
            level.level_music = cmd->parameters[0];
        }
        else if(cmd->name == "AttackUnits")
        {
            // normal attack units
            std::vector<int> list;
            if(str2int(cmd->parameters, list, 0, m_unit_names.size() - 1))
                return(1);
            level.attack_units = list;
        }
        else if(cmd->name == "AttackSpecialUnits")
        {
            // special attack units
            std::vector<int> list;
            if(str2int(cmd->parameters,list,0,m_unit_names.size() - 1))
                return(1);
            level.attack_spec_units = list;
        }
        else if(cmd->name == "AttackFlags")
        {
            // attack flags
            if(cmd->parameters.size() != 6)
                return(1);
            std::vector<int> list;
            if(str2int(cmd->parameters,list,0))
                return(1);
            if(list[1] > 50)
                return(1);
            if(list[0] > list[1])
                return(1);
            if(list[2] > 12)
                return(1);
            if(list[3] > 12)
                return(1);
            level.attack_flags_non_spec = list[0];
            level.attack_flags_total = list[1];
            level.attack_flags_xp_level = list[2];
            level.attack_flags_xp_level2 = list[3];
            level.attack_flags_xp_f_attack_b = list[4];
            level.attack_flags_xp_f_attack_a = list[5];
        }
        else if(cmd->name == "End")
        {
            // end level
            int val;
            if(str2int(cmd->parameters[0],val,0))
                return(1);
            bigmap.final_terr = val;
        }
    }

    return(0);
}


// decodder of CLK territory files
//   note: from https://github.com/luboshorak/spellcross_restoration_tools/blob/main/spellcross-map-edit-main/source/forms/form_level.cpp
bool DecodeCLK(const std::vector<uint8_t>& clkBytes,int& outW,int& outH,std::vector<uint8_t>& values)
{
    outW = 0;
    outH = 0;
    values.clear();

    if(clkBytes.size() < 4)
        return false;

    auto rd16 = [&](size_t off) -> unsigned {
        if(off + 1 >= clkBytes.size()) return 0;
        return (unsigned)clkBytes[off] | ((unsigned)clkBytes[off + 1] << 8);
    };

    // NOTE: format observed in python tool: uint16 H, uint16 W
    const unsigned H = rd16(0);
    const unsigned W = rd16(2);
    if(W == 0 || H == 0)
        return false;

    const size_t offsets_off = 4;
    const size_t offsets_size = (size_t)H * 2;
    if(offsets_off + offsets_size > clkBytes.size())
        return false;

    std::vector<unsigned> offsets;
    offsets.reserve(H);
    for(unsigned y = 0; y < H; ++y)
        offsets.push_back(rd16(offsets_off + (size_t)y * 2));

    values.assign((size_t)W * H,0);

    for(unsigned y = 0; y < H; ++y)
    {
        const unsigned start = offsets[y];
        const unsigned end = (y + 1 < H) ? offsets[y + 1] : (unsigned)clkBytes.size();
        if(start >= clkBytes.size() || end > clkBytes.size() || end <= start)
            continue;

        size_t x = 0;
        for(unsigned i = start; i + 1 < end && x < W; i += 2)
        {
            const unsigned run_len = clkBytes[i];
            const unsigned val = clkBytes[i + 1];
            if(run_len == 0)
                continue;
            const size_t x2 = std::min((size_t)W,x + (size_t)run_len);
            std::fill(values.begin() + (size_t)y * W + x,values.begin() + (size_t)y * W + x2,(unsigned char)val);
            x = x2;
        }
    }

    outW = (int)W;
    outH = (int)H;

    return true;
}