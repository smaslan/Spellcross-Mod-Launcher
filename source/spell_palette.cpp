//=============================================================================
// Spellcross color palette related stuff.
// 
// This code is part of Spellcross Map Editor project.
// (c) 2021-2026, Stanislav Maslan, s.maslan@seznam.cz
// url: https://github.com/smaslan/spellcross-map-edit
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================
#include "spell_palette.h"
//#include "sprites.h"
//#include "fs_archive.h"
//#include "fsu_archive.h"
//#include "spell_units.h"
//#include "LZ_spell.h"
//#include "spell_texts.h"
#include "other.h"
//#include "map.h"

#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <regex>
#include <filesystem>

#include <wx/rawbmp.h>


//=============================================================================
// Spellcross palette
//=============================================================================

// make copy of palette with optional chunks filter
SpellPalette::SpellPalette(SpellPalette &pal,std::vector<std::string> chunks)
{
	m_name = pal.m_name;
	m_pal.assign(3*256,0);
	m_used.assign(256,0);
	m_used_user.assign(256,0);
	
	if(chunks.empty())
		std::ranges::transform(pal.m_chunks,std::back_inserter(chunks),&Chunk::name);

	for(auto &chunk: pal.m_chunks)
	{
		if(std::ranges::find(chunks,chunk.name) == chunks.end())
			continue;

		m_chunks.push_back(chunk);
		memcpy(m_pal.data() + chunk.offset*3,pal.m_pal.data() + chunk.offset*3,chunk.size*3);
		memset(m_used.data() + chunk.offset,1,chunk.size);
	}
	
	// keep sorted by chunk positions
	SortChunks();
}

// make palette record
SpellPalette::SpellPalette(std::string name)
{
	m_name = name;
	m_pal.assign(3*256,0);
	m_used.assign(256,0);
	m_used_user.assign(256,0);
}

// clear palette colors
void SpellPalette::Clear()
{
	m_pal.assign(3*256,0);
	m_used.assign(256,0);
	m_used_user.assign(256,0);
	m_name = "";
	m_chunks.clear();
}

// sort chunks by offsets
void SpellPalette::SortChunks()
{
	std::ranges::sort(m_chunks,[](const Chunk& a,const Chunk& b) {return(b.offset >= a.offset);});
}

// get raw palette 2D array [index][channel]
uint8_t(*SpellPalette::GetPal())[3]
{
	return((uint8_t(*)[3])m_pal.data());
}

// check if pixel index belongs to chunk
bool SpellPalette::Chunk::isWithin(uint8_t pixel)
{
	return(pixel >= offset && pixel <= offset + size);
}

// check if any pixel belongs to chunk
bool SpellPalette::Chunk::isWithin(std::vector<uint8_t>& pixels)
{
	int c_ofs = offset;
	int c_size = size;
	return(std::ranges::any_of(pixels,[c_ofs,c_size](const uint8_t &pixel){return(pixel >= c_ofs && pixel <= c_ofs + c_size);}));
}

// place chunk of data to palette with offset (0 - 255)
int SpellPalette::Insert(uint8_t *data,std::string name,int offset,int count)
{
	if(!count)
		return(1);
	if(offset + count > 256)
		return(1);

	memcpy(m_pal.data() + offset*3,data,count*3);
	memset(m_used.data() + offset,1,count);

	// add chunk record
	SpellPalette::Chunk chunk;
	chunk.name = name;
	chunk.offset = offset;
	chunk.size = count;
	m_chunks.push_back(chunk);

	// keep sorted by chunk positions
	SortChunks();

	return(0);
}

// place chunk of data to palette with offset (0 - 255)
int SpellPalette::Insert(std::vector<uint8_t>& data,std::string name,int offset,int count)
{
	if(!count)
		count = data.size()/3;
	if(3*count > data.size())
		return(1);
	if(count && offset + count > 256)
		return(1);
	
	memcpy(m_pal.data() + offset*3,data.data(),count*3);
	memset(m_used.data() + offset,1,count);
	
	// add chunk record
	SpellPalette::Chunk chunk;
	chunk.name = name;
	chunk.offset = offset;
	chunk.size = count;
	m_chunks.push_back(chunk);

	// keep sorted by chunk positions
	SortChunks();
		
	return(0);
}

// place chunk of data to palette from file with offset (0 - 255)
int SpellPalette::Insert(std::filesystem::path path,int offset,std::string used)
{
	std::vector<uint8_t> chunk;
	if(loaddata(path,chunk))
		return(1);

	auto name = std::filesystem::path(path).filename().string();
	if(Insert(chunk,name,offset))
		return(1);
	
	// try assign mask of used colors from string style: 0, 128-191, ...
	if(used.empty())
		return(1);
	m_used.assign(256,0);

	// parse used string
	auto used_temp = ParseRangeStr(used);
	if(used_temp.empty())
		return(1);
	m_used = used_temp;

	// keep sorted by chunk positions
	SortChunks();

	return(0);
}

// insert blank chunk
int SpellPalette::InsertBlank(std::string name,int offset,int size,std::string color_str_filter)
{	
	// parse color range string
	auto used = ParseRangeStr(color_str_filter);
	if(used.empty())
		return(1);
	m_used = used;

	// filter out stuff outside range str (should not happen)
	if(!color_str_filter.empty())
	{
		for(int k = 0; k < used.size(); k++)
			if(k < offset || k >= offset + size)
				m_used[k] = 0;
	}

	// clear affected colors
	for(int k = 0; k < used.size(); k++)
		if(m_used[k])
			std::memset(&m_pal[k*3],0x00,3);

	// add chunk to list
	Chunk chunk;
	chunk.name = name;
	chunk.offset = offset;
	chunk.size = size;
	m_chunks.push_back(chunk);
	
	// keep sorted by chunk positions
	SortChunks();

	return(0);
}

// assign colors to palette
int SpellPalette::AssignColors(std::vector<ImgQuantize::Pixel>& colors,std::vector<uint8_t> mask)
{
	if(mask.empty())
		mask.assign(256,1);
	if(mask.size() != 256)
		return(1);

	for(int k = 0; k < 256 ;k++)
		mask[k] = !!(mask[k] && m_used[k]);

	// place colors
	auto pal = GetPal();
	for(auto &col: colors)
	{
		auto p = std::ranges::find(mask,1);
		if(p == mask.end())
			return(1); // colors wont fit to slots
		*p = 0;
		auto id = p - mask.begin();
		pal[id][0] = col.r;
		pal[id][1] = col.g;
		pal[id][2] = col.b;
	}

	return(0);
}

// get used color count
int SpellPalette::GetUsedCount()
{
	return(std::ranges::count(m_used,1));
}

// get assigned range
std::tuple<int,int> SpellPalette::GetRange(int start)
{
	auto beg = std::find(m_used.begin() + start, m_used.end(), 1);
	if(beg == m_used.end())
		return std::tuple(-1,-1);
	auto end = std::find(beg,m_used.end(),0);
	if(end == m_used.end())
		end = m_used.end();	
	return std::tuple(beg - m_used.begin(),end - m_used.begin() - 1);
}

// get assigned range string
std::string SpellPalette::GetRangeString(bool add_zero,std::vector<std::string> chunk_names)
{
	if(chunk_names.empty())
	{
		// all chunks
		int offset = 0;
		bool was0 = false;
		std::string palstr = "";
		while(true)
		{
			auto [p1,p2] = GetRange(offset);
			if(p1 < 0 || p2 < 0)
				break;
			if(p1 == 0)
				was0 = true;
			if(offset)
				palstr += ", ";
			palstr += string_format("%d-%d",p1,p2);
			offset = p2 + 1;
		}
		if(!was0 && add_zero)
			palstr = "0, " + palstr;
		return(palstr);
	}
	
	// selected chunks
	bool was0 = false;
	std::vector<std::string> list;
	for(auto &chunk: m_chunks)
	{
		if(!std::ranges::any_of(chunk_names,[chunk](const std::string &item){return(iequals(item,chunk.name));}))
			continue;
		list.push_back(string_format("%d-%d",chunk.offset,chunk.offset+chunk.size-1));
		was0 = (chunk.offset == 0);
	}
	auto palstr = merge_text_lines(list,", ");
	if(!was0 && add_zero)
		palstr = "0, " + palstr;
	return(palstr);
}

// save entire palette to file
int SpellPalette::Save(std::filesystem::path path)
{
	return(savedata(path,m_pal));
}

// save palette chunks to file
int SpellPalette::SaveChunks(std::filesystem::path directory_path)
{
	// for each palette chunk:
	for(auto &chunk: m_chunks)
	{
		if(chunk.offset + chunk.size > m_pal.size()/3)
			return(1);

		// create chunk
		std::vector<uint8_t> pal(m_pal.begin() + chunk.offset*3,m_pal.begin() + (chunk.offset + chunk.size)*3);
				

		auto path = std::filesystem::path(directory_path).append(chunk.name);
		if(std::filesystem::exists(path))
		{
			// skip chunks that already exist and are identical
			std::vector<uint8_t> old_pal;
			if(loaddata(path,old_pal))
				return(1);
			if(pal == old_pal)
				continue;
		}

		// store chunk
		if(savedata(path, pal))
			return(1);		
	}

	return(0);
}



// save palette as info file
int SpellPalette::SaveInfo(std::filesystem::path path,std::vector<std::string> chunks)
{
	SpellPalette pal(*this,chunks);
	
	std::string info = "";

	info += string_format("// Spellcross palette meta file (autogenerated by Spellcross Map Editor)\n");
	info += string_format("name:: %s\n",pal.m_name.c_str());
	info += string_format("size:: %d\n",pal.m_pal.size()/3);
	info += string_format("assigned:: %s\n",pal.GetRangeString().c_str());

	std::vector<std::string> list;
	for(int k = 0; k < m_pal.size() - 2; k += 3)
		list.push_back(string_format("%3u, %3u, %3u",pal.m_pal[k + 0],pal.m_pal[k + 1],pal.m_pal[k + 2]));
	info += string_format("\n");
	info += info_make_text_vector("colors",list,"// format: R, G, B");

	std::vector<std::string> chunk_names;
	for(auto &chunk: pal.m_chunks)
		chunk_names.push_back(chunk.name);	
	info += string_format("\n");
	info += info_make_text_vector("chunks list",chunk_names);
			
	for(auto& chunk: pal.m_chunks)
	{
		std::string cinf = "";
		
		cinf += string_format("offset:: %d\n",chunk.offset);
		cinf += string_format("size:: %d\n\n",chunk.size);
		
		std::vector<std::string> list;
		for(int k = chunk.offset; k < chunk.offset + chunk.size; k += 3)
			list.push_back(string_format("%3u, %3u, %3u",m_pal[k + 0],m_pal[k + 1],m_pal[k + 2]));
		cinf += info_make_text_vector("colors",list,"// format: R, G, B");

		info += string_format("\n");
		info += info_make_section(chunk.name,cinf);
	}
	
	// save file only if differs from existing
	std::string info_old;
	loadstr(path,info_old);
	if(info_old == info)
		return(0);
	return(savestr(path,info));
}

// load palette from info file
int SpellPalette::LoadInfo(std::filesystem::path path)
{
	Clear();

	// load source info
	std::string infostr;
	if(loadstr(path,infostr))
		return(1);
	auto info = get_text_lines(infostr);

	// palette name
	m_name = info_get_string(info, "name");
	if(m_name.empty())
		return(1);
	
	// read common colors list
	auto colors_list = info_get_text_vector(info,"colors");
	int cid = 0;
	for(auto &color: colors_list)
	{
		int r,g,b;
		if(cid >= 3*256 || std::sscanf(color.c_str(),"%d,%d,%d",&r,&g,&b) != 3)
		{
			Clear();
			return(1);
		}
		m_pal[cid + 0] = r;
		m_pal[cid + 1] = g;
		m_pal[cid + 2] = b;
		cid += 3;		
	}

	// parse assigned string
	auto assigned = info_get_string(info,"assigned");
	if(assigned.empty())
	{
		Clear();
		return(1);
	}
	auto chunks = get_text_lines(assigned, true, ',');
	for(auto& chunk: chunks)
	{
		auto list = regexp_get(chunk,"\\s*([\\d]+)\\s*-*\\s*([\\d]+)*");
		if(list.size() < 1)
		{
			Clear();
			return(1);
		}
		int from = std::atoi(list[0].c_str());
		int to = -1;
		if(list.size() >= 2 && !list[1].empty())
			to = std::atoi(list[1].c_str());
		if(from > 255 || to > 255)
		{
			Clear();
			return(1);
		}
		m_used[from] = 1;
		if(to > 0)
			std::fill(m_used.begin() + from,m_used.begin() + to,1);
	}

	// parse chunks
	auto chunk_list = info_get_text_vector(info, "chunks list");
	for(auto &chunk_name: chunk_list)
	{
		auto cinf = info_get_section(info,chunk_name);
		Chunk chunk;
		chunk.name = chunk_name;
		chunk.offset = info_get_int(cinf,"offset",-1);
		chunk.size = info_get_int(cinf,"size",-1);
		if(chunk.size < 0 || chunk.offset < 0 || chunk.offset + chunk.size > 256)
		{
			Clear();
			return(1);
		}
		m_chunks.push_back(chunk);
	}

	return(0);
}

// try get chunk by name
SpellPalette::Chunk* SpellPalette::GetChunk(std::string name)
{
	auto chunk = std::ranges::find_if(m_chunks,[name](Chunk &chunk){return(iequals(chunk.name,name));});
	if(chunk == m_chunks.end())
		return(NULL);
	return(&*chunk);
}

// get chunk names list
std::vector<std::string> SpellPalette::GetChunkNames()
{
	std::vector<std::string> list;
	std::ranges::transform(m_chunks,std::back_inserter(list),&Chunk::name);
	return(list);
}

// get list of chunks required for the pixels
std::vector<std::string> SpellPalette::GetChunkNames(std::vector<uint8_t>& pixels)
{
	std::vector<std::string> list;
	for(auto &chunk: m_chunks)
	{
		if(!chunk.isWithin(pixels))
			continue;
		list.push_back(chunk.name);
	}
	return(list);
}

// check if pixels are covered by given chunk names within the palette
bool SpellPalette::CheckPixels(std::vector<uint8_t>& pixels,std::vector<std::string> chunk_names)
{
	auto list = GetChunkNames(pixels);
	for(auto &name: list)
		if(!std::ranges::any_of(chunk_names,[name](const std::string &item){return(iequals(item,name));}))
			return(false);
	return(true);
}

// render palette into bitmap (scale up as much as possible)
int SpellPalette::Render(wxBitmap& bmp)
{
	// canvas size
	int surf_x = bmp.GetWidth();
	int surf_y = bmp.GetHeight();

	int x_color_width = surf_x/256;
	int x_ofs = (surf_x - x_color_width*256)/2;
	int x_end = x_ofs + x_color_width*256;

	// split vertically
	int filter_y_limit = surf_y/2;

	// palette
	uint8_t (*pal)[3] = (uint8_t(*)[3])m_pal.data();

	// render 24bit RGB data to raw bmp buffer
	wxNativePixelData data(bmp);
	wxNativePixelData::Iterator p(data);
	for(int y = 0; y < surf_y; ++y)
	{
		uint8_t* scan = p.m_ptr;
		for(int x = 0; x < surf_x; x++)
		{
			if(x < 1 || x >= surf_x-1 || y < 1 || y >= surf_y-1)
			{
				*scan++ = 0x00;
				*scan++ = 0x00;
				*scan++ = 0x00;
			}
			else if(x >= x_ofs && x < x_end)
			{
				int color = (x - x_ofs)/x_color_width;
				*scan++ = pal[color][2];
				*scan++ = pal[color][1];
				*scan++ = pal[color][0];
			}
			else
			{
				uint8_t color = (!(x&32) ^ !(y&32))?0x88:0xAA;
				*scan++ = color;
				*scan++ = color;
				*scan++ = color;
			}
		}
		p.OffsetY(data,1);
	}

	return(0);
}

// render palette color selection into canvas
int SpellPalette::RenderPaletteColor(wxBitmap& bmp,int x_size,int x_pos,uint8_t* filter)
{
	// canvas size
	int surf_x = bmp.GetWidth();
	int surf_y = bmp.GetHeight();

	// palette position and size in canvas
	int x_color_width = x_size/256;
	int x_ofs = (x_size - x_color_width*256)/2;
	int x_end = x_ofs + x_color_width*256;

	int is_selected = x_pos >= x_ofs && x_pos < x_end;
	int pal_id = (is_selected)?((x_pos - x_ofs)/x_color_width):-1;
	if(filter)
		pal_id = filter[pal_id];

	uint8_t(*pal)[3] = (uint8_t(*)[3])m_pal.data();

	// render 24bit RGB data to raw bmp buffer
	wxNativePixelData data(bmp);
	wxNativePixelData::Iterator p(data);
	for(int y = 0; y < surf_y; ++y)
	{
		uint8_t* scan = p.m_ptr;
		for(int x = 0; x < surf_x; x++)
		{
			if(x > 0 && x < surf_x-1 && y > 0 && y < surf_y-1)
			{
				if(is_selected)
				{
					*scan++ = pal[pal_id][2];
					*scan++ = pal[pal_id][1];
					*scan++ = pal[pal_id][0];
				}
				else
				{
					uint8_t color = (!(x&8) ^ !(y&8))?0x88:0xAA;
					*scan++ = color;
					*scan++ = color;
					*scan++ = color;
				}
			}
			else
			{
				*scan++ = 0x00;
				*scan++ = 0x00;
				*scan++ = 0x00;
			}
		}
		p.OffsetY(data,1);
	}

	return(pal_id);
}


// clear user range
int SpellPalette::ClearUserRange()
{
	m_used_user.assign(256,0);
	return(0);
}
// add whole chunk to user range
int SpellPalette::AddUserRange(std::string chunk_name)
{
	auto chunk = GetChunk(chunk_name);
	if(!chunk)
		return(1);
	std::fill(m_used_user.begin() + chunk->offset, m_used_user.begin() + chunk->offset + chunk->size, 1);
	return(0);
}
// add whole chunk(s) to user range
int SpellPalette::AddUserRange(std::vector<std::string>& chunk_names)
{
	for(auto &name: chunk_names)
		if(AddUserRange(name))
			return(1);
	return(0);
}
// add pixels indices to user range
int SpellPalette::AddUserRange(std::vector<uint8_t>& pixels)
{
	for(auto &pix: pixels)
		m_used_user[pix] = 1;
	return(0);
}
// add user range from range string (e.g.: "0-127,220-229")
int SpellPalette::AddUserRangeStr(std::string range_string)
{
	auto used = ParseRangeStr(range_string);
	if(used.empty())
		return(1);
	m_used_user = used;
	return(0);
}
// get assigned user range
std::tuple<int,int> SpellPalette::GetUserRange(int start)
{
	auto beg = std::find(m_used_user.begin() + start,m_used_user.end(),1);
	if(beg == m_used_user.end())
		return std::tuple(-1,-1);
	auto end = std::find(beg,m_used_user.end(),0);
	if(end == m_used_user.end())
		end = m_used_user.end();
	return std::tuple(beg - m_used_user.begin(),end - m_used_user.begin() - 1);
}
// get color range string from user range
std::string SpellPalette::GetUserRangeString(bool add_zero)
{
	// all chunks
	int offset = 0;
	bool was0 = false;
	std::string palstr = "";
	while(true)
	{
		auto [p1,p2] = GetUserRange(offset);
		if(p1 < 0 || p2 < 0)
			break;
		if(p1 == 0)
			was0 = true;
		if(offset)
			palstr += ", ";
		if(p1 == p2)
			palstr += string_format("%d",p1);
		else
			palstr += string_format("%d-%d",p1,p2);
		offset = p2 + 1;
	}
	if(!was0 && add_zero)
		palstr = "0, " + palstr;
	return(palstr);
}
// check if given chunk is in user range
bool SpellPalette::isInUserRange(std::string chunk_name)
{
	auto chunk = GetChunk(chunk_name);
	if(!chunk)
		return(false);
	for(auto k = chunk->offset; k < chunk->offset + chunk->size; k++)
		if(m_used_user[k])
			return(true);
	return(false);
}

// parse range string to used vector
std::vector<uint8_t> SpellPalette::ParseRangeStr(std::string range_string,int count)
{
	// parse used string
	std::vector<uint8_t> blank;
	std::vector<uint8_t> used(count,0);
	auto chunks = get_text_lines(range_string,true,',');
	for(auto& chunk: chunks)
	{
		auto list = regexp_get(chunk,"\\s*([\\d]+)\\s*-*\\s*([\\d]+)*");
		if(list.size() < 1)
			return(blank);
		int from = std::atoi(list[0].c_str());
		int to = -1;
		if(list.size() >= 2 && !list[1].empty())
			to = std::atoi(list[1].c_str());
		if(from >= count || to >= count)
			return(blank);
		used[from] = 1;
		if(to > 0)
			std::fill(used.begin() + from,used.begin() + to + 1,1);
	}
	return(used);
}