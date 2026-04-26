#pragma once

#include <wx/listctrl.h>
#include <wx/treectrl.h>
#include <wx/dialog.h>

class wxListCtrlVirtual : public wxListCtrl {
private:
	std::function<wxString(long item)> m_get_item_text_cb;
	std::function<int(long item)> m_get_item_image_cb;
	wxString OnGetItemText(long item,long column) const { if(m_get_item_text_cb) return(m_get_item_text_cb(item)); return("");};
	int OnGetItemImage(long item) const { if(m_get_item_image_cb) return(m_get_item_image_cb(item)); return(-1);};
public:
	wxListCtrlVirtual(wxWindow* parent,wxWindowID id,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize,long style=wxLC_ICON,const wxValidator& validator=wxDefaultValidator,const wxString& name=wxListCtrlNameStr)
		: wxListCtrl(parent,id,pos,size,style,validator,name) {};
	//virtual ~wxListCtrlSprite() = default;

	// set callback function for wxListCtrl->GetItemText() callback
	void SetGetItemTextCb(std::function<wxString(long item)> cb) { m_get_item_text_cb = cb;};
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

std::wstring GetExecutableDir();
wxBitmapBundle LoadSVGiconsBundle(const char* resrouce_name);

int RescaleWindowDPI(wxWindow *win);
