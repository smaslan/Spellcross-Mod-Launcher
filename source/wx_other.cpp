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