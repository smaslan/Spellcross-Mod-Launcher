///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

// <wxFormsBuilder-include> - Section auto-inserted from 'forms.h' class 'FormEdit' on 2026-05-02 21:33:41
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
#include <wx/statline.h>
#include <wx/checkbox.h>
#include <wx/textctrl.h>
#include <wx/frame.h>
#include <wx/statbmp.h>
#include <wx/dialog.h>
#include <wx/listbox.h>
#include <wx/checklst.h>

// </wxFormsBuilder-include> - Section auto-inserted from 'forms.h' class 'FormEdit' on 2026-05-02 21:33:41

#include <filesystem>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class FormEdit
///////////////////////////////////////////////////////////////////////////////
class FormEdit : public wxFrame
{
private:
	void OnClose(wxCloseEvent& ev);
	void OnCloseClick(wxCommandEvent& event);
	void OnSave(wxCommandEvent& event);
	void OnOpen(wxCommandEvent& event);
	void OnEdit(wxCommandEvent& event);

	bool m_modified;
	std::filesystem::path m_path;

protected:
	// <wxFormsBuilder> - Section auto-inserted from 'forms.h' class 'FormEdit' on 2026-05-02 21:33:41
	enum
	{
		wxID_FORM_EDIT = 6000,
		wxID_SBAR,
		wxID_MM_OPEN,
		wxID_MM_SAVE,
		wxID_MM_EXIT,
		wxID_TEXT,
		wxID_BTN_SAVE,
		wxID_BTN_EXIT,
	};
	
	wxStatusBar* sbar;
	wxMenuBar* m_menubar2;
	wxMenu* m_menu6;
	wxTextCtrl* text;
	wxButton* btnSave;
	wxButton* btnExit;

	// </wxFormsBuilder> - Section auto-inserted from 'forms.h' class 'FormEdit' on 2026-05-02 21:33:41

public:

	FormEdit(wxWindow* parent,wxWindowID id = wxID_ANY,const wxString& title = _("Text editor"),const wxPoint& pos = wxDefaultPosition,const wxSize& size = wxSize(608,418),long style = wxDEFAULT_FRAME_STYLE|wxSTAY_ON_TOP|wxTAB_TRAVERSAL);
	~FormEdit();
	void SetPath(std::filesystem::path path);

};

