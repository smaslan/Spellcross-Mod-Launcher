///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

// <wxFormsBuilder-include> - Section auto-inserted from 'forms.h' class 'FormAbout' on 2026-09-04 19:27:33
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

// </wxFormsBuilder-include> - Section auto-inserted from 'forms.h' class 'FormAbout' on 2026-09-04 19:27:33

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class FormAbout
///////////////////////////////////////////////////////////////////////////////
class FormAbout : public wxDialog
{
	private:

		void OnClose(wxCommandEvent& event);
		void OnURL(wxTextUrlEvent& event);

	protected:
		// <wxFormsBuilder> - Section auto-inserted from 'forms.h' class 'FormAbout' on 2026-09-04 19:27:33
		enum
		{
			wxID_FORM_ABOUT = 6000,
			wxID_TXT_VER,
			wxID_TXT_NAME,
			wxID_TXT_EMAIL,
			wxID_TXT_URL,
			wxID_TXT_LICENSE,
			wxID_IMG,
			wxID_TXT_DESC,
			wxID_BTN_OK,
		};
		
		wxStaticText* m_staticText80;
		wxStaticLine* m_staticline29;
		wxTextCtrl* m_textCtrl17;
		wxTextCtrl* m_textCtrl171;
		wxTextCtrl* m_textCtrl172;
		wxTextCtrl* m_textCtrl173;
		wxTextCtrl* m_textCtrl174;
		wxTextCtrl* txtVersion;
		wxTextCtrl* txtName;
		wxTextCtrl* txtEmail;
		wxTextCtrl* txtURL;
		wxTextCtrl* txtLicense;
		wxStaticBitmap* imgLogo;
		wxStaticLine* m_staticline30;
		wxTextCtrl* txtDesc;
		wxStaticLine* m_staticline31;
		wxButton* btnOK;

		// </wxFormsBuilder> - Section auto-inserted from 'forms.h' class 'FormAbout' on 2026-09-04 19:27:33

	public:

		FormAbout( wxWindow* parent,std::string ver_label,wxWindowID id = wxID_FORM_ABOUT, const wxString& title = wxT("About Spellcross Mod Launcher"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 900,500 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		~FormAbout();		

};

