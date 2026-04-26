#pragma once

#include <string>
#include <filesystem>
#include <functional>

class SpellLaunch
{
private:
    static const std::string box_exec_name;
    static const std::string box_setup_name;
    static const std::string run_dosbox_name;
    static const std::string setup_dosbox_name;
    static const std::string run_win32_name;
    static const std::string setup_win32_name;

public:
    static int RunGame(std::filesystem::path spell_dir,std::string run_mode,std::function<void(std::string)> console_cb=NULL);
    static int RunSetup(std::filesystem::path spell_dir,std::string run_mode,std::function<void(std::string)> console_cb=NULL);
    
    static int MakeSpellCD(std::filesystem::path spell_dir,std::string drive);
    static int MakeSpellDOSboxSetupBat(std::filesystem::path spell_dir);
    static int MakeSpellDOSboxExecBat(std::filesystem::path spell_dir,std::filesystem::path spellcd_dir,std::string drive,std::string spell_exe);
    static int MakeSpellDOSboxRunBat(std::filesystem::path spell_dir,std::filesystem::path dosbox_path,bool no_autoexec,bool fullscreen);
    static int MakeSpellDOSboxRunSetupBat(std::filesystem::path spell_dir,std::filesystem::path dosbox_path);
    static int MakeSpellWin32RunBat(std::filesystem::path spell_dir,std::filesystem::path spellcd_dir,std::string drive,std::string spell_exe);


};


