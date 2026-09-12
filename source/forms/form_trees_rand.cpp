///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include <wx/filedlg.h>
#include <wx/msgdlg.h>

#include "form_trees_rand.h"
#include "../other.h"
#include "../fs_archive.h"
#include "../sprites.h"
#include "../spell_randomizer.h"
#include "../spell_palette.h"
#include <algorithm>
#include <numeric>
#include <ranges>

// just dummy place holder
class SpellData{
public:
};

///////////////////////////////////////////////////////////////////////////


FormTreeRand::FormTreeRand(wxWindow* parent,wxWindowID id,const wxString& title,const wxPoint& pos,const wxSize& size,long style) : wxFrame(parent,id,title,pos,size,style)
{
	// <wxFormsBuilder> - Section auto-inserted from 'forms.cpp' class 'FormTreeRand' on 2026-09-09 20:00:31
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_MENU ) );
	
	sbar = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_SBAR );
	m_menubar2 = new wxMenuBar( 0 );
	m_menu6 = new wxMenu();
	wxMenuItem* mmLoadPreset;
	mmLoadPreset = new wxMenuItem( m_menu6, wxID_MM_LOAD_PRESET, wxString( _("Load preset") ) + wxT('\t') + wxT("Ctrl+O"), wxEmptyString, wxITEM_NORMAL );
	m_menu6->Append( mmLoadPreset );
	
	wxMenuItem* mmSavePreset;
	mmSavePreset = new wxMenuItem( m_menu6, wxID_MM_SAVE_PRESET, wxString( _("Save preset") ) + wxT('\t') + wxT("Ctrl+S"), wxEmptyString, wxITEM_NORMAL );
	m_menu6->Append( mmSavePreset );
	
	m_menu6->AppendSeparator();
	
	wxMenuItem* mmExit;
	mmExit = new wxMenuItem( m_menu6, wxID_MM_EXIT, wxString( _("Exit") ) , wxEmptyString, wxITEM_NORMAL );
	m_menu6->Append( mmExit );
	
	m_menubar2->Append( m_menu6, _("File") );
	
	this->SetMenuBar( m_menubar2 );
	
	wxBoxSizer* bSizer19;
	bSizer19 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText46 = new wxStaticText( this, wxID_ANY, _("Terrain class:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText46->Wrap( -1 );
	bSizer19->Add( m_staticText46, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	wxArrayString chTerrClassChoices;
	chTerrClass = new wxChoice( this, wxID_CH_TERR_CLASS, wxDefaultPosition, wxDefaultSize, chTerrClassChoices, 0 );
	chTerrClass->SetSelection( 0 );
	bSizer19->Add( chTerrClass, 0, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer88;
	bSizer88 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer106;
	bSizer106 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText67 = new wxStaticText( this, wxID_ANY, _("Rules (pop-up menu):"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText67->Wrap( -1 );
	bSizer106->Add( m_staticText67, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	listRules = new wxListCtrlVirtual(this,wxID_LIST_RULES,wxDefaultPosition,wxSize(150,-1),wxLC_NO_HEADER|wxLC_REPORT|wxLC_SINGLE_SEL|wxLC_VIRTUAL|wxALWAYS_SHOW_SB|wxVSCROLL|wxLC_EDIT_LABELS);
	bSizer106->Add( listRules, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	bSizer88->Add( bSizer106, 0, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer871;
	bSizer871 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText49 = new wxStaticText( this, wxID_ANY, _("Source trees (pop-up menu):"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText49->Wrap( -1 );
	bSizer871->Add( m_staticText49, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	wxArrayString lboxSourceTreesChoices;
	lboxSourceTrees = new wxCheckListBox( this, wxID_LBOX_SRC_TREES, wxDefaultPosition, wxDefaultSize, lboxSourceTreesChoices, wxLB_ALWAYS_SB );
	bSizer871->Add( lboxSourceTrees, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	bSizer88->Add( bSizer871, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer86;
	bSizer86 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText44 = new wxStaticText( this, wxID_ANY, _("Random trees (pop-up menu):"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText44->Wrap( -1 );
	bSizer86->Add( m_staticText44, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	wxArrayString lboxTreesChoices;
	lboxTrees = new wxCheckListBox( this, wxID_LBOX_TREES, wxDefaultPosition, wxDefaultSize, lboxTreesChoices, wxLB_ALWAYS_SB );
	bSizer86->Add( lboxTrees, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	bSizer88->Add( bSizer86, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer87;
	bSizer87 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText45 = new wxStaticText( this, wxID_ANY, _("Probabilities (pop-up menu):"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText45->Wrap( -1 );
	bSizer87->Add( m_staticText45, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	pgProbab = new wxPropertyGrid(this, wxID_PG_PROB, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE);
	bSizer87->Add( pgProbab, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	bSizer88->Add( bSizer87, 1, wxEXPAND, 5 );
	
	
	bSizer19->Add( bSizer88, 1, wxEXPAND, 5 );
	
	m_staticline18 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer19->Add( m_staticline18, 0, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* bSizer107;
	bSizer107 = new wxBoxSizer( wxHORIZONTAL );
	
	canvas = new wxPanel( this, wxID_SPR_CANVAS, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE|wxTAB_TRAVERSAL );
	canvas->SetMinSize( wxSize( 150,150 ) );
	
	bSizer107->Add( canvas, 0, wxEXPAND | wxALL, 5 );
	
	m_staticline20 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_VERTICAL );
	bSizer107->Add( m_staticline20, 0, wxEXPAND|wxTOP|wxBOTTOM, 5 );
	
	wxBoxSizer* bSizer108;
	bSizer108 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText48 = new wxStaticText( this, wxID_ANY, _("Randomizer global config:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText48->Wrap( -1 );
	bSizer108->Add( m_staticText48, 0, wxRIGHT|wxLEFT, 5 );
	
	pgConfig = new wxPropertyGrid(this, wxID_PG_CONFIG, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE);
	bSizer108->Add( pgConfig, 1, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );
	
	
	bSizer107->Add( bSizer108, 1, wxEXPAND, 5 );
	
	
	bSizer19->Add( bSizer107, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer19 );
	this->Layout();
	
	this->Centre( wxBOTH );
	

	// </wxFormsBuilder> - Section auto-inserted from 'forms.cpp' class 'FormTreeRand' on 2026-09-09 20:00:31
	// === AUTO GENERATED END ===

	RescaleWindowDPI(this);


	// set icon
	wxIcon appIcon;
	appIcon.LoadFile("IDI_ICON2",wxBITMAP_TYPE_ICO_RESOURCE);
	if(appIcon.IsOk())
		SetIcon(appIcon);

	mmLoadPreset->SetBitmaps(LoadSVGiconsBundle("IDR_OPEN"));
	mmSavePreset->SetBitmaps(LoadSVGiconsBundle("IDR_SAVE"));
	mmExit->SetBitmaps(LoadSVGiconsBundle("IDR_EXIT"));


	listRules->SetGetItemTextCb(std::bind(&FormTreeRand::OnGetRuleGroupItem,this,std::placeholders::_1));
	Bind(wxEVT_LIST_ITEM_SELECTED,&FormTreeRand::OnRuleGroupItemSelect,this,wxID_LIST_RULES);
	listRules->Connect(wxEVT_RIGHT_DOWN,wxMouseEventHandler(FormTreeRand::OnRulePupupOpen),NULL,this);
		
	Bind(wxEVT_COMMAND_MENU_SELECTED,&FormTreeRand::OnCloseClick,this,wxID_MM_EXIT);
	Bind(wxEVT_COMMAND_MENU_SELECTED,&FormTreeRand::OnSave,this,wxID_MM_SAVE_PRESET);
	Bind(wxEVT_COMMAND_MENU_SELECTED,&FormTreeRand::OnOpen,this,wxID_MM_LOAD_PRESET);
	Bind(wxEVT_CLOSE_WINDOW,&FormTreeRand::OnClose,this,wxID_ANY);

	Bind(wxEVT_COMMAND_CHOICE_SELECTED,&FormTreeRand::OnSelectTerrain,this,wxID_CH_TERR_CLASS);
	Bind(wxEVT_COMMAND_LISTBOX_SELECTED,&FormTreeRand::OnSelectSprite,this,wxID_LBOX_SRC_TREES);
	Bind(wxEVT_COMMAND_LISTBOX_SELECTED,&FormTreeRand::OnSelectSprite,this,wxID_LBOX_TREES);
	Bind(wxEVT_COMMAND_CHECKLISTBOX_TOGGLED,&FormTreeRand::OnSrcTreeClick,this,wxID_LBOX_SRC_TREES);
	Bind(wxEVT_COMMAND_CHECKLISTBOX_TOGGLED,&FormTreeRand::OnRandTreeClick,this,wxID_LBOX_TREES);	
	Bind(wxEVT_PG_SELECTED,&FormTreeRand::OnSelectProbSprite,this,wxID_PG_PROB);
	Bind(wxEVT_PG_CHANGED,&FormTreeRand::OnProbChange,this,wxID_PG_PROB);
	lboxSourceTrees->Bind(wxEVT_RIGHT_DOWN,&FormTreeRand::OnTreePupupOpen,this);
	lboxTrees->Bind(wxEVT_RIGHT_DOWN,&FormTreeRand::OnTreePupupOpen,this);
	
	
	listRules->Bind(wxEVT_LIST_END_LABEL_EDIT,&FormTreeRand::OnEndRuleLabelEdit,this);
	pgProbab->Bind(wxEVT_RIGHT_DOWN,&FormTreeRand::OnProbabPupupOpen,this);
	
	canvas->SetDoubleBuffered(true);
	canvas->Bind(wxEVT_PAINT,&FormTreeRand::OnCanvasRepaint,this);

	
	const int sizes[] = {-1, 150};
	sbar->SetFieldsCount(2,sizes);

	m_rules = NULL;
	m_rules_group = NULL;

}

FormTreeRand::~FormTreeRand()
{
}


// set terrains
int FormTreeRand::SetTerrains(std::vector<std::shared_ptr<FSarchive>> terrains,SpellTreeRandomizerRules* rules)
{
	m_last_error.clear();

	if(!rules)
	{
		m_last_error = string_format("Randomizer rules not provided!");
		return(1);
	}
	m_rules = rules;

	m_terrain_fs = terrains;

	// just dummy place holder
	static SpellData spell_data;

	// common palette
	SpellPalette pal;

	// load terrains
	chTerrClass->Freeze();
	chTerrClass->Clear();
	for(auto &terr_fs: terrains)
	{			
		if(iequals(terr_fs->m_fs_name,"COMMON.FS"))
		{
			// common.fs: load common palette chunks

			std::vector<uint8_t> chunk;
			if(terr_fs->GetFile("UNITS.PAL",chunk) || chunk.size() != 96*3)
			{
				m_last_error = string_format("Missing or corrupted palette: UNITS.PAL!");
				return(1); // missing item!
			}
			pal.Insert(chunk,"UNITS.PAL",128);
			
			// load SYSTEM.PAL palette chunk for maps
			if(terr_fs->GetFile("SYSTEM.PAL",chunk) || chunk.size() != 32*3)
			{
				m_last_error = string_format("Missing or corrupted palette: SYSTEM.PAL!");
				return(1); // missing item!
			}
			pal.Insert(chunk,"SYSTEM.PAL",224);

			continue;
		}
				
		// load terrain
		m_terrains.push_back(std::make_shared<Terrain>(spell_data));
		auto terr = m_terrains.back();
		auto map_pal = (uint8_t(*)[3])pal.m_pal.data();
		if(terr->Load(terr_fs.get(),map_pal,NULL,NULL))
		{
			// failed
			m_last_error = string_format("Failed loading terrain %s!",terr_fs->m_fs_name);
			return(1);
		}

		// add terrain to list if not there yet as preset
		chTerrClass->Append(terr->name);		
		auto rule = m_rules->GetTerrain(terr->name);
		if(!rule)
			rule = m_rules->AddTerrain(terr->name);
		if(!rule)
		{
			m_last_error = string_format("Failed creating terrain %s rules!",terr_fs->m_fs_name);
			return(1);
		}
		rule->terr = terr;

		// try load map editor toolset with trees
		auto toolset_info_path = GetExecutableDir() / "data" / string_format("trees_%s.info",terr->name);
		rule->map_toolset.LoadInfo(toolset_info_path, terr->name);
	}
	chTerrClass->Thaw();
	chTerrClass->Select(0);
	
	wxCommandEvent evt;
	OnSelectTerrain(evt);

	return(0);
}


// select tarrain class
void FormTreeRand::OnSelectTerrain(wxCommandEvent& event)
{		
	int rid = listRules->GetNextItem(-1,wxLIST_NEXT_ALL,wxLIST_STATE_SELECTED);
	listRules->ClearAll();
	lboxTrees->Clear();

	m_rules_group = m_rules->GetTerrain(chTerrClass->GetStringSelection().ToStdString());
	if(!m_rules_group)
		return;	
		
	int count = m_rules_group->rules.size();
	listRules->AppendColumn("list",wxLIST_FORMAT_LEFT,wxLIST_AUTOSIZE);
	listRules->SetItemCount(count);
	listRules->SetColumnWidth(0,wxLIST_AUTOSIZE_USEHEADER);	
	rid = std::min(rid,count - 1);
	if(rid < 0)
		rid = count - 1;
	if(rid >= 0)
	{
		listRules->SetItemState(rid,wxLIST_STATE_SELECTED,wxLIST_STATE_SELECTED);
		listRules->EnsureVisible(rid);
	}
		
	// fill target trees
	lboxTrees->Freeze();
	lboxTrees->Clear();
	for(auto& spr: m_rules_group->terr->sprites)
	{
		if(!wildcmp("STA_*",spr->name.c_str()))
			continue;		
		lboxTrees->Append(spr->name,(void*)spr);
	}
	lboxTrees->Thaw();
	
	//OnRandTreeClick(event);
	OnRuleGroupItemSelect(event);	
}

// render group item: get item name
wxString FormTreeRand::OnGetRuleGroupItem(long item_id)
{
	if(!m_rules_group)
		return("");
	if(item_id >= m_rules_group->rules.size())
		return("");
	return(m_rules_group->rules[item_id].name);
}

// rules popup menu
void FormTreeRand::OnRulePupupOpen(wxMouseEvent& event)
{
	wxMenu menu;
	int flags;
	auto pos = event.GetPosition();
	static auto sel_id = -1;
	sel_id = -1; // must be cleared before HitTest()!
	sel_id = listRules->HitTest(pos,flags);
	if(sel_id >= 0)
		listRules->SetItemState(sel_id,wxLIST_STATE_SELECTED,wxLIST_STATE_SELECTED);
	if(!m_rules_group || sel_id >= m_rules_group->rules.size())
		sel_id = -1;
	bool is_sel = (sel_id >= 0);
	menu.SetClientData(&sel_id);

	menu.Append((int)PopupActions::ADD_RULE,"Add new rule");
	if(sel_id >= 0)
	{
		menu.Append((int)PopupActions::REM_RULE,"Remove rule");
		menu.Append((int)PopupActions::RENAME_RULE,"Rename rule");		
	}

	menu.Connect(wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(FormTreeRand::OnRulePupup),NULL,this);
	PopupMenu(&menu);
}
void FormTreeRand::OnRulePupup(wxCommandEvent& event)
{
	auto menu_id = (PopupActions)event.GetId();
	auto menu = (wxMenu*)event.GetEventObject();
	if(!menu)
		return;
	auto sel_id = *(int*)menu->GetClientData();

	if(menu_id == PopupActions::ADD_RULE)
	{
		if(!m_rules_group)
			return;
		m_rules_group->AddRule();
	}
	else if(menu_id == PopupActions::REM_RULE)
	{
		if(!m_rules_group)
			return;
		if(sel_id < 0 || sel_id >= m_rules_group->rules.size())
			return;		
		m_rules_group->RemoveRule(m_rules_group->rules[sel_id].name);
	}
	else if(menu_id == PopupActions::RENAME_RULE)
	{
		if(!m_rules_group)
			return;
		if(sel_id < 0 || sel_id >= m_rules_group->rules.size())
			return;
		listRules->SetFocus();
		listRules->SetItemState(sel_id,wxLIST_STATE_SELECTED | wxLIST_STATE_FOCUSED, wxLIST_STATE_SELECTED | wxLIST_STATE_FOCUSED);
		this->CallAfter([this,sel_id]() {
			listRules->SetFocus();
			listRules->EditLabel(sel_id);
			});
	}

	OnSelectTerrain(event);
	listRules->Refresh();
}

// rename rule
void FormTreeRand::OnEndRuleLabelEdit(wxListEvent& event)
{
	if(!event.IsAllowed())
		return;

	auto rid = event.GetIndex();
	if(!m_rules_group || rid < 0 || rid >= m_rules_group->rules.size())
		return;

	if(event.GetText().empty())
		return;

	auto &rule = m_rules_group->rules[rid];
	rule.name = event.GetText();

	event.Skip();
}


// select rule
void FormTreeRand::OnRuleGroupItemSelect(wxCommandEvent& event)
{
	lboxSourceTrees->Clear();
	pgProbab->Clear();

	if(!m_rules_group)
		return;
	
	// select rule
	auto rid = listRules->GetNextItem(-1,wxLIST_NEXT_ALL,wxLIST_STATE_SELECTED);
	if(rid < 0 || rid >= m_rules_group->rules.size())
		return;
	auto &rule = m_rules_group->rules[rid];

	// fill available source trees
	lboxSourceTrees->Freeze();	
	auto used = m_rules_group->GetUsedTrees(&rule);
	for(auto &spr: m_rules_group->terr->sprites)
	{				
		if(!wildcmp("STA_*",spr->name.c_str()))
			continue;
		if(std::find(used.begin(),used.end(),spr->name) != used.end())
			continue;
		int id = lboxSourceTrees->Append(spr->name,(void*)spr);		
		auto item = rule.GetSrcTree(spr->name);
		lboxSourceTrees->Check(id,!!item);
	}
	lboxSourceTrees->Thaw();

	// fill available random trees
	lboxTrees->Freeze();
	for(int k = 0; k < lboxTrees->GetCount(); k++)
	{
		auto item = rule.GetRandTree(lboxTrees->GetString(k).ToStdString());
		lboxTrees->Check(k,!!item);
	}
	lboxTrees->Thaw();

	OnRandTreeClick(event);
}

// pick source tree
void FormTreeRand::OnSrcTreeClick(wxCommandEvent& event)
{
	if(!m_rules_group)
		return;

	auto rid = listRules->GetNextItem(-1,wxLIST_NEXT_ALL,wxLIST_STATE_SELECTED);
	if(rid < 0 || rid >= m_rules_group->rules.size())
		return;
	auto &rule = m_rules_group->rules[rid];
		
	for(int k = 0; k < lboxSourceTrees->GetCount(); k++)
	{
		auto is_sel = lboxSourceTrees->IsChecked(k);
		auto spr = (Sprite*)lboxSourceTrees->GetClientData(k);
		if(!spr)
			continue;
		rule.UpdateSrcTree(spr->name,is_sel);
	}

	auto sid = event.GetInt();
	if(sid >= 0 && sid < lboxSourceTrees->GetCount())
		lboxSourceTrees->SetSelection(sid);
	OnSelectSprite(event);
}


// pick random tree
void FormTreeRand::OnRandTreeClick(wxCommandEvent& event)
{
	pgProbab->Clear();

	if(!m_rules_group)
		return;

	auto rid = listRules->GetNextItem(-1,wxLIST_NEXT_ALL,wxLIST_STATE_SELECTED);
	if(rid < 0 || rid >= m_rules_group->rules.size())
		return;
	auto& rule = m_rules_group->rules[rid];

	pgProbab->Freeze();
	for(int k = 0; k < lboxTrees->GetCount(); k++)
	{
		auto is_sel = lboxTrees->IsChecked(k);
		auto spr = (Sprite*)lboxTrees->GetClientData(k);
		if(!spr)
			continue;
		rule.UpdateRandTree(spr->name,is_sel);

		if(!is_sel)
			continue;
		auto item = rule.GetRandTree(spr->name);
		if(!item)
			continue;
		pgProbab->Append(new wxRealPropertyExt(spr->name,wxT(""),&item->probab,1,0.0,100.0));
	}	
	pgProbab->Thaw();
	pgProbab->FitColumns();

	auto sid = event.GetInt();
	if(sid >= 0 && sid < lboxTrees->GetCount())
		lboxTrees->SetSelection(sid);
	OnSelectSprite(event);
}





// rules popup menu
void FormTreeRand::OnTreePupupOpen(wxMouseEvent& event)
{
	if(!m_rules_group)
		return;
	
	wxMenu menu;
	menu.SetClientData(event.GetEventObject());

	menu.Append((int)PopupActions::SEL_ALL,"Select all");
	menu.Append((int)PopupActions::CLR_ALL,"Clear all");

	std::vector<PopupActions> fun_list = {PopupActions::SEL_TOOL,PopupActions::CLR_TOOL};
	for(auto &fun: fun_list)
	{
		if(m_rules_group->map_toolset.m_tools.empty())
			continue;
		wxMenu *sub_menu = new wxMenu();		
		for(auto &tool: m_rules_group->map_toolset.m_tools)
			sub_menu->Append((int)fun + tool.id, tool.name);
		sub_menu->SetClientData(event.GetEventObject());
		menu.AppendSubMenu(sub_menu,((fun == PopupActions::SEL_TOOL)?"Select":"Clear"));
	}

	menu.Connect(wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(FormTreeRand::OnTreePupup),NULL,this);
	PopupMenu(&menu);
}
void FormTreeRand::OnTreePupup(wxCommandEvent& event)
{
	auto menu_id = (PopupActions)event.GetId();
	auto menu = (wxMenu*)event.GetEventObject();
	if(!menu)
		return;
	auto list = (wxCheckListBox*)menu->GetClientData();
	if(!list)
		return;
	
	if(menu_id == PopupActions::SEL_ALL)
	{
		for(int k = 0; k < list->GetCount(); k++)
			list->Check(k, true);
	}
	else if(menu_id == PopupActions::CLR_ALL)
	{
		for(int k = 0; k < list->GetCount(); k++)
			list->Check(k,false);
	}
	else if(menu_id >= PopupActions::SEL_TOOL && (int)menu_id < ((int)PopupActions::SEL_TOOL + 1000))
	{
		auto tool_id = (int)menu_id - (int)PopupActions::SEL_TOOL;
		if(!m_rules_group || tool_id < 0 || tool_id >= m_rules_group->map_toolset.m_tools.size())
			return;
		auto &tool = m_rules_group->map_toolset.m_tools[tool_id];
		for(int k = 0; k < list->GetCount(); k++)
			if(std::ranges::find(tool.sprites, list->GetString(k).ToStdString()) != tool.sprites.end())
				list->Check(k, true);
	}
	else if(menu_id >= PopupActions::CLR_TOOL && (int)menu_id < ((int)PopupActions::CLR_TOOL + 1000))
	{
		auto tool_id = (int)menu_id - (int)PopupActions::CLR_TOOL;
		if(!m_rules_group || tool_id < 0 || tool_id >= m_rules_group->map_toolset.m_tools.size())
			return;
		auto& tool = m_rules_group->map_toolset.m_tools[tool_id];
		for(int k = 0; k < list->GetCount(); k++)
			if(std::ranges::find(tool.sprites,list->GetString(k).ToStdString()) != tool.sprites.end())
				list->Check(k,false);
	}

	OnSrcTreeClick(event);
	OnRandTreeClick(event);	
}








// edit probability properties
void FormTreeRand::OnProbChange(wxPropertyGridEvent& event)
{
	auto pgrid = (wxPropertyGrid*)event.GetEventObject();
	if(!pgrid)
		return;

	auto prop = event.GetProperty();
	auto obj = (wxPGobj*)prop->GetClientObject();
	if(obj)
	{
		obj->Update(prop);		
	}
}

// probabilities popup menu
void FormTreeRand::OnProbabPupupOpen(wxMouseEvent& event)
{
	wxMenu menu;

	menu.Append((int)PopupActions::CLEAR_PROB,"Clear all");
	menu.Append((int)PopupActions::EQUAL_PROB,"Set equal");
	menu.Append((int)PopupActions::RAND_PROB,"Set random");
	menu.Append((int)PopupActions::FIX_PROB,"Fix to 100% sum");
	
	menu.Connect(wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(FormTreeRand::OnProbabPupup),NULL,this);
	PopupMenu(&menu);
}
void FormTreeRand::OnProbabPupup(wxCommandEvent& event)
{
	auto menu_id = (PopupActions)event.GetId();
	auto menu = (wxMenu*)event.GetEventObject();
	if(!menu)
		return;

	auto rid = listRules->GetNextItem(-1,wxLIST_NEXT_ALL,wxLIST_STATE_SELECTED);
	if(rid < 0 || rid >= m_rules_group->rules.size())
		return;
	auto& rule = m_rules_group->rules[rid];

	if(menu_id == PopupActions::EQUAL_PROB)
	{
		rule.SetEqualProb();
	}
	else if(menu_id == PopupActions::RAND_PROB)
	{
		rule.SetRandomProb();
	}
	else if(menu_id == PopupActions::CLEAR_PROB)
	{
		rule.ClearProb();
	}
	else if(menu_id == PopupActions::FIX_PROB)
	{
		rule.FixProb();
	}
	
	OnRuleGroupItemSelect(event);
}





// on select sprite
void FormTreeRand::OnSelectSprite(wxCommandEvent& event)
{
	if(event.GetId() != wxID_LBOX_TREES && event.GetId() != wxID_LBOX_SRC_TREES)
		return;
	auto list = (wxCheckListBox*)event.GetEventObject();
	if(!list)
		return;
	auto sid = list->GetSelection();
	if(sid < 0)
		return;
	auto spr = (Sprite*)list->GetClientData(sid);
	if(!spr)
		return;
	auto size = canvas->GetClientSize();
	m_bmp.reset(spr->Render((uint8_t*)spr->terr->pal,1.5,-1,-1));	
	canvas->Refresh();
}
void FormTreeRand::OnSelectProbSprite(wxPropertyGridEvent& event)
{
	auto prop = pgProbab->GetSelection();
	if(!prop)
		return;
	if(!m_rules_group)
		return;

	auto rid = listRules->GetNextItem(-1,wxLIST_NEXT_ALL,wxLIST_STATE_SELECTED);
	if(rid < 0 || rid >= m_rules_group->rules.size())
		return;
	auto& rule = m_rules_group->rules[rid];
	
	auto spr = m_rules_group->terr->GetSprite(prop->GetLabel().ToStdString().c_str());
	if(!spr)
		return;

	auto size = canvas->GetClientSize();
	m_bmp.reset(spr->Render((uint8_t*)spr->terr->pal,1.5,-1,-1,false));
	canvas->Refresh();
}

// repaint tree preview
void FormTreeRand::OnCanvasRepaint(wxPaintEvent& event)
{
	wxPaintDC pdc(canvas);
	if(m_bmp)
	{
		auto bmp_size = m_bmp->GetSize();
		auto canvas_size = canvas->GetClientSize();
		auto x_ofs = std::max((canvas_size.x - bmp_size.x)/2,0);
		auto y_ofs = std::max((canvas_size.y - bmp_size.y)/2,0);
		pdc.DrawBitmap(*m_bmp.get(),wxPoint(x_ofs,y_ofs));
	}
	else
		pdc.Clear();
}


// close window
void FormTreeRand::OnClose(wxCloseEvent& ev)
{
	ev.Skip();	
}

// on close form
void FormTreeRand::OnCloseClick(wxCommandEvent& event)
{	
	Close();
}

// on save file
void FormTreeRand::OnSave(wxCommandEvent& event)
{
	if(!m_rules_group)
		return;

	std::wstring dir = GetExecutableDir();
	std::wstring name = L"trees_randomizer_preset.info";
	if(!m_rules_group->m_path.empty())
	{
		dir = m_rules_group->m_path.parent_path();
		name = m_rules_group->m_path.filename();
	}
	wxFileDialog openFileDialog(this,"Save trees randomizer preset",dir,name,"Trees Randomizer preset file (*.info)|*.info",wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
	if(openFileDialog.ShowModal() == wxID_CANCEL)
		return;
	auto path = std::filesystem::path(openFileDialog.GetPath().ToStdWstring());

	if(m_rules->StorePreset(path,m_rules_group->name))
	{
		wxMessageBox("Saving trees randomizer preset failed!","Error",wxICON_ERROR);
	}
}

// on open file
void FormTreeRand::OnOpen(wxCommandEvent& event)
{
	if(!m_rules_group)
		return;

	std::wstring dir = GetExecutableDir();
	std::wstring name = L"trees_randomizer_preset.info";
	if(!m_rules_group->m_path.empty())
	{
		dir = m_rules_group->m_path.parent_path();
		name = m_rules_group->m_path.filename();
	}
	wxFileDialog openFileDialog(this,"Load trees randomizer preset",dir,name,"Trees Randomizer preset file (*.info)|*.info",wxFD_OPEN|wxFD_FILE_MUST_EXIST);
	if(openFileDialog.ShowModal() == wxID_CANCEL)
		return;
	auto path = std::filesystem::path(openFileDialog.GetPath().ToStdWstring());

	std::string terr_name;
	if(m_rules->LoadPreset(path,terr_name))
	{
		wxMessageBox("Loading trees randomizer preset failed!","Error",wxICON_ERROR);
	}

	chTerrClass->SetStringSelection(terr_name);
	OnSelectTerrain(event);
}


