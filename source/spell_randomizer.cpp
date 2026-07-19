//=============================================================================
// Spellcross units randomizer stuff
// 
// This code is part of Spellcross Map Editor project.
// (c) 2026, Stanislav Maslan, s.maslan@seznam.cz
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================
#include "spell_randomizer.h"
#include "other.h"
#include "spell_def.h"
#include "spell_units.h"


// init auto randomizer
UnitRandomizer::UnitRandomizer()
{
	last_error = "";
	rules.clear();
}

void UnitRandomizer::Clear()
{
	last_error = "";
	rules.clear();
}

// build DEF section with randomizer rules
std::string UnitRandomizer::MakeRulesDEF(std::string indent)
{
	std::string def;
	for(auto &rule: rules)
	{
		if(rule.rand_units.empty())
			continue;		
		def += indent + "AutoRandomizeRule(";
		def += string_format("%d,",rule.ref_unit);
		for(int k = 0; k < rule.rand_units.size(); k++)
		{
			def += string_format("%d",rule.rand_units[k]);
			if(k + 1 < rule.rand_units.size())
				def += ",";
		}
		def += ")\n";
	}
	return(def);
}

// try parse auto randomizer definition command
int UnitRandomizer::AddRule(SpellDefCmd *cmd, SpellUnits *units)
{
	last_error = "";
	if(!units)
		return(1);
	
	if(cmd->parameters.size() < 2)
	{
		last_error = string_format("Not enough parameters for auto randomzier command '%s'!",cmd->full_command.c_str());
		return(1);
	}

	// parse rule
	UnitRandomizerRule rule;
	for(int k = 0; k < cmd->parameters.size(); k++)
	{
		auto& unit_id_str = cmd->parameters[k];
		char* send;
		auto unit_type_id = std::strtol(unit_id_str.c_str(),&send,10);
		auto rand_unit = units->GetUnit(unit_type_id);
		if(!rand_unit)
		{
			last_error = string_format("Unit type '%s' not recognized for auto randomzier command '%s'!",unit_id_str.c_str(),cmd->full_command.c_str());
			return(1);
		}
		if(k == 0)
			rule.ref_unit = unit_type_id;
		else
		{
			if(std::find(rule.rand_units.begin(),rule.rand_units.end(),unit_type_id) != rule.rand_units.end())
			{
				last_error = string_format("Unit type '%d' already in auto randomzier random units list! Command '%s'!",unit_type_id,cmd->full_command.c_str());
				return(1);
			}
			rule.rand_units.push_back(unit_type_id);
		}
	}

	// check duplicate rules
	for(auto &rr: rules)
		if(rule.ref_unit == rr.ref_unit)
		{
			last_error = string_format("Reference unit type '%d' already in auto randomzier rules list! Command '%s'!",rule.ref_unit,cmd->full_command.c_str());
			return(1);
		}

	rules.push_back(rule);
	SortRules();
	return(0);
}

// try parse auto randomizer definition command
int UnitRandomizer::AddRule(SpellUnits* units,int& new_pos)
{
	last_error = "";
	new_pos = -1;
	if(!units)
		return(1);	
	
	// select first unused unit type as reference
	std::vector<int> list;
	for(auto &unit: units->GetUnits())
		list.push_back(unit->type_id);
	int new_ref_id = -1;
	for(auto &id: list)
	{
		bool dup = false;
		for(auto &rule: rules)
			if(rule.ref_unit == id)
				dup = true;
		if(!dup)
		{
			new_ref_id = id;
			break;
		}
	}
	if(new_ref_id < 0)
	{
		last_error = "Cannot add new rule! All unit types are already listed.";
		return(1);
	}

	UnitRandomizerRule rule;
	rule.ref_unit = new_ref_id;
	rules.push_back(rule);
	SortRules();
	for(auto &rule: rules)
		if(rule.ref_unit == new_ref_id)
			new_pos = &rule - rules.data();
	return(0);
}

// check rules for validity (duplicates and stuff)
int UnitRandomizer::CheckRules()
{
	last_error = "";
	for(auto &rule: rules)
	{
		bool dup = false;
		for(auto& rule2: rules)
		{
			if(&rule2 == &rule)
				continue;
			if(rule.ref_unit == rule2.ref_unit)
				dup = true;
		}
		if(dup)
		{
			last_error = string_format("Reference unit type '%d' already in auto randomzier rules list!",rule.ref_unit);
			return(1);
		}

		//std::sort(rule.rand_units.begin(),rule.rand_units.end());
		for(auto &id: rule.rand_units)
		{
			bool dup = false;
			for(auto &id2: rule.rand_units)
			{
				if(&id2 == &id)
					continue;
				if(id2 == id)
					dup = true;
			}
			if(dup)
			{
				last_error = string_format("Unit type '%d' is duplicate in auto randomzier random units list!",id);
				return(1);
			}
		}
	}
	return(0);
}

// remove auto randomizer rule
int UnitRandomizer::RemoveRule(int pos)
{
	last_error = "";
	if(pos < 0 || pos >= rules.size())
	{
		last_error = string_format("Auto randomizer rule '%d' not in the rule list!",pos);
		return(1);
	}
	rules.erase(rules.begin() + pos);
	return(0);
}

// try add mirror rules (e.g. for orc->{orc,kaorc} add kaorc->{orc,kaorc})
int UnitRandomizer::AddMirrorRules(int pos,int& new_pos)
{
	new_pos = pos;
	if(pos < 0 || pos >= rules.size())
		return(1);
	auto rule = rules[pos];
	for(auto &unit: rule.rand_units)
	{
		bool found = false;
		for(auto &rr: rules)
			if(rr.ref_unit == unit)
				found = true;
		if(!found)
		{
			UnitRandomizerRule nrule;
			nrule.ref_unit = unit;
			nrule.rand_units = rule.rand_units;
			rules.push_back(nrule);			
		}
	}
	new_pos = SortRules(pos);
	return(0);
}

// sort rules by ref unit type
int comp_rules(UnitRandomizerRule &a,UnitRandomizerRule& b)
{
	return(b.ref_unit >= a.ref_unit);
}
int UnitRandomizer::SortRules(int old_pos)
{
	for(int k = 0; k < rules.size(); k++)
		rules[k].id = k;
	std::sort(rules.begin(), rules.end(),comp_rules);
	for(int k = 0; k < rules.size(); k++)
		if(rules[k].id == old_pos)
			return(k);
	return(-1);
}

// get randomizer rule for unit type
UnitRandomizerRule* UnitRandomizer::GetRule(int type_id)
{
	for(auto &rule: rules)
		if(rule.ref_unit == type_id)
			return(&rule);
	return(NULL);
}


// randomize unit in map file
int UnitRandomizer::RandomizeMap(std::string& def, SpellUnits* units,std::string& error)
{	
	error = "";
	if(!units)
		return(1);

	// parse to lines
	auto lines = get_text_lines(def);

	// leave because it's not mission DEF but no error
	if(lines.empty() || !lines[0].starts_with("MissionData"))
		return(0);
	
	std::srand(time(0));

	// randomizer rules
	UnitRandomizer rules;

	// process all lines
	for(auto &line: lines)
	{

		if(line.starts_with("AutoRandomizeRule"))
		{
			SpellDefCmd cmd(line);
			if(!cmd.valid)
			{
				// invalid command
				error = "Possibly somehow incomplete command AutoRandomizeRule()?";
				return(1);
			}
			if(rules.AddRule(&cmd,units))
			{
				// invalid command
				error = string_format("Possibly invalid command %s. %s",cmd.full_command.c_str(),rules.last_error.c_str());
				return(1);
			}
			continue;
		}
		if(line.starts_with("AddUnit") || line.starts_with("AddSpecialUnit"))
		{
			SpellDefCmd cmd(line);
			if(!cmd.valid)
			{
				// invalid command
				error = "Possibly somehow incomplete command AddUnit() or AddSpecialUnit()?";
				return(1);
			}						
			if((cmd.name == "AddUnit" && cmd.parameters.size() != 7) || (cmd.name == "AddSpecialUnit" && cmd.parameters.size() != 6))
			{
				// invalid params count
				error = "Wrong parameters count for command AddUnit() or AddSpecialUnit().";
				return(1);
			}

			// check eventual sub-command
			if(!cmd.sub_valid)
				continue;
			if(cmd.sub_name != "Randomize")
			{
				// unknown sub-command
				error = string_format("Unknown sub-command \"%s\" for command AddUnit() or AddSpecialUnit().",cmd.sub_name.c_str());
				return(1);
			}

			// check original unit type
			int orig_unit_type = std::atoi(cmd.parameters[1].c_str());
			auto orig_unit = units->GetUnit(orig_unit_type);
			if(!orig_unit)
			{
				// unknown unit type
				error = string_format("Unknown unit type for command \"%s\".",cmd.full_command.c_str());
				return(1);
			}

			// get original health
			double health = (double)std::atoi(cmd.parameters[4].c_str()) / (double)orig_unit->cnt;


			std::vector<int> rand_list;
			if(cmd.sub_params.size() == 1 && iequals(cmd.sub_params[0],"OFF"))
			{
				// disabled
				continue;
			}
			else if(cmd.sub_params.size() == 1 && iequals(cmd.sub_params[0],"AUTO"))
			{
				// auto mode (defined globally for the map)

				// try fetch randomizer rule
				auto rule = rules.GetRule(orig_unit_type);
				if(!rule)
					continue;
				rand_list = rule->rand_units;

			}
			else if(!cmd.sub_params.empty())
			{
				// explicit list of unit codes				
				for(auto& unit_id_str: cmd.sub_params)
				{
					char* send;
					auto unit_type_id = std::strtol(unit_id_str.c_str(),&send,10);
					auto rand_unit = units->GetUnit(unit_type_id);
					if(!rand_unit || send == unit_id_str.c_str())
					{
						// invalid unit
						error = string_format("Unknown unit type \"%s\" in randomizer for command \"%s\".",unit_id_str.c_str(),cmd.sub_full_command.c_str());
						return(1);
					}
					rand_list.push_back(unit_type_id);
				}
			}
			else
				continue;
			if(rand_list.empty())
				continue;
			
			// randomize unit type			
			int rand_id = std::rand() % rand_list.size();
			auto unit_id = rand_list[rand_id];
			auto unit = units->GetUnit(unit_id);
			if(!unit)
			{
				// random unit ID not found
				error = string_format("Unknown unit type %d in randomizer for command \"%s\".",unit_id,cmd.sub_full_command.c_str());
				return(1);
			}
			// fix health
			int unit_health = std::max((int)(std::min(health,1.0)*(double)unit->cnt),1);
			
			// rebuild unit command
			cmd.parameters[1] = string_format("%d",unit_id);
			cmd.parameters[4] = string_format("%d",unit_health);
			line = cmd.name + "(" + merge_text_lines(cmd.parameters,",") + ")";

			continue;
		}

	}

	// merge modified lines
	def = merge_text_lines(lines);

	return(0);
}

