
#include "core.hpp"

struct RGB_Image
{
	unsigned long w, h;
	std::vector<RGB> pix;
};

extern void LoadBMP(RGB_Image& img, std::istream& in);

extern void SaveBMP(const RGB_Image& img, std::ostream& out);

extern void FadeToBlack(RGB_Image& img, float amount);

extern HSV RgbToHsv(RGB rgb);
extern RGB HsvToRgb(HSV hsv);

extern void Resize(const RGB_Image& src, RGB_Image& dst, UL w, UL h);
extern void Resize(const RGB_Image& src, RGB_Image& dst, float scale);

