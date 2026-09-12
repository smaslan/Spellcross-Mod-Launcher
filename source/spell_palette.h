//=============================================================================
// Spellcross color palette related stuff.
// 
// This code is part of Spellcross Map Editor project.
// (c) 2026, Stanislav Maslan, s.maslan@seznam.cz
// url: https://github.com/smaslan/spellcross-map-edit
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================
#pragma once

#include "cstdint"
#include <vector>
#include <string>
#include <filesystem>

class wxBitmap;

// palette record
class SpellPalette
{
public:
	std::string m_name;
	std::vector<uint8_t> m_pal;
	std::vector<uint8_t> m_used;

	class Chunk{
	public:
		std::string name;
		int offset;
		int size;
	};
	std::vector<Chunk> m_chunks;

	SpellPalette();
	SpellPalette(std::string name);
	void Clear();
	int Insert(std::vector<uint8_t> &data,std::string name="",int offset=0,int count=0);
	int Insert(std::wstring path,int offset=0,std::string used="");
	std::tuple<int,int> GetRange(int start=0);
	std::string GetRangeString();
	int Save(std::wstring path);
	int SaveChunks(std::wstring directory_path);
	int LoadInfo(std::filesystem::path path);
	int SaveInfo(std::filesystem::path path);
	int Render(wxBitmap& bmp);
	int RenderPaletteColor(wxBitmap& bmp,int x_size,int x_pos,uint8_t* filter=NULL);
};
