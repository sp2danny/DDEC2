
#include "stdafx.h"

#include <cassert>
#include <iostream>

#include "frame.hpp"

#include "perftimer.h"

// ----------------------------------------------------------------------------

// ************
// *** Misc ***
// ************

namespace {

template<typename T>
T add(T t) { return t; }

template<typename T, /*typename U,*/ typename... Args>
T add(T t, T u, Args... args)
{
	return t + add<T>(u, args...);
}

template<typename... Ts>
RGB avg_n(Ts... ts)
{
	constexpr int n = sizeof...(ts);
	RGB ret;
	ret.r = (UC)(add<long>((ts.r)...) / n);
	ret.g = (UC)(add<long>((ts.g)...) / n);
	ret.b = (UC)(add<long>((ts.b)...) / n);
	return ret;
}

signed short diffshart_4b[] =
{
//     +1      +3      +5      +7        +9        +11       +13
	0, +1, -1, +4, -4, +9, -9, +16, -16, +25, -25, +36, -36, +49, -49
//  0   1   2   3   4   5   6    7    8    9   10   11   12   13   14 
};

UC add_diff_4b(UC prev, UC diff, UC targbit);
UL err_4b(UC prev, UC curr, UC targbit);

UC find_diff_4b(UC prev, UC curr, UC targbit)
{
	UL err = abs(int(prev) - int(curr));
	UC idx = 0;

	for (UC i = 1; i <= 14; ++i)
	{
		if (!err) break;
		UC pred = add_diff_4b(prev, i, targbit);
		UL e = abs(int(curr) - int(pred));
		if (e < err)
		{
			err = e;
			idx = i;
		}
	}

	return idx;
}

UC add_diff_4b(UC prev, UC diff, UC targbit)
{
	signed short aa = prev;
	assert(diff < 15);
	aa += diffshart_4b[diff];
	UC mask = (1 << targbit) - 1;
	return aa & mask;
}

void ReadXY(bitsource& bbr, signed short& dx, signed short& dy)
{
	dx = (signed short)(bbr.getS(4));
	dy = (signed short)(bbr.getS(4));
}

void WriteXY(bittarget& bbw, const signed short& dx, const signed short& dy)
{
	bbw.put(((UL)dx) & 0x0f, 4);
	bbw.put(((UL)dy) & 0x0f, 4);
}

constexpr signed short wrap_x(signed short x, signed short w)
{
	while (x <  w) x += w;
	while (x >= w) x -= w;
	return x;
}

constexpr signed short wrap_y(signed short y, signed short h)
{
	while (y <  h) y += h;
	while (y >= h) y -= h;
	return y;
}

struct DiffXY { signed char dx, dy; };

std::vector<DiffXY> makeDiffs()
{
	std::vector<DiffXY> vd;
	vd.push_back({0,0});
	for (signed char y = -8; y < 8; ++y)
		for (signed char x = -8; x < 8; ++x)
			if (x || y)
				vd.push_back({x,y});
	return vd;
}

std::vector<DiffXY> diffs = makeDiffs();

UL err_4b(UC prev, UC curr, UC targbit)
{
	UC diff = find_diff_4b(prev, curr, targbit);
	UC pred = add_diff_4b(prev, diff, targbit);
	return abs(int(curr) - int(pred));
}

UL diff_error(DiffXY d, const PredictBlock& pb, const Frame& prv)
{
	UL acc = 0;
	for (short y = 0; y < 10; ++y)
	{
		for (short x = 0; x < 10; ++x)
		{
			short x2 = wrap_x(x + pb.ox + d.dx, prv.w * 10);
			short y2 = wrap_y(y + pb.oy + d.dy, prv.h * 10);

			HSV p1 = pb.Pix(x, y);
			HSV p2 = prv.Pix(x2, y2);

			acc += err_4b(p2.h, p1.h, HBIT);
			acc += err_4b(p2.s, p1.s, SBIT);
			acc += err_4b(p2.v, p1.v, VBIT) << 1;
		}
	}
	return acc;
}

}

// ----------------------------------------------------------------------------

// *************
// *** Block ***
// *************

HSV Block::Pix(int x, int y) const
{
	assert((x >= 0) && (x < 10) && (y >= 0) && (y < 10));
	[[maybe_unused]] UL c1 = y * 10 + x;
	[[maybe_unused]] UL c2 = (y / 2) * 5 + (x / 2);
	return {
		#if HDS == 1
			h[c1],
		#else
			h[c2],
		#endif
		#if SDS == 1
			s[c1],
		#else
			s[c2],
		#endif
		#if VDS == 1
			v[c1]
		#else
			v[c2]
		#endif
	};
}

void Block::Save(bittarget& bt) const
{
	for (UC c : h) bt.put(c, HBIT);
	for (UC c : s) bt.put(c, SBIT);
	for (UC c : v) bt.put(c, VBIT);
}

bool Block::Load(bitsource& bs)
{
	for (UC& c : h) { if (!bs.have(HBIT)) return false; c = (UC)bs.get(HBIT); }
	for (UC& c : s) { if (!bs.have(SBIT)) return false; c = (UC)bs.get(SBIT); }
	for (UC& c : v) { if (!bs.have(VBIT)) return false; c = (UC)bs.get(VBIT); }
	return true;
}

void assignBlock(Block& lhs, const Block& rhs)
{
	lhs = rhs;
}

// ----------------------------------------------------------------------------

// ********************
// *** PredictBlock ***
// ********************

bool PredictBlock::ReadDiff(bitsource& bbr)
{
	if (!bbr.have(8))
		return false;
	ReadXY(bbr, dx, dy);

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

void PredictBlock::MakeBlock(const Frame& prv)
{
	TP t1;
	if (pt)
		t1 = hrc::now();

	for (short y = 0; y < 10; ++y)
	{
		for (short x = 0; x < 10; ++x)
		{
			short x2 = wrap_x(x + ox + dx, prv.w*10);
			short y2 = wrap_y(y + oy + dy, prv.h*10);

			HSV p2 = prv.Pix(x2, y2);

			auto idx1 = x + y * 10;

			#if HDS == 1
			h[idx1] = add_diff_4b(p2.h, hd[idx1], HBIT);
			#endif
			#if SDS == 1
			s[idx1] = add_diff_4b(p2.s, sd[idx1], SBIT);
			#endif
			#if VDS == 1
			v[idx1] = add_diff_4b(p2.v, vd[idx1], VBIT);
			#endif

			if ((x % 2) && (y % 2))
			{
				[[maybe_unused]] auto idx2 = (x / 2) + (y / 2) * 5;

				#if HDS == 2
				h[idx2] = add_diff_4b(p2.h, hd[idx2], HBIT);
				#endif

				#if SDS == 2
				s[idx2] = add_diff_4b(p2.s, sd[idx2], SBIT);
				#endif

				#if VDS == 2
				v[idx2] = add_diff_4b(p2.v, vd[idx2], VBIT);
				#endif
			}
		}
	}

	if (pt)
		pt->makeframe += Dur(hrc::now() - t1);
}

UL PredictBlock::MakeDiff(const Frame& prv)
{
	UL minerr = std::numeric_limits<UL>::max();
	DiffXY bsf{0,0};

	for (auto& d : diffs)
	{
		auto err = diff_error(d, *this, prv);
		if (err < minerr)
		{
			bsf = d;
			minerr = err;
		}
		if (!err) break;
	}

	dx = bsf.dx;
	dy = bsf.dy;

	for (short y = 0; y < 10; ++y)
	{
		for (short x = 0; x < 10; ++x)
		{
			short x2 = wrap_x(x + ox + dx, prv.w*10);
			short y2 = wrap_y(y + oy + dy, prv.h*10);

			HSV p1 = Pix(x, y);
			HSV p2 = prv.Pix(x2, y2);

			[[maybe_unused]] auto idx1 = x + y * 10;
			[[maybe_unused]] auto idx2 = (x / 2) + (y / 2) * 5;

			#define PROC_1( A, M, BT )  A[idx1] = find_diff_4b(p2.M ,p1.M, BT);
			#define PROC_2( A, M, BT )  if ((x%2) && (y%2)) A[idx2] = find_diff_4b(p2.M, p1.M, BT);

			#if HDS==1
				PROC_1(hd, h, HBIT)
			#else
				PROC_2(hd, h, HBIT)
			#endif

			#if SDS==1
				PROC_1(sd, s, SBIT)
			#else
				PROC_2(sd, s, SBIT)
			#endif

			#if VDS==1
				PROC_1(vd, v, VBIT)
			#else
				PROC_2(vd, v, VBIT)
			#endif

			#undef PROC_1
			#undef PROC_2

		}
	}
	return minerr;
}

UL PredictBlock::MakeDiffFast(const Frame& prv)
{
	signed char xx = 0, yy = 0;
	UL err, minerr = diff_error(DiffXY{xx,yy}, *this, prv);

	while (true)
	{
		bool found = false;
		if (!found) if (xx > -8)
		{
			err = diff_error(DiffXY{xx-1,yy}, *this, prv);
			if (err<minerr) {
				minerr = err;
				--xx;
				found = true;
			}
		}
		if (!found) if (xx < +7)
		{
			err = diff_error(DiffXY{xx+1,yy}, *this, prv);
			if (err < minerr) {
				minerr = err;
				++xx;
				found = true;
			}
		}
		if (!found) if (yy > -8)
		{
			err = diff_error(DiffXY{xx,yy-1}, *this, prv);
			if (err < minerr) {
				minerr = err;
				--yy;
				found = true;
			}
		}
		if (!found) if (yy < +7)
		{
			err = diff_error(DiffXY{xx,yy+1}, *this, prv);
			if (err < minerr) {
				minerr = err;
				++yy;
				found = true;
			}
		}
		if (!found) break;
	}

	dx = xx;
	dy = yy;

	for (short y = 0; y < 10; ++y)
	{
		for (short x = 0; x < 10; ++x)
		{
			short x2 = wrap_x(x + ox + dx, prv.w*10);
			short y2 = wrap_y(y + oy + dy, prv.h*10);

			HSV p1 = Pix(x, y);
			HSV p2 = prv.Pix(x2, y2);

			[[maybe_unused]] auto idx1 = x + y * 10;
			[[maybe_unused]] auto idx2 = (x / 2) + (y / 2) * 5;

			#define PROC_1( A, M, BT )  A[idx1] = find_diff_4b(p2.M ,p1.M, BT);
			#define PROC_2( A, M, BT )  if ((x%2) && (y%2)) A[idx2] = find_diff_4b(p2.M, p1.M, BT);

			#if HDS==1
				PROC_1(hd, h, HBIT)
			#else
				PROC_2(hd, h, HBIT)
			#endif

			#if SDS==1
				PROC_1(sd, s, SBIT)
			#else
				PROC_2(sd, s, SBIT)
			#endif

			#if VDS==1
				PROC_1(vd, v, VBIT)
			#else
				PROC_2(vd, v, VBIT)
			#endif

			#undef PROC_1
			#undef PROC_2
		}
	}

	return minerr;
}

void PredictBlock::WriteDiff(bittarget& bbw) const
{
	WriteXY(bbw, dx, dy);

	auto wrbl = [&bbw](const UC* p, int n) -> void
	{
		int endz = 0;
		for (int i = (n - 1); i >= 0; --i)
		{
			if (p[i])
				break;
			else
				++endz;
		}
		for (int i = 0; i < (n - endz); ++i)
		{
			bbw.put(p[i], 4);
		}
		if (endz) bbw.put(15, 4);
	};

	wrbl(hd, H_SZ);
	wrbl(sd, S_SZ);
	wrbl(vd, V_SZ);

}

//-----------------------------------------------------------------------------

// *************
// *** Frame ***
// *************

HSV Frame::Pix(int x, int y) const
{
	assert((x >= 0) && (x < w*10) && (y >= 0) && (y < h*10));
	const Block& bl = pix(UC(x/10), UC(y/10));
	return bl.Pix(x % 10, y % 10);
}

void ToFrame(const RGB_Image& src, Frame& dst)
{
	assert((src.w % 10) == 0);
	assert((src.h % 10) == 0);

	dst.resize(UC(src.w / 10), UC(src.h / 10));

	#define PROC_1(M, BIT)                                                     \
		for (UL y = 0; y < src.h; y += 1) {                                    \
			for (UL x = 0; x < src.w; x += 1) {                                \
				auto bl_x = UC(x / 10);                                        \
				auto bl_y = UC(y / 10);                                        \
				auto& bl = dst.pix(bl_x, bl_y);                                \
				const RGB& rgb = src.pix[x + y * src.w];                       \
				HSV hsv = RgbToHsv(rgb);                                       \
				auto bl_ix = x % 10;                                           \
				auto bl_iy = y % 10;                                           \
				bl. M [bl_ix + bl_iy * 10] = (hsv. M >> (8 - BIT));            \
		} }

	#define PROC_2(M, BIT)                                                     \
		for (UL y = 0; y < src.h; y += 2) {                                    \
			for (UL x = 0; x < src.w; x += 2) {                                \
				auto bl_x = UC(x / 10);                                        \
				auto bl_y = UC(y / 10);                                        \
				auto& bl = dst.pix(bl_x, bl_y);                                \
				const RGB& rgb_1 = src.pix[(x + 0) + (y + 0) * src.w];         \
				const RGB& rgb_2 = src.pix[(x + 1) + (y + 0) * src.w];         \
				const RGB& rgb_3 = src.pix[(x + 0) + (y + 1) * src.w];         \
				const RGB& rgb_4 = src.pix[(x + 1) + (y + 1) * src.w];         \
				RGB rgb = avg_n(rgb_1, rgb_2, rgb_3, rgb_4);                   \
				HSV hsv = RgbToHsv(rgb);                                       \
				auto bl_ix = x % 10;                                           \
				auto bl_iy = y % 10;                                           \
				bl. M [bl_ix / 2 + (bl_iy / 2) * 5] = (hsv. M >> (8 - BIT));   \
		} }

	#if HDS==1
		PROC_1(h, HBIT)
	#else
		PROC_2(h, HBIT)
	#endif

	#if SDS==1
		PROC_1(s, SBIT)
	#else
		PROC_2(s, SBIT)
	#endif

	#if VDS==1
		PROC_1(v, VBIT)
	#else
		PROC_2(v, VBIT)
	#endif

	#undef PROC_1
	#undef PROC_2
}

void FromFrame(const Frame& frame, RGB_Image& image)
{
	auto W = image.w = frame.w*10;
	auto H = image.h = frame.h*10;
	image.pix.resize(W*H);

	for (UL y = 0; y < H; ++y)
	{
		for (UL x = 0; x < W; ++x)
		{
			auto bl_x = UC(x / 10);
			auto bl_y = UC(y / 10);
			auto& bl = frame.pix(bl_x, bl_y);
			RGB& rgb = image.pix[x + y * W];
			auto bl_ix = x % 10;
			auto bl_iy = y % 10;
			HSV hsv = bl.Pix(bl_ix, bl_iy);
			hsv.h = hsv.h << (8 - HBIT);
			hsv.s = hsv.s << (8 - SBIT);
			hsv.v = hsv.v << (8 - VBIT);
			rgb = HsvToRgb(hsv);
		}
	}
}

void Frame::Save(bittarget& out) const
{
	for (UC y = 0; y < h; ++y)
	{
		for (UC x = 0; x < w; ++x)
		{
			const Block& bl = pix(x,y);
			bl.Save(out);
		}
	}
}

bool Frame::Load(bitsource& in)
{
	for (UC y = 0; y < h; ++y)
	{
		for (UC x = 0; x < w; ++x)
		{
			Block& bl = pix(x,y);
			if (!bl.Load(in))
				return false;
		}
	}
	return true;
}

void Frame::Black()
{
	for (UC y = 0; y < h; ++y)
	{
		for (UC x = 0; x < w; ++x)
		{
			Block& bl = pix(x,y);
			for (int i = 0; i < H_SZ; ++i)
				bl.h[i] = 0;
			for (int i = 0; i < S_SZ; ++i)
				bl.s[i] = 0;
			for (int i = 0; i < V_SZ; ++i)
				bl.v[i] = 0;
		}
	}
}

void assignFrame(Frame& lhs, const DiffFrame& rhs)
{
	TP t1;
	if (pt)
		t1 = hrc::now();

	UC w = rhs.w;
	UC h = rhs.h;
	lhs.w = w;
	lhs.h = h;
	lhs.blocks.resize(w*h);

	for (UC y = 0; y < h; ++y)
	{
		for (UC x = 0; x < w; ++x)
		{
			auto& lbl = lhs.pix(x,y);
			const auto& rbl = rhs.pix(x,y);
			assignBlock(lbl, rbl);
		}
	}

	if (pt)
		pt->makeframe += Dur(hrc::now() - t1);
}

//-----------------------------------------------------------------------------

// *****************
// *** DiffFrame ***
// *****************

DiffFrame::DiffFrame(UC w, UC h)
{
	resize(w,h);
}

void DiffFrame::resize(UC ww, UC hh)
{
	w = ww; h = hh;
	blocks.resize(w*h);
	for (UC y = 0; y < h; ++y)
	{
		for (UC x = 0; x < w; ++x)
		{
			PredictBlock& pbl = pix(x, y);
			pbl.ox = short(x) * 10;
			pbl.oy = short(y) * 10;
		}
	}
}



DiffFrame::operator Frame()
{
	Frame f;
	assignFrame(f, *this);
	return f;
}

void DiffFrame::construct(const Frame& fr)
{
	for (UC y = 0; y < h; ++y)
	{
		for (UC x = 0; x < w; ++x)
		{
			PredictBlock& pbl = pix(x, y);
			const Block& bl = fr.pix(x, y);
			assignBlock(pbl, bl);
		}
	}
}

UL DiffFrame::mkDiff(const Frame& prev, const Frame& curr)
{
	UL acc = 0;

	construct(curr);

	for (UC y = 0; y < h; ++y)
	{
		for (UC x = 0; x < w; ++x)
		{
			PredictBlock& pbl = pix(x,y);
			acc += pbl.MakeDiff(prev);
		}
	}

	return acc;
}

UL DiffFrame::mkDiffFast(const Frame& prev, const Frame& curr)
{
	UL acc = 0;

	construct(curr);

	for (UC y = 0; y < h; ++y)
	{
		for (UC x = 0; x < w; ++x)
		{
			PredictBlock& pbl = pix(x, y);
			acc += pbl.MakeDiffFast(prev);
		}
	}

	return acc;
}

void DiffFrame::mkFrame(const Frame& prev)
{
	for (UC y = 0; y < h; ++y)
	{
		for (UC x = 0; x < w; ++x)
		{
			PredictBlock& pbl = pix(x,y);
			pbl.MakeBlock(prev);
		}
	}
}

void DiffFrame::Save(bittarget& bbw) const
{
	for (UC y = 0; y < h; ++y)
	{
		for (UC x = 0; x < w; ++x)
		{
			const PredictBlock& pbl = pix(x,y);
			pbl.WriteDiff(bbw);
		}
	}
}

bool DiffFrame::Load(bitsource& bbr)
{
	for (UC y = 0; y < h; ++y)
	{
		for (UC x = 0; x < w; ++x)
		{
			PredictBlock& pbl = pix(x,y);
			if (!pbl.ReadDiff(bbr))
				return false;
		}
	}
	return true;
}

void FrameReport([[maybe_unused]] std::ostream& out, const DiffFrame& df)
{
	for (UC y = 0; y < df.h; ++y)
	{
		for (UC x = 0; x < df.w; ++x)
		{
			[[maybe_unused]] const PredictBlock& pbl = df.pix(x,y);
			out << "(" << pbl.dx << "," << pbl.dy << ")";
		}
		out << "\n";
	}
	out << "   -----   " << std::endl;
	out.flush();
}

