#include "SpellSaves.h"
#include "other.h"
#include <format>

/*template <typename TP>
std::time_t to_time_t(TP tp)
{
    using namespace std::chrono;
    auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now()
        + system_clock::now());
    return system_clock::to_time_t(sctp);
}*/

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
    auto err = loadstr(path,save.name);
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