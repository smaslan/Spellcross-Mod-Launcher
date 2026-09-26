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
#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>
#include <filesystem>
#include <type_traits>

#include "graphics.h"

class wxBitmap;

// palette record
class SpellPalette
{
private:
	void SortChunks();

public:
	std::string m_name;
	std::vector<uint8_t> m_pal;
	std::vector<uint8_t> m_used;
	std::vector<uint8_t> m_used_user;

	class Chunk{
	public:
		std::string name;
		int offset;
		int size;
		bool isWithin(uint8_t pixel);
		bool isWithin(std::vector<uint8_t> &pixel);
	};
	std::vector<Chunk> m_chunks;

	// in-place gamma correction for count rgb tripplets
	template <typename TRGB>
	static void Gamma(double gamma,TRGB *rgb,int count=1)
	{
		if constexpr(std::is_same_v<TRGB,ImgQuantize::Pixel>)
		{
			auto col = static_cast<ImgQuantize::Pixel*>(rgb);
			for(int k = 0; k < count; k++)
			{
				col[k].r = (pow((double)col[k].r/255.0, 1.0/gamma)*255.0);
				col[k].g = (pow((double)col[k].g/255.0, 1.0/gamma)*255.0);
				col[k].b = (pow((double)col[k].b/255.0, 1.0/gamma)*255.0);
			}
		}
		else
		{
			for(int k = 0; k < 3*count; k++)
				rgb[k] = (pow((double)rgb[k] / 255.0,1.0/gamma)*255.0);
		}
	}
	
	// in-place color saturation correction count rgb tripplets
	template <typename TRGB>
	static void Saturation(double sat,TRGB *rgb,int count=1)
	{
		if constexpr(std::is_same_v<TRGB,ImgQuantize::Pixel>)
		{
			auto col = static_cast<ImgQuantize::Pixel*>(rgb);
			for(int k = 0; k < count; k++)
			{
				double luma = 0.2126*col[k].r + 0.7152*col[k].g + 0.0722*col[k].b;
				col[k].r = (std::clamp(luma + sat*(col[k].r - luma),0.0,255.0));
				col[k].g = (std::clamp(luma + sat*(col[k].g - luma),0.0,255.0));
				col[k].b = (std::clamp(luma + sat*(col[k].b - luma),0.0,255.0));
			}
		}
		else
		{
			for(int k = 0; k < count; k++)
			{
				double luma = 0.2126*rgb[3*k + 0] + 0.7152*rgb[3*k + 1] + 0.0722*rgb[3*k + 2];
				for(int c = 0; c < 3; c++)
					rgb[3*k + c] = (std::clamp(luma + sat*(rgb[3*k + c] - luma),0.0,255.0));
			}
		}
	}

	// in-place color chroma correction of count rgb tripplets
	template <typename TRGB>
	static void Chroma(double chroma,TRGB* rgb,int count=1)
	{
		if constexpr(std::is_same_v<TRGB,ImgQuantize::Pixel>)
		{
			auto col = static_cast<ImgQuantize::Pixel*>(rgb);
			for(int k = 0; k < count; k++)
			{
				double luma = 0.299*col[k].r + 0.587*col[k].g + 0.114*col[k].b;
				col[k].r = (std::clamp(luma + chroma*(col[k].r - luma),0.0,255.0));
				col[k].g = (std::clamp(luma + chroma*(col[k].g - luma),0.0,255.0));
				col[k].b = (std::clamp(luma + chroma*(col[k].b - luma),0.0,255.0));
			}
		}
		else
		{
			for(int k = 0; k < count; k++)
			{
				double luma = 0.299*rgb[3*k + 0] + 0.587*rgb[3*k + 1] + 0.114*rgb[3*k + 2];
				for(int c = 0; c < 3; c++)
					rgb[3*k + c] = (std::clamp(luma + chroma*(rgb[3*k + c] - luma),0.0,255.0));
			}
		}
	}


	// (de)linearize sRGB (Gama 2.4)
	static inline float srgbToLinear(float v) {
		return (v <= 0.04045f) ? (v / 12.92f) : std::pow((v + 0.055f) / 1.055f,2.4f);
	}
	static inline float linearToSrgb(float v) {
		v = std::clamp(v,0.0f,1.0f);
		return (v <= 0.0031308f) ? (v * 12.92f) : (1.055f * std::pow(v,1.0f / 2.4f) - 0.055f);
	}
	// non-linear brightenss correction CIELAB
	static inline float labF(float t) {
		constexpr float delta = 6.0f / 29.0f;
		return (t > delta * delta * delta) ? std::pow(t,1.0f / 3.0f) : (t / (3.0f * delta * delta)) + (4.0f / 29.0f);
	}
	static inline float labFInverse(float t) {
		constexpr float delta = 6.0f / 29.0f;
		return (t > delta) ? (t * t * t) : 3.0f * delta * delta * (t - 4.0f / 29.0f);
	}

	// in-place color chroma-hue correction of count rgb tripplets according to Gimp implementation
	template <typename TRGB>
	static void GimpChromaHue(float chroma,float hue,TRGB* rgb,int count=1)
	{
		chroma = std::clamp(chroma,-1.0f,1.0f) + 1.0f;
		hue = std::clamp(hue,-1.0f,1.0f)*M_PI;
		
		// Reference White D65
		constexpr float Xn = 0.950489f;
		constexpr float Yn = 1.000000f;
		constexpr float Zn = 1.088840f;

		if constexpr(std::is_same_v<TRGB,ImgQuantize::Pixel>)
		{
			auto col = static_cast<ImgQuantize::Pixel*>(rgb);
			for(int k = 0; k < count; k++)
			{
				// Linear sRGB to CIEXYZ
				float r_lin = srgbToLinear(col[k].r / 255.0f);
				float g_lin = srgbToLinear(col[k].g / 255.0f);
				float b_lin = srgbToLinear(col[k].b / 255.0f);
				float X = r_lin * 0.4124564f + g_lin * 0.3575761f + b_lin * 0.1804375f;
				float Y = r_lin * 0.2126729f + g_lin * 0.7151522f + b_lin * 0.0721750f;
				float Z = r_lin * 0.0193339f + g_lin * 0.1191920f + b_lin * 0.9503041f;

				// CIEXYZ to CIELAB
				float fX = labF(X / Xn);
				float fY = labF(Y / Yn);
				float fZ = labF(Z / Zn);
				float L = 116.0f * fY - 16.0f;
				float a = 500.0f * (fX - fY);
				float b = 200.0f * (fY - fZ);

				// CIELAB to polar CIE LCh
				float C = std::sqrt(a * a + b * b);
				float h = std::atan2(b,a);

				// chroma and hue correction
				C *= chroma;
				h += hue;

				// normalize hue
				if(h > M_PI)
					h -= 2.0f * M_PI;
				else if(h < -M_PI)
					h += 2.0f * M_PI;

				// vack to CIELAB
				a = C * std::cos(h);
				b = C * std::sin(h);

				// CIELAB to CIEXYZ
				fY = (L + 16.0f) / 116.0f;
				fX = fY + (a / 500.0f);
				fZ = fY - (b / 200.0f);
				X = labFInverse(fX) * Xn;
				Y = labFInverse(fY) * Yn;
				Z = labFInverse(fZ) * Zn;

				// back to linear sRGB
				float r_out = X *  3.2404542f + Y * -1.5371385f + Z * -0.4985314f;
				float g_out = X * -0.9692660f + Y *  1.8760108f + Z *  0.0415560f;
				float b_out = X *  0.0556434f + Y * -0.2040259f + Z *  1.0572252f;
				col[k].r = static_cast<uint8_t>(std::round(linearToSrgb(r_out) * 255.0f));
				col[k].g = static_cast<uint8_t>(std::round(linearToSrgb(g_out) * 255.0f));
				col[k].b = static_cast<uint8_t>(std::round(linearToSrgb(b_out) * 255.0f));
			}
		}
		else
		{
			for(int k = 0; k < count; k++)
			{
				// Linear sRGB to CIEXYZ
				float r_lin = srgbToLinear(rgb[3*k + 0] / 255.0f);
				float g_lin = srgbToLinear(rgb[3*k + 1] / 255.0f);
				float b_lin = srgbToLinear(rgb[3*k + 2] / 255.0f);
				float X = r_lin * 0.4124564f + g_lin * 0.3575761f + b_lin * 0.1804375f;
				float Y = r_lin * 0.2126729f + g_lin * 0.7151522f + b_lin * 0.0721750f;
				float Z = r_lin * 0.0193339f + g_lin * 0.1191920f + b_lin * 0.9503041f;

				// CIEXYZ to CIELAB
				float fX = labF(X / Xn);
				float fY = labF(Y / Yn);
				float fZ = labF(Z / Zn);
				float L = 116.0f * fY - 16.0f;
				float a = 500.0f * (fX - fY);
				float b = 200.0f * (fY - fZ);

				// CIELAB to polar CIE LCh
				float C = std::sqrt(a * a + b * b);
				float h = std::atan2(b,a);

				// chroma and hue correction
				C *= chroma;
				h += hue;

				// normalize hue
				if(h > M_PI)
					h -= 2.0f * M_PI;
				else if(h < -M_PI)
					h += 2.0f * M_PI;

				// vack to CIELAB
				a = C * std::cos(h);
				b = C * std::sin(h);

				// CIELAB to CIEXYZ
				fY = (L + 16.0f) / 116.0f;
				fX = fY + (a / 500.0f);
				fZ = fY - (b / 200.0f);
				X = labFInverse(fX) * Xn;
				Y = labFInverse(fY) * Yn;
				Z = labFInverse(fZ) * Zn;

				// back to linear sRGB
				float r_out = X *  3.2404542f + Y * -1.5371385f + Z * -0.4985314f;
				float g_out = X * -0.9692660f + Y *  1.8760108f + Z *  0.0415560f;
				float b_out = X *  0.0556434f + Y * -0.2040259f + Z *  1.0572252f;
				rgb[3*k + 0] = static_cast<uint8_t>(std::round(linearToSrgb(r_out) * 255.0f));
				rgb[3*k + 1] = static_cast<uint8_t>(std::round(linearToSrgb(g_out) * 255.0f));
				rgb[3*k + 2] = static_cast<uint8_t>(std::round(linearToSrgb(b_out) * 255.0f));
			}
		}
	}


	SpellPalette(SpellPalette &pal,std::vector<std::string> chunks={});
	SpellPalette(std::string name="empty");
	void Clear();
	int Insert(uint8_t *data,std::string name,int offset,int count);
	int Insert(std::vector<uint8_t> &data,std::string name="",int offset=0,int count=0);
	int Insert(std::filesystem::path path,int offset=0,std::string used="");
	int InsertBlank(std::string name,int offset,int size,std::string color_str_filter="");
	int AssignColors(std::vector<ImgQuantize::Pixel> &colors, std::vector<uint8_t> mask={});
	int GetUsedCount();
	std::tuple<int,int> GetRange(int start=0);
	std::string GetRangeString(bool add_zero=true,std::vector<std::string> chunk_names={});
	int Save(std::filesystem::path path);
	int SaveChunks(std::filesystem::path directory_path);
	int LoadInfo(std::filesystem::path path);
	int SaveInfo(std::filesystem::path path,std::vector<std::string> chunks={});
	int Render(wxBitmap& bmp);
	int RenderPaletteColor(wxBitmap& bmp,int x_size,int x_pos,uint8_t* filter=NULL);
	uint8_t (*GetPal())[3];
	Chunk *GetChunk(std::string name);
	std::vector<std::string> GetChunkNames();
	std::vector<std::string> GetChunkNames(std::vector<uint8_t> &pixels);
	bool CheckPixels(std::vector<uint8_t>& pixels, std::vector<std::string> chunk_names={});
	
	// to mark used color by user (not used for exporting)
	int ClearUserRange();
	int AddUserRange(std::string chunk_name);
	int AddUserRange(std::vector<std::string> &chunk_names);
	int AddUserRange(std::vector<uint8_t> &pixels);
	int AddUserRangeStr(std::string range_string);
	std::tuple<int,int> GetUserRange(int start=0);
	std::string GetUserRangeString(bool add_zero=true);
	bool isInUserRange(std::string chunk_name);

	static std::vector<uint8_t> ParseRangeStr(std::string range_string,int count=256);
};
