//=============================================================================
// class FSarchive - Loading Spellcross FS archive data to memory.
// Works only for *.FS files, but not *.FSU file!
// 
// This code is part of Spellcross Map Editor project.
// (c) 2021, Stanislav Maslan, s.maslan@seznam.cz
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================
#pragma once

#include <vector>
#include <string>
#include <cstdint>
#include "LZ_spell.h"

class FSarchive
{
	public:

		class Options
		{
		public:
			static constexpr int NONE = 0x00;
			static constexpr int DELZ_LZ = 0x01;
			static constexpr int DELZ_LZ0 = 0x02;
			static constexpr int DELZ_ALL = DELZ_LZ | DELZ_LZ0;
			static constexpr int NO_LOAD = 0x04; // do not load files data
		};

		class FSfile
		{
		public:
			bool is_loaded;
			std::streampos file_pos;
			uint32_t file_size;
			std::string name;
			std::vector<uint8_t> data;
			bool lz_unpacked;
		};		

		~FSarchive();
		FSarchive(std::string name="");
		FSarchive(std::wstring path, int options=Options::NONE);
		void Append(std::wstring path,int options=Options::NONE);
		int AddFile(std::string name, std::vector<uint8_t> &data, bool allow_replace=false);
		int LoadFolder(std::wstring dir, std::string wild_filter="*", bool allow_replace=false);
		int SaveFile(std::wstring path=L"",bool allow_overwrite=false);
		bool CompareArchives(FSarchive* fs);
		std::vector<FSfile*> &GetFiles();
		int GetFile(const char* name, uint8_t** data, int* size);
		int GetFile(std::string &name,uint8_t** data,int* size);
		int GetFile(int id, uint8_t** data, int* size, const char** name=NULL);		
		inline std::string GetFile(const char* name);
		std::string GetFile(std::string& name);
		int GetFile(const char* name,std::vector<uint8_t> &data);
		FSfile *GetFileRec(const char* name);
		std::vector<uint8_t>* GetFileData(const char* name);
		int FreeFileData(const char* name);
		int FreeFileData(std::string name);
		int FreeFileData(int id);
		int Count();
		const char *GetFileName(int id);
		std::string GetFSname(bool with_extension=true);
		std::vector<std::string> GetFileNames(std::string wild="*");

	private:			
		
		int m_options;
		std::wstring m_file_path;
		std::string m_fs_name;
		LZWexpand* m_lzw;
		std::vector<FSfile*> m_files;

		int LoadFile(FSfile* fsfile);
		int FreeFileData(FSfile* fsfile);
};

