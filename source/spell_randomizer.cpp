//=============================================================================
// Spellcross map/game scripts randomizer stuff (units, trees, ...).
// 
// This code is part of Spellcross Map Editor project.
// (c) 2026, Stanislav Maslan, s.maslan@seznam.cz
// url: https://github.com/smaslan/spellcross-map-edit
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================
#include "spell_randomizer.h"
#include "other.h"
#include "spell_def.h"
#include "spell_units.h"

#include <algorithm>
#include <numeric>
#include <vector>
#include <ranges>
#include <filesystem>


//-----------------------------------------------------------------------------
// Units randomizer stuff
//-----------------------------------------------------------------------------

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
	srand_init();

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
				m_last_error = string_format("Failed parsing command \"%s\" parameters.",cmd.full_command);
				return(1);
			}
			auto orig_unit = units->GetUnit(orig_unit_type);
			if(!orig_unit)
			{
				// unknown unit type
				m_last_error = string_format("Unknown unit type for command \"%s\".",cmd.full_command);
				return(1);
			}

			// get original health
			int men;
			if(str2int(cmd.parameters[4],men,0,100))
			{
				m_last_error = string_format("Failed parsing command \"%s\" parameters. HP %s over 100?",cmd.full_command,cmd.parameters[4]);
				return(1);
			}
			double health = (double)men/(double)orig_unit->cnt;

			// get original XP level
			int xp_level;
			if(str2int(cmd.parameters[3],xp_level,0,12))
			{
				m_last_error = string_format("Failed parsing command \"%s\" parameters. XP level %s above 12?",cmd.full_command,cmd.parameters[3]);
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
				m_last_error = string_format("Failed randomization using global rules for command \"%s\".",cmd.full_command);
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
					m_last_error = string_format("Unknown sub-command \"%s\" for command AddUnit() or AddSpecialUnit().",cmd.sub_name);
					return(1);
				}			
				
				if(cmd.sub_params.size() == 1 && iequals(cmd.sub_params[0],"OFF"))
				{
					// disabled
					if(!glob_rules || !glob_rules->override_off_rule)
						continue;
				}
				else if(cmd.sub_params.size() == 1 && iequals(cmd.sub_params[0],"AUTO"))
				{
					// auto mode (defined globally for the map)

					// try fetch randomizer rule
					auto rule = rules.GetRule(orig_unit_type);
					if(rule && !glob_rules)
					{
						// use map rules
						rand_list = rule->rand_units;
						map_rules = true;
					}
					else if(!rule && !glob_rules)
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
								m_last_error = string_format("Unknown unit type \"%s\" in randomizer for command \"%s\".",unit_id_str,cmd.sub_full_command);
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
				m_last_error = string_format("Unknown unit type %d in randomizer for command \"%s\".",unit_id,cmd.sub_full_command);
				return(1);
			}
			// fix health
			int unit_health = std::max((int)(std::min(health,1.0)*(double)unit->cnt),1);
			
			// rebuild unit command
			cmd.parameters[1] = string_format("%d",unit_id);
			cmd.parameters[3] = string_format("%d",xp_level);
			cmd.parameters[4] = string_format("%d",unit_health);			
			line = cmd.name + "(" + merge_text_lines(cmd.parameters,",") + ")";

			continue;
		}

	}

	// merge modified lines
	def = merge_text_lines(lines);

	return(0);
}


// randomize units in level?.def files
int UnitRandomizer::RandomizeLevel(std::string& def,SpellUnits* units,UnitRandomizerSetup* glob_rules)
{
	m_last_error = "";
	if(!units)
		return(1);
	if(!glob_rules)
		return(1);

	// parse to lines
	auto lines = get_text_lines(def);

	// leave because it's not level DEF but no error
	if(lines.empty() || !lines[0].starts_with("LevelInit"))
		return(0);

	// randomize seed
	srand_init();
	

	// process all lines
	for(auto& line: lines)
	{
		if(line.starts_with("Army") && glob_rules->apply_level_army || 
			line.starts_with("AttackUnits") && glob_rules->apply_level_rand_attack_units ||
			line.starts_with("AttackSpecialUnits") && glob_rules->apply_level_rand_spec_units)
		{
			SpellDefCmd cmd(line);
			if(!cmd.valid)
			{
				// invalid command
				m_last_error = string_format("Possibly somehow incomplete command \"%s\"?",line);
				return(1);
			}

			// for each listed unit:
			std::vector<int> army_ids;
			if(str2int(cmd.parameters,army_ids,0,units->Count()-1))
			{
				// invalid command
				m_last_error = string_format("Some of the units in command \"%s\" not recognized in command \"%s\".",cmd.name,cmd.full_command);
				return(1);
			}
			for(auto& orig_unit_type: army_ids)
			{
				// check original unit type
				auto orig_unit = units->GetUnit(orig_unit_type);
				if(!orig_unit)
				{
					// unknown unit type
					m_last_error = string_format("Unknown unit type #%d in command \"%s\".",orig_unit_type,cmd.full_command);
					return(1);
				}

				// try randomize using global rule if enabled
				int unit_id = -1;
				if(glob_rules->Randomize(units,orig_unit_type,unit_id))
				{
					m_last_error = string_format("Failed randomization using local rules in command \"%s\".",cmd.full_command);
					return(1);
				}
				if(glob_rules && unit_id >= 0 &&
					(glob_rules->apply_level_army))
				{
					// swap unit type
					auto unit = units->GetUnit(unit_id);
					if(!unit)
					{
						// random unit ID not found
						m_last_error = string_format("Unknown new randomizer unit type #%d in command \"%s\".",unit_id,cmd.sub_full_command);
						return(1);
					}
					orig_unit_type = unit_id;
				}
			}
			
			// replace command data
			line = cmd.name + "(" + merge_vector(army_ids,",") + ")";
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

// check rules are applicable for given units list or fix to valid range
int UnitRandomizerGlobRule::Validate(SpellUnits* units,bool fix)
{
	if(!units)
		return(1);
	int max_unit_id = units->Count() - 1;
	std::vector<int> temp_units;
	std::vector<double> temp_prob;
	for(int k = 0; k < unit_list.size(); k++)
	{
		auto uid = unit_list[k];
		if(uid > max_unit_id && !fix)
			return(1); // failed
		if(uid > max_unit_id)
			continue;
		temp_units.push_back(uid);
		temp_prob.push_back(probab_list[k]);
	}
	unit_list = temp_units;
	probab_list = temp_prob;
	Normalize();
	return(0);
}

// normalize weights to 100% sum
void UnitRandomizerGlobRule::Normalize()
{
	if(probab_list.empty())
		return;
	
	int count = probab_list.size();
	if(count == 1)
	{
		probab_list[0] = 100.0;
		return;
	}
	
	double sum = std::accumulate(probab_list.begin(), probab_list.end(),0.0);
	if(sum == 0.0)
	{
		probab_list.assign(count,100.0/count);
		return;
	}
	for(auto &it: probab_list)
		it *= (100.0/sum);
	
	return;
}

// generate randomized unit
int UnitRandomizerGlobRule::Randomize(int src_unit_id, int &uid)
{	
	if(probab_list.size() != unit_list.size())
		return(1); // wrong rule?
	if(probab_list.empty())
		return(0); // no rules - skip
	if(!isSrcUnit(src_unit_id))
		return(0); // not source unit - skip

	// generate cumsum
	std::vector<double> prob(probab_list.size());
	std::partial_sum(probab_list.begin(),probab_list.end(),prob.begin());
	
	double rng = (double)std::rand()/(float)(RAND_MAX)*100.0;
	uid = unit_list[std::min((int)(std::upper_bound(prob.begin(), prob.end(), rng) - prob.begin()), (int)prob.size() - 1)];
	return(0);
}

// global rule can override local rules?
bool UnitRandomizerSetup::canOverrideLocalRule()
{
	return(override_explicit_rule || override_off_rule);
}

// check rules are applicable for given units list or fix to valid range
int UnitRandomizerSetup::Validate(SpellUnits* units,bool fix)
{
	if(rules_light.Validate(units,fix))
		return(1);
	if(rules_armor.Validate(units,fix))
		return(1);
	if(rules_air.Validate(units,fix))
		return(1);
	return(0);
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
	
	int rand_unit = -1;
	if(rule->Randomize(src_unit_id,rand_unit))
		return(1);
	if(rand_unit >= 0)
		unit = rand_unit;
	
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

	filter_by_save_research = ini.GetBoolValue("SETUP","filter_by_save_research",false);
	apply_tough_def = ini.GetBoolValue("SETUP","apply_tough_def",true);
	apply_static = ini.GetBoolValue("SETUP","apply_static",true);
	apply_events = ini.GetBoolValue("SETUP","apply_events",true);
	apply_level_army = ini.GetBoolValue("SETUP","apply_level_army",true);
	apply_level_rand_attack_units = ini.GetBoolValue("SETUP","apply_level_rand_attack_units",true);
	apply_level_rand_spec_units = ini.GetBoolValue("SETUP","apply_level_rand_spec_units",true);
	override_explicit_rule = ini.GetBoolValue("SETUP","override_explicit_rule",true);
	override_off_rule = ini.GetBoolValue("SETUP","override_off_rule",true);
	randomize_xp = ini.GetBoolValue("SETUP","randomize_xp",false);
	xp_min = ini.GetLongValue("SETUP","xp_min",1);
	xp_max = ini.GetLongValue("SETUP","xp_max",12);

	std::vector<std::pair<std::string,UnitRandomizerGlobRule*>> type_list ={{"LIGHT",&rules_light},{"ARMORED",&rules_armor},{"AIR",&rules_air}};
	for(auto& item: type_list)
	{
		auto section_label = string_format("CLASS:%s",item.first);
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
	
	ini.SetBoolValue("SETUP","filter_by_save_research",filter_by_save_research,"; Filter target units list by available research in latest save game?");
	ini.SetBoolValue("SETUP","apply_tough_def",apply_tough_def,"; Apply to ToughDefence class units?");
	ini.SetBoolValue("SETUP","apply_static",apply_static,"; Apply to static enemies?");
	ini.SetBoolValue("SETUP","apply_events",apply_events,"; Apply to event-spawned enemies?");
	ini.SetBoolValue("SETUP","apply_level_army",apply_level_army,"; Apply to event-driven counter attack enemies?");
	ini.SetBoolValue("SETUP","apply_level_rand_attack_units",apply_level_rand_attack_units,"; Apply to random counter attack enemies?");
	ini.SetBoolValue("SETUP","apply_level_rand_spec_units",apply_level_rand_spec_units,"; Apply to random counter attack special enemies?");
	ini.SetBoolValue("SETUP","override_explicit_rule",override_explicit_rule,"; Override explicit randomization map rule?");
	ini.SetBoolValue("SETUP","override_off_rule",override_off_rule,"; Override map randomizer rule set to OFF?");
	ini.SetBoolValue("SETUP","randomize_xp",randomize_xp,"; Randomizer unit XP level?");
	ini.SetLongValue("SETUP","xp_min",xp_min,"; Randomize unit XP (min value)?");
	ini.SetLongValue("SETUP","xp_max",xp_max,"; Randomize unit XP (max value)?");

	std::vector<std::pair<std::string, UnitRandomizerGlobRule*>> type_list = {{"LIGHT",&rules_light},{"ARMORED",&rules_armor},{"AIR",&rules_air}};	
	for(auto &item: type_list)
	{
		auto section_label = string_format("CLASS:%s",item.first);
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











//-----------------------------------------------------------------------------
// Trees randomizer stuff
//-----------------------------------------------------------------------------

// add randomizer terrain rules set
SpellTreeRandomizerTerrain* SpellTreeRandomizerRules::AddTerrain(SpellTreeRandomizerTerrain *terrain)
{
	if(!terrain)
		return(NULL);
	if(GetTerrain(terrain->name))
		return(NULL);
	terrains.push_back(*terrain);
	return(GetTerrain(terrain->name));
}

// add randomizer terrain rules set by name
SpellTreeRandomizerTerrain* SpellTreeRandomizerRules::AddTerrain(std::string terrain_name)
{
	if(GetTerrain(terrain_name))
		return(NULL);
	SpellTreeRandomizerTerrain terr;
	terr.name = terrain_name;
	terr.m_path.clear();
	terrains.push_back(terr);
	return(GetTerrain(terrain_name));
}

// remove randomizer terrain rules set by name
int SpellTreeRandomizerRules::RemoveTerrain(std::string terrain_name)
{
	if(std::erase_if(terrains, [&terrain_name](SpellTreeRandomizerTerrain &terr){return(terr.name == terrain_name);}))
		return(0);	
	return(1);
}

// get randomizer terraub rules set by terrain name
SpellTreeRandomizerTerrain* SpellTreeRandomizerRules::GetTerrain(std::string terrain_name)
{
	for(auto& item: terrains)
		if(iequals(item.name,terrain_name))
			return(&item);
	return(NULL);
}

// try load preset to terrain
int SpellTreeRandomizerRules::LoadPreset(std::filesystem::path info_path,std::string& terrain_name,bool add_new)
{
	terrain_name.clear();

	std::string info_str;
	if(loadstr(info_path,info_str))
		return(1);
	auto info = get_text_lines(info_str);

	auto terr_name = info_get_string(info,"Terrain");
	auto terr = GetTerrain(terr_name);
	if(!terr && add_new)
		terr = AddTerrain(terr_name);
	if(!terr)
		return(1);

	auto rule_names = info_get_text_vector(info,"Rules");
	for(auto& rule_name: rule_names)
	{
		auto rule_info = info_get_section(info,rule_name);
		if(rule_info.empty())
			return(1);

		auto rule = terr->GetRule(rule_name);
		if(!rule)
			rule = terr->AddRule(rule_name);
		if(!rule)
			return(1);

		auto src_trees = info_get_text_vector(rule_info,"Source trees");
		rule->src_trees.clear();
		std::ranges::for_each(src_trees,[&rule](std::string& name) { rule->src_trees.emplace_back(name);});

		auto rand_trees = info_get_text_vector(rule_info,"Random trees");
		rule->rand_trees.clear();
		for(auto& item: rand_trees)
		{
			auto toks = regexp_get(item,"([^:]+):([^\%]+)\%");
			if(toks.size() != 2)
				return(1);
			auto name = toks[0];
			double prob;
			if(str2real(toks[1],prob,0.0,100.01))
				return(1);
			rule->rand_trees.emplace_back(name,prob);
		}
	}

	terr->m_path = info_path;
	terrain_name = terr_name;
	
	return(0);
}


// store preset
int SpellTreeRandomizerRules::StorePreset(std::filesystem::path info_path,std::string terrain_name)
{
	auto terr = GetTerrain(terrain_name);
	if(!terr)
		return(1);

	std::vector<std::string> rules;
	std::ranges::transform(terr->rules,std::back_inserter(rules),&SpellTreeRandomizerRule::name);

	std::string info;
	info += "// Trees randomizer for Spellcross Mod Launcher\n";
	info += string_format("// created: %s\n\n",get_local_time_str());

	info += string_format("Terrain:: %s\n\n",terr->name);
	info += info_make_text_vector("Rules",rules,"// rules list");

	for(auto& rule: terr->rules)
	{
		std::string info_rule;

		std::vector<std::string> list;
		std::ranges::transform(rule.src_trees,std::back_inserter(list),&SpellTreeRandomizerItem::name);
		info_rule += info_make_text_vector("Source trees",list,"\n// applicable trees list");

		list.clear();
		std::ranges::for_each(rule.rand_trees,[&list](SpellTreeRandomizerItem tree) { list.push_back(string_format("%s:%0.3f%%",tree.name,tree.probab));});
		info_rule += info_make_text_vector("Random trees",list,"\n// random trees list: name:probability");
		info_rule += "\n";

		info += "\n";
		info += info_make_section(rule.name,info_rule);
	}

	auto ret = savestr(info_path,info);
	if(ret)
		return(ret);
	terr->m_path = info_path;
	return(0);
}

// clear all loaded stuff
void SpellTreeRandomizerRules::Clear()
{
	terrains.clear();
}

// load tree randomizer setup from ini file (usually application ini)
int SpellTreeRandomizerRules::LoadINIpresets(CSimpleIniA *ini, std::string section, std::string key)
{
	Clear();
	if(!ini)
		return(1);

	// for each possible preset in ini:
	for(int k = 1; k <= 10; k++)
	{
		auto pres_key = string_format("%s<%d>",key,k);
		auto pstr = ini->GetValue(section.c_str(),pres_key.c_str());
		if(!pstr)
			continue;
		auto path = std::filesystem::path(pstr);
		if(path.empty() || !std::filesystem::exists(path))
			continue;
		
		std::string terr_name;
		LoadPreset(path, terr_name, true);
	}

	return(0);
}

// save tree randomizer setup to ini file (usually application ini)
int SpellTreeRandomizerRules::SaveINIpresets(CSimpleIniA* ini,std::string section,std::string key)
{
	if(!ini)
		return(1);
	
	// for each terrain try make a preset record:
	int pres_id = 1;
	for(auto &terr: terrains)
	{
		if(terr.m_path.empty())
			continue;
		if(!std::filesystem::exists(terr.m_path))
			continue;
		
		const char *com = "; last trees randomizer presets";
		if(pres_id > 1)
			com = NULL;
		auto pres_key = string_format("%s<%d>",key,pres_id++);
		ini->SetValue(section.c_str(), pres_key.c_str(),terr.m_path.string().c_str(),com);
	}

	// remove old stuff
	for(int k = pres_id; k <= 10; k++)
	{
		auto pres_key = string_format("%s<%d>",key,k);
		ini->DeleteValue(section.c_str(),pres_key.c_str(),NULL,true);
	}

	return(0);
}


// new terrain
SpellTreeRandomizerTerrain::SpellTreeRandomizerTerrain()
{
}

// get rule by name
SpellTreeRandomizerRule* SpellTreeRandomizerTerrain::GetRule(std::string name)
{
	for(auto& item: rules)
		if(iequals(item.name,name))
			return(&item);
	return(NULL);
}

// add new rule
SpellTreeRandomizerRule* SpellTreeRandomizerTerrain::AddRule(std::string name)
{
	auto rule = GetRule(name);
	if(rule)
		return(NULL);

	for(int k = 1;;k++)
	{
		name = string_format("New rule %d",k);
		auto rule = GetRule(name);
		if(!rule)
			break;
	}

	SpellTreeRandomizerRule new_rule;
	new_rule.name = name;
	rules.push_back(new_rule);
	rule = GetRule(name);
	return(rule);
}

// remove randomizer rule by name
int SpellTreeRandomizerTerrain::RemoveRule(std::string name)
{
	auto item = std::find_if(rules.begin(),rules.end(),[name](SpellTreeRandomizerRule x) { return iequals(x.name,name);});
	if(item == rules.end())
		return(1);
	rules.erase(item);
	return(0);
}

// get used trees from all but ref_rule
std::vector<std::string> SpellTreeRandomizerTerrain::GetUsedTrees(SpellTreeRandomizerRule* ref_rule)
{
	std::vector<std::string> list;
	for(auto& rule: rules)
	{
		if(&rule == ref_rule)
			continue;
		for(auto& tree: rule.src_trees)
			list.push_back(tree.name);
	}
	return(list);
}

// filter all rules by available tree list, also remove rules that are empty
int SpellTreeRandomizerTerrain::FilterTrees(std::vector<std::string>& list)
{
	std::vector<std::string> rem_list;
	for(auto &rule: rules)
		if(rule.FilterTrees(list))
			rem_list.push_back(rule.name);
	for(auto &name: rem_list)
		RemoveRule(name);
	if(rules.empty())
		return(1);
	return(0);
}

// prepare rules list per sprite names, also assign name list ID to each random sprite
int SpellTreeRandomizerTerrain::MakeSpriteRules(std::vector<std::string>& names)
{
	m_sprite_names = names;	
	m_sprite_rules.clear();
	for(auto &name: names)
	{		
		// make ref to sprite
		SpellTreeRandomizerRule *p_rule = NULL;
		for(auto &rule: rules)
			if(rule.GetSrcTree(name))
			{			
				p_rule = &rule;
				break;
			}
		m_sprite_rules.push_back(p_rule);

		// assign name id to random sprites
		for(auto& rule: rules)
		{
			auto tree = rule.GetRandTree(name);
			if(!tree)
				continue;
			tree->id = &name - names.data();
		}
	}

	// prepare pdf functions for randomizer
	for(auto& rule: rules)
		rule.PrepareRng();
	
	return(0);
}

// get random tree using sprite rules
int SpellTreeRandomizerTerrain::GetRandomTreeID(int id)
{
	if(id < 0 || id >= m_sprite_rules.size())
		return(id);
	auto rule = m_sprite_rules[id];
	if(!rule)
		return(id);
	auto tree = rule->GetRandomTree();
	if(!tree)
		return(id);
	return(tree->id);
}



// get source tree by name
SpellTreeRandomizerItem* SpellTreeRandomizerRule::GetSrcTree(std::string name)
{
	auto item = std::find_if(src_trees.begin(),src_trees.end(),[name](SpellTreeRandomizerItem x) { return x.name == name;});
	if(item == src_trees.end())
		return(NULL);
	return(&*item);
}

// get source tree by name
SpellTreeRandomizerItem* SpellTreeRandomizerRule::GetRandTree(std::string name)
{
	auto item = std::find_if(rand_trees.begin(),rand_trees.end(),[name](SpellTreeRandomizerItem x) { return x.name == name;});
	if(item == rand_trees.end())
		return(NULL);
	return(&*item);
}

// filter tree lists by allowed list of names
int SpellTreeRandomizerRule::FilterTrees(std::vector<std::string> &list)
{
	std::erase_if(src_trees, [&list](SpellTreeRandomizerItem &item){ return(std::ranges::find(list, item.name) == list.end()); });
	std::erase_if(rand_trees,[&list](SpellTreeRandomizerItem& item) { return(std::ranges::find(list,item.name) == list.end()); });
	if(src_trees.empty() || rand_trees.empty())
		return(1);
	FixProb();
	return(0);
}

// update tree state in list (add or remove)
int SpellTreeRandomizerRule::UpdateSrcTree(std::string name,bool state)
{
	if(state)
	{
		// add
		for(auto& item: src_trees)
			if(item.name == name)
			{
				// already there
				return(1);
			}
		src_trees.emplace_back(name);
	}
	else
	{
		// remove
		return(!std::erase_if(src_trees,[name](SpellTreeRandomizerItem x) { return x.name == name;}));
	}
}

// update random tree state in list (add or remove)
int SpellTreeRandomizerRule::UpdateRandTree(std::string name,bool state)
{
	if(state)
	{
		// add
		for(auto& item: rand_trees)
			if(item.name == name)
			{
				// already there
				return(1);
			}
		rand_trees.emplace_back(name,0.0);
	}
	else
	{
		// remove
		return(!std::erase_if(rand_trees,[name](SpellTreeRandomizerItem x) { return x.name == name;}));
	}
}

// set equal probabilities
int SpellTreeRandomizerRule::SetEqualProb()
{
	auto prob = 100.0/rand_trees.size();
	std::for_each(rand_trees.begin(),rand_trees.end(),[prob](SpellTreeRandomizerItem& tree) { tree.probab = prob; });
	return(0);
}
// set random probabilities
int SpellTreeRandomizerRule::SetRandomProb()
{
	if(rand_trees.empty())
		return(1);
	srand_init();
	std::for_each(rand_trees.begin(),rand_trees.end(),[](SpellTreeRandomizerItem& tree) { tree.probab = std::rand(); });
	auto sum = std::accumulate(rand_trees.begin(),rand_trees.end(),0.0,[](double sum,const SpellTreeRandomizerItem& item) {return sum + item.probab;});
	if(sum == 0.0)
		return(1);
	auto scale = 100.0/sum;
	std::for_each(rand_trees.begin(),rand_trees.end(),[scale](SpellTreeRandomizerItem& tree) { tree.probab *= scale; });
	return(0);
}
// clear probabilities
int SpellTreeRandomizerRule::ClearProb()
{
	std::for_each(rand_trees.begin(),rand_trees.end(),[](SpellTreeRandomizerItem& tree) { tree.probab = 0.0; });
	return(0);
}
// fix probabilities so the sum is 100%, optional exclude ref_name item
int SpellTreeRandomizerRule::FixProb(std::string ref_name)
{
	if(rand_trees.empty())
		return(1);

	if(rand_trees.size() == 1)
	{
		rand_trees[0].probab = 100.0;
		return(0);
	}

	auto ref = GetRandTree(ref_name);
	if(!ref)
	{
		auto sum = std::accumulate(rand_trees.begin(),rand_trees.end(),0.0,[](double sum,const SpellTreeRandomizerItem& item) {return sum + item.probab;});
		if(sum == 0.0)
		{
			// set equal
			double prob = 100.0/rand_trees.size();
			std::for_each(rand_trees.begin(),rand_trees.end(),[prob](SpellTreeRandomizerItem& tree) { tree.probab = prob; });
			return(1);
		}
		auto scale = 100.0/sum;
		std::for_each(rand_trees.begin(),rand_trees.end(),[scale](SpellTreeRandomizerItem& tree) { tree.probab *= scale; });
		return(0);
	}

	auto sum = std::accumulate(rand_trees.begin(),rand_trees.end(),0.0,[](double sum,const SpellTreeRandomizerItem& item) {return sum + item.probab;});
	if(ref)
		sum -= ref->probab;
	if(sum == 0.0)
		return(1);
	auto scale = 100.0/sum;
	std::for_each(rand_trees.begin(),rand_trees.end(),[scale,ref](SpellTreeRandomizerItem& tree) { if(ref != &tree) tree.probab *= scale; });
	return(0);
}



// precalculate pdf for randomizer
int SpellTreeRandomizerRule::PrepareRng()
{
	// normalize first
	FixProb();

	// generate cumsum
	m_pdf.assign(rand_trees.size(),0.0);	
	auto prob = rand_trees | std::views::transform(&SpellTreeRandomizerItem::probab);
	std::partial_sum(prob.begin(),prob.end(),m_pdf.begin());
	
	return(0);
}

// get random tree using precalculated pdf function
SpellTreeRandomizerItem* SpellTreeRandomizerRule::GetRandomTree()
{
	if(m_pdf.empty() || m_pdf.size() != rand_trees.size())
		return(NULL);
	double rng = (double)std::rand()/(float)(RAND_MAX)*100.0;
	return(&rand_trees[std::min((int)(std::upper_bound(m_pdf.begin(),m_pdf.end(),rng) - m_pdf.begin()),(int)m_pdf.size() - 1)]);
}

// try load toolset from exported info from map editor
int SpellTreeToolset::LoadInfo(std::filesystem::path info_path,std::string terr_name)
{
	m_name.clear();
	m_tools.clear();
	m_terrain_name.clear();

	// load and parse to lines
	std::string info_str;
	if(loadstr(info_path,info_str))
		return(1);
	auto info = get_text_lines(info_str);

	// check file type
	if(info_get_string(info,"What") != "Spellcross toolset")
		return(1);

	// terrain type
	m_terrain_name = info_get_string(info,"Terrain");
	if(m_terrain_name.empty())
		return(1);
	if(!terr_name.empty() && m_terrain_name != terr_name)
		return(1);

	// load tools
	auto tools_list = info_get_text_vector(info,"Tools list");

	for(auto& tool: tools_list)
	{
		auto list = info_get_text_vector(info,tool);
		m_tools.emplace_back(&tool - tools_list.data(),tool,list);
	}

	return(0);
}

// prepare randomizing rules: filter rules by available terrain sprites, make fast tree lookups
int SpellTreeRandomizer::PrepareRules(SpellTreeRandomizerRules& rules,std::vector<std::shared_ptr<FSarchive>> terrain_fs_archives)
{	
	m_last_error.clear();

	for(auto &fs_archive: terrain_fs_archives)
	{
		auto terr_name = std::filesystem::path(fs_archive->m_fs_name).stem().string();

		// try make local terrain from provided rules
		auto rule = rules.GetTerrain(terr_name);
		if(!rule)
			continue;
		auto terr = m_rules.AddTerrain(rule);
		if(!terr)
		{
			m_last_error = string_format("Failed creating terrain \"%s\" rules set!", terr_name);
			return(1);
		}

		// get available tree names and get rid of extensions
		auto tree_names = fs_archive->GetFileNames("STA_*.DTA");
		std::ranges::for_each(tree_names, [](std::string &name){ name = std::filesystem::path(name).stem().string();});

		// filter rules
		if(terr->FilterTrees(tree_names))
		{
			// remove empty terrains
			m_rules.RemoveTerrain(terr_name);
			continue;
		}

		// make rule ref for each sprite in terrain
		auto spr_names = fs_archive->GetFileNames("*.DTA");
		std::ranges::for_each(spr_names,[](std::string& name) { name = std::filesystem::path(name).stem().string();});
		terr->MakeSpriteRules(spr_names);
	}
	
	return(0);
}

// randomzize map DTA file using current rules
int SpellTreeRandomizer::RandomizeMapDTA(std::vector<uint8_t>& dta, std::string dta_name)
{
	m_last_error.clear();
	
	uint8_t *data = dta.data();
	uint8_t *dend = data + dta.size();

	if(data + sizeof(uint32_t) >= dend)
		return(0); // possibly not map DTA?
	
	if(data + sizeof(uint32_t) >= dend)
		return(0); // possibly not map DTA?
	int L1_offset = *(uint32_t*)data; data += sizeof(uint32_t);
	if(dta.data() + L1_offset >= dend)
		return(0); // possibly not map DTA?

	if(data + sizeof(uint32_t) >= dend)
		return(0); // possibly not map DTA?
	int L1_count = *(uint32_t*)data; data += sizeof(uint32_t);
	if(L1_count > 4095)
		return(0); // possibly not map DTA?

	// version check
	if(data + sizeof(uint8_t) >= dend)
		return(0); // possibly not map DTA?
	if(*data++ != 0x12)
		return(0); // possibly not map DTA?
	// from now on we assume it is map DTA, so any error is fail

	// get map size
	if(data + 2*sizeof(uint16_t) >= dend)
	{
		m_last_error = string_format("Failed parsing map DTA file \"%s\"! Possibly corrupted file?",dta_name);
		return(1);
	}
	int x_size = *(int16_t*)data; data += sizeof(uint16_t);
	int y_size = *(int16_t*)data; data += sizeof(uint16_t);

	// get map terrain name
	if(data + 13 >= dend)
	{
		m_last_error = string_format("Failed parsing map DTA file \"%s\"! Possibly corrupted file?",dta_name);
		return(1);
	}
	char terr_name[14];
	std::memset((void*)terr_name,'\0',sizeof(terr_name));
	std::memcpy((void*)terr_name,data,13);
	data += 13;

	// check if we have this terrain in rules
	auto terr = m_rules.GetTerrain(terr_name);
	if(!terr)
		return(0); // nope, just leave

	// skip Layer 1: terrain
	int L1_size = L1_count*8 + x_size*y_size*2;
	if(data + L1_size >= dend)
	{
		m_last_error = string_format("Failed parsing map DTA file \"%s\"! Possibly corrupted file?",dta_name);
		return(1);
	}
	data += L1_size;

	// get L2 sprites count
	auto p_L2_start = data - dta.data();
	if(data + sizeof(uint32_t) >= dend)
	{
		m_last_error = string_format("Failed parsing map DTA file \"%s\"! Possibly corrupted file?",dta_name);
		return(1);
	}
	int L2_count = *(uint32_t*)data; data += sizeof(uint32_t);
	if(L2_count > 255)
	{
		m_last_error = string_format("Failed parsing map DTA file \"%s\"! Too many L2 unique sprites %d (max 255)?",dta_name,L2_count);
		return(1);
	}
	if(data + L2_count*8 >= dend)
	{
		m_last_error = string_format("Failed parsing map DTA file \"%s\"! Possibly corrupted file?",dta_name);
		return(1);
	}

	// load list of used sprites, reindex to global terrain indices	
	auto p_L2_list = data - dta.data();
	std::vector<int> L2_list;
	for(int k = 0; k < L2_count; k++)
	{
		// read sprite name
		char name[9];
		std::memset((void*)name,'\0',sizeof(name));
		std::memcpy((void*)name,(void*)data,8);
		data += 8;

		// try to find matching sprite in randomizer list
		auto sid = std::ranges::find(terr->m_sprite_names,std::string(name));
		if(sid == terr->m_sprite_names.end())
		{		
			m_last_error = string_format("Failed parsing map DTA file \"%s\"! L2 sprite %s not found in currently loaded sprites?",dta_name,name);
			return(1);
		}
		int id = sid - terr->m_sprite_names.begin();
		L2_list.push_back(id);		
	}

	// load L2 data	
	auto p_L2_data = data - dta.data();
	if(data + x_size*y_size*2 >= dend)
	{
		m_last_error = string_format("Failed parsing map DTA file \"%s\"! Possibly corrupted file?",dta_name);
		return(1);
	}
	std::vector<int> L2_map;
	for(int k = 0; k < x_size*y_size; k++)
	{
		// get sprite index
		int sid = *data++;
		// get sprite flags
		int code = *data++;
		
		if(!sid)
		{
			// empty
			L2_map.push_back(-1);
			continue;
		}
		// try randomize
		if(sid - 1 >= L2_list.size())
		{
			m_last_error = string_format("Failed parsing map DTA file \"%s\"! Possibly corrupted file? L2 sprite index %d out of range of listed used sprites.",dta_name,sid - 1);
			return(1);
		}
		sid = terr->GetRandomTreeID(L2_list[sid - 1]);
		L2_map.push_back(sid);
	}
	
	// find unique sprites
	auto L2_list_new = L2_map;
	std::erase_if(L2_list_new, [](int &item){ return(item < 0);});
	std::ranges::sort(L2_list_new);
	auto L2_list_new_res = std::ranges::unique(L2_list_new);
	L2_list_new.erase(L2_list_new_res.begin(),L2_list_new_res.end());
	// too many?
	L2_count = L2_list_new.size();
	if(L2_count > 255)
	{
		m_last_error = string_format("Failed parsing map DTA file \"%s\"! Too many unique L2 sprites %d (max 255). Reduce randomizer random sprites count.",dta_name,L2_count);
		return(1);
	}
	
	// make new DTA
	
	// remove old L2 sprites list
	dta.erase(dta.begin() + p_L2_list, dta.begin() + p_L2_data);
	// inset new one (empty)
	dta.insert(dta.begin() + p_L2_list, L2_count*8, 0);
	
	// new L2 sprites count
	data = dta.data() + p_L2_start;
	*(uint32_t*)data = L2_count;
	data += sizeof(uint32_t);

	// put new sprite names
	for(auto &sid: L2_list_new)
	{
		auto name = terr->m_sprite_names[sid];
		name.resize(8,'\0');
		memcpy(data,name.c_str(),8);
		data += 8;
	}

	// put new sprite index map
	for(auto &sid: L2_map)
	{
		if(sid < 0)
		{
			// no sprite
			*data++ = 0;
			data++;
			continue;
		}

		// reindex to used sprites
		auto nid = std::ranges::find(L2_list_new, sid);
		if(nid == L2_list_new.end())
		{
			// this should not happen
			m_last_error = string_format("Failed building map DTA file \"%s\"! Unknown error?",dta_name);
			return(1);
		}

		// replace sprite id
		*data++ = nid - L2_list_new.begin() + 1;
		// leave flags
		data++;
	}
	
	return(0);
}
