//=============================================================================
// Spellcross installer.
// It loads INSTALL.DTA and generates Spellcross game folder from its content.
// 
// This code is part of Spellcross Mod Launcher.
// (c) 2026, Stanislav Maslan, s.maslan@seznam.cz
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================

#pragma once

#include <filesystem>

class SpellInstall{
public:
    static std::string m_last_error;
    static int InstallDTA(std::filesystem::path dta_path, std::filesystem::path target_dir);
};
