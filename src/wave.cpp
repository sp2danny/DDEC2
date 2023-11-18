
#include <memory>
#include <limits>
#include <bit>

#include "core.hpp"

#include "wave.hpp"

#include "fft/pffft.h"

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


void wavCompress(
	[[maybe_unused]]short* b,
	[[maybe_unused]]short* e,
	[[maybe_unused]]bittarget& dst)
{
	int n= (e-b);
	
	while(n%32) --n;
	
	if (!n) return;
	
	auto fsrc = std::make_unique<float[]>(n);
	auto fdst = std::make_unique<float[]>(n);

	for (int i=0; i<n; ++i)
		fsrc[i] = 0.0f + b[i];
		
	PFFFT_Setup* fft = pffft_new_setup(n, PFFFT_REAL);
	
	pffft_transform(fft, fsrc.get(), fdst.get(), nullptr, PFFFT_FORWARD);

	int bucket[65536] = {0};
	
	for(int i=0; i<n; ++i)
	{
		float f = fdst[i];
		if (f < std::numeric_limits<short>::min()) continue;
		if (f > std::numeric_limits<short>::max()) continue;
		short s = f;
		bucket[ std::bit_cast<unsigned short>(s) ] += 1;
	}
	
	int a_idx = -1, a_val = 0;
	int b_idx = -1, b_val = 0;
	int c_idx = -1, c_val = 0;
	
	for (int i=0; i<n; ++i)
	{
		int val = bucket[i];
		if (!val) continue;
		/**/ if (val > a_val) {
			c_idx = b_idx; c_val = b_val;
			b_idx = a_idx; b_val = a_val;
			a_idx = i; a_val = val;
		} else if (val > b_val) {
			c_idx = b_idx; c_val = b_val;
			b_idx = i; b_val = val;	
		} else if (val > c_val) {
			c_idx = i; c_val = val;	
		}
	}
	
	if (a_idx != -1) {
		std::cout << "#1 freq " << std::bit_cast<short>( (unsigned short)a_idx ) << " count " << a_val << "\n";
	}
	if (b_idx != -1) {
		std::cout << "#2 freq " << std::bit_cast<short>( (unsigned short)b_idx ) << " count " << b_val << "\n";
	}
	if (c_idx != -1) {
		std::cout << "#3 freq " << std::bit_cast<short>( (unsigned short)c_idx ) << " count " << c_val << "\n";
	}
	
	
}

extern void wavDecompress(bitsource&, std::vector<short>&);


