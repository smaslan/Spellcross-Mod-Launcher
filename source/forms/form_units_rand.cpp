///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include <algorithm>
#include <numeric>

#include "../other.h"
#include "../wx_other.h"
#include "../fs_archive.h"
#include "../spell_units.h"
#include "../spell_randomizer.h"

#include "form_units_rand.h"

///////////////////////////////////////////////////////////////////////////

FormUnitRand::FormUnitRand(wxWindow* parent,UnitRandomizerSetup& randomizer,wxWindowID id,const wxString& title,const wxPoint& pos,const wxSize& size,long style) 
	: wxFrame(parent,id,title,pos,size,style),
	m_randomizer(randomizer)
{
	// <wxFormsBuilder> - Section auto-inserted from 'forms.cpp' class 'FormUnitRand' on 2026-08-22 09:02:19
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
	
	m_staticText46 = new wxStaticText( this, wxID_ANY, _("Source unit class:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText46->Wrap( -1 );
	bSizer19->Add( m_staticText46, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	wxArrayString chUnitClassChoices;
	chUnitClass = new wxChoice( this, wxID_CH_UNIT_CLASS, wxDefaultPosition, wxDefaultSize, chUnitClassChoices, 0 );
	chUnitClass->SetSelection( 0 );
	bSizer19->Add( chUnitClass, 0, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer88;
	bSizer88 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer871;
	bSizer871 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText49 = new wxStaticText( this, wxID_ANY, _("Source (applicable) units (pop-up menu):"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText49->Wrap( -1 );
	bSizer871->Add( m_staticText49, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	wxArrayString lboxSourceUnitsChoices;
	lboxSourceUnits = new wxCheckListBox( this, wxID_LBOX_SRC_UNITS, wxDefaultPosition, wxDefaultSize, lboxSourceUnitsChoices, wxLB_ALWAYS_SB );
	bSizer871->Add( lboxSourceUnits, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	bSizer88->Add( bSizer871, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer86;
	bSizer86 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText44 = new wxStaticText( this, wxID_ANY, _("Target units (pop-up menu):"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText44->Wrap( -1 );
	bSizer86->Add( m_staticText44, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	wxArrayString lboxUnitsChoices;
	lboxUnits = new wxCheckListBox( this, wxID_LBOX_UNITS, wxDefaultPosition, wxDefaultSize, lboxUnitsChoices, wxLB_ALWAYS_SB );
	bSizer86->Add( lboxUnits, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	cbLimitSave = new wxCheckBox( this, wxID_CB_FILTER_SAVE, _("Filter only units available in WORKDIR save?"), wxDefaultPosition, wxDefaultSize, 0 );
	cbLimitSave->SetValue(true);
	bSizer86->Add( cbLimitSave, 0, wxALL, 5 );
	
	m_staticText47 = new wxStaticText( this, wxID_ANY, _("Filter units:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText47->Wrap( -1 );
	bSizer86->Add( m_staticText47, 0, wxRIGHT|wxLEFT, 5 );
	
	wxArrayString chFilterChoices;
	chFilter = new wxChoice( this, wxID_CH_FILTER, wxDefaultPosition, wxDefaultSize, chFilterChoices, 0 );
	chFilter->SetSelection( 0 );
	bSizer86->Add( chFilter, 0, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );
	
	
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
	
	m_staticText48 = new wxStaticText( this, wxID_ANY, _("Randomizer global config:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText48->Wrap( -1 );
	bSizer19->Add( m_staticText48, 0, wxRIGHT|wxLEFT, 5 );
	
	pgConfig = new wxPropertyGrid(this, wxID_PG_CONFIG, wxDefaultPosition, wxDefaultSize, wxPG_DEFAULT_STYLE);
	bSizer19->Add( pgConfig, 0, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	
	this->SetSizer( bSizer19 );
	this->Layout();
	
	this->Centre( wxBOTH );
	

	// </wxFormsBuilder> - Section auto-inserted from 'forms.cpp' class 'FormUnitRand' on 2026-08-22 09:02:19
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

	// units classes
	chUnitClass->Append("Light");
	chUnitClass->Append("Armored");
	chUnitClass->Append("Air");
	chUnitClass->Select(0);
	m_rand_rule = &m_randomizer.rules_light;

	// units filter
	chFilter->Append("All");
	chFilter->Append("Other Side");
	chFilter->Append("Alliance");
	chFilter->Select(0);

	
	

	Bind(wxEVT_COMMAND_MENU_SELECTED,&FormUnitRand::OnCloseClick,this,wxID_MM_EXIT);
	Bind(wxEVT_COMMAND_MENU_SELECTED,&FormUnitRand::OnSave,this,wxID_MM_SAVE_PRESET);
	Bind(wxEVT_COMMAND_MENU_SELECTED,&FormUnitRand::OnOpen,this,wxID_MM_LOAD_PRESET);

	Bind(wxEVT_COMMAND_CHOICE_SELECTED,&FormUnitRand::OnChangeFilter,this,wxID_CH_FILTER);	
	Bind(wxEVT_COMMAND_CHOICE_SELECTED,&FormUnitRand::OnChangeUnitClass,this,wxID_CH_UNIT_CLASS);

	Bind(wxEVT_COMMAND_CHECKLISTBOX_TOGGLED,&FormUnitRand::OnSelectUnit,this,wxID_LBOX_UNITS);
	Bind(wxEVT_COMMAND_CHECKLISTBOX_TOGGLED,&FormUnitRand::OnUpdateSrcUnits,this,wxID_LBOX_SRC_UNITS);
	Bind(wxEVT_PG_CHANGED,&FormUnitRand::OnPGprobChange,this,wxID_PG_PROB);
	Bind(wxEVT_PG_CHANGED,&FormUnitRand::OnPGoptionsChange,this,wxID_PG_CONFIG);
		

	lboxUnits->Connect(wxEVT_RIGHT_DOWN,wxMouseEventHandler(FormUnitRand::OnUnitsPupupOpen),NULL,this);
	lboxSourceUnits->Connect(wxEVT_RIGHT_DOWN,wxMouseEventHandler(FormUnitRand::OnSrcUnitsPupupOpen),NULL,this);
	pgProbab->Connect(wxEVT_RIGHT_DOWN,wxMouseEventHandler(FormUnitRand::OnProbabPupupOpen),NULL,this);
		

	

	Bind(wxEVT_CLOSE_WINDOW,&FormUnitRand::OnClose,this,wxID_ANY);
	

	const int sizes[] = {-1, 150};
	sbar->SetFieldsCount(2,sizes);
}

FormUnitRand::~FormUnitRand()
{
}

// load common.fs
int FormUnitRand::SetCommon(std::filesystem::path common_path)
{
	// load common.fs
	m_last_error.clear();
	try{
		m_common_fs = std::make_unique<FSarchive>(common_path);
	}catch(const std::runtime_error& error) {
		m_last_error = string_format("Loading COMMON.FS archive failed (%s)!",error.what());
		return(1);
	}

	// load units def
	std::vector<uint8_t> data;
	if(m_common_fs->GetFile("JEDNOTKY.DEF",data))
	{
		m_last_error = string_format("Loading JEDNOTKY.DEF file failed!");
		return(1);
	}	
	try {
		m_units = std::make_unique<SpellUnits>(data.data(),data.size());
	}catch(const std::runtime_error& error) {
		m_last_error = string_format("Loading JEDNOTKY.DEF file failed (%s)!",error.what());
		return(1);
	}
	m_probab.assign(m_units->Count(),0);

	LoadOptions();
	wxCommandEvent event;
	OnChangeUnitClass(event);

	return(0);
}

// close window
void FormUnitRand::OnClose(wxCloseEvent& ev)
{
	if(m_modified)
	{
		wxMessageDialog dial(this,string_format("File was modified! Do you want to save it first?"),_("Open file..."),wxYES_NO | wxICON_QUESTION);
		if(dial.ShowModal() == wxID_YES)
		{
			wxCommandEvent event;
			OnSave(event);
		}
	}
	ev.Skip();
}

// on close form
void FormUnitRand::OnCloseClick(wxCommandEvent& event)
{	
	Close();
}

// on save file
void FormUnitRand::OnSave(wxCommandEvent& event)
{	
	std::wstring dir = GetExecutableDir();
	std::wstring name = L"randomizer.ini";
	if(!m_randomizer.m_path.empty())
	{
		dir = m_randomizer.m_path.parent_path();
		name = m_randomizer.m_path.filename();
	}
	wxFileDialog openFileDialog(this,"Save randomizer preset",dir,name,"Randomizer INI file (*.ini)|*.ini",wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
	if(openFileDialog.ShowModal() == wxID_CANCEL)
		return;
	auto path = std::filesystem::path(openFileDialog.GetPath().ToStdWstring());

	if(m_randomizer.SaveIni(path,m_units.get()))
	{
		wxMessageBox("Saving randomizer preset failed!","Error",wxICON_ERROR);
	}
}

// on open file
void FormUnitRand::OnOpen(wxCommandEvent& event)
{
	std::wstring dir = GetExecutableDir();
	std::wstring name = L"randomizer.ini";
	if(!m_randomizer.m_path.empty())
	{
		dir = m_randomizer.m_path.parent_path();
		name = m_randomizer.m_path.filename();
	}
	wxFileDialog openFileDialog(this,"Load randomizer preset",dir,name,"Randomizer INI file (*.ini)|*.ini",wxFD_OPEN|wxFD_FILE_MUST_EXIST);
	if(openFileDialog.ShowModal() == wxID_CANCEL)
		return;
	auto path = std::filesystem::path(openFileDialog.GetPath().ToStdWstring());

	if(m_randomizer.LoadIni(path,m_units.get()))
	{
		wxMessageBox("Loading randomizer preset failed!","Error",wxICON_ERROR);
	}

	LoadOptions();
	OnChangeUnitClass(event);
}

// reload options menu
void FormUnitRand::LoadOptions()
{
	// fill options
	pgConfig->Freeze();
	pgConfig->Clear();
	pgConfig->Append(new wxBoolPropertyExt("Apply to ToughDefence units",wxT(""),&m_randomizer.apply_tough_def));
	pgConfig->Append(new wxBoolPropertyExt("Apply to static enemies",wxT(""),&m_randomizer.apply_static));
	pgConfig->Append(new wxBoolPropertyExt("Apply to event-spawned enemies",wxT(""),&m_randomizer.apply_events));
	pgConfig->Append(new wxBoolPropertyExt("Override explicit rules",wxT(""),&m_randomizer.override_explicit_rule));
	pgConfig->Append(new wxBoolPropertyExt("Override disabled rules",wxT(""),&m_randomizer.override_off_rule));
	pgConfig->Append(new wxBoolPropertyExt("Enable XP level randomization",wxT(""),&m_randomizer.randomize_xp));
	pgConfig->Append(new wxIntPropertyExt("Min XP level",wxT(""),&m_randomizer.xp_min,1,12));
	pgConfig->Append(new wxIntPropertyExt("Max XP level",wxT(""),&m_randomizer.xp_max,1,12));
	pgConfig->Thaw();
	pgConfig->FitColumns();
	pgConfig->SetAutoLayout(true);
	setPGsize(pgConfig);
}

void FormUnitRand::OnChangeFilter(wxCommandEvent& event)
{
	LoadUnitList(lboxUnits);
	LoadUnitList(lboxSourceUnits);
}

// on change unit class
void FormUnitRand::OnChangeUnitClass(wxCommandEvent& event)
{
	auto sel = chUnitClass->GetStringSelection();
	if(sel == "Light")
		m_rand_rule = &m_randomizer.rules_light;
	else if(sel == "Armored")
		m_rand_rule = &m_randomizer.rules_armor;
	else if(sel == "Air")
		m_rand_rule = &m_randomizer.rules_air;

	m_probab.assign(m_units->Count(),0.0);
	for(int k = 0; k < m_rand_rule->unit_list.size(); k++)
	{
		auto uid = m_rand_rule->unit_list[k];
		auto* unit = m_units->GetUnit(uid);
		if(k < m_rand_rule->probab_list.size())
			m_probab[unit->type_id] = m_rand_rule->probab_list[k];
	}

	LoadUnitList(lboxSourceUnits);
	LoadUnitList(lboxUnits);
}

// show popup menu on probab list
void FormUnitRand::OnProbabPupupOpen(wxMouseEvent& event)
{
	wxMenu menu;
	menu.Append((int)ProbabPopup::CLEAR,"Clear all");
	menu.Append((int)ProbabPopup::UNIFORM,"Set all equal");
	menu.Append((int)ProbabPopup::RANDOM,"Set random");
	menu.Connect(wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(FormUnitRand::OnProbabPupup),NULL,this);
	PopupMenu(&menu);
}
void FormUnitRand::OnProbabPupup(wxCommandEvent& event)
{
	auto menu_id = (ProbabPopup)event.GetId();
	auto menu = (wxMenu*)event.GetEventObject();
	if(!menu)
		return;
	
	if(menu_id == ProbabPopup::CLEAR)
		m_probab.assign(m_units->Count(),0);
	else if(menu_id == ProbabPopup::UNIFORM)
	{
		int count = getPGcount(pgProbab);
		if(!count)
			return;
		for(int k = 0; k < lboxUnits->GetCount(); k++)
		{
			auto* unit = (SpellUnitRec*)lboxUnits->GetClientData(k);
			if(!unit)
				continue;
			if(!lboxUnits->IsChecked(k))
				continue;
			m_probab[k] = 100.0/count;
		}
	}
	else if(menu_id == ProbabPopup::RANDOM)
	{
		int count = getPGcount(pgProbab);
		if(!count)
			return;

		std::vector<double> rng(count);
		std::generate(rng.begin(),rng.end(),std::rand);
		double sum = std::accumulate(rng.begin(),rng.end(),0.0)/100.0;

		for(int k = 0; k < lboxUnits->GetCount(); k++)
		{
			auto* unit = (SpellUnitRec*)lboxUnits->GetClientData(k);
			if(!unit)
				continue;
			if(!lboxUnits->IsChecked(k))
				continue;
			m_probab[k] = rng.back()/sum;
			rng.pop_back();
		}
	}
	UpdateWList();
}

// show popup menu on units list
void FormUnitRand::OnUnitsPupupOpen(wxMouseEvent& event)
{	
	wxMenu menu;
	menu.SetClientData(event.GetEventObject());

	menu.Append((int)UnitsPopup::CLEAR,"Clear ALL");
	menu.Append((int)UnitsPopup::CLEAR_LIGHT,"Clear light");
	menu.Append((int)UnitsPopup::CLEAR_ARMOR,"Clear armored");
	menu.Append((int)UnitsPopup::CLEAR_AIR,"Clear air");
	menu.AppendSeparator();
	menu.Append((int)UnitsPopup::SET,"Select ALL");
	menu.Append((int)UnitsPopup::SET_LIGHT,"Select light");
	menu.Append((int)UnitsPopup::SET_ARMOR,"Select armored");
	menu.Append((int)UnitsPopup::SET_AIR,"Select air");
	menu.AppendSeparator();
	menu.Append((int)UnitsPopup::TOGGLE_LIGHT,"Toggle light");
	menu.Append((int)UnitsPopup::TOGGLE_ARMOR,"Toggle armored");
	menu.Append((int)UnitsPopup::TOGGLE_AIR,"Toggle air");
	
	menu.Connect(wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(FormUnitRand::OnUnitsPupup),NULL,this);
	PopupMenu(&menu);
}
void FormUnitRand::OnSrcUnitsPupupOpen(wxMouseEvent& event)
{
	wxMenu menu;
	menu.SetClientData(event.GetEventObject());

	menu.Append((int)UnitsPopup::SET,"Select ALL");
	menu.Append((int)UnitsPopup::CLEAR,"Clear ALL");
	menu.AppendSeparator();
	menu.Append((int)UnitsPopup::SET_ALLIANCE,"Select Alliance");	
	menu.Append((int)UnitsPopup::CLEAR_ALLIANCE,"Clear Alliance");
	menu.AppendSeparator();	
	menu.Append((int)UnitsPopup::SET_OS,"Select Other Side");
	menu.Append((int)UnitsPopup::CLEAR_OS,"Clear Other Side");	

	menu.Connect(wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(FormUnitRand::OnUnitsPupup),NULL,this);
	PopupMenu(&menu);
}
void FormUnitRand::OnUnitsPupup(wxCommandEvent& event)
{	
	auto menu_id = (UnitsPopup)event.GetId();
	auto menu = (wxMenu*)event.GetEventObject();
	if(!menu)
		return;
	auto lbox = (wxCheckListBox*)menu->GetClientData();
	bool is_src_units = lbox == lboxSourceUnits;
	bool is_clear = menu_id == UnitsPopup::CLEAR || menu_id == UnitsPopup::CLEAR_ARMOR || menu_id == UnitsPopup::CLEAR_LIGHT || menu_id == UnitsPopup::CLEAR_AIR || menu_id == UnitsPopup::CLEAR_ALLIANCE || menu_id == UnitsPopup::CLEAR_OS;
	bool is_set = menu_id == UnitsPopup::SET || menu_id == UnitsPopup::SET_ARMOR || menu_id == UnitsPopup::SET_LIGHT || menu_id == UnitsPopup::SET_AIR || menu_id == UnitsPopup::SET_ALLIANCE || menu_id == UnitsPopup::SET_OS;
	bool is_toggle = menu_id == UnitsPopup::TOGGLE_ARMOR || menu_id == UnitsPopup::TOGGLE_LIGHT || menu_id == UnitsPopup::TOGGLE_AIR;
	bool is_light = menu_id == UnitsPopup::SET || menu_id == UnitsPopup::CLEAR || menu_id == UnitsPopup::TOGGLE_LIGHT || menu_id == UnitsPopup::SET_LIGHT || menu_id == UnitsPopup::CLEAR_LIGHT;
	bool is_armor = menu_id == UnitsPopup::SET || menu_id == UnitsPopup::CLEAR || menu_id == UnitsPopup::TOGGLE_ARMOR || menu_id == UnitsPopup::SET_ARMOR || menu_id == UnitsPopup::CLEAR_ARMOR;
	bool is_air = menu_id == UnitsPopup::SET || menu_id == UnitsPopup::CLEAR || menu_id == UnitsPopup::TOGGLE_AIR || menu_id == UnitsPopup::SET_AIR || menu_id == UnitsPopup::CLEAR_AIR;
	bool is_alliance = menu_id == UnitsPopup::SET_ALLIANCE || menu_id == UnitsPopup::CLEAR_ALLIANCE;
	bool is_os = menu_id == UnitsPopup::SET_OS || menu_id == UnitsPopup::CLEAR_OS;
	
	
	for(int k = 0; k < lbox->GetCount(); k++)
	{
		auto* unit = (SpellUnitRec*)lbox->GetClientData(k);
		if(!unit)
			continue;
		bool state = lbox->IsChecked(k);
		if(!(unit->isLight() && is_light || unit->isArmored() && is_armor || unit->isAir() && is_air || unit->isAlliance() && is_alliance || unit->isOS() && is_os))
			continue;
		if(is_set)
			state = true;
		if(is_clear)
			state = false;
		if(is_toggle)
			state = !state;		
		lbox->Check(k,state);
	}
	
	if(is_src_units)
		OnUpdateSrcUnits(event);
	else
		UpdateWList();
}


// load list of units
void FormUnitRand::LoadUnitList(wxCheckListBox *lbox)
{
	bool is_src_unit = (lbox == lboxSourceUnits);

	// filter unit by side
	auto filt = chFilter->GetStringSelection();
	bool filt_os = filt == "All" || filt == "Other Side" || is_src_unit;
	bool filt_alliance = filt == "All" || filt == "Alliance" || is_src_unit;

	// unit class
	auto sel = chUnitClass->GetStringSelection();
		
	lbox->Freeze();
	lbox->Clear();
	for(auto &unit: m_units->GetUnits())
	{
		if(!unit->canAttack())
			continue;
		if(!unit->isMobile())
			continue;
		if(unit->isOS() && !filt_os)
			continue;
		if(unit->isAlliance() && !filt_alliance)
			continue;
		if(is_src_unit && !(sel == "Light" && unit->isLight() || (sel == "Armored" && unit->isArmored()) || (sel == "Air" && unit->isAir())))
			continue;
		
		auto label = wstring_format(L"%ls",unit->name.c_str());
		lbox->Append(label, (void*)unit.get());
		
		bool is_selected;
		if(is_src_unit)
			is_selected = m_rand_rule->isSrcUnit(unit->type_id);
		else
			is_selected = m_rand_rule->isTargetUnit(unit->type_id);
		lbox->Check(lbox->GetCount() - 1, is_selected);
	}
	lbox->Thaw();
	
	if(!is_src_unit)
		UpdateWList();
}

// unit picked
void FormUnitRand::OnSelectUnit(wxCommandEvent& event)
{
	UpdateWList();
}

// update w-list of selected units
void FormUnitRand::UpdateWList()
{
	pgProbab->Freeze();
	pgProbab->Clear();
	m_rand_rule->unit_list.clear();
	m_rand_rule->probab_list.clear();
	for(int k = 0; k < lboxUnits->GetCount(); k++)
	{
		auto* unit = (SpellUnitRec*)lboxUnits->GetClientData(k);
		if(!unit)
			continue;
		if(!lboxUnits->IsChecked(k))
			continue;
		auto name = lboxUnits->GetString(k);		
		pgProbab->Append(new wxRealPropertyExt(name,wxT(""),&m_probab[k],1,0.0,100.0));
		m_rand_rule->unit_list.push_back(unit->type_id);
		m_rand_rule->probab_list.push_back(m_probab[k]);
	}
	pgProbab->Thaw();
	pgProbab->FitColumns();
}

// update w-list of selected units
void FormUnitRand::OnUpdateSrcUnits(wxCommandEvent& event)
{
	m_rand_rule->src_unit_list.clear();
	for(int k = 0; k < lboxSourceUnits->GetCount(); k++)
	{
		auto* unit = (SpellUnitRec*)lboxSourceUnits->GetClientData(k);
		if(!unit)
			continue;
		if(!lboxSourceUnits->IsChecked(k))
			continue;
		m_rand_rule->src_unit_list.push_back(unit->type_id);
	}
}

// edit options
void FormUnitRand::OnPGoptionsChange(wxPropertyGridEvent& event)
{
	auto pgrid = (wxPropertyGrid*)event.GetEventObject();
	if(!pgrid)
		return;
	auto prop = event.GetProperty();
	auto obj = (wxPGobj*)prop->GetClientObject();
	if(obj)
		obj->Update(prop);
}

// edit level properties
void FormUnitRand::OnPGprobChange(wxPropertyGridEvent& event)
{
	auto pgrid = (wxPropertyGrid*)event.GetEventObject();
	if(!pgrid)
		return;

	auto prop = event.GetProperty();
	auto obj = (wxPGobj*)prop->GetClientObject();
	if(obj)
	{
		obj->Update(prop);		
		int ref_id = (double*)obj->m_data - m_probab.data();
		FixProbs(ref_id);
		UpdateWList();
	}
}

void FormUnitRand::FixProbs(int ref_id)
{
	double ref_prob = m_probab[ref_id];
	
	for(int m = 0; m < 10; m++)
	{
		double other_prob = 0.0;
		int count = 0;
		for(int k = 0; k < lboxUnits->GetCount(); k++)
		{
			auto* unit = (SpellUnitRec*)lboxUnits->GetClientData(k);
			if(!unit)
				continue;
			if(!lboxUnits->IsChecked(k))
				continue;
			if(unit->type_id == ref_id)
				continue;		
			double prob = m_probab[unit->type_id];			
			other_prob += prob;
			count++;
		}

		if(!count)
		{
			m_probab[ref_id] = 100;
			break;
		}

		double delta = 100.0 - ref_prob - other_prob;

		
		for(int k = 0; k < lboxUnits->GetCount(); k++)
		{
			auto* unit = (SpellUnitRec*)lboxUnits->GetClientData(k);
			if(!unit)
				continue;
			if(!lboxUnits->IsChecked(k))
				continue;
			if(unit->type_id == ref_id)
				continue;
			double org_prob = m_probab[unit->type_id];
			double step = delta/count;
			double prob = std::min(std::max(0.0,org_prob + step),100.0);
			delta -= (prob - org_prob);
			count--;
			m_probab[unit->type_id] = prob;			
		}

	}
}
