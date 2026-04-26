///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/statline.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/dialog.h>

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
		enum
		{
			wxID_FORM_ABOUT = 5999,
			wxID_TXT_VER,
			wxID_TXT_NAME,
			wxID_TXT_EMAIL,
			wxID_TXT_URL,
			wxID_TXT_LICENSE,
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
		wxStaticLine* m_staticline30;
		wxTextCtrl* txtDesc;
		wxStaticLine* m_staticline31;
		wxButton* btnOK;

	public:

		FormAbout( wxWindow* parent, wxWindowID id = wxID_FORM_ABOUT, const wxString& title = wxT("About Spellcross Mod Launcher"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 600,350 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		~FormAbout();
		

};

