#ifndef _VIDEO_N64_H_
#define _VIDEO_N64_H_

#include "emu.h"
#include "video/rdpblend.h"
#include "video/rdptri.h"
#include "video/rdpfb.h"
#include "video/rdptpipe.h"
#include "video/rdpspn16.h"

/*****************************************************************************/

#define PIXEL_SIZE_4BIT			0
#define PIXEL_SIZE_8BIT			1
#define PIXEL_SIZE_16BIT		2
#define PIXEL_SIZE_32BIT		3

#define CYCLE_TYPE_1			0
#define CYCLE_TYPE_2			1
#define CYCLE_TYPE_COPY			2
#define CYCLE_TYPE_FILL			3

#define SAMPLE_TYPE_1x1         0
#define SAMPLE_TYPE_2x2         1

#define BYTE_ADDR_XOR		BYTE4_XOR_BE(0)
#define WORD_ADDR_XOR		(WORD_XOR_BE(0) >> 1)

#define XOR_SWAP_BYTE_SHIFT		2
#define XOR_SWAP_WORD_SHIFT		1
#define XOR_SWAP_DWORD_SHIFT	0

#define XOR_SWAP_BYTE	4
#define XOR_SWAP_WORD	2
#define XOR_SWAP_DWORD	1

#define FORMAT_RGBA				0
#define FORMAT_YUV				1
#define FORMAT_CI				2
#define FORMAT_IA				3
#define FORMAT_I				4

#ifdef LSB_FIRST
#define BYTE_XOR_DWORD_SWAP 7
#define WORD_XOR_DWORD_SWAP 3
#else
#define BYTE_XOR_DWORD_SWAP 4
#define WORD_XOR_DWORD_SWAP 2
#endif
#define DWORD_XOR_DWORD_SWAP 1

#define GET_LOW_RGBA16_TMEM(x)	(m_rdp->ReplicatedRGBA[((x) >> 1) & 0x1f])
#define GET_MED_RGBA16_TMEM(x)	(m_rdp->ReplicatedRGBA[((x) >> 6) & 0x1f])
#define GET_HI_RGBA16_TMEM(x)	(m_rdp->ReplicatedRGBA[((x) >> 11) & 0x1f])

#define MEM8_LIMIT  0x7fffff
#define MEM16_LIMIT 0x3fffff
#define MEM32_LIMIT 0x1fffff

#define RREADADDR8(in) (((in) <= MEM8_LIMIT) ? (((UINT8*)rdram)[(in) ^ BYTE_ADDR_XOR]) : 0)
#define RREADIDX16(in) (((in) <= MEM16_LIMIT) ? (((UINT16*)rdram)[(in) ^ WORD_ADDR_XOR]) : 0)
#define RREADIDX32(in) (((in) <= MEM32_LIMIT) ? (rdram[(in)]) : 0)

#define RWRITEADDR8(in, val)	{if ((in) <= MEM8_LIMIT) ((UINT8*)rdram)[(in) ^ BYTE_ADDR_XOR] = val;}
#define RWRITEIDX16(in, val)	{if ((in) <= MEM16_LIMIT) ((UINT16*)rdram)[(in) ^ WORD_ADDR_XOR] = val;}
#define RWRITEIDX32(in, val)	{if ((in) <= MEM32_LIMIT) rdram[(in)] = val;}

#define GETLOWCOL(x)	(((x) & 0x3e) << 2)
#define GETMEDCOL(x)	(((x) & 0x7c0) >> 3)
#define GETHICOL(x)		(((x) & 0xf800) >> 8)

#define HREADADDR8(in)			(((in) <= MEM8_LIMIT) ? (m_rdp->HiddenBits[(in) ^ BYTE_ADDR_XOR]) : 0)
#define HWRITEADDR8(in, val)	{if ((in) <= MEM8_LIMIT) m_rdp->HiddenBits[(in) ^ BYTE_ADDR_XOR] = val;}

//sign-extension macros
#define SIGN22(x)	(((x) & 0x200000) ? ((x) | ~0x3fffff) : ((x) & 0x3fffff))
#define SIGN17(x)	(((x) & 0x10000) ? ((x) | ~0x1ffff) : ((x) & 0x1ffff))
#define SIGN16(x)	(((x) & 0x8000) ? ((x) | ~0xffff) : ((x) & 0xffff))
#define SIGN13(x)	(((x) & 0x1000) ? ((x) | ~0x1fff) : ((x) & 0x1fff))
#define SIGN11(x)	(((x) & 0x400) ? ((x) | ~0x7ff) : ((x) & 0x7ff))
#define SIGN9(x)	(((x) & 0x100) ? ((x) | ~0x1ff) : ((x) & 0x1ff))
#define SIGN8(x)	(((x) & 0x80) ? ((x) | ~0xff) : ((x) & 0xff))

#define KURT_AKELEY_SIGN9(x)	((((x) & 0x180) == 0x180) ? ((x) | ~0x1ff) : ((x) & 0x1ff))

/*****************************************************************************/

class n64_periphs;

namespace N64
{

namespace RDP
{

class Processor;
class MiscStateT;
class OtherModesT;

class Color
{
	public:
		Color()
		{
			c = 0;
		}

		union
		{
			UINT32 c;
#ifdef LSB_FIRST
			struct { UINT8 a, b, g, r; } i;
#else
			struct { UINT8 r, g, b, a; } i;
#endif
		};
};

enum
{
	BIT_DEPTH_32 = 0,
	BIT_DEPTH_16,

	BIT_DEPTH_COUNT
};

class Tile
{
	public:
		int format;	// Image data format: RGBA, YUV, CI, IA, I
		int size; // Size of texel element: 4b, 8b, 16b, 32b
		int line; // Size of tile line in bytes
		int tmem; // Starting tmem address for this tile in bytes
		int palette; // Palette number for 4b CI texels
		int ct, mt, cs, ms; // Clamp / mirror enable bits for S / T direction
		int mask_t, shift_t, mask_s, shift_s; // Mask values / LOD shifts
		UINT16 sl, tl, sh, th;		// 10.2 fixed-point, starting and ending texel row / column
		int num;
};

class MiscStateT
{
	public:
		MiscStateT()
		{
			CurrentPixCvg = 0;
			CurrentMemCvg = 0;
			CurrentCvgBit = 0;
			CurrentPixOverlap = 0;

			MaxLevel = 0;
			MinLevel = 0;
		}

		int FBFormat;				// Framebuffer pixel format index (0 - I, 1 - IA, 2 - CI, 3 - RGBA)
		int FBSize;					// Framebuffer pixel size index (0 - 4bpp, 1 - 8bpp, 2 - 16bpp, 3 - 32bpp)
		int FBWidth;				// Framebuffer width, in pixels
		int FBHeight;				// Framebuffer height, in scanlines
		UINT32 FBAddress;			// Framebuffer source address offset (in bytes) from start of RDRAM

		UINT32 ZBAddress;			// Z-buffer source address offset (in bytes) from start of RDRAM

		int TIFormat;				// Format for Texture Interface (TI) transfers
		int TISize;					// Size (in bytes) of TI transfers
		int TIWidth;				// Width (in pixels) of TI transfers
		UINT32 TIAddress;			// Destination address for TI transfers

		UINT32 CurrentPixCvg;		// Coverage for the current pixel
		UINT32 CurrentMemCvg;		// Incoming coverage, in memory, for the current pixel
		UINT32 CurrentCvgBit;		// Bitfield representation of current coverage value
		UINT32 CurrentPixOverlap;	// Current overlap value, in coverage indices, for the current pixel

		UINT8 RandomSeed;			// %HACK%, adds 19 each time it's read and is more or less random

		int SpecialBlendSelect0;	// Special blend-mode select for cycle 0
		int SpecialBlendSelect1;	// Special blend-mode select for cycle 1

		UINT32 MaxLevel;			// Maximum LOD level for texture filtering
		UINT32 MinLevel;			// Minimum LOD level for texture filtering

		UINT16 PrimitiveZ;			// Forced Z value for current primitive, if applicable
		UINT16 PrimitiveDZ;			// Forced Delta-Z value for current primitive, if applicable
};

class CombineModes
{
	public:
		int sub_a_rgb0;
		int sub_b_rgb0;
		int mul_rgb0;
		int add_rgb0;
		int sub_a_a0;
		int sub_b_a0;
		int mul_a0;
		int add_a0;

		int sub_a_rgb1;
		int sub_b_rgb1;
		int mul_rgb1;
		int add_rgb1;
		int sub_a_a1;
		int sub_b_a1;
		int mul_a1;
		int add_a1;
};

class OtherModesT
{
	public:
		int cycle_type;
		bool persp_tex_en;
		bool detail_tex_en;
		bool sharpen_tex_en;
		bool tex_lod_en;
		bool en_tlut;
		bool tlut_type;
		bool sample_type;
		bool mid_texel;
		bool bi_lerp0;
		bool bi_lerp1;
		bool convert_one;
		bool key_en;
		int rgb_dither_sel;
		int alpha_dither_sel;
		int blend_m1a_0;
		int blend_m1a_1;
		int blend_m1b_0;
		int blend_m1b_1;
		int blend_m2a_0;
		int blend_m2a_1;
		int blend_m2b_0;
		int blend_m2b_1;
		int tex_edge;
		bool force_blend;
		bool alpha_cvg_select;
		bool cvg_times_alpha;
		int z_mode;
		int cvg_dest;
		bool color_on_cvg;
		bool image_read_en;
		bool z_update_en;
		bool z_compare_en;
		bool antialias_en;
		bool z_source_sel;
		bool dither_alpha_en;
		bool alpha_compare_en;
};

class ColorInputsT
{
	public:
		// combiner inputs
		UINT8 *combiner_rgbsub_a_r[2];
		UINT8 *combiner_rgbsub_a_g[2];
		UINT8 *combiner_rgbsub_a_b[2];
		UINT8 *combiner_rgbsub_b_r[2];
		UINT8 *combiner_rgbsub_b_g[2];
		UINT8 *combiner_rgbsub_b_b[2];
		UINT8 *combiner_rgbmul_r[2];
		UINT8 *combiner_rgbmul_g[2];
		UINT8 *combiner_rgbmul_b[2];
		UINT8 *combiner_rgbadd_r[2];
		UINT8 *combiner_rgbadd_g[2];
		UINT8 *combiner_rgbadd_b[2];

		UINT8 *combiner_alphasub_a[2];
		UINT8 *combiner_alphasub_b[2];
		UINT8 *combiner_alphamul[2];
		UINT8 *combiner_alphaadd[2];

		// blender input
		UINT8 *blender1a_r[2];
		UINT8 *blender1a_g[2];
		UINT8 *blender1a_b[2];
		UINT8 *blender1b_a[2];
		UINT8 *blender2a_r[2];
		UINT8 *blender2a_g[2];
		UINT8 *blender2a_b[2];
		UINT8 *blender2b_a[2];
};

struct Rectangle
{
	UINT16 m_xl;	// 10.2 fixed-point
	UINT16 m_yl;	// 10.2 fixed-point
	UINT16 m_xh;	// 10.2 fixed-point
	UINT16 m_yh;	// 10.2 fixed-point
};

class Processor
{
	public:
		Processor()
		{
			m_cmd_ptr = 0;
			m_cmd_cur = 0;

			m_start = 0;
			m_end = 0;
			m_current = 0;
			m_status = 0x88;

			for (int i = 0; i < 8; i++)
			{
				m_tiles[i].num = i;
			}

			OneColor.c = 0xffffffff;
			ZeroColor.c = 0x00000000;

			ColorInputs.combiner_rgbsub_a_r[0] = ColorInputs.combiner_rgbsub_a_r[1] = &OneColor.i.r;
			ColorInputs.combiner_rgbsub_a_g[0] = ColorInputs.combiner_rgbsub_a_g[1] = &OneColor.i.g;
			ColorInputs.combiner_rgbsub_a_b[0] = ColorInputs.combiner_rgbsub_a_b[1] = &OneColor.i.b;
			ColorInputs.combiner_rgbsub_b_r[0] = ColorInputs.combiner_rgbsub_b_r[1] = &OneColor.i.r;
			ColorInputs.combiner_rgbsub_b_g[0] = ColorInputs.combiner_rgbsub_b_g[1] = &OneColor.i.g;
			ColorInputs.combiner_rgbsub_b_b[0] = ColorInputs.combiner_rgbsub_b_b[1] = &OneColor.i.b;
			ColorInputs.combiner_rgbmul_r[0] = ColorInputs.combiner_rgbmul_r[1] = &OneColor.i.r;
			ColorInputs.combiner_rgbmul_g[0] = ColorInputs.combiner_rgbmul_g[1] = &OneColor.i.g;
			ColorInputs.combiner_rgbmul_b[0] = ColorInputs.combiner_rgbmul_b[1] = &OneColor.i.b;
			ColorInputs.combiner_rgbadd_r[0] = ColorInputs.combiner_rgbadd_r[1] = &OneColor.i.r;
			ColorInputs.combiner_rgbadd_g[0] = ColorInputs.combiner_rgbadd_g[1] = &OneColor.i.g;
			ColorInputs.combiner_rgbadd_b[0] = ColorInputs.combiner_rgbadd_b[1] = &OneColor.i.b;

			ColorInputs.combiner_alphasub_a[0] = ColorInputs.combiner_alphasub_a[1] = &OneColor.i.a;
			ColorInputs.combiner_alphasub_b[0] = ColorInputs.combiner_alphasub_b[1] = &OneColor.i.a;
			ColorInputs.combiner_alphamul[0] = ColorInputs.combiner_alphamul[1] = &OneColor.i.a;
			ColorInputs.combiner_alphaadd[0] = ColorInputs.combiner_alphaadd[1] = &OneColor.i.a;

			ColorInputs.blender1a_r[0] = ColorInputs.blender1a_r[1] = &PixelColor.i.r;
			ColorInputs.blender1a_g[0] = ColorInputs.blender1a_g[1] = &PixelColor.i.r;
			ColorInputs.blender1a_b[0] = ColorInputs.blender1a_b[1] = &PixelColor.i.r;
			ColorInputs.blender1b_a[0] = ColorInputs.blender1b_a[1] = &PixelColor.i.r;
			ColorInputs.blender2a_r[0] = ColorInputs.blender2a_r[1] = &PixelColor.i.r;
			ColorInputs.blender2a_g[0] = ColorInputs.blender2a_g[1] = &PixelColor.i.r;
			ColorInputs.blender2a_b[0] = ColorInputs.blender2a_b[1] = &PixelColor.i.r;
			ColorInputs.blender2b_a[0] = ColorInputs.blender2b_a[1] = &PixelColor.i.r;

			m_tmem = NULL;

			m_machine = NULL;

			//memset(m_hidden_bits, 3, 8388608);

			PrimLODFraction = 0;
			LODFraction = 0;

			for (int i = 0; i < 256; i++)
			{
				m_gamma_table[i] = sqrt((float)(i << 6));
				m_gamma_table[i] <<= 1;
			}

			for (int i = 0; i < 0x4000; i++)
			{
				m_gamma_dither_table[i] = sqrt((float)i);
				m_gamma_dither_table[i] <<= 1;
			}

			z_build_com_table();

			for (int i = 0; i < 0x4000; i++)
			{
				UINT32 exponent = (i >> 11) & 7;
				UINT32 mantissa = i & 0x7ff;
				z_complete_dec_table[i] = ((mantissa << z_dec_table[exponent].shift) + z_dec_table[exponent].add) & 0x3fffff;
			}

			precalc_cvmask_derivatives();

			for(int i = 0; i < 0x200; i++)
			{
				switch((i >> 7) & 3)
				{
				case 0:
				case 1:
					m_special_9bit_clamptable[i] = i & 0xff;
					break;
				case 2:
					m_special_9bit_clamptable[i] = 0xff;
					break;
				case 3:
					m_special_9bit_clamptable[i] = 0;
					break;
				}
			}

			for(int i = 0; i < 32; i++)
			{
				ReplicatedRGBA[i] = (i << 3) | ((i >> 2) & 7);
			}
		}

		~Processor() { }

		void	Dasm(char *buffer);

		void	ProcessList();
		UINT32	ReadData(UINT32 address);

		void set_span_base(int dr, int dg, int db, int da, int ds, int dt, int dw, int dz, int dymax, int dzpix)
		{
			m_span_dr = dr;
			m_span_dg = dg;
			m_span_db = db;
			m_span_da = da;
			m_span_ds = ds;
			m_span_dt = dt;
			m_span_dw = dw;
			m_span_dz = dz;
			m_span_dymax = dymax;
			m_span_dzpix = dzpix;
		}

		void set_span_base_y(int dr, int dg, int db, int da, int dz)
		{
			m_span_drdy = dr;
			m_span_dgdy = dg;
			m_span_dbdy = db;
			m_span_dady = da;
			m_span_dzdy = dz;
		}

		running_machine &machine() const { assert(m_machine != NULL); return *m_machine; }

		void	InitInternalState()
		{
			m_tmem = auto_alloc_array(machine(), UINT8, 0x1000);
			memset(m_tmem, 0, 0x1000);

			UINT8 *normpoint = machine().region("normpoint")->base();
			UINT8 *normslope = machine().region("normslope")->base();

			for(INT32 i = 0; i < 64; i++)
			{
				m_norm_point_rom[i] = (normpoint[(i << 1) + 1] << 8) | normpoint[i << 1];
				m_norm_slope_rom[i] = (normslope[(i << 1) + 1] << 8) | normslope[i << 1];
			}
		}

		void		SetMachine(running_machine& machine) { m_machine = &machine; }

		// CPU-visible registers
		void		SetStartReg(UINT32 val) { m_start = val; }
		UINT32		GetStartReg() const { return m_start; }

		void		SetEndReg(UINT32 val) { m_end = val; }
		UINT32		GetEndReg() const { return m_end; }

		void		SetCurrentReg(UINT32 val) { m_current = val; }
		UINT32		GetCurrentReg() const { return m_current; }

		void		SetStatusReg(UINT32 val) { m_status = val; }
		UINT32		GetStatusReg() const { return m_status; }

		// Internal state
		CombineModes*	GetCombine() { return &m_combine; }

		// Color Combiner
		INT32		ColorCombinerEquation(INT32 a, INT32 b, INT32 c, INT32 d);
		INT32		AlphaCombinerEquation(INT32 a, INT32 b, INT32 c, INT32 d);
		void		ColorCombiner2Cycle(bool noisecompute);
		void		ColorCombiner1Cycle(bool noisecompute);
		void		SetSubAInputRGB(UINT8 **input_r, UINT8 **input_g, UINT8 **input_b, int code);
		void		SetSubBInputRGB(UINT8 **input_r, UINT8 **input_g, UINT8 **input_b, int code);
		void		SetMulInputRGB(UINT8 **input_r, UINT8 **input_g, UINT8 **input_b, int code);
		void		SetAddInputRGB(UINT8 **input_r, UINT8 **input_g, UINT8 **input_b, int code);
		void		SetSubInputAlpha(UINT8 **input, int code);
		void		SetMulInputAlpha(UINT8 **input, int code);

		// Texture memory
		UINT8*		GetTMEM() { return m_tmem; }
		UINT16*		GetTMEM16() { return (UINT16*)m_tmem; }
		UINT32*		GetTMEM32() { return (UINT32*)m_tmem; }
		UINT16*		GetTLUT() { return (UINT16*)(m_tmem + 0x800); }
		Tile*		GetTiles(){ return m_tiles; }

		// Emulation Accelerators
		UINT8		GetRandom() { return MiscState.RandomSeed += 0x13; }

		// YUV Factors
		void		SetYUVFactors(INT32 k0, INT32 k1, INT32 k2, INT32 k3, INT32 k4, INT32 k5) { m_k0 = k0; m_k1 = k1; m_k2 = k2; m_k3 = k3; m_k4 = k4; m_k5 = k5; }
		INT32		GetK0() const { return m_k0; }
		INT32		GetK1() const { return m_k1; }
		INT32		GetK2() const { return m_k2; }
		INT32		GetK3() const { return m_k3; }
		INT32*		GetK4() { return &m_k4; }
		INT32*		GetK5() { return &m_k5; }

		// Blender-related (move into RDP::Blender)
		void		SetBlenderInput(int cycle, int which, UINT8 **input_r, UINT8 **input_g, UINT8 **input_b, UINT8 **input_a, int a, int b);

		// Render-related (move into eventual drawing-related classes?)
		Rectangle*		GetScissor() { return &m_scissor; }
		void			TCDiv(INT32 ss, INT32 st, INT32 sw, INT32* sss, INT32* sst);
		void			TCDivNoPersp(INT32 ss, INT32 st, INT32 sw, INT32* sss, INT32* sst);
		UINT32			GetLog2(UINT32 lod_clamp);
		void			RenderSpans(int start, int end, int tilenum, bool flip);
		void			GetAlphaCvg(UINT8 *comb_alpha);
		const UINT8*	GetBayerMatrix() const { return s_bayer_matrix; }
		const UINT8*	GetMagicMatrix() const { return s_magic_matrix; }
		int				GetCurrFIFOIndex() const { return m_cmd_cur; }

		void			ZStore(UINT32 zcurpixel, UINT32 dzcurpixel, UINT32 z);
		UINT32			ZDecompress(UINT32 zcurpixel);
		UINT32			DZDecompress(UINT32 zcurpixel, UINT32 dzcurpixel);
		UINT32			DZCompress(UINT32 value);
		INT32			NormalizeDZPix(INT32 sum);
		bool			ZCompare(UINT32 zcurpixel, UINT32 dzcurpixel, UINT32 sz, UINT16 dzpix);

		// Fullscreen update-related
		void			VideoUpdate(n64_periphs *n64, bitmap_rgb32 &bitmap);

		// Commands
		void		CmdInvalid(UINT32 w1, UINT32 w2);
		void		CmdNoOp(UINT32 w1, UINT32 w2);
		void		CmdTriangle(UINT32 w1, UINT32 w2);
		void		CmdTriangleZ(UINT32 w1, UINT32 w2);
		void		CmdTriangleT(UINT32 w1, UINT32 w2);
		void		CmdTriangleTZ(UINT32 w1, UINT32 w2);
		void		CmdTriangleS(UINT32 w1, UINT32 w2);
		void		CmdTriangleSZ(UINT32 w1, UINT32 w2);
		void		CmdTriangleST(UINT32 w1, UINT32 w2);
		void		CmdTriangleSTZ(UINT32 w1, UINT32 w2);
		void		CmdTexRect(UINT32 w1, UINT32 w2);
		void		CmdTexRectFlip(UINT32 w1, UINT32 w2);
		void		CmdSyncLoad(UINT32 w1, UINT32 w2);
		void		CmdSyncPipe(UINT32 w1, UINT32 w2);
		void		CmdSyncTile(UINT32 w1, UINT32 w2);
		void		CmdSyncFull(UINT32 w1, UINT32 w2);
		void		CmdSetKeyGB(UINT32 w1, UINT32 w2);
		void		CmdSetKeyR(UINT32 w1, UINT32 w2);
		void		CmdSetFillColor32(UINT32 w1, UINT32 w2);
		void		CmdSetConvert(UINT32 w1, UINT32 w2);
		void		CmdSetScissor(UINT32 w1, UINT32 w2);
		void		CmdSetPrimDepth(UINT32 w1, UINT32 w2);
		void		CmdSetOtherModes(UINT32 w1, UINT32 w2);
		void		CmdLoadTLUT(UINT32 w1, UINT32 w2);
		void		CmdSetTileSize(UINT32 w1, UINT32 w2);
		void		CmdLoadBlock(UINT32 w1, UINT32 w2);
		void		CmdLoadTile(UINT32 w1, UINT32 w2);
		void		CmdFillRect(UINT32 w1, UINT32 w2);
		void		CmdSetTile(UINT32 w1, UINT32 w2);
		void		CmdSetFogColor(UINT32 w1, UINT32 w2);
		void		CmdSetBlendColor(UINT32 w1, UINT32 w2);
		void		CmdSetPrimColor(UINT32 w1, UINT32 w2);
		void		CmdSetEnvColor(UINT32 w1, UINT32 w2);
		void		CmdSetCombine(UINT32 w1, UINT32 w2);
		void		CmdSetTextureImage(UINT32 w1, UINT32 w2);
		void		CmdSetMaskImage(UINT32 w1, UINT32 w2);
		void		CmdSetColorImage(UINT32 w1, UINT32 w2);

		void		Triangle(bool shade, bool texture, bool zbuffer);
		UINT32		AddRightCvg(UINT32 x, UINT32 k);
		UINT32		AddLeftCvg(UINT32 x, UINT32 k);

		UINT32*		GetCommandData() { return m_cmd_data; }
		UINT32*		GetTempRectData() { return m_temp_rect_data; }

		void		GetDitherValues(int x, int y, int* cdith, int* adith);


		int 			m_span_dr;
		int 			m_span_drdy;
		int 			m_span_dg;
		int 			m_span_dgdy;
		int 			m_span_db;
		int 			m_span_dbdy;
		int 			m_span_da;
		int 			m_span_dady;
		int 			m_span_ds;
		int 			m_span_dt;
		int 			m_span_dw;
		int 			m_span_dz;
		int 			m_span_dzdy;
		int 			m_span_dymax;
		int 			m_span_dzpix;

		UINT32*			GetSpecial9BitClampTable() { return m_special_9bit_clamptable; }

		UINT16 decompress_cvmask_frombyte(UINT8 x);
		void lookup_cvmask_derivatives(UINT32 mask, UINT8* offx, UINT8* offy);

		MiscStateT		MiscState;

		// Color constants
		Color			MemoryColor;
		Color			PixelColor;
		Color			InvPixelColor;
		Color			BlendedPixelColor;
		Color			BlendColor;

		Color			PrimColor;
		Color			EnvColor;
		Color			FogColor;
		Color			CombinedColor;
		Color			Texel0Color;
		Color			Texel1Color;
		Color			NextTexelColor;
		Color			ShadeColor;
		Color			KeyScale;
		Color			NoiseColor;

		Color			OneColor;
		Color			ZeroColor;

		UINT8			LODFraction;
		UINT8			PrimLODFraction;

		UINT32			FillColor;

		OtherModesT		OtherModes;
		ColorInputsT	ColorInputs;

		BlenderT		Blender;

		Span Spans[4096];

		FramebufferT	Framebuffer;
		TexturePipeT	TexPipe;

		UINT8 HiddenBits[0x800000];

		UINT8 ReplicatedRGBA[32];

	protected:
		CombineModes	m_combine;

		typedef struct
		{
			UINT8 cvg;
			UINT8 cvbit;
			UINT8 xoff;
			UINT8 yoff;
		} CVMASKDERIVATIVE;
		CVMASKDERIVATIVE cvarray[(1 << 8)];

		UINT16 z_com_table[0x40000]; //precalced table of compressed z values, 18b: 512 KB array!
		UINT32 z_complete_dec_table[0x4000]; //the same for decompressed z values, 14b
		UINT8 compressed_cvmasks[0x10000]; //16bit cvmask -> to byte

		UINT32		m_cmd_data[0x1000];
		UINT32		m_temp_rect_data[0x1000];

		int 		m_cmd_ptr;
		int 		m_cmd_cur;

		UINT32		m_start;
		UINT32		m_end;
		UINT32		m_current;
		UINT32		m_status;

		UINT8*		m_tmem;

		Tile		m_tiles[8];

		running_machine* m_machine;

		// YUV factors
		INT32 m_k0;
		INT32 m_k1;
		INT32 m_k2;
		INT32 m_k3;
		INT32 m_k4;
		INT32 m_k5;

		// Render-related (move into eventual drawing-related classes?)
		Rectangle m_scissor;

		// Texture perspective division
		INT32 m_norm_point_rom[64];
		INT32 m_norm_slope_rom[64];

		static const UINT8 s_bayer_matrix[16];
		static const UINT8 s_magic_matrix[16];

		INT32 m_gamma_table[256];
		INT32 m_gamma_dither_table[0x4000];

		UINT32 m_special_9bit_clamptable[512];

		INT32 m_dzpix_enc;
		INT32 m_dz_enc;

		class ZDecompressEntry
		{
			public:
				UINT32 shift;
				UINT32 add;
		};

		void precalc_cvmask_derivatives(void);
		void z_build_com_table(void);
		static const ZDecompressEntry z_dec_table[8];

		// Internal screen-update functions
		void			VideoUpdate16(n64_periphs *n64, bitmap_rgb32 &bitmap);
		void			VideoUpdate32(n64_periphs *n64, bitmap_rgb32 &bitmap);

		typedef void (*Command)(UINT32 w1, UINT32 w2);

		static const Command m_commands[0x40];
};

} // namespace RDP

} // namespace N64

#endif // _VIDEO_N64_H_
