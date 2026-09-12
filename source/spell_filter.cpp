//=============================================================================
// Spellcross palette filter stuff.
// 
// This code is part of Spellcross Map Editor project.
// (c) 2022, Stanislav Maslan, s.maslan@seznam.cz
// url: https://github.com/smaslan/spellcross-map-edit
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================
#include "spell_filter.h"

//#include <filesystem>
#include <fstream>

using namespace std;

// create filter record
SpellFilterRec::SpellFilterRec(uint8_t* data,string name)
{
    this->name = name;
    memcpy(filter,data,256);
}
// make default filter
SpellFilterRec::SpellFilterRec(string name)
{
    this->name = name;
    for(unsigned i = 0; i <= 255; i++)
		filter[i] = (uint8_t)i;
}
// craete custom filter from palette and [R,G,B] weights <0.0;2.0>
void SpellFilterRec::SetFilter(uint8_t* pal,string name,double red,double green,double blue)
{	
	// make list of desired colors
	uint8_t color[256][3];
	for(int k = 0; k < 256; k++)
	{
		color[k][0] = min((unsigned)(min(red,1.0)*pal[k*3 + 0] + 100.0*max(red-1.0,0.0)),255u);
		color[k][1] = min((unsigned)(min(green,1.0)*pal[k*3 + 1] + 100.0*max(green-1.0,0.0)),255u);
		color[k][2] = min((unsigned)(min(blue,1.0)*pal[k*3 + 2] + 100.0*max(blue-1.0,0.0)),255u);
	}
	
	// make default noll filter
	for(unsigned k = 0; k < 256; k++)
		filter[k] = k;

	// look for closest available color in input palette (that's how spellcross filters work)
	for(int k = 0; k < 256; k++)
	{
		if(k >= 240 && k < 250)
			continue; // skip water cycle.pal

		int best_dev = 1<<30;
		unsigned best_id  = k;
		for(int m = 0; m < 256; m++)
		{
			if(m >= 240 && m < 250)
				continue; // skip water cycle.pal
			
			int dev = 0;
			for(int c = 0; c < 3; c++)
				dev += abs((int)color[k][c] - (int)pal[m*3 + c])*abs((int)color[k][c] - (int)pal[m*3 + c]);
			if(dev < best_dev)
			{
				best_dev = dev;
				best_id = m;
			}
		}
		filter[k] = best_id;
	}
}

// save filter to Spellcross filter file (256 bytes long)
int SpellFilterRec::SaveFilter(wstring& path)
{
	ofstream fw(path,ios::out | ios::binary);
	if(!fw.is_open())
		return(1);
	fw.write((char*)filter, sizeof(filter));
	fw.close();
	return(0);
}


// make default void filter
SpellFilters::SpellFilters()
{
    // make void filter as first item in the list    
	list.emplace_back(new SpellFilterRec("nullfilter"));
	nullpal = list.back()->filter;

	// make temp filter (for generation of new ones)
	list.emplace_back(new SpellFilterRec("New Filter *"));
	temppal = list.back()->filter;
	
	// default fast links
	darkpal = nullpal;
	darkpal2 = nullpal;
	darker = nullpal;
	redpal = nullpal;
	bluepal = nullpal;
	dbluepal = nullpal;
	goldpal = nullpal;
}
// cleanup filters
SpellFilters::~SpellFilters()
{
	for(auto & filter : list)
		delete filter;
	list.clear();
}

// add new filter to the list
void SpellFilters::AddFilter(uint8_t* data,const char* name)
{
    // add new iterm hoping size of data is 256
	auto filter = new SpellFilterRec(data,string(name));	
	auto item = list.insert(list.begin()+1,filter);
	
	// try assign fast link
	if(!_strcmpi(name,"darkpal.pal"))
		darkpal = filter->filter;
	else if(!_strcmpi(name,"darkpal2.pal"))
		darkpal2 = filter->filter;
	else if(!_strcmpi(name,"darker.pal"))
		darker = filter->filter;
	else if(!_strcmpi(name,"bluepal.pal"))
		bluepal = filter->filter;
	else if(!_strcmpi(name,"dbluepal.pal"))
		dbluepal = filter->filter;
	else if(!_strcmpi(name,"redpal.pal"))
		redpal = filter->filter;
	else if(!_strcmpi(name,"goldpal.pal"))
		goldpal = filter->filter;
}

// get temp work filter
SpellFilterRec* SpellFilters::GetTempFilter()
{
	return(list.back());
}
