//=============================================================================
// class FSarchive - Loading Spellcross FS archive data to memory.
// Works only for *.FS files, but not *.FSU file!
// 
// This code is part of Spellcross Map Editor project.
// (c) 2021, Stanislav Maslan, s.maslan@seznam.cz
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================

#include "fs_archive.h"
#include "other.h"
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>

using namespace std;

// make empty archive
FSarchive::FSarchive(std::string name)
{
	int m_options = Options::NONE;
	m_file_path = L"";
	m_fs_name = name;
	m_lzw = NULL;
	m_files.clear();
}

// load entire archive file
FSarchive::FSarchive(wstring path, int options)
{
	m_lzw = NULL;
	m_fs_name = std::filesystem::path(path).filename().string();
	Append(path, options);
}

// load more data to FS archive object
void FSarchive::Append(wstring path,int options)
{
	// try to open FS archive
	m_file_path = path;
	ifstream fr(path,ios::in | ios::binary);
	if(!fr)
		throw runtime_error(string_format("Cannot open \"%ls\"!",path.c_str()));

	// init LZW decoder?
	m_options = options;
	if(options & Options::DELZ_ALL && !m_lzw)
	{
		m_lzw = new LZWexpand(1000000);
		if(!m_lzw)
			throw runtime_error("Cannot create LZW decoder!");
	}

	// get count of files in archive
	uint32_t count;
	fr.read((char*)&count,sizeof(uint32_t));

	// rellocate file list
	m_files.reserve(m_files.size() + count);

	for(int i = 0; i < count; i++)
	{	
		// make file record
		FSfile *file = new FSfile();
		
		//read file name
		char name[14];
		std::memset(name,'\0',14);
		fr.read(name,13);
		file->name = name;
		file->name = toupper(file->name);
		
		// get file offset
		uint32_t afofs;
		fr.read((char*)&afofs,sizeof(uint32_t));

		// get file len
		uint32_t aflen;
		fr.read((char*)&aflen,sizeof(uint32_t));

		// file position and size on FS
		file->file_size = aflen;
		file->file_pos = afofs;
		file->lz_unpacked = false;

		if(options & FSarchive::Options::NO_LOAD)
		{
			// no load data mode - just store file params:
			file->is_loaded = false;
		}
		else
		{
			// load data mode:
			file->is_loaded = true;
			
			// copy file data to buffer		
			file->data.resize(aflen);
			streampos next = fr.tellg();
			fr.seekg(afofs);
			fr.read((char*)file->data.data(),aflen);
			fr.seekg(next);

			// optional deLZ of known types
			if(m_lzw)
			{
				std::string ext = std::filesystem::path(name).extension().string();
				if(options & Options::DELZ_LZ && ext.compare(".LZ") == 0 
					|| options & Options::DELZ_LZ0 && ext.compare(".LZ0") == 0)
				{				
					auto &data = m_lzw->Decode(&file->data[0],&file->data[0] + aflen);
					if(data.empty())
					{
						delete file;
						throw std::runtime_error(string_format("DeLZ of file \"%s\" failed!",name));
					}
					file->data = data;				
					file->lz_unpacked = true;
				}
			}
		}

		// store file data to list
		m_files.push_back(file);
	}

	// close file
	fr.close();
}

// load file data for given file record if not loaded before
int FSarchive::LoadFile(FSfile *fsfile)
{
	if(fsfile->is_loaded)
		return(0);

	if(fsfile->file_pos < 0)
		return(1); // cannot reload if it was added outside original archive!

	// try open FS archive
	ifstream fr(m_file_path,ios::in | ios::binary);
	if(!fr)
		return(1);

	// load file data to buffer
	fsfile->data.resize(fsfile->file_size);	
	fr.seekg(fsfile->file_pos);
	fr.read((char*)fsfile->data.data(),fsfile->file_size);
	fr.close();
	fsfile->lz_unpacked = false;
	
	// optional deLZ of known types
	if(m_lzw)
	{
		std::string ext = std::filesystem::path(fsfile->name).extension().string();
		if(m_options & Options::DELZ_LZ && ext.compare(".LZ") == 0
			|| m_options & Options::DELZ_LZ0 && ext.compare(".LZ0") == 0)
		{
			auto& data = m_lzw->Decode(&fsfile->data[0],&fsfile->data[0] + fsfile->file_size);
			if(data.empty())
				return(1);
			fsfile->data = data;
			fsfile->lz_unpacked = true;
		}
	}

	// mark file as loaded
	fsfile->is_loaded = true;

	return(0);
}

// loads files from folder into archive
int FSarchive::LoadFolder(std::wstring dir, std::string wild_filter, bool allow_replace)
{
	auto path = std::filesystem::path(dir);
	if(path.has_extension())
		return(1); // likely not folder

	if(m_fs_name.empty())
	{
		// make archive name from folder
		m_fs_name = path.filename().concat(".FS").string();
	}

	bool do_wild = !(wild_filter == "*" || wild_filter.empty());

	// list folder
	for(const auto& entry: std::filesystem::directory_iterator(path))
	{
		auto item = entry.path();
		if(entry.is_directory())
			continue;
		auto name = item.filename().string();
		name = toupper(name);
		
		// skip non matching files
		if(do_wild && !wildcmp(wild_filter, name))
			continue;

		// try get existing file
		auto file = GetFileRec(name.c_str());
		if(file && !allow_replace)
			continue; // skip if exist and not allowed replace
		// make new if needed
		bool make_new = !file;
		if(make_new)
			file = new FSfile();

		// try load file data
		if(loaddata(item,file->data))
		{
			delete file;
			return(1);
		}
		
		// store file info
		file->name = name;
		file->file_pos = -1;
		file->file_size = file->data.size();
		file->is_loaded = true;
		file->lz_unpacked = false;

		// add to list
		if(make_new)
			m_files.push_back(file);
	}

	return(0);
}

// add file to archive, optional replace existing
int FSarchive::AddFile(std::string name,std::vector<uint8_t> &data,bool allow_replace)
{
	// try get existing
	auto file = GetFileRec(name.c_str());
	if(file && !allow_replace)
		return(1); // replace not allowed

	// make new if not present
	if(!file)
	{
		file = new FSfile();
		m_files.push_back(file);
	}

	// add or rewrite
	file->name = toupper(name);
	file->file_pos = -1;
	file->file_size = data.size();
	file->is_loaded = true;
	file->data = data;
	file->lz_unpacked = false;
	
	return(0);
}

// save archive to file
int FSarchive::SaveFile(std::wstring path,bool allow_overwrite)
{
	if(!path.empty())
		m_file_path = path;
	if(m_file_path.empty())
		return(1);
	if(std::filesystem::exists(path) && !allow_overwrite)
		return(1);

	// make new file (overwrite)
	ofstreamext fw(path,std::ios::out | std::ios::binary | std::ios::trunc);
	if(!fw.is_open())
		return(1);

	// write files count
	uint32_t count = (uint32_t)m_files.size();
	fw.write(count);

	// first file offset
	uint32_t offset = sizeof(uint32_t) + count*(13 + 2*sizeof(uint32_t));

	// write files list 
	for(auto& file: m_files)
	{
		// write file name
		char name[13];
		if(file->name.size() > sizeof(name))
			return(1);
		memset(name,'\0',sizeof(name));
		memcpy(name,file->name.c_str(),file->name.size());
		fw.write(name,sizeof(name));

		// write file offset
		fw.write(offset);

		// write file size
		uint32_t size = file->data.size();
		fw.write(size);

		offset += size;
	}

	// write files data
	for(auto& file: m_files)
	{
		fw.write((char*)file->data.data(),file->data.size());
	}

	fw.close();
	return(0);
}

// compare archive to another archive by content (must contain same files, order does not matter)
bool FSarchive::CompareArchives(FSarchive *fs)
{
	if(fs->m_files.size() != m_files.size())
		return(false);

	// for each file:
	for(auto &file: fs->m_files)
	{
		// try get this archive's file
		auto ref_file = GetFileRec(file->name.c_str());
		if(!ref_file)
			return(false);
		
		// load ref file
		if(fs->LoadFile(file))
			return(false);

		// load local file
		if(LoadFile(ref_file))
			return(false);

		// compare content
		if(file->data != ref_file->data)
			return(false);
	}
	return(true);
}

// free loaded file data (will be reloaded on next access)
int FSarchive::FreeFileData(FSfile* fsfile)
{
	if(!fsfile)
		return(1);
	fsfile->data.clear();
	fsfile->is_loaded = false;
	return(0);
}

// free loaded file data (will be reloaded on next access)
int FSarchive::FreeFileData(std::string name)
{
	return(FreeFileData(name.c_str()));
}
// free loaded file data (will be reloaded on next access)
int FSarchive::FreeFileData(const char *name)
{
	auto file = GetFileRec(name);
	if(!file)
		return(1);
	FreeFileData(file);	
	return(0);
}
// free loaded file data (will be reloaded on next access)
int FSarchive::FreeFileData(int id)
{
	if(id < 0 || id >= m_files.size())
		return(1);
	return(FreeFileData(m_files[id]));
}


// clear archive data
FSarchive::~FSarchive()
{
	for(auto & file : m_files)
		delete file;
	m_files.clear();

	if(m_lzw)
		delete m_lzw;
}

// return files count
int FSarchive::Count()
{
	return((int)m_files.size());
}

// return all files
std::vector<FSarchive::FSfile*> &FSarchive::GetFiles()
{
	return(m_files);
}

// get file item by name
FSarchive::FSfile* FSarchive::GetFileRec(const char* name)
{
	for(auto &file: m_files)
		if(_strcmpi(file->name.c_str(), name) == 0)
			return(file);
	return(NULL);
}

// get pointer to file data
std::vector<uint8_t> *FSarchive::GetFileData(const char* name)
{
	auto file = GetFileRec(name);
	if(!file)
		return(NULL);
	if(!file->is_loaded && LoadFile(file))
		return(NULL);		
	return(&file->data);
}

// get file by name
int FSarchive::GetFile(string &name,uint8_t** data,int* size)
{
	return(GetFile(name.c_str(), data, size));
}
int FSarchive::GetFile(const char* name, uint8_t** data, int* size)
{
	auto file = GetFileRec(name);
	if(!file)
		return(1);
			
	if(!file->is_loaded)
	{
		// load file data first
		if(LoadFile(file))
			return(1);
	}
			
	// file loaded - just return
	if(data)
		*data = file->data.data();
	if(size)
		*size = file->data.size();
	return(0);
}

// get file by name
string FSarchive::GetFile(string &name)
{
	return(GetFile(name.c_str()));
}
string FSarchive::GetFile(const char* name)
{
	string text;
	uint8_t* data;
	int size;
	if(GetFile(name, &data, &size))
		return(text);
	text.resize(size);
	memcpy(&text[0],data,size);
	return(text);
}

// get file by name
int FSarchive::GetFile(const char* name,std::vector<uint8_t> &data)
{
	data.clear();
	uint8_t *ptr;
	int size;
	if(GetFile(name,&ptr,&size))
		return(1);
	data.resize(size);
	memcpy(data.data(),ptr,size);		
	return(0);
}

// get file by order index (zero based)
int FSarchive::GetFile(int id, uint8_t** data, int* size, const char **name)
{	
	if (id >= m_files.size())
		return(1);
	if(LoadFile(m_files[id]))
		return(1);
	if (data)
		*data = m_files[id]->data.data();
	if (size)
		*size = m_files[id]->data.size();
	if (name)
		*name = m_files[id]->name.c_str();
	return(0);
}

// get name of file index
const char* FSarchive::GetFileName(int id)
{
	if(id >= m_files.size())
		return(NULL);
	return(m_files[id]->name.c_str());
}

// get name of FS archive
std::string FSarchive::GetFSname(bool with_extension)
{
	if(with_extension)
		return(m_fs_name);
	else
		return(std::filesystem::path(m_fs_name).replace_extension().string());
}

// get wildcard filtered file names
std::vector<std::string> FSarchive::GetFileNames(std::string wild)
{
	//auto *list = new std::vector<std::string>;
	std::vector<std::string> list;
	for(auto &file: m_files)
		if(wildcmp(wild.c_str(),file->name.c_str()))
			list.push_back(file->name);
	//return(std::move(*list));
	return(list);
}