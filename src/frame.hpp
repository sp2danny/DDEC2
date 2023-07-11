
#pragma once

#include "core.hpp"
#include "bitmap.hpp"
#include "bitstream.hpp"

#define HDS 2
#define SDS 2
#define VDS 1

constexpr int H_SZ = 100 / (HDS*HDS);
constexpr int S_SZ = 100 / (SDS*SDS);
constexpr int V_SZ = 100 / (VDS*VDS);

constexpr int HBIT = 6;
constexpr int SBIT = 6;
constexpr int VBIT = 6;

struct Block
{
	UC h[H_SZ];
	UC s[S_SZ];
	UC v[V_SZ];

	HSV Pix(int x, int y) const;

	//HSV operator[](int x, int y) const { return Pix(x,y); }

	void Save(bittarget&) const;
	bool Load(bitsource&);
};

struct Frame;
typedef Frame* FrameP;

struct PredictBlock : Block
{
	signed short dx, dy;
	signed short ox, oy;

	UC hd[H_SZ];
	UC sd[S_SZ];
	UC vd[V_SZ];

	bool ReadDiff(bitsource&);
	void MakeBlock(const Frame&);
	UL MakeDiff(const Frame&);
	void WriteDiff(bittarget&) const;
	UL MakeDiffFast(const Frame&);

	template<typename SRC>
	bool ReadDiffT(SRC&);
};

struct Frame
{
	Frame() : w(0), h(0) {}
	Frame(UC w, UC h)
		: w(w), h(h)
	{
		blocks.resize(w*h);
	}
	void resize(UC ww, UC hh)
	{
		w = ww; h = hh;
		blocks.resize(w*h);
	}
	std::vector<Block> blocks;
	Block& pix(UC x, UC y) { return blocks[x + w * y]; }
	//Block& operator[](int x, int y) { return blocks[x + w * y]; }
	const Block& pix(UC x, UC y) const { return blocks[x + w * y]; }
	//const Block& operator[](int x, int y) const { return blocks[x + w * y]; }
	void Save(bittarget&) const;
	bool Load(bitsource&);
	void Black();
	UC w, h;

	HSV Pix(int x, int y) const;
};

struct DiffFrame
{
	std::vector<PredictBlock> blocks;
	int w, h;

	DiffFrame() : w(0), h(0) {}
	DiffFrame(UC, UC);

	void resize(UC, UC);

	PredictBlock& pix(int x, int y) { return _pix(x,y); }
	const PredictBlock& pix(int x, int y) const { return _pix(x,y); }

	//PredictBlock& operator[](int x, int y) { return pix(x,y); }
	//const PredictBlock& operator[](int x, int y) const { return pix(x,y); }

	operator Frame();

	void Save(bittarget&) const;
	bool Load(bitsource&);

	template<typename SRC>
	bool LoadT(SRC&);

	int mkDiff(const Frame& prev, const Frame& curr);
	int mkDiffFast(const Frame& prev, const Frame& curr);

	void mkFrame(const Frame& prev);

	void construct(const Frame& fr);

private:
	PredictBlock& _pix(int x, int y) const { return (PredictBlock&)blocks[x + w * y]; }
};

extern void LoadBMP(RGB_Image&, std::istream&);
extern void SaveBMP(const RGB_Image&, std::ostream&);

extern void ToFrame(const RGB_Image&, Frame&);
extern void FromFrame(const Frame&, RGB_Image&);

extern void assignBlock(Block& lhs, const Block& rhs);
extern void assignFrame(Frame& lhs, const DiffFrame& rhs);

template<typename SRC>
bool DiffFrame::LoadT(SRC& bbr)
{
	for (int y = 0; y < h; ++y) {
		for (int x = 0; x < w; ++x) {
			PredictBlock& pbl = pix(x,y);
			if (!pbl.ReadDiffT(bbr))
				return false;
		}
	}
	return true;
}

extern void ReadXY(bitsource& bbr, signed short& dx, signed short& dy);
extern void LogXY(signed short& dx, signed short& dy);

template<typename SRC>
bool PredictBlock::ReadDiffT(SRC& bbr)
{
	if (!bbr.have(8))
		return false;

	dx = (signed short)(bbr.getS(4));
	dy = (signed short)(bbr.getS(4));

	auto rdbl = [&bbr](UC* p, int n) -> bool
	{
		int i;
		for (i = 0; i < n; ++i)
		{
			if (!bbr.have(4))
				return false;
			UC bits = (UC)bbr.get(4);
			if (bits == 15) break;
			p[i] = bits;
		}
		for (; i < n; ++i)
			p[i] = 0;
		return true;
	};

	if (!rdbl(hd, H_SZ)) return false;
	if (!rdbl(sd, S_SZ)) return false;
	if (!rdbl(vd, V_SZ)) return false;

	return true;
}

