//=============================================================================
// Spellcross Mod Launcher
// ----------------------------------------------------------------------------
// Top level functions, wxWidgets GUI.
// 
// This code is part of Spellcross Mod Launcher project.
// (c) 2026, Stanislav Maslan, s.maslan@seznam.cz
// url: 
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================

#ifndef MAIN_H
#define MAIN_H

#include "simpleini.h"
#include "SpellMod.h"

#include <wx/thread.h>
#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/statusbr.h>
#include <wx/stattext.h>
#include <wx/choice.h>
#include <wx/sizer.h>
#include <wx/statline.h>
#include <wx/checkbox.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/tglbtn.h>
#include <wx/frame.h>


// app entry point class
class MyApp : public wxApp
{
private:
    CSimpleIniA ini;
public:
    virtual bool OnInit();
    virtual int OnExit();
};

class ProcTh : public wxThread
{
public:

	enum Event : int
	{
		PRINT,
		CLEAR,
		STATUS,
		DONE
	};

	enum Action : int
	{
		BUILD,
		RESTORE,
		SWAP,
		RUN,
		SETUP
	};

	typedef std::vector<Action> ActionsList;

	class Params
	{
	public:
		std::filesystem::path mod_path;
		std::filesystem::path spell_dir;
		std::filesystem::path spellcd_dir;
		std::filesystem::path state_ini_path;
		std::filesystem::path doxbox_path;
		std::string cd_drive;
		std::string run_mode;
		bool allow_cd_mod;
		bool dosbox_no_autoexec;
		bool dosbox_fullscreen;
		bool move_saves;
	};

	ProcTh(wxFrame* parent,Params &config,ActionsList &actions);
	virtual ExitCode Entry();	

private:
	wxFrame* m_parent;
	Params m_config;
	ActionsList m_actions;

	void ConsoleStringCallback(std::string info);
	void ConsoleClearCallback();
	void SetStatusCallback(std::string info);

};

class FormMain : public wxFrame
{
private:
	void OnClose(wxCloseEvent& ev);
	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnHelp(wxCommandEvent& event);
	void OnEdit(wxCommandEvent& event);
	void OnEditDOSboxConf(wxCommandEvent& event);
	void OnSelectSpellPath(wxCommandEvent& event);
	void OnSelectDOSboxPath(wxCommandEvent& event);
	void OnSelectModPath(wxCommandEvent& event);
	void OnBuildMod(wxCommandEvent& event);
	void OnBuildModEvent(wxThreadEvent& event);
	void OnBuildLaunchFiles(wxCommandEvent& event);
	void OnRunGame(wxCommandEvent& event);
	void OnRunGameSetup(wxCommandEvent& event);
	void OnModInfo(wxCommandEvent& event);

	//void ConsoleStringCallback(std::string info);
	
	void EditFile(std::filesystem::path path);
	void SetControlsState(bool busy);
	void FillChoicePaths(wxChoice* choice,CSimpleIniA* ini,std::string section,std::string key);
	void SaveChoicePaths(wxChoice* choice,CSimpleIniA* ini,std::string section,std::string key,std::string comment="");
	void ChoiceCheckPaths(wxChoice *choice);
	std::wstring GetPathChoiceLastPath(wxChoice* choice,std::wstring default_path=L"");	
	void ListSpellExecutables(std::filesystem::path spell_dir, wxChoice* choice);

	void MakeSpellLaunchFiles();
	
	CSimpleIniA *m_ini;	

	void StartProcTh(ProcTh::Params& params, ProcTh::ActionsList actions);
	ProcTh *m_th_proc;

	const std::string str_choice_browse=">>> Browse <<<";
	const std::string str_choice_none=">>> None found <<<";
	const std::string str_mod_state_ini_none="mod_state.ini";
	

	FormEdit *form_edit;

protected:
	int wxID_FORM_EDIT = 5999;

	enum
	{
		wxID_FORM_MAIN = 6000,
		wxID_MM,
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
		wxID_MM_HELP,
		wxID_MM_ABOUT,
		wxID_SBAR,
		wxID_CH_SPELL_PATH,
		wxID_CH_SPELL_EXE,
		wxID_CH_SPELLCD_PATH,
		wxID_CH_CD_LETTER,
		wxID_CH_DOSBOX_PATH,
		wxID_CB_NO_AUTOEXEC,
		wxID_CB_FULLSCREEN,
		wxID_CH_RUN_MODE,
		wxID_CH_MOD_PATH,
		wxID_CB_ALLOW_CD_MOD,
		wxID_CB_MOD_SAVES,
		wxID_TEXT_OUTPUT,
		wxID_BTN_RUN_ORIG,
		wxID_BTN_MOD_INFO,
		wxID_BTN_RESTORE,
		wxID_BTN_CLEANUP,
		wxID_BTN_BUILD,
		wxID_BTN_BUILD_SWAP,
		wxID_BTN_RUN_MOD,
	};

	wxMenuBar* mMenu;
	wxMenu* mmFile;
	wxMenu* mmSetup;
	wxMenu* mmMod;
	wxMenu* m_menu7;
	wxMenu* m_menu1;
	wxMenu* m_menu11;
	wxMenu* mmRun;
	wxMenu* mmHelpMenu;
	wxStatusBar* sbar;
	wxStaticText* m_staticText115;
	wxChoice* chSpellPath;
	wxStaticText* m_staticText1151;
	wxChoice* chSpellExec;
	wxStaticText* m_staticText116;
	wxChoice* chSpellCdPath;
	wxStaticText* m_staticText117;
	wxChoice* chVirtCD;
	wxStaticLine* m_staticline42;
	wxStaticText* m_staticText118;
	wxChoice* chDOSboxPath;
	wxCheckBox* cbNoAutoexec;
	wxCheckBox* cbFullscreen;
	wxStaticLine* m_staticline4;
	wxStaticText* m_staticText9;
	wxChoice* chRunMode;
	wxStaticLine* m_staticline43;
	wxStaticText* m_staticText119;
	wxChoice* chModPath;
	wxCheckBox* cbAllowCDmod;
	wxCheckBox* cbModSaves;
	wxStaticLine* m_staticline44;
	wxStaticText* m_staticText120;
	wxTextCtrl* textOutput;
	wxButton* btnRunOrig;
	wxButton* btnModInfo;
	wxButton* btnRestore;
	wxButton* btnCleanupMod;
	wxButton* btnBuild;
	wxButton* btnBuildSwap;
	wxButton* btnRunMod;

public:
	
	static const int wxID_PROC_THREAD = 7000;

	FormMain(wxWindow* parent,CSimpleIniA* ini,wxWindowID id = wxID_FORM_MAIN, const wxString& title = _("Spellcross Mod Launcher"),const wxPoint& pos = wxDefaultPosition,const wxSize& size = wxSize(900,650),long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL);

	~FormMain();

};


#endif