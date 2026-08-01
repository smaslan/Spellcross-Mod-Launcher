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
#include "SpellMod.h"

#include <regex>
#include <stdexcept>
#include <filesystem>
#include <sys/stat.h>

#include "other.h"
#include "fs_archive.h"
#include "fsu_archive.h"
#include "spell_units.h"
#include "spell_randomizer.h"
#include "spell_font.h"
#include "spell_def.h"
#include "SpellLaunch.h"
#include "SimpleIni.h"
#include "cparse/shunting-yard.h"


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
    m_last_error = "";
}

// load data archive
SpellArchive::SpellArchive(SpellModPath &path,Type explicit_archive_type)
{
    m_fs = NULL;
    m_fsu = NULL;
    m_path = "";
    m_last_error = "";
    if(Load(path,explicit_archive_type))
        throw std::runtime_error(m_last_error);        
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

// load archive from path (must be empty before)
int SpellArchive::Load(std::filesystem::path path, Type explicit_archive_type)
{
    m_last_error = "";
    
    if(!path.has_extension())
    {
        // source if folder: make archive from folder data:
        if(explicit_archive_type == Type::FSU)
        {        
            // FSU archive:
            if(m_fsu)
            {
                m_last_error = string_format("Creating FSU archive from directory \"%ls\" to object SpellArchive failed! Object already has some archive loaded.",path.wstring().c_str());
                return(1);
            }
            m_fsu = new FSUarchive();
            if(m_fsu->LoadFolder(path))
            {
                m_last_error = m_fsu->m_last_error;
                delete m_fsu;
                m_fsu = NULL;
                return(1);
            }
            m_path = path;        
        }
        else
        {
            // FS archive:
            if(m_fs)
            {
                m_last_error = string_format("Creating FS archive from directory \"%ls\" to object SpellArchive failed! Object already has some archive loaded.",path.wstring().c_str());
                return(1);
            }
            m_fs = new FSarchive();
            if(m_fs->LoadFolder(path))
            {
                m_last_error = m_fs->m_last_error;
                delete m_fs;
                m_fs = NULL;
                return(1);
            }
            m_path = path;
        }

    }    
    else if(iequals(path.extension().string(),".FS"))
    {
        // try load FS archive data
        if(m_fsu)
        {
            m_last_error = string_format("Loading FS archive \"%ls\" to object SpellArchive failed! Object already has some archive loaded.",path.wstring().c_str());
            return(1);
        }
        try{
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
        if(m_fsu)
        {
            m_last_error = string_format("Loading FSU archive \"%ls\" to object SpellArchive failed! Object already has some archive loaded.",path.wstring().c_str());
            return(1);
        }
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
    
    std::vector<std::filesystem::path> path_list = {path.path, path.alt_path};
    for(auto& path: path_list)
    {
        if(path.empty())
            return(1);
        if(!Load(path,explicit_archive_type))
            return(0);
    }
    return(1);
}

// save archive to path
int SpellArchive::Save(std::filesystem::path path,bool allow_overwrite)
{
    m_last_error = "";
    if(m_fs)
    {
        // FS archive
        auto err = m_fs->SaveFile(path,allow_overwrite);
        m_last_error = m_fs->m_last_error;
        return(err);
    }
    else if(m_fsu)
    {
        // FSU archive
        auto err = m_fsu->Save(path,allow_overwrite);
        m_last_error = m_fsu->m_last_error;
        return(err);
    }
    else
        return(1);
}

// get last error string
std::string SpellArchive::GetLastError()
{
    return(m_last_error);
}

// is archive folder?
bool SpellArchive::isFolder()
{
    return(!m_path.has_extension());
}

// is archive empty?
bool SpellArchive::isEmpty()
{
    if(!m_fs && !m_fsu)
        return(true);
    if(m_fs && !m_fs->Count())
        return(true);
    if(m_fsu && !m_fsu->GetCount())
        return(true);
    return(false);
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
int SpellArchive::GetFile(std::string name,std::vector<uint8_t>& data)
{
    m_last_error = "";

    data.clear();
    if(m_fs)
    {
        auto err = m_fs->GetFile(name.c_str(), data);
        m_last_error = m_fs->m_last_error;
        return(err);
    }
    else if(m_fsu)
    {
        m_last_error = "GetFile from FSU archive not implemented!";
        return(1);
    }
    m_last_error = string_format("Get file \"%s\" from spell archive failed! No archive loaded yet.",name.c_str());
    return(1);
}

// try get file from archive
/*std::vector<uint8_t> *SpellArchive::GetFile(std::string& name)
{
    m_last_error = "";
    if(!m_fs)
        return(NULL);
    return(m_fs->GetFileData(name.c_str()));
}*/

// remove file from archive (no error if not found)
int SpellArchive::RemoveFile(std::string& name,bool ignore_error)
{
    m_last_error = "";

    if(!m_fs && !m_fsu && !ignore_error)
    {
        m_last_error = string_format("Cannot remove file \"%s\" from archive! Empty archive.",name.c_str());
        return(1);
    }

    if(m_fs)
    {
        // FS archive:
        int err = m_fs->RemoveFile(name);
        if(err && !ignore_error)
        {
            m_last_error = m_fs->m_last_error;
            return(1);
        }
    }
    else if(m_fsu)
    {
        // FSU archive:
        if(m_fsu->RemoveResource(name,ignore_error))
        {
            m_last_error = string_format("Cannot remove resource \"%s\" from FSU archive! Resource not found.",name.c_str());
            return(1);
        }        
    }
    return(0);
}

// add file from another archive
int SpellArchive::AddFile(SpellArchive &src, std::string& name, bool allow_replace, std::string new_name)
{   
    m_last_error = "";

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
        {
            m_last_error = string_format("Cannot add FSU data to FS archive!");
            return(1);
        }
        auto data = src.m_fs->GetFileData(name.c_str());
        if(!data)
        {
            m_last_error = string_format("Adding file \"%s\" to FS archive failed! File not found in source archive \"%ls\".",name.c_str(),src.m_fs->m_file_path.c_str());
            return(1);
        }
        // optional rename
        if(!new_name.empty())
            name = new_name;
        auto err = m_fs->AddFile(name,*data,allow_replace);
        m_last_error = m_fs->m_last_error;
        return(err);
    }
    if(m_fsu)
    {
        // FSU archive:
        if(!src.m_fsu)
        {
            m_last_error = string_format("Cannot add FS data to FSU archive!");
            return(1);
        }
        auto data = src.m_fsu->GetResource(name.c_str());        
        if(!data)
        {
            m_last_error = string_format("Adding file \"%s\" to FSU archive failed! File not found in source archive.",name.c_str());
            return(1);
        }        
        auto err = m_fsu->AddResource(data,allow_replace,new_name);
        m_last_error = m_fsu->m_last_error;
        return(err);
    }
    return(1);
}

// add file data from raw string
int SpellArchive::AddFile(std::string &string,std::string& name,bool allow_replace)
{
    m_last_error = "";

    if(m_fsu)
    {
        m_last_error = string_format("Cannot add raw text to FSU archive!");
        return(1);
    }
    if(!m_fs)
    {
        // first file to add: make blank FS
        m_fs = new FSarchive();
    }
    if(m_fs)
    {
        // FS archive:
        std::vector<uint8_t> data(string.begin(), string.end());
        auto err = m_fs->AddFile(name,data,allow_replace);
        m_last_error = m_fs->m_last_error;
        return(err);
    }
    if(m_fsu)
    {
        // FSU archive:        
    }
    return(1);
}

// add file data from raw vector
int SpellArchive::AddFile(std::vector<uint8_t>& data,std::string& name,bool allow_replace)
{
    m_last_error = "";

    if(m_fsu)
    {
        m_last_error = string_format("Cannot add raw data to FSU archive!");
        return(1);
    }
    if(!m_fs)
    {
        // first file to add: make blank FS
        m_fs = new FSarchive();
    }
    if(m_fs)
    {
        // FS archive:
        auto err = m_fs->AddFile(name,data,allow_replace);
        m_last_error = m_fs->m_last_error;
        return(err);
    }
    if(m_fsu)
    {
        // FSU archive:        
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

// get DEF section
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
        start_line = cmd_line + 1;
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
        
        /*auto cmd = regexp_get(lines[cmd_line],"\\s*^(?!//)(.*?(?=\\/\\/|$))");
        if(cmd.empty())
            continue;
        cmd = regexp_get(cmd[0], "(.*);");
        if(cmd.empty())
            continue;*/

        // try extract command portion of line
        auto line = lines[cmd_line];
        bool is_string = false;
        auto pend = line.begin();
        for(auto p = line.begin(); p < line.end(); p++)
        {
            if(*p == '\"')
            {
                // string section
                is_string = !is_string;
                continue;
            }
            
            // potential end of command mark
            if(!is_string && *p == ';')
                pend = p;

            if(!is_string && (p + 1) < line.end() && p[0] == '/' && p[1] == '/')
            {
                // comment
                break;
            }
        }
        line = line.substr(0,pend - line.begin());
        if(line.empty())
            continue;

        commands.emplace_back();
        commands.back().m_cmd = line;
        commands.back().m_raw = lines[cmd_line];
        commands.back().m_line = cmd_line + 1;
    }
    
    return(0);
}

// check if command is asignement "left=right", return left, right
int SpellModCmd::isAssign(std::string& left,std::string& right)
{
    /*auto tokens = regexp_get(m_cmd,"(.+?)\\((.*?)\\)");
    if(!tokens.empty())
        return(0);
    left.clear();
    right.clear();
    tokens = regexp_get(m_cmd,"(.+?)\s*=\s*?(.*)");
    if(tokens.size() != 2)
        return(0);
    left = tokens[0];
    right = tokens[1];*/

    std::vector<std::string> func_params;
    std::string rest;
    auto is_func = isFunction(left,func_params,rest);
    if(is_func)
        return(0);
    if(func_params.size() != 1)
        return(0);
    right = func_params[0];
    return(1);
}

// check if command is function, separete name an parameters list
int SpellModCmd::isFunction(std::string& func_name,std::vector<std::string>& func_params,std::string& rest)
{
    func_params.clear();
    func_name.clear();
    rest.clear();
    
    std::string par;
    std::string::iterator par_start;
    std::string::iterator qu_end;
    bool is_func = false;
    bool is_string = false;
    bool was_quote = false;
    for(auto p = m_cmd.begin(); p < m_cmd.end(); p++)
    {
        if(*p == '\"')
        {
            // string section
            is_string = !is_string;
            if(is_string)
            {                
                par = trim_whites(m_cmd.substr(par_start - m_cmd.begin(),p - par_start));
                if(par.empty())
                {
                    par_start = p + 1;
                    was_quote = true;
                }
            }
            else
            {
                if(was_quote)
                    par = m_cmd.substr(par_start - m_cmd.begin(),p - par_start);
                qu_end = p + 1;
            }
            continue;
        }
        if(is_string)
            continue;
        
        if(!is_func && *p == '=')
        {
            func_name = trim_whites(m_cmd.substr(0,p - m_cmd.begin()));
            par = trim_whites(m_cmd.substr(p - m_cmd.begin() + 1));
            auto toks = regexp_get(par,"^\"(.*)\"$");
            if(!toks.empty())
                par = toks[0];
            func_params.push_back(par);
            break;
        }
        if(*p == '(')
        {
            func_name = m_cmd.substr(0,p - m_cmd.begin());
            par_start = p + 1;
            is_func = true;
            continue;
        }
        if(*p == ')' || *p == ',')
        {
            if(!was_quote)
                par = trim_whites(m_cmd.substr(par_start - m_cmd.begin(),p - par_start));
            else
            {
                // append rest of parameter range if it is not empty, e.g. (...,{"quoted string";some extra stuff},...)
                auto rest = m_cmd.substr(qu_end - m_cmd.begin(),p - qu_end);
                if(!trim_whites(rest).empty())
                    par += rest;
            }
            func_params.push_back(par);
            par.clear();
            par_start = p + 1;            
            was_quote = false;
        }
        if(is_func && *p == ')')
        {
            rest = trim_whites(m_cmd.substr(p - m_cmd.begin() + 1));
            break;
        }
    }

    /*auto tokens = regexp_get(m_cmd,"(.+?)\\((.*?)\\)");
    if(tokens.size() != 2)
        return(0);
    func_name = tokens[0];
    func_params = get_text_lines(tokens[1],true,',');*/

    return(is_func);
}


// clead mod record
void SpellMod::Clear()
{
    m_paths.clear();
    for(auto &item: m_sources)   
        delete item;
    m_sources.clear();
    m_options.clear();
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
    m_last_error.clear();

    // check duplicates
    auto cur_path = GetPath(name);
    if(cur_path && !overwrite)
    {
        m_last_error = string_format("Cannot define path variable \"%s\". It already exist.",name.c_str());
        return(NULL);
    }

    std::string var_value;
    if(!GetVar(name,var_value))
    {
        m_last_error = string_format("Cannot define path variable \"%s\". It already exist as local variable.",name.c_str());
        return(NULL);
    }

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

// clear local variable
void SpellMod::ClearVars()
{
    m_vars.clear();
}

// try get variable
int SpellMod::GetVar(std::string name,std::string& value)
{
    for(auto it = m_vars.begin(); it != m_vars.end(); it++)
    {
        if(it->first != name)
            continue;
        value = it->second;
        return(0);
    }
    return(1);
}

// try add variable to list
int SpellMod::AddVar(std::string name,std::string value,bool allow_update)
{    
    m_last_error.clear();
    auto path = GetPath(name);
    if(path)
    {
        m_last_error = string_format("Cannot define variable \"%s\". It exist as path variable.",name.c_str());
        return(1);
    }
    for(auto it = m_vars.begin(); it != m_vars.end(); it++)
    {
        if(it->first != name)
            continue;
        if(!allow_update)
        {
            m_last_error = string_format("Cannot define variable \"%s\". It already exists.",name.c_str());
            return(1);
        }
        it->second = value;
        return(0);
    }    
    m_vars.insert({name,value});
    return(0);
}

// replace variable(s) in string
void SpellMod::ReplaceVars(std::string &string)
{
    for(auto it = m_vars.begin(); it != m_vars.end(); it++)
    {
        auto key = string_format("%%%s%%",it->first.c_str());
        string = strrep(string,key,it->second);
    }
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
    m_last_error = "";

    SpellArchive *arch = GetArchive(path);
    if(arch)
        return(arch);

    // try load archive data
    try {
        arch = new SpellArchive(path,arch_type);
    }catch(const std::runtime_error& error) {
        m_last_error = string_format("%s",error.what());
        return(NULL);
    }

    // add to list
    m_sources.push_back(arch);
    
    return(arch);
}


// try get mod option
SpellModOption *SpellMod::GetOption(std::string label)
{
    for(auto &opt: m_options)
        if(opt.label == label)
            return(&opt);
    return(NULL);
}

// add mod options record
SpellModOption *SpellMod::AddOption(std::string label,std::string description,int min,int max,int def,std::vector<std::string> &enum_strings)
{
    if(GetOption(label))
    {
        // already exist
        m_last_error = string_format("Option with label \"%s\" already exists!",label.c_str());
        return(NULL);
    }

    // add option
    SpellModOption opt;
    opt.label = label;
    opt.description = description;
    opt.min_value = min;
    opt.max_value = max;
    opt.def_value = def;
    opt.value = opt.def_value;
    opt.enum_list = enum_strings;
    m_options.push_back(opt);

    return(&m_options.back());
}

// is mod option enum type?
bool SpellModOption::isEnum()
{
    return(!enum_list.empty());
}

// get map of options
std::map<int,std::string> SpellModOption::GetEnumMap()
{
    std::map<int,std::string> list;
    int val = min_value;
    for(auto &item: enum_list)
        list.insert({val++,item});
    return(list);
}

// get enym string value of option
std::string SpellModOption::GetEnumValue()
{
    if(value < min_value || value > max_value || (value - min_value) >= enum_list.size())
        return(string_format("<%d>",value));
    return(enum_list[value - min_value]);
}


// basic parse of mod DEF file
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
        std::string func_name;
        std::string func_rest;
        std::vector<std::string> func_params;
        if(cmd.isAssign(var_name,var_value))
        {
            // likely path assign
            auto path = std::filesystem::path(var_value).lexically_normal();
            auto p_path = ParsePath(path);
            if(!p_path.isValid())
            {
                PrintConsole("failed! Line %d: adding path \"%s\".\n",cmd.m_line,cmd.m_raw.c_str());
                return(1);
            }            
            if(std::filesystem::path(p_path.path).is_relative())
                path = mod_dir / path;
            if(!AddPath(var_name,path,"",true))
            {
                PrintConsole("failed! Line %d: adding path \"%s\".\n",cmd.m_line,cmd.m_raw.c_str());
                return(1);
            }
        }
        else if(cmd.isFunction(func_name,func_params,func_rest))
        {
            // functions
            if(func_name == "version")
            {
                // lock mod to certain game version:
                //   version(version_name); // where version name is CZE or ENG
                if(func_params.size() != 1)
                {
                    PrintConsole("failed! Line %d: wrong params count in command \"%s\".\n",cmd.m_line,cmd.m_raw.c_str());
                    return(1);
                }
                SpellLaunch::EngineVersion target_ver = SpellLaunch::EngineVersion::NONE;
                auto ver_str = toupper(func_params[0]);
                if(ver_str == "EN" || ver_str == "ENG")
                    target_ver = SpellLaunch::EngineVersion::ENG;
                else if(ver_str == "CZ" || ver_str == "CZE")
                    target_ver = SpellLaunch::EngineVersion::CZE;
                else
                {
                    PrintConsole("failed! Line %d: wrong value of version name in command \"%s\". Must be CZE or ENG\n",cmd.m_line,cmd.m_raw.c_str());
                    return(1);
                }
                if(config.ver != target_ver)
                {
                    PrintConsole("failed! Line %d: requested game version %s not matching selected game enegine version in command \"%s\". \n",cmd.m_line,ver_str.c_str(),cmd.m_raw.c_str());
                    return(1);
                }                
            }
            else if(func_name == "option")
            {
                // mod option definition
                if(func_params.size() < 5)
                {
                    PrintConsole("failed! Line %d: not enough parameters for command \"%s\".\n",cmd.m_line,cmd.m_raw.c_str());
                    return(1);
                }
                auto opt_label = func_params[0];
                int opt_min,opt_max,opt_default;
                if(str2int(func_params[2],opt_min))
                {
                    PrintConsole("failed! Line %d: wrong value of parameter 3 (min value) for command \"%s\".\n",cmd.m_line,cmd.m_raw.c_str());
                    return(1);
                }
                if(str2int(func_params[3],opt_max) || opt_max < opt_min)
                {
                    PrintConsole("failed! Line %d: wrong value of paramter 4 (max value) for command \"%s\".\n",cmd.m_line,cmd.m_raw.c_str());
                    return(1);
                }
                if(str2int(func_params[4],opt_default) || opt_default < opt_min || opt_default > opt_max)
                {
                    PrintConsole("failed! Line %d: wrong value of paramter 5 (default value) for command \"%s\".\n",cmd.m_line,cmd.m_raw.c_str());
                    return(1);
                }
                std::vector<std::string> opt_enums;
                if(func_params.size() >= 6)
                {
                    auto enum_str = regexp_get(func_params[5],"\\{(.*)\\}$");
                    if(!enum_str.empty())
                        opt_enums = str_split(enum_str[0],';',true);
                    if(!enum_str.empty() && opt_enums.size() != opt_max - opt_min + 1)
                    {
                        PrintConsole("failed! Line %d: wrong count of option strings in paramter 6 for command \"%s\".\n",cmd.m_line,cmd.m_raw.c_str());
                        return(1);
                    }
                }
                auto option = AddOption(opt_label,func_params[1],opt_min,opt_max,opt_default,opt_enums);
                if(!option)
                {
                    PrintConsole("failed! Line %d: failed addition mod option by command \"%s\".\n",cmd.m_line,cmd.m_raw.c_str());
                    return(1);
                }

                // try assign option value if exist
                for(auto &opt: config.options)
                    if(opt.label == opt_label)
                    {
                        if(opt.value < opt_min || opt.value > opt_max)
                        {
                            PrintConsole("failed! Line %d: provided option value %d outside defined range %d to %d for command \"%s\".\n",cmd.m_line,opt.value,opt_min,opt_max,cmd.m_raw.c_str());
                            return(1);
                        }
                        option->value = opt.value;
                    }
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

// try parse expression for conditional commands
int SpellMod::ParseExpression(std::string expr,bool &result)
{
    m_last_error = "";
    result = false;

    // replace %option% by _option_ and build expression parser variables list
    cparse::TokenMap vars;    
    for(auto &opt: m_options)
    {
        auto opt_name = string_format("%%%s%%",opt.label.c_str());
        auto var_name = string_format("_%s_",opt.label.c_str());
        vars[var_name] = opt.value;
        expr = strrep(expr,opt_name,var_name);
    }

    // parse expression  
    try{
        auto token = cparse::calculator::calculate(expr.c_str(),&vars);
        result = token.asBool();
    }catch(...) {
        m_last_error = string_format("Error parsing expression \"%s\"!",expr.c_str());
        return(1);
    }

    return(0);
}

// make initial screen with mod text
// params: title(pos_x,pos_y,align,color_text,shadow_text,text)
int SpellMod::MakeTitle(SpellArchive &arch, std::vector<std::string> &params)
{
    if(params.size() < 6)
    {
        m_last_error = string_format("Wrong params count for command title()!");
        return(1);
    }
    
    std::string img_name = "MAINMENU.LZ";
    std::string pal_name = "MAINMENU.PAL";
    std::string font_name = "FONT_001.FNT";

    // get existing image
    std::vector<uint8_t> lzdata;
    if(arch.GetFile(img_name,lzdata))
    {
        m_last_error = string_format("Missing file %s in source archive!",img_name.c_str());
        return(1);
    }
    LZWexpand lze(1000000);
    auto img = lze.Decode(lzdata);
    if(img.empty())
    {
        m_last_error = string_format("Decompressing file %s failed!",img_name.c_str());
        return(1);
    }
    int x_size = 640;
    int y_size = 480;
    if(img.size() != x_size*y_size)
    {
        m_last_error = string_format("Wring size of title image! Must be 640x480.");
        return(1);
    }

    // load palette
    std::vector<uint8_t> pal_data;
    if(arch.GetFile(pal_name,pal_data) || pal_data.size() != 3*256)
    {
        m_last_error = string_format("Missing file %s in source archive!",pal_name.c_str());
        return(1);
    }
    if(pal_data.size() != 3*256)
    {
        m_last_error = string_format("Invalid size of palette file %s!",pal_name.c_str());
        return(1);
    }
    uint8_t(*pal)[3] = (uint8_t(*)[3])pal_data.data();

    // load font
    std::vector<uint8_t> fontdata;
    if(arch.GetFile(font_name,fontdata))
    {
        m_last_error = string_format("Missing file %s in source archive!",font_name.c_str());
        return(1);
    }
    std::unique_ptr<SpellFont> font;
    try{
        font = std::make_unique<SpellFont>(fontdata.data(), fontdata.size());
    }catch(const std::runtime_error& error) {
        m_last_error = string_format("Failed loading font file %s!",font_name.c_str());
        return(1);
    }

    // parse command parameters
    int x_pos, y_pos;
    if(str2int(params[0],x_pos))
    {
        m_last_error = string_format("Invalid value of x_pos parameter \"%s\"!",params[0].c_str());
        return(1);
    }
    if(str2int(params[1],y_pos))
    {
        m_last_error = string_format("Invalid value of y_pos parameter \"%s\"!",params[1].c_str());
        return(1);    
    }
    auto align = params[2];
    bool is_center = false;
    if(iequals(align,"center"))
        is_center = true;
    else if(!iequals(align,"left"))
    {
        m_last_error = string_format("Invalid value of align parameter \"%s\"!",align.c_str());
        return(1);
    }
    std::vector<int> rgb_list(2);
    if(str2int(params[3],rgb_list[0],0,256*256*256-1,16))
    {
        m_last_error = string_format("Invalid value of color parameter \"%s\"! Must be hex number format RRGGBB.",params[2].c_str());
        return(1);
    }
    if(str2int(params[4],rgb_list[1],0,256*256*256-1,16))
    {
        m_last_error = string_format("Invalid value of color parameter \"%s\"! Must be hex number format RRGGBB.",params[2].c_str());
        return(1);
    }
    auto text = params[5];

    // replace option variables
    for(auto &opt: m_options)
    {
        auto key = string_format("%%%s%%",opt.label.c_str());
        std::string val = string_format("%d",opt.value);
        if(opt.isEnum())
            val = opt.GetEnumValue();
        text = strrep(text, key, val);
    }
    text = strrep(text,"%DATE%",get_local_time_str().c_str());

    // find nearest color index in palette
    std::vector<int> color_list;
    for(auto &rgb: rgb_list)
    {
        int rr = (rgb >> 16) & 0xFF;
        int gg = (rgb >> 8) & 0xFF;
        int bb = rgb & 0xFF;
        int min_dist = 256*256*3;
        int color = 0;
        for(int k = 0; k < 256; k++)
        {        
            int dist = ((int)pal[k][0] - rr)*((int)pal[k][0] - rr) + ((int)pal[k][1] - gg)*((int)pal[k][1] - gg) + ((int)pal[k][2] - bb)*((int)pal[k][2] - bb);
            if(dist < min_dist)
            {
                min_dist = dist;
                color = k;
            }
        }
        color_list.push_back(color);
    }

    // calc position
    int x_width = font->GetTextWidth(text);
    int y_font = font->GetHeight();
    if(is_center)
        x_pos = x_pos - x_width/2;
    if(x_pos < 0 || x_pos + x_width >= x_size)
    {
        m_last_error = string_format("Title text wont fit to given x-position!");
        return(1);
    }
    y_pos = y_pos - y_font/2;
    if(y_pos < 0 || y_pos + y_font >= y_size)
    {
        m_last_error = string_format("Title text wont fit to given y-position!");
        return(1);
    }
    
    // try render text
    font->Render(img.data(), img.data() + img.size(), x_size, x_pos, y_pos, text,color_list[0],color_list[1],SpellFont::FontShadow::DIAG2);
    
    // compress image back to LZ
    try{
        LZspell(img.data(), img.size(), lzdata);
    }catch(const std::runtime_error& error) {
        m_last_error = string_format("Failed compressing image %s!",img_name.c_str());
        return(1);
    }
    
    // replace original image
    if(arch.AddFile(lzdata, img_name, true))
    {
        m_last_error = string_format("Failed replacing image %s in archive!",img_name.c_str());
        return(1);
    }
    
    return(0);
}

// replace particular units from source
int SpellMod::ReplaceUnits(SpellArchive* dest,SpellArchive *src, std::string name, std::vector<int> &list)
{
    // try load source units
    std::vector<uint8_t> src_units;
    if(src->GetFile(name,src_units))
    {
        m_last_error = string_format("Source file %s not found in source archive!",name.c_str());
        return(1);
    }

    // try load destination units
    std::string dest_name = "JEDNOTKY.DEF";
    std::vector<uint8_t> dest_units;
    if(dest->GetFile(dest_name,dest_units))
    {
        m_last_error = string_format("Destination file %s not found in destination archive!",dest_name.c_str());
        return(1);
    }

    // identify CZ/EN version
    int size = -1;
    if(src_units.size() % 206 == 0 && dest_units.size() % 206 == 0)
        size = 206;
    else if(src_units.size() % 207 == 0 && dest_units.size() % 207 == 0)
        size = 207;
    else
    {
        m_last_error = string_format("Source/destination files %s have wrong size(s)!",name.c_str());
        return(1);
    }

    if(list.empty())
    {
        // copy all units
        dest_units = src_units;
    }
    else
    {
        // sopy selection
        for(auto id: list)
        {
            int ofs = id*size;
            int end = ofs + size;
            if(end > src_units.size() || end > dest_units.size())
            {
                m_last_error = string_format("Unit ID #%d out of range of source or destination data!",id);
                return(1);
            }
            std::memcpy(dest_units.data() + ofs, src_units.data() + ofs, size);
        }
    }
    
    // replace original image
    if(dest->AddFile(dest_units,dest_name,true))
    {
        m_last_error = string_format("Failed replacing file %s in archive!",dest_name.c_str());
        return(1);
    }

    return(0);
}

// replace particular units from source
int SpellMod::SwapUnits(SpellArchive* arch,std::pair<int,int> pair)
{
    // try load DEF file units
    std::string dest_name = "JEDNOTKY.DEF";
    std::vector<uint8_t> dest_units;
    if(arch->GetFile(dest_name,dest_units))
    {
        m_last_error = string_format("Definition file %s not found in destination archive!",dest_name.c_str());
        return(1);
    }

    // identify CZ/EN version
    int size = -1;
    if(dest_units.size() % 206 == 0)
        size = 206;
    else if(dest_units.size() % 207 == 0)
        size = 207;
    else
    {
        m_last_error = string_format("Definition file %s have wrong size(s)!",dest_name.c_str());
        return(1);
    }

    if(pair.first >= dest_units.size())
    {
        m_last_error = string_format("Unit A #%d out of range of definition data!",pair.first);
        return(1);
    }
    if(pair.second >= dest_units.size())
    {
        m_last_error = string_format("Unit B #%d out of range of definition data!",pair.second);
        return(1);
    }

    // swap records
    int ofs_a = pair.first*size;
    int ofs_b = pair.second*size;
    std::vector<uint8_t> temp_a(dest_units.begin() + ofs_a,dest_units.begin() + ofs_a + size);
    std::vector<uint8_t> temp_b(dest_units.begin() + ofs_b,dest_units.begin() + ofs_b + size);
    dest_units.erase(dest_units.begin() + ofs_a,dest_units.begin() + ofs_a + size);
    dest_units.insert(dest_units.begin()+ ofs_a, temp_b.begin(), temp_b.end());
    dest_units.erase(dest_units.begin() + ofs_b,dest_units.begin() + ofs_b + size);
    dest_units.insert(dest_units.begin()+ ofs_b,temp_a.begin(),temp_a.end());

    // replace original image
    if(arch->AddFile(dest_units,dest_name,true))
    {
        m_last_error = string_format("Failed replacing file %s in archive!",dest_name.c_str());
        return(1);
    }

    return(0);
}

// swap unit IDs within map definition files
int SpellMod::SwapMapUnits(std::string &def, SpellUnits *units, std::map<int,int> &swap_map_units_list)
{
    // parse to lines
    auto lines = get_text_lines(def);

    // leave because it's not mission DEF but no error
    if(lines.empty() || !lines[0].starts_with("MissionData"))
        return(0);
    
    // process all lines
    for(auto& line: lines)
    {
        if(line.starts_with("AddUnit") || line.starts_with("AddSpecialUnit"))
        {
            SpellDefCmd cmd(line);
            if(!cmd.valid)
            {
                // invalid command
                m_last_error = "Possibly somehow incomplete command AddUnit() or AddSpecialUnit()?";
                return(1);
            }
            if((cmd.name == "AddUnit" && cmd.parameters.size() != 7) || (cmd.name == "AddSpecialUnit" && cmd.parameters.size() != 6))
            {
                // invalid params count
                m_last_error = "Wrong parameters count for command AddUnit() or AddSpecialUnit().";
                return(1);
            }                 

            // check original unit type
            int orig_unit_type = std::atoi(cmd.parameters[1].c_str());
            auto orig_unit = units->GetUnit(orig_unit_type);
            if(!orig_unit)
            {
                // unknown unit type
                m_last_error = string_format("Unknown unit type for command \"%s\".",cmd.full_command.c_str());
                return(1);
            }

            // get original health
            double health = (double)std::atoi(cmd.parameters[4].c_str()) / (double)orig_unit->cnt;
            
            // try get new unit type based on provided swap pairs
            int new_unit_type = -1;
            auto it = swap_map_units_list.find(orig_unit_type);
            if(it == swap_map_units_list.end())
                it = std::find_if(swap_map_units_list.begin(),swap_map_units_list.end(),[orig_unit_type](const auto& item) {return(item.second == orig_unit_type);});
            if(it == swap_map_units_list.end())
                continue;
            if(it->first == orig_unit_type)
                new_unit_type = it->second;
            else
                new_unit_type = it->first;
            
            // randomize unit type			            
            auto unit = units->GetUnit(new_unit_type);
            if(!unit)
            {
                // random unit ID not found
                m_last_error = string_format("Unknown new unit type %d in unit swap function for command \"%s\".",new_unit_type,cmd.sub_full_command.c_str());
                return(1);
            }
            // fix health
            int unit_health = std::max((int)(std::min(health,1.0)*(double)unit->cnt),1);

            // rebuild unit command
            cmd.parameters[1] = string_format("%d",new_unit_type);
            cmd.parameters[4] = string_format("%d",unit_health);
            line = cmd.name + "(" + merge_text_lines(cmd.parameters,",") + ")";

            continue;
        }
    }

    // merge modified lines
    def = merge_text_lines(lines);

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
    std::vector<std::string> archive_names = {"COMMON.FS", "T11.FS", "PUST.FS", "DEVAST.FS", "TEXTS.FS", "SAMPLES.FS", "MUSIC.FS", "RESEARCH.FS", "INFO.FS", "SPEAKER.FS", "MOVIE.FS", "UNITS.FSU"};
    for(auto &arch_name: archive_names)
    {
        // target archive absolute path
        auto arch_path = make_dir / arch_name;
        bool is_common = arch_name == "COMMON.FS";

        // try find original archive path
        std::filesystem::path org_path = "";
        auto org_p_path = ParsePath(std::filesystem::path("%SPELLANY%") / "data" /arch_name);
        if(org_p_path.isValid())
        {
            if(std::filesystem::exists(org_p_path.path))
                org_path = org_p_path.path;
            else if(std::filesystem::exists(org_p_path.alt_path))
                org_path = org_p_path.alt_path;
        }

        // try load archive definition class        
        if(GetClass(m_def,arch_name,cmd_list))
            continue;

        PrintConsole(" - Building archive %s ... ",arch_name.c_str());

        // cleanup local variables
        ClearVars();

        // make blank archive
        SpellArchive arch;
        bool glob_replace = 0;
        bool is_optional = false;
        std::map<int,int> swap_map_units_list;

        // section exists
        for(auto& cmd: cmd_list)
        {
            std::string var_name;
            std::string var_value;
            std::string cmd_rest;
            std::vector<std::string> par_list;
            if(cmd.isAssign(var_name,var_value))
            {
                // var assign
                ReplaceVars(var_value);

                if(var_name == "replace")
                    glob_replace = std::atoi(var_value.c_str());
                else if(var_name == "optional")
                    is_optional = std::atoi(var_value.c_str());                
                else
                {
                    // define local variable
                    if(AddVar(var_name, var_value))
                    {
                        PrintConsole("failed! Line %d: cannot add local variable in command \"%s\". %s\n",cmd.m_line,cmd.m_raw.c_str(),m_last_error.c_str());
                        return(1);
                    }
                }
            }
            else if(cmd.isFunction(var_name, par_list,cmd_rest))
            {
                // function command

                // replace local variable
                for(auto &par: par_list)
                    ReplaceVars(par);

                if(var_name == "if")
                {
                    // process if() conditional
                    if(par_list.size() != 1)
                    {
                        PrintConsole("failed! Line %d: wrong condition for command \"%s\".\n",cmd.m_line,cmd.m_raw.c_str());
                        return(1);
                    }
                    bool result;
                    if(ParseExpression(par_list[0],result))
                    {
                        PrintConsole("failed! Line %d: parsing conditional failed for command \"%s\" (%s).\n",cmd.m_line,cmd.m_raw.c_str(),m_last_error.c_str());
                        return(1);
                    }
                    if(!result)
                        continue;                    
                    cmd.m_cmd = cmd_rest;
                    if(!cmd.isFunction(var_name,par_list,cmd_rest))
                    {
                        PrintConsole("failed! Line %d: missing or crippled conditional command \"%s\".\n",cmd.m_line,cmd.m_raw.c_str());
                        return(1);
                    }
                }

                if(var_name == "title")
                {
                    // make main menu title:
                    //   title(x_pos,y_pos,text_color,shadow_color,text)
                    if(!is_common)
                    {
                        PrintConsole("failed! Line %d: title() command must be placed in COMMON.FS archive.\n",cmd.m_line);
                        return(1);
                    }
                    if(MakeTitle(arch, par_list))
                    {
                        PrintConsole("failed! Line %d: command \"%s\" failed with error: %s\n",cmd.m_line,cmd.m_raw.c_str(),m_last_error.c_str());
                        return(1);
                    }
                }
                else if(var_name == "rem")
                {
                    // remove file(s) from archive:
                    //   rem(wild_card)
                    if(par_list.size() != 1)
                    {
                        PrintConsole("failed! Line %d: wrong parameters count for command \"%s\".\n",cmd.m_line,cmd.m_raw.c_str());
                        return(1);
                    }

                    // list to remove
                    std::vector<std::string> list;
                    for(auto &file: arch.GetItemNames())
                        if(wildcmp(par_list[0],file))
                            list.push_back(file);

                    // now try remove
                    for(auto &name: list)
                        if(arch.RemoveFile(name,true))
                        {
                            PrintConsole("failed! Line %d: removing archive resource \"%s\" for command \"%s\".\n",cmd.m_line,name.c_str(),cmd.m_raw.c_str());
                            return(1);
                        }
                }
                else if(var_name == "copyunits")
                {
                    // copy units from source file to JEDNOTKY.DEF with optional filter
                    //   copyunits(source_archive,source_file_name,optional_list)
                    if(!is_common)
                    {
                        PrintConsole("failed! Line %d: copyunits() command must be placed in COMMON.FS archive.\n",cmd.m_line);
                        return(1);
                    }                    
                    if(par_list.size() < 2 || par_list.size() > 3)
                    {
                        PrintConsole("failed! Line %d: wrong params count in command \"%s\".\n",cmd.m_line,cmd.m_raw.c_str());
                        return(1);
                    }
                    auto src_path = par_list[0];
                    auto src_name = par_list[1];
                    std::vector<int> unit_list;
                    if(par_list.size() == 3)
                    {
                        auto toks = regexp_get(par_list[2],"^\\{(.*)\\}$");
                        if(toks.size() != 1)
                        {
                            PrintConsole("failed! Line %d: wrong format of units list in command \"%s\". Must be semicolon list in {}.\n",cmd.m_line,cmd.m_raw.c_str());
                            return(1);
                        }
                        toks = str_split(toks[0],';',true);
                        for(auto &vv: toks)
                        {
                            int val;
                            if(str2int(vv,val,0))
                            {
                                PrintConsole("failed! Line %d: wrong format of units list in command \"%s\". Must be semicolon list in {}.\n",cmd.m_line,cmd.m_raw.c_str());
                                return(1);
                            }
                            unit_list.push_back(val);
                        }                        
                    }

                    // parse source path (expand vars)                    
                    auto parsed_path = ParsePath(src_path);
                    if(!parsed_path.isValid())
                    {
                        // parsing path for add() command failed
                        PrintConsole("failed! Line %d: parsing source path failed in command \"%s\".\n",cmd.m_line,cmd.m_raw.c_str());
                        return(1);
                    }

                    // try load source archive
                    auto src = LoadArchive(parsed_path,SpellArchive::Type::FS);
                    if(!src)
                    {
                        // loading source data for add() command failed
                        PrintConsole("failed! Line %d: loading source data failed in command \"%s\".\n %s\n",cmd.m_line,cmd.m_raw.c_str(),m_last_error.c_str());
                        return(1);
                    }                                                       

                    // try replace units
                    if(ReplaceUnits(&arch,src,src_name,unit_list))
                    {
                        PrintConsole("failed! Line %d: copying units data in command \"%s\".\n %s\n",cmd.m_line,cmd.m_raw.c_str(),m_last_error.c_str());
                        return(1);
                    }
                    
                }
                else if(var_name == "swapmapunits")
                {
                    // swap units IDs in all maps
                    //   swapmapunits(unit_a,unit_b)
                    if(!is_common)
                    {
                        PrintConsole("failed! Line %d: swapmapunits() command must be placed in COMMON.FS archive.\n",cmd.m_line);
                        return(1);
                    }
                    if(par_list.size() != 2)
                    {
                        PrintConsole("failed! Line %d: wrong params count in command \"%s\".\n",cmd.m_line,cmd.m_raw.c_str());
                        return(1);
                    }
                    std::pair<int,int> pair;
                    if(str2int(par_list[0],pair.first,0,89) || str2int(par_list[1],pair.second,0,89))
                    {
                        PrintConsole("failed! Line %d: wrong param values in command \"%s\".\n",cmd.m_line,cmd.m_raw.c_str());
                        return(1);
                    }
                    // just place swap pairs to the list
                    swap_map_units_list.insert(pair);                    

                }
                else if(var_name == "swapunits")
                {
                    // swap units records within JEDNOTKY.DEF file
                    //   swapunits(unit_a,unit_b)
                    if(!is_common)
                    {
                        PrintConsole("failed! Line %d: swapunits() command must be placed in COMMON.FS archive.\n",cmd.m_line);
                        return(1);
                    }
                    if(par_list.size() != 2)
                    {
                        PrintConsole("failed! Line %d: wrong params count in command \"%s\".\n",cmd.m_line,cmd.m_raw.c_str());
                        return(1);
                    }
                    std::pair<int,int> pair;
                    if(str2int(par_list[0],pair.first,0,89) || str2int(par_list[1],pair.second,0,89))
                    {
                        PrintConsole("failed! Line %d: wrong param values in command \"%s\".\n",cmd.m_line,cmd.m_raw.c_str());
                        return(1);
                    }
                    if(SwapUnits(&arch, pair))
                    {
                        PrintConsole("failed! Line %d: swaping unit recirds data in command \"%s\".\n %s\n",cmd.m_line,cmd.m_raw.c_str(),m_last_error.c_str());
                        return(1);
                    }

                }
                else if(var_name == "copy")
                {
                    // copy single file with renaming: 
                    //   copy(source_archive,source_file_name,dest_file_name)
                    //   copy(ADD,source_archive,source_file_name,dest_file_name)
                    //   copy(NEW,source_archive,source_file_name,dest_file_name)

                    std::string mode = (glob_replace)?"ALL":"NEW";
                    if(par_list.size() > 4 || par_list.size() < 3)
                    {
                        // wrong parameters count for add() command
                        PrintConsole("failed! Line %d: wrong params count is command \"%s\".\n",cmd.m_line,cmd.m_raw.c_str());
                        return(1);
                    }
                    if(par_list.size() == 4)
                    {
                        if(par_list[0] == "NEW" || par_list[0] == "ADD")
                            mode = par_list[0];
                        else
                        {
                            // unknown mode parameter for add() command
                            PrintConsole("failed! Line %d: unknown copy mode \"%s\" in command \"%s\".\n",cmd.m_line,mode.c_str(),cmd.m_raw.c_str());
                            return(1);
                        }
                        par_list.erase(par_list.begin() + 0);
                    }
                    auto path = par_list[0];
                    auto src_name = par_list[1];
                    auto dest_name = par_list[2];
                    bool replace = (mode == "ALL");

                    // parse source path (expand vars)                    
                    auto parsed_path = ParsePath(path);
                    if(!parsed_path.isValid())
                    {
                        // parsing path for add() command failed
                        PrintConsole("failed! Line %d: parsing source path failed in command \"%s\".\n",cmd.m_line,cmd.m_raw.c_str());
                        return(1);
                    }
                    
                    // try load source archive
                    SpellArchive::Type arch_type = SpellArchive::Type::AUTO;
                    if(wildcmp("*.FSU",arch_name.c_str()))
                        arch_type = SpellArchive::Type::FSU;
                    if(!std::filesystem::exists(parsed_path.path) && !std::filesystem::exists(parsed_path.alt_path) && is_optional)
                        continue; // optional mode: skip
                    auto src = LoadArchive(parsed_path,arch_type);
                    if(!src)
                    {
                        // loading source data for add() command failed
                        PrintConsole("failed! Line %d: loading source data failed in command \"%s\".\n%s\n",cmd.m_line,cmd.m_raw.c_str(),m_last_error.c_str());
                        return(1);
                    }
                                                           
                    // try copy file
                    if(arch.AddFile(*src,src_name,replace,dest_name))
                    {
                        // adding file to archive in add() command failed
                        PrintConsole("failed! Line %d: copying file \"%s\" to \"%s\" in command \"%s\".\n%s\n",cmd.m_line,src_name.c_str(),dest_name.c_str(),cmd.m_raw.c_str(),arch.GetLastError().c_str());
                        return(1);
                    }
                }
                else if(var_name == "add")
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
                    if(!std::filesystem::exists(parsed_path.path) && !std::filesystem::exists(parsed_path.alt_path) && is_optional)
                        continue; // optional mode: skip
                    auto src = LoadArchive(parsed_path,arch_type);
                    if(!src)
                    {
                        // loading source data for add() command failed
                        PrintConsole("failed! Line %d: loading source data failed in command \"%s\".\n%s\n",cmd.m_line,cmd.m_raw.c_str(),m_last_error.c_str());
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
                            PrintConsole("failed! Line %d: adding file \"%s\" in command \"%s\".\n%s\n",cmd.m_line,name.c_str(),cmd.m_raw.c_str(),arch.GetLastError().c_str());
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

        // units randomizer and/or swapper?
        if(is_common && (config.randomize || !swap_map_units_list.empty()))
        {
            // parse units definition
            std::vector<uint8_t> data;
            if(arch.GetFile("JEDNOTKY.DEF",data))
            {
                // unknown command
                PrintConsole("failed! Unit randomizer cannot find file JEDNOTKY.DEF in COMMON.FS.\n");
                return(1);
            }
            std::unique_ptr<SpellUnits> units;
            try{
                units = std::make_unique<SpellUnits>(data.data(), data.size());
            }catch(const std::runtime_error& error) {
                PrintConsole(string_format("failed! Unit randomizer cannot decode file JEDNOTKY.DEF in COMMON.FS.\n"));
                return(1);
            }
            
            // for each possible map script:
            for(auto& name: arch.GetItemNames())
            {
                std::string key = "M??_*.DEF";
                if(!wildcmp(key,name))
                    continue;
                if(arch.GetFile(name,data))
                {
                    // unknown command
                    PrintConsole("failed! Unit randomizer cannot load file \"%s\" in COMMON.FS.\n",name.c_str());
                    return(1);
                }
                std::string def(data.begin(),data.end());

                // try randomize?
                if(config.randomize)
                {
                    std::string errstr;
                    if(UnitRandomizer::RandomizeMap(def,units.get(),errstr))
                    {
                        PrintConsole("failed! Unit randomizer modifying \"%s\" failed: %s\n",name.c_str(),errstr.c_str());
                        return(1);
                    }
                }

                // swap units?
                if(!swap_map_units_list.empty())
                {
                    if(SwapMapUnits(def,units.get(),swap_map_units_list))
                    {
                        PrintConsole("failed! Unit swap in \"%s\" failed: %s\n",name.c_str(),m_last_error.c_str());
                        return(1);
                    }
                }

                // replace
                if(arch.AddFile(def,name,true))
                {
                    PrintConsole("failed! Unit randomizer modifying \"%s\" failed: %s\n",name.c_str(),arch.GetLastError().c_str());
                    return(1);
                }
            }            
        }


        

        // --- save archive:
        // check if target archive differs from newly built one
        bool must_write = config.force_write && !arch.isEmpty();
        if(!must_write && !arch.isEmpty() && std::filesystem::exists(arch_path))
        {            
            // try load archive from target path            
            SpellArchive ref_arch;
            if(ref_arch.Load(arch_path))
            {
                // archive loading failed
                PrintConsole("failed! Loading target archive \"%ls\".\n%s\n",arch_path.wstring().c_str(),ref_arch.GetLastError().c_str());
                return(1);
            }
            // compare by content
            must_write |= !arch.Compare(ref_arch);            
        }
        if(!must_write && !arch.isEmpty() && std::filesystem::exists(org_path) && !std::filesystem::exists(arch_path))
        {
            // try load archive from spellcross target path
            SpellArchive ref_arch;
            if(ref_arch.Load(org_path))
            {
                // archive loading failed
                PrintConsole("failed! Loading target archive \"%ls\".\n%s\n",org_path.wstring().c_str(),ref_arch.GetLastError().c_str());
                return(1);
            }
            // compare by content
            must_write |= !arch.Compare(ref_arch);
        }
        if(!must_write && !arch.isEmpty() && !std::filesystem::exists(arch_path) && org_path.empty())
        {
            // force write if archive not present in-game
            must_write =  true;
        }
        if(!must_write)
        {
            //PrintConsole("done at \"%ls\" (no change).\n",arch_path.wstring().c_str());
            PrintConsole("done (not saving: no change detected).\n");
            continue;
        }
        if(arch.isEmpty())
        {
            PrintConsole("done (not saving: empty archive).\n");
            continue;
        }

        // try save archive
        if(arch.Save(arch_path, true))
        {
            // writting archive failed
            PrintConsole("failed! Saving target archive \"%ls\".\n%s\n",arch_path.wstring().c_str(),arch.GetLastError().c_str());
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
        bool has_original = !where_original.empty();

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
                
        if(has_original && !std::filesystem::exists(where_original))
        {
            PrintConsole("failed! Original game archive path not exist (%ls).\n",where_original.wstring().c_str());
            was_error = true;
            continue;
        }
                
        // move original from temp back to game location
        if(has_original && fs_rename(where_original,location))
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
    /*std::vector<std::filesystem::path> spell_dirs = {config.spell_dir};
    if(config.allow_cd_mod && !config.spellcd_dir.empty())
        spell_dirs.push_back(config.spellcd_dir);      */

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
        arch.dest = config.spell_dir / "DATA" / name;
        auto cd_dest = config.spellcd_dir / "DATA" / name;
        if(!config.spellcd_dir.empty() && config.allow_cd_mod && std::filesystem::exists(cd_dest))
            arch.dest = cd_dest;        
        /*if(arch.dest.empty())
        {            
            continue;
            //return(1);
        }*/

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
        bool has_orig = std::filesystem::exists(arch.dest);

        PrintConsole(" - Replacing archive %s ... ", arch.name.c_str());
        
        // move original to temp
        if(has_orig && fs_rename(arch.dest,temp_path))
        {
            PrintConsole("failed! Moving original (%ls) to temp location (%ls).\n",arch.dest.wstring().c_str(), temp_path.wstring().c_str());            
            was_error = true;
            break;
        }
        ini.SetBoolValue(arch.name.c_str(),"modified",true);
        ini.SetValue(arch.name.c_str(),"location",arch.dest.string().c_str());
        if(has_orig)
            ini.SetValue(arch.name.c_str(),"where_original",temp_path.string().c_str());
        else
            ini.SetValue(arch.name.c_str(),"where_original","");
        ini.SetValue(arch.name.c_str(),"mod_from","");

        // move mod to original
        if(fs_rename(arch.source,arch.dest))
        {
            PrintConsole("failed! Moving mod (%ls) to game location (%ls).\n",arch.source.wstring().c_str(),arch.dest.wstring().c_str());
            was_error = true;
            break;
        }
        ini.SetValue(arch.name.c_str(),"mod_from",arch.source.string().c_str());
                
        PrintConsole("done.\n");
    }
    
    // swap save games?
    if(!was_error && config.move_saves)
    {        
        auto save_dir = config.spell_dir / "SAVE";
        auto mod_save_dir = mod_dir / "save";
        auto save_temp_dir = config.spell_dir / "spellcross_mod_builder_orig_save_temp";
        
        PrintConsole(" - Replacing SAVE games ...");
        if(!std::filesystem::exists(mod_save_dir))
        {
            // no save games in mod folder yet: make a copy of current game saves
            if(fs_copy(save_dir,mod_save_dir,std::filesystem::copy_options::recursive))
            {
                PrintConsole("failed! Making initial copy of games saves (%ls) to mod location (%ls).\n",mod_save_dir.wstring().c_str(),save_dir.wstring().c_str());
                was_error = true;
            }
            else
                MakeSaveIni(mod_save_dir,"Initial copy of original Spellcross SAVE folder");
        }
                
        
        // now try to swap saves
        if(!was_error && fs_rename(save_dir,save_temp_dir))
        {
            PrintConsole("failed! Moving original saves (%ls) to temp location (%ls).\n",save_dir.wstring().c_str(),save_temp_dir.wstring().c_str());
            was_error = true;
        }
        else if(!was_error)
        {
            MakeSaveIni(save_temp_dir,"Temporary copy of original game SAVE folder");

            ini.SetBoolValue("SAVE","modified",true);
            ini.SetValue("SAVE","location",save_dir.string().c_str());
            ini.SetValue("SAVE","where_original",save_temp_dir.string().c_str());
            ini.SetValue("SAVE","mod_from","");     

            MakeSaveIni(mod_save_dir,string_format("Save games of mod \"%ls\"",config.mod_path.wstring().c_str()));
                        
            if(fs_rename(mod_save_dir,save_dir))
            {
                PrintConsole("failed! Moving mod saves (%ls) to game location (%ls).\n",mod_save_dir.wstring().c_str(),save_dir.wstring().c_str());
                was_error = true;
            }
            else
            {
                ini.SetValue("SAVE","mod_from",mod_save_dir.string().c_str());            
            }
        }

        if(!was_error)
            PrintConsole(" done\n");
        
    }

    // save state
    ini.SaveFile(config.state_ini_path.wstring().c_str());
    if(was_error)
        PrintConsole(" - Game modification failed!\n");
    else
        PrintConsole(" - Game mod done!\n");

    return(was_error);
}

// cleanup mod state file
int SpellMod::CleanupMod(Config& config)
{
    PrintConsole("Cleaun up mod state file ... ");
    
    CSimpleIniA ini;
    ini.SetUnicode(true);
    if(ini.LoadFile(config.state_ini_path.wstring().c_str()) != SI_OK)
    {
        PrintConsole("done (no state file found)!\n");
        return(0); // nothing to restore
    }

    ini.Reset();
    ini.SaveFile(config.state_ini_path.wstring().c_str());

    PrintConsole("done!\n");
        
    return(0);
}


// write save.ini to given folder
int SpellMod::MakeSaveIni(std::filesystem::path save_dir, std::string description)
{
    CSimpleIniA save_ini;
    save_ini.SetValue("SAVE","what_is_it","Set of Spellcross save games (SAVE folder) moved/copied by Spellcross Mod Builder","; Info");
    save_ini.SetValue("SAVE","description",description.c_str(),"; Save games set description");
    save_ini.SetValue("SAVE","date",get_local_time_str().c_str(),"; Timestamp of last move of this saves set");
    auto save_ini_path = save_dir / "save.ini";
    return(save_ini.SaveFile(save_ini_path.wstring().c_str()));
}

// read save.ini in save folder
int SpellMod::GetSaveIni(std::filesystem::path save_dir,SaveInfo& info)
{    
    info.description = "";
    info.time = "";

    auto save_ini_path = save_dir / "save.ini";
    if(save_ini_path.empty())
        return(1);
    CSimpleIniA save_ini;
    save_ini.LoadFile(save_ini_path.wstring().c_str());

    auto desc = save_ini.GetValue("SAVE","description");
    if(desc)
        info.description = desc;

    auto date = save_ini.GetValue("SAVE","date");
    if(date)
        info.time = date;

    return(0);
}

// make ini with mod options values
int SpellMod::MakeOptionsIni(std::filesystem::path ini_path,std::vector<SpellModOption>& options)
{    
    CSimpleIniA ini;
    for(auto &opt: options)
        ini.SetLongValue("OPTIONS",opt.label.c_str(),opt.value,string_format("; %s",opt.description.c_str()).c_str());
    return(ini.SaveFile(ini_path.wstring().c_str()));
}

// load option values from ini (if exist)
int SpellMod::LoadOptionsIni(std::filesystem::path ini_path,std::vector<SpellModOption>& options)
{
    CSimpleIniA ini;
    ini.LoadFile(ini_path.wstring().c_str());
    CSimpleIniA save_ini;
    for(auto& opt: options)
        opt.value = ini.GetLongValue("OPTIONS",opt.label.c_str(),opt.value);        
    return(0);
}



