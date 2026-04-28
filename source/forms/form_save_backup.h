///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/statusbr.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/stattext.h>
#include <wx/choice.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/bmpbuttn.h>
#include <wx/button.h>
#include <wx/listbox.h>
#include <wx/checklst.h>
#include <wx/statline.h>
#include <wx/frame.h>

#include <filesystem>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class FormEdit
///////////////////////////////////////////////////////////////////////////////
class FormSaveBack : public wxFrame
{
private:
	void OnClose(wxCloseEvent& ev);
	void OnCloseClick(wxCommandEvent& event);
	void OnOK(wxCommandEvent& event);
	void OnRemove(wxCommandEvent& event);
	void OnSetModPath(wxCommandEvent& event);
	void OnChangeSet(wxCommandEvent& event);
	void OnChangeBackup(wxCommandEvent& event);
	void OnChangeBackupName(wxCommandEvent& event);
	void OnSavesPupupOpen(wxMouseEvent& event);
	void OnSavesPupup(wxCommandEvent& event);
	
	void OnUpdateList(std::string backup_to_select="");

	std::filesystem::path m_spell_path;
	std::filesystem::path m_backup_dir;

	std::string str_backup = "Backup";
	std::string str_restore = "Restore";

	std::string str_orig_saves = "Original game saves";
	std::string str_mod_saves = "Mod save games";

	std::wstring str_backup_subdir = L"save_backups";

	enum SavesPopup : int{
		ALL,
		NONE,
		VALID
	};

	class SavesData: public wxClientData {
	public:
		SavesData(int empty) : m_empty(empty) {}
		int m_empty;
	};


protected:
	enum
	{
		wxID_FORM_SAVE_BACK = 6000,
		wxID_SBAR,
		wxID_MM_EXIT,
		wxID_CH_SRC_DEST,
		wxID_CH_OPER,
		wxID_TXT_MOD_PATH,
		wxID_BTN_MOD_PATH,
		wxID_LBL_DEST,
		wxID_TXT_DEST,
		wxID_LBL_SRC,
		wxID_TXT_SRC,
		wxID_LBL_GAME_SAVES,
		wxID_LB_GAME_SAVES,
		wxID_LBL_BACKUP_SAVES,
		wxID_LB_BACKUP_SAVES,
		wxID_LB_LIST,
		wxID_TXT_NAME,
		wxID_TXT_DATE,
		wxID_LBL_DESC,
		wxID_TXT_DESCRIPTION,
		wxID_BTN_REMOVE,
		wxID_BTN_OK,
		wxID_BTN_EXIT,
	};

	wxStatusBar* sbar;
	wxMenuBar* m_menubar2;
	wxMenu* m_menu6;
	wxStaticText* m_staticText15;
	wxChoice* chSourceDest;
	wxStaticText* m_staticText152;
	wxChoice* chOperation;
	wxStaticText* m_staticText151;
	wxTextCtrl* txtModPath;
	wxBitmapButton* btnModPath;
	wxStaticText* lblDest;
	wxTextCtrl* txtDest;
	wxStaticText* lblSource;
	wxTextCtrl* txtSource;
	wxStaticText* lblGameSaves;
	wxListBox* lbGameSaves;
	wxStaticText* lblBackupSaves;
	wxCheckListBox* lbBackupSaves;
	wxStaticLine* m_staticline10;
	wxStaticText* m_staticText22;
	wxListBox* lbList;
	wxStaticLine* m_staticline8;
	wxStaticText* m_staticText24;
	wxTextCtrl* txtName;
	wxStaticText* m_staticText23;
	wxTextCtrl* txtDate;
	wxStaticText* lblDesc;
	wxTextCtrl* txtDescription;
	wxStaticLine* m_staticline9;
	wxButton* btnRemove;
	wxButton* btnConfirm;
	wxButton* btnExit;

public:

	FormSaveBack(wxWindow* parent,wxWindowID id = wxID_FORM_SAVE_BACK,const wxString& title = _("Save Game Backup Manager"),const wxPoint& pos = wxDefaultPosition,const wxSize& size = wxSize(900,750),long style = wxDEFAULT_FRAME_STYLE|wxFRAME_FLOAT_ON_PARENT|wxTAB_TRAVERSAL);
	~FormSaveBack();
	
	void SetSpellPath(std::filesystem::path path);
	void SetModPath(std::filesystem::path path);
	void SetMod(bool is_mod);
	void SetBackup(bool is_backup);
	

};

