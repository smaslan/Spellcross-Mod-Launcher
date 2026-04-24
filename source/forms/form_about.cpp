///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "form_about.h"
#include <chrono>
#include <iomanip>
#include <sstream>

///////////////////////////////////////////////////////////////////////////

FormAbout::FormAbout( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 600,350 ), wxDefaultSize );

	wxBoxSizer* bSizer73;
	bSizer73 = new wxBoxSizer( wxVERTICAL );

	m_staticText80 = new wxStaticText( this, wxID_ANY, wxT("Spellcross Map Editor"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText80->Wrap( -1 );
	m_staticText80->SetFont( wxFont( 16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxEmptyString ) );

	bSizer73->Add( m_staticText80, 0, wxALIGN_CENTER|wxALL, 5 );

	m_staticline29 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer73->Add( m_staticline29, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );

	wxBoxSizer* bSizer74;
	bSizer74 = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bSizer75;
	bSizer75 = new wxBoxSizer( wxVERTICAL );

	m_textCtrl17 = new wxTextCtrl( this, wxID_ANY, wxT("Version:"), wxDefaultPosition, wxDefaultSize, wxTE_RICH|wxBORDER_NONE );
	m_textCtrl17->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	m_textCtrl17->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );

	bSizer75->Add( m_textCtrl17, 0, wxALL|wxEXPAND, 5 );

	m_textCtrl171 = new wxTextCtrl( this, wxID_ANY, wxT("Author:"), wxDefaultPosition, wxDefaultSize, wxTE_RICH|wxBORDER_NONE );
	m_textCtrl171->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	m_textCtrl171->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );

	bSizer75->Add( m_textCtrl171, 0, wxALL, 5 );

	m_textCtrl172 = new wxTextCtrl( this, wxID_ANY, wxT("Email:"), wxDefaultPosition, wxDefaultSize, wxTE_RICH|wxBORDER_NONE );
	m_textCtrl172->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	m_textCtrl172->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );

	bSizer75->Add( m_textCtrl172, 0, wxALL, 5 );

	m_textCtrl173 = new wxTextCtrl( this, wxID_ANY, wxT("URL:"), wxDefaultPosition, wxDefaultSize, wxTE_RICH|wxBORDER_NONE );
	m_textCtrl173->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	m_textCtrl173->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );

	bSizer75->Add( m_textCtrl173, 0, wxALL, 5 );

	m_textCtrl174 = new wxTextCtrl( this, wxID_ANY, wxT("License:"), wxDefaultPosition, wxDefaultSize, wxTE_RICH|wxBORDER_NONE );
	m_textCtrl174->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	m_textCtrl174->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );

	bSizer75->Add( m_textCtrl174, 0, wxALL, 5 );


	bSizer74->Add( bSizer75, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer76;
	bSizer76 = new wxBoxSizer( wxVERTICAL );

	txtVersion = new wxTextCtrl( this, wxID_TXT_VER, wxT("V1.00, 2025-11-16"), wxDefaultPosition, wxDefaultSize, wxTE_RICH|wxBORDER_NONE );
	txtVersion->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	txtVersion->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );

	bSizer76->Add( txtVersion, 0, wxBOTTOM|wxEXPAND|wxTOP, 5 );

	txtName = new wxTextCtrl( this, wxID_TXT_NAME, L"Stanislav Mašláò", wxDefaultPosition, wxDefaultSize, wxTE_RICH|wxBORDER_NONE );
	txtName->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	txtName->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );

	bSizer76->Add( txtName, 0, wxBOTTOM|wxEXPAND|wxRIGHT|wxTOP, 5 );

	txtEmail = new wxTextCtrl( this, wxID_TXT_EMAIL, wxT("s.maslan@seznam.cz"), wxDefaultPosition, wxDefaultSize, wxTE_RICH|wxBORDER_NONE );
	txtEmail->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	txtEmail->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );

	bSizer76->Add( txtEmail, 0, wxBOTTOM|wxEXPAND|wxTOP, 5 );

	txtURL = new wxTextCtrl( this, wxID_TXT_URL, wxT("https://github.com/smaslan/spellcross-map-edit"), wxDefaultPosition, wxDefaultSize, wxTE_AUTO_URL|wxTE_RICH|wxBORDER_NONE );
	txtURL->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	txtURL->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );

	bSizer76->Add( txtURL, 0, wxBOTTOM|wxEXPAND|wxTOP, 5 );

	txtLicense = new wxTextCtrl( this, wxID_TXT_LICENSE, L"MIT License, (c) 2026, Stanislav Mašláò", wxDefaultPosition, wxDefaultSize, wxTE_RICH|wxBORDER_NONE );
	txtLicense->SetFont( wxFont( 12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxEmptyString ) );
	txtLicense->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );

	bSizer76->Add( txtLicense, 0, wxBOTTOM|wxEXPAND|wxTOP, 5 );


	bSizer74->Add( bSizer76, 1, wxEXPAND, 5 );


	bSizer73->Add( bSizer74, 0, wxEXPAND, 5 );

	m_staticline30 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer73->Add( m_staticline30, 0, wxEXPAND | wxALL, 5 );

	txtDesc = new wxTextCtrl( this, wxID_TXT_DESC, wxT("Very experimental editor for Spellcross map files and collection of Spellcross data loaders and viewers."), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxTE_RICH|wxTE_WORDWRAP|wxBORDER_NONE );
	txtDesc->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );

	bSizer73->Add( txtDesc, 1, wxALL|wxEXPAND, 5 );

	m_staticline31 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	bSizer73->Add( m_staticline31, 0, wxEXPAND | wxALL, 5 );

	btnOK = new wxButton( this, wxID_BTN_OK, wxT("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer73->Add( btnOK, 0, wxALL|wxEXPAND, 5 );


	this->SetSizer( bSizer73 );
	this->Layout();

	this->Centre( wxBOTH );


	// === AUTO GENERATED END ===


	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormAbout::OnClose,this,wxID_BTN_OK);

	// assign button shortcuts
	std::vector<wxAcceleratorEntry> entries;
	entries.emplace_back(wxACCEL_NORMAL,WXK_RETURN,wxID_BTN_OK);
	entries.emplace_back(wxACCEL_NORMAL,WXK_ESCAPE,wxID_BTN_OK);
	wxAcceleratorTable accel(entries.size(),entries.data());
	this->SetAcceleratorTable(accel);

	/*std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	std::time_t currentTime = std::chrono::system_clock::to_time_t(now);
	std::stringstream ver;
	ver << std::put_time(std::localtime(&currentTime),"%Y-%m-%d %H:%M:%S");	*/	
	txtVersion->SetValue("V1.00, build: " __DATE__);

	auto desc = "Simple experimental tool for runtime build of mods of Spellcross game archives.\nIt can also be used as game launcher via DOSbox.";
	txtDesc->SetValue(desc);
}

FormAbout::~FormAbout()
{
}

// on close form
void FormAbout::OnClose(wxCommandEvent& event)
{
	EndModal(wxID_OK);
}