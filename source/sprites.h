//=============================================================================
// Unsorted Spellcross data handling routines.
// 
// This code is part of Spellcross Map Editor project.
// (c) 2021, Stanislav Maslan, s.maslan@seznam.cz
// url: https://github.com/smaslan/spellcross-map-edit
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================
//#pragma once
#ifndef _SPRITES_H_
#define _SPRITES_H_

#include "cstdint"
#include <fstream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <thread>

#include "other.h"

#include "wx/dcbuffer.h"


#define MAX_STR 256
#define MAX_SPRITE_NAME 8

class SpellSound;
class SpellSounds;
class SpellPalette;
class SpellGraphicItem;
class SpellGraphics;
class SpellFilters;
class SpellFont;
class AnimPNM;
class FSarchive;


class SpellL2classRec
{
public:
	std::string tag;
	std::string label;
	std::string name;
	int index;
	int defence;
	int hp;
	int sound_class_id;
	SpellSound* sound_hit;
	SpellSound* sound_destruct;
	SpellL2classRec();
	~SpellL2classRec();
};

class DestructibleRec
{
public:
	DestructibleRec();
	SpellL2classRec* destructible;
	std::string pnm_name;
	int target;
	int destructed;
	int two_stage_desctruct;
	std::string alt_name;
};

class SpellL2classes
{
private:
	std::vector<SpellL2classRec*> wall_list;
	std::vector<SpellL2classRec*> bridge_list;
	std::vector<SpellL2classRec*> spec_list;
public:
	SpellL2classes(FSarchive* fs,SpellSounds* sounds,std::function<void(std::string)> status_list=NULL,std::function<void(std::string)> status_item=NULL);
	~SpellL2classes();
	DestructibleRec GetClass(const char *sprite_name);
};




typedef struct {
	double x, y, z;
}TFxyz;

typedef struct {
	int x, y;
}Txy;

class Txyz {
public:
	int x,y,z;
	Txyz(int xx,int yy,int zz) {x=xx;y=yy;z=zz;};
};

class Terrain;

class Sprite
{
	public:
		// owning terrain backref
		Terrain *terr;
		// place holder sprite
		bool is_dummy;
		// marked as to be removed
		bool is_removed;
		// sprite name tag
		std::string name;
		// sprite data
		std::vector<uint8_t> data;
		// sprite size
		int x_size;
		int y_size;
		// sprite offset
		int y_ofs;
		int x_ofs;
		// land type
		int land_type;
		// has transparency
		int has_transp;
		// sprite index in terrain
		int index;
		// destructible parameters (or NULL)
		SpellL2classRec *destructible;
		// alternative-paired sprite when destructible
		Sprite *destructible_alt;
		int is_destructed;
		int two_stage_desctruct;
		int is_target;
		AnimPNM *destroy_pnm;
		
		// void constructor
		Sprite(Sprite &source);
		Sprite();
		~Sprite();
		/*Sprite& operator=(const Sprite& other);
		Sprite& operator=(const Sprite* other);*/
		uint8_t* GetPixel(int x,int y);
		uint8_t* GetPixelNeighbor(int x,int y,int dir);
		std::tuple<int,int> GetPixelNeighborXY(int x,int y,int dir);
		int SetPixel(int x, int y,uint8_t color);		
		void SetIndex(int id);
		int GetIndex();
		//void Render(uint8_t* buffer,uint8_t* buf_end,int buf_x,int buf_y, int x_size);
		void Render(std::vector<uint8_t>& buf,int buf_x,int buf_y,int x_size,uint8_t* filter=NULL);
		void Render(uint8_t* buffer, uint8_t* buf_end, int buf_x, int buf_y, int x_size, uint8_t* filter=NULL);
		wxBitmap* Render(uint8_t* pal,double gamma=1.3, int x_size=-1, int y_size=-1, bool no_zoom=true);
		int Decode(uint8_t* data,const char* name);
		static int SaveSprite(std::filesystem::path path,std::vector<uint8_t>& buffer,int x_buf_size,int x_offset,int y_offset,int land_type);
		int ExportInfo(std::filesystem::path path,std::filesystem::path image_name,SpellPalette& palette);
		void GetTileModel(TFxyz* vert, int* face=NULL, int* face_count=NULL,int triangle_faces=false);
		int GetTileEdge(int edge,TFxyz* vert);
		TFxyz ProjectVertex(TFxyz *vert);
		void RenderTileWires(uint8_t* buffer, uint8_t* buf_end, int buf_x, int buf_y, int x_size, uint8_t color);
		char GetSlope();
		
		double GetTileZ(double x, double y);
		double GetTileProjY(double x, double y);

		int CompareSpriteContextAlt(Sprite *alt);
		int CheckShadingValid(Sprite* neig,int eid);
		int CheckNeighborValid(Sprite* neig,int eid);

		// sprite context stuff
		int ReserveContext(int quadrant,int tile,int size);
		int AddContext(int quadrant,Sprite* sprite,bool no_check=false);
		int GetContextCount(int quadrant);
		int GetContextCount(int quadrant, int tile);
		int GetContextCount(int quadrant,int tile,int random);
		int GetContextCountRng(int quadrant,int tile);
		Sprite* GetContext(int quadrant,int index);
		Sprite* GetContext(int quadrant,int tile,int index);		
		Sprite* GetContext(int quadrant,int tile,int index,int random);
		Sprite* GetContextRng(int quadrant,int tile,int index);
		void RemoveContext(int quadrant,int tile,std::vector<int> &list);
		void RemoveContext(int quadrant,std::vector<int>& list);
		int CheckContext(int quadrant,Sprite *sprite);
		int ClearContext(int quadrant=-1);
		int RandomizeContext();
		int ShuffleContext();
		uint32_t SetFlags(uint32_t new_flags);
		uint32_t GetFlags();
		uint32_t SetGlyphFlags(uint32_t new_flags);
		uint32_t GetGlyphFlags();
		void SetEdgeClass(int edge,uint32_t new_class);
		uint32_t GetEdgeClass(int edge);
		uint32_t SetShadingFlags(uint32_t new_flags);
		uint32_t GetShadingFlags();
		uint32_t SetShadingMask(uint32_t mask);
		uint32_t GetShadingMask();
		bool isClassGlyph() { return(!!(flags2 & LandFlags::IS_GLYPH));};
		bool isSideShaded(int side);
		bool isSideShadedMask(int side);
		bool isCornerShaded(int corner);
		uint32_t GetMapFlags() {return(this->map_flags);};
		void SetMapFlags(uint32_t flags) { this->map_flags = flags; };

		int GetSpecClassCount();
		std::string GetSpecClassName(int id);
		void SetSpecClass(uint32_t class_id);
		uint32_t GetSpecClass();

		void SetToolClass(int id);
		void SetToolClassGroup(int id);
		int GetToolClass();
		int GetToolClassGroup();


		static constexpr int MODE_SET = 0;
		static constexpr int MODE_ADD = 1;
		static constexpr int MODE_CLR = 2;

		class LandFlags
		{
		public:
			static constexpr uint32_t IS_GRASS =        0x00000001;
			static constexpr uint32_t IS_DGRASS =       0x00000002;
			static constexpr uint32_t IS_BLOOD =        0x00000004;
			static constexpr uint32_t IS_MUD =          0x00000008;
			static constexpr uint32_t IS_SWAMP =        0x00000010;
			static constexpr uint32_t IS_ASH =          0x00000020;
			static constexpr uint32_t IS_HIGHLAND =     0x00000040;
			static constexpr uint32_t IS_ROAD =         0x00000080;
			static constexpr uint32_t IS_BROKEN =       0x00000100;
			static constexpr uint32_t IS_DIRT_ROAD =    0x00000200;
			static constexpr uint32_t IS_MUD_PATH =     0x00000400;
			static constexpr uint32_t IS_CLIFF =        0x00000800;
			static constexpr uint32_t IS_WATER =        0x00001000;
			static constexpr uint32_t IS_WOOD_BRIDGE =  0x00002000;
			static constexpr uint32_t IS_BRIDGE =       0x00004000;
			static constexpr uint32_t IS_FORD =         0x00008000;
			static constexpr uint32_t IS_SAND =         0x00010000;
			static constexpr uint32_t IS_SHADOW =       0x00020000;
			static constexpr uint32_t IS_RIDGE =        0x00040000;
			static constexpr uint32_t IS_RIDGE_BRIDGE = 0x00080000;
			static constexpr uint32_t IS_WALL_BASE =    0x00100000;
			static constexpr uint32_t IS_OBJECT =       0x00200000;
			static constexpr uint32_t IS_SCAR =         0x00400000;

			static constexpr uint32_t IS_DARKER_SAND =  0x00000002; // PUST darker sand
			static constexpr uint32_t IS_DARK_SAND   =  0x00000010; // PUST darkest sand
			static constexpr uint32_t IS_OIL_SAND    =  0x00000008; // PUST oily sand
			static constexpr uint32_t IS_SAND_PATH   =  0x00000400; // PUST sand path

			static constexpr uint32_t IS_GLYPH =           0x80000000; /* use tile as glyph for general tile class display */
			static constexpr uint32_t IS_TOOL_ITEM_GLYPH = 0x40000000; /* use tile as glyph for tool item */
			static constexpr uint32_t IS_FAULTY =          0x20000000; /* sprite known to have faulty context in existing maps */
			static constexpr uint32_t Q1_NOFILT =          0x01000000; /* not filter sprite context in Q1 */
			static constexpr uint32_t Q2_NOFILT =          0x02000000; /* not filter sprite context in Q2 */
			static constexpr uint32_t Q3_NOFILT =          0x04000000; /* not filter sprite context in Q3 */
			static constexpr uint32_t Q4_NOFILT =          0x08000000; /* not filter sprite context in Q4 */
		};
		
		// special tile classes
		static constexpr uint32_t SPEC_CLASS_NONE = 0;
		static constexpr uint32_t SPEC_CLASS_GRAVE = 1; /* graves of any kind */
		static constexpr uint32_t SPEC_CLASS_RUBBLE1 = 2; /* light rubble */
		static constexpr uint32_t SPEC_CLASS_RUBBLE2 = 3; /* average dense rubble */
		static constexpr uint32_t SPEC_CLASS_RUBBLE3 = 4; /* more dense rubble */
		static constexpr uint32_t SPEC_CLASS_ROCK = 5; /* big rock */
		static constexpr uint32_t SPEC_CLASS_FIREPLACE = 6; /* fireplaces */
		static constexpr uint32_t SPEC_CLASS_SCAR = 7; /* generic texture stuff on the basic ground like scars, high grass and stuff */
		static constexpr uint32_t SPEC_CLASS_OBJECT = 8; /* like buildings, etc. */		
		static constexpr uint32_t SPEC_CLASS_MAX = SPEC_CLASS_OBJECT; /* highest class index */

		class LandClass
		{
		public:
			static constexpr uint32_t GENERIC = 0;
			static constexpr uint32_t GRASS = 1;
			static constexpr uint32_t DARK_GRASS = 2;
			static constexpr uint32_t MUD = 3;
			static constexpr uint32_t SWAMP = 4;
			static constexpr uint32_t ASH = 5;
			static constexpr uint32_t BLOOD = 6;
			static constexpr uint32_t HIGH_LAND = 7;
			static constexpr uint32_t SAND = 8;
			static constexpr uint32_t ROAD = 9;
			static constexpr uint32_t DIRT_ROAD = 10;
			static constexpr uint32_t MUD_PATH = 11;
			static constexpr uint32_t SAND_PATH = 12;
			static constexpr uint32_t DARKER_SAND = 13;
			static constexpr uint32_t DARK_SAND = 14;
			static constexpr uint32_t OILY_SAND = 15;
		};

		static constexpr uint32_t SHADING_SIDE_Q1 = 0x01;
		static constexpr uint32_t SHADING_SIDE_Q2 = 0x02;
		static constexpr uint32_t SHADING_SIDE_Q3 = 0x04;
		static constexpr uint32_t SHADING_SIDE_Q4 = 0x08;
		static constexpr uint32_t SHADING_CORNER_Q1 = 0x10;
		static constexpr uint32_t SHADING_CORNER_Q2 = 0x20;
		static constexpr uint32_t SHADING_CORNER_Q3 = 0x40;
		static constexpr uint32_t SHADING_CORNER_Q4 = 0x80;

		enum {
			NEIGHBOR_INVALID = 0,
			NEIGHBOR_VALID = 1,
			NEIGHBOR_UNDECIDED = 2
		};

		// tile elevation step in pixels
		static constexpr double TILE_ELEVATION = 22.5;

		// tile projection angle (degrees)
		static constexpr double PROJECTION_ANGLE = 37.0;

		class WallParams{
		public:
			int class_id = -1; // matches MURY.DEF numbers
			int type_id = -1; // unique type id per wall type
			bool damage = false; // is damaged?
			bool q[4] = {false,false,false,false}; // continues to quadrants?
		};
		WallParams wall_params;

	private:
		int MaskHasTransp(uint8_t* mask);
		void InitWallParams();
		
		

		// sprite context stuff:
		// quadrant lists of allowed tile neighbors [quadrant, tile_type]
		std::vector<Sprite*> quad[4]['M'-'A'+1];
		uint32_t shading;
		uint32_t flags;
		uint32_t flags2;
		uint32_t edge_class[4];
		uint32_t spec_class;
		uint32_t map_flags; // map L2 flags

		// randomized context
		std::vector<std::vector<Sprite*>> quad_rng[4]['M'-'A'+1];

		// tool classes 
		uint32_t tool_class;
		uint32_t tool_group;
};



class AnimL1
{
	public:
		// animation name
		char name[MAX_SPRITE_NAME + 1];
		// frames list
		std::vector<Sprite*> frames;
		// sprite size
		int x_size;
		int y_size;
		// sprite vertical offset
		int y_ofs;

		// void constructor
		AnimL1();
		~AnimL1();
		int Decode(uint8_t* data, char* name);

};

class AnimPNM
{
	public:
		// placeholder item
		bool is_dummy;
		// animation name
		std::string name;
		// frames list
		std::vector<Sprite*> frames;
		// sprite data limits relative to origin defined by frames
		int x_min;
		int x_max;
		int y_min;
		int y_max;
		// order id
		int index;
								
		// void constructor
		AnimPNM();
		~AnimPNM();
		int Decode(uint8_t* data, const char* name);
		static int Encode(std::filesystem::path path,std::vector<std::unique_ptr<SpellGraphicItem>>& frames);

};

class MapSprite;
class MapLayer4;
class MapXY;

// spellcross terrain object (group of sprites forming e.g. house)
#ifndef MINIMAL_SPRITES
class SpellObject
{
private:	
	// item description
	std::string description;
	// sprite positions
	std::vector<MapXY> sprite_pos;
	// sprite pointers
	std::vector<Sprite*> L1_sprites;
	std::vector<Sprite*> L2_sprites;
	std::vector<uint8_t> flags;
	std::vector<MapLayer4> L4_list;
	// bitmap data
	int surf_x;
	int surf_y;
	std::vector<uint8_t> pic;	
	// tool classes 
	uint32_t tool_class;
	uint32_t tool_group;

	double last_gamma;
	uint8_t pal[256][3];
	uint8_t gamma_pal[256][3];

	int RenderObjectGlyph();

public:

	// vritual tool created on startup not to be stored to context file
	bool is_virtual;
	// all object components are available in loaded data
	bool is_valid;
		
	enum GLYPH_FORMAT{		
		INDEX_8BIT = 0,
		LZ_INDEX_8BIT,
		NONE
	};
	
	SpellObject(ifstreamext& fr,std::vector<Sprite*>& sprite_list,std::vector<AnimPNM*>& pnm_list,uint8_t* palette);
	SpellObject(std::vector<MapXY>& xy,std::vector<Sprite*>& L1_list,std::vector<Sprite*>& L2_list,std::vector<uint8_t>& flag_list,std::vector<MapLayer4>& pnm_list,uint8_t* palette=NULL,std::string desc="");	
	~SpellObject();
	int RenderPreview(wxBitmap& bmp,double gamma=1.30);
	std::tuple<int, int> GetGlyphSize();
	wxBitmap *RenderPreview(double gamma = 1.30, int x_size = -1, int y_size = -1, bool no_zoom=true);
	int WriteToFile(ofstreamext& fw);
	std::string GetDescription();
	void SetDescription(std::string name);
	int GetObjectData(std::vector<MapXY> *pos, std::vector<MapSprite> *tiles, std::vector<MapLayer4> *pnm_list);
	bool Compare(SpellObject *obj,bool exact_pnm=true);

	void SetToolClass(int id);
	void SetToolClassGroup(int id);
	int GetToolClass();
	int GetToolClassGroup();
};


class SpellToolsGroup
{
private:
	
public:
	std::vector<std::string> items;
	std::string name;
	std::string title;
	int glyph_x;
	int glyph_y;
	int glyph_mode;

	static constexpr int SCALE_MEAN = 0;
	static constexpr int SCALE_MAX = 1;
	static constexpr int SCALE_MAX_NOZOOM = 2;
};


// editing tool selection pointer (can point to class/tools (sprite) or objects)
class SpellTool
{
private:
	SpellObject* obj;
	int class_id;
	int tool_id;
public:
	SpellTool() { obj = NULL; class_id = -1; tool_id = -1; };
	void Set(int class_id,int tool_id) { this->class_id = class_id; this->tool_id = tool_id; this->obj = NULL; };
	void Set() { this->class_id = -1; this->tool_id = -1; this->obj = NULL; };
	void Set(SpellObject* obj) { this->class_id = -1; this->tool_id = -1; this->obj = obj; };
	bool isObject() { return(!!obj); };
	bool isTool() { return(class_id >=0 && tool_id >= 0); };
	bool isActive() { return(isObject() || isTool()); };
	SpellObject* GetObject() { return(obj); };
	std::tuple<int,int> GetTool() { return(std::make_tuple(class_id,tool_id)); };
	bool isSame(SpellTool &tool) {return(obj == tool.obj && class_id == tool.class_id && tool_id == tool.tool_id);};
};
#else
class SpellObject;
class SpellTool;
class SpellToolsGroup;
#endif


class SpellData;

class Terrain
{
private:
	SpellData &m_spell_data;

	//double gamma;
	double last_gamma;
	uint8_t gamma_pal[256][3];
	// sprite context file path
	std::wstring context_path;
	// list of generic tile glyph pointers
	std::vector<Sprite*> glyphs[13];

	// tools list
	std::vector<SpellToolsGroup*> tools;

	void RandomizeSpriteContextTh(std::vector<Sprite*> *spr,int ofs,int step);
	int SwapToolSetItems(int toolset_id,int posa,int posb);
	int SwapToolSets(int posa,int posb);

	//int CheckNeighborValid(Sprite *ref,Sprite *neig,int eid);	

	

public:

	static constexpr int RENDER_CENTER = 0x01;
	static constexpr int RENDER_ZOOMX2 = 0x02;

	// terrain name
	std::string name;
	// sprites of particular layers
	std::vector<Sprite*> sprites;
	// sprites context
	//SpriteContext* context;
	// Layer 1 animations (ANM)
	std::vector<AnimL1*> anms;
	// Layer 4 animations (PNM)
	std::vector<AnimPNM*> pnms;
	// terrain objects list
	std::vector<SpellObject*> objects;
	// color palette
	uint8_t pal[256][3];
	// filters
	std::unique_ptr<SpellFilters> filter;
	// fonts
	SpellFont *font;
	SpellFont *font7;

	enum WildMode { FIRST,RANDOM };

	// void contructor
	Terrain(SpellData &spell_data);
	~Terrain();
	int Load(FSarchive *terrain_fs, uint8_t map_pal[][3], SpellGraphics *gres, SpellL2classes* L2=NULL,std::function<void(std::string)> status_item=NULL);
	Sprite* GetSprite(const char* name);
	Sprite* GetSprite(int index);
	int GetSpriteID(Sprite *spr);
	int GetSpriteID(const char* name);
	int GetSpriteCount();
	Sprite* GetSpriteWild(const char* wild,WildMode mode);
	AnimL1* GetANM(std::string name);
	AnimL1* GetANM(const char* name);
	AnimPNM* GetPNM(const char* name);
	AnimPNM* GetPNM(std::string name);

	class SpriteFlag{
	public:
		uint32_t flag;
		std::string name;
	};
	std::vector<SpriteFlag> GetSpriteFlagList();
		
	int InitSpriteContext(std::filesystem::path& path);
	int SaveSpriteContext(std::wstring& path);	
	std::wstring &GetSpriteContextPath();
	void ClearSpriteContext();
	int RandomizeSpriteContext();
	int ShuffleSpriteContext();
	int UpdateSpriteContext(std::function<void(std::string)> status_cb=NULL);
	int InitSpriteContextShading();
	int FixSpriteLandTypes();
	int InitSpriteMapTileFlags();
	int GenWallNeighbors();
	std::vector<Sprite*> FindWallSprites(int type_id,int* edges,bool damage,int min_match=1,bool rand=false);
	int UpdateTileGlyphs();
	Sprite *GetTileGlyph(Sprite *sprite,uint32_t flags = 0);

	int RenderPreview(wxBitmap& bmp,int count,int* data,int flags,double gamma);
	int RenderSpritePreview(wxBitmap& bmp,std::vector<Sprite*>& tiles,int flags,double gamma);
	int RenderSpritePreview(wxBitmap& bmp,Sprite* tile,int flags,double gamma);
	int RenderPNMpreview(wxBitmap& bmp,Sprite* spr,int flags=RENDER_CENTER,double gamma=1.0);
		
	SpellObject* AddObject(std::vector<MapXY> xy,std::vector<Sprite*> L1_list,std::vector<Sprite*> L2_list,std::vector<uint8_t> flag_list,std::vector<MapLayer4> pnm_list,uint8_t* palette,std::string desc);
	SpellObject* AddObject(SpellObject* obj);
	bool CheckObjectDuplicates(SpellObject* obj,bool exact_pnm=true);
	int RemoveObject(int id);
	int MoveObject(int posa, int posb);
	int RenameObject(int id, std::string name);
	int GetObjectsCount();
	SpellObject *GetObject(int id);
	std::vector<SpellObject*> &GetObjects();
	std::vector<std::string> GetObjectNames(int toolset_id=-1);
	int FindObject(SpellObject* obj);
	
	int GetToolsCount();
	std::vector<std::string> GetToolSetNames();
	std::string GetToolSetName(int id);
	std::string GetToolSetTitle(int id);
	int SetToolSetName(int id,std::string name);
	int SetToolSetTitle(int id,std::string title);
	int GetToolSetGlyphScalingMode(int id);
	int SetToolSetGlyphScalingMode(int id, int mode);
	std::tuple<int, int> GetToolSetGlyphScaling(int id);
	int SetToolSetGlyphScaling(int id, int x, int y);
	int GetToolSetItemsCount(int id);
	int GetToolSetItem(int toolset_id,std::string tool_name);
	std::vector<std::string> GetToolSetItems(int toolset_id);
	std::string GetToolSetItem(int toolset_id, int tool_id);	
	int AddToolSetItem(int toolset_id,std::string& item);
	int AddToolSetItem(int toolset_id,std::string &item, int &position);
	int RenameToolSetItem(int toolset_id,std::string item, int position);
	int RemoveToolSetItem(int toolset_id, int position);
	int MoveToolSetItem(int toolset_id,int posa,int posb,bool insert=false);	
	int AddToolSet(std::string& name,std::string title);
	int AddToolSet(std::string &name,std::string title,int &position);
	int RemoveToolSet(int position);
	int MoveToolSet(int posa, int posb,bool insert=false);
	int GetToolSetID(std::string& name);
	int GetToolSetID(const char *name);
	int ExportToolSetInfo(std::filesystem::path info_path, int id);
	wxBitmap* RenderToolSetItemImage(int tool_id, int item_id, double gamma=1.30, int x_size=-1, int y_size=-1, bool no_zoom=true);
	std::tuple<int, int> GetToolSetItemImageSize(int tool_id, int item_id);

	std::vector<Sprite*> GetToolSprites(SpellTool &tool);
	std::vector<SpellObject*> GetToolObjects(SpellTool& tool);
	int AddSpecialTools();


	int RenderPalette(wxBitmap& bmp,uint8_t* filter=NULL,int relative_time=0);
	int RenderPaletteColor(wxBitmap& bmp,int x_size,int x_pos,uint8_t* filter=NULL);
	

};




#endif