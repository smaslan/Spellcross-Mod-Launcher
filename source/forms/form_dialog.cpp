///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "form_dialog.h"
#include "../other.h"
#include "../wx_other.h"

///////////////////////////////////////////////////////////////////////////


FormDialog::FormDialog(wxWindow* parent, wxString message, wxWindowID id,const wxString& title,const wxPoint& pos,const wxSize& size,long style) : wxDialog(parent,id,title,pos,size,style)
{
	// <wxFormsBuilder> - Section auto-inserted from 'forms.cpp' class 'FormDialog' on 2026-09-05 21:18:12
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer100;
	bSizer100 = new wxBoxSizer( wxVERTICAL );
	
	txtMessage = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
	bSizer100->Add( txtMessage, 1, wxEXPAND|wxALL, 5 );
	
	wxBoxSizer* bSizer101;
	bSizer101 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer101->Add( 0, 0, 1, wxEXPAND, 5 );
	
	btnYes = new wxButton( this, wxID_BTN_YES, _("Yes"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer101->Add( btnYes, 0, wxALL, 5 );
	
	btnNo = new wxButton( this, wxID_BTN_NO, _("No"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer101->Add( btnNo, 0, wxALL, 5 );
	
	
	bSizer100->Add( bSizer101, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer100 );
	this->Layout();
	
	this->Centre( wxBOTH );
	

	// </wxFormsBuilder> - Section auto-inserted from 'forms.cpp' class 'FormDialog' on 2026-09-05 21:18:12
	// === AUTO GENERATED END ===
	RescaleWindowDPI(this);
		
	// set icon
	wxIcon appIcon;
	appIcon.LoadFile("IDI_ICON2",wxBITMAP_TYPE_ICO_RESOURCE);
	if(appIcon.IsOk())
		SetIcon(appIcon);

	txtMessage->SetValue(message);

	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormDialog::OnCloseClick,this,wxID_BTN_YES);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormDialog::OnCloseClick,this,wxID_BTN_NO);
	
	// assign button shortcuts
	std::vector<wxAcceleratorEntry> entries;
	entries.emplace_back(wxACCEL_NORMAL,WXK_RETURN,wxID_BTN_YES);
	entries.emplace_back(wxACCEL_NORMAL,WXK_ESCAPE,wxID_BTN_NO);
	wxAcceleratorTable accel(entries.size(),entries.data());
	this->SetAcceleratorTable(accel);

}

FormDialog::~FormDialog()
{
}

// close window
void FormDialog::OnClose(wxCloseEvent& ev)
{
	
}

// on close form
void FormDialog::OnCloseClick(wxCommandEvent& event)
{	
	EndModal(event.GetId() == wxID_BTN_YES);
	//Close();
}

