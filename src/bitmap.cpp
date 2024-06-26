
#include <iostream>
#include <cassert>
#include <cstring>
#include <cstdint>

#include "bitmap.hpp"

#pragma pack(push, 1)

typedef struct {
	
	std::uint16_t     bfType;
	std::uint32_t     bfSize;
	std::uint16_t     bfReserved1;
	std::uint16_t     bfReserved2;
	std::uint32_t     bfOffBits;
} BITMAPFILEHEADER;

typedef struct {
	std::uint32_t     biSize;
	std::int32_t      biWidth;
	std::int32_t      biHeight;
	std::uint16_t     biPlanes;
	std::uint16_t     biBitCount;
	std::uint32_t     biCompression;
	std::uint32_t     biSizeImage;
	std::int32_t      biXPelsPerMeter;
	std::int32_t      biYPelsPerMeter;
	std::uint32_t     biClrUsed;
	std::uint32_t     biClrImportant;
} BITMAPINFOHEADER;

#pragma pack(pop)

void LoadBMP(RGB_Image& img, std::istream& in)
{
	auto fhsz = sizeof(BITMAPFILEHEADER);
	auto ihsz = sizeof(BITMAPINFOHEADER);

	BITMAPFILEHEADER fh;
	in.read((char*)&fh, fhsz);

	assert(fh.bfType == 'MB');

	BITMAPINFOHEADER ih;
	in.read((char*)&ih, ihsz);

	int ofs = int(fh.bfOffBits) - int(fhsz+ihsz);
	if (ofs)
		in.seekg(ofs, in.cur);

	auto w = img.w = ih.biWidth;
	auto h = img.h = std::abs(ih.biHeight);
	img.pix.resize(w*h);

	assert(ih.biSize == ihsz);
	assert(ih.biBitCount == 24);
	assert(ih.biPlanes == 1);
	assert(ih.biCompression == 0);

	auto stride = w*3;
	while (stride%4) ++stride;
	auto padding = stride - (w * 3);

	int pixidx;
	for (auto y = 0; y < h; ++y)
	{
		if (ih.biHeight<0)
			pixidx = (h-y-1) * w;
		else
			pixidx = y * w;
		for (auto x = 0; x < w; ++x)
		{
			RGB rgb;
			if (!in.good())
			{
				std::cerr << std::boolalpha << "bad  " << in.bad()  << std::endl;
				std::cerr << std::boolalpha << "fail " << in.fail() << std::endl;
				std::cerr << std::boolalpha << "eof  " << in.eof()  << std::endl;
			}
			in.read( (char*)&rgb.b, 1 );
			in.read( (char*)&rgb.g, 1 );
			in.read( (char*)&rgb.r, 1 );
			img.pix[pixidx++] = rgb;
		}
		if (padding)
			in.seekg(padding, in.cur);
	}
}

void SaveBMP(const RGB_Image& img, std::ostream& out)
{
	BITMAPFILEHEADER fh;
	BITMAPINFOHEADER ih;

	fh.bfSize = sizeof(fh) + sizeof(ih);
	fh.bfType = 'MB';
	fh.bfReserved1 = fh.bfReserved2 = 0;
	fh.bfOffBits = sizeof(fh) + sizeof(ih);

	memset( &ih, 0, sizeof(ih) );

	int w = ih.biWidth  = img.w;
	int h = ih.biHeight = img.h;

	ih.biSize          = sizeof(ih);
	ih.biBitCount      = 24;
	ih.biPlanes        = 1;
	ih.biCompression   = 0;

	auto stride = w * 3;
	while (stride % 4) ++stride;

	auto padding = stride - (w * 3);

	char buf[] = {0,0,0,0};

	fh.bfSize += stride*h;

	out.write((char*)&fh, sizeof(fh));
	out.write((char*)&ih, sizeof(ih));

	for (int y = 0; y < h; ++y)
	{
		for (int x = 0; x < w; ++x)
		{
			RGB rgb = img.pix[x + y*w];
			out.write((char*)&rgb.b, 1);
			out.write((char*)&rgb.g, 1);
			out.write((char*)&rgb.r, 1);
		}
		if (padding)
			out.write(buf, padding);
	}
}

void FadeToBlack(RGB_Image& img, float amount)
{
	if (amount <= 0.0f) return;
	if (amount >= 1.0f)
	{
		for (auto& pix : img.pix)
			pix = RGB{0,0,0};
		return;
	}

	for (auto& pix : img.pix)
	{
		HSV hsv = RgbToHsv(pix);
		float v = hsv.v;
		v = v * (1.0f-amount);
		if (v>255) v=255;
		hsv.v = (UC)v;
		pix = HsvToRgb(hsv);
	}
}

HSV RgbToHsv(RGB rgb)
{
	HSV hsv;
	UC rgbMin, rgbMax;

	rgbMin = rgb.r < rgb.g ? (rgb.r < rgb.b ? rgb.r : rgb.b) : (rgb.g < rgb.b ? rgb.g : rgb.b);
	rgbMax = rgb.r > rgb.g ? (rgb.r > rgb.b ? rgb.r : rgb.b) : (rgb.g > rgb.b ? rgb.g : rgb.b);

	hsv.v = rgbMax;
	if (hsv.v == 0)
	{
		hsv.h = 0;
		hsv.s = 0;
		return hsv;
	}

	hsv.s = 255 * long(rgbMax - rgbMin) / hsv.v;
	if (hsv.s == 0)
	{
		hsv.h = 0;
		return hsv;
	}

	if (rgbMax == rgb.r)
		hsv.h =   0 + 43 * (rgb.g - rgb.b) / (rgbMax - rgbMin);
	else if (rgbMax == rgb.g)
		hsv.h =  85 + 43 * (rgb.b - rgb.r) / (rgbMax - rgbMin);
	else
		hsv.h = 171 + 43 * (rgb.r - rgb.g) / (rgbMax - rgbMin);

	return hsv;
}

RGB HsvToRgb(HSV hsv)
{
	RGB rgb;
	UC region, remainder, p, q, t;

	if (hsv.s == 0)
	{
		rgb.r = hsv.v;
		rgb.g = hsv.v;
		rgb.b = hsv.v;
		return rgb;
	}

	region = hsv.h / 43;
	remainder = (hsv.h - (region * 43)) * 6;

	p = (hsv.v * (255 - hsv.s)) >> 8;
	q = (hsv.v * (255 - ((hsv.s * remainder) >> 8))) >> 8;
	t = (hsv.v * (255 - ((hsv.s * (255 - remainder)) >> 8))) >> 8;

	switch (region)
	{
	case 0:
		rgb.r = hsv.v; rgb.g = t; rgb.b = p;
		break;
	case 1:
		rgb.r = q; rgb.g = hsv.v; rgb.b = p;
		break;
	case 2:
		rgb.r = p; rgb.g = hsv.v; rgb.b = t;
		break;
	case 3:
		rgb.r = p; rgb.g = q; rgb.b = hsv.v;
		break;
	case 4:
		rgb.r = t; rgb.g = p; rgb.b = hsv.v;
		break;
	default:
		rgb.r = hsv.v; rgb.g = p; rgb.b = q;
		break;
	}

	return rgb;
}

