//=============================================================================
// Spellcross Mod Launcher
// ----------------------------------------------------------------------------
// Top level functions, wxWidgets GUI.
// 
// This code is part of Spellcross Mod Launcher project.
// (c) 2026, Stanislav Maslan, s.maslan@seznam.cz
// url: https://github.com/smaslan/Spellcross-Mod-Launcher
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================
// 
// For compilers that support precompilation, includes "wx/wx.h".
//#define wxMSVC_VERSION_ABI_COMPAT
#include <wx/wxprec.h>
//#include <wx/msw/wx.rc>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

/*#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")*/

#include <wx/timer.h>
#include <wx/filedlg.h>
#include <wx/stdpaths.h>
#include <wx/timer.h>
#include <wx/volume.h>

#include <filesystem>
#include <string>

#include "forms/form_about.h"
#include "forms/form_edit.h"

#include "main.h"
#include "other.h"
#include "wx_other.h"
#include "simpleini.h"
#include "SpellMod.h"
#include "SpellLaunch.h"


wxIMPLEMENT_APP(MyApp);
bool MyApp::OnInit()
{
    // for saving PNG file (among other stuff)
    wxInitAllImageHandlers();

	//InitCommonControls();

    // load config.ini
    //ini.SetUnicode();
    ini.LoadFile("config.ini");      

    // default window size
    int win_x_size = ini.GetLongValue("STATE","win_x_size",900);
    int win_y_size = ini.GetLongValue("STATE","win_y_size",650);
    bool win_maximize = ini.GetBoolValue("STATE","win_maximize",false);

    // limit to screen size
    int disp_x_size;
    int disp_y_size;
    wxDisplaySize(&disp_x_size,&disp_y_size);
    win_x_size = std::min(win_x_size,disp_x_size);
    win_y_size = std::min(win_y_size,disp_y_size);

    // --- run main form    
    // main window frame
    FormMain* frame = new FormMain(NULL,&ini);
    frame->SetSize(win_x_size,win_y_size);
    if(win_maximize)
        frame->Maximize();

    // set icon
    wxIcon appIcon;
    appIcon.LoadFile("IDI_ICON2",wxBITMAP_TYPE_ICO_RESOURCE);
    if(appIcon.IsOk())
        frame->SetIcon(appIcon);

    frame->Center();
    // show main frame
    frame->Show(true);
    return(true);
}

// exit app
int MyApp::OnExit()
{
    // save INI
    ini.SaveFile("config.ini");

    return(0);
}



FormMain::FormMain(wxWindow* parent,CSimpleIniA* ini,wxWindowID id,const wxString& title,const wxPoint& pos,const wxSize& size,long style) : wxFrame(parent,id,title,pos,size,style)
{
	this->SetSizeHints(wxSize(900,650),wxDefaultSize);
	this->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT));
	this->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_MENU));

	mMenu = new wxMenuBar(0);
	mmFile = new wxMenu();
	wxMenuItem* mmModPath;
	mmModPath = new wxMenuItem(mmFile,wxID_MM_MOD_PATH,wxString(_("Select mod")) + wxT('\t') + wxT("Ctrl+O"),wxEmptyString,wxITEM_NORMAL);
	mmFile->Append(mmModPath);

	mmFile->AppendSeparator();

	wxMenuItem* mmSpellPath;
	mmSpellPath = new wxMenuItem(mmFile,wxID_MM_SPELL_PATH,wxString(_("Set spellcross path")),wxEmptyString,wxITEM_NORMAL);
	mmFile->Append(mmSpellPath);

	wxMenuItem* mmSpellcdPath;
	mmSpellcdPath = new wxMenuItem(mmFile,wxID_MM_SPELLCD_PATH,wxString(_("Set Spellcross CD path")),wxEmptyString,wxITEM_NORMAL);
	mmFile->Append(mmSpellcdPath);

	mmFile->AppendSeparator();

	wxMenuItem* mmDosboxPath;
	mmDosboxPath = new wxMenuItem(mmFile,wxID_MM_DOSBOX_PATH,wxString(_("Set DOSbox path")),wxEmptyString,wxITEM_NORMAL);
	mmFile->Append(mmDosboxPath);

	mmFile->AppendSeparator();

	wxMenuItem* mmExit;
	mmExit = new wxMenuItem(mmFile,wxID_MM_EXIT,wxString(_("Exit")) + wxT('\t') + wxT("Alt+F4"),wxEmptyString,wxITEM_NORMAL);
	mmFile->Append(mmExit);

	mMenu->Append(mmFile,_("File"));

	mmSetup = new wxMenu();
	wxMenuItem* mmEditSpellCfg;
	mmEditSpellCfg = new wxMenuItem(mmSetup,wxID_MM_EDIT_SPELLCFG,wxString(_("Edit SPELCROS.CFG")),wxEmptyString,wxITEM_NORMAL);
	mmSetup->Append(mmEditSpellCfg);

	wxMenuItem* mmEditDOSboxCfg;
	mmEditDOSboxCfg = new wxMenuItem(mmSetup,wxID_MM_EDIT_DOSBOX_CFG,wxString(_("Edit DOSbox config file")),wxEmptyString,wxITEM_NORMAL);
	mmSetup->Append(mmEditDOSboxCfg);

	mmSetup->AppendSeparator();

	wxMenuItem* mmRunSetupBat;
	mmRunSetupBat = new wxMenuItem(mmSetup,wxID_MM_RUN_SETUPBAT,wxString(_("Run game SETUP.BAT")),wxEmptyString,wxITEM_NORMAL);
	mmSetup->Append(mmRunSetupBat);

	mMenu->Append(mmSetup,_("Setup"));

	mmMod = new wxMenu();
	wxMenuItem* mmModInfo;
	mmModInfo = new wxMenuItem(mmMod,wxID_MM_MOD_INFO,wxString(_("Show mod info")) + wxT('\t') + wxT("Ctrl+I"),wxEmptyString,wxITEM_NORMAL);
	mmMod->Append(mmModInfo);

	mmMod->AppendSeparator();

	wxMenuItem* mmModBuild;
	mmModBuild = new wxMenuItem(mmMod,wxID_MM_MOD_BUILD,wxString(_("Build mod")) + wxT('\t') + wxT("F2"),wxEmptyString,wxITEM_NORMAL);
	mmMod->Append(mmModBuild);

	wxMenuItem* mmModBuildSwap;
	mmModBuildSwap = new wxMenuItem(mmMod,wxID_MM_MOD_BUILD_SWAP,wxString(_("Build and swap")) + wxT('\t') + wxT("Shift+F2"),wxEmptyString,wxITEM_NORMAL);
	mmMod->Append(mmModBuildSwap);

	wxMenuItem* mmModRestore;
	mmModRestore = new wxMenuItem(mmMod,wxID_MM_MOD_RESTORE,wxString(_("Restore original")) + wxT('\t') + wxT("F3"),wxEmptyString,wxITEM_NORMAL);
	mmMod->Append(mmModRestore);

	wxMenuItem* mmModCleanup;
	mmModCleanup = new wxMenuItem(mmMod,wxID_MM_MOD_CLEAN,wxString(_("Cleanup mod config")),wxEmptyString,wxITEM_NORMAL);
	mmMod->Append(mmModCleanup);

	mmMod->AppendSeparator();

	wxMenuItem* mmEditModDEF;
	mmEditModDEF = new wxMenuItem(mmMod,wxID_MM_EDIT_MOD_DEF,wxString(_("Edit mod DEF file")),wxEmptyString,wxITEM_NORMAL);
	mmMod->Append(mmEditModDEF);

	mMenu->Append(mmMod,_("Mod"));

	m_menu7 = new wxMenu();
	msmSaveOrig = new wxMenu();
	wxMenuItem* msmSaveOrigItem = new wxMenuItem(m_menu7,wxID_ANY,_("Original game saves"),wxEmptyString,wxITEM_NORMAL,msmSaveOrig);
	wxMenuItem* mmSaveWDorg;
	mmSaveWDorg = new wxMenuItem(msmSaveOrig,wxID_MM_SAVE_WD_ORG,wxString(_("Backup WORKDIR")),wxEmptyString,wxITEM_NORMAL);
	msmSaveOrig->Append(mmSaveWDorg);

	wxMenuItem* mmRestoreWDorg;
	mmRestoreWDorg = new wxMenuItem(msmSaveOrig,wxID_MM_RESTORE_WD_ORG,wxString(_("Restore WORKDIR")),wxEmptyString,wxITEM_NORMAL);
	msmSaveOrig->Append(mmRestoreWDorg);

	msmSaveOrig->AppendSeparator();

	wxMenuItem* mmSaveOrig;
	mmSaveOrig = new wxMenuItem(msmSaveOrig,wxID_MM_SAVE_ORIG,wxString(_("Backup SAVEGAME")),wxEmptyString,wxITEM_NORMAL);
	msmSaveOrig->Append(mmSaveOrig);

	wxMenuItem* mmRestoreSaveOrig;
	mmRestoreSaveOrig = new wxMenuItem(msmSaveOrig,wxID_RESTORE_SAVE_ORIG,wxString(_("Restore SAVEGAME")),wxEmptyString,wxITEM_NORMAL);
	msmSaveOrig->Append(mmRestoreSaveOrig);

	m_menu7->Append(msmSaveOrigItem);

	msmSaveMod = new wxMenu();
	wxMenuItem* msmSaveModItem = new wxMenuItem(m_menu7,wxID_ANY,_("Mod save games"),wxEmptyString,wxITEM_NORMAL,msmSaveMod);
	wxMenuItem* mmSaveWDmod;
	mmSaveWDmod = new wxMenuItem(msmSaveMod,wxID_MM_SAVE_WD_MOD,wxString(_("Backup WORKDIR")),wxEmptyString,wxITEM_NORMAL);
	msmSaveMod->Append(mmSaveWDmod);

	wxMenuItem* mmRestoreWDmod;
	mmRestoreWDmod = new wxMenuItem(msmSaveMod,wxID_MM_RESTORE_WD_MOD,wxString(_("Restore WORKDIR")),wxEmptyString,wxITEM_NORMAL);
	msmSaveMod->Append(mmRestoreWDmod);

	msmSaveMod->AppendSeparator();

	wxMenuItem* mmSaveMod;
	mmSaveMod = new wxMenuItem(msmSaveMod,wxID_MM_SAVE_MOD,wxString(_("Backup SAVEGAME")),wxEmptyString,wxITEM_NORMAL);
	msmSaveMod->Append(mmSaveMod);

	wxMenuItem* mmRestoreSaveMod;
	mmRestoreSaveMod = new wxMenuItem(msmSaveMod,wxID_RESTORE_SAVE_MOD,wxString(_("Restore SAVEGAME")),wxEmptyString,wxITEM_NORMAL);
	msmSaveMod->Append(mmRestoreSaveMod);

	m_menu7->Append(msmSaveModItem);

	mMenu->Append(m_menu7,_("Savegame"));

	mmRun = new wxMenu();
	wxMenuItem* mmBuildLaunch;
	mmBuildLaunch = new wxMenuItem(mmRun,wxID_MM_BUILD_LAUNCH,wxString(_("Build launch files")),wxEmptyString,wxITEM_NORMAL);
	mmRun->Append(mmBuildLaunch);

	mmRun->AppendSeparator();

	wxMenuItem* mmRunOrig;
	mmRunOrig = new wxMenuItem(mmRun,wxID_MM_RUN_ORIG,wxString(_("Run original")) + wxT('\t') + wxT("F5"),wxEmptyString,wxITEM_NORMAL);
	mmRun->Append(mmRunOrig);

	wxMenuItem* mmRunMod;
	mmRunMod = new wxMenuItem(mmRun,wxID_MM_RUN_MOD,wxString(_("Run mod")) + wxT('\t') + wxT("F7"),wxEmptyString,wxITEM_NORMAL);
	mmRun->Append(mmRunMod);

	mMenu->Append(mmRun,_("Run"));

	mmHelpMenu = new wxMenu();
	wxMenuItem* mmHelp;
	mmHelp = new wxMenuItem(mmHelpMenu,wxID_MM_HELP,wxString(_("Help")) + wxT('\t') + wxT("F1"),wxEmptyString,wxITEM_NORMAL);
	mmHelpMenu->Append(mmHelp);

	wxMenuItem* mmAbout;
	mmAbout = new wxMenuItem(mmHelpMenu,wxID_MM_ABOUT,wxString(_("About")),wxEmptyString,wxITEM_NORMAL);
	mmHelpMenu->Append(mmAbout);

	mMenu->Append(mmHelpMenu,_("Help"));

	this->SetMenuBar(mMenu);

	sbar = this->CreateStatusBar(1,wxSTB_SIZEGRIP,wxID_SBAR);
	wxBoxSizer* bSizer114;
	bSizer114 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer(wxVERTICAL);

	m_staticText115 = new wxStaticText(this,wxID_ANY,_("Spellcross installation folder:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText115->Wrap(-1);
	bSizer17->Add(m_staticText115,0,wxTOP|wxRIGHT|wxLEFT,5);

	wxBoxSizer* bSizer211;
	bSizer211 = new wxBoxSizer(wxHORIZONTAL);

	wxArrayString chSpellPathChoices;
	chSpellPath = new wxChoice(this,wxID_CH_SPELL_PATH,wxDefaultPosition,wxDefaultSize,chSpellPathChoices,0);
	chSpellPath->SetSelection(0);
	bSizer211->Add(chSpellPath,1,wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT,5);

	btnSpellPath = new wxBitmapButton(this,wxID_BTN_SPELL_PATH,wxNullBitmap,wxDefaultPosition,wxDefaultSize,wxBU_AUTODRAW|wxBORDER_NONE);
	bSizer211->Add(btnSpellPath,0,wxRIGHT,5);


	bSizer17->Add(bSizer211,1,wxEXPAND,5);


	bSizer16->Add(bSizer17,1,wxEXPAND,5);

	wxBoxSizer* bSizer171;
	bSizer171 = new wxBoxSizer(wxVERTICAL);

	bSizer171->SetMinSize(wxSize(200,-1));
	m_staticText1151 = new wxStaticText(this,wxID_ANY,_("Spellcross executable:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText1151->Wrap(-1);
	bSizer171->Add(m_staticText1151,0,wxTOP|wxRIGHT|wxLEFT,5);

	wxArrayString chSpellExecChoices;
	chSpellExec = new wxChoice(this,wxID_CH_SPELL_EXE,wxDefaultPosition,wxDefaultSize,chSpellExecChoices,0);
	chSpellExec->SetSelection(0);
	bSizer171->Add(chSpellExec,0,wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT,5);


	bSizer16->Add(bSizer171,0,wxEXPAND,5);


	bSizer114->Add(bSizer16,0,wxEXPAND,5);

	wxBoxSizer* bSizer115;
	bSizer115 = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* bSizer116;
	bSizer116 = new wxBoxSizer(wxVERTICAL);

	m_staticText116 = new wxStaticText(this,wxID_ANY,_("Spellcross CD path (if available):"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText116->Wrap(-1);
	bSizer116->Add(m_staticText116,0,wxRIGHT|wxLEFT,5);

	wxBoxSizer* bSizer221;
	bSizer221 = new wxBoxSizer(wxHORIZONTAL);

	wxArrayString chSpellCdPathChoices;
	chSpellCdPath = new wxChoice(this,wxID_CH_SPELLCD_PATH,wxDefaultPosition,wxDefaultSize,chSpellCdPathChoices,0);
	chSpellCdPath->SetSelection(0);
	bSizer221->Add(chSpellCdPath,1,wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT,5);

	btnSpellcdPath = new wxBitmapButton(this,wxID_BTN_SPELLCD_PATH,wxNullBitmap,wxDefaultPosition,wxDefaultSize,wxBU_AUTODRAW|wxBORDER_NONE);
	bSizer221->Add(btnSpellcdPath,0,wxRIGHT,5);


	bSizer116->Add(bSizer221,1,wxEXPAND,5);


	bSizer115->Add(bSizer116,1,wxEXPAND,5);

	wxBoxSizer* bSizer117;
	bSizer117 = new wxBoxSizer(wxVERTICAL);

	bSizer117->SetMinSize(wxSize(200,-1));
	m_staticText117 = new wxStaticText(this,wxID_ANY,_("Virtual CD letter:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText117->Wrap(-1);
	bSizer117->Add(m_staticText117,0,wxRIGHT|wxLEFT,5);

	wxArrayString chVirtCDChoices;
	chVirtCD = new wxChoice(this,wxID_CH_CD_LETTER,wxDefaultPosition,wxSize(-1,-1),chVirtCDChoices,0);
	chVirtCD->SetSelection(0);
	bSizer117->Add(chVirtCD,0,wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND,5);


	bSizer115->Add(bSizer117,0,wxEXPAND,5);


	bSizer114->Add(bSizer115,0,wxEXPAND,5);

	m_staticline42 = new wxStaticLine(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLI_HORIZONTAL);
	bSizer114->Add(m_staticline42,0,wxEXPAND | wxALL,5);

	wxBoxSizer* bSizer118;
	bSizer118 = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* bSizer119;
	bSizer119 = new wxBoxSizer(wxVERTICAL);

	m_staticText118 = new wxStaticText(this,wxID_ANY,_("DOSbox path:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText118->Wrap(-1);
	bSizer119->Add(m_staticText118,0,wxRIGHT|wxLEFT,5);

	wxBoxSizer* bSizer23;
	bSizer23 = new wxBoxSizer(wxHORIZONTAL);

	wxArrayString chDOSboxPathChoices;
	chDOSboxPath = new wxChoice(this,wxID_CH_DOSBOX_PATH,wxDefaultPosition,wxDefaultSize,chDOSboxPathChoices,0);
	chDOSboxPath->SetSelection(0);
	bSizer23->Add(chDOSboxPath,1,wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT,5);

	btnDOSboxPath = new wxBitmapButton(this,wxID_BTN_DB_PATH,wxNullBitmap,wxDefaultPosition,wxDefaultSize,wxBU_AUTODRAW|wxBORDER_NONE);
	bSizer23->Add(btnDOSboxPath,0,wxRIGHT,5);


	bSizer119->Add(bSizer23,1,wxEXPAND,5);


	bSizer118->Add(bSizer119,1,wxEXPAND,5);

	wxBoxSizer* bSizer120;
	bSizer120 = new wxBoxSizer(wxVERTICAL);

	bSizer120->SetMinSize(wxSize(180,-1));
	cbNoAutoexec = new wxCheckBox(this,wxID_CB_NO_AUTOEXEC,_("Disable autoexec.bat"),wxDefaultPosition,wxDefaultSize,0);
	bSizer120->Add(cbNoAutoexec,0,wxALL,5);

	cbFullscreen = new wxCheckBox(this,wxID_CB_FULLSCREEN,_("Start fullscreen"),wxDefaultPosition,wxDefaultSize,0);
	bSizer120->Add(cbFullscreen,0,wxBOTTOM|wxRIGHT|wxLEFT,5);


	bSizer118->Add(bSizer120,0,wxEXPAND,5);

	m_staticline4 = new wxStaticLine(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLI_VERTICAL);
	bSizer118->Add(m_staticline4,0,wxEXPAND,5);

	wxBoxSizer* bSizer21;
	bSizer21 = new wxBoxSizer(wxVERTICAL);

	bSizer21->SetMinSize(wxSize(200,-1));
	m_staticText9 = new wxStaticText(this,wxID_ANY,_("Run mode:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText9->Wrap(-1);
	bSizer21->Add(m_staticText9,0,wxRIGHT|wxLEFT,5);

	wxArrayString chRunModeChoices;
	chRunMode = new wxChoice(this,wxID_CH_RUN_MODE,wxDefaultPosition,wxDefaultSize,chRunModeChoices,0);
	chRunMode->SetSelection(0);
	bSizer21->Add(chRunMode,0,wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT,5);


	bSizer118->Add(bSizer21,0,wxEXPAND,5);


	bSizer114->Add(bSizer118,0,wxEXPAND,5);

	m_staticline43 = new wxStaticLine(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLI_HORIZONTAL);
	bSizer114->Add(m_staticline43,0,wxEXPAND | wxALL,5);

	wxBoxSizer* bSizer121;
	bSizer121 = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* bSizer22;
	bSizer22 = new wxBoxSizer(wxVERTICAL);

	m_staticText119 = new wxStaticText(this,wxID_ANY,_("Mod path:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText119->Wrap(-1);
	bSizer22->Add(m_staticText119,0,wxRIGHT|wxLEFT,5);

	wxBoxSizer* bSizer24;
	bSizer24 = new wxBoxSizer(wxHORIZONTAL);

	wxArrayString chModPathChoices;
	chModPath = new wxChoice(this,wxID_CH_MOD_PATH,wxDefaultPosition,wxDefaultSize,chModPathChoices,0);
	chModPath->SetSelection(0);
	bSizer24->Add(chModPath,1,wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT,5);

	btnModPath = new wxBitmapButton(this,wxID_BTN_MOD_PATH,wxNullBitmap,wxDefaultPosition,wxDefaultSize,wxBU_AUTODRAW|wxBORDER_NONE);
	bSizer24->Add(btnModPath,0,wxRIGHT,5);


	bSizer22->Add(bSizer24,1,wxEXPAND,5);


	bSizer121->Add(bSizer22,1,wxEXPAND,5);

	wxBoxSizer* bSizer122;
	bSizer122 = new wxBoxSizer(wxVERTICAL);

	bSizer122->SetMinSize(wxSize(200,-1));
	cbAllowCDmod = new wxCheckBox(this,wxID_CB_ALLOW_CD_MOD,_("Allow CD mod"),wxDefaultPosition,wxSize(-1,-1),0);
	bSizer122->Add(cbAllowCDmod,0,wxALL,5);

	cbModSaves = new wxCheckBox(this,wxID_CB_MOD_SAVES,_("Move SAVE games"),wxDefaultPosition,wxDefaultSize,0);
	cbModSaves->SetToolTip(_("Move SAVE folder with save games to mod folder and restores it when mod run again. So multiple mods can have their own sets of save games."));

	bSizer122->Add(cbModSaves,0,wxBOTTOM|wxRIGHT|wxLEFT,5);


	bSizer121->Add(bSizer122,0,wxEXPAND,5);


	bSizer114->Add(bSizer121,0,wxEXPAND,5);

	m_staticline44 = new wxStaticLine(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLI_HORIZONTAL);
	bSizer114->Add(m_staticline44,0,wxEXPAND | wxALL,5);

	m_staticText120 = new wxStaticText(this,wxID_ANY,_("Spellcross mod builder status:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText120->Wrap(-1);
	bSizer114->Add(m_staticText120,0,wxRIGHT|wxLEFT,5);

	textOutput = new wxTextCtrl(this,wxID_TEXT_OUTPUT,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_MULTILINE|wxTE_READONLY);
	bSizer114->Add(textOutput,1,wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT,5);

	wxBoxSizer* bSizer123;
	bSizer123 = new wxBoxSizer(wxHORIZONTAL);

	bSizer123->SetMinSize(wxSize(-1,100));
	btnRunOrig = new wxButton(this,wxID_BTN_RUN_ORIG,_("Run Original"),wxDefaultPosition,wxSize(130,50),0);
	bSizer123->Add(btnRunOrig,0,wxALL|wxEXPAND,5);


	bSizer123->Add(0,0,1,wxEXPAND,5);

	wxBoxSizer* bSizer125;
	bSizer125 = new wxBoxSizer(wxVERTICAL);

	btnModInfo = new wxButton(this,wxID_BTN_MOD_INFO,_("Show\nMod Info"),wxDefaultPosition,wxSize(-1,-1),0);
	btnModInfo->SetMinSize(wxSize(130,-1));

	bSizer125->Add(btnModInfo,1,wxALL|wxEXPAND,5);


	bSizer123->Add(bSizer125,0,wxEXPAND,5);


	bSizer123->Add(0,0,1,wxEXPAND,5);

	btnRunMod = new wxButton(this,wxID_BTN_RUN_MOD,_("Run Mod"),wxDefaultPosition,wxSize(130,50),0);
	bSizer123->Add(btnRunMod,0,wxALL|wxEXPAND,5);


	bSizer114->Add(bSizer123,0,wxEXPAND,5);


	this->SetSizer(bSizer114);
	this->Layout();

	this->Centre(wxBOTH);
    
    // ------------ no edit above (auto generated by wxFormBuilder) ----------------
	m_ini = ini;
	m_th_proc = NULL;
	form_edit = NULL;

	wxIcon ico;
	ico.LoadFile("IDI_SPELCROS",wxBITMAP_TYPE_ICO_RESOURCE);
	if(ico.IsOk())
		btnRunOrig->SetBitmap(ico);
	btnRunOrig->SetBitmapPosition(wxDirection::wxBOTTOM);
	btnRunOrig->SetBitmapMargins(0,5);

	ico.LoadFile("IDI_SPELL_MOD",wxBITMAP_TYPE_ICO_RESOURCE);
	if(ico.IsOk())
		btnRunMod->SetBitmap(ico);
	btnRunMod->SetBitmapPosition(wxDirection::wxBOTTOM);
	btnRunMod->SetBitmapMargins(0,5);
	
	btnModInfo->SetBitmap(LoadSVGiconsBundle("IDR_INFO").GetBitmap(wxSize(32,32)));
	btnModInfo->SetBitmapPosition(wxDirection::wxBOTTOM);
	btnModInfo->SetBitmapMargins(0,5);
	
	auto op_sz = FromDIP(wxSize(16,16));
	btnSpellPath->SetBitmap(LoadSVGiconsBundle("IDR_OPEN3").GetBitmap(op_sz));
	btnSpellPath->SetBitmapHover(LoadSVGiconsBundle("IDR_OPEN2").GetBitmap(op_sz));
	btnSpellcdPath->SetBitmap(LoadSVGiconsBundle("IDR_OPEN3").GetBitmap(op_sz));
	btnSpellcdPath->SetBitmapHover(LoadSVGiconsBundle("IDR_OPEN2").GetBitmap(op_sz));
	btnDOSboxPath->SetBitmap(LoadSVGiconsBundle("IDR_OPEN3").GetBitmap(op_sz));
	btnDOSboxPath->SetBitmapHover(LoadSVGiconsBundle("IDR_OPEN2").GetBitmap(op_sz));
	btnModPath->SetBitmap(LoadSVGiconsBundle("IDR_OPEN3").GetBitmap(op_sz));
	btnModPath->SetBitmapHover(LoadSVGiconsBundle("IDR_OPEN2").GetBitmap(op_sz));

	mmExit->SetBitmaps(LoadSVGiconsBundle("IDR_EXIT"));
	
	msmSaveOrigItem->SetBitmaps(LoadSVGiconsBundle("IDR_SAVE"));
	msmSaveModItem->SetBitmaps(LoadSVGiconsBundle("IDR_SAVE"));
	mmSaveWDorg->SetBitmaps(LoadSVGiconsBundle("IDR_SAVE"));
	mmSaveWDmod->SetBitmaps(LoadSVGiconsBundle("IDR_SAVE"));
	mmSaveOrig->SetBitmaps(LoadSVGiconsBundle("IDR_SAVE"));
	mmSaveMod->SetBitmaps(LoadSVGiconsBundle("IDR_SAVE"));
	mmRestoreWDorg->SetBitmaps(LoadSVGiconsBundle("IDR_RELOAD"));
	mmRestoreWDmod->SetBitmaps(LoadSVGiconsBundle("IDR_RELOAD"));
	mmRestoreSaveOrig->SetBitmaps(LoadSVGiconsBundle("IDR_RELOAD"));
	mmRestoreSaveMod->SetBitmaps(LoadSVGiconsBundle("IDR_RELOAD"));

	mmModPath->SetBitmaps(LoadSVGiconsBundle("IDR_OPEN"));
	mmSpellPath->SetBitmaps(LoadSVGiconsBundle("IDR_OPEN"));
	mmSpellcdPath->SetBitmaps(LoadSVGiconsBundle("IDR_OPEN"));
	mmDosboxPath->SetBitmaps(LoadSVGiconsBundle("IDR_OPEN"));

	mmEditDOSboxCfg->SetBitmaps(LoadSVGiconsBundle("IDR_EDIT"));
	mmEditModDEF->SetBitmaps(LoadSVGiconsBundle("IDR_EDIT"));
	mmEditSpellCfg->SetBitmaps(LoadSVGiconsBundle("IDR_EDIT"));

	mmBuildLaunch->SetBitmaps(LoadSVGiconsBundle("IDR_BUILD"));
	mmModBuild->SetBitmaps(LoadSVGiconsBundle("IDR_BUILD"));
	mmModBuildSwap->SetBitmaps(LoadSVGiconsBundle("IDR_BUILD"));
	mmModRestore->SetBitmaps(LoadSVGiconsBundle("IDR_RELOAD"));
	mmModCleanup->SetBitmaps(LoadSVGiconsBundle("IDR_CLOSE"));

	mmRunSetupBat->SetBitmaps(LoadSVGiconsBundle("IDR_SETUP"));
	mmRunOrig->SetBitmaps(LoadSVGiconsBundle("IDR_START"));
	mmRunMod->SetBitmaps(LoadSVGiconsBundle("IDR_START"));

	mmModInfo->SetBitmaps(LoadSVGiconsBundle("IDR_INFO"));
	mmAbout->SetBitmaps(LoadSVGiconsBundle("IDR_INFO"));
	mmHelp->SetBitmaps(LoadSVGiconsBundle("IDR_INFO"));



    FillChoicePaths(chSpellPath, m_ini, "PATH", "spellcross_dir");
    FillChoicePaths(chSpellCdPath,m_ini,"PATH","spellcross_cd_dir");
    FillChoicePaths(chDOSboxPath,m_ini,"PATH","dosbox_path");
    FillChoicePaths(chModPath,m_ini,"PATH","mod_path");

	// run mode selector
	chRunMode->Clear();
	chRunMode->Append("DOSbox");
	chRunMode->Append("Win32");
	chRunMode->SetStringSelection(m_ini->GetValue("SETUP","run_mode","DOSbox"));

	// find executables
	auto spell_dir = GetPathChoiceLastPath(chSpellPath);
	ListSpellExecutables(spell_dir,chSpellExec);
	auto spell_exe = std::string(m_ini->GetValue("PATH","spellcross_executable","SPELCROS.EXE"));
	chSpellExec->SetStringSelection(toupper(spell_exe));

	// fill drive letters
	wxArrayString volumes = wxFSVolume::GetVolumes();
	chVirtCD->Freeze();
	chVirtCD->Clear();
	for(int vol = 'D'; vol <= 'Z'; vol++)
		chVirtCD->Append(string_format("%c",vol));
	if(!chVirtCD->SetStringSelection(m_ini->GetValue("SETUP","spellcross_cd_volume","S")) && chVirtCD->GetCount())
		chVirtCD->Select(chRunMode->GetCount() - 1);
	chVirtCD->Thaw();

	// load other options
	cbNoAutoexec->SetValue(m_ini->GetBoolValue("SETUP","DOSbox_no_autoexec",false));
	cbFullscreen->SetValue(m_ini->GetBoolValue("SETUP","DOSbox_fullscreen",false));
	cbAllowCDmod->SetValue(m_ini->GetBoolValue("SETUP","allow_cd_mod",true));
	cbModSaves->SetValue(m_ini->GetBoolValue("SETUP","move_mod_saves",false));

			
	Bind(wxEVT_CLOSE_WINDOW,&FormMain::OnClose,this,wxID_FORM_MAIN);
	Bind(wxEVT_COMMAND_MENU_SELECTED,&FormMain::OnExit,this,wxID_MM_EXIT);
	Bind(wxEVT_COMMAND_MENU_SELECTED,&FormMain::OnAbout,this,wxID_MM_ABOUT);
	Bind(wxEVT_COMMAND_MENU_SELECTED,&FormMain::OnHelp,this,wxID_MM_HELP);
	Bind(wxEVT_COMMAND_MENU_SELECTED,&FormMain::OnEdit,this,wxID_MM_EDIT_SPELLCFG);
	Bind(wxEVT_COMMAND_MENU_SELECTED,&FormMain::OnEdit,this,wxID_MM_EDIT_MOD_DEF);
	Bind(wxEVT_COMMAND_MENU_SELECTED,&FormMain::OnEditDOSboxConf,this,wxID_MM_EDIT_DOSBOX_CFG);
    Bind(wxEVT_COMMAND_CHOICE_SELECTED,&FormMain::OnSelectSpellPath,this,wxID_CH_SPELL_PATH);
    Bind(wxEVT_COMMAND_CHOICE_SELECTED,&FormMain::OnSelectSpellPath,this,wxID_CH_SPELLCD_PATH);
    Bind(wxEVT_COMMAND_CHOICE_SELECTED,&FormMain::OnSelectDOSboxPath,this,wxID_CH_DOSBOX_PATH);
    Bind(wxEVT_COMMAND_CHOICE_SELECTED,&FormMain::OnSelectModPath,this,wxID_CH_MOD_PATH);

	Bind(wxEVT_COMMAND_MENU_SELECTED,&FormMain::OnBuildLaunchFiles,this,wxID_MM_BUILD_LAUNCH);
	Bind(wxEVT_COMMAND_MENU_SELECTED,&FormMain::OnBuildMod,this,wxID_MM_MOD_BUILD);
	Bind(wxEVT_COMMAND_MENU_SELECTED,&FormMain::OnBuildMod,this,wxID_MM_MOD_BUILD_SWAP);
	Bind(wxEVT_COMMAND_MENU_SELECTED,&FormMain::OnBuildMod,this,wxID_MM_MOD_RESTORE);
	Bind(wxEVT_COMMAND_MENU_SELECTED,&FormMain::OnCleanupMod,this,wxID_MM_MOD_CLEAN);
		

	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormMain::OnRunGame,this,wxID_BTN_RUN_ORIG);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormMain::OnRunGame,this,wxID_BTN_RUN_MOD);	
	Bind(wxEVT_COMMAND_MENU_SELECTED,&FormMain::OnRunGame,this,wxID_MM_RUN_ORIG);
	Bind(wxEVT_COMMAND_MENU_SELECTED,&FormMain::OnRunGame,this,wxID_MM_RUN_MOD);
	Bind(wxEVT_COMMAND_MENU_SELECTED,&FormMain::OnRunGameSetup,this,wxID_MM_RUN_SETUPBAT);

	Bind(wxEVT_COMMAND_MENU_SELECTED,&FormMain::OnModInfo,this,wxID_MM_MOD_INFO);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormMain::OnModInfo,this,wxID_BTN_MOD_INFO);

	Bind(wxEVT_COMMAND_MENU_SELECTED,&FormMain::OnBackupSaveWD,this,wxID_MM_SAVE_WD_ORG);
	Bind(wxEVT_COMMAND_MENU_SELECTED,&FormMain::OnBackupSaveWD,this,wxID_MM_RESTORE_WD_ORG);
	Bind(wxEVT_COMMAND_MENU_SELECTED,&FormMain::OnBackupSaveWD,this,wxID_MM_SAVE_WD_MOD);
	Bind(wxEVT_COMMAND_MENU_SELECTED,&FormMain::OnBackupSaveWD,this,wxID_MM_RESTORE_WD_MOD);

	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormMain::OnSelectSpellPath,this,wxID_BTN_SPELL_PATH);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormMain::OnSelectSpellPath,this,wxID_BTN_SPELLCD_PATH);
	Bind(wxEVT_COMMAND_MENU_SELECTED,&FormMain::OnSelectSpellPath,this,wxID_MM_SPELL_PATH);
	Bind(wxEVT_COMMAND_MENU_SELECTED,&FormMain::OnSelectSpellPath,this,wxID_MM_SPELLCD_PATH);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormMain::OnSelectDOSboxPath,this,wxID_BTN_DB_PATH);
	Bind(wxEVT_COMMAND_MENU_SELECTED,&FormMain::OnSelectDOSboxPath,this,wxID_MM_DOSBOX_PATH);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormMain::OnSelectModPath,this,wxID_BTN_MOD_PATH);
	Bind(wxEVT_COMMAND_MENU_SELECTED,&FormMain::OnSelectModPath,this,wxID_MM_MOD_PATH);
	
	
	Bind(wxEVT_THREAD,&FormMain::OnBuildModEvent,this,wxID_PROC_THREAD);

	sbar->SetFieldsCount(1);

	
	// check mod state and offer restore
	SpellMod mod;
	SpellMod::Config config;
	config.state_ini_path = std::filesystem::path(GetExecutableDir()) / str_mod_state_ini_none;
	bool something_to_restore;
	mod.CheckRestoreMod(config,something_to_restore);
	if(something_to_restore)
	{
		wxMessageDialog dial(this,"Game archives seems to be in modified state. Possibly due to system crash during last run?\nRestore original state before doing anything (recommended)?",_("Restore original game?"),wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);
		if(dial.ShowModal() == wxID_YES)
		{
			wxCommandEvent evt;
			evt.SetId(wxID_MM_MOD_RESTORE);
			FormMain::OnBuildMod(evt);
		}
	}		
}

FormMain::~FormMain()
{
}

// on form close
void FormMain::OnClose(wxCloseEvent& ev)
{
	SaveChoicePaths(chSpellPath,m_ini,"PATH","spellcross_dir","; Spellcross installation path");
	SaveChoicePaths(chSpellCdPath,m_ini,"PATH","spellcross_cd_dir","; Spellcross CD copy path (if applicable)");
	SaveChoicePaths(chDOSboxPath,m_ini,"PATH","dosbox_path","; DOSbox executable path (if exist)");
	SaveChoicePaths(chModPath,m_ini,"PATH","mod_path","; Spellcross mod path");
	
	auto spell_exec = std::string(chSpellExec->GetStringSelection());
	if(spell_exec != str_choice_none)
		m_ini->SetValue("PATH","spellcross_executable",spell_exec.c_str(),"; Spellcross executable to run");

	m_ini->SetValue("SETUP","run_mode",chRunMode->GetStringSelection(),"; Spellcross execution mode");
	m_ini->SetValue("SETUP","spellcross_cd_volume",chVirtCD->GetStringSelection(),"; Virtual CD drive letter");
	
	m_ini->SetBoolValue("SETUP","DOSbox_no_autoexec",cbNoAutoexec->GetValue(), "; Disable autoexec for DOSbox");
	m_ini->SetBoolValue("SETUP","DOSbox_fullscreen",cbFullscreen->GetValue(), "; Run in fulscreen in DOSbox");
	m_ini->SetBoolValue("SETUP","allow_cd_mod",cbAllowCDmod->GetValue(), "; Allow modification of CD game archives");
	m_ini->SetBoolValue("SETUP","move_mod_saves",cbModSaves->GetValue(), "; Move save games with the mod");	

	ev.Skip();
}
void FormMain::OnExit(wxCommandEvent& event)
{
    Close(true);
}

// about message
void FormMain::OnAbout(wxCommandEvent& event)
{
    auto form = new FormAbout(this);
    if(form->ShowModal() == wxID_OK)
    {
        // --- confirmed
    }
    delete form;
}

// show help
void FormMain::OnHelp(wxCommandEvent& event)
{
	auto dir = GetExecutableDir();
	auto path = std::filesystem::path(dir) / "help" / "help_eng.html";	
	auto hinst = ShellExecute(NULL, L"open", path.wstring().c_str(), NULL, NULL, SW_SHOWNORMAL);
	if((int)hinst <= 32)
	{
		// failed
		wxMessageDialog dial(this,string_format("Cannot open help HTML:\n%ls:\n\nThe file is either not found or it is blocked by system setup. Try to open manually in tool's folder.",path.wstring().c_str()),_("Opening help..."),wxICON_ERROR);
		dial.ShowModal();
	}
}

// text editor
void FormMain::OnEdit(wxCommandEvent& event)
{	
	std::filesystem::path file_path;
	if(event.GetId() == wxID_MM_EDIT_SPELLCFG)
	{
		auto spell_dir = GetPathChoiceLastPath(chSpellPath);
		if(spell_dir.empty())
			return;	
		file_path = std::filesystem::path(spell_dir) / "SPELCROS.CFG";
	}
	else if(event.GetId() == wxID_MM_EDIT_MOD_DEF)
	{
		auto mod_path = GetPathChoiceLastPath(chModPath);
		if(mod_path.empty())
			return;
		file_path = std::filesystem::path(mod_path);
	}
	else
		return;
	EditFile(file_path);	
}

// edit dosbox conf
void FormMain::OnEditDOSboxConf(wxCommandEvent& event)
{
	auto box_path = GetPathChoiceLastPath(chDOSboxPath);
	if(box_path.empty())
		return;	
	auto cmd = string_format("%ls -printconf -noconsole",box_path.c_str());
	std::system(cmd.c_str());
	auto path_file = std::filesystem::path(box_path).parent_path() / "stdout.txt";
	std::string conf_path;
	loadstr(path_file,conf_path);
	conf_path = trim_whites(conf_path);
	if(conf_path.empty())
		return;	
	EditFile(conf_path);
}

// show mod info
void FormMain::OnModInfo(wxCommandEvent& event)
{
	auto mod_path = GetPathChoiceLastPath(chModPath);
	if(mod_path.empty())
		return;
	auto path = std::filesystem::path(mod_path).parent_path() / "info" / "info.html";
	auto hinst = ShellExecute(NULL,L"open",path.wstring().c_str(),NULL,NULL,SW_SHOWNORMAL);
	if((int)hinst <= 32)
	{
		// failed
		wxMessageDialog dial(this,string_format("Cannot open mod info HTML file:\n%ls:\n\nThe file is either not found or it is blocked by system setup (security setup). Try to open it manually from mod folder.",path.wstring().c_str()),_("Opening help..."),wxICON_ERROR);
		dial.ShowModal();
	}
}

// on backup save WORKDIR
void FormMain::OnBackupSaveWD(wxCommandEvent& event)
{
	bool is_save = event.GetId() == wxID_MM_SAVE_WD_ORG || event.GetId() == wxID_MM_SAVE_WD_MOD;
	bool is_mod = event.GetId() == wxID_MM_SAVE_WD_MOD || event.GetId() == wxID_MM_RESTORE_WD_MOD;

	std::filesystem::path save_dir;
	if(is_mod)
	{
		auto mod_path = GetPathChoiceLastPath(chModPath);
		save_dir = std::filesystem::path(mod_path).parent_path() / "save";
	}
	else
	{
		auto spell_dir = GetPathChoiceLastPath(chSpellPath);
		if(spell_dir.empty())
			return;	
		save_dir = std::filesystem::path(spell_dir) / "SAVE";
	}

	auto src_path = save_dir / "WORKDIR";
	auto dest_path = save_dir / "WORKDIR_backup";
	if(!is_save)
		std::swap(src_path, dest_path);

	std::vector<std::string> names;
	fs_list_dir(dest_path, "*", true, true, &names);
		
	std::string info = "";
	if(is_save)
		info += "Making backup of Spellcross temporary save game WORKDIR.\n\n";
	else
		info += "Restoring backup of Spellcross temporary save game WORKDIR.\n\n";

	if(!std::filesystem::exists(save_dir) || !std::filesystem::exists(src_path))
	{
		info += string_format("Source savegame directory:\n  %ls\ndoes not exist! ",save_dir.wstring().c_str());
		info += string_format("You may have wrong paths set? Or maybe no save was made yet?",save_dir.wstring().c_str());
		wxMessageDialog dial(this,info,"Backup of savegame ...",wxICON_ERROR);
		dial.ShowModal();
		return;
	}

	if(names.empty())
	{
		if(is_save)
			info += string_format("Current WORKDIR save game:\n  %ls\nwith its content will be copied to backup location:\n  %ls.",src_path.wstring().c_str(),dest_path.wstring().c_str());
		else
			info += string_format("Backup WORKDIR save game/:\n  %ls\nwith its content will be restored to location:\n  %ls.",src_path.wstring().c_str(),dest_path.wstring().c_str());
	}
	else
	{
		if(is_save)
			info += string_format("Existing backup of WORKDIR save game:\n  %ls\nwith its content:\n",dest_path.wstring().c_str());
		else
			info += string_format("Existing WORKDIR save game:\n  %ls\nwith its content:\n",dest_path.wstring().c_str());
		for(auto &item: names)
			info += string_format("  %s\n",item.c_str());
		if(is_save)
			info += string_format("will be removed and replaced by current save game WORKDIR:\n  %ls.",src_path.wstring().c_str());
		else
			info += string_format("will be removed and replaced by backup of WORKDIR save game:\n  %ls.",src_path.wstring().c_str());
	}

	wxMessageDialog dial(this,info,"Backup of savegame ...", wxYES_NO | wxICON_QUESTION);
	if(dial.ShowModal() != wxID_YES)
		return;

	// try remove old save
	if(std::filesystem::exists(dest_path))
	{
		if(fs_remove(dest_path, true))
		{
			// failed
			wxMessageDialog dial(this,string_format("Cannot remove old save directory:\n  %ls\nPossibly sharing violation. Maybe you are currently viewing the save folder?",dest_path.wstring().c_str()),"Backup of savegame ...",wxICON_ERROR);
			dial.ShowModal();
			return;
		}
	}

	// make or restore backup
	if(fs_copy(src_path, dest_path, std::filesystem::copy_options::recursive))
	{
		// failed
		wxMessageDialog dial(this,string_format("Cannot copy save directory:\n  %ls\nto its new location:\n  %ls\nPossibly sharing violation. Maybe you are currently viewing the save folder?",src_path.wstring().c_str(),dest_path.wstring().c_str()),"Backup of savegame ...",wxICON_ERROR);
		dial.ShowModal();
		return;
	}

	// ok
	{
		wxMessageDialog dial(this,string_format("Done!",src_path.wstring().c_str(),dest_path.wstring().c_str()),"Backup of savegame ...",wxICON_INFORMATION);
		dial.ShowModal();	
	}
}


// mod builder thread
ProcTh::ProcTh(wxFrame* parent,Params &config,ActionsList &actions)
{
	m_parent = parent;
	m_config = config;
	m_actions = actions;
}

// mod builder thread entry point
wxThread::ExitCode ProcTh::Entry()
{		
	SpellMod mod;

	SpellMod::Config mod_config;
	mod_config.allow_cd_mod = m_config.allow_cd_mod;
	mod_config.move_saves = m_config.move_saves;
	mod_config.mod_path = m_config.mod_path;
	mod_config.spell_dir = m_config.spell_dir;
	mod_config.spellcd_dir = m_config.spellcd_dir;
	mod_config.state_ini_path = m_config.state_ini_path;
	
	// register console output callback
	mod.SetConsoleOutCb(bind(&ProcTh::ConsoleStringCallback,this,std::placeholders::_1));
	
	// do stuff
	for(auto &action: m_actions)
	{		
		if(action == Action::BUILD)
		{
			SetStatusCallback("Building mod ...");
			if(mod.BuildMod(mod_config))
				break;
		}
		
		if(action == Action::SWAP)
		{
			SetStatusCallback("Replacing game files ...");
			if(mod.SwapMod(mod_config))
				break;
		}
		
		if(action == Action::RESTORE)
		{
			SetStatusCallback("Restoring game files ...");
			if(mod.RestoreMod(mod_config))
				break;
		}

		if(action == Action::RUN)
		{
			SetStatusCallback("Running game ...");
			if(SpellLaunch::RunGame(m_config.spell_dir, m_config.run_mode,bind(&ProcTh::ConsoleStringCallback,this,std::placeholders::_1)))
				break;
		}

		if(action == Action::SETUP)
		{
			SetStatusCallback("Running game SETUP ...");
			if(SpellLaunch::RunSetup(m_config.spell_dir,m_config.run_mode,bind(&ProcTh::ConsoleStringCallback,this,std::placeholders::_1)))
				break;
		}

		if(action == Action::CLEANUP)
		{
			SetStatusCallback("Cleanup mod state INI file ...");
			if(mod.CleanupMod(mod_config))
				break;
		}
	}
	
	// signalize we are done
	auto evt = new wxThreadEvent(wxEVT_THREAD,FormMain::wxID_PROC_THREAD);
	evt->SetInt(Event::DONE);
	auto handler = m_parent->GetEventHandler();
	handler->QueueEvent(evt);

	return(0);
}

// console output print callback
void ProcTh::ConsoleStringCallback(std::string info)
{
	auto evt = new wxThreadEvent(wxEVT_THREAD,FormMain::wxID_PROC_THREAD);
	evt->SetInt(Event::PRINT);
	evt->SetString(info);
	auto handler = m_parent->GetEventHandler();
	handler->QueueEvent(evt);
}

// console output clear callback
void ProcTh::ConsoleClearCallback()
{
	auto evt = new wxThreadEvent(wxEVT_THREAD,FormMain::wxID_PROC_THREAD);
	evt->SetInt(Event::CLEAR);
	auto handler = m_parent->GetEventHandler();
	handler->QueueEvent(evt);
}

// status bar callback
void ProcTh::SetStatusCallback(std::string info)
{
	auto evt = new wxThreadEvent(wxEVT_THREAD,FormMain::wxID_PROC_THREAD);
	evt->SetInt(Event::STATUS);
	evt->SetString(info);
	auto handler = m_parent->GetEventHandler();
	handler->QueueEvent(evt);
}




// mod builder event
void FormMain::OnBuildModEvent(wxThreadEvent& event)
{
	auto what = event.GetInt();
	if(what == ProcTh::Event::DONE)
	{
		// processing done		
		sbar->SetStatusText("Ready!");
		SetControlsState(false);
	}
	else if(what == ProcTh::Event::CLEAR)
	{
		// console print
		textOutput->Clear();
	}
	else if(what == ProcTh::Event::PRINT)
	{		
		// console print
		textOutput->AppendText(event.GetString());
	}
	else if(what == ProcTh::Event::STATUS)
	{
		// status bar
		sbar->SetStatusText(event.GetString());
	}

}

// start processing thread
void FormMain::StartProcTh(ProcTh::Params &params,ProcTh::ActionsList actions)
{
	// process mod
	m_th_proc = new ProcTh(this,params,actions);
	if(m_th_proc->Create() != wxTHREAD_NO_ERROR)
	{
		wxMessageBox(_("Couldn't create processing thread!"));
		return;
	}
	if(m_th_proc->Run() != wxTHREAD_NO_ERROR)
	{
		wxMessageBox(_("Couldn't run thread!"));
		return;
	}
}


// enables/disables controls during async operations
void FormMain::SetControlsState(bool busy)
{
	std::vector<int> list = {
		wxID_MM_MOD_PATH,
		wxID_MM_SPELL_PATH,
		wxID_MM_SPELLCD_PATH,
		wxID_MM_DOSBOX_PATH,
		wxID_MM_EXIT,
		wxID_MM_EDIT_SPELLCFG,
		wxID_MM_EDIT_DOSBOX_CFG,
		wxID_MM_RUN_SETUPBAT,
		wxID_MM_MOD_INFO,
		wxID_MM_MOD_BUILD,
		wxID_MM_MOD_BUILD_SWAP,
		wxID_MM_MOD_RESTORE,
		wxID_MM_EDIT_MOD_DEF,
		wxID_MM_MOD_CLEAN,
		wxID_MM_SAVE_WD_ORG,
		wxID_MM_RESTORE_WD_ORG,
		wxID_MM_SAVE_ORIG,
		wxID_RESTORE_SAVE_ORIG,
		wxID_MM_SAVE_WD_MOD,
		wxID_MM_RESTORE_WD_MOD,
		wxID_MM_SAVE_MOD,
		wxID_RESTORE_SAVE_MOD,
		wxID_MM_BUILD_LAUNCH,
		wxID_MM_RUN_ORIG,
		wxID_MM_RUN_MOD,
		//wxID_MM_HELP,
		//wxID_MM_ABOUT,
		wxID_SBAR,
		wxID_CH_SPELL_PATH,
		wxID_BTN_SPELL_PATH,
		wxID_CH_SPELL_EXE,
		wxID_CH_SPELLCD_PATH,
		wxID_BTN_SPELLCD_PATH,
		wxID_CH_CD_LETTER,
		wxID_CH_DOSBOX_PATH,
		wxID_BTN_DB_PATH,
		wxID_CB_NO_AUTOEXEC,
		wxID_CB_FULLSCREEN,
		wxID_CH_RUN_MODE,
		wxID_CH_MOD_PATH,
		wxID_BTN_MOD_PATH,
		wxID_CB_ALLOW_CD_MOD,
		wxID_CB_MOD_SAVES,
		//wxID_TEXT_OUTPUT,
		wxID_BTN_RUN_ORIG,
		wxID_BTN_MOD_INFO,
		wxID_BTN_RUN_MOD,
	};

	for(auto &id: list)
	{
		auto item = FindItem(id);
		if(item)
		{
			item->Enable(!busy);
			continue;
		}
		auto mitem = mMenu->FindItem(id);
		if(mitem)
		{
			mitem->Enable(!busy);
			continue;
		}
	}
}


// cleanup mod
void FormMain::OnCleanupMod(wxCommandEvent& event)
{
	std::string info;
	info += "Cleanup mod operation is LAST RESORT solution when Restore mod fails!\n";
	info += "This operation will cleanup mod state INI file that contains information which archives (and save games) are modified and where the originals are located.\n";
	info += "It will very likely lead to inconsistent game files and possible loss of save games when those were moved along with the archives. You might need to manually restore them after this operation!\n\n";
	info += "Before you try this operation, make sure the Restore Mod operation is not failing e.g. because of file sharing violations like when you are currently viewing content of some of the game files or folders that needs to be moved!";
	wxMessageDialog dial(this,info,_("Cleanup Mod State?"),wxYES_NO | wxNO_DEFAULT | wxICON_EXCLAMATION);
	if(dial.ShowModal() != wxID_YES)
		return;

	// load setup
	ProcTh::Params config;
	config.state_ini_path = std::filesystem::path(GetExecutableDir()) / str_mod_state_ini_none;

	// what to do?
	ProcTh::ActionsList actions;
	actions.push_back(ProcTh::Action::CLEANUP);

	// run mod async
	SetControlsState(true);
	StartProcTh(config,actions);
}

// build mod
void FormMain::OnBuildMod(wxCommandEvent& event)
{
	textOutput->Clear();

	// load setup
	ProcTh::Params config;
	config.mod_path = GetPathChoiceLastPath(chModPath);
	config.spell_dir = GetPathChoiceLastPath(chSpellPath);
	config.spellcd_dir = GetPathChoiceLastPath(chSpellCdPath);
	config.allow_cd_mod = cbAllowCDmod->GetValue();
	config.move_saves = cbModSaves->GetValue();
	config.state_ini_path = std::filesystem::path(GetExecutableDir()) / str_mod_state_ini_none;
	
	if(config.mod_path.empty())
		return;
	if(config.spell_dir.empty())
		return;		
	
	// what to do?
	ProcTh::ActionsList actions;	
	auto evt_id = event.GetId();
	if(evt_id == wxID_MM_MOD_BUILD)
		actions.push_back(ProcTh::Action::BUILD);
	else if(evt_id == wxID_MM_MOD_BUILD_SWAP)
	{
		actions.push_back(ProcTh::Action::BUILD);
		actions.push_back(ProcTh::Action::SWAP);
	}
	else if(evt_id == wxID_MM_MOD_RESTORE)
		actions.push_back(ProcTh::Action::RESTORE);
	else
		return;

	SetControlsState(true);

	// run mod async
	StartProcTh(config, actions);	
}

// on build lanch files
void FormMain::OnBuildLaunchFiles(wxCommandEvent& event)
{
	MakeSpellLaunchFiles();
}

// on run game
void FormMain::OnRunGame(wxCommandEvent& event)
{		
	textOutput->Clear();

	// load setup
	ProcTh::Params config;	
	config.mod_path = GetPathChoiceLastPath(chModPath);
	config.spell_dir = GetPathChoiceLastPath(chSpellPath);
	config.spellcd_dir = GetPathChoiceLastPath(chSpellCdPath);
	config.allow_cd_mod = cbAllowCDmod->GetValue();
	config.move_saves = cbModSaves->GetValue();
	config.state_ini_path = std::filesystem::path(GetExecutableDir()) / str_mod_state_ini_none;
	config.run_mode = chRunMode->GetStringSelection();

	// first make launch files
	MakeSpellLaunchFiles();
	
	if(config.run_mode.empty())
		return;
	if(config.spell_dir.empty())
		return;

	// do mod before run?
	ProcTh::ActionsList actions;
	auto evt_id = event.GetId();
	if(evt_id == wxID_BTN_RUN_MOD || evt_id == wxID_MM_RUN_MOD)
	{
		// mod: build and swap first
		actions.push_back(ProcTh::Action::BUILD);
		actions.push_back(ProcTh::Action::SWAP);
	}
	else
	{
		// original: restore original
		actions.push_back(ProcTh::Action::RESTORE);
	}
	
	// always run
	actions.push_back(ProcTh::Action::RUN);

	// restore original if modified
	if(evt_id == wxID_BTN_RUN_MOD || evt_id == wxID_MM_RUN_MOD)
		actions.push_back(ProcTh::Action::RESTORE);

	SetControlsState(true);

	// run game async
	StartProcTh(config,actions);
}

// on run game setup
void FormMain::OnRunGameSetup(wxCommandEvent& event)
{
	textOutput->Clear();

	// load setup
	ProcTh::Params config;
	config.spell_dir = GetPathChoiceLastPath(chSpellPath);
	config.run_mode = chRunMode->GetStringSelection();

	// first make launch files
	MakeSpellLaunchFiles();

	if(config.run_mode.empty())
		return;
	if(config.spell_dir.empty())
		return;

	// run setup
	ProcTh::ActionsList actions;
	actions.push_back(ProcTh::Action::SETUP);
	
	SetControlsState(true);

	// run game async
	StartProcTh(config,actions);
}



// load file to editor
void FormMain::EditFile(std::filesystem::path path)
{
	if(FindWindowById(wxID_FORM_EDIT))
		return;
	if(!std::filesystem::exists(path))
	{
		wxMessageDialog dial(this,string_format("File \"%ls\" does not exist?",path.wstring().c_str()),_("Edit file..."),wxOK | wxICON_INFORMATION);
		dial.ShowModal();
		return;
	}
	form_edit = new FormEdit(this,wxID_FORM_EDIT);
	if(!form_edit)
		return;
	form_edit->SetPath(path);
	form_edit->Show();
}

// try load path choices from ini
void FormMain::FillChoicePaths(wxChoice* choice,CSimpleIniA *ini, std::string section, std::string key)
{
    choice->Freeze();
    choice->Clear();
    for(int k = 0; k < 10; k++)
    {
        auto hkey = key;
        if(k)
            hkey = string_format("%s<%d>",key.c_str(), k);
        auto path = ini->GetValue(section.c_str(),hkey.c_str());
        if(path && strlen(path))
            choice->Append(path);
    }
	choice->Append(str_choice_no_select);
	choice->Append(str_choice_browse);
    choice->Thaw();

    auto path = ini->GetValue(section.c_str(), key.c_str());
    if(path && strlen(path))
        choice->Select(0);
    else
        choice->SetStringSelection(str_choice_no_select);

    ChoiceCheckPaths(choice);
}

// save path choice to ini
void FormMain::SaveChoicePaths(wxChoice* choice,CSimpleIniA* ini,std::string section,std::string key,std::string comment)
{
    // remove rubbish
    ChoiceCheckPaths(choice);

    // make list of unique paths
    std::vector<std::wstring> list;
	if(choice->GetStringSelection() == str_choice_no_select)
		list.push_back(L"");
    for(auto& item: choice->GetStrings())
    {
        if(item == str_choice_browse || item == str_choice_no_select)
            continue;
        list.push_back(item.ToStdWstring());
    }

    // store selection and history
    for(int id = 0; id < 10; id++)
    {
        auto hkey = key;
        const char *com = NULL;
		if(!id)
			com = comment.c_str();
		if(id)
            hkey = string_format("%s<%d>",key.c_str(),id);
        if(id < list.size())
            ini->SetValue(section.c_str(), hkey.c_str(),wstring2string(list[id]).c_str(),com);
        else
            ini->SetValue(section.c_str(),hkey.c_str(),"",com);
    }
}

// look for 
void FormMain::ListSpellExecutables(std::filesystem::path spell_dir, wxChoice* choice)
{
	auto last_sel = choice->GetStringSelection();
	choice->Freeze();
	choice->Clear();
	for(const auto& entry: std::filesystem::directory_iterator(spell_dir))
	{
		auto item = entry.path();
		if(entry.is_directory())
			continue;
		auto name = item.filename().string();
		name = toupper(name);
		if(!wildcmp("*.EXE",name.c_str()))
			continue;
		if(name == "DOS4GW.EXE" || name == "INSTALUJ.EXE" || name == "INSTALL95.EXE" || name == "SETUP.EXE")
			continue;
		choice->Append(name);
	}
	if(!choice->GetCount())
		choice->Append(str_choice_none);
	choice->Thaw();
	if(!choice->SetStringSelection(last_sel))
		choice->Select(0);
}

// check listed paths (remove duplicates, non-existent, move selected to top)
void FormMain::ChoiceCheckPaths(wxChoice *choice)
{
    auto path = choice->GetStringSelection().ToStdWstring();
    if(path == str_choice_browse || path == str_choice_no_select)
        choice->Select(-1);

    // make list of unique paths
    std::vector<std::wstring> list;
    for(auto &item: choice->GetStrings())
    {
        if(item == str_choice_browse || item == str_choice_no_select)
            continue;
        if(!std::filesystem::exists(std::filesystem::path(item.ToStdWstring())))
            continue;
        if(std::find(list.begin(), list.end(), item) != list.end())
            continue;
        list.push_back(item.ToStdWstring());
    }

    // refill list
    choice->Freeze();
    choice->Clear();
    for(auto &item: list)
        choice->Append(item);
	choice->Append(str_choice_no_select);
	choice->Append(str_choice_browse);
    choice->Thaw();
    if(path == str_choice_no_select)
		choice->SetStringSelection(str_choice_no_select);
	else if(choice->SetStringSelection(path))
    {
        // make sure selected path is first
        choice->Delete(choice->GetSelection());
        choice->Insert(path, 0);
        choice->Select(0);
    }
    else
		choice->SetStringSelection(str_choice_no_select);
}

// get first valid path of choice path selector or deftaul if none found
std::wstring FormMain::GetPathChoiceLastPath(wxChoice *choice, std::wstring default_path)
{
	if(!choice || choice->GetCount() < 3 || choice->GetStringSelection() == str_choice_no_select)
		return(default_path);
	return(choice->GetString(0).ToStdWstring());
}

// on change spellcross path
void FormMain::OnSelectSpellPath(wxCommandEvent& event)
{    
    auto id = event.GetId();
    auto choice = (wxChoice*)FindItem(id);
	if(id == wxID_BTN_SPELL_PATH || id == wxID_MM_SPELL_PATH)
		choice = chSpellPath;
	if(id == wxID_BTN_SPELLCD_PATH || id == wxID_MM_SPELLCD_PATH)
		choice = chSpellCdPath;
    if(choice->GetStringSelection() == str_choice_browse || id == wxID_BTN_SPELLCD_PATH || id == wxID_BTN_SPELL_PATH || id == wxID_MM_SPELL_PATH || id == wxID_MM_SPELLCD_PATH)
    {
        // browse new path
		auto def_path = GetPathChoiceLastPath(choice);
        auto title = (id == wxID_CH_SPELLCD_PATH || id == wxID_BTN_SPELLCD_PATH || id == wxID_MM_SPELLCD_PATH)?"Select Spellcross CD folder":"Select Spellcross installation folder";
        wxDirDialog saveDirDialog(this,title,def_path,wxDD_DIR_MUST_EXIST);
        if(saveDirDialog.ShowModal() == wxID_CANCEL)
            return;        
        choice->Insert(saveDirDialog.GetPath(),0);
        choice->Select(0);
    }
    // check validity
    ChoiceCheckPaths(choice);

	// find executables
	auto spell_dir = GetPathChoiceLastPath(chSpellPath);
	ListSpellExecutables(spell_dir, chSpellExec);        
}

// on change DOSbox path
void FormMain::OnSelectDOSboxPath(wxCommandEvent& event)
{
    auto id = event.GetId();
    auto choice = chDOSboxPath;
    if(choice->GetStringSelection() == str_choice_browse || id == wxID_BTN_DB_PATH || id == wxID_MM_DOSBOX_PATH)
    {
        // browse new path
		auto def_path = GetPathChoiceLastPath(choice);
		auto dir = std::filesystem::path(def_path).parent_path().wstring();
		auto name = std::filesystem::path(def_path).filename().wstring();
        wxFileDialog openFileDialog(this,"Select DOSbox executable path",dir,name,"DOSbox executable (*.exe)|*.exe",
            wxFD_OPEN|wxFD_FILE_MUST_EXIST);
        if(openFileDialog.ShowModal() == wxID_CANCEL)
            return;        
        choice->Insert(openFileDialog.GetPath(),0);
        choice->Select(0);
    }
    // check validity
    ChoiceCheckPaths(choice);
}

// on change DOSbox path
void FormMain::OnSelectModPath(wxCommandEvent& event)
{
    auto id = event.GetId();
    auto choice = chModPath;
    if(choice->GetStringSelection() == str_choice_browse || id == wxID_BTN_MOD_PATH || id == wxID_MM_MOD_PATH)
    {
        // browse new path
		auto def_path = GetPathChoiceLastPath(choice);
		auto dir = std::filesystem::path(def_path).parent_path().wstring();
		auto name = std::filesystem::path(def_path).filename().wstring();
        wxFileDialog openFileDialog(this,"Select mod path",dir,name,"Spellcross mod path (*.def)|*.def",
            wxFD_OPEN|wxFD_FILE_MUST_EXIST);
        if(openFileDialog.ShowModal() == wxID_CANCEL)
            return;
        choice->Insert(openFileDialog.GetPath(),0);
        choice->Select(0);
    }
    // check validity
    ChoiceCheckPaths(choice);
}


// make all game launch files using current GUI setup
void FormMain::MakeSpellLaunchFiles()
{	
	sbar->SetStatusText("Building launch files ...");

	textOutput->AppendText("Making launch files...\n");
	
	auto spell_dir = GetPathChoiceLastPath(chSpellPath);
	auto spellcd_dir = GetPathChoiceLastPath(chSpellCdPath);
	auto dosbox_path = GetPathChoiceLastPath(chDOSboxPath);
	auto drive = chVirtCD->GetStringSelection().ToStdString();
	auto spell_exe = chSpellExec->GetStringSelection();
	auto fullscreen = cbFullscreen->GetValue();
	auto no_autoexec = cbNoAutoexec->GetValue();
	auto run_mode = chRunMode->GetStringSelection();
	bool is_dosbox = run_mode == "DOSbox";
	
	if(spell_dir.empty())
	{
		textOutput->AppendText(" - failed! Missing Spellcross directory path.\n");
		sbar->SetStatusText("Ready!");
		return;
	}
	if(is_dosbox && dosbox_path.empty())
	{
		textOutput->AppendText(" - failed! Missing DOSbox path.\n");
		sbar->SetStatusText("Ready!");
		return;
	}
	if(spell_exe == str_choice_none)
	{
		textOutput->AppendText(" - failed! Missing Spellcross executable name.\n");
		sbar->SetStatusText("Ready!");
		return;
	}
	
	// make CD drive select file
	textOutput->AppendText(" - making SPELCROS.CD file\n");
	SpellLaunch::MakeSpellCD(spell_dir,drive);
	
	if(!dosbox_path.empty())
	{
		// make batch files to be run inside DOSbox
		textOutput->AppendText(" - making box_exec.bat file\n");
		SpellLaunch::MakeSpellDOSboxSetupBat(spell_dir);
		textOutput->AppendText(" - making box_setup.bat file\n");
		SpellLaunch::MakeSpellDOSboxExecBat(spell_dir, spellcd_dir, drive, spell_exe.ToStdString());
	
		// make batch files to run DOSbox
		textOutput->AppendText(" - making run_DOXbox.bat file\n");
		SpellLaunch::MakeSpellDOSboxRunBat(spell_dir, dosbox_path, no_autoexec, fullscreen);
		textOutput->AppendText(" - making setup_DOXbox.bat file\n");
		SpellLaunch::MakeSpellDOSboxRunSetupBat(spell_dir, dosbox_path);
	}

	// make Win32 launch batch
	textOutput->AppendText(" - making run_Win32.bat file\n");
	SpellLaunch::MakeSpellWin32RunBat(spell_dir, spellcd_dir, drive, spell_exe.ToStdString());
	
	textOutput->AppendText(" - all done!\n");

	sbar->SetStatusText("Ready!");
}



