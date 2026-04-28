#ifndef other_H
#define other_H

#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdarg.h>
#include <vector>

//using namespace std;

std::wstring char2wstring(const char* str);
std::wstring string2wstring(std::string str);
std::string wstring2string(const std::wstring& str);
std::wstring char2wstringCP852(const char* str);
std::wstring char2wstringCP895(const char* str);
char wchar2charCP895(wchar_t sym);
std::string wstring2stringCP895(std::wstring str);
std::string& toupper(std::string& str);
std::string string_format(const std::string fmt,...);
bool iequals(const std::string& a,const std::string& b);
std::string& strrep(std::string& str,std::string key,std::string rep);
int savestr(std::wstring path,std::string& str);
int loadstr(std::filesystem::path path,std::string& strbuf);
int loaddata(std::filesystem::path path,std::vector<uint8_t>& data);
std::string trim_whites(std::string str);
std::string get_timestr_iso();
std::string get_local_time_str();

template <typename TP>
std::time_t to_time_t(TP tp)
{
    using namespace std::chrono;
    auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now() + system_clock::now());
    return system_clock::to_time_t(sctp);
}

// get local time string
template <typename TP> std::string get_time_str(TP tp)
{
    //auto time = std::chrono::system_clock::now();
    std::time_t tt = to_time_t(tp);
    std::tm* gmt = std::localtime(&tt);
    std::stringstream buffer;
    buffer << std::put_time(gmt,"%Y-%m-%d %H:%M:%S");
    return(buffer.str());
}

bool fs_rename(std::filesystem::path source,std::filesystem::path dest);
bool fs_copy(std::filesystem::path source,std::filesystem::path dest,std::filesystem::copy_options options=std::filesystem::copy_options::none);
bool fs_remove(std::filesystem::path path,bool all=false);
int fs_list_dir(std::filesystem::path dir,std::string wild,bool files,bool folders,std::vector<std::string>* names=NULL,std::vector<std::filesystem::path>* paths=NULL);

bool iswild(std::string wild);
bool iswild(const char* wild);
int wildcmp(std::string &wild,std::string &string);
int wildcmp(const char* wild,const char* string);
void strcpy_noext(const char* dest,const char* src);
int hex2num(char hex);
char num2hex(int num);
std::string fix_no_duplicate_string(std::string str,std::vector<std::string>& list);

std::string info_get_string(std::string info,std::string key,std::string default_value="");
int info_get_int(std::string info,std::string key,int default_value=0);
std::string info_make_text_vector(std::string key,std::vector<std::string> list,std::string comment="");
std::string info_make_section(std::string section_name,std::string data,std::string comment="");
std::vector<std::string> info_get_text_vector(std::string info,std::string key);
std::string info_get_section(std::string info,std::string section);

std::vector<std::string> get_text_lines(std::string string,bool trim_whites=true,char separator='\n');
std::vector<std::string> regexp_get(std::string str,std::string regkey);

uint32_t popcount(uint32_t v);

//int ostream_write_string(ofstream& fw,std::string &str);
//int ostream_write_string(std::ofstream& fw,std::string str);
//int ostream_write_string(std::ofstream& fw,const char* str);
//std::string istream_read_string(std::ifstream& fr);
//int ostream_write_u8(std::ofstream& fw,uint8_t val);
//int ostream_write_u16(std::ofstream& fw,uint16_t val);
//int ostream_write_u32(std::ofstream& fw,uint32_t val);
//int ostream_write_i32(std::ofstream& fw,int32_t val);
//uint32_t istream_read_u32(std::ifstream& fr);
//int32_t istream_read_i32(std::ifstream& fr);


void plot_line(uint8_t* buffer,uint8_t* buf_end,int buf_x,int buf_y,int x_size,uint8_t color,int x0,int y0,int x1,int y1);

void apply_gamma(uint8_t* pal,double gamma);

int mod(int x,int y);
double rand(double min,double max);
double randexp(double pow,double limit=1e9);
double randgman(double shape,double scale,double max=1e9,double min=0);

/*template <class charT,class Traits>
class ofstreamext : public basic_ofstream<charT,Traits> {
public:
    ofstreamext(const char* name)
        : std::basic_ofstream<charT,Traits>(name) {}
    
    basic_ofstream<charT,Traits>& write(uint32_t data){
        
    };

};*/

//template <class charT,class Traits>
class ofstreamext : public std::ofstream {
public:
    ofstreamext(const char* name,int options)
        : std::ofstream(name,options) {};
    ofstreamext(std::string name,int options)
        : std::ofstream(name,options) {};
    ofstreamext(std::wstring name,int options)
        : std::ofstream(name,options) {};
    ~ofstreamext() = default;
    
    int write(const char *data, long size) {
        std::ofstream::write((const char*)data,size);
        return(0);
    };

    int write(uint32_t data) {
        std::ofstream::write((const char*)&data,sizeof(uint32_t));
        return(0);
    };

    int write(int32_t data) {
        std::ofstream::write((const char*)&data,sizeof(int32_t));
        return(0);
    };

    int write(uint16_t data) {
        std::ofstream::write((const char*)&data,sizeof(uint16_t));
        return(0);
    };

    int write(int16_t data) {
        std::ofstream::write((const char*)&data,sizeof(int16_t));
        return(0);
    };

    int write(uint8_t data) {
        std::ofstream::write((const char*)&data,sizeof(uint8_t));
        return(0);
    };

    int write(int8_t data) {
        std::ofstream::write((const char*)&data,sizeof(int8_t));
        return(0);
    };

    // write string with 16bit size prefix
    int write_str_p16(const char *data) {
        uint16_t len = std::strlen(data) + 1;
        std::ofstream::write((char*)&len,sizeof(uint16_t));
        std::ofstream::write(data,len);
        return(0);
    };

    /*int write(std::string& data) {
        return(write(data.c_str()));
    };*/

    int write_str_p16(std::string data) {
        return(write_str_p16(data.c_str()));
    };

};


class ifstreamext : public std::ifstream {
public:
    ifstreamext(const char* name,int options)
        : std::ifstream(name,options) {};
    ifstreamext(std::string name,int options)
        : std::ifstream(name,options) {};
    ifstreamext(std::wstring name,int options)
        : std::ifstream(name,options) {};
    ~ifstreamext() = default;

    uint32_t read_u32(){
        uint32_t val;
        std::ifstream::read((char*)&val,sizeof(uint32_t));
        return(val);
    };

    int32_t read_i32() {
        int32_t val;
        std::ifstream::read((char*)&val,sizeof(int32_t));
        return(val);
    };

    uint16_t read_u16() {
        uint16_t val;
        std::ifstream::read((char*)&val,sizeof(uint16_t));
        return(val);
    };

    int16_t read_i16() {
        int16_t val;
        std::ifstream::read((char*)&val,sizeof(int16_t));
        return(val);
    };

    uint8_t read_u8() {
        uint8_t val;
        std::ifstream::read((char*)&val,sizeof(uint8_t));
        return(val);
    };

    int8_t read_i8() {
        int8_t val;
        std::ifstream::read((char*)&val,sizeof(int8_t));
        return(val);
    };

    std::vector<uint8_t> read_vector() {
        seekg(0,SEEK_END);
        auto flen = tellg();
        seekg(0);
        std::vector<uint8_t> data(flen);
        read((char*)data.data(),flen);
        return(data);
    }
  
    // read string item with size prefix (16bit)
    std::string read_str_p16()
    {
        uint16_t len = read_u16();
        std::string str(len,'\0');
        std::ifstream::read((char*)str.data(),len);
        str.resize(len-1);
        return(str);
    }

    // read entire file to string
    std::string read_str()
    {
        seekg(0,SEEK_END);
        auto flen = tellg();
        seekg(0);
        std::string str(flen,'\0');
        read((char*)str.data(),flen);        
        return(str);
    }

};


/*class ofstreamext : public basic_ofstream<char,char_traits<char>>
{
public:
    void write(uint32_t u32);
};*/

// write numeric
/*void fwrite_num(ofstream &fw, uint32_t u32)
{
    fw.write((char*)&u32, sizeof(uint32_t));
}
void fwrite_num(ofstream& fw,uint16_t u16)
{
    fw.write((char*)&u16,sizeof(uint16_t));
}
// write string with size prefix
void fwrite_string(ofstream& fw,string str)
{
    uint16_t len = str.size()+1;
    fw.write((char*)&len,sizeof(uint16_t));
    fw.write((char*)str.c_str(),len);
}*/


#endif


