//=============================================================================
// Spellcross DEF file parsing stuff
// 
// This code is part of Spellcross Map Editor project.
// (c) 2026, Stanislav Maslan, s.maslan@seznam.cz
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================
#pragma once

#include <vector>
#include <string>
#include <filesystem>

//using namespace std;

class SpellDefCmd
{
	public:
		// command name string
		std::string name;		
		std::vector<std::string> parameters;
		std::string comment;
		std::string full_command;
		bool valid;

		std::string sub_name;
		std::vector<std::string> sub_params;
		std::string sub_full_command;
		bool sub_valid;

		SpellDefCmd(std::string command, std::string params, std::string comment);
		SpellDefCmd(std::string line);
		~SpellDefCmd();
};

class SpellDefSection
{
private:
	std::vector<SpellDefCmd*> list;

public:
	SpellDefSection();
	~SpellDefSection();
		
	void Add(SpellDefCmd* cmd);
	int Size();
	SpellDefCmd* operator[](int index);
	std::vector<SpellDefCmd*> &GetData();
};

class SpellDEF
{
	private: 
		std::string data;

	public:
		SpellDEF(std::string &str);
		SpellDEF(std::filesystem::path path);
		SpellDEF(uint8_t *data, int size);
		~SpellDEF();

		SpellDefSection *GetSection(std::string section);
};

