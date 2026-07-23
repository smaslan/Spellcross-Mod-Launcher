//=============================================================================
// Spellcross font related stuff. Uses FONT_001.FNT file.
// 
// This code is part of Spellcross Map Editor project.
// (c) 2022, Stanislav Maslan, s.maslan@seznam.cz
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================

#include "spell_font.h"
#include "other.h"
#include <vector>
#include <fstream>
#include <stdexcept>

using namespace std;

// read 0-terminated string
std::string get_string_null(uint8_t** buf,uint8_t* buf_end)
{
	string text;
	while(*buf < buf_end && **buf)
	{
		text.append(1,**buf);
		(*buf)++;
	}
	if(*buf < buf_end)
		(*buf)++;
	return(text);
}

// blank symbol constructor
SpellFontSymbol::SpellFontSymbol(int sym_h, int sym_w)
{
	name = "";
	pixels.resize(sym_h);
	for(int y = 0; y < sym_h; y++)
	{
		auto& row = pixels[y];
		row.assign(sym_w,0);
	}
}

// decode symbol
SpellFontSymbol::SpellFontSymbol(uint8_t** buf,uint8_t* buf_end,int sym_h)
{	
	if(sym_h)
	{
		// SimpleFontEditor2.0 format:
		
		// get symbol name
		name = get_string_null(buf,buf_end);

		if(*buf >= buf_end)
			throw std::exception("Ivalid data in SimpleFont2.0 symbol!");
		int sym_w = (int)(unsigned)**buf;(*buf)++;

		// check data consistency
		if(*buf + 32*32 >= buf_end)
			throw std::exception("Ivalid data in SimpleFont2.0 symbol!");

		// load pixels
		pixels.resize(sym_h);
		for(int y = 0; y < 32; y++)
		{
			if(y < sym_h)
			{
				auto& row = pixels[y];
				row.resize(sym_w);			
				for(int x = 0; x < 32; x++)
				{
					if(x < sym_w)
						row[x] = (*buf)[y + x*32];
				}
			}
			
		}
		(*buf) += 32*32;
	}
	else
	{
		// spellcross FONT_001.FNT:

		// get symbol size
		if(*buf >= buf_end)
			throw std::exception("Ivalid data in FONT_001.FNT!");
		int sym_w = (int)(unsigned)**buf;(*buf)++;
		if(*buf >= buf_end)
			throw std::exception("Ivalid data in FONT_001.FNT!");
		int sym_h = (int)(unsigned)**buf;(*buf)++;

		// check data integrity
		int sym_size = (sym_w/8 + (sym_w%8)?1:0)*sym_h;
		if(*buf + sym_size > buf_end)
			throw std::exception("Ivalid data in FONT_001.FNT!");

		// decode symbol pixels
		name = "";
		pixels.resize(sym_h);
		for(int y = 0; y < sym_h; y++)
		{
			auto & row = pixels[y];
			row.resize(sym_w);
			uint8_t bit = **buf; (*buf)++;
			for(int x = 0; x < sym_w; x++)
			{
				row[x] = (bit&0x80)?1:0;
				bit <<= 1;
			}
		}
	}
}

// get symbol size
int SpellFontSymbol::GetHeight()
{
	return(pixels.size());
}
int SpellFontSymbol::GetWidth()
{
	if(GetHeight())
		return(pixels[0].size());
	return(0);
}
// is symbol blank?
int SpellFontSymbol::IsBlank()
{	
	for(auto & row : pixels)
		for(auto & pix : row)
			if(pix)
				return(false);
	return(true);
}

// decode spellcross font file (load into vector)
SpellFont::SpellFont(uint8_t* data, int len)
{
	uint8_t *data_end = &data[len];
	
	// get chars count
	if(len < 1)
		throw std::exception("Ivalid data in FONT_001.FNT!");
	int count = (int)((unsigned)data[0]);

	// store font name
	m_font_name = "FONT_001.FNT";

	// max symbol dims
	m_max_y = 0;
	m_max_x = 0;

	if(3 > len)
		throw std::exception("Ivalid data in FONT_001.FNT!");
	int sym_w = (int)(unsigned)data[1];
	int sym_h = (int)(unsigned)data[2];

	// insert blank symbols to start
	for(int k = 0; k < 31; k++)
		m_symbols.emplace_back(sym_h,1);
	// insert space symbol
	m_symbols.emplace_back(sym_h,4);

	// load symbols
	uint8_t *ptr = &data[1];
	for(int k = 0; k < count; k++)
	{		
		// decode symbol
		m_symbols.emplace_back(&ptr,data_end);

		// get symbol size
		int sym_w = m_symbols.back().GetWidth();
		int sym_h = m_symbols.back().GetHeight();
		
		// rememebr maximum symbol sizes
		m_max_y = max(m_max_y, sym_h);
		m_max_x = max(m_max_x, sym_w);
	}

	// symbol spacing
	m_symbol_gap_x = 2;

	// default semitransparent filter
	m_filter = NULL;
}




// load font from file
SpellFont::SpellFont(std::wstring font_path)
{
	// try open file
	ifstream fr(font_path.c_str(), ios::in | ios::binary | ios::ate);
	if(!fr.is_open())
		throw runtime_error("Cannot open font file!");

	// read to local buffer and close
	streampos flen = fr.tellg();
	fr.seekg(0);
	vector<uint8_t> data(flen);		
	fr.read((char*)&data[0],flen);
	fr.close();
	
	// check format type
	const char *simple_font_mark = "SMfont_generV2.0";
	if(flen < strlen(simple_font_mark))
		throw std::exception("Ivalid data in font file!");
	int is_simple_font20 = memcmp(&data[0], simple_font_mark, strlen(simple_font_mark)) == 0;

	if(!is_simple_font20)
	{
		// it is probably spellcross font FONT_001.FNT
		SpellFont(&data[0], flen);
		return;
	}	
	// it is SimpleFontEditor2.0 font
	uint8_t *buf = &data[strlen(simple_font_mark)];
	uint8_t *buf_end = &data[0] + flen;

	// read font name
	m_font_name = get_string_null(&buf,buf_end);

	// get font height
	m_max_y = *buf++;

	// max symbol dims
	m_max_x = 0;

	// load symbols
	uint8_t* ptr = &data[1];
	for(int k = 0; k < 256; k++)
	{
		// decode symbol
		m_symbols.emplace_back(&buf,buf_end,m_max_y);

		// get symbol size
		int sym_w = m_symbols.back().GetWidth();
		int sym_h = m_symbols.back().GetHeight();

		// rememebr maximum symbol sizes
		m_max_x = max(m_max_x,sym_w);
	}	

	// symbol spacing
	m_symbol_gap_x = 1;

	// default semitransparent filter
	m_filter = NULL;
}

// merge font to current font (only blanks can be replaced by source font)
int SpellFont::Merge(SpellFont &font)
{
	if(font.m_max_y != m_max_y)
		return(1);
	
	for(int k = 0; k < font.m_symbols.size(); k++)
	{
		auto src = &font.m_symbols[k];
		if(src->IsBlank())
			continue;

		// fill gap if needed
		while(k > m_symbols.size())
			m_symbols.emplace_back();
		if(!m_symbols[k].IsBlank())
			continue;

		// replace symbol
		m_symbols[k] = *src;
	}
	return(1);
}

// set spellcross filter for text background (semi-transparent)
void SpellFont::SetFilter(uint8_t* filter)
{
	m_filter = filter;
}


// render symbol
int SpellFont::RenderSymbol(uint8_t* buffer,uint8_t* buf_end,int buf_x_size,int x_pos,int y_pos,int code,int color,int bg_color)
{
	if(code < 0 || code >= m_symbols.size())
		return(0);
	auto& sym = m_symbols[code];

	// check buffer limits 
	int sym_w = sym.GetWidth();
	int sym_h = sym.GetHeight();
	if(x_pos + sym_w >= buf_x_size)
		return(0);
	if(&buffer[(y_pos + m_max_y)*buf_x_size + x_pos + sym_w] >= buf_end)
		return(0);

	// redner symbol
	for(int y = 0; y < sym_h; y++)
	{
		uint8_t *pix = &buffer[(y_pos + y)*buf_x_size + x_pos];
		for(int x = 0; x < sym_w; x++)
		{
			if(sym.pixels[y][x])
				*pix++ = color;
			else if(bg_color >= 0)
				*pix++ = bg_color;
			else
				pix++;
		}
	}

	return(sym_w);
}

// render centered text (multiline, multicolor)
int SpellFont::Render(uint8_t* buffer,uint8_t* buf_end,int buf_x_size,int x_pos,int y_pos,int x_limit,int y_limit,vector<std::string> text,vector<int> color,int bg_color,FontShadow shadow)
{
	int tot_y = text.size()*m_max_y;
	y_pos = y_pos + y_limit/2 - tot_y/2 - 0*m_max_y/2;
	int x_end = 0;
	for(int k = 0; k < text.size(); k++)
	{
		auto& txt = text[k];
		int clr = color[min(k,(int)color.size()-1)];
		x_end = max(x_end, Render(buffer,buf_end,buf_x_size,x_pos,y_pos,x_limit,m_max_y,txt,clr,bg_color,shadow));
		y_pos += m_max_y;
	}
	next_y = y_pos;
	return(x_end);
}
// render centered text (multiline)
int SpellFont::Render(uint8_t* buffer,uint8_t* buf_end,int buf_x_size,int x_pos,int y_pos,int x_limit,int y_limit,vector<std::string> text,int color,int bg_color,FontShadow shadow)
{
	std::vector<int> colors = {color};
	return(Render(buffer,buf_end,buf_x_size,x_pos,y_pos,x_limit,y_limit,text,colors,bg_color,shadow));
}
// render centered text (single line)
int SpellFont::Render(uint8_t* buffer,uint8_t* buf_end,int buf_x_size,int x_pos,int y_pos,int x_limit, int y_limit, std::string text,int color,int bg_color,FontShadow shadow)
{
	// get text size
	int x_shift = 0;
	for(int k = 0; k < text.size(); k++)
	{
		// get symbol
		int sym = (int)(unsigned char)text[k];
		int sym_w = m_symbols[sym].GetWidth();
		// move to next symbol
		x_shift += sym_w;
		x_shift += m_symbol_gap_x;
	}
	x_shift -= m_symbol_gap_x;
	
	// update render position to center the text
	x_pos += (x_limit - x_shift)/2;
	y_pos += (y_limit - m_max_y-1)/2;

	// put text
	return(Render(buffer, buf_end, buf_x_size, x_pos, y_pos, text, color, bg_color, shadow));
}

// render text
int SpellFont::Render(uint8_t* buffer,uint8_t* buf_end,int buf_x_size,int x_pos,int y_pos,std::wstring text,int color,int bg_color,FontShadow shadow,FontAlign align)
{
	return(Render(buffer, buf_end, buf_x_size, x_pos, y_pos,wstring2stringCP895(text),color, bg_color, shadow,align));
}
int SpellFont::Render(uint8_t* buffer,uint8_t* buf_end,int buf_x_size,int x_pos,int y_pos,std::string text,int color,int bg_color,FontShadow shadow,FontAlign align)
{
	if(align == RIGHT)
	{
		// get text size
		int x_shift = 0;
		for(int k = 0; k < text.size(); k++)
		{
			// get symbol
			int sym = (int)(unsigned char)text[k];
			int sym_w = m_symbols[sym].GetWidth();
			// move to next symbol
			x_shift += sym_w;
			x_shift += m_symbol_gap_x;
		}
		x_shift -= m_symbol_gap_x;
		x_pos -= x_shift;
	}
	
	int end_x_pos = 0;	
	for(int sid = 0; sid < 3; sid++)
	{
		int x_shift = x_pos;
		int y_shift = y_pos;
		if(sid == 0 && shadow == DIAG)
		{
			x_shift += 1;
			y_shift += 1;
		}
		else if(sid == 0 && shadow == DIAG2)
		{
			x_shift += 2;
			y_shift += 2;
		}
		else if(sid == 0 && shadow == DIAG3)
		{
			x_shift += 3;
			y_shift += 3;
		}
		else if(sid == 0 && shadow == RIGHT_DOWN)
		{
			x_shift += 1;
		}
		else if(sid == 1 && shadow == RIGHT_DOWN)
		{
			y_shift += 1;
		}
		else if(sid == 0 && shadow == SOLID && m_filter)
		{
			// optional solid background
			int x_size_txt = GetTextWidth(text);
			for(int y = y_pos; y < y_pos + m_max_y; y++)
			{
				uint8_t* pix = &buffer[max(x_pos,0) + y*buf_x_size];
				if(y < 0 || pix >= buf_end)
					continue;
				for(int x = max(x_pos,0); x < min(x_pos + x_size_txt,buf_x_size); x++)
					*pix++ = m_filter[*pix];
			}
		}
		else if(sid == 0 || sid == 1)
			continue;				

		for(int k = 0; k < text.size(); k++)
		{
			// put symbol
			int sym = (int)(unsigned char)text[k];
			int sym_w = RenderSymbol(buffer, buf_end, buf_x_size, x_shift, y_shift, sym, (sid == 2)?color:bg_color,(shadow == NONE)?bg_color:(-1));
			if(!sym_w)
				break;

			// move to next symbol
			x_shift += sym_w;
			x_shift += m_symbol_gap_x;
		}
		if(sid == 2)
			end_x_pos = x_shift;
	}
	return(end_x_pos);
}

// get width of symbol + space
int SpellFont::GetSymbolWidth(wchar_t sym)
{
	int code = (uint8_t)wchar2charCP895(sym);
	if(code >= m_symbols.size())
		return(0);
	return(m_symbols[code].GetWidth() + m_symbol_gap_x);
}

// get width of symbol + space
int SpellFont::GetSymbolWidth(uint8_t sym)
{
	int code = (uint8_t)sym;
	if(code >= m_symbols.size())
		return(0);
	return(m_symbols[code].GetWidth() + m_symbol_gap_x);
}


// get width of text
int SpellFont::GetTextWidth(string &text)
{
	int x_size = 0;
	for(int k = 0; k < text.size(); k++)
	{
		// put symbol
		int sym = (int)(unsigned char)text[k];
		int sym_w = m_symbols[sym].GetWidth();		
		x_size += sym_w;
		x_size += m_symbol_gap_x;
	}
	return(x_size);
}

int SpellFont::GetHeight()
{
	return(m_max_y);
}
	