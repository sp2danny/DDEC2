
#pragma once

#include <cstdint>
#include <vector>
#include <iostream>

#pragma pack(push, 1)

struct RiffHdr
{
	typedef std::int16_t s16;
	typedef std::int32_t s32;

	char riff[4];
	s32  ChunkSize;
	char wave[4];
	char Subchunk1ID[4];
	s32  Subchunk1Size;
	s16  AudioFormat;
	s16  NumChannels;
	s32  SampleRate;
	s32  ByteRate;
	s16  BlockAlign;
	s16  BitsPerSample;
	char Subchunk2ID[4];
	s32  Subchunk2Size;

};

#pragma pack(pop)

extern bool readWave(std::istream&, std::vector<short>&);


