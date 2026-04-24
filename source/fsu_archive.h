//=============================================================================
// class FSUarchive - Loading Spellcross FSU units graphics resource archive.
// It loads all resources from FSU, decompresses LZW compressed sprites
// and stores the sprites to lists. IT can take some time to decode'em as there 
// are a lot of them (>20000).
// 
// This code is part of Spellcross Map Editor project.
// (c) 2021, Stanislav Maslan, s.maslan@seznam.cz
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================
#ifndef fsu_archiveH
#define fsu_archiveH
//---------------------------------------------------------------------------
#include "LZ_spell.h"
//#include "sprites.h"
#include <vector>
#include <fstream>
#include <string>
#include <functional>

class FSU_sprite
{
public:
	std::string name;
	int y_size;
	int y_ofs;
	// used range of pixels
	int x_min;
	int x_max;
	// sprite data
	std::vector<uint8_t> data;
	int len;
	bool not_decoded;

	FSU_sprite();
	~FSU_sprite();
	void Render(uint8_t* buffer, uint8_t* buf_end, int buf_x_pos, int buf_y_pos, int buf_x_size, uint8_t* shadow_filter,uint8_t* filter=NULL,int zoom=1);
};

class FSU_resource
{
public:
	// unit tag name
	std::string name;
	// unit data offset in FSU
	uint32_t offset;
	// total sprites count
	uint16_t tot_sprites;
	// sprite list
	std::vector<FSU_sprite> list;

	class Txy{
	public:
		int x;
		int y;
		Txy() {x = 0; y = 0;};
		Txy(int xx, int yy) {x=xx;y=yy;};
	};
	
	// --- sorted sprites groups (standing, moving, animation, ...)
	struct {
		// slopes available (only for tanks, otherwise 1)
		int slopes;
		// azimuths available
		int azimuths;
		// standing unit (flesh ones) or tanks (no animations) [slope][azimuth]
		std::vector<FSU_sprite*> lists['M' - 'A' + 1];
		// fire origin [slope][azimuth]
		std::vector<Txy> fire_origin['M' - 'A' + 1];
		// turret (unit) center for fire azimuth calculation
		Txy fire_center['M' - 'A' + 1];
	} stat;
	
	struct {
		// frames count
		int frames;
		// azimuths count
		int azimuths;
		int slopes; /* alternative to azimuths, used only for hell cavalery */
		// lists of sprites [azimuth/slope][frame]
		//FSU_sprite*** lists;
		std::vector<std::vector<FSU_sprite*>> lists;
		// fire origin [azimuth/slope]
		std::vector<Txy> fire_origin;
		// turret (unit) center for fire azimuth calculation
		Txy fire_center;
	} anim;	

	FSU_sprite* GetSprite(const char* name);
	Txy GetStatFireOriginMean(int slope);
	Txy GetAnimFireOriginMean();
	int GetAnimAzim(double angle);
	double GetAnimAngle(int azim);
	int GetStaticAzim(double angle);
	double GetStaticAngle(int azim);
	int GetStaticFireAzim(int slope,double angle);
	int GetAnimFireAzim(double angle);

	FSU_resource();
	~FSU_resource();
};


class FSUarchive{	
	public:
		int xmin;
		int xmax;
		int ymin;
		int ymax;
		std::string m_last_error;

		// loader options
		enum Options: int
		{
			NONE = 0,
			NO_DECODE // do not decode sprites, load just raw compressed data
		};		
		friend Options operator|(Options lhs,Options rhs) {return static_cast<Options>(static_cast<std::underlying_type_t<Options>>(lhs) |	static_cast<std::underlying_type_t<Options>>(rhs));}
		friend Options operator&(Options lhs,Options rhs) {return static_cast<Options>(static_cast<std::underlying_type_t<Options>>(lhs) & static_cast<std::underlying_type_t<Options>>(rhs));}		
		/*friend bool operator!=(Options lhs,int rhs) { return(static_cast<std::underlying_type_t<Options>>(lhs) != rhs); }
		explicit Options::operator bool() const { return(static_cast<std::underlying_type_t<Options>>(lhs) != 0); }*/
		
		int GetCount();
		std::wstring GetAuxDataPath() {return(aux_data_path);};

		FSUarchive();
		FSUarchive(std::wstring path,Options options=Options::NONE,std::function<void(std::string)> status_item=NULL);
		int LoadFolder(std::filesystem::path dir,std::string wild_filter="*");
		int Save(std::filesystem::path path,bool allow_overwrite=false);
		~FSUarchive();
		FSU_resource* GetResource(const char* name);
		int AddResource(FSU_resource* res);
		int SaveAuxData(std::wstring path);
		int LoadAuxData(std::wstring path);
		bool Compare(FSUarchive *ref);
		std::vector<std::string> GetResourceNanes();

	private:
		// graphics resource list
		std::vector<FSU_resource*> m_list;

		int LoadResource(uint8_t* data,int rid,FSU_resource* res,LZWexpand* delz);
		int LoadResourceGroup(uint8_t* data,int first,int count,int step,std::vector<FSU_resource*>* list,Options options=Options::NONE,std::function<void(std::string)> status_item=NULL);
		
		std::wstring aux_data_path;
};


#endif

