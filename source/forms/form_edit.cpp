///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include <wx/filedlg.h>
#include <wx/msgdlg.h>

#include "form_edit.h"
#include "../other.h"
#include "../wx_other.h"

///////////////////////////////////////////////////////////////////////////


FormEdit::FormEdit(wxWindow* parent,wxWindowID id,const wxString& title,const wxPoint& pos,const wxSize& size,long style) : wxFrame(parent,id,title,pos,size,style)
{
	this->SetSizeHints(wxDefaultSize,wxDefaultSize);
	this->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_MENU));

	sbar = this->CreateStatusBar(1,wxSTB_SIZEGRIP,wxID_SBAR);
	m_menubar2 = new wxMenuBar(0);
	m_menu6 = new wxMenu();
	wxMenuItem* mmOpen;
	mmOpen = new wxMenuItem(m_menu6,wxID_MM_OPEN,wxString(_("Open")),wxEmptyString,wxITEM_NORMAL);
	m_menu6->Append(mmOpen);

	wxMenuItem* mmSave;
	mmSave = new wxMenuItem(m_menu6,wxID_MM_SAVE,wxString(_("Save")),wxEmptyString,wxITEM_NORMAL);
	m_menu6->Append(mmSave);

	m_menu6->AppendSeparator();

	wxMenuItem* mmExit;
	mmExit = new wxMenuItem(m_menu6,wxID_MM_EXIT,wxString(_("Exit")),wxEmptyString,wxITEM_NORMAL);
	m_menu6->Append(mmExit);

	m_menubar2->Append(m_menu6,_("File"));

	this->SetMenuBar(m_menubar2);

	wxBoxSizer* bSizer19;
	bSizer19 = new wxBoxSizer(wxVERTICAL);

	text = new wxTextCtrl(this,wxID_TEXT,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_MULTILINE);
	bSizer19->Add(text,1,wxEXPAND|wxTOP|wxRIGHT|wxLEFT,5);

	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer(wxHORIZONTAL);


	bSizer20->Add(0,0,1,wxEXPAND,5);

	btnSave = new wxButton(this,wxID_BTN_SAVE,_("Save"),wxDefaultPosition,wxDefaultSize,0);
	btnSave->SetMinSize(wxSize(-1,30));

	bSizer20->Add(btnSave,0,wxALL,5);

	btnExit = new wxButton(this,wxID_BTN_EXIT,_("Exit"),wxDefaultPosition,wxDefaultSize,0);
	btnExit->SetMinSize(wxSize(-1,30));

	bSizer20->Add(btnExit,0,wxALL,5);


	bSizer19->Add(bSizer20,0,wxEXPAND,5);


	this->SetSizer(bSizer19);
	this->Layout();

	this->Centre(wxBOTH);

	// === AUTO GENERATED END ===
	m_modified = false;
	m_path = "";

	mmOpen->SetBitmaps(LoadSVGiconsBundle("IDR_OPEN"));
	mmSave->SetBitmaps(LoadSVGiconsBundle("IDR_SAVE"));
	mmExit->SetBitmaps(LoadSVGiconsBundle("IDR_EXIT"));

	Bind(wxEVT_COMMAND_MENU_SELECTED,&FormEdit::OnCloseClick,this,wxID_MM_EXIT);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormEdit::OnCloseClick,this,wxID_BTN_EXIT);
	Bind(wxEVT_COMMAND_MENU_SELECTED,&FormEdit::OnSave,this,wxID_MM_SAVE);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormEdit::OnSave,this,wxID_BTN_SAVE);
	Bind(wxEVT_COMMAND_MENU_SELECTED,&FormEdit::OnOpen,this,wxID_MM_OPEN);

	Bind(wxEVT_COMMAND_TEXT_UPDATED,&FormEdit::OnEdit,this,wxID_TEXT);

	Bind(wxEVT_CLOSE_WINDOW,&FormEdit::OnClose,this,wxID_ANY);
	

	// assign button shortcuts
	std::vector<wxAcceleratorEntry> entries;
	//entries.emplace_back(wxACCEL_NORMAL,WXK_RETURN,wxID_BTN_OK);
	entries.emplace_back(wxACCEL_NORMAL,WXK_ESCAPE,wxID_BTN_EXIT);
	wxAcceleratorTable accel(entries.size(),entries.data());
	this->SetAcceleratorTable(accel);

	const int sizes[] = {-1, 150};
	sbar->SetFieldsCount(2,sizes);
}

FormEdit::~FormEdit()
{
}

// set initial file path
void FormEdit::SetPath(std::filesystem::path path)
{
	m_path = path;
	if(!m_path.empty())
	{
		std::string buf;
		loadstr(m_path,buf);
		text->SetValue(buf);
		m_modified = false;
		sbar->SetStatusText("",1);
		sbar->SetStatusText(m_path.wstring(),0);
	}
}

// close window
void FormEdit::OnClose(wxCloseEvent& ev)
{
	if(m_modified)
	{
		wxMessageDialog dial(this,string_format("File was modified! Do you want to save it first?"),_("Open file..."),wxYES_NO | wxICON_QUESTION);
		if(dial.ShowModal() == wxID_YES)
		{
			wxCommandEvent event;
			OnSave(event);
		}
	}
	ev.Skip();
}

// on close form
void FormEdit::OnCloseClick(wxCommandEvent& event)
{	
	Close();
}

// on save file
void FormEdit::OnSave(wxCommandEvent& event)
{
	std::wstring dir = L"";
	std::wstring name = L"";
	if(!m_path.empty())
	{
		dir = m_path.parent_path().wstring();
		name = m_path.filename().wstring();
	}

	wxFileDialog openFileDialog(this,"Open file",dir,name,"Any text file (*.*)|*.*",
		wxFD_OPEN|wxFD_FILE_MUST_EXIST);
	if(openFileDialog.ShowModal() == wxID_CANCEL)
		return;
	std::filesystem::path path = std::filesystem::path(openFileDialog.GetPath().ToStdWstring());
	SetPath(path);
	
	sbar->SetStatusText("",1);
	m_modified = false;
}

// on open file
void FormEdit::OnOpen(wxCommandEvent& event)
{
	if(m_modified)
	{
		wxMessageDialog dial(this,string_format("File was modified! Do you want to save it first?"),_("Open file..."),wxYES_NO | wxICON_QUESTION);
		if(dial.ShowModal() == wxID_YES)
			OnSave(event);		
	}
	std::wstring dir = L"";
	if(!m_path.empty())
		dir = m_path.parent_path().wstring();

	wxFileDialog openFileDialog(this,"Open file",dir,"","Any text file (*.*)|*.*",
		wxFD_OPEN|wxFD_FILE_MUST_EXIST);
	if(openFileDialog.ShowModal() == wxID_CANCEL)
		return;
	std::filesystem::path path = std::filesystem::path(openFileDialog.GetPath().ToStdWstring());
	SetPath(path);
}

// on edit text
void FormEdit::OnEdit(wxCommandEvent& event)
{
	m_modified = true;
	sbar->SetStatusText("Modified",1);
}