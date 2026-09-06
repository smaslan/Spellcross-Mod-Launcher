///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "form_save_check.h"
#include "../other.h"
#include "../wx_other.h"
#include <wx/filedlg.h>
#include <wx/dirdlg.h>
#include <wx/msgdlg.h>

///////////////////////////////////////////////////////////////////////////


FormSaveCheck::FormSaveCheck(wxWindow* parent,Params& params,wxWindowID id,const wxString& title,const wxPoint& pos,const wxSize& size,long style) : wxDialog(parent,id,title,pos,size,style)
{
	// <wxFormsBuilder> - Section auto-inserted from 'forms.cpp' class 'FormSaveCheck' on 2026-09-06 09:49:45
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer100;
	bSizer100 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText53 = new wxStaticText( this, wxID_ANY, _("Save game root folder:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText53->Wrap( -1 );
	bSizer100->Add( m_staticText53, 0, wxTOP|wxRIGHT|wxLEFT, 5 );
	
	wxBoxSizer* bSizer106;
	bSizer106 = new wxBoxSizer( wxHORIZONTAL );
	
	txtSaveDir = new wxTextCtrl( this, wxID_TXT_SAVE_DIR, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	bSizer106->Add( txtSaveDir, 1, wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT, 5 );
	
	btnOpenSaveDir = new wxBitmapButton( this, wxID_BTN_OPEN_SAVE_DIR, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|wxBORDER_NONE );
	bSizer106->Add( btnOpenSaveDir, 0, wxRIGHT|wxLEFT, 5 );
	
	
	bSizer100->Add( bSizer106, 0, wxEXPAND, 5 );
	
	m_staticText54 = new wxStaticText( this, wxID_ANY, _("Reference common.fs archive:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText54->Wrap( -1 );
	bSizer100->Add( m_staticText54, 0, wxRIGHT|wxLEFT, 5 );
	
	wxBoxSizer* bSizer107;
	bSizer107 = new wxBoxSizer( wxHORIZONTAL );
	
	txtCommonFs = new wxTextCtrl( this, wxID_TXT_COMMON_FS, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	bSizer107->Add( txtCommonFs, 1, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );
	
	btnOpenCommonFs = new wxBitmapButton( this, wxID_BTN_OPEN_COMMON_FS, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|wxBORDER_NONE );
	bSizer107->Add( btnOpenCommonFs, 0, wxRIGHT|wxLEFT, 5 );
	
	
	bSizer100->Add( bSizer107, 0, wxEXPAND, 5 );
	
	txtMessage = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
	bSizer100->Add( txtMessage, 1, wxEXPAND|wxALL, 5 );
	
	m_staticText55 = new wxStaticText( this, wxID_ANY, _("Select save games to fix (pop-up menu):"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText55->Wrap( -1 );
	bSizer100->Add( m_staticText55, 0, wxRIGHT|wxLEFT, 5 );
	
	wxArrayString lboxSavesChoices;
	lboxSaves = new wxCheckListBox( this, wxID_LBOX_SAVES, wxDefaultPosition, wxDefaultSize, lboxSavesChoices, 0 );
	bSizer100->Add( lboxSaves, 1, wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND, 5 );
	
	wxBoxSizer* bSizer101;
	bSizer101 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer101->Add( 0, 0, 1, wxEXPAND, 5 );
	
	btnYes = new wxButton( this, wxID_BTN_YES, _("Fix saves"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer101->Add( btnYes, 0, wxALL, 5 );
	
	btnNo = new wxButton( this, wxID_BTN_NO, _("Exit"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer101->Add( btnNo, 0, wxALL, 5 );
	
	
	bSizer100->Add( bSizer101, 0, wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer100 );
	this->Layout();
	
	this->Centre( wxBOTH );
	

	// </wxFormsBuilder> - Section auto-inserted from 'forms.cpp' class 'FormSaveCheck' on 2026-09-06 09:49:45
	// === AUTO GENERATED END ===
	RescaleWindowDPI(this);
		
	// set icon
	wxIcon appIcon;
	appIcon.LoadFile("IDI_ICON2",wxBITMAP_TYPE_ICO_RESOURCE);
	if(appIcon.IsOk())
		SetIcon(appIcon);

	auto op_sz = FromDIP(wxSize(16,16));
	btnOpenCommonFs->SetBitmap(LoadSVGiconsBundle("IDR_OPEN3").GetBitmap(op_sz));
	btnOpenCommonFs->SetBitmapHover(LoadSVGiconsBundle("IDR_OPEN2").GetBitmap(op_sz));
	btnOpenSaveDir->SetBitmap(LoadSVGiconsBundle("IDR_OPEN3").GetBitmap(op_sz));
	btnOpenSaveDir->SetBitmapHover(LoadSVGiconsBundle("IDR_OPEN2").GetBitmap(op_sz));

	// initial check test
	m_common_fs = params.common_fs_path;
	m_saves_dir = params.saves_dir;
	params.result = UpdateCheck();

	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormSaveCheck::OnFixSavesClick,this,wxID_BTN_YES);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormSaveCheck::OnCloseClick,this,wxID_BTN_NO);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormSaveCheck::OnSetCommonPathClick,this,wxID_BTN_OPEN_COMMON_FS);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormSaveCheck::OnSetSavesPathClick,this,wxID_BTN_OPEN_SAVE_DIR);
	lboxSaves->Connect(wxEVT_RIGHT_DOWN,wxMouseEventHandler(FormSaveCheck::OnPupupOpen),NULL,this);
	
	// assign button shortcuts
	std::vector<wxAcceleratorEntry> entries;
	entries.emplace_back(wxACCEL_NORMAL,WXK_RETURN,wxID_BTN_YES);
	entries.emplace_back(wxACCEL_NORMAL,WXK_ESCAPE,wxID_BTN_NO);
	wxAcceleratorTable accel(entries.size(),entries.data());
	this->SetAcceleratorTable(accel);

}

FormSaveCheck::~FormSaveCheck()
{
}

// close window
void FormSaveCheck::OnClose(wxCloseEvent& ev)
{
	
}

// on close form
void FormSaveCheck::OnCloseClick(wxCommandEvent& event)
{	
	EndModal(event.GetId() == wxID_BTN_YES);
}

// on fix saves
void FormSaveCheck::OnFixSavesClick(wxCommandEvent& event)
{
	UpdateCheck(true);
	UpdateCheck(false);
}

// update test
bool FormSaveCheck::UpdateCheck(bool do_fix)
{
	txtCommonFs->SetValue(m_common_fs.wstring());
	txtSaveDir->SetValue(m_saves_dir.wstring());
		
	SpellSave::LoadSaves(m_saves_dir,m_saves);	
	for(auto& save: m_saves)
	{
		auto id = &save - m_saves.data();
		save.do_fix = false;
		if(do_fix && id < lboxSaves->GetCount())
			save.do_fix = lboxSaves->IsChecked(id);		
	}	
	lboxSaves->Freeze();
	lboxSaves->Clear();
	for(auto& save: m_saves)
	{		
		lboxSaves->Append(string_format("%s: %s",save.dir_name.c_str(),wstring2string(save.name).c_str()));
		lboxSaves->Check(&save - m_saves.data(),!do_fix || save.do_fix);
	}
	lboxSaves->Thaw();

	std::string report;
	if(SpellSave::FixSaves(m_saves,m_common_fs,report,!do_fix))
	{
		// something wrong detected								
		auto lines = get_text_lines(report,false);
		for(auto& line: lines)
			line = "  " + line;

		report = string_format("Following inconsistencies between provided save games and COMMON.FS were found:\n\n");
		report += merge_text_lines(lines);
		report += "\nThese inconsistencies likely come from changes in updated mod files or maybe those are saves from unmodified game or another mod.\n";
		report += "Try to fix them (consider making backup before)?";
		txtMessage->SetValue(report);

		lboxSaves->Freeze();
		lboxSaves->Clear();
		for(auto& save: m_saves)
		{
			lboxSaves->Append(string_format("%s: %s",save.dir_name.c_str(),wstring2string(save.name).c_str()));
			lboxSaves->Check(&save - m_saves.data(),!save.is_consistent);
		}
		lboxSaves->Thaw();

		if(do_fix)
			wxMessageBox("Fixing saves inconsistency done.","Checking save game consistency...",wxICON_INFORMATION);

		return(true);
	}
	
	txtMessage->SetValue("No inconsistencies found.");
	return(false);
}



// select common.fs path
void FormSaveCheck::OnSetCommonPathClick(wxCommandEvent& event)
{
	auto dir = std::filesystem::path(m_common_fs).parent_path().wstring();
	auto name = std::filesystem::path(m_common_fs).filename().wstring();
	wxFileDialog openFileDialog(this,"Select reference common.fs archive path",dir,name,"Spellcross COMMON.FS archive|COMMON.FS",
		wxFD_OPEN|wxFD_FILE_MUST_EXIST);
	if(openFileDialog.ShowModal() == wxID_CANCEL)
		return;
	m_common_fs = openFileDialog.GetPath().ToStdWstring();
	
	UpdateCheck();
}

// select saves dir path
void FormSaveCheck::OnSetSavesPathClick(wxCommandEvent& event)
{
	auto dir = std::filesystem::path(m_saves_dir).wstring();	
	wxDirDialog openDirDialog(this,"Select saves directory",dir,wxDD_DIR_MUST_EXIST);
	if(openDirDialog.ShowModal() == wxID_CANCEL)
		return;
	m_saves_dir = openDirDialog.GetPath().ToStdWstring();
	
	UpdateCheck();
}



enum class PopupActions : int{
	ALL,
	CLEAR
};
// show popup menu on probab list
void FormSaveCheck::OnPupupOpen(wxMouseEvent& event)
{
	wxMenu menu;
	menu.Append((int)PopupActions::ALL,"Select all");
	menu.Append((int)PopupActions::CLEAR,"Clear all");
	menu.Connect(wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(FormSaveCheck::OnPupup),NULL,this);
	PopupMenu(&menu);
}
void FormSaveCheck::OnPupup(wxCommandEvent& event)
{
	auto menu_id = (PopupActions)event.GetId();
	auto menu = (wxMenu*)event.GetEventObject();
	if(!menu)
		return;

	if(menu_id == PopupActions::ALL)
	{
		for(int k = 0; k < lboxSaves->GetCount(); k++)
			lboxSaves->Check(k,true);
	}
	else if(menu_id == PopupActions::CLEAR)
	{
		for(int k = 0; k < lboxSaves->GetCount(); k++)
			lboxSaves->Check(k,false);
	}	
}
