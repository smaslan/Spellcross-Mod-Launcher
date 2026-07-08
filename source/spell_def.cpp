//=============================================================================
// Spellcross DEF file parsing stuff
// 
// This code is part of Spellcross Map Editor project.
// (c) 2026, Stanislav Maslan, s.maslan@seznam.cz
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================
#include "spell_def.h"
#include "other.h"

#include <string>
#include <vector>
#include <stdexcept>
#include <regex>
//#include <filesystem>

//using namespace std;

//=============================================================================
// class SpellDEF - DEF files parser
//=============================================================================
SpellDefCmd::SpellDefCmd(std::string command, std::string params, std::string comment)
{
	name = command;
	this->comment = comment;
	this->full_command = command + "(" + params + ")";	
	if(!params.empty())
		this->parameters = get_text_lines(params,true,',');
}
// faster parser of full command line without whitespaces
SpellDefCmd::SpellDefCmd(std::string line)
{
	valid = false;
	full_command = line;
	if(line.empty() || line[0] == ';')
		return;

	// params list start
	auto param = line.find('(');
	if(param == std::string::npos || param < 1)
		return;

	// command name
	name = line.substr(0, param);

	// params list end
	auto param_end = line.find(')',param);
	if(param_end == std::string::npos)
		return;

	// parse command parameters
	auto params = line.substr(param + 1, param_end - param - 1);
	parameters = get_text_lines(params,true,',');
	full_command = name + "(" + params + ")";
	valid = true;

	// check eventual sub-command
	auto sub_cmd_start = line.find(',',param_end);
	if(sub_cmd_start == std::string::npos || sub_cmd_start + 1 >= line.length())
		return;

	// params list start
	param = line.find('(', sub_cmd_start + 1);
	if(param == std::string::npos || param < sub_cmd_start + 2)
		return;

	// sub-command name
	sub_name = line.substr(sub_cmd_start + 1, param - sub_cmd_start - 1);

	// params list end
	param_end = line.find(')',sub_cmd_start);
	if(param_end == std::string::npos)
		return;

	// parse command parameters
	params = line.substr(param + 1,param_end - param - 1);
	sub_params = get_text_lines(params,true,',');		
	sub_full_command = sub_name + "(" + params + ")";
	sub_valid = true;
	
}
SpellDefCmd::~SpellDefCmd()
{	
}

// --- section of DEF file
SpellDefSection::SpellDefSection()
{
}
SpellDefSection::~SpellDefSection()
{
	for(unsigned k = 0; k < list.size(); k++)
		delete list[k];
	list.clear();
}
// get section items count
int SpellDefSection::Size()
{
	return(list.size());
}
// add item
void SpellDefSection::Add(SpellDefCmd *cmd)
{
	list.push_back(cmd);
}
// get item
SpellDefCmd* SpellDefSection::operator[](int index)
{
	if(index < 0 || index > list.size())
		return(NULL);
	return(list[index]);
}
// get all sections
std::vector<SpellDefCmd*>& SpellDefSection::GetData()
{
	return(list);
}



// construct from file
SpellDEF::SpellDEF(std::wstring &path)
{
	// try open file
	if (loadstr(std::filesystem::path(path),data))
		throw std::runtime_error("Cannot open DEF file!");
}
// construct from data buffer
SpellDEF::SpellDEF(uint8_t* data, int size)
{
	this->data.resize(size);
	std::memcpy((void*)this->data.data(), (void*)data, size);
}
// construct from string
SpellDEF::SpellDEF(std::string &str)
{
	data = str;
}
// destroy
SpellDEF::~SpellDEF()
{
	data.clear();
}

// returns vector of commands in given section
SpellDefSection *SpellDEF::GetSection(std::string section)
{	
	// split to lines
	auto lines = get_text_lines(data);

	// try find section
	int first_row = -1;
	int last_row = -1;
	for(auto &line: lines)
	{
		if(first_row < 0 && line.starts_with(section))
			first_row = &line - lines.data() + 1;
		if(first_row >= 0 && line.starts_with("}"))
		{
			last_row = &line - lines.data();
			break;
		}		
	}
	if(first_row < 0 || last_row < 0)
		return(NULL);
	lines = std::vector<std::string>(lines.data() + first_row, lines.data() + last_row);

	SpellDefSection* sec_data = new SpellDefSection();
	for(auto &line: lines)
	{
		auto tokens = regexp_get(line, "^([^;]+?)\\(([^\\)]*)\\)((?:,)([^;\n]+))?");
		if(tokens.size() < 2)
			continue;
		
		// parse basic command
		SpellDefCmd* cmd = new SpellDefCmd(tokens[0],tokens[1],"");
		sec_data->Add(cmd);

		if(tokens.size() < 4 || tokens[3].empty())
			continue;

		// try parse sub-sommand
		tokens = regexp_get(tokens[3],"^([^;]+?)\\(([^\\)]*)\\)");
		if(tokens.size() < 2)
			continue;
		cmd->sub_name = tokens[0];
		if(!tokens[1].empty())
			cmd->sub_params  = get_text_lines(tokens[1],true,',');							
		cmd->sub_full_command = cmd->sub_name + "(" + tokens[1] + ")";
	}			

	return(sec_data);
}
