#pragma once

#include <filesystem>
#include <string>

class SpellSave{
public:

    class Save{
    public:
        std::string dir_name;
        std::filesystem::path dir_path;
        std::string name;
        std::string date;
        bool is_empty;
    };
    typedef std::vector<Save> Saves;

    static int LoadSave(std::filesystem::path dir,Save &save,bool allow_empty=true);
    static int LoadSaves(std::filesystem::path dir, Saves &saves,bool skip_missing=true);
};
