///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

// <wxFormsBuilder-include> - Section auto-inserted from 'forms.h' class 'FormPatch' on 2026-09-30 20:08:22
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

// </wxFormsBuilder-include> - Section auto-inserted from 'forms.h' class 'FormPatch' on 2026-09-30 20:08:22

#include <filesystem>
#include <vector>
#include <string>

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class FormEdit
///////////////////////////////////////////////////////////////////////////////
class FormPatch : public wxDialog
{
private:
	void OnClose(wxCloseEvent& ev);
	void OnCloseClick(wxCommandEvent& event);

protected:
	// <wxFormsBuilder> - Section auto-inserted from 'forms.h' class 'FormPatch' on 2026-09-30 20:08:22
	enum
	{
		wxID_TXT_CUR_VER = 6000,
		wxID_CH_PATCHES,
		wxID_TXT_MSG,
		wxID_BTN_PATCH,
		wxID_BTN_EXIT,
	};
	
	wxStaticText* m_staticText62;
	wxTextCtrl* txtCurVer;
	wxStaticText* m_staticText63;
	wxChoice* chPatches;
	wxTextCtrl* txtMessage;
	wxButton* btnPatch;
	wxButton* btnExit;

	// </wxFormsBuilder> - Section auto-inserted from 'forms.h' class 'FormPatch' on 2026-09-30 20:08:22

public:

	FormPatch(wxWindow* parent,wxWindowID id = wxID_ANY,const wxString& title = _("Patching SPELCROS.EXE"),const wxPoint& pos = wxDefaultPosition,const wxSize& size = wxSize(800,500),
		long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxSTAY_ON_TOP);
	~FormPatch();

	void SetOptions(std::vector<std::string> &options);
	void SetMessage(std::string& msg,std::string& ver);
	std::string GetOption();

};

