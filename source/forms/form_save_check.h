///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

// <wxFormsBuilder-include> - Section auto-inserted from 'forms.h' class 'FormSaveCheck' on 2026-09-06 09:49:45
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
#include <wx/bmpbuttn.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/statline.h>
#include <wx/checkbox.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#include <wx/frame.h>
#include <wx/statbmp.h>
#include <wx/dialog.h>
#include <wx/listbox.h>
#include <wx/checklst.h>
#include <wx/grid.h>
#include <wx/panel.h>
#include <wx/notebook.h>

// </wxFormsBuilder-include> - Section auto-inserted from 'forms.h' class 'FormSaveCheck' on 2026-09-06 09:49:45

#include <filesystem>
#include "../SpellSaves.h"

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class FormEdit
///////////////////////////////////////////////////////////////////////////////
class FormSaveCheck : public wxDialog
{
private:
	void OnClose(wxCloseEvent& ev);
	void OnCloseClick(wxCommandEvent& event);
	void OnFixSavesClick(wxCommandEvent& event);
	void OnSetCommonPathClick(wxCommandEvent& event);
	void OnSetSavesPathClick(wxCommandEvent& event);
	void OnPupupOpen(wxMouseEvent& event);
	void OnPupup(wxCommandEvent& event);
	
	bool UpdateCheck(bool do_fix=false);
	
	std::filesystem::path m_common_fs;
	std::filesystem::path m_saves_dir;
	SpellSave::Saves m_saves;

protected:
	// <wxFormsBuilder> - Section auto-inserted from 'forms.h' class 'FormSaveCheck' on 2026-09-06 09:49:45
	enum
	{
		wxID_FORM_SAVE_CHECK = 6000,
		wxID_TXT_SAVE_DIR,
		wxID_BTN_OPEN_SAVE_DIR,
		wxID_TXT_COMMON_FS,
		wxID_BTN_OPEN_COMMON_FS,
		wxID_LBOX_SAVES,
		wxID_BTN_YES,
		wxID_BTN_NO,
	};
	
	wxStaticText* m_staticText53;
	wxTextCtrl* txtSaveDir;
	wxBitmapButton* btnOpenSaveDir;
	wxStaticText* m_staticText54;
	wxTextCtrl* txtCommonFs;
	wxBitmapButton* btnOpenCommonFs;
	wxTextCtrl* txtMessage;
	wxStaticText* m_staticText55;
	wxCheckListBox* lboxSaves;
	wxButton* btnYes;
	wxButton* btnNo;

	// </wxFormsBuilder> - Section auto-inserted from 'forms.h' class 'FormSaveCheck' on 2026-09-06 09:49:45

public:

	class Params{
	public:
		std::filesystem::path common_fs_path;
		std::filesystem::path saves_dir;
		std::vector<std::string> save_names;
		int result;
	};

	FormSaveCheck(wxWindow* parent,Params &params,wxWindowID id = wxID_FORM_SAVE_CHECK,const wxString& title = _("Checking save games"),const wxPoint& pos = wxDefaultPosition,const wxSize& size = wxSize(981,665),long style = wxCAPTION|wxCLOSE_BOX|wxDEFAULT_DIALOG_STYLE|wxSTAY_ON_TOP|wxRESIZE_BORDER);
	~FormSaveCheck();

};

