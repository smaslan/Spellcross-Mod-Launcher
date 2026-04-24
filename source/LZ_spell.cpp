//=============================================================================
// LZW decoder/encoder for Spellcross. It is almost standard LZW, 
// but there are few changes. It is quite nasty code, but seems to work.
// It keeps dictionary buffer and local output data buffer allocated so
// it can be reused for many files, which is essential for decoding FSU 
// archive with 20000+ sprites.
// 
// This code is part of Spellcross Map Editor project.
// (c) 2021-2025, Stanislav Maslan, s.maslan@seznam.cz
// url: https://github.com/smaslan/spellcross-map-edit
// Distributed under MIT license, https://opensource.org/licenses/MIT.
//=============================================================================
#include "LZ_spell.h"

#include <cstdint>
#include <stdexcept>
//---------------------------------------------------------------------------

// get u8 from stream
uint8_t gu8(uint8_t **p)
{
	uint8_t b = *((uint8_t*)(*p));
	(*p)++;
	return(b);
}

// get u16 from stream
uint16_t gu16(uint8_t**p)
{
	uint16_t w = *((uint16_t*)(*p));
	*p = (uint8_t*)(((uint16_t*)*p) + 1);
	return(w);
}

//---------------------------------------------------------------------------
// LZ dictionary - constructor
//---------------------------------------------------------------------------
LZWdct::LZWdct()
{
	data_used = 0;
	idict_len = 0;
	cdict_len = 0;
	data = new uint8_t[DMLEN * DMLEN];
}

int LZWdct::Load(uint8_t **src, uint8_t *end)
{
	int i;

	// check parameters
	if(!src || !end)
	{
		return(1);
	}
	
	// --- initial dictionary ---

	// dictionary len
	int len = gu16(src);

	// item len
	ibw = gu8(src);

	// get initial dictionary
	for (i = 0; i < len; i++)
	{
		data[i] = (len & 0x100) ? i : gu8(src);
		idict[i].len = 1;
		idict[i].ofs = i;		
	}
	idict_len = len;
	data_used = len;

	// set current dictionary
	LZWreinitDict();

	return(0);
}

//---------------------------------------------------------------------------
// copy initial dictionary to current one
//---------------------------------------------------------------------------
int LZWdct::LZWreinitDict()
{
	// copy initial to current dictionary
	std::memcpy((void*)cdict, (void*)idict, idict_len*sizeof(LZWdictElem));
	cdict_len = idict_len;
	data_used = idict_len;

	// set current bitstream width
	cbw = ibw;

	return(0);
}

//---------------------------------------------------------------------------
// LZ dictionary destructor
//---------------------------------------------------------------------------
LZWdct::~LZWdct(void)
{
	data_used = 0;
	idict_len = 0;
	cdict_len = 0;
	delete[] data;
}

//---------------------------------------------------------------------------
// LZ dictionary - add item
// Adds combination of item A with first byte from item B
//---------------------------------------------------------------------------
int LZWdct::ins(LZWdictElem *ita, LZWdictElem *itb)
{
	if(ita==NULL || itb==NULL)
		return(1);

	// copy A
	cdict[cdict_len].len = ita->len + 1;
	cdict[cdict_len].ofs = data_used;	
	std::memcpy((void*)&data[data_used], (void*)&data[ita->ofs], ita->len);
	data_used += ita->len;
	// copy first symbol of B
	data[data_used++] = data[itb->ofs];
	// update current dict size
	cdict_len++;

	// update stream width
	if (cdict_len + 1 >= (1 << cbw))
		cbw++;

	return(0);
}

//---------------------------------------------------------------------------
// LZ dictionary - is item clear code?
//---------------------------------------------------------------------------
int inline LZWdct::isclr(int code)
{
	// ###todo: this should be maybe derived from current dict instead of initial???
	return(code == idict_len);
}


//---------------------------------------------------------------------------
// LZ dictionary - get dictionary item or empty if error
//---------------------------------------------------------------------------
LZWdictElem *LZWdct::get(int code)
{
	if (code >= cdict_len)
	{
		// failed
		return(NULL);
	}
	
	// return poitner to data of requested item
	cdict[code].data = &data[cdict[code].ofs];
	return(&cdict[code]);	
}




//---------------------------------------------------------------------------
// LZW decoder
//---------------------------------------------------------------------------

// construct with initial output buffer size
LZWexpand::LZWexpand(int buf_size)
{	
	// allocate initial output buffer (this will exist until class destroyed and grow whenever needed)
	buffer.resize(buf_size);
}

// close LZW decoder
LZWexpand::~LZWexpand()
{
}

//---------------------------------------------------------------------------
// LZ - get word form bitstream
// returns -1 if data end reached
//---------------------------------------------------------------------------
int LZWexpand::GetWord(uint8_t** dsrc, uint8_t* dend)
{
	static const uint32_t mskl[12] = { 0x80000000u,0xC0000000u,0xE0000000u,0xF0000000u,
									   0xF8000000u,0xFC000000u,0xFE000000u,0xFF000000u,
									   0xFF800000u,0xFFC00000u,0xFFE00000u,0xFFF00000u };

	// reset bitstream
	if (!dsrc)
	{
		bofs = 7;
		return(0);
	}

	// get word from stream
	uint32_t msk = mskl[dct.cbw - 1] >> (7 - bofs);
	union {
		uint32_t u32;
		uint8_t u8[4];
	}dwrd;
	uint8_t* wdat = (uint8_t*)(*dsrc);
	// bswap	
	dwrd.u8[3] = *wdat++;
	dwrd.u8[2] = *wdat++;
	dwrd.u8[1] = *wdat++;
	dwrd.u8[0] = *wdat++;
	// mask and align to LSB
	dwrd.u32 = (dwrd.u32 & msk) >> (25 + bofs - dct.cbw);

	//new offset
	bofs -= dct.cbw;
	while (bofs < 0)
	{
		if (*dsrc >= dend - 1)
			return(-1);
		bofs += 8;
		(*dsrc)++;
	}

	return(dwrd.u32);
}


// decode LZW data
int LZWexpand::Decode(uint8_t* dsrc,uint8_t* dend,uint8_t** dest,int* dlen)
{
	if(!dest || !dlen)
		return(1);
	
	// deLZ
	auto err = DecodeCore(dsrc,dend);
	if(err)
		return(err);

	// make and copy local data to output buffer
	*dlen = output_size;
	*dest = new uint8_t[buffer.size()];
	std::memcpy((void*)*dest, buffer.data(),output_size);

	return(0);
}
std::vector<uint8_t>& LZWexpand::Decode(uint8_t* dsrc,uint8_t* dend)
{
	// deLZ
	auto err = DecodeCore(dsrc,dend);
	if(err)
	{
		buffer.resize(0);
		return(buffer);
	}

	// return directly working buffer resized to actual data size
	buffer.resize(output_size);
	return(buffer);
}
int LZWexpand::DecodeCore(uint8_t*dsrc, uint8_t *dend)
{
	LZWdictElem *cwrd = NULL;
	LZWdictElem pwrd;
	int ccd;

	// reset local output buffer
	output_size = 0;
	buffer.resize(buffer.capacity());

	// missing some parameters?
	if(!dsrc || !dend)
		return(1);

	// invalid parameter values?
	if(dsrc>=dend)
		return(1);

	// no output data yet
	/**dlen = 0;
	*dest = NULL;*/
	
	// load dictionary (clears the old one)
	dct.Load(&dsrc, dend);

	// init bitstream
	GetWord(NULL, NULL);
		
	// and start decoding
	pwrd.len = 0;
	while(dsrc < dend)
	{
		// get item from stream
		ccd = GetWord(&dsrc,dend);
		if(ccd < 0)
		{
			// finished
			break;
		}

		// clear dictionary?
		if(dct.isclr(ccd))
		{
			// yeah, clear dictionary

			// store previous chunk into local memory buffer first (because it
			// can't be lost with dictionary)
			if(pwrd.len > CMLEN)
			{
				// too long - leave (this should never happen but if so ...)
				return(3);
			}
			if(pwrd.len != 0)
				std::memcpy((void*)temp_chunk,(void*)pwrd.data,pwrd.len);
			pwrd.data = temp_chunk;

			// now clear dictionary
			dct.LZWreinitDict();

			// and cotinue
			continue;
		}

		// update dictionary?
		if(ccd < dct.cdict_len)
		{
			// yeah - append chunk

			// get new item
			cwrd = dct.get(ccd);

			// combine it with previous item and add into dictionary
			if(dct.ins(&pwrd,cwrd))
			{
				// inserting failed
				return(5);
			}
		}
		else if(ccd == dct.cdict_len)
		{
			// yeah - duplicate chunk

			// duplicate previous chunk and add into dictionary
			if(dct.ins(&pwrd,&pwrd))
			{
				// inserting failed
				return(5);
			}

			// get new item from dictionary
			cwrd = dct.get(ccd);
		}
		else
		{
			// failed - invalid dictionary entry			
			return(4);
		}

		// write chunk to local output buffer:
		//  ###note: this is much faster than using vector resize every time even if preallocated by reserve()
		int min_size = output_size + cwrd->len;
		if(buffer.size() < min_size)
			buffer.resize(buffer.size()*2);
		std::memcpy(&buffer[output_size], cwrd->data, cwrd->len);
		output_size += cwrd->len;

		// current word to previous word
		pwrd = *cwrd;
	}
	
	return(0);
}









//-------------------------------------------------------------------------------------------------
// Compression stuff
//-------------------------------------------------------------------------------------------------

// create initial dictionary
LZdct::LZdct(uint8_t* sdat,int len)
{
	int i;

	if(!sdat || !len)
	{
		return;
		//throw Exception("LZ dictionary constructor failed, invalid parameters!");
	}

	// scan for used values
	uint8_t idd[256];
	std::memset((void*)idd,0,256);
	for(i=0;i<len;i++)
		idd[*sdat++]=1;

	// get initial dictionary len
	idn=0;
	for(i=0;i<256;i++)
		if(idd[i])
		{
			idc[idn]=(uint8_t)i;
			map[i]=idn++;
		}

	// calc item width
	ibw=0;
	while((1<<ibw)<(idn+2))
		ibw++;
}

// set initial dictionary as current one
void LZdct::DCfillIDC(void)
{
	int i;

	// copy initial dict into current one
	for(i=0;i<idn;i++)
	{
		// item data
		dc[i].val=idc[i];
		// item links
		dc[i].left=-1;
		dc[i].right=-1;
		dc[i].pfx=-1;
		dc[i].first=-1;
	}

	// add clear code to dictionary (it's never used)
	dcn=idn+1;

	// calc current item width
	bw=0;
	while((1<<bw)<(dcn+1))
		bw++;
}

// add item into dictionary if not there, otherwise return found match index
int LZdct::DCadd(TDCitem* item)
{
	// return current code if no prefix index defined yet
	if(item->pfx==-1)
		return((int)((unsigned int)item->val));

	// first item index for current prefix
	int id=dc[item->pfx].first;

	// id exists?
	if(id==-1)
	{
		// no - write new item id as first item index
		dc[item->pfx].first=dcn;
	}
	else
	{
		// yaha - we have to search through current prefix items :-(

		while(1)
		{
			// return id if match
			if(item->val==dc[id].val)
				return(id);

			// scan list
			if(item->val<dc[id].val)
			{
				// new item higher than dict item
				int idn=dc[id].left;
				if(idn==-1)
				{
					// undefined so no match found
					// insert new item into dict
					dc[id].left=dcn;
					// and leave with no match
					break;
				}
				// defined, get next search location (next item with same prefix)
				id=idn;
			}
			else
			{
				// new item lower than dict item
				int idn=dc[id].right;
				if(idn==-1)
				{
					// undefined so no match found
					// insert new item into dict
					dc[id].right=dcn;
					// and leave with no match
					break;
				}
				// defined, get next search location (next item with same prefix)
				id=idn;
			}
		}
	}

	// item not found in dictionary if we have reached this point
	// so add it in
	dc[dcn++]=*item;

	// update item width
	if(dcn-1>=(1<<bw))
		bw++;

	// check for dict full and return no match
	if(dcn>DCMAXN-2)
		return(-2);
	return(-1);
}

// find/add item into dictionary
// return  1 if new item alredy in dict
//         0 if not (was added)
//        -1 if not (was added) and dictionary needs reset
int LZdct::DCfindAdd(TDCitem* item)
{
	// try to add item
	int idx=DCadd(item);
	if(idx>=0)
	{
		// match found
		item->pfx=idx;
		return(1);
	}
	if(idx<-1)
		return(idx);
	return(0);
}

// encode stuff and return data
LZspell::LZspell(uint8_t* sdat,int len,std::vector<uint8_t>& lzdata) : lzd(sdat,len)
{
	data.clear();
	duse = 0;
	rlen = 0;
	lzdata.clear();
	if(!Encode(sdat,len))
		lzdata = data;
}

// encode stuff
LZspell::LZspell(uint8_t* sdat,int len) : lzd(sdat,len)
{
	data.clear();
	duse = 0;
	rlen = 0;
	Encode(sdat,len);
}

// encode stuff
int LZspell::Encode(uint8_t* sdat,int len)
{
	data.clear();
	duse = 0;
	rlen = 0;

	// check parameters
	if(!sdat || !len)
	{
		// nothing to do
		return(0);
	}

	// reset LZ dict
	//lzd = LZdct(sdat,len);
	lzd.DCfillIDC();

	// init LZ bitstream writer
	LZbitPut(NULL,0,0);

	// put dictionary into output buffer
	if(LZputDC())
	{
		rlen = 0;
		return(1);
		//throw Exception("LZ encoder failed, memory allocation failed!");
	}

	// === encode data ===
	TDCitem item ={0,-1,-1,-1,-1};
	uint8_t* dptr = sdat;
	uint8_t* dend = sdat+len;
	do {
		// get byte, map it through initial dict
		item.val = lzd.map[*dptr++];

		// item in dict?
		int ret = lzd.DCfindAdd(&item);
		if(ret<=0)
		{
			// no, was added

			// put its index into output buffer
			if(LZputItem(item.pfx))
			{
				// failed
				rlen = 0;
				return(1);
				//throw Exception("LZ encoder failed, memory allocation failed!");
			}

			// and clear dict if neccessary
			if(ret<0)
			{
				// put clear code
				if(LZputClear())
				{
					// failed				
					rlen = 0;
					return(1);
					//throw Exception("LZ encoder failed, memory allocation failed!");
				}

				// clear dict
				lzd.DCfillIDC();
			}

			// and reset string (prefix)
			item.pfx = (int)((unsigned int)item.val);
		}
		// repeat for each byte
	} while(dptr<dend);

	// put last item index into output buffer
	if(LZputItem(item.pfx))
	{
		// failed
		rlen = 0;
		return(1);
		//throw Exception("LZ encoder failed, memory allocation failed!");
	}

	// put double clear code into output buffer (Spellcross speciality I think)
	if(LZputClear())
	{
		// failed
		rlen = 0;
		return(1);
		//throw Exception("LZ encoder failed, memory allocation failed!");
	}
	lzd.DCfillIDC();
	if(LZputClear())
	{
		// failed
		rlen = 0;
		return(1);
		//throw Exception("LZ encoder failed, memory allocation failed!");
	}

	// crop data to actual size
	data.resize(rlen);

	return(0);
}

// close LZ output buffer
LZspell::~LZspell(void)
{
	data.clear();
	duse = 0;
	rlen = 0;
}

// add memmory to LZ output buffer
int LZspell::LZmemAdd(int len)
{
	if(duse + len > data.size())
		data.resize(duse + len,0x00);
	return(0);
}


uint32_t bswap(uint32_t dw)
{
	return((dw >> 24) | ((dw&0x0000FF00) << 8) | ((dw&0x00FF0000) >> 8) | (dw << 24));
}

// put item into bitstream
int LZspell::LZbitPut(void** dsrc,int bw,int id)
{
	static const uint32_t mskl[13]={0x00000000u,
		0x80000000u,0xC0000000u,0xE0000000u,0xF0000000u,
		0xF8000000u,0xFC000000u,0xFE000000u,0xFF000000u,
		0xFF800000u,0xFFC00000u,0xFFE00000u,0xFFF00000u};	
	static int bofs;
	int i;

	// reset bitstream
	if(bw==0)
	{
		bofs=0;
		return(0);
	}

	int delta=0;
	if(bofs==0)
		delta++;

	// get word from stream
	uint32_t msk = ~(mskl[bw]>>bofs);
	uint32_t *data = (uint32_t*)(*(uint8_t**)dsrc);
	uint32_t val = ((uint32_t)id) << (32 - bofs - bw);
	*data = bswap((bswap(*data) & msk) | val);

/*	asm{
		// data pointer
		mov		ecx,DWORD[dsrc]
		mov		edx,[ecx]

		// get DWORD
		mov		eax,[edx]

		// add new item
		bswap eax
		and 	eax,DWORD[msk]
		mov 	cl,32
		sub 	cl,BYTE[bofs]
		sub 	cl,BYTE[bw]
		mov		ebx,DWORD[id]
		rol 	ebx,cl
		or  	eax,ebx
		bswap eax

		// write resulting DWORD back
		mov[edx],eax
	}*/

	// new offset
	bofs += bw;
	while(bofs>7)
	{
		bofs -= 8;
		*dsrc = (uint8_t*)*dsrc + 1;
		//((uint8_t*)*dsrc)++;
		if(bofs != 0)
			delta++;
	}

	return(delta);
}

// put initial dictionary to LZ output buffer
int LZspell::LZputDC(void)
{
	int i;

	// add memory
	if(LZmemAdd(1+2+lzd.idn))
	{
		return(1);
	}

	// write dict len
	*(uint16_t*)data.data() = lzd.idn;

	// item len
	data[2] = lzd.ibw;
	duse += 3;

	// wirte dict data
	if(lzd.idn != 256)
	{
		for(i=0; i < lzd.idn; i++)
			data[duse+i] = lzd.idc[i];
		duse += lzd.idn;
	}

	// real output len
	rlen = duse;

	return(0);
}

// put item index into LZ output buffer
int LZspell::LZputItem(int id)
{
	if(id >= lzd.dcn)
		return(1);

	// add memory
	if(LZmemAdd(8))
		return(2);

	// put item
	uint8_t* dptr = data.data() + duse;
	int delta = LZbitPut((void**)&dptr,lzd.bw,id);
	duse = dptr - data.data();
	rlen += delta;

	return(0);
}

// put clear code into LZ output buffer
int LZspell::LZputClear(void)
{
	// add memory
	if(LZmemAdd(8))
		return(2);

	// put clear code
	uint8_t* dptr = data.data() + duse;
	int delta = LZbitPut((void**)&dptr, lzd.bw, lzd.idn);
	duse = dptr - data.data();
	rlen += delta;

	return(0);
}

// get compressed data
void LZspell::LZgetData(uint8_t** lzdat,int* lzlen)
{
	// return data pointer
	if(lzdat)
		*lzdat = data.data();

	// return data len
	if(lzlen)
		*lzlen = rlen;
}