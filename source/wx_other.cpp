#include "wx_other.h"

#include <wx/stdpaths.h>
#include <wx/menu.h>
#include <wx/menuitem.h>


// get executable directory via wxWidgets (should be multiplatform)
std::filesystem::path GetExecutableDir()
{
	// get exec path
	std::wstring exe_path = ::wxStandardPaths::Get().GetExecutablePath().ToStdWstring();
#ifdef __WXMSW__
	exe_path = std::filesystem::path(exe_path).parent_path();
#endif
	return(exe_path);
}

// load SVG resource and make icon variants in all common sizes
wxBitmapBundle LoadSVGiconsBundle(const char *resource_name)
{
	// ###note: I have dound no other ways then render SVG to all common sizes...
	wxVector<wxBitmap> bmp_list;
	int sizes[] = {16,20,24,28,32,48,64};
	for(auto dim: sizes)
		bmp_list.push_back(wxBitmapBundle::FromSVGResource(resource_name,wxSize(dim,dim)).GetBitmap(wxSize(dim,dim)));
	wxBitmapBundle bundle = wxBitmapBundle::FromBitmaps(bmp_list);	
	return(bundle);
}

// try assign SVG resource icon to wxFrame menu item by item id
int AssignSVGresourceToMenu(wxMenu *frame, int item_id,const char* resource_name)
{
	if(!frame || !item_id)
		return(1);
	auto item = frame->FindItem(item_id);
	if(!item)
		return(1);
	auto res = LoadSVGiconsBundle(resource_name);
	item->SetBitmaps(res);
	return(0);
	
}

// try assign SVG resource icon to wxFrame menu item
int AssignSVGresourceToMenu(wxMenuItem* item,const char* resource_name)
{
	if(!item)
		return(1);
	auto res = LoadSVGiconsBundle(resource_name);
	item->SetBitmaps(res);
	return(0);

}

// rescale window based on DPI, if has parent, center to parent
int RescaleWindowDPI(wxWindow *win)
{
	if(!win)
		return(1);
	//if(win->GetParent())
	{
		auto pos = win->GetPosition();
		auto size = win->GetSize();
		auto cx = pos.x + size.x/2;
		auto cy = pos.y + size.y/2;
		auto new_size = win->FromDIP(size);
		auto new_x = cx - new_size.x/2;
		auto new_y = cy - new_size.y/2;
		new_x -= std::min(new_x,0);
		new_y -= std::min(new_y,0);
		int disp_x_size;
		int disp_y_size;
		wxDisplaySize(&disp_x_size,&disp_y_size);
		new_size.x = std::min(new_size.x,disp_x_size - new_x);
		new_size.y = std::min(new_size.y,disp_y_size - new_y);
		win->SetPosition(wxPoint(new_x,new_y));
		win->SetSize(new_size);
		win->Layout();
	}
	return(0);
}

// make PropertyGrid choices from std::map<int,string> (careful - not reentrant!)
wxPGChoices& MapToPGenumChoices(const std::map<int,std::string>& map)
{
	static wxPGChoices ch;
	ch.Clear();
	for(auto& item: map)
		ch.Add(item.second,item.first);
	return(ch);
}
wxPGChoices& MapToPGenumChoices(const std::map<int,std::wstring>& map)
{
	static wxPGChoices ch;
	ch.Clear();
	for(auto& item: map)
		ch.Add(item.second,item.first);
	return(ch);
}