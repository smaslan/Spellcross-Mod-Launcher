#pragma once

#include <filesystem>
#include <mutex>
#include <stdarg.h>
#include <string>
#include "other.h"

class LogFile{
private:
    static std::filesystem::path m_path;
    static bool m_enabled;
    static std::mutex m_lock;
    static int m_indent;
    static int m_indent_step;

public:

    static void SetPath(std::filesystem::path path,bool clean=false);
    static void SetState(bool enabled);
    static int ClearLog();
    static void SetIndentStep(int step = 2);
    static void SetIndent(int indent_increment = -1000);
    
    template<typename... Args> static int Write(const std::string fmt,Args... args)
    {
        if(!m_enabled)
            return(0);
        if(m_path.empty())
            return(1);

        m_lock.lock();
        std::fstream fw(m_path,std::ios::in | std::ios::out | std::ios::ate);
        if(!fw)
        {
            m_lock.unlock();
            return(1);
        }
        
        bool eol = true;
        auto flen = fw.tellg();
        if(flen)
        {
            eol = false;
            fw.seekg(-1,std::ios_base::_Seekend);
            char sym;
            fw.read(&sym,1);            
            if(sym == '\r' || sym == '\n')
                eol = true;
        }
        fw.seekg(0,std::ios_base::_Seekend);
        if(eol)
        {
            std::string indent(m_indent*m_indent_step,' ');
            fw << indent;
        }
        fw << string_format(fmt.c_str(),args...);
        fw.close();
        m_lock.unlock();
        return(0);
    }

};
