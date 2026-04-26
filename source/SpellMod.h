//=============================================================================
// Spellcross Mod Launcher
// ----------------------------------------------------------------------------
// Builder of game archives.
// 
// This code is part of Spellcross Mod Launcher project.
// (c) 2026, Stanislav Maslan, s.maslan@seznam.cz
// url: https://github.com/smaslan/Spellcross-Mod-Launcher
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================
#pragma once

#include <filesystem>
#include <string>
#include <functional>

class SpellModCmd
{
public:
    int m_line;
    std::string m_cmd;
    std::string m_raw;

    int isAssign(std::string &left,std::string &right);
    int isFunction(std::string& func_name,std::vector<std::string>& func_params);
};

typedef std::vector<SpellModCmd> SpellModCmdList;

class SpellModPath
{
public:
    std::string name;
    std::filesystem::path path;
    std::filesystem::path alt_path;
    bool isValid();
};


class FSarchive;
class FSUarchive;
class SpellArchive
{
private:
    std::string m_last_error;
public:
    std::filesystem::path m_path;
    FSarchive *m_fs;
    FSUarchive* m_fsu;

    enum Type
    {
        AUTO,
        FS,
        FSU
    };
    
    SpellArchive();
    SpellArchive(SpellModPath& path,Type explicit_archive_type=Type::AUTO);
    int Load(SpellModPath& path,Type explicit_archive_type=Type::AUTO);
    int Load(std::filesystem::path path,Type explicit_archive_type=Type::AUTO);
    ~SpellArchive();
    bool isFolder();
    std::vector<std::string> GetItemNames();
    int GetFile(std::string &name, std::vector<uint8_t> &data);
    std::vector<uint8_t>* GetFile(std::string& name);
    int AddFile(SpellArchive &src, std::string &name, bool allow_replace);
    bool Compare(SpellArchive& ref);
    int Save(std::filesystem::path path, bool allow_overwrite=false);
};

class SpellMod
{
public:

    class Config
    {
    public:
        std::filesystem::path mod_path;
        std::filesystem::path spell_dir;
        std::filesystem::path spellcd_dir;
        std::filesystem::path state_ini_path;
        bool allow_cd_mod;
        bool move_saves;
    };

    class ModArchivesList
    {
    public:
        std::string name;
        std::filesystem::path source;
        std::filesystem::path dest;
        std::filesystem::path dest_dir;
    };


    SpellMod();
    ~SpellMod();

    void Clear();
    void SetConsoleOutCb(std::function<void(std::string)> status_cb);

    int BuildMod(Config& config,bool allow_restore = true);
    int CheckRestoreMod(Config& config,bool& something_to_restore);
    int RestoreMod(Config& config);
    int SwapMod(Config& config, bool allow_restore = true);
    int CleanupMod(Config& config);

    class SaveInfo {
    public:
        std::string description;
        std::string time;
    };
    static int GetSaveIni(std::filesystem::path save_dir,SaveInfo& info);
    static int MakeSaveIni(std::filesystem::path save_dir,std::string description);
private:
    std::vector<SpellModPath> m_paths;
    std::vector<SpellArchive*> m_sources;
    std::function<void(std::string)> m_stdout_cb;
    std::string m_def;

    //void PrintConsole(const std::string fmt,...);
    template<typename... Args> void PrintConsole(const std::string fmt,Args... args);

    int LoadDEF(Config& config);
    
    SpellModPath* GetPath(std::string name);
    SpellModPath ParsePath(std::filesystem::path path,std::filesystem::path alt_path="",std::string name="");
    SpellModPath* AddPath(std::string name,std::filesystem::path path,std::filesystem::path alt_path="",bool overwrite=false);

    int GetClass(std::string def,std::string class_name,SpellModCmdList& commands);     
    SpellArchive *GetArchive(SpellModPath &path);
    SpellArchive *LoadArchive(SpellModPath &path,SpellArchive::Type arch_type=SpellArchive::Type::AUTO);

    

};

