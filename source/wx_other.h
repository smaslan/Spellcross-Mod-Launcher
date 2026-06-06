#pragma once

#include <wx/listctrl.h>
#include <wx/treectrl.h>
#include <wx/dialog.h>
#include <wx/propgrid/advprops.h>

#include <filesystem>
#include <map>

class wxListCtrlVirtual : public wxListCtrl {
private:
	std::function<wxString(long item)> m_get_item_text_cb;
	std::function<wxListItemAttr*(long item)> m_get_item_attr_cb;
	std::function<int(long item)> m_get_item_image_cb;
	wxString OnGetItemText(long item,long column) const { if(m_get_item_text_cb) return(m_get_item_text_cb(item)); return("");};
	wxListItemAttr* OnGetItemAttr(long item) const { if(m_get_item_attr_cb) return(m_get_item_attr_cb(item)); return(NULL); };
	int OnGetItemImage(long item) const { if(m_get_item_image_cb) return(m_get_item_image_cb(item)); return(-1);};
public:
	wxListCtrlVirtual(wxWindow* parent,wxWindowID id,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize,long style=wxLC_ICON,const wxValidator& validator=wxDefaultValidator,const wxString& name=wxListCtrlNameStr)
		: wxListCtrl(parent,id,pos,size,style,validator,name) {};
	//virtual ~wxListCtrlSprite() = default;

	// set callback function for wxListCtrl->GetItemText() callback
	void SetGetItemTextCb(std::function<wxString(long item)> cb) { m_get_item_text_cb = cb;};
	// set callback function for wxListCtrl->GetItemAttr() callback
	void SetGetItemAttrCb(std::function<wxListItemAttr* (long item)> cb) { m_get_item_attr_cb = cb; };
	// set callback function for wxListCtrl->GetItemImage() callback
	void SetGetItemImageCb(std::function<int(long item)> cb) { m_get_item_image_cb = cb; };
};


// use to store wxTreeCtrl item states, and restore them after new fillup
class wxTreeLister{
public:

	wxTreeLister() {};
	wxTreeLister(wxTreeCtrl* ctrl,wxTreeItemId id=wxTreeItemId(),std::string root_str="",int level=0) { treeCtrlRecStates(ctrl,id,root_str,level); };
	~wxTreeLister() {};

	class TreeCtrlState {
	public:
		int level;
		bool state;
		bool sel;
		std::string name;
	};
	std::vector<TreeCtrlState> list;	

	// build recoursive list of tree elements with names, level and expand states
	void treeCtrlRecStates(wxTreeCtrl* ctrl,wxTreeItemId id=wxTreeItemId(),std::string root_str="",int level=0)
	{
		if(!id.IsOk() && level==0)
			id = ctrl->GetRootItem();
		if(!id.IsOk())
			return;
		wxTreeItemIdValue cookie;
		auto node_id = ctrl->GetFirstChild(id,cookie);
		while(node_id.IsOk())
		{
			auto new_root_str = root_str + "->" + ctrl->GetItemText(node_id).ToStdString();
			TreeCtrlState state ={level, ctrl->IsExpanded(node_id), ctrl->IsSelected(node_id), new_root_str};
			list.push_back(state);
			treeCtrlRecStates(ctrl,node_id,new_root_str,level+1);
			node_id = ctrl->GetNextChild(id,cookie);
		}
	}

	// try set expand state based on recoursive list of previous states
	void treeCtrlSetStates(wxTreeCtrl* ctrl,wxTreeItemId id=wxTreeItemId(),std::string root_str="",int level=0)
	{
		if(!id.IsOk() && level==0)
			id = ctrl->GetRootItem();
		if(!id.IsOk())
			return;
		wxTreeItemIdValue cookie;
		auto node_id = ctrl->GetFirstChild(id,cookie);
		while(node_id.IsOk())
		{
			auto new_root_str = root_str + "->" + ctrl->GetItemText(node_id).ToStdString();
			for(auto& item: list)
				if(item.level == level && item.name.compare(new_root_str) == 0)
				{
					if(item.state)
						ctrl->Expand(node_id);
					if(item.sel)
						ctrl->SelectItem(node_id);
					break;
				}
			treeCtrlSetStates(ctrl,node_id,new_root_str,level+1);
			node_id = ctrl->GetNextChild(id,cookie);
		}
		if(level)
			return;
		auto sel_id = ctrl->GetSelection();
		if(sel_id.IsOk() && !ctrl->IsVisible(sel_id))
			ctrl->EnsureVisible(sel_id);
	}
};

// PropGrid client object holding data type and pointer to linked variable
class wxPGobj : public wxClientData
{
public:
	enum class DataType
	{
		UINT,
		INT,
		STRING,
		WSTRING,
		ENUM,
		ARRAY
	};

	DataType m_type;
	void* m_data;

	wxPGobj(DataType type,void* data) :
		m_type(type),m_data(data) {};

	bool Update(wxPGProperty *prop)
	{
		if(m_type == wxPGobj::DataType::INT)
			*(int*)m_data = prop->GetValue().GetLong();
		else if(m_type == wxPGobj::DataType::UINT)
			*(unsigned int*)m_data = prop->GetValue().GetLong();
		else if(m_type == wxPGobj::DataType::STRING)
			*(std::string*)m_data = prop->GetValue().GetString();
		else if(m_type == wxPGobj::DataType::WSTRING)
			*(std::wstring*)m_data = prop->GetValue().GetString();
		else if(m_type == wxPGobj::DataType::ENUM)
			*(int*)m_data = prop->GetValue().GetLong();
		else if(m_type == wxPGobj::DataType::ARRAY)
		{
			auto data = (std::vector<int>*)m_data;
			data->clear();
			auto selections = prop->GetValue().GetArrayString();
			auto ch = prop->GetChoices().GetLabels();
			for(auto &sel: selections)
			{
				auto sel_id = std::find(ch.begin(), ch.end(), sel);
				if(sel_id == ch.end())
					continue;
				data->push_back(sel_id - ch.begin());
			}				
		}
		else
			return(false);
		return(true);
	};
};

// PropGrid integer property using pointer to linked variable
class wxIntPropertyExt : public wxIntProperty
{
public:
	wxIntPropertyExt(const wxString& label,const wxString& name,int* value)
		: wxIntProperty(label,name,*value)
	{
		SetClientObject(new wxPGobj(wxPGobj::DataType::INT,value));
	}
};

// PropGrid unsigned integer property using pointer to linked variable
class wxUIntPropertyExt : public wxUIntProperty
{
public:
	wxUIntPropertyExt(const wxString& label,const wxString& name,unsigned int* value)
		: wxUIntProperty(label,name,*value)
	{
		SetClientObject(new wxPGobj(wxPGobj::DataType::UINT,value));
	}
};

// PropGrid string property using pointer to linked variable
class wxStringPropertyExt : public wxStringProperty
{
public:
	wxStringPropertyExt(const wxString& label,const wxString& name,std::string *value,int max_len=-1)
		: wxStringProperty(label,name,*value)
	{
		if(max_len)
			SetMaxLength(max_len);
		SetClientObject(new wxPGobj(wxPGobj::DataType::STRING,value));
	};
	wxStringPropertyExt(const wxString& label,const wxString& name,std::wstring* value,int max_len=-1)
		: wxStringProperty(label,name,*value)
	{
		if(max_len)
			SetMaxLength(max_len);
		SetClientObject(new wxPGobj(wxPGobj::DataType::WSTRING,value));
	};
};

// PropGrid unsigned integer property using pointer to linked variable
class wxEnumPropertyExt : public wxEnumProperty
{
public:
	wxEnumPropertyExt(const wxString& label,const wxString& name,wxPGChoices &choices, int *value)
		: wxEnumProperty(label,name,choices,*value)
	{
		SetClientObject(new wxPGobj(wxPGobj::DataType::ENUM,value));
	}
};

// PropGrid multi-choice strings with selection indices vector using linker variable
class wxMultiChoicePropertyExt : public wxMultiChoiceProperty
{
private:
	wxArrayString GenSelections(wxPGChoices& choices, std::vector<int> *values)
	{		
		auto ch_list = choices.GetLabels();
		wxArrayString list;
		for(auto id: *values)
			list.push_back(ch_list[id]);
		return(list);
	};
public:
	wxMultiChoicePropertyExt(const wxString& label,const wxString& name,wxPGChoices& choices,std::vector<int> *value)
		: wxMultiChoiceProperty(label,name,choices,GenSelections(choices,value))
	{
		SetClientObject(new wxPGobj(wxPGobj::DataType::ARRAY,value));
	};
};



std::filesystem::path GetExecutableDir();
wxBitmapBundle LoadSVGiconsBundle(const char* resource_name);
int AssignSVGresourceToMenu(wxMenu* frame,int item_id,const char* resource_name);
int AssignSVGresourceToMenu(wxMenuItem* item,const char* resource_name);

int RescaleWindowDPI(wxWindow *win);
wxPGChoices& MapToPGenumChoices(const std::map<int,std::string>& map);
wxPGChoices& MapToPGenumChoices(const std::map<int,std::wstring>& map);