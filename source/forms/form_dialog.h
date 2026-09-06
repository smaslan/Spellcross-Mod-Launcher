///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

// <wxFormsBuilder-include> - Section auto-inserted from 'forms.h' class 'FormDialog' on 2026-09-05 21:18:12
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

// </wxFormsBuilder-include> - Section auto-inserted from 'forms.h' class 'FormDialog' on 2026-09-05 21:18:12

#include <filesystem>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class FormEdit
///////////////////////////////////////////////////////////////////////////////
class FormDialog : public wxDialog
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
	// <wxFormsBuilder> - Section auto-inserted from 'forms.h' class 'FormDialog' on 2026-09-05 21:18:12
	enum
	{
		wxID_FORM_DIALOG = 6000,
		wxID_BTN_YES,
		wxID_BTN_NO,
	};
	
	wxTextCtrl* txtMessage;
	wxButton* btnYes;
	wxButton* btnNo;

	// </wxFormsBuilder> - Section auto-inserted from 'forms.h' class 'FormDialog' on 2026-09-05 21:18:12

public:

	FormDialog(wxWindow* parent,wxString message,wxWindowID id = wxID_FORM_DIALOG,const wxString& title = wxEmptyString,const wxPoint& pos = wxDefaultPosition,const wxSize& size = wxSize(981,539),long style = wxCAPTION|wxCLOSE_BOX|wxDEFAULT_DIALOG_STYLE|wxSTAY_ON_TOP);
	~FormDialog();

};

