#include "other.h"

#include <string>
#include <codecvt>
#include <filesystem>
#include <random>
#include <regex>
#include <sstream>
#include <chrono>
#include <format>

std::wstring char2wstring(const char* str)
{
    //setup converter
    typedef std::codecvt_utf8<wchar_t> convert_type;
    std::wstring_convert<convert_type,wchar_t> converter;
    return converter.from_bytes(str);
}

std::wstring string2wstring(std::string str)
{
    return char2wstring(str.c_str());
}

// convert wstring to UTF-8 string
std::string wstring2string(const std::wstring& str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    return myconv.to_bytes(str);
}

// convert char str to wstring using CZ CP852 encoding
std::wstring char2wstringCP852(const char* str)
{
    std::wstring codes = {L"ÇüéâäůćçłëŐőîŹÄĆ"
                     L"ÉĹĺôöĽľŚśÖÜŤťŁ×č"
                     L"áíóúĄąŽžĘę¬źČş«»"
                     L"░▒▓│┤ÁÂĚŞ╣║╗╝Żż┐"
                     L"└┴┬├─┼Ăă╚╔╩╦╠═╬¤"
                     L"đĐĎËďŇÍÎě┘┌█▄ŢŮ▀"
                     L"ÓßÔŃńňŠšŔÚŕŰýÝţ´"
                     L" ˝˛ˇ˘§÷¸°¨˙űŘř■ "};
    
    std::wstring result;
    result.resize(strlen(str));
    wchar_t *data = (wchar_t*)result.data();
    while(*str)
    {
        int code = (unsigned)(unsigned char)*str++;
        wchar_t wcode = code;
        if(code >= 128)
            wcode = codes.data()[code-128];
        *data++ = wcode;
    }
    return(result);
}

// convert char str to wstring using CZ CP895 encoding
std::wstring char2wstringCP895(const char* str)
{
    std::wstring codes ={ L"ČüéďäĎŤčěĚĹÍľĺÄÁ"
                     L"ÉžŽôöÓůÚýÖÜŠĽÝŘť"
                     L"áíóúňŇŮÔšřŕŔ¼§«»"
                     L"░▒▓│┤╡╢╖╕╣║╗╝╜╛┐"
                     L"└┴┬├─┼╞╟╚╔╩╦╠═╬╧"
                     L"╨╤╥╙╘╒╓╫╪┘┌█▄▌▐▀"
                     L"αßΓπΣσµτΦΘΩδ∞φε∩"
                     L"≡±≥≤⌠⌡÷≈°∙·√ⁿ²■ "};

    std::wstring result;
    result.resize(strlen(str));
    wchar_t* data = (wchar_t*)result.data();
    while(*str)
    {
        int code = (unsigned)(unsigned char)*str++;
        wchar_t wcode = code;
        if(code >= 128)
            wcode = codes.data()[code-128];
        *data++ = wcode;
    }
    return(result);
}

// convert unicode symbol to char using CP895 table
char wchar2charCP895(wchar_t sym)
{
    std::wstring codes = {L"ČüéďäĎŤčěĚĹÍľĺÄÁ"
                     L"ÉžŽôöÓůÚýÖÜŠĽÝŘť"
                     L"áíóúňŇŮÔšřŕŔ¼§«»"
                     L"░▒▓│┤╡╢╖╕╣║╗╝╜╛┐"
                     L"└┴┬├─┼╞╟╚╔╩╦╠═╬╧"
                     L"╨╤╥╙╘╒╓╫╪┘┌█▄▌▐▀"
                     L"αßΓπΣσµτΦΘΩδ∞φε∩"
                     L"≡±≥≤⌠⌡÷≈°∙·√ⁿ²■ "};
    if(sym < 128)
        return(char(sym));
    int unisym = codes.find(sym);
    if(unisym == std::wstring::npos)
        return(' ');
    return(128 + unisym);
}

// convert wstring using CZ CP895 encoding to string
std::string wstring2stringCP895(std::wstring str)
{
    std::string result;
    result.assign(str.size(),'\0');
    for(int k = 0; k < str.size(); k++)        
        result[k] = wchar2charCP895(str[k]);
    return(result);
}

// string to upper case
std::string &toupper(std::string& str)
{
    for(auto &sym: str)
        sym = toupper(sym);
    return(str);
}

// inplace replacement of key occurences to rep
std::string& strrep(std::string& str,std::string key,std::string rep)
{
    size_t index = 0;
    while(true) {
        index = str.find(key,index);
        if(index == std::string::npos)
            break;
        str.replace(index,key.size(),rep);
        index += rep.size();
    }
    return(str);
}

// is string wildcard?
bool iswild(std::string wild)
{
    return(iswild(wild.c_str()));
}
bool iswild(const char* wild)
{
    return(strchr(wild,'*') || strchr(wild,'?'));
}

// compare wildcard string to string
int wildcmp(std::string &wild,std::string &string)
{
    return(wildcmp(wild.c_str(),string.c_str()));
}
int wildcmp(const char* wild,const char* string)
{
    // Written by Jack Handy - <A href="mailto:jakkhandy@hotmail.com">jakkhandy@hotmail.com</A>
    const char* cp = NULL,* mp = NULL;

    while((*string) && (*wild != '*')) {
        if((*wild != *string) && (*wild != '?')) {
            return 0;
        }
        wild++;
        string++;
    }

    while(*string) {
        if(*wild == '*') {
            if(!*++wild) {
                return 1;
            }
            mp = wild;
            cp = string+1;
        }
        else if((*wild == *string) || (*wild == '?')) {
            wild++;
            string++;
        }
        else {
            wild = mp;
            string = cp++;
        }
    }

    while(*wild == '*') {
        wild++;
    }
    return !*wild;
}

// copy string with no extension, assuming destiantion has enough space!
void strcpy_noext(const char *dest, const char *src)
{
    char *ptr = (char*)dest;
    while(*src && *src != '.')
        *ptr++ = *src++;
    *ptr = '\0';
}

// convert single hex digit to int
int hex2num(char hex)
{
    if(hex >= '0' && hex <= '9')
        return(hex - '0');
    if(hex >= 'A' && hex <= 'F')
        return(hex - 'A' + 10);
    return(-1);
}
// convert single digit num to ascii char
char num2hex(int num)
{
    if(num >= 0 && num <= 9)
        return(num + '0');
    if(num >= 0x0A && num <= 0x0F)
        return(num - 0x0A + 'A');
    return('_');
}


std::string string_format(const std::string fmt,...) {
    int size = ((int)fmt.size()) * 2 + 50;   // Use a rubric appropriate for your code
    std::string str;
    va_list ap;
    while(1) {     // Maximum two passes on a POSIX system...
        str.resize(size);
        va_start(ap,fmt);
        int n = vsnprintf((char*)str.data(),size,fmt.c_str(),ap);
        va_end(ap);
        if(n > -1 && n < size) {  // Everything worked
            str.resize(n);
            return str;
        }
        if(n > -1)  // Needed size returned
            size = n + 1;   // For null char
        else
            size *= 2;      // Guess at a larger size (OS specific)
    }
    return str;
}

// compare strings case insensitive
bool iequals(const std::string& a,const std::string& b)
{
    return(_strcmpi(a.c_str(), b.c_str()) == 0);
    
    // ###: this was incredibly slow! at leas in debug mode...
    /*return std::equal(a.begin(),a.end(),
        b.begin(),b.end(),
        [](char a,char b) {
            return std::tolower(a) == std::tolower(b);
        });*/
}

// checks string for duplicates in the list, modify it to not be duplicate, like e.g. "my string 1" to "my string 2", etc.
std::string fix_no_duplicate_string(std::string str, std::vector<std::string> &list)
{
    while(std::find(list.begin(), list.end(), str) != list.end())
    {
        std::regex secexp("(.*\\D+)(\\d*$)");
        std::smatch match;
        std::regex_search(str,match,secexp);
        if(match.size() != 3)
        {
            // failed
            return(str);
        }
        std::string prefix = match[1];
        std::string num = match[2];
        if(num.empty())
            num = " 1";
        else
            num = std::to_string(std::stoi(num) + 1);
        str = prefix + num;
    }
    return(str);
}

// fetch info file key value
std::string info_get_string(std::string info, std::string key, std::string default_value)
{
    // look for entire section
    std::regex secexp("^\\s*" + key + ":: ([^\\n]+)");
    std::smatch match;
    std::regex_search(info,match,secexp);
    if(match.size() < 2)
        return(default_value);
    return(match[1]);
}

// fetch info file key integer value
int info_get_int(std::string info,std::string key,int default_value)
{
    // look for entire section
    auto str = info_get_string(info,key,"");
    if(str.empty())
        return(default_value);
    return(std::atoi(str.c_str()));
}

// make info matrix of vector of strings (vertical)
std::string info_make_text_vector(std::string key,std::vector<std::string> list,std::string comment)
{
    std::string info = comment;
    if(!comment.empty())
        info += string_format("\n");
    info += string_format("#startmatrix:: %s\n",key.c_str());
    for(auto& item: list)
        info += string_format("\t%s\n",item.c_str());
    info += string_format("#endmatrix:: %s\n",key.c_str());
    return(info);
}

// make info section and fill with data
std::string info_make_section(std::string section_name,std::string data,std::string comment)
{
    std::string info = comment;
    if(!comment.empty())
        info += string_format("\n");
    info += string_format("#startsection:: %s\n",section_name.c_str());
    std::istringstream iss(data);
    std::string line;
    while(std::getline(iss,line,'\n')) {
        info += string_format("\t%s\n",line.c_str());
    };
    info += string_format("#endsection:: %s\n",section_name.c_str());
    return(info);
}

// get strings vector from info file
std::vector<std::string> info_get_text_vector(std::string info,std::string key)
{
    std::vector<std::string> rows;

    // look for entire matrix section
    auto regstr = string_format(".*?#startmatrix::\\s*%s\\n([\\s\\S]*?)#endmatrix::\\s*%s\\n*",key.c_str(),key.c_str());
    std::regex secexp(regstr);
    std::smatch match;
    std::regex_search(info,match,secexp);
    if(match.size() < 2)
        return(rows);

    // split rows
    std::istringstream iss(match[1]);
    std::string line;
    while(std::getline(iss,line,'\n')) {
        rows.push_back(std::regex_replace(line,std::regex("^\\s+|\\s+$|(\\s)\\s+"),"$1"));
    };
    return(rows);
}

// get section content
std::string info_get_section(std::string info,std::string section)
{
    auto regstr = string_format(".*?#startsection::\\s*%s\\n([\\s\\S]*?)#endsection::\\s*%s\\n*",section.c_str(),section.c_str());
    std::regex secexp(regstr);
    std::smatch match;
    std::regex_search(info,match,secexp);
    if(match.size() < 2)
        return("");
    return(match[1]);
}

// split string by lines (or other separators), by default also trims white chars both ends
std::vector<std::string> get_text_lines(std::string string, bool trim_whites, char separator)
{
    // split rows
    std::vector<std::string> rows;
    std::istringstream iss(string);
    std::string line;
    while(std::getline(iss,line,separator)) {
        if(trim_whites)
            rows.push_back(std::regex_replace(line,std::regex("^\\s+|\\s+$|(\\s)\\s+"),"$1"));
        else
            rows.push_back(line);
    };
    return(rows);
}

// get stuff using regex
std::vector<std::string> regexp_get(std::string str,std::string regkey)
{
    // look for entire section
    std::regex secexp(regkey);
    std::smatch match;
    std::regex_search(str,match,secexp);
    std::vector<std::string> list;
    for(int k = 1; k < match.size(); k++)
        list.push_back(match[k]);
    return(list);
}

// trim white spaces from string (both ends)
std::string trim_whites(std::string str)
{
    return(std::regex_replace(str,std::regex("^\\s+|\\s+$|(\\s)\\s+"),"$1"));
}

// get ISO8601 time/date string
std::string get_timestr_iso()
{
    auto time = std::chrono::system_clock::now();
    std::time_t tt = to_time_t(time);
    std::tm* gmt = std::gmtime(&tt);
    std::stringstream buffer;
    buffer << std::put_time(gmt,"%Y-%m-%dT%H:%M:%S%z");
    return(buffer.str());
    
    // ###note: this causes memory leaks! (bad MSVC implementation)
    /*std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    return(std::format("{0:%F}T{0:%T%z}.",now));*/
}

// get local time string
std::string get_local_time_str()
{
    auto time = std::chrono::system_clock::now();
    std::time_t tt = std::chrono::system_clock::to_time_t(std::chrono::time_point_cast<std::chrono::system_clock::duration>(time));
    std::tm* gmt = std::localtime(&tt);
    std::stringstream buffer;
    buffer << std::put_time(gmt,"%Y-%m-%d %H:%M:%S");
    return(buffer.str());
}




// save string to a file (save as binary - no alteration of line breaks)
int savestr(std::wstring path, std::string &str)
{
    std::ofstream fw(path,std::ios::out | std::ios::binary | std::ios::trunc);
    if(!fw.is_open())
        return(1);
    fw.write(str.c_str(),str.size());
    fw.close();
    return(0);
}

// load text file to string as binary
int loadstr(std::filesystem::path path, std::string &strbuf)
{
    strbuf.clear();
    std::ifstream fr(path,std::ios::in | std::ios::binary | std::ios::ate);
    if(!fr.is_open())
        return(1);    
    std::streampos flen = fr.tellg();
    fr.seekg(0);
    strbuf.resize(flen);
    fr.read((char*)strbuf.c_str(),strbuf.size());
    fr.close();
    return(0);
}

// load binary file
int loaddata(std::filesystem::path path,std::vector<uint8_t> &data)
{
    data.clear();
    std::ifstream fr(path,std::ios::in | std::ios::binary | std::ios::ate);
    if(!fr.is_open())
        return(1);
    std::streampos flen = fr.tellg();
    fr.seekg(0);
    data.resize(flen);
    fr.read((char*)data.data(),data.size());
    fr.close();
    return(0);
}

// rename file or folder, return non-zero 
bool fs_rename(std::filesystem::path source,std::filesystem::path dest)
{
    std::error_code err;
    bool err_except = false;
    try {
        std::filesystem::rename(source,dest,err);
    }catch(const std::runtime_error& error) {
        err_except = true;
    }
    return(err || err_except);
}

// copy file or folder, return non-zero 
bool fs_copy(std::filesystem::path source,std::filesystem::path dest,std::filesystem::copy_options options)
{
    std::error_code err;
    bool err_except = false;
    try {
        std::filesystem::copy(source,dest,options,err);
    }catch(const std::runtime_error& error) {
        err_except = true;
    }
    return(err || err_except);
}

// remove file or folder (optional all=true: with subfolders)
bool fs_remove(std::filesystem::path path, bool all)
{
    std::error_code err;
    bool err_except = false;
    try {
        if(all)
            std::filesystem::remove_all(path,err);
        else
            std::filesystem::remove(path,err);
    }
    catch(const std::runtime_error& error) {
        err_except = true;
    }
    return(err || err_except);
}

// list items 
int fs_list_dir(std::filesystem::path dir, std::string wild, bool files, bool folders, std::vector<std::string> *names, std::vector<std::filesystem::path> *paths)
{    
    int count = 0;
    if(names)
        names->clear();
    if(paths)
        paths->clear();
    if(!std::filesystem::exists(dir))
        return(0);
    for(const auto& entry: std::filesystem::directory_iterator(dir))
    {
        auto item = entry.path();
        if(!entry.is_directory() || !folders)
            continue;
        auto name = item.filename().string();        
        if(!wildcmp(wild.c_str(),name.c_str()))
            continue;
        if(names)
            names->push_back(name);
        if(paths)
            paths->push_back(dir / name);
        count++;
    }
    for(const auto& entry: std::filesystem::directory_iterator(dir))
    {
        auto item = entry.path();
        if(entry.is_directory() || !files)
            continue;
        auto name = item.filename().string();
        if(!wildcmp(wild.c_str(),name.c_str()))
            continue;
        if(names)
            names->push_back(name);
        if(paths)
            paths->push_back(dir / name);
        count++;
    }
    return(count);
}



// count bits in varible
uint32_t popcount(uint32_t v) {
    v = v - ((v >> 1) & 0x55555555);                // put count of each 2 bits into those 2 bits
    v = (v & 0x33333333) + ((v >> 2) & 0x33333333); // put count of each 4 bits into those 4 bits  
    return ((v + (v >> 4) & 0xF0F0F0F) * 0x1010101) >> 24;
}


// Bresneham algorithm (based on GitHub/bert algorithm)
void plot_line(uint8_t* buffer,uint8_t* buf_end,int buf_x,int buf_y,int x_size,uint8_t color,int x0,int y0,int x1,int y1)
{
    int dx =  abs(x1 - x0),sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0),sy = y0 < y1 ? 1 : -1;
    int err = dx + dy,e2; /* error value e_xy */

    for(;;) {  /* loop */
        uint8_t* pix = &buffer[buf_x + x0 + (buf_y + y0) * x_size];
        if(pix >= buffer && pix < buf_end)
            *pix = color;
        if(x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if(e2 >= dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
        if(e2 <= dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
    }
}

// apply gamma to given palette
void apply_gamma(uint8_t *pal, double gamma)
{
    for(int k = 0; k < 256*3; k++)
        *pal++ = (uint8_t)(pow((double)*pal / 255.0,1.0 / gamma) * 255.0);
}

// correct modulo
int mod(int x, int y) {
    if(y == -1) return 0;
    return x - y * (x / y - (x % y && (x ^ y) < 0));
}

// generate random number in range min-max, uniform distribution
double rand(double min,double max)
{
    static std::default_random_engine re((unsigned)std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_real_distribution<double> unif(min,max);
    return(unif(re));
}

// generate exponential random number with max value limit
double randexp(double pow,double limit)
{
    static std::default_random_engine re((unsigned)std::chrono::system_clock::now().time_since_epoch().count());
    std::exponential_distribution<double> distribution(pow);
    double num;
    do{
        num = distribution(re);
    }while(num > limit && limit > 1e-3);
    return(num);    
}

// generate gamma distribution number normalized, so maximum PDF is 1.0, with optional maximum output limits
double randgman(double shape,double scale,double max, double min)
{
    static std::default_random_engine re((unsigned)std::chrono::system_clock::now().time_since_epoch().count());
    std::gamma_distribution<double> distribution(shape,scale);
    
    // maximum of PDF
    double imode = 1.0/((shape - 1.0)*scale);
    
    // get random number lower than limit
    double num;
    do {
        num = distribution(re)*imode;
    } while(num > max || num < min);

    return(num);
}


