///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "form_patch.h"
#include "../other.h"
#include "../wx_other.h"

///////////////////////////////////////////////////////////////////////////


FormPatch::FormPatch(wxWindow* parent,wxWindowID id,const wxString& title,const wxPoint& pos,const wxSize& size,long style) : wxDialog(parent,id,title,pos,size,style)
{
	// <wxFormsBuilder> - Section auto-inserted from 'forms.cpp' class 'FormPatch' on 2026-09-30 20:08:21
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer104;
	bSizer104 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText62 = new wxStaticText( this, wxID_ANY, _("Current version:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText62->Wrap( -1 );
	bSizer104->Add( m_staticText62, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	txtCurVer = new wxTextCtrl( this, wxID_TXT_CUR_VER, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	bSizer104->Add( txtCurVer, 0, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );
	
	m_staticText63 = new wxStaticText( this, wxID_ANY, _("Available patches:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText63->Wrap( -1 );
	bSizer104->Add( m_staticText63, 0, wxRIGHT|wxLEFT, 5 );
	
	wxArrayString chPatchesChoices;
	chPatches = new wxChoice( this, wxID_CH_PATCHES, wxDefaultPosition, wxDefaultSize, chPatchesChoices, 0 );
	chPatches->SetSelection( 0 );
	bSizer104->Add( chPatches, 0, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );
	
	txtMessage = new wxTextCtrl( this, wxID_TXT_MSG, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxTE_WORDWRAP );
	bSizer104->Add( txtMessage, 1, wxEXPAND|wxALL, 5 );
	
	wxBoxSizer* bSizer105;
	bSizer105 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer105->Add( 0, 0, 1, wxEXPAND, 5 );
	
	btnPatch = new wxButton( this, wxID_BTN_PATCH, _("Apply Patch"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer105->Add( btnPatch, 0, wxALL, 5 );
	
	btnExit = new wxButton( this, wxID_BTN_EXIT, _("Exit"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer105->Add( btnExit, 0, wxALL, 5 );
	
	
	bSizer104->Add( bSizer105, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer104 );
	this->Layout();
	
	this->Centre( wxBOTH );
	

	// </wxFormsBuilder> - Section auto-inserted from 'forms.cpp' class 'FormPatch' on 2026-09-30 20:08:21
	// === AUTO GENERATED END ===
	RescaleWindowDPI(this);
		
	// set icon
	wxIcon appIcon;
	appIcon.LoadFile("IDI_ICON2",wxBITMAP_TYPE_ICO_RESOURCE);
	if(appIcon.IsOk())
		SetIcon(appIcon);

	
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormPatch::OnCloseClick,this,wxID_BTN_EXIT);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormPatch::OnCloseClick,this,wxID_BTN_PATCH);
	
	// assign button shortcuts
	/*std::vector<wxAcceleratorEntry> entries;
	entries.emplace_back(wxACCEL_NORMAL,WXK_RETURN,wxID_BTN_YES);
	entries.emplace_back(wxACCEL_NORMAL,WXK_ESCAPE,wxID_BTN_NO);
	wxAcceleratorTable accel(entries.size(),entries.data());
	this->SetAcceleratorTable(accel);*/

}

FormPatch::~FormPatch()
{
}

// close window
void FormPatch::OnClose(wxCloseEvent& ev)
{
	
}

// on close form
void FormPatch::OnCloseClick(wxCommandEvent& event)
{	
	EndModal(event.GetId() == wxID_BTN_PATCH);
	//Close();
}

// set patch options
void FormPatch::SetOptions(std::vector<std::string>& options)
{
	chPatches->Freeze();
	chPatches->Clear();
	if(options.empty())
		chPatches->Append("<Not available>");
	else
		for(auto &opt: options)
			chPatches->Append(opt);
	chPatches->Thaw();
	chPatches->Select(chPatches->GetCount()-1);
}
// set messages
void FormPatch::SetMessage(std::string& msg,std::string& ver)
{
	txtMessage->SetValue(msg);
	txtCurVer->SetValue(ver);
}

// get selection
std::string FormPatch::GetOption()
{
	return(chPatches->GetStringSelection().ToStdString());
}