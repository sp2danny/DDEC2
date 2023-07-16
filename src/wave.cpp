
#include "core.hpp"

#include "wave.hpp"

static bool cmp4(const char* s1, const char* s2)
{
	return
		(s1[0] == s2[0]) &&
		(s1[1] == s2[1]) &&
		(s1[2] == s2[2]) &&
		(s1[3] == s2[3]) ;
}

bool readWave(std::istream& in, std::vector<short>& data)
{
	RiffHdr hdr;

	in.read((char*)&hdr, sizeof(hdr));
	if (!in) return false;
	if (!cmp4(hdr.riff, "RIFF")) return false;
	if (!cmp4(hdr.wave, "WAVE")) return false;
	if (!cmp4(hdr.Subchunk1ID, "fmt ")) return false;
	if (hdr.Subchunk1Size != 16) return false;
	if (hdr.AudioFormat != 1) return false;
	if (hdr.NumChannels != 1) return false;
	if (hdr.SampleRate != 22050) return false;
	if (hdr.ByteRate != 44100) return false;
	if (hdr.BlockAlign != 2) return false;
	if (hdr.BitsPerSample != 16) return false;
	if (!cmp4(hdr.Subchunk2ID, "data")) return false;

	auto sz = hdr.Subchunk2Size / 2;

	data.resize(sz);
	in.read((char*)data.data(), hdr.Subchunk2Size);

	auto gc = in.gcount();
	std::cerr << "read " << gc << " from wave\n";

	bool ok = (gc == hdr.Subchunk2Size);

	return ok;
}


