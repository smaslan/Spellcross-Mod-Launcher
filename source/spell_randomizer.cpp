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
#include "simpleini.h"

#include <algorithm>
#include <numeric>
#include <vector>


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
std::string UnitRandomizer::m_last_error;
int UnitRandomizer::RandomizeMap(std::string& def, SpellUnits* units,UnitRandomizerSetup* glob_rules)
{	
	m_last_error = "";
	if(!units)
		return(1);

	// parse to lines
	auto lines = get_text_lines(def);

	// leave because it's not mission DEF but no error
	if(lines.empty() || !lines[0].starts_with("MissionData"))
		return(0);
	
	// randomize seed
	std::srand(time(0));

	// local randomizer rules
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
				m_last_error = "Possibly somehow incomplete command AutoRandomizeRule()?";
				return(1);
			}
			if(rules.AddRule(&cmd,units))
			{
				// invalid command
				m_last_error = string_format("Possibly invalid command %s. %s",cmd.full_command.c_str(),rules.last_error.c_str());
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
			int orig_unit_type;
			if(str2int(cmd.parameters[1],orig_unit_type,0,units->Count()-1))
			{
				m_last_error = string_format("Failed parsing command \"%s\" parameters.",cmd.full_command.c_str());
				return(1);
			}
			auto orig_unit = units->GetUnit(orig_unit_type);
			if(!orig_unit)
			{
				// unknown unit type
				m_last_error = string_format("Unknown unit type for command \"%s\".",cmd.full_command.c_str());
				return(1);
			}

			// get original health
			int men;
			if(str2int(cmd.parameters[4],men,0,100))
			{
				m_last_error = string_format("Failed parsing command \"%s\" parameters.",cmd.full_command.c_str());
				return(1);
			}
			double health = (double)men/(double)orig_unit->cnt;

			// get original XP level
			int xp_level;
			if(str2int(cmd.parameters[3],xp_level,0,12))
			{
				m_last_error = string_format("Failed parsing command \"%s\" parameters.",cmd.full_command.c_str());
				return(1);
			}
			
			// get unit behaviour
			std::string behave;
			if(cmd.name == "AddUnit")
				behave = cmd.parameters[5];
			
			// get unit special type
			std::string spec_type;
			if(cmd.name == "AddSpecialUnit")
				spec_type = cmd.parameters[0];
			
			// try randomize using global rule if enabled
			int unit_id = -1;
			if(glob_rules && glob_rules->Randomize(units,orig_unit_type,unit_id))
			{
				m_last_error = string_format("Failed randomization using global rules for command \"%s\".",cmd.full_command.c_str());
				return(1);
			}
			std::vector<int> rand_list;
			if(glob_rules && unit_id >= 0 && 
				(spec_type.empty() || spec_type == "EnemyUnit") && 
				(glob_rules->apply_tough_def || behave != "ToughDefence") &&
				(glob_rules->apply_static && cmd.name == "AddUnit" || glob_rules->apply_events && cmd.name == "AddSpecialUnit"))
				rand_list.push_back(unit_id);
			
			// check eventual sub-command
			bool map_rules = false;
			if(cmd.sub_valid)
			{				
				if(cmd.sub_name != "Randomize")
				{
					// unknown sub-command
					m_last_error = string_format("Unknown sub-command \"%s\" for command AddUnit() or AddSpecialUnit().",cmd.sub_name.c_str());
					return(1);
				}			
				
				if(cmd.sub_params.size() == 1 && iequals(cmd.sub_params[0],"OFF"))
				{
					// disabled
					if(glob_rules && !glob_rules->override_off_rule)
						continue;
				}
				else if(cmd.sub_params.size() == 1 && iequals(cmd.sub_params[0],"AUTO"))
				{
					// auto mode (defined globally for the map)

					// try fetch randomizer rule
					auto rule = rules.GetRule(orig_unit_type);
					if(rule && !glob_rules)
					{
						rand_list = rule->rand_units;
						map_rules = true;
					}
					else if(!rule)
						continue;
				}
				else if(!cmd.sub_params.empty())
				{
					// explicit list of unit codes				
					if(!glob_rules || !glob_rules->override_explicit_rule)
					{
						for(auto& unit_id_str: cmd.sub_params)
						{
							char* send;
							auto unit_type_id = std::strtol(unit_id_str.c_str(),&send,10);
							auto rand_unit = units->GetUnit(unit_type_id);
							if(!rand_unit || send == unit_id_str.c_str())
							{
								// invalid unit
								m_last_error = string_format("Unknown unit type \"%s\" in randomizer for command \"%s\".",unit_id_str.c_str(),cmd.sub_full_command.c_str());
								return(1);
							}
							rand_list.push_back(unit_type_id);
						}
						map_rules = true;
					}
				}
				else
					continue;
			}
			if(rand_list.empty())
				continue;

			// randomize XP level?
			if(!map_rules && glob_rules && glob_rules->randomize_xp)
				xp_level = glob_rules->xp_min + std::rand() % (glob_rules->xp_max - glob_rules->xp_min + 1);
			
			// randomize unit type			
			int rand_id = std::rand() % rand_list.size();
			unit_id = rand_list[rand_id];
			auto unit = units->GetUnit(unit_id);
			if(!unit)
			{
				// random unit ID not found
				m_last_error = string_format("Unknown unit type %d in randomizer for command \"%s\".",unit_id,cmd.sub_full_command.c_str());
				return(1);
			}
			// fix health
			int unit_health = std::max((int)(std::min(health,1.0)*(double)unit->cnt),1);
			
			// rebuild unit command
			cmd.parameters[1] = string_format("%d",unit_id);
			cmd.parameters[4] = string_format("%d",unit_health);
			cmd.parameters[5] = string_format("%d",xp_level);
			line = cmd.name + "(" + merge_text_lines(cmd.parameters,",") + ")";

			continue;
		}

	}

	// merge modified lines
	def = merge_text_lines(lines);

	return(0);
}

// is source unit?
bool UnitRandomizerGlobRule::isSrcUnit(int uid)
{
	return(std::find(src_unit_list.begin(),src_unit_list.end(), uid) != src_unit_list.end());
}
// is target unit?
bool UnitRandomizerGlobRule::isTargetUnit(int uid)
{
	return(std::find(unit_list.begin(),unit_list.end(),uid) != unit_list.end());
}

// generate randomized unit
int UnitRandomizerGlobRule::Randomize(int &uid)
{	
	if(probab_list.size() != unit_list.size())
		return(1); // wrong rule?
	if(probab_list.empty())
		return(0); // no rules - skip
	if(!isSrcUnit(uid))
		return(0); // not source unit - skip

	// generate cumsum
	std::vector<double> prob(probab_list.size());
	std::partial_sum(probab_list.begin(),probab_list.end(),prob.begin());
	
	double rng = (double)std::rand()/(float)(RAND_MAX)*100.0;
	uid = std::min((int)(std::upper_bound(prob.begin(), prob.end(), rng) - prob.begin()), (int)prob.size() - 1);
	return(0);
}

// global rule can override local rules?
bool UnitRandomizerSetup::canOverrideLocalRule()
{
	return(override_explicit_rule || override_off_rule);
}

// randomize unit based on rules
int UnitRandomizerSetup::Randomize(SpellUnits* units,int src_unit_id,int& unit)
{	
	auto src_unit = units->GetUnit(src_unit_id);
	if(!src_unit)
		return(1);

	UnitRandomizerGlobRule *rule;
	if(src_unit->isLight())
		rule = &rules_light;
	else if(src_unit->isArmored())
		rule = &rules_armor;
	else if(src_unit->isAir())
		rule = &rules_air;
	else
		return(1);

	unit = src_unit_id;
	if(rule->Randomize(unit))
		return(1);		
	
	return(0);
}

// load rules from ini file
int UnitRandomizerSetup::LoadIni(std::filesystem::path path, SpellUnits *units)
{
	CSimpleIniA ini;
	if(ini.LoadFile(path.c_str()))
		return(1);

	int max_unit_id = 89;
	if(units)
		max_unit_id = units->Count() - 1;

	auto type_str = ini.GetValue("INFO","what_is_it");
	if(!type_str || strcmp(type_str,"Spellcross Unit Randomizer Preset") != 0)
		return(1);

	apply_tough_def = ini.GetBoolValue("SETUP","apply_tough_def",true);
	apply_static = ini.GetBoolValue("SETUP","apply_static",true);
	apply_events = ini.GetBoolValue("SETUP","apply_events",true);
	override_explicit_rule = ini.GetBoolValue("SETUP","override_explicit_rule",true);
	override_off_rule = ini.GetBoolValue("SETUP","override_off_rule",true);
	randomize_xp = ini.GetBoolValue("SETUP","randomize_xp",false);
	xp_min = ini.GetLongValue("SETUP","xp_min",1);
	xp_max = ini.GetLongValue("SETUP","xp_max",12);

	std::vector<std::pair<std::string,UnitRandomizerGlobRule*>> type_list ={{"LIGHT",&rules_light},{"ARMORED",&rules_armor},{"AIR",&rules_air}};
	for(auto& item: type_list)
	{
		auto section_label = string_format("CLASS:%s",item.first.c_str());
		auto& rules = item.second;

		auto src_list_str = ini.GetValue(section_label.c_str(),"source_units");
		auto src_list = str_split(src_list_str,',',true);
		if(str2int(src_list,rules->src_unit_list,0,max_unit_id))
			return(1);

		std::list<CSimpleIniA::Entry> list;
		ini.GetAllKeys(section_label.c_str(),list);

		rules->unit_list.clear();
		rules->probab_list.clear();
		for(auto &unit: list)
		{
			auto tok = regexp_get(unit.pItem,"(unit_probability)\\[(\\d+)\\]");
			if(tok.empty())
				continue;
			if(tok.size() != 2)
				return(1);
			int unit_id;
			if(str2int(tok[1], unit_id, 0, max_unit_id))
				return(1);
			auto prob = ini.GetDoubleValue(section_label.c_str(), unit.pItem, 0.0);
			rules->unit_list.push_back(unit_id);
			rules->probab_list.push_back(prob);
		}
	}

	m_path = path;

	return(0);
}

// save rules to ini file
int UnitRandomizerSetup::SaveIni(std::filesystem::path path,SpellUnits* units)
{
	CSimpleIniA ini;
	ini.SetUnicode(true);

	ini.SetValue("INFO","what_is_it","Spellcross Unit Randomizer Preset");
	ini.SetValue("INFO","created",get_local_time_str().c_str());
		
	ini.SetBoolValue("SETUP","apply_tough_def",apply_tough_def,"; Apply to ToughDefence class units?");
	ini.SetBoolValue("SETUP","apply_static",apply_static,"; Apply to static enemies?");
	ini.SetBoolValue("SETUP","apply_events",apply_events,"; Apply to event-spawned enemies?");
	ini.SetBoolValue("SETUP","override_explicit_rule",override_explicit_rule,"; Override explicit randomization map rule?");
	ini.SetBoolValue("SETUP","override_off_rule",override_off_rule,"; Override map randomizer rule set to OFF?");
	ini.SetBoolValue("SETUP","randomize_xp",randomize_xp,"; Randomizer unit XP level?");
	ini.SetLongValue("SETUP","xp_min",xp_min,"; Randomize unit XP (min value)?");
	ini.SetLongValue("SETUP","xp_max",xp_max,"; Randomize unit XP (max value)?");

	std::vector<std::pair<std::string, UnitRandomizerGlobRule*>> type_list = {{"LIGHT",&rules_light},{"ARMORED",&rules_armor},{"AIR",&rules_air}};	
	for(auto &item: type_list)
	{
		auto section_label = string_format("CLASS:%s",item.first.c_str());
		auto &rules = item.second;

		ini.SetValue(section_label.c_str(), "source_units",merge_vector(rules->src_unit_list,",").c_str(),"; source (applicable) units list");
		
		for(int k = 0; k < rules->unit_list.size(); k++)
		{
			int unit_id = rules->unit_list[k];
			double prob = rules->probab_list[k];
			std::string unit_label = string_format("unit_probability[%d]",unit_id);
			SpellUnitRec *unit;
			std::string comment;
			if(units && (unit = units->GetUnit(unit_id)))
				comment = string_format("; %ls", unit->name.c_str());			
			ini.SetDoubleValue(section_label.c_str(),unit_label.c_str(),prob,(comment.empty())?NULL:(comment.c_str()));
		}		
	}

	if(ini.SaveFile(path.c_str()))
		return(1);
	m_path = path;

	return(0);
}