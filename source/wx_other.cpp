#include "wx_other.h"

#include <wx/stdpaths.h>
#include <filesystem>

// get executable directory via wxWidgets (should be multiplatform)
std::wstring GetExecutableDir()
{
	// get exec path
	std::wstring exe_path = ::wxStandardPaths::Get().GetExecutablePath().ToStdWstring();
#ifdef __WXMSW__
	exe_path = std::filesystem::path(exe_path).parent_path();
#endif
	return(exe_path);
}

// load SVG resource and make icon variants in all common sizes
wxBitmapBundle LoadSVGiconsBundle(const char *resrouce_name)
{
	// ###note: I have dound no other ways then render SVG to all common sizes...
	wxVector<wxBitmap> bmp_list;
	int sizes[] = {16,20,24,28,32,48,64};
	for(auto dim: sizes)
		bmp_list.push_back(wxBitmapBundle::FromSVGResource(resrouce_name,wxSize(dim,dim)).GetBitmap(wxSize(dim,dim)));
	wxBitmapBundle bundle = wxBitmapBundle::FromBitmaps(bmp_list);	
	return(bundle);
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