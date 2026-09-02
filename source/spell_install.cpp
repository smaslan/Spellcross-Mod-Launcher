//=============================================================================
// Spellcross installer.
// It loads INSTALL.DTA and generates Spellcross game folder from its content.
// 
// This code is part of Spellcross Mod Launcher.
// (c) 2026, Stanislav Maslan, s.maslan@seznam.cz
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================

#include "spell_install.h"
#include "other.h"
#include <vector>
#include <fstream>
#include <string>

std::string SpellInstall::m_last_error;

// install game files from INSTALL.DTA archive
int SpellInstall::InstallDTA(std::filesystem::path dta_path,std::filesystem::path target_dir)
{
    m_last_error = "";

    ifstreamext fr(dta_path,std::ios::in | std::ios::binary);
    if(!fr)
    {
        m_last_error = string_format("Opening file \"%ls\" failed!",dta_path.wstring().c_str());
        return(1);
    }

    // check file version
    auto ver = fr.read_u32();
    if(!fr.is_ok())
    {
        m_last_error = string_format("Reading file \"%ls\" failed! Possibly corrupted file.",dta_path.wstring().c_str());
        return(1);
    }
    if(ver != 0x12)
        return(1);

    // directories count
    auto dir_count = fr.read_u32();
    if(!fr.is_ok())
    {
        m_last_error = string_format("Reading file \"%ls\" failed! Possibly corrupted file.",dta_path.wstring().c_str());
        return(1);
    }
    
    // read dir list, make dirs
    std::vector<std::filesystem::path> dir_paths;
    for(int k = 0; k < dir_count; k++)
    {
        auto dir = fr.read_str_null();
        if(!fr.is_ok())
        {
            m_last_error = string_format("Reading file \"%ls\" failed! Possibly corrupted file.",dta_path.wstring().c_str());
            return(1);
        }

        // abs path
        auto dir_path = target_dir / dir;
        dir_paths.push_back(dir_path);

        // try create it
        try{
            std::filesystem::create_directories(dir_path);
        }catch(const std::runtime_error& error) {            
            m_last_error = string_format("Creating directory \"%ls\" failed!",dir_path.wstring().c_str());
            return(1);
        }
    }

    // files count
    auto file_count = fr.read_u32();
    if(!fr.is_ok())
        return(1);

    // extract files
    for(int k = 0; k < file_count; k++)
    {
        // file name
        auto name = fr.read_str_fixed(13);
        if(!fr.is_ok())
        {
            m_last_error = string_format("Reading file \"%ls\" failed! Possibly corrupted file.",dta_path.wstring().c_str());
            return(1);
        }
        
        // data offset
        auto ofs = fr.read_u32();
        if(!fr.is_ok())
        {
            m_last_error = string_format("Reading file \"%ls\" failed! Possibly corrupted file.",dta_path.wstring().c_str());
            return(1);
        }

        // data size
        auto size = fr.read_u32();
        if(!fr.is_ok())
        {
            m_last_error = string_format("Reading file \"%ls\" failed! Possibly corrupted file.",dta_path.wstring().c_str());
            return(1);
        }

        // dir index
        auto dir_id = fr.read_u32();
        if(!fr.is_ok())
        {
            m_last_error = string_format("Reading file \"%ls\" failed! Possibly corrupted file.",dta_path.wstring().c_str());
            return(1);
        }
        if(dir_id >= dir_paths.size())
        {
            m_last_error = string_format("Reading file \"%ls\" failed! Possibly corrupted file.",dta_path.wstring().c_str());
            return(1);
        }

        // target path
        auto path = dir_paths[dir_id] / name;

        // try read data
        auto pos = fr.tellg();
        fr.seekg(ofs);
        auto data = fr.read_vector(size);
        if(!fr.is_ok())
        {
            m_last_error = string_format("Reading file \"%ls\" failed! Possibly corrupted file.",dta_path.wstring().c_str());
            return(1);
        }
        fr.seekg(pos);

        // try save file
        if(savedata(path,data))
        {
            m_last_error = string_format("Writing file \"%ls\" failed!",path.wstring().c_str());
            return(1);
        }
    }

    // try make save directories if not exists
    auto save_dir = target_dir / "SAVE";
    if(!std::filesystem::exists(save_dir))
        std::filesystem::create_directory(save_dir);
    std::vector<std::filesystem::path> save_dirs;
    for(int k = 0; k <= 8; k++)
        save_dirs.push_back(save_dir / string_format("SAVE%04d",k));
    save_dirs.push_back(save_dir / "WORKDIR");
    for(auto &dir: save_dirs)
        if(!std::filesystem::exists(dir))
            std::filesystem::create_directory(dir);

    return(0);
}