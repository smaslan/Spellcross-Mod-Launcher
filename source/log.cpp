#include "log.h"

#include <fstream>
#include "other.h"

// log file path
std::filesystem::path LogFile::m_path;
// log enabled>
bool LogFile::m_enabled = false;
// exclusive lock just in case of multithreaded access
std::mutex LogFile::m_lock;
// print indent level
int LogFile::m_indent = 0;
// indent step
int LogFile::m_indent_step = 2;

// set log file path
void LogFile::SetPath(std::filesystem::path path, bool clean)
{
    m_path = path;
}

// enable/disable logging
void LogFile::SetState(bool enabled)
{
    m_enabled = enabled;
}

// change indent by increment
void LogFile::SetIndentStep(int step)
{
    m_indent_step = step;
}

// change indent by increment
void LogFile::SetIndent(int indent_increment)
{
    m_indent = std::max(m_indent + indent_increment,0);
}

// clear log file
int LogFile::ClearLog()
{
    if(!m_enabled)
        return(0);

    if(m_path.empty())
        return(1);

    m_lock.lock();
    std::ifstream fw(m_path,std::ios::out | std::ios::trunc);
    if(!fw.is_open())
    {
        m_lock.unlock();
        return(1);    
    }
    fw.close();
    m_lock.unlock();

    return(0);
}
