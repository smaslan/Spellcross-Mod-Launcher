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