//=============================================================================
// Spellcross palette filter stuff.
// 
// This code is part of Spellcross Map Editor project.
// (c) 2022, Stanislav Maslan, s.maslan@seznam.cz
// url: https://github.com/smaslan/spellcross-map-edit
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================
#pragma once

#include "cstdint"
#include <vector>
#include <string>

using namespace std;

class SpellFilterRec
{
public:
	uint8_t filter[256];
	string name;

	SpellFilterRec(string name);
	SpellFilterRec(uint8_t *data, string name);
	void SetFilter(uint8_t* pal,string name,double red,double green,double blue);
	int SaveFilter(wstring &path);
};

class SpellFilters
{
public:
	vector<SpellFilterRec*> list;

	uint8_t* temppal;
	uint8_t* nullpal;
	uint8_t* darkpal;
	uint8_t* darkpal2;
	uint8_t* darker;
	uint8_t* redpal;
	uint8_t* bluepal;
	uint8_t* dbluepal;
	uint8_t* goldpal;
	
	SpellFilters();
	~SpellFilters();
	void AddFilter(uint8_t* data, const char *name);
	SpellFilterRec *GetTempFilter();
};

