// Spellcross MOD class
#include "SpellMod.h"

#include <regex>
#include <stdexcept>
#include <filesystem>
#include <sys/stat.h>

#include "other.h"
#include "fs_archive.h"
#include "fsu_archive.h"
#include "SimpleIni.h"

// check if command is asignement "left=right", return left, right
int SpellModCmd::isAssign(std::string &left, std::string &right)
{
    left.clear();
    right.clear();
    auto tokens = regexp_get(m_cmd,"(.+?)\s*=\s*?(.*)");
    if(tokens.size() != 2)
        return(0);
    left = tokens[0];
    right = tokens[1];
    return(1);
}

// check if command is function, separete name an parameters list
int SpellModCmd::isFunction(std::string& func_name, std::vector<std::string> &func_params)
{
    func_params.clear();
    func_name.clear();
    auto tokens = regexp_get(m_cmd,"(.+?)\\((.*?)\\)");
    if(tokens.size() != 2)
        return(0);
    func_name = tokens[0];
    func_params = get_text_lines(tokens[1],true,',');
    return(1);
}

// is path valid?
bool SpellModPath::isValid()
{
    return(!path.empty() || !alt_path.empty());
}


// make empty archive
SpellArchive::SpellArchive()
{
    m_fs = NULL;
    m_fsu = NULL;
    m_path = "";
}

// load data archive
SpellArchive::SpellArchive(SpellModPath &path,Type explicit_archive_type)
{
    m_fs = NULL;
    m_fsu = NULL;
    m_path = "";    
    if(Load(path,explicit_archive_type))
        throw std::runtime_error(m_last_error);        
}

// load archive from path (must be empty before)
int SpellArchive::Load(std::filesystem::path path, Type explicit_archive_type)
{
    m_last_error = "";
    if(m_fs)
        return(1);
    
    if(!path.has_extension())
    {
        // source if folder: make archive from folder data:
        if(explicit_archive_type == Type::FSU)
        {        
            // FSU archive:
            m_fsu = new FSUarchive();            
            if(m_fsu->LoadFolder(path))
            {
                m_last_error = m_fsu->m_last_error;
                return(1);
            }
            m_path = path;        
        }
        else
        {
            // FS archive:
            m_fs = new FSarchive();
            if(m_fs->LoadFolder(path))
            {
                m_last_error = string_format("Loading folder (%s) into FS archive failed!",path.string().c_str());
                return(1);
            }
            m_path = path;
        }

    }    
    else if(iequals(path.extension().string(),".FS"))
    {
        // try load FS archive data
        try {
            m_fs = new FSarchive(path);
            m_path = path;
        }
        catch(const std::runtime_error& error) {
            m_last_error = string_format("Loading FS archive failed (%s)!",error.what());
            return(1);
        }
    }
    else if(iequals(path.extension().string(),".FSU"))
    {
        // try load FSU archive data
        try {
            m_fsu = new FSUarchive(path.wstring(), FSUarchive::Options::NO_DECODE);
            m_path = path;
        }catch(const std::runtime_error& error) {
            m_last_error = string_format("Loading FSU archive (%ls) failed (%s)!",path.wstring().c_str(),error.what());
            return(1);
        }
    }
    return(0);
}

// load archive from path (must be empty before)
int SpellArchive::Load(SpellModPath& path,Type explicit_archive_type)
{
    m_last_error = "";
    if(m_fs)
        return(1);
    
    std::vector<std::filesystem::path> path_list = {path.path, path.alt_path};
    for(auto& path: path_list)
    {
        if(path.empty())
        {
            m_last_error = string_format("Loading archive failed (%s)!",path.string().c_str());
            return(1);
        }
        if(!Load(path,explicit_archive_type))
            return(0);
    }
    return(1);
}

// save archive to path
int SpellArchive::Save(std::filesystem::path path,bool allow_overwrite)
{
    if(m_fs)
    {
        // FS archive
        return(m_fs->SaveFile(path, allow_overwrite));
    }
    else if(m_fsu)
    {
        // FSU archive
        return(m_fsu->Save(path,allow_overwrite));
    }
    else
        return(1);
}

SpellArchive::~SpellArchive()
{
    if(m_fs)
        delete m_fs;
    m_fs = NULL;
    if(m_fsu)
        delete m_fsu;
    m_fsu = NULL;
}

// is archive folder?
bool SpellArchive::isFolder()
{
    return(!m_path.has_extension());
}

// get list of items in archive
std::vector<std::string> SpellArchive::GetItemNames()
{
    if(m_fs)
        return(m_fs->GetFileNames());
    else if(m_fsu)
        return(m_fsu->GetResourceNanes());
    std::vector<std::string> list;
    return(list);
}

// try get file from archive
int SpellArchive::GetFile(std::string& name,std::vector<uint8_t>& data)
{
    data.clear();
    if(!m_fs)
        return(1);
    return(m_fs->GetFile(name.c_str(), data));
}

// try get file from archive
std::vector<uint8_t> *SpellArchive::GetFile(std::string& name)
{
    if(!m_fs)
        return(NULL);
    return(m_fs->GetFileData(name.c_str()));
}

// add file from another archive
int SpellArchive::AddFile(SpellArchive &src, std::string& name, bool allow_replace)
{   
    if(src.m_fs && !m_fs)
    {
        // first file to add: make blank FS
        m_fs = new FSarchive();
    }
    if(src.m_fsu && !m_fsu)
    {
        // first file to add: make blank FSU
        m_fsu = new FSUarchive();
    }

    if(m_fs)
    {    
        // FS archive:
        if(!src.m_fs)
            return(1);
        auto data = src.m_fs->GetFileData(name.c_str());
        if(!data)
            return(1);
        return(m_fs->AddFile(name,*data,allow_replace));
    }
    if(m_fsu)
    {
        // FSU archive:
        if(!src.m_fsu)
            return(1);
        auto data = src.m_fsu->GetResource(name.c_str());
        if(!data)
            return(1);
        return(m_fsu->AddResource(data));
    }
    return(1);
}

// compare two archive by content
bool SpellArchive::Compare(SpellArchive& ref)
{
    if(m_fs)
    {
        if(!ref.m_fs)
            return(false);
        return(m_fs->CompareArchives(ref.m_fs));
    }
    else if(m_fsu)
    {
        if(!ref.m_fsu)
            return(false);
        return(m_fsu->Compare(ref.m_fsu));
    }
    else
        return(false);
}


SpellMod::SpellMod()
{    
    m_stdout_cb = NULL;    
    Clear();
};

SpellMod::~SpellMod()
{
    for(auto &item: m_sources)
        delete item;
    m_sources.clear();
    m_paths.clear();
    m_def.clear();
};

// set console output function
void SpellMod::SetConsoleOutCb(std::function<void(std::string)> status_cb)
{
    m_stdout_cb = status_cb;
}

// write stuff to console
template<typename... Args> void SpellMod::PrintConsole(const std::string fmt, Args... args)
{
    if(m_stdout_cb)
        m_stdout_cb(string_format(fmt,args...));
}

int SpellMod::GetClass(std::string def, std::string class_name,SpellModCmdList &commands)
{
    commands.clear();

    auto lines = get_text_lines(def);
    int start_line = -1;
    int end_line = -1;
    for(int cmd_line = 0; cmd_line < lines.size(); cmd_line++)
    {
        auto tokens = regexp_get(lines[cmd_line], "^\\s*(class\\s+" + class_name + ")\\s*\\{?");
        if(tokens.empty())
            continue;
        start_line = cmd_line;
        break;
    }
    if(start_line < 0)
        return(1);
    for(int cmd_line = start_line; cmd_line < lines.size(); cmd_line++)
    {
        auto tokens = regexp_get(lines[cmd_line],"\\s*^(?!//)(\\})");
        if(tokens.empty())
            continue;
        end_line = cmd_line;
        break;
    }
    if(end_line < 0)
        return(1);

    // extract class section
    /*std::regex secexp("([\\s\\S]*?^\\s*class\\s+" + class_name + "[\\s\\S]*?\\{)([\\s\\S]*?)^\\s*?\\}");
    std::smatch match;
    std::regex_search(def,match,secexp);
    if(match.size() != 3)
        return(1);

    // count lines before class section
    auto prefix = match[1].str();
    int cmd_line = std::count(prefix.begin(), prefix.end(), '\n');

    // parse class content
    auto lines = get_text_lines(match[2]);*/

    for(int cmd_line = start_line; cmd_line < end_line; cmd_line++)
    {
        auto cmd = regexp_get(lines[cmd_line], "\\s*^(?!//)(.*?);");
        if(cmd.empty())
            continue;
        commands.emplace_back();
        commands.back().m_cmd = cmd[0];
        commands.back().m_raw = lines[cmd_line];
        commands.back().m_line = cmd_line + 1;
    }
    
    return(0);
}

// clead mod record
void SpellMod::Clear()
{
    m_paths.clear();
    for(auto &item: m_sources)   
        delete item;
    m_sources.clear();
}

// try to get path variable or null
SpellModPath *SpellMod::GetPath(std::string name)
{
    for(auto &path: m_paths)
        if(path.name == name)
            return(&path);
    return(NULL);
}

// build/parse path (expand variables)
SpellModPath SpellMod::ParsePath(std::filesystem::path path,std::filesystem::path alt_path,std::string name)
{
    SpellModPath new_path;
    new_path.name = name;

    // try replace variables
    auto path_str = path.string();
    auto alt_path_str = path_str;
    bool has_alt_path = false;
    while(true)
    {
        auto var = regexp_get(path_str,"%(.+?)%");
        if(var.empty())
            break;
        auto subst = GetPath(var[0]);
        if(!subst)
            return(new_path);
        path_str = strrep(path_str,"%"+var[0]+"%",subst->path.string());
        if(!subst->alt_path.empty())
        {
            alt_path_str = strrep(alt_path_str,"%"+var[0]+"%",subst->alt_path.string());
            has_alt_path = true;
        }
    }

    if(has_alt_path && !alt_path.empty())
        return(new_path); // alt path double defined
    if(has_alt_path)
        alt_path = alt_path_str;

    // make new path
    new_path.path = std::filesystem::path(path_str).lexically_normal();
    if(!alt_path.empty())
        new_path.alt_path = std::filesystem::path(alt_path).lexically_normal();
    return(new_path);
}

// add path, returns pointer or null if duplicate exists or error
SpellModPath* SpellMod::AddPath(std::string name,std::filesystem::path path,std::filesystem::path alt_path,bool overwrite)
{
    // check duplicates
    auto cur_path = GetPath(name);
    if(cur_path && !overwrite)
        return(NULL); 

    auto new_path = ParsePath(path, alt_path, name);
    if(!new_path.isValid())
        return(NULL);
    if(cur_path)
    {
        // exists - just modify
        *cur_path = new_path;
        return(cur_path);
    }
    // add new
    m_paths.push_back(new_path);
    return(&m_paths.back());
}

// get source archive from memory (or null if not loaded)
SpellArchive* SpellMod::GetArchive(SpellModPath &path)
{
    for(auto &item: m_sources)
        if(item->m_path == path.path)
            return(item);
    for(auto& item: m_sources)
        if(item->m_path == path.alt_path)
            return(item);
    return(NULL);
}

// load source archive to memory or return pointer to already loaded
SpellArchive* SpellMod::LoadArchive(SpellModPath &path, SpellArchive::Type arch_type)
{
    SpellArchive *arch = GetArchive(path);
    if(arch)
        return(arch);

    // try load archive data
    try {
        arch = new SpellArchive(path,arch_type);
    }catch(const std::runtime_error& error) {
        return(NULL);
    }

    // add to list
    m_sources.push_back(arch);
    
    return(arch);
}


int SpellMod::LoadDEF(Config& config)
{
    auto mod_dir = config.mod_path.parent_path();

    // define basic paths if not done yet
    AddPath("SPELL",config.spell_dir);
    if(!config.spellcd_dir.empty())
        AddPath("SPELLCD",config.spellcd_dir);
    AddPath("SPELLANY",config.spell_dir,config.spellcd_dir);

    // load DEF file
    PrintConsole(" - Loading mod DEF file (%ls) ... ",config.mod_path.wstring().c_str());
    if(loadstr(config.mod_path,m_def))
    {
        PrintConsole("failed!\n");
        return(1);
    }
    PrintConsole("done.\n");
    // normalize line breaks
    m_def = strrep(m_def,"\r\n","\n");

    // parse PATH section
    PrintConsole(" - Parsing PATH section ... ");
    SpellModCmdList cmd_list;
    if(GetClass(m_def,"PATH",cmd_list))
    {
        PrintConsole("failed! Missing PATH section.\n");
        return(1);
    }
    for(auto& cmd: cmd_list)
    {
        std::string var_name;
        std::string var_value;
        if(cmd.isAssign(var_name,var_value))
        {
            // likely path assign
            auto path = std::filesystem::path(var_value).lexically_normal();
            if(path.is_relative())
                path = mod_dir / path;
            if(!AddPath(var_name,path,"",true))
            {
                PrintConsole("failed! Line %d: adding path \"%s\".\n",cmd.m_line,cmd.m_raw.c_str());
                return(1);
            }
        }
        else
        {
            PrintConsole("failed! Line %d: uknown command \"%s\".\n",cmd.m_line,cmd.m_raw.c_str());
            return(1);
        }
    }

    // check MAKE folder
    auto path = GetPath("MAKE");
    if(!path->isValid())
    {
        // missing MAKE folder definition
        PrintConsole("failed! Missing MAKE variable defintion.");
        return(1);
    }
    auto make_dir = path->path;
    PrintConsole("done.\n");

    // make sure MAKE folder exist
    std::filesystem::create_directories(make_dir);

    return(0);
}

// try load mod DEF file
int SpellMod::BuildMod(Config& config, bool allow_restore)
{
    auto mod_dir = config.mod_path.parent_path();

    // first check if there is unrestored stuff and restore it
    bool something_to_restore;
    CheckRestoreMod(config,something_to_restore);
    if(something_to_restore && !allow_restore)
    {
        PrintConsole("Building mod ... failed! There is unrestored mod. Restore mod first.\n");
        return(1);
    }
    if(something_to_restore)
    {
        int err = RestoreMod(config);
        if(err)
            return(err);
    }

    PrintConsole("Building mod ...\n");
    
    // basic parse of DEF file
    if(LoadDEF(config))
        return(1);

    // check MAKE folder
    auto path = GetPath("MAKE");
    if(!path->isValid())
        return(1);
    auto make_dir = path->path;
        
    // parse SRC section (optional)
    PrintConsole(" - Parsing SRC section ... ");
    SpellModCmdList cmd_list;
    GetClass(m_def,"SRC",cmd_list);
    for(auto& cmd: cmd_list)
    {
        std::string var_name;
        std::string var_value;
        if(cmd.isAssign(var_name,var_value))
        {
            // likely path assign
            auto path = std::filesystem::path(var_value).lexically_normal();            
            if(!AddPath(var_name,path))
            {
                PrintConsole("failed! Line %d: adding path \"%s\".\n",cmd.m_line,cmd.m_raw.c_str());
                return(1);
            }
        }
        else
        {
            PrintConsole("failed! Line %d: uknown command \"%s\".\n",cmd.m_line,cmd.m_raw.c_str());
            return(1);
        }
    }
    PrintConsole("done.\n");

    // parse archive section(s)
    //std::vector<std::string> archive_names = {"COMMON.FS", "T11.FS", "PUST.FS", "DEVAST.FS", "TEXTS.FS", "SAMPLES.FS", "MUSIC.FS", "RESEARCH.FS", "INFO.FS", "MOVIE.FS", "UNITS.FSU"};
    std::vector<std::string> archive_names ={"UNITS.FSU"};
    for(auto &arch_name: archive_names)
    {
        auto arch_path = make_dir / arch_name;

        // try load archive definition class        
        if(GetClass(m_def,arch_name,cmd_list))
            continue;

        PrintConsole(" - Building %s archive ... ",arch_name.c_str());

        // make blank archive
        //FSarchive *fs = new FSarchive(arch_name);        
        SpellArchive arch;
        bool glob_replace = 0;

        // section exists
        for(auto& cmd: cmd_list)
        {
            std::string var_name;
            std::string var_value;
            std::vector<std::string> par_list;
            if(cmd.isAssign(var_name,var_value))
            {
                // var assign
                if(var_name == "replace")
                    glob_replace = std::atoi(var_value.c_str());
                else
                {
                    // unknown variable                    
                    PrintConsole("failed! Line %d: unknown command \"%s\".\n",cmd.m_line,cmd.m_raw.c_str());
                    return(1);
                }
            }
            else if(cmd.isFunction(var_name, par_list))
            {
                // function
                if(var_name == "add")
                {
                    // add stuff
                    std::string path = "";
                    std::string mode = (glob_replace)?"ALL":"NEW";
                    std::string wild = "*";
                    if(par_list.size() == 3)
                    {
                        // add(mode, path, wildcard)
                        mode = par_list[0];
                        path = par_list[1];
                        mode = par_list[2];
                    }
                    else if(par_list.size() == 2)
                    {
                        if(par_list[0] == "ALL" || par_list[0] == "NEW")
                        {
                            // add(mode, path)
                            mode = par_list[0];
                            path = par_list[1];
                        }
                        else
                        {
                            // add(path, wildcard)
                            path = par_list[0];
                            wild = par_list[1];
                        }
                    }
                    else if(par_list.size() == 1)
                    {
                        // add(path)
                        path = par_list[0];
                    }
                    else
                    {
                        // wrong parameters count for add() command
                        PrintConsole("failed! Line %d: wrong params combination is command \"%s\".\n",cmd.m_line,cmd.m_raw.c_str());
                        return(1);
                    }
                    if(mode != "ALL" && mode != "NEW")
                    {
                        // unknown mode parameter for add() command
                        PrintConsole("failed! Line %d: unknown mode \"%s\" in command \"%s\".\n",cmd.m_line,mode.c_str(),cmd.m_raw.c_str());
                        return(1);
                    }
                    bool replace = (mode == "ALL");
                    
                    // parse path (expand vars)
                    auto parsed_path = ParsePath(path);
                    if(!parsed_path.isValid())
                    {
                        // parsing path for add() command failed
                        PrintConsole("failed! Line %d: parsing source path failed in command \"%s\".\n",cmd.m_line,cmd.m_raw.c_str());
                        return(1);
                    }
                    
                    // load source archive
                    SpellArchive::Type arch_type = SpellArchive::Type::AUTO;
                    if(wildcmp("*.FSU",arch_name.c_str()))
                        arch_type = SpellArchive::Type::FSU;
                    auto src = LoadArchive(parsed_path,arch_type);
                    if(!src)
                    {
                        // loading source data for add() command failed
                        PrintConsole("failed! Line %d: loading source data failed in command \"%s\".\n",cmd.m_line,cmd.m_raw.c_str());
                        return(1);
                    }

                    // add stuff
                    int count = 0;
                    for(auto &name: src->GetItemNames())
                    {
                        if(!wildcmp(wild, name))
                            continue;                        
                        if(arch.AddFile(*src, name, replace))
                        {
                            // adding file to archive in add() command failed
                            PrintConsole("failed! Line %d: adding file \"%s\" in command \"%s\".\n",cmd.m_line,name.c_str(),cmd.m_raw.c_str());
                            return(1);
                        }
                        count++;
                    }
                    if(!count && !iswild(wild))
                    {
                        // source file in add() command not found
                        PrintConsole("failed! Line %d: source file \"%s\" not found in command \"%s\".\n",cmd.m_line,wild.c_str(),cmd.m_raw.c_str());
                        return(1);
                    }

                }
                else
                {
                    // unknown command
                    PrintConsole("failed! Line %d: unknown command \"%s\".\n",cmd.m_line,cmd.m_raw.c_str());
                    return(1);
                }
            }
            else
            {
                // unknown command
                PrintConsole("failed! Line %d: unknown command \"%s\".\n",cmd.m_line,cmd.m_raw.c_str());
                return(1);
            }
        }
        // save archive

        // check if target archive differs from new one
        bool can_write = true;
        if(std::filesystem::exists(arch_path))
        {            
            // try load archive from target path            
            SpellArchive ref_arch;
            if(ref_arch.Load(arch_path))
            {
                // archive loading failed
                PrintConsole("failed! Loading target archive \"%ls\".\n",arch_path.wstring().c_str());
                return(1);
            }
            // compare by content
            can_write = !arch.Compare(ref_arch);
        }
        if(!can_write)
        {
            PrintConsole("saved to \"%ls\".\n",arch_path.wstring().c_str());
            continue;
        }

        // try save archive
        if(arch.Save(arch_path, true))
        {
            // writting archive failed
            PrintConsole("failed! Saving target archive \"%ls\".\n",arch_path.wstring().c_str());
            return(1);
        }
        PrintConsole("saved to \"%ls\".\n",arch_path.wstring().c_str());
    }

    PrintConsole(" - Building mod done!\n");

    return(0);
}

// checks if there is something to restore (silent - no console output)
int SpellMod::CheckRestoreMod(Config& config, bool &something_to_restore)
{
    something_to_restore = false;

    // try load state ini
    CSimpleIniA ini;
    ini.SetUnicode(true);
    if(ini.LoadFile(config.state_ini_path.wstring().c_str()) != SI_OK)
    {
        return(0); // nothing to restore
    }
    
    // scan archives list
    std::list<CSimpleIniA::Entry> arch_list;
    ini.GetAllSections(arch_list);
    bool was_error = false;
    for(auto& arch: arch_list)
    {
        std::string name = arch.pItem;
        auto modified = ini.GetBoolValue(name.c_str(),"modified",false);        
        if(modified)
        {
            something_to_restore = true;
            return(0);
        }
    }
    return(0);
}

// try restore files state based on state ini
int SpellMod::RestoreMod(Config& config)
{
    PrintConsole("Restoring game archives ... ");

    bool something_to_restore;
    CheckRestoreMod(config, something_to_restore);
    if(!something_to_restore)
    {
        PrintConsole("done (nothing to do)!\n");
        return(0); // nothing to restore
    }

    CSimpleIniA ini;
    ini.SetUnicode(true);
    if(ini.LoadFile(config.state_ini_path.wstring().c_str()) != SI_OK)
    {
        PrintConsole("done (nothing to do)!\n");
        return(0); // nothing to restore
    }
    
    PrintConsole("\n - loading state file (%ls)\n",config.state_ini_path.wstring().c_str());
    std::list<CSimpleIniA::Entry> arch_list;
    ini.GetAllSections(arch_list);        
    bool was_error = false;
    for(auto& arch: arch_list)
    {
        std::string name = arch.pItem;

        // is this save game folder?
        bool is_save = name == "SAVE";

        PrintConsole(" - restoring %s ... ", name.c_str());

        auto modified = ini.GetBoolValue(name.c_str(), "modified", false);
        auto location_cstr = ini.GetValue(name.c_str(), "location", NULL);
        auto where_original_cstr = ini.GetValue(name.c_str(),"where_original",NULL);
        auto mod_from_cstr = ini.GetValue(name.c_str(),"mod_from",NULL);
        if(!modified)
        {         
            ini.Delete(name.c_str(),NULL);
            PrintConsole("done\n",name.c_str());
            continue;
        }
        if(!location_cstr || !where_original_cstr || !mod_from_cstr)
        {            
            PrintConsole("failed! Missing some of original/mod/location path values.\n");
            was_error = true;
            continue;
        }
        auto location = std::filesystem::path(location_cstr);
        auto where_original = std::filesystem::path(where_original_cstr);
        auto mod_from = std::filesystem::path(mod_from_cstr);
        
        if(!mod_from.empty())
        {
            if(!std::filesystem::exists(mod_from.parent_path()))
            {
                PrintConsole("failed! Mod MAKE directory not exist (%ls).\n",mod_from.parent_path().wstring().c_str());
                was_error = true;
                continue;
            }
            if(!std::filesystem::exists(location))
            {
                PrintConsole("failed! Game archive path not exist (%ls).\n",location.wstring().c_str());
                was_error = true;
                continue;
            }

            // move mod file back to MAKE location
            if(fs_rename(location,mod_from))
            {
                PrintConsole("failed! Moving mod (%ls) back to MAKE location (%ls).\n",location.wstring().c_str(), mod_from.wstring().c_str());
                was_error = true;
                continue;
            }            
            if(is_save)
                MakeSaveIni(mod_from,string_format("Save games of mod \"%ls\"",config.mod_path.wstring().c_str()));
        }       
                
        if(!std::filesystem::exists(where_original))
        {
            PrintConsole("failed! Original game archive path not exist (%ls).\n",where_original.wstring().c_str());
            was_error = true;
            continue;
        }
                
        // move original from temp back to game location
        if(fs_rename(where_original,location))
        {
            PrintConsole("failed! Moving original from temp location (%ls) back (%ls).\n",where_original.wstring().c_str(),location.wstring().c_str());
            was_error = true;
            continue;
        }
        if(is_save)
            MakeSaveIni(location,"Original game SAVE folder");

        PrintConsole("done.\n");
        ini.Delete(name.c_str(),NULL);
    }

    if(ini.SaveFile(config.state_ini_path.wstring().c_str()) != SI_OK)
    {
        PrintConsole(" - Saving mod state ini failed!\n");
        return(1);
    }
    if(was_error)
        PrintConsole(" - Restoration done with errors!\n");
    else
        PrintConsole(" - Restoration done!\n");
    return(0);
}

// swap mod file to game
int SpellMod::SwapMod(Config& config, bool allow_restore)
{
    auto mod_dir = config.mod_path.parent_path();

    // first check if there is unrestored stuff and restore it
    bool something_to_restore;
    CheckRestoreMod(config,something_to_restore);
    if(something_to_restore && !allow_restore)
    {
        PrintConsole("Modifying game archives ... failed! There is unrestored mod. Restore mod first.\n");
        return(1);
    }
    if(something_to_restore)
    {
        int err = RestoreMod(config);
        if(err)
            return(err);
    }

    PrintConsole("Modifying game archives ... \n");

    // dow basic parse of mod DEF file
    if(LoadDEF(config))
    {        
        return(1);
    }

    // check MAKE folder
    auto path = GetPath("MAKE");
    if(!path->isValid())
        return(1);
    auto make_dir = path->path;

    // candidate search paths of original Spellcross archives
    std::vector<std::filesystem::path> spell_dirs = {config.spell_dir};
    if(config.allow_cd_mod && !config.spellcd_dir.empty())
        spell_dirs.push_back(config.spellcd_dir);      

    // look for all archives in mod MAKE
    PrintConsole(" - Reading MAKE folder archives...\n");
    std::vector<ModArchivesList> arch_list;
    std::vector<std::filesystem::path> dir_list;
    for(const auto& entry: std::filesystem::directory_iterator(make_dir))
    {
        auto item = entry.path();
        if(entry.is_directory())
            continue;
        auto name = item.filename().string();
        name = toupper(name);
        if(!wildcmp("*.FS*",name.c_str()))
            continue;        

        ModArchivesList arch;
        arch.name = name;
        arch.source = item;

        // try to find where to place it original spellcross data (can be installation or CD)
        for(auto &dir: spell_dirs)
        {
            auto path = dir / "DATA" / name;
            if(!std::filesystem::exists(path))
                continue;
            arch.dest = path;
        }
        if(arch.dest.empty())
        {            
            continue;
            //return(1);
        }

        arch.dest_dir = arch.dest.parent_path();
        dir_list.push_back(arch.dest_dir);
        arch_list.push_back(arch);
    }

    // make temp directory where original spellcross data will be stored
    auto temp_dir_name = "spellcross_mod_builder_orig_data_temp";
    
    // make temp directories
    std::sort(dir_list.begin(),dir_list.end());
    auto dir_list_un = std::unique(dir_list.begin(), dir_list.end());
    dir_list.resize(std::distance(dir_list.begin(),dir_list_un));
    for(auto dir: dir_list)
    {
        dir.append(temp_dir_name);
        std::filesystem::create_directories(dir);
    }

    CSimpleIniA ini;
    //ini.LoadFile(config.state_ini_path.wstring().c_str());
    
    // swap archives
    bool was_error = false;
    for(auto &arch: arch_list)
    {
        auto temp_path = arch.dest_dir / temp_dir_name / arch.name;

        PrintConsole(" - Replacing archive %s ... ", arch.name.c_str());
        
        // move original to temp
        if(fs_rename(arch.dest,temp_path))
        {
            PrintConsole("failed! Moving original (%ls) to temp location (%ls).\n",arch.dest.wstring().c_str(), temp_path.wstring().c_str());            
            break;
        }
        ini.SetBoolValue(arch.name.c_str(),"modified",true);
        ini.SetValue(arch.name.c_str(),"location",arch.dest.string().c_str());
        ini.SetValue(arch.name.c_str(),"where_original",temp_path.string().c_str());
        ini.SetValue(arch.name.c_str(),"mod_from","");

        // move mod to original
        if(fs_rename(arch.source,arch.dest))
        {
            PrintConsole("failed! Moving mod (%ls) to game location (%ls).\n",arch.source.wstring().c_str(),arch.dest.wstring().c_str());
            break;
        }
        ini.SetValue(arch.name.c_str(),"mod_from",arch.source.string().c_str());
                
        PrintConsole("done.\n");
    }
    
    // swap save games?
    if(config.move_saves)
    {        
        auto save_dir = config.spell_dir / "SAVE";
        auto mod_save_dir = mod_dir / "save";
        auto save_temp_dir = config.spell_dir / "spellcross_mod_builder_orig_save_temp";
        
        PrintConsole(" - Replacing SAVE games ...");
        if(!std::filesystem::exists(mod_save_dir))
        {
            // no save games in mod folder yet: make copy of current game saves
            if(fs_copy(save_dir,mod_save_dir,std::filesystem::copy_options::recursive))
                PrintConsole("failed! Making initial copy of games saves (%ls) to mod location (%ls).\n",mod_save_dir.wstring().c_str(),save_dir.wstring().c_str());
            else
                MakeSaveIni(mod_save_dir,"Initial copy of original Spellcross SAVE folder");
        }

        MakeSaveIni(save_dir,"Temporary copy of original game SAVE folder");
        
        // now try to swap saves
        if(fs_rename(save_dir,save_temp_dir))
            PrintConsole("failed! Moving original saves (%ls) to temp location (%ls).\n",save_dir.wstring().c_str(),save_temp_dir.wstring().c_str());
        else
        {
            ini.SetBoolValue("SAVE","modified",true);
            ini.SetValue("SAVE","location",save_dir.string().c_str());
            ini.SetValue("SAVE","where_original",save_temp_dir.string().c_str());
            ini.SetValue("SAVE","mod_from","");     

            MakeSaveIni(mod_save_dir,string_format("Save games of mod \"%ls\"",config.mod_path.wstring().c_str()));
                        
            if(fs_rename(mod_save_dir,save_dir))
                PrintConsole("failed! Moving mod saves (%ls) to game location (%ls).\n",mod_save_dir.wstring().c_str(),save_dir.wstring().c_str());
            else
            {
                ini.SetValue("SAVE","mod_from",mod_save_dir.string().c_str());            
                
                
            }
        }

        PrintConsole(" done\n");
        
    }

    // save state
    ini.SaveFile(config.state_ini_path.wstring().c_str());
    if(was_error)
        PrintConsole(" - Game modification failed!\n");
    else
        PrintConsole(" - Game mod done!\n");

    return(0);
}

// write save.ini to given folder
int SpellMod::MakeSaveIni(std::filesystem::path save_dir, std::string description)
{
    CSimpleIniA save_ini;
    save_ini.SetValue("SAVE","what_is_it","Set of Spellcross save games (SAVE folder) moved/copied by Spellcross Mod Builder","; Info");
    save_ini.SetValue("SAVE","description",description.c_str(),"; Save games set description");
    save_ini.SetValue("SAVE","date",get_timestr_iso().c_str(),"; Timestamp of last move of this saves set");
    auto save_ini_path = save_dir / "save.ini";
    return(save_ini.SaveFile(save_ini_path.wstring().c_str()));
}





