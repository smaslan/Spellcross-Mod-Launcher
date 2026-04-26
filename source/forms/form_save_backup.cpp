///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 4.2.1-0-g80c4cb6)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include <wx/filedlg.h>
#include <wx/msgdlg.h>

#include "form_save_backup.h"
#include "../other.h"
#include "../wx_other.h"
#include "../SpellMod.h"

///////////////////////////////////////////////////////////////////////////


FormSaveBack::FormSaveBack(wxWindow* parent,wxWindowID id,const wxString& title,const wxPoint& pos,const wxSize& size,long style) : wxFrame(parent,id,title,pos,size,style)
{
	this->SetSizeHints(wxDefaultSize,wxDefaultSize);
	this->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_MENU));

	sbar = this->CreateStatusBar(1,wxSTB_SIZEGRIP,wxID_SBAR);
	m_menubar2 = new wxMenuBar(0);
	m_menu6 = new wxMenu();
	wxMenuItem* mmExit;
	mmExit = new wxMenuItem(m_menu6,wxID_MM_EXIT,wxString(_("Exit")),wxEmptyString,wxITEM_NORMAL);
	m_menu6->Append(mmExit);

	m_menubar2->Append(m_menu6,_("File"));

	this->SetMenuBar(m_menubar2);

	wxBoxSizer* bSizer19;
	bSizer19 = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* bSizer49;
	bSizer49 = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* bSizer37;
	bSizer37 = new wxBoxSizer(wxVERTICAL);

	m_staticText15 = new wxStaticText(this,wxID_ANY,_("Save target type:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText15->Wrap(-1);
	bSizer37->Add(m_staticText15,0,wxTOP|wxRIGHT|wxLEFT,5);

	wxArrayString chSourceDestChoices;
	chSourceDest = new wxChoice(this,wxID_CH_SRC_DEST,wxDefaultPosition,wxDefaultSize,chSourceDestChoices,0);
	chSourceDest->SetSelection(0);
	bSizer37->Add(chSourceDest,0,wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND,5);


	bSizer49->Add(bSizer37,1,wxEXPAND,5);

	wxBoxSizer* bSizer372;
	bSizer372 = new wxBoxSizer(wxVERTICAL);

	m_staticText152 = new wxStaticText(this,wxID_ANY,_("Save operation:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText152->Wrap(-1);
	bSizer372->Add(m_staticText152,0,wxTOP|wxRIGHT|wxLEFT,5);

	wxArrayString chOperationChoices;
	chOperation = new wxChoice(this,wxID_CH_OPER,wxDefaultPosition,wxDefaultSize,chOperationChoices,0);
	chOperation->SetSelection(0);
	bSizer372->Add(chOperation,0,wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND,5);


	bSizer49->Add(bSizer372,1,wxEXPAND,5);


	bSizer19->Add(bSizer49,0,wxEXPAND,5);

	wxBoxSizer* bSizer371;
	bSizer371 = new wxBoxSizer(wxVERTICAL);

	m_staticText151 = new wxStaticText(this,wxID_ANY,_("Mod path (if applicable):"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText151->Wrap(-1);
	bSizer371->Add(m_staticText151,0,wxRIGHT|wxLEFT,5);

	wxBoxSizer* bSizer53;
	bSizer53 = new wxBoxSizer(wxHORIZONTAL);

	txtModPath = new wxTextCtrl(this,wxID_TXT_MOD_PATH,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_READONLY);
	bSizer53->Add(txtModPath,1,wxBOTTOM|wxRIGHT|wxLEFT,5);

	btnModPath = new wxBitmapButton(this,wxID_BTN_MOD_PATH,wxNullBitmap,wxDefaultPosition,wxDefaultSize,wxBU_AUTODRAW|wxBORDER_NONE);
	bSizer53->Add(btnModPath,0,wxRIGHT|wxLEFT,5);


	bSizer371->Add(bSizer53,1,wxEXPAND,5);


	bSizer19->Add(bSizer371,0,wxEXPAND,5);

	wxBoxSizer* bSizer42;
	bSizer42 = new wxBoxSizer(wxVERTICAL);

	lblSource = new wxStaticText(this,wxID_LBL_SRC,_("Source path:"),wxDefaultPosition,wxDefaultSize,0);
	lblSource->Wrap(-1);
	bSizer42->Add(lblSource,0,wxRIGHT|wxLEFT,5);

	txtSource = new wxTextCtrl(this,wxID_TXT_SRC,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_READONLY);
	bSizer42->Add(txtSource,0,wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND,5);


	bSizer19->Add(bSizer42,0,wxEXPAND,5);

	wxBoxSizer* bSizer421;
	bSizer421 = new wxBoxSizer(wxVERTICAL);

	lblDest = new wxStaticText(this,wxID_LBL_DEST,_("Destination path:"),wxDefaultPosition,wxDefaultSize,0);
	lblDest->Wrap(-1);
	bSizer421->Add(lblDest,0,wxRIGHT|wxLEFT,5);

	txtDest = new wxTextCtrl(this,wxID_TXT_DEST,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_READONLY);
	bSizer421->Add(txtDest,0,wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND,5);


	bSizer19->Add(bSizer421,0,wxEXPAND,5);

	wxBoxSizer* bSizer46;
	bSizer46 = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* bSizer47;
	bSizer47 = new wxBoxSizer(wxVERTICAL);

	m_staticText22 = new wxStaticText(this,wxID_ANY,_("Existing save sets:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText22->Wrap(-1);
	bSizer47->Add(m_staticText22,0,wxRIGHT|wxLEFT,5);

	lbList = new wxListBox(this,wxID_LB_LIST,wxDefaultPosition,wxDefaultSize,0,NULL,wxLB_ALWAYS_SB);
	bSizer47->Add(lbList,1,wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND,5);


	bSizer46->Add(bSizer47,1,wxEXPAND,5);

	m_staticline8 = new wxStaticLine(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLI_VERTICAL);
	bSizer46->Add(m_staticline8,0,wxEXPAND|wxTOP|wxBOTTOM,5);

	wxBoxSizer* bSizer48;
	bSizer48 = new wxBoxSizer(wxVERTICAL);

	m_staticText24 = new wxStaticText(this,wxID_ANY,_("Name:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText24->Wrap(-1);
	bSizer48->Add(m_staticText24,0,wxRIGHT|wxLEFT,5);

	txtName = new wxTextCtrl(this,wxID_TXT_NAME,wxEmptyString,wxDefaultPosition,wxDefaultSize,0);
	bSizer48->Add(txtName,0,wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND,5);

	m_staticText23 = new wxStaticText(this,wxID_ANY,_("Date:"),wxDefaultPosition,wxDefaultSize,0);
	m_staticText23->Wrap(-1);
	bSizer48->Add(m_staticText23,0,wxRIGHT|wxLEFT,5);

	txtDate = new wxTextCtrl(this,wxID_TXT_DATE,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_READONLY);
	bSizer48->Add(txtDate,0,wxBOTTOM|wxRIGHT|wxLEFT|wxEXPAND,5);

	lblDesc = new wxStaticText(this,wxID_LBL_DESC,_("Description:"),wxDefaultPosition,wxDefaultSize,0);
	lblDesc->Wrap(-1);
	bSizer48->Add(lblDesc,0,wxRIGHT|wxLEFT,5);

	txtDescription = new wxTextCtrl(this,wxID_TXT_DESCRIPTION,wxEmptyString,wxDefaultPosition,wxDefaultSize,wxTE_MULTILINE);
	bSizer48->Add(txtDescription,1,wxEXPAND|wxBOTTOM|wxRIGHT|wxLEFT,5);


	bSizer46->Add(bSizer48,1,wxEXPAND,5);


	bSizer19->Add(bSizer46,1,wxEXPAND,5);

	m_staticline9 = new wxStaticLine(this,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxLI_HORIZONTAL);
	bSizer19->Add(m_staticline9,0,wxEXPAND|wxRIGHT|wxLEFT,5);

	wxBoxSizer* bSizer20;
	bSizer20 = new wxBoxSizer(wxHORIZONTAL);

	bSizer20->SetMinSize(wxSize(-1,45));

	bSizer20->Add(0,0,1,wxEXPAND,5);

	btnRemove = new wxButton(this,wxID_BTN_REMOVE,_("REMOVE"),wxDefaultPosition,wxDefaultSize,0);
	btnRemove->SetMinSize(wxSize(120,-1));

	bSizer20->Add(btnRemove,0,wxALL|wxEXPAND,5);

	btnConfirm = new wxButton(this,wxID_BTN_OK,_("OK"),wxDefaultPosition,wxDefaultSize,0);
	btnConfirm->SetMinSize(wxSize(120,-1));

	bSizer20->Add(btnConfirm,0,wxALL|wxEXPAND,5);

	btnExit = new wxButton(this,wxID_BTN_EXIT,_("EXIT"),wxDefaultPosition,wxDefaultSize,0);
	btnExit->SetMinSize(wxSize(120,-1));

	bSizer20->Add(btnExit,0,wxALL|wxEXPAND,5);


	bSizer19->Add(bSizer20,0,wxEXPAND,5);


	this->SetSizer(bSizer19);
	this->Layout();

	this->Centre(wxBOTH);

	// === AUTO GENERATED END ===
	RescaleWindowDPI(this);
		
	m_spell_path = "";
	m_backup_dir = "";
	
	// set icon
	wxIcon appIcon;
	appIcon.LoadFile("IDI_ICON2",wxBITMAP_TYPE_ICO_RESOURCE);
	if(appIcon.IsOk())
		SetIcon(appIcon);

	mmExit->SetBitmaps(LoadSVGiconsBundle("IDR_EXIT"));
	/*mmOpen->SetBitmaps(LoadSVGiconsBundle("IDR_OPEN"));
	mmSave->SetBitmaps(LoadSVGiconsBundle("IDR_SAVE"));*/

	auto op_sz = FromDIP(wxSize(16,16));	
	btnExit->SetBitmap(LoadSVGiconsBundle("IDR_EXIT").GetBitmap(op_sz), wxLEFT);
	btnExit->SetBitmapMargins(10,0);
	btnRemove->SetBitmap(LoadSVGiconsBundle("IDR_CLOSE").GetBitmap(op_sz),wxLEFT);
	btnRemove->SetBitmapMargins(10,0);		
	btnModPath->SetBitmap(LoadSVGiconsBundle("IDR_OPEN3").GetBitmap(op_sz));
	btnModPath->SetBitmapHover(LoadSVGiconsBundle("IDR_OPEN2").GetBitmap(op_sz));

	Bind(wxEVT_COMMAND_MENU_SELECTED,&FormSaveBack::OnCloseClick,this,wxID_MM_EXIT);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormSaveBack::OnCloseClick,this,wxID_BTN_EXIT);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormSaveBack::OnOK,this,wxID_BTN_OK);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormSaveBack::OnRemove,this,wxID_BTN_REMOVE);
	Bind(wxEVT_COMMAND_BUTTON_CLICKED,&FormSaveBack::OnSetModPath,this,wxID_BTN_MOD_PATH);
	Bind(wxEVT_COMMAND_CHOICE_SELECTED,&FormSaveBack::OnChangeSet,this,wxID_CH_SRC_DEST);
	Bind(wxEVT_COMMAND_CHOICE_SELECTED,&FormSaveBack::OnChangeSet,this,wxID_CH_OPER);	
	Bind(wxEVT_COMMAND_LISTBOX_SELECTED,&FormSaveBack::OnChangeBackup,this,wxID_LB_LIST);
	Bind(wxEVT_COMMAND_TEXT_UPDATED,&FormSaveBack::OnChangeBackupName,this,wxID_TXT_NAME);

	Bind(wxEVT_CLOSE_WINDOW,&FormSaveBack::OnClose,this,wxID_ANY);
	

	chOperation->Append(str_backup);
	chOperation->Append(str_restore);
	chOperation->Select(0);

	chSourceDest->Append(str_orig_saves);
	chSourceDest->Append(str_mod_saves);
	chSourceDest->Select(0);

	// assign button shortcuts
	std::vector<wxAcceleratorEntry> entries;
	//entries.emplace_back(wxACCEL_NORMAL,WXK_RETURN,wxID_BTN_OK);
	entries.emplace_back(wxACCEL_NORMAL,WXK_ESCAPE,wxID_BTN_EXIT);
	wxAcceleratorTable accel(entries.size(),entries.data());
	this->SetAcceleratorTable(accel);

	const int sizes[] = {-1, 150};
	sbar->SetFieldsCount(2,sizes);
}

FormSaveBack::~FormSaveBack()
{
}

// set spellcross path
void FormSaveBack::SetSpellPath(std::filesystem::path path)
{
	m_spell_path = path;
	OnUpdateList();
}
// set mod path
void FormSaveBack::SetModPath(std::filesystem::path path)
{
	txtModPath->SetValue(path.wstring());
	OnUpdateList();
}
// set mod/orig switch
void FormSaveBack::SetMod(bool is_mod)
{
	chSourceDest->SetStringSelection((is_mod)?str_mod_saves:str_orig_saves);
	OnUpdateList();
}
// set backup/restore
void FormSaveBack::SetBackup(bool is_backup)
{
	chOperation->SetStringSelection((is_backup)?str_backup:str_restore);
	OnUpdateList();
}

// close window
void FormSaveBack::OnClose(wxCloseEvent& ev)
{	
	ev.Skip();
}

// on close form
void FormSaveBack::OnCloseClick(wxCommandEvent& event)
{	
	Close();
}



// browse mod path
void FormSaveBack::OnSetModPath(wxCommandEvent& event)
{
	std::filesystem::path mod_path = txtModPath->GetValue().ToStdWstring();
	auto dir = mod_path.parent_path().wstring();
	auto name = mod_path.filename().wstring();
	wxFileDialog openFileDialog(this,"Select mod DEF path",dir,name,"Spellcross mod definition file (*.def)|*.def",
		wxFD_OPEN|wxFD_FILE_MUST_EXIST);
	if(openFileDialog.ShowModal() == wxID_CANCEL)
		return;
	txtModPath->SetValue(openFileDialog.GetPath());
	OnUpdateList();
}

// change operation/source setting
void FormSaveBack::OnChangeSet(wxCommandEvent& event)
{
	OnUpdateList();	
}

// change backup selection
void FormSaveBack::OnChangeBackup(wxCommandEvent& event)
{	
	txtName->SetValue(lbList->GetStringSelection());
	
	auto backup_name = txtName->GetValue().ToStdWstring();
	auto backup_path = m_backup_dir / backup_name;
	if(backup_name.empty())
		backup_path = "";

	SpellMod::SaveInfo info;
	SpellMod::GetSaveIni(backup_path, info);
	
	txtDescription->SetValue(info.description);
	txtDate->SetValue(info.time);
}

// change backup name
void FormSaveBack::OnChangeBackupName(wxCommandEvent& event)
{
	auto backup_name = txtName->GetValue().ToStdWstring();
	auto backup_path = m_backup_dir / backup_name;
	if(backup_name.empty())
		backup_path = "";
	txtSource->SetValue(backup_path.wstring());
}

// on OK
void FormSaveBack::OnOK(wxCommandEvent& event)
{
	bool is_mod = chSourceDest->GetStringSelection() == str_mod_saves;
	bool is_backup = chOperation->GetStringSelection() == str_backup;

	std::filesystem::path save_path = txtDest->GetValue().ToStdWstring();
	auto backup_path = std::filesystem::path(txtSource->GetValue().ToStdWstring());	
	auto backup_name = txtName->GetValue().ToStdString();
	
	if(save_path.empty())
	{
		wxMessageDialog dial(this,string_format("No save game path defined!"),_("Save backup ..."),wxICON_ERROR);
		dial.ShowModal();
		return;
	}
	if(backup_name.empty())
	{
		wxMessageDialog dial(this,string_format("No backup name defined!"),_("Save backup ..."),wxICON_ERROR);
		dial.ShowModal();
		return;
	}

	if(is_backup)
	{
		// make root backups dir if not there yet
		auto backup_dir = backup_path.parent_path();
		std::filesystem::create_directories(backup_dir);

		// check overwrite
		std::vector<std::string> names;
		fs_list_dir(backup_path,"*",true,true,&names);
		if(!names.empty())
		{
			// backup exist, ask for proceed
			auto info = string_format("Backup name \"%s\" already exist at path:\n",backup_name.c_str());
			info += string_format("  %ls\n",backup_path.wstring().c_str());
			info += string_format("Following files/sub-directories will be removed before beckup:\n");
			for(auto& item: names)
				info += string_format("  %s\n",item.c_str());
			info += string_format("\nProceed with backup to this location?");
			wxMessageDialog dial(this,info,_("Save backup ..."),wxYES_NO | wxNO_DEFAULT | wxICON_EXCLAMATION);
			if(dial.ShowModal() != wxID_YES)
				return;
			// confirmed, try remove old backup
			if(fs_remove(backup_path,true))
			{
				// failed
				wxMessageDialog dial(this,string_format("Cannot remove backup save directory:\n  %ls\nPossibly sharing violation. Maybe you are currently viewing the save folder?",backup_path.wstring().c_str()),"Backup of savegame ...",wxICON_ERROR);
				dial.ShowModal();
				return;
			}
		}

		// make new backup
		fs_copy(save_path, backup_path, std::filesystem::copy_options::recursive);

		// make/update save ini
		auto desc = txtDescription->GetValue();
		SpellMod::MakeSaveIni(backup_path, desc.ToStdString());
		
		//  refresh list
		OnUpdateList(backup_name);
	}
}

// on remove backup
void FormSaveBack::OnRemove(wxCommandEvent& event)
{
	//bool is_mod = chSourceDest->GetStringSelection() == str_mod_saves;
	//bool is_backup = chOperation->GetStringSelection() == str_backup;

	auto backup_path = std::filesystem::path(txtSource->GetValue().ToStdWstring());
	auto backup_name = txtName->GetValue().ToStdString();
	if(backup_name.empty())
	{
		wxMessageDialog dial(this,string_format("No backup name defined!"),_("Remove save backup ..."),wxICON_ERROR);
		dial.ShowModal();
		return;
	}
	if(!std::filesystem::exists(backup_path))
	{
		wxMessageDialog dial(this,string_format("Backup \"%s\" at path:\n  %ls\ndoes not exist!",backup_name.c_str(),backup_path.wstring().c_str()),_("Remove save backup ..."),wxICON_ERROR);
		dial.ShowModal();
		return;
	}

	// check exit
	std::vector<std::string> names;
	fs_list_dir(backup_path,"*",true,true,&names);
	
	// backup exist, ask for proceed
	auto info = string_format("Backup name \"%s\" at path:\n",backup_name.c_str());
	info += string_format("  %ls\n",backup_path.wstring().c_str());
	if(!names.empty())
	{
		info += string_format("with its content:\n");
		for(auto& item: names)
			info += string_format("  %s\n",item.c_str());
	}
	info += string_format("will be removed.\nProceed with removal?");
	wxMessageDialog dial(this,info,_("Remove save backup ..."),wxYES_NO | wxNO_DEFAULT | wxICON_EXCLAMATION);
	if(dial.ShowModal() != wxID_YES)
		return;

	// confirmed, try remove old backup
	if(fs_remove(backup_path,true))
	{
		// failed
		wxMessageDialog dial(this,string_format("Cannot remove backup save directory:\n  %ls\nPossibly sharing violation. Maybe you are currently viewing the save folder?",backup_path.wstring().c_str()),"Remove save backup ...",wxICON_ERROR);
		dial.ShowModal();
		return;
	}
	
	//  refresh list
	OnUpdateList(backup_name);	
}

// update saves list
void FormSaveBack::OnUpdateList(std::string backup_to_select)
{
	bool is_mod = chSourceDest->GetStringSelection() == str_mod_saves;
	bool is_backup = chOperation->GetStringSelection() == str_backup;

	if(is_backup)
	{
		btnConfirm->SetLabel("BACKUP");
		auto op_sz = FromDIP(wxSize(16,16));
		btnConfirm->SetBitmap(LoadSVGiconsBundle("IDR_SAVE").GetBitmap(op_sz), wxLEFT);
		btnConfirm->SetBitmapMargins(10,0);
	}
	else
	{
		btnConfirm->SetLabel("RESTORE");
		auto op_sz = FromDIP(wxSize(16,16));
		btnConfirm->SetBitmap(LoadSVGiconsBundle("IDR_RELOAD").GetBitmap(op_sz),wxLEFT);
		btnConfirm->SetBitmapMargins(10,0);
	}

	// save directories root
	std::filesystem::path save_root = m_spell_path;
	if(is_mod)
		save_root = std::filesystem::path(txtModPath->GetValue().ToStdWstring()).parent_path();
	
	// directory with save backups
	m_backup_dir = save_root / str_backup_subdir;
	if(save_root.empty())
		m_backup_dir = "";

	// target save location
	auto save_dir = save_root / "save";
	if(save_root.empty())
		save_dir = "";

	// get list of existing backups
	std::vector<std::string> backup_names;
	std::vector<std::filesystem::path> backup_paths;
	fs_list_dir(m_backup_dir, "*", false, true, &backup_names, &backup_paths);
	
	// fill backup list
	auto last_sel = lbList->GetStringSelection();
	lbList->Freeze();
	lbList->Clear();
	for(auto &item: backup_names)
		lbList->Append(item);
	lbList->Thaw();
	if(!backup_to_select.empty())
		last_sel = backup_to_select;
	if(!lbList->SetStringSelection(last_sel) && lbList->GetCount())
		lbList->Select(0);

	// target path info
	if(is_backup)
		lblDest->SetLabel("Source SAVE path from which to make backup:");
	else
		lblDest->SetLabel("Destination SAVE path to which restore selected backup:");
	txtDest->SetValue(save_dir.wstring());

	// backup path info
	if(is_backup)
		lblSource->SetLabel("Backup SAVE path to which backup will be saved:");
	else
		lblSource->SetLabel("Backup SAVE path from which backup will be restored:");
	txtSource->Clear();

	


	wxCommandEvent event;
	OnChangeBackup(event);
}