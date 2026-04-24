//=============================================================================
// class FSUarchive - Loading Spellcross FSU units graphics resource archive.
// It loads all resources from FSU, decompresses LZW compressed sprites
// and stores the sprites to lists. IT can take some time to decode'em as there 
// are a lot of them (>20000).
// 
// This code is part of Spellcross Map Editor project.
// (c) 2021-2026, Stanislav Maslan, s.maslan@seznam.cz
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================
#undef _HAS_STD_BYTE
#define _HAS_STD_BYTE 0

#include "fsu_archive.h"
//#include "sprites.h"
#include "LZ_spell.h"
#include "other.h"

#include "cstdint"
#include <fstream>
#include <vector>
#include <stdexcept>
#include <thread>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace std;

//---------------------------------------------------------------------------
// load FS archive
//---------------------------------------------------------------------------
// make blank
FSUarchive::FSUarchive()
{

}

// load from file
FSUarchive::FSUarchive(std::wstring path,Options options,std::function<void(std::string)> status_item)
{
	// try open file
	ifstream fr(path, ios::in | ios::binary | ios::ate);
	
	// read to local buffer and close
	streampos flen = fr.tellg();
	fr.seekg(0);
	std::vector<uint8_t> data(flen);
	fr.read((char*)data.data(), flen);	
	fr.close();

	// read total graphic resources count
	uint32_t units_count = *(uint32_t*)&data[4];
	
	
	// --- multithreaded load (was too slow):
	// max cores
	int cores = thread::hardware_concurrency();
	std::vector<std::thread> threads;
	std::vector<std::vector<FSU_resource*>> lists(cores);

	// load/decode:
	for (int k = 0; k < cores; k++)
		threads.emplace_back(&FSUarchive::LoadResourceGroup,this,data.data(),k,units_count,cores,&lists[k],options,status_item);

	// wait for threads
	for(auto & thread : threads)
		thread.join();

	// merge results
	for(auto & batch : lists)
		m_list.insert(m_list.end(),batch.begin(),batch.end());

	threads.clear();
}

// make archive from folder data (no decoding mode, just load compressed sprites)
int FSUarchive::LoadFolder(std::filesystem::path dir, std::string wild_filter)
{
	m_last_error = "";

	auto path = std::filesystem::path(dir);
	if(path.has_extension())
		return(1); // likely not folder

	bool do_wild = !(wild_filter == "*" || wild_filter.empty());

	// list folder for unit folders:
	for(const auto& entry: std::filesystem::directory_iterator(path))
	{
		auto item = entry.path();
		if(!entry.is_directory())
			continue;
		auto name = item.filename().string();
		name = toupper(name);
		if(name.size() > 5)
		{
			m_last_error = string_format("Loading FSU archive from folder (%ls) failed! Unit folder (%s) has name longer than 5 characters.", path.wstring().c_str(), name.c_str());
			return(1);
		}

		// skip non matching folders
		if(do_wild && !wildcmp(wild_filter,name))
			continue;

		if(GetResource(name.c_str()))
		{
			m_last_error = string_format("Loading FSU archive from folder (%ls) failed! Unit name (%s) already in archive.",path.wstring().c_str(),name.c_str());
			return(1);
		}

		// make new resource
		FSU_resource *unit = new FSU_resource();
		unit->name = name;
		unit->tot_sprites = 0;
		unit->offset = -1;

		// list sprites:
		auto unit_path = path / name;
		for(const auto& entry: std::filesystem::directory_iterator(unit_path))
		{
			auto item = entry.path();
			if(entry.is_directory())
				continue;
			auto sprite_name = item.filename().string();
			sprite_name = toupper(sprite_name);
			if(sprite_name.size() != 3)
			{
				delete unit;
				m_last_error = string_format("Loading FSU archive from folder (%ls) failed! Unit folder (%s) contains sprite (%s) with wrong name size (must be 3 characters).",path.wstring().c_str(),name.c_str(),sprite_name.c_str());		
				return(1);
			}
			auto sprite_path = unit_path / sprite_name;

			// load sprite data
			//FSU_sprite *spr = new FSU_sprite();
			auto spr = &unit->list.emplace_back();
			spr->not_decoded = true;
			spr->x_max = 0;
			spr->x_min = 0;
			spr->y_ofs = 0;
			spr->y_size = 0;
			spr->name = sprite_name;
			if(loaddata(sprite_path, spr->data))
			{
				delete spr;
				delete unit;				
				m_last_error = string_format("Loading FSU archive from folder (%ls) failed! Unit folder (%s) contains sprite (%s) that cannot be read?",path.wstring().c_str(),name.c_str(),sprite_name.c_str());
				return(1);
			}
			spr->len = spr->data.size();
			
			// add to sprite list
			//unit->list.push_back(spr);
			unit->tot_sprites++;
		}

		// add unit to list
		m_list.push_back(unit);
	}

	return(0);
}

// add resource to archive
int FSUarchive::AddResource(FSU_resource* res)
{
	if(!res)
		return(1);
	if(GetResource(res->name.c_str()))
		return(1);	
	auto res_copy = new FSU_resource(*res);
	m_list.push_back(res_copy);
	return(0);
}

// compare two archive by content (order of items does not matter)
bool FSUarchive::Compare(FSUarchive* ref)
{
	if(!ref)
		return(false);
	if(m_list.size() != ref->m_list.size())
		return(false);

	// for each resource:
	for(auto &unit: m_list)
	{
		auto ref_unit = ref->GetResource(unit->name.c_str());
		if(!ref_unit)
			return(false);
		if(ref_unit->list.size() != unit->list.size())
			return(false);

		// for each sprite:
		for(auto &sprite: unit->list)
		{
			auto ref_sprite = ref_unit->GetSprite(sprite.name.c_str());
			if(!ref_sprite)
				return(false);
			if(sprite.data != ref_sprite->data)
				return(false);
		}		
	}
	return(true);
}

// get resource names list
std::vector<std::string> FSUarchive::GetResourceNanes()
{
	std::vector<std::string> names_list;
	for(auto &item: m_list)
		names_list.push_back(item->name);
	return(names_list);
}

//---------------------------------------------------------------------------
// save FSU archive to file (must be undecoded one!)
//---------------------------------------------------------------------------
int FSUarchive::Save(std::filesystem::path path,bool allow_overwrite)
{
	if(std::filesystem::exists(path) && !allow_overwrite)
		return(1);
	
	ofstreamext fw(path,std::ios::out | std::ios::binary | std::ios::trunc);
	if(!fw.is_open())
		return(1);

	uint32_t unit_count = m_list.size();
	uint32_t data_offset = sizeof(uint32_t) + sizeof(uint32_t) + unit_count*(5 + sizeof(uint32_t));
	for(auto &unit: m_list)
	{
		unit->offset = data_offset;
		data_offset += sizeof(uint16_t) + unit->tot_sprites*(3 + sizeof(uint32_t) + sizeof(uint16_t));
	}

	// first data offset
	fw.write(data_offset);

	// units count
	fw.write(unit_count);

	// write units list
	for(auto& unit: m_list)
	{		
		// unit name str padded by '_'
		auto name = unit->name;
		name.resize(5,'_');
		fw.write(name.c_str(), 5);

		// sprite list offset
		fw.write(unit->offset);
	}

	// write sprite list for each unit
	for(auto& unit: m_list)
	{
		// sprites count
		fw.write((uint16_t)unit->tot_sprites);

		for(auto &sprite: unit->list)
		{
			// write sprite name
			if(sprite.name.size() != 3)
				return(1);
			fw.write(sprite.name.c_str(),3);

			// write data offset
			fw.write(data_offset);
			auto data_size = sprite.data.size();
			data_offset += data_size;

			// write data size			
			if(data_size > 65535)
				return(1);
			fw.write((uint16_t)data_size);
		}
	}

	// write sprite data
	for(auto& unit: m_list)
		for(auto& sprite: unit->list)
			fw.write((char*)sprite.data.data(),sprite.data.size());

	// done
	fw.close();
	return(0);
}

//---------------------------------------------------------------------------
// Close FS archive
//---------------------------------------------------------------------------
FSUarchive::~FSUarchive()
{
	for(auto item : m_list)
		delete item;
	m_list.clear();
}

//---------------------------------------------------------------------------
// Thread that loads multiple resources
//---------------------------------------------------------------------------
int FSUarchive::LoadResourceGroup(uint8_t* data, int first, int count, int step, std::vector<FSU_resource*> *list,Options options,std::function<void(std::string)> status_item)
{
	// read total graphic resources count
	uint32_t max_count = *(uint32_t*)&data[4];

	// initialize LZW decoder
	// note: this object should not be destroyed for each sprite otherwise it will be slow as hell!
	LZWexpand lz_decoder(100000);
	auto *delz = &lz_decoder;
	if(options & Options::NO_DECODE)
		delz = NULL;

	for (unsigned k = first; k < max_count; k += step)
	{
		// make new unit record
		FSU_resource* res = new FSU_resource();
		
		// try decode		
		LoadResource(data, k, res, delz);

		if(status_item)
			status_item(res->name);

		// add to list
		list->push_back(res);

		if (list->size() >= (unsigned)count)
			break;
	}

	return(0);
}

//---------------------------------------------------------------------------
// Load item content into list
//---------------------------------------------------------------------------
int FSUarchive::LoadResource(uint8_t *data, int rid, FSU_resource *res, LZWexpand* delz)
{
	int i,j,k;

	uint8_t* head = &data[8 + rid * (5 + 4)];

	// get unit name
	res->name.resize(5);
	auto *pstr = res->name.data();
	for(int c = 0; c < 5; c++)
	{
		char sym = *head++;		
		if (sym != '_') /* loose suffix symbols '_' */
			*pstr++ = sym;
	}
	res->name.resize(pstr - res->name.data());

	// get unit sprite list offset in FSU data
	res->offset = *(uint32_t*)head;

	// go to unit sprites list
	uint8_t* plist = &data[res->offset];

	// read total sprites count
	res->tot_sprites = *(uint16_t*)plist; plist += sizeof(uint16_t);

	res->list.reserve(res->tot_sprites);
			
	// for each sprite
	xmin  =65536;
	xmax = 0;
	ymin = 65536;
	ymax = 0;
	for (i = 0; i < res->tot_sprites; i++)
	{
		// make new sprite
		auto sprite = &res->list.emplace_back();

		// get sprite name
		sprite->name.resize(3);
		memcpy((void*)sprite->name.data(), (void*)plist, 3);
		plist += 3;
		
		// get sprite data offset in FSU
		uint32_t start = *(uint32_t*)plist; plist += sizeof(uint32_t);

		// get sprite data len
		sprite->len = *(uint16_t*)plist; plist += sizeof(uint16_t);

		// load compressed sprite data
		uint8_t* src = &data[start];
		// sprite data end
		uint8_t* srce = src + sprite->len;

		if(!delz)
		{
			// no decding mode: just store raw data
			sprite->data.resize(sprite->len);			
			memcpy(sprite->data.data(), src,sprite->len);			
			sprite->not_decoded = true;
			continue;
		}					

		// try to decode sprite archive		
		auto &spr = delz->Decode(src, srce);
		if (spr.empty())
		{
			// something has fucked while decompressing
			return(1);
		}

		// sprite data local pointer
		uint8_t* sd = spr.data();
		
		// === decode sprite ===
		// get sprite heigth
		sprite->y_size = *(uint16_t*)sd; sd += sizeof(uint16_t);

		// get sprite offset
		sprite->y_ofs = (int)*(int16_t*)sd; sd += sizeof(int16_t);
		if(sprite->y_ofs < ymin)
			ymin = sprite->y_ofs;
		
		// sprite data end
		uint8_t *se = spr.data() + spr.size();

		// alloc decoded image data (maximum possible size)
		sprite->data.resize(spr.size()*2);		
		uint8_t *img_data = sprite->data.data();
		uint8_t *img = img_data;

		// for each line
		int lxmax = 0;
		int lxmin = 256;
		for (j = 0; j < sprite->y_size; j++)
		{
			// check for EOF (don't know why, but some are shorter that they should be)
			if(sd >= (se-4))
			{
				*(int*)img = 0; img += sizeof(int);
				*(int*)img = 0; img += sizeof(int);
				continue;
			}

			// get line offset
			int ofs = (int)*(int16_t*)sd; sd += sizeof(int16_t);
			*(int*)img = ofs; img += sizeof(int);
			
			// reserve line line item
			uint8_t* llen = img; img += sizeof(int);

			// get full chunks count
			int ful = *sd++;

			// get partial chunks count
			int par = *sd++;

			// detect minimal offset
			if((ful || par) && ofs<lxmin)
				lxmin = ofs;

			// read full parts
			for(int k = 0; k < ful*4; k++)
			{
				if(sd[k])
					img[k] = sd[k];
				else
					img[k] = 254; // swap 0 to other black because of transparencies
			}			
			img += ful*4;
			sd += ful*4;
			uint8_t *md = sd+4;

			// read partial chunks
			uint8_t *imge = img;
			for (k = 0; k < par; k++)
			{
				int m;
				for (m = 0; m < 4; m++)
				{
					if(md[m])
						*img++ = 0x00;
					else
					{
						if(sd[m])
							*img++ = sd[m];
						else
							*img++ = 254; // swap 0 to other black because of transparencies
						imge = img;
					}
				}
				sd += 8;
				md += 8;
			}
			img = imge;

			// check real line len
			int xlen = (int)(imge - (llen + sizeof(int)));

			// store real line len
			*(int*)llen = xlen;

			// detect maximal line len
			if(ofs + xlen>lxmax)
				lxmax = ofs + xlen;
		}

		// global ymax
		if(sprite->y_ofs + sprite->y_size > ymax)
			ymax = sprite->y_ofs + sprite->y_size;

		// store x limits
		sprite->x_min = lxmin;
		sprite->x_max = lxmax;

		// global xmin
		if(lxmin < xmin)
			xmin = lxmin;

		// global xmax
		if(lxmax > xmax)
			xmax = lxmax;

		// store image
		int img_data_len = (int)(img - img_data);
		sprite->data.resize(img_data_len);

		// store new sprite
		sprite->not_decoded = false;
	}

	/*if(strcmp(res->name,"SRANG") == 0)
		i++;*/
	
	
	// ====== analyze sprite names ======
	for (i = 0; i < res->tot_sprites; i++)
		if(res->list[i].name[0] == 'F')
			break;
	if(i != res->tot_sprites)
	{
		// === two separate groups: movement+statical ===

		// sprites in the static group (name starts with F__) - azimuths
		res->stat.azimuths = res->tot_sprites - i;
		// single slope only
		res->stat.slopes = 1;
		// make list of standing sprites, one for each azimuth		
		res->stat.lists[0].resize(res->stat.azimuths);
		for(int k = 0; k < res->stat.azimuths; k++)
			res->stat.lists[0][k] = &res->list[k];
		// make default fire azimuths
		res->stat.fire_origin[0].assign(res->stat.azimuths,{0,0});
		

		// --- movement group ---
		if(i > 0)
		{
			// count azimuths
			char ffc = res->list[0].name[1];
			for (j = 0; j < i; j++)
				if(res->list[j].name[1] != ffc)
					break;
			
			// azimuths count
			res->anim.azimuths = j;
			// slopes count
			res->anim.slopes = 0;
			// frames count
			res->anim.frames = i / j;
			// make list of azimuths
			res->anim.lists.resize(res->anim.azimuths);
			// for each azimuth:
			for (int azid = 0; azid < res->anim.azimuths; azid++)
			{
				res->anim.lists[azid].resize(res->anim.frames);
				for (int k=0; k < res->anim.frames; k++)
					res->anim.lists[azid][k] = &res->list[azid + k*res->anim.azimuths];
			}
			// make default fire azimuths
			res->anim.fire_origin.assign(res->anim.azimuths,{0,0});
		}
		else
		{
			// no movement images			
			res->anim.azimuths = 0;
			res->anim.frames = 0;
			res->anim.lists.clear();
		}
	}
	else
	{
		// === single image group ===
		
		for (i = 0; i < res->tot_sprites; i++)
			if(res->list[i].name[1] != '0')
				break;

		if(res->name == "DPEKJ")
		{
			// --- animation with single azimuth, but more slopes (hell cavallery)

			// azimuths count
			res->anim.azimuths = 0;
			// slopes count
			res->anim.slopes = 13;
			// frames count
			res->anim.frames = res->tot_sprites / 13;
			// make list of azimuths
			res->anim.lists.resize(res->anim.slopes);
			// for each azimuth:
			for(int azid = 0; azid < res->anim.slopes; azid++)
			{
				res->anim.lists[azid].resize(res->anim.frames);
				for(int k = 0; k < res->anim.frames; k++)
					res->anim.lists[azid][k] = &res->list[k + azid*res->anim.frames];
			}
			// make default fire azimuths
			res->anim.fire_origin.assign(res->anim.slopes,{0,0});
		}
		else if(i != res->tot_sprites)
		{
			// --- it's animation ---
		
			// count azimuths
			char ffc = res->list[0].name[1];
			for (j = 0; j < i; j++)
				if(res->list[j].name[1] != ffc)
					break;

			// azimuths count
			res->anim.azimuths = j;
			// slopes count
			res->anim.slopes = 0;
			// frames count
			res->anim.frames = res->tot_sprites / j;
			// make list of azimuths
			res->anim.lists.resize(res->anim.azimuths);
			// for each azimuth:
			for (int azid = 0; azid < res->anim.azimuths; azid++)
			{
				res->anim.lists[azid].resize(res->anim.frames);
				for (int k = 0; k < res->anim.frames; k++)
					res->anim.lists[azid][k] = &res->list[azid + k*res->anim.azimuths];
			}
			// make default fire azimuths
			res->anim.fire_origin.assign(res->anim.azimuths,{0,0});			
		}
		else
		{
			// it's mechanical unit (has multiple terrain slope variants)

			// count azimuths
			char ffc = res->list[0].name[0];
			for(j=0;j<i;j++)
				if(res->list[j].name[0] != ffc)
					break;

			// sprites in the static group (name starts with F__) - azimuths
			res->stat.azimuths = j;
			// single slope only
			res->stat.slopes = res->tot_sprites / j;
			// make list of standing sprites, one for each azimuth		
			for (int sid = 0; sid < res->stat.slopes; sid++)
			{
				res->stat.lists[sid].resize(res->stat.azimuths);
				for(int k = 0; k < res->stat.azimuths; k++)
					res->stat.lists[sid][k] = &res->list[k + sid*res->stat.azimuths];
				// make default fire azimuths
				res->stat.fire_origin[sid].assign(res->stat.azimuths,{0,0});
			}
		}
	}
	
	return(0);
}

// get graphic resource by name
FSU_resource *FSUarchive::GetResource(const char* name)
{
	for (unsigned k = 0; k < m_list.size(); k++)
		if (_strcmpi(m_list[k]->name.c_str(), name) == 0)
			return(m_list[k]);
	return(NULL);
}

// get count of resources
int FSUarchive::GetCount()
{
	return(m_list.size());
}

// save auxiliary data asociated to FSU archive
int FSUarchive::SaveAuxData(std::wstring path)
{
	// create file
	ofstreamext fw(path,ios::out | ios::binary);
	if(!fw.is_open())
		return(1);

	// store last path
	aux_data_path = path;

	// store version string
	const char ver[] = "SpellFSUcontextV1.0";
	fw.write(ver,sizeof(ver));

	// store resource count
	fw.write((int32_t)GetCount());

	// store list of resource names
	for(auto & res : m_list)
		fw.write_str_p16(res->name);
	
	// store resouce data for each:
	for(auto& res : m_list)
	{
		// static data:
		auto &stat = res->stat;
		fw.write((int32_t)stat.slopes);
		for(int sid = 0; sid < stat.slopes; sid++)
		{
			auto &forg = stat.fire_origin[sid];
			fw.write((int32_t)forg.size());
			for(auto & aorg : forg)
			{
				fw.write((int32_t)aorg.x);
				fw.write((int32_t)aorg.y);
			}
			fw.write((int32_t)stat.fire_center[sid].x);
			fw.write((int32_t)stat.fire_center[sid].y);
		}

		// animation data:
		auto& anim = res->anim;
		fw.write((int32_t)anim.fire_origin.size());
		for(auto& aorg : anim.fire_origin)
		{
			fw.write((int32_t)aorg.x);
			fw.write((int32_t)aorg.y);
		}
		fw.write((int32_t)anim.fire_center.x);
		fw.write((int32_t)anim.fire_center.y);
	}

	// close file
	fw.close();

	return(0);
}

// save auxiliary data asociated to FSU archive
int FSUarchive::LoadAuxData(std::wstring path)
{
	// create file
	ifstreamext fr(path,ios::in | ios::binary);
	if(!fr.is_open())
		return(1);

	// store last path
	aux_data_path = path;

	// check version
	const char ver_ref[] = "SpellFSUcontextV1.0";
	char ver[sizeof(ver_ref)];
	fr.read(ver,sizeof(ver_ref));
	if(memcmp(ver,ver_ref,sizeof(ver_ref)))
	{
		fr.close();
		return(1);
	}

	// get aux resouce count
	uint32_t count = fr.read_u32();

	// load list of resources
	vector<string> names;
	for(int uid = 0; uid < count; uid++)
	{
		// resource name
		string name = fr.read_str_p16();
		names.push_back(name);
	}

	// load aux data for each unit graphic resource
	for(auto & rname : names)
	{
		// try to find matching resource
		auto *res = GetResource(rname.c_str());

		// load statics
		int stat_slopes = fr.read_i32();
		for(int sid = 0; sid < stat_slopes; sid++)
		{
			int stat_azims = fr.read_i32();
			for(int aid = 0; aid < stat_azims; aid++)
			{
				int x = fr.read_i32();
				int y = fr.read_i32();
				if(res && sid < res->stat.slopes && aid < res->stat.azimuths)
					res->stat.fire_origin[sid][aid] = FSU_resource::Txy(x,y);				
			}
			int x = fr.read_i32();
			int y = fr.read_i32();
			if(res && sid < res->stat.slopes)
				res->stat.fire_center[sid] = FSU_resource::Txy(x,y);
		}

		// load animations
		uint32_t anim_azims = fr.read_i32();
		for(int aid = 0; aid < anim_azims; aid++)
		{
			int x = fr.read_i32();
			int y = fr.read_i32();
			if(res && aid < res->anim.azimuths)
				res->anim.fire_origin[aid] = FSU_resource::Txy(x,y);
		}		
		int x = fr.read_i32();
		int y = fr.read_i32();
		if(res)
			res->anim.fire_center = FSU_resource::Txy(x,y);
	}

	// close file
	fr.close();

	return(0);
}







// single resource constructor
FSU_resource::FSU_resource()
{
	stat.azimuths = 0;
	stat.slopes = 0;
	stat.lists->clear();
	anim.azimuths = 0;
	anim.frames = 0;	
	anim.lists.clear();
}
// single resource cleaner
FSU_resource::~FSU_resource()
{
	// clear static unit lists
	stat.lists->clear();
	stat.azimuths = 0;
	stat.slopes = 0;
	
	// clear unit animation lists
	anim.lists.clear();
	anim.azimuths = 0;
	anim.frames = 0;

	// clear actual sprite data
	/*for(unsigned k = 0; k < list.size(); k++)
		delete list[k];*/
	list.clear();
}

// get sprite by name
FSU_sprite *FSU_resource::GetSprite(const char *name)
{
	for(auto &item: list)
		if(iequals(item.name, name))
			return(&item);
	return(NULL);
}


// get azimuth index for fiven angle
int FSU_resource::GetAnimAzim(double angle)
{
	if(!anim.azimuths)
		return(0);
	int azim = round((90.0 - angle)/360.0*(double)anim.azimuths);
	while(azim < 0)
		azim += anim.azimuths;
	azim = azim % anim.azimuths;
	return(azim);
}
// get azimuth angle (deg for fiven azimuth index
double FSU_resource::GetAnimAngle(int azim)
{
	if(azim < 0 || azim >= anim.azimuths)
		return(0.0);
	double angle = 90.0 - (double)azim*360.0/(double)anim.azimuths;
	return(angle);
}
// get azimuth index for fiven angle
int FSU_resource::GetStaticAzim(double angle)
{
	if(!stat.azimuths)
		return(0);
	int azim = round((90.0 - angle)/360.0*(double)stat.azimuths);
	while(azim < 0)
		azim += stat.azimuths;
	azim = azim % stat.azimuths;
	return(azim);
}
// get azimuth angle (deg for fiven azimuth index
double FSU_resource::GetStaticAngle(int azim)
{
	if(azim < 0 || azim >= stat.azimuths)
		return(0.0);	
	double angle = 90.0 - (double)azim*360.0/(double)stat.azimuths;
	return(angle);
}

// get azimuth index for fiven angle using fire angles
int FSU_resource::GetStaticFireAzim(int slope,double angle)
{
	if(stat.slopes == 1)
		slope = 0;
	auto &origins = stat.fire_origin[slope];
	double angle_min = 1e9;
	int azimuth_id = 0;
	auto xy0 = GetStatFireOriginMean(slope);
	for(int aid = 0; aid < stat.azimuths; aid++)
	{
		auto &org = origins[aid];
		double dx = org.x - xy0.x;
		double dy = org.y - xy0.y;
		double da = abs(atan2(-dy,dx)*180.0/M_PI - angle);
		if(da < angle_min)
		{
			angle_min = da;
			azimuth_id = aid;
		}
	}
	return(azimuth_id);
}
// get azimuth index for fiven angle using fire angles
int FSU_resource::GetAnimFireAzim(double angle)
{
	auto& origins = anim.fire_origin;
	double angle_min = 1e9;
	int azimuth_id = 0;
	auto xy0 = GetAnimFireOriginMean();
	for(int aid = 0; aid < anim.azimuths; aid++)
	{
		auto& org = origins[aid];
		double dx = org.x - xy0.x;
		double dy = org.y - xy0.y;
		double da = abs(atan2(-dy,dx)*180.0/M_PI - angle);
		if(da < angle_min)
		{
			angle_min = da;
			azimuth_id = aid;
		}
	}
	return(azimuth_id);
}

// get mean origin of firing
FSU_resource::Txy FSU_resource::GetStatFireOriginMean(int slope)
{
	if(slope >= stat.slopes)
		return(Txy(0,0));
	int x = 0;
	int y = 0;
	for(auto & org : stat.fire_origin[slope])
	{
		x += org.x;
		y += org.y;
	}
	x = x/stat.azimuths;
	y = y/stat.azimuths;		
	return(Txy(x,y));
}

// get mean origin of firing
FSU_resource::Txy FSU_resource::GetAnimFireOriginMean()
{
	int x = 0;
	int y = 0;
	int count = 0;
	for(auto& org : anim.fire_origin)
	{
		x += org.x;
		y += org.y;
		count++;
	}
	x = x/count;
	y = y/count;
	return(Txy(x,y));
}


// FSU sprite constructor
FSU_sprite::FSU_sprite()
{
	name.clear();
	not_decoded = true;
	y_size = 0;
	y_ofs = 0;
	x_min = 0;
	x_max = 0;
	len = 0;
}
// FSU sprite desctructor
FSU_sprite::~FSU_sprite()
{	
}

// render sprite to target buffer, buffer is sprite origin, x_size is buffer width
void FSU_sprite::Render(uint8_t* buffer, uint8_t* buf_end, int buf_x_pos, int buf_y_pos, int buf_x_size,uint8_t* shadow_filter,uint8_t* filter,int zoom)
{
	// source data
	uint8_t* data = this->data.data();

	// initial xy-position
	uint8_t *dest = &buffer[buf_x_pos + (buf_y_pos + (y_ofs - 128)*zoom) * buf_x_size];
	// check valid write range
	if (dest < buffer)
		return;
		
	for (int y = 0; y < y_size; y++)
	{
		// line x-offset
		int offset = *(int*)data; data += sizeof(int);
		// pixels count
		int count = *(int*)data; data += sizeof(int);

		uint8_t *line_data = data;
		for(int z = 0; z < zoom; z++)
		{
			// init line data position
			data = line_data;
			// initial pixel in destination buffer
			uint8_t* scan = &dest[offset*zoom];
			// check safe range
			if (scan + count*zoom > buf_end)
				break;
			// render line
			if(filter)
			{
				for(int x = 0; x < count; x++)
				{
					if(*data == 0xFD && shadow_filter)
					{
						// shadow it is - convert original color using filter
						//*scan = filter[shadow_filter[*scan]];
						*scan = shadow_filter[*scan];
					}
					else if(*data)
					{
						// normal visible pixel
						*scan = filter[*data];
					}
					data++;
					scan++;
				}
			}
			else
			{
				for (int x = 0; x < count; x++)
				{
					for(int zx = 0; zx < zoom; zx++)
					{
						if(*data == 0xFD && shadow_filter)
						{
							// shadow it is - convert original color using filter
							*scan = shadow_filter[*scan];
						}
						else if (*data)
						{
							// normal visible pixel
							*scan = *data;
						}
						scan++;
					}
					data++;
				}
			}
			// move to next buffer line
			dest += buf_x_size;
		}		
	}
}

