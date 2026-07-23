//=============================================================================
// Spellcross font related stuff. Uses FONT_001.FNT file.
// 
// This code is part of Spellcross Map Editor project.
// (c) 2022, Stanislav Maslan, s.maslan@seznam.cz
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================
#pragma once

#include <vector>
#include <string>
#include "cstdint"

using namespace std;

class SpellFontSymbol
{
private:
    std::string name;
        
public:
    
    vector<vector<uint8_t>> pixels;
    
    SpellFontSymbol(int sym_h=1,int sym_w=2);
    SpellFontSymbol(uint8_t **buf,uint8_t* buf_end, int is_sfe20 = false);
    int GetWidth();
    int GetHeight();
    int IsBlank();
};

class SpellFont
{    
private:
    vector<SpellFontSymbol> m_symbols;
    int m_max_y;
    int m_max_x;
    int m_symbol_gap_x;
    string m_font_name;
    uint8_t *m_filter;
    int next_y;

public:    
    enum FontShadow : int
    {
        NONE = 0,
        DIAG,
        RIGHT_DOWN,
        DIAG2,
        DIAG3,
        SOLID
    };
    enum FontAlign : int
    {
        LEFT = 0,
        RIGHT
    };
    
    SpellFont(uint8_t* data,int len);
    SpellFont(std::wstring font_path);
    void SetFilter(uint8_t *filter);
    int Merge(SpellFont& font);
    int RenderSymbol(uint8_t* buffer,uint8_t* buf_end,int buf_x_size,int x_pos,int y_pos,int code,int color,int bg_color=-1);    
    int Render(uint8_t* buffer,uint8_t* buf_end,int buf_x_size,int x_pos,int y_pos,std::wstring text,int color,int bg_color=-1,FontShadow shadow=NONE,FontAlign align=LEFT);
    int Render(uint8_t* buffer,uint8_t* buf_end,int buf_x_size,int x_pos,int y_pos,std::string text, int color,int bg_color=-1,FontShadow shadow=NONE,FontAlign align=LEFT);
    int Render(uint8_t* buffer,uint8_t* buf_end,int buf_x_size,int x_pos,int y_pos,int x_limit,int y_limit,vector<std::string> text,vector<int> color,int bg_color,FontShadow shadow=NONE);
    int Render(uint8_t* buffer,uint8_t* buf_end,int buf_x_size,int x_pos,int y_pos,int x_limit,int y_limit,vector<std::string> text,int color,int bg_color,FontShadow shadow=NONE);
    int Render(uint8_t* buffer,uint8_t* buf_end,int buf_x_size,int x_pos,int y_pos,int x_limit,int y_limit,std::string text,int color,int bg_color,FontShadow shadow=NONE);
    int GetNextY() {return(next_y);};
    int GetHeight();
    int GetSymbolWidth(wchar_t sym);
    int GetSymbolWidth(uint8_t sym);
    int GetTextWidth(string &text);

    
};


