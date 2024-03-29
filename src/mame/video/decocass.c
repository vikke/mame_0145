/***********************************************************************

    DECO Cassette System video

 ***********************************************************************/

#include "emu.h"
#include "machine/decocass.h"


static const UINT32 tile_offset[32*32] = {
	0x078,0x079,0x07a,0x07b,0x07c,0x07d,0x07e,0x07f,0x0ff,0x0fe,0x0fd,0x0fc,0x0fb,0x0fa,0x0f9,0x0f8,0x278,0x279,0x27a,0x27b,0x27c,0x27d,0x27e,0x27f,0x2ff,0x2fe,0x2fd,0x2fc,0x2fb,0x2fa,0x2f9,0x2f8,
	0x070,0x071,0x072,0x073,0x074,0x075,0x076,0x077,0x0f7,0x0f6,0x0f5,0x0f4,0x0f3,0x0f2,0x0f1,0x0f0,0x270,0x271,0x272,0x273,0x274,0x275,0x276,0x277,0x2f7,0x2f6,0x2f5,0x2f4,0x2f3,0x2f2,0x2f1,0x2f0,
	0x068,0x069,0x06a,0x06b,0x06c,0x06d,0x06e,0x06f,0x0ef,0x0ee,0x0ed,0x0ec,0x0eb,0x0ea,0x0e9,0x0e8,0x268,0x269,0x26a,0x26b,0x26c,0x26d,0x26e,0x26f,0x2ef,0x2ee,0x2ed,0x2ec,0x2eb,0x2ea,0x2e9,0x2e8,
	0x060,0x061,0x062,0x063,0x064,0x065,0x066,0x067,0x0e7,0x0e6,0x0e5,0x0e4,0x0e3,0x0e2,0x0e1,0x0e0,0x260,0x261,0x262,0x263,0x264,0x265,0x266,0x267,0x2e7,0x2e6,0x2e5,0x2e4,0x2e3,0x2e2,0x2e1,0x2e0,
	0x058,0x059,0x05a,0x05b,0x05c,0x05d,0x05e,0x05f,0x0df,0x0de,0x0dd,0x0dc,0x0db,0x0da,0x0d9,0x0d8,0x258,0x259,0x25a,0x25b,0x25c,0x25d,0x25e,0x25f,0x2df,0x2de,0x2dd,0x2dc,0x2db,0x2da,0x2d9,0x2d8,
	0x050,0x051,0x052,0x053,0x054,0x055,0x056,0x057,0x0d7,0x0d6,0x0d5,0x0d4,0x0d3,0x0d2,0x0d1,0x0d0,0x250,0x251,0x252,0x253,0x254,0x255,0x256,0x257,0x2d7,0x2d6,0x2d5,0x2d4,0x2d3,0x2d2,0x2d1,0x2d0,
	0x048,0x049,0x04a,0x04b,0x04c,0x04d,0x04e,0x04f,0x0cf,0x0ce,0x0cd,0x0cc,0x0cb,0x0ca,0x0c9,0x0c8,0x248,0x249,0x24a,0x24b,0x24c,0x24d,0x24e,0x24f,0x2cf,0x2ce,0x2cd,0x2cc,0x2cb,0x2ca,0x2c9,0x2c8,
	0x040,0x041,0x042,0x043,0x044,0x045,0x046,0x047,0x0c7,0x0c6,0x0c5,0x0c4,0x0c3,0x0c2,0x0c1,0x0c0,0x240,0x241,0x242,0x243,0x244,0x245,0x246,0x247,0x2c7,0x2c6,0x2c5,0x2c4,0x2c3,0x2c2,0x2c1,0x2c0,
	0x038,0x039,0x03a,0x03b,0x03c,0x03d,0x03e,0x03f,0x0bf,0x0be,0x0bd,0x0bc,0x0bb,0x0ba,0x0b9,0x0b8,0x238,0x239,0x23a,0x23b,0x23c,0x23d,0x23e,0x23f,0x2bf,0x2be,0x2bd,0x2bc,0x2bb,0x2ba,0x2b9,0x2b8,
	0x030,0x031,0x032,0x033,0x034,0x035,0x036,0x037,0x0b7,0x0b6,0x0b5,0x0b4,0x0b3,0x0b2,0x0b1,0x0b0,0x230,0x231,0x232,0x233,0x234,0x235,0x236,0x237,0x2b7,0x2b6,0x2b5,0x2b4,0x2b3,0x2b2,0x2b1,0x2b0,
	0x028,0x029,0x02a,0x02b,0x02c,0x02d,0x02e,0x02f,0x0af,0x0ae,0x0ad,0x0ac,0x0ab,0x0aa,0x0a9,0x0a8,0x228,0x229,0x22a,0x22b,0x22c,0x22d,0x22e,0x22f,0x2af,0x2ae,0x2ad,0x2ac,0x2ab,0x2aa,0x2a9,0x2a8,
	0x020,0x021,0x022,0x023,0x024,0x025,0x026,0x027,0x0a7,0x0a6,0x0a5,0x0a4,0x0a3,0x0a2,0x0a1,0x0a0,0x220,0x221,0x222,0x223,0x224,0x225,0x226,0x227,0x2a7,0x2a6,0x2a5,0x2a4,0x2a3,0x2a2,0x2a1,0x2a0,
	0x018,0x019,0x01a,0x01b,0x01c,0x01d,0x01e,0x01f,0x09f,0x09e,0x09d,0x09c,0x09b,0x09a,0x099,0x098,0x218,0x219,0x21a,0x21b,0x21c,0x21d,0x21e,0x21f,0x29f,0x29e,0x29d,0x29c,0x29b,0x29a,0x299,0x298,
	0x010,0x011,0x012,0x013,0x014,0x015,0x016,0x017,0x097,0x096,0x095,0x094,0x093,0x092,0x091,0x090,0x210,0x211,0x212,0x213,0x214,0x215,0x216,0x217,0x297,0x296,0x295,0x294,0x293,0x292,0x291,0x290,
	0x008,0x009,0x00a,0x00b,0x00c,0x00d,0x00e,0x00f,0x08f,0x08e,0x08d,0x08c,0x08b,0x08a,0x089,0x088,0x208,0x209,0x20a,0x20b,0x20c,0x20d,0x20e,0x20f,0x28f,0x28e,0x28d,0x28c,0x28b,0x28a,0x289,0x288,
	0x000,0x001,0x002,0x003,0x004,0x005,0x006,0x007,0x087,0x086,0x085,0x084,0x083,0x082,0x081,0x080,0x200,0x201,0x202,0x203,0x204,0x205,0x206,0x207,0x287,0x286,0x285,0x284,0x283,0x282,0x281,0x280,
	0x178,0x179,0x17a,0x17b,0x17c,0x17d,0x17e,0x17f,0x1ff,0x1fe,0x1fd,0x1fc,0x1fb,0x1fa,0x1f9,0x1f8,0x378,0x379,0x37a,0x37b,0x37c,0x37d,0x37e,0x37f,0x3ff,0x3fe,0x3fd,0x3fc,0x3fb,0x3fa,0x3f9,0x3f8,
	0x170,0x171,0x172,0x173,0x174,0x175,0x176,0x177,0x1f7,0x1f6,0x1f5,0x1f4,0x1f3,0x1f2,0x1f1,0x1f0,0x370,0x371,0x372,0x373,0x374,0x375,0x376,0x377,0x3f7,0x3f6,0x3f5,0x3f4,0x3f3,0x3f2,0x3f1,0x3f0,
	0x168,0x169,0x16a,0x16b,0x16c,0x16d,0x16e,0x16f,0x1ef,0x1ee,0x1ed,0x1ec,0x1eb,0x1ea,0x1e9,0x1e8,0x368,0x369,0x36a,0x36b,0x36c,0x36d,0x36e,0x36f,0x3ef,0x3ee,0x3ed,0x3ec,0x3eb,0x3ea,0x3e9,0x3e8,
	0x160,0x161,0x162,0x163,0x164,0x165,0x166,0x167,0x1e7,0x1e6,0x1e5,0x1e4,0x1e3,0x1e2,0x1e1,0x1e0,0x360,0x361,0x362,0x363,0x364,0x365,0x366,0x367,0x3e7,0x3e6,0x3e5,0x3e4,0x3e3,0x3e2,0x3e1,0x3e0,
	0x158,0x159,0x15a,0x15b,0x15c,0x15d,0x15e,0x15f,0x1df,0x1de,0x1dd,0x1dc,0x1db,0x1da,0x1d9,0x1d8,0x358,0x359,0x35a,0x35b,0x35c,0x35d,0x35e,0x35f,0x3df,0x3de,0x3dd,0x3dc,0x3db,0x3da,0x3d9,0x3d8,
	0x150,0x151,0x152,0x153,0x154,0x155,0x156,0x157,0x1d7,0x1d6,0x1d5,0x1d4,0x1d3,0x1d2,0x1d1,0x1d0,0x350,0x351,0x352,0x353,0x354,0x355,0x356,0x357,0x3d7,0x3d6,0x3d5,0x3d4,0x3d3,0x3d2,0x3d1,0x3d0,
	0x148,0x149,0x14a,0x14b,0x14c,0x14d,0x14e,0x14f,0x1cf,0x1ce,0x1cd,0x1cc,0x1cb,0x1ca,0x1c9,0x1c8,0x348,0x349,0x34a,0x34b,0x34c,0x34d,0x34e,0x34f,0x3cf,0x3ce,0x3cd,0x3cc,0x3cb,0x3ca,0x3c9,0x3c8,
	0x140,0x141,0x142,0x143,0x144,0x145,0x146,0x147,0x1c7,0x1c6,0x1c5,0x1c4,0x1c3,0x1c2,0x1c1,0x1c0,0x340,0x341,0x342,0x343,0x344,0x345,0x346,0x347,0x3c7,0x3c6,0x3c5,0x3c4,0x3c3,0x3c2,0x3c1,0x3c0,
	0x138,0x139,0x13a,0x13b,0x13c,0x13d,0x13e,0x13f,0x1bf,0x1be,0x1bd,0x1bc,0x1bb,0x1ba,0x1b9,0x1b8,0x338,0x339,0x33a,0x33b,0x33c,0x33d,0x33e,0x33f,0x3bf,0x3be,0x3bd,0x3bc,0x3bb,0x3ba,0x3b9,0x3b8,
	0x130,0x131,0x132,0x133,0x134,0x135,0x136,0x137,0x1b7,0x1b6,0x1b5,0x1b4,0x1b3,0x1b2,0x1b1,0x1b0,0x330,0x331,0x332,0x333,0x334,0x335,0x336,0x337,0x3b7,0x3b6,0x3b5,0x3b4,0x3b3,0x3b2,0x3b1,0x3b0,
	0x128,0x129,0x12a,0x12b,0x12c,0x12d,0x12e,0x12f,0x1af,0x1ae,0x1ad,0x1ac,0x1ab,0x1aa,0x1a9,0x1a8,0x328,0x329,0x32a,0x32b,0x32c,0x32d,0x32e,0x32f,0x3af,0x3ae,0x3ad,0x3ac,0x3ab,0x3aa,0x3a9,0x3a8,
	0x120,0x121,0x122,0x123,0x124,0x125,0x126,0x127,0x1a7,0x1a6,0x1a5,0x1a4,0x1a3,0x1a2,0x1a1,0x1a0,0x320,0x321,0x322,0x323,0x324,0x325,0x326,0x327,0x3a7,0x3a6,0x3a5,0x3a4,0x3a3,0x3a2,0x3a1,0x3a0,
	0x118,0x119,0x11a,0x11b,0x11c,0x11d,0x11e,0x11f,0x19f,0x19e,0x19d,0x19c,0x19b,0x19a,0x199,0x198,0x318,0x319,0x31a,0x31b,0x31c,0x31d,0x31e,0x31f,0x39f,0x39e,0x39d,0x39c,0x39b,0x39a,0x399,0x398,
	0x110,0x111,0x112,0x113,0x114,0x115,0x116,0x117,0x197,0x196,0x195,0x194,0x193,0x192,0x191,0x190,0x310,0x311,0x312,0x313,0x314,0x315,0x316,0x317,0x397,0x396,0x395,0x394,0x393,0x392,0x391,0x390,
	0x108,0x109,0x10a,0x10b,0x10c,0x10d,0x10e,0x10f,0x18f,0x18e,0x18d,0x18c,0x18b,0x18a,0x189,0x188,0x308,0x309,0x30a,0x30b,0x30c,0x30d,0x30e,0x30f,0x38f,0x38e,0x38d,0x38c,0x38b,0x38a,0x389,0x388,
	0x100,0x101,0x102,0x103,0x104,0x105,0x106,0x107,0x187,0x186,0x185,0x184,0x183,0x182,0x181,0x180,0x300,0x301,0x302,0x303,0x304,0x305,0x306,0x307,0x387,0x386,0x385,0x384,0x383,0x382,0x381,0x380
};

/********************************************
    state saving setup
 ********************************************/
void decocass_video_state_save_init( running_machine &machine )
{
	decocass_state *state = machine.driver_data<decocass_state>();

	state->save_item(NAME(state->m_watchdog_count));
	state->save_item(NAME(state->m_watchdog_flip));
	state->save_item(NAME(state->m_color_missiles));
	state->save_item(NAME(state->m_color_center_bot));
	state->save_item(NAME(state->m_mode_set));
	state->save_item(NAME(state->m_back_h_shift));
	state->save_item(NAME(state->m_back_vl_shift));
	state->save_item(NAME(state->m_back_vr_shift));
	state->save_item(NAME(state->m_part_h_shift));
	state->save_item(NAME(state->m_part_v_shift));
	state->save_item(NAME(state->m_center_h_shift_space));
	state->save_item(NAME(state->m_center_v_shift));
}

/********************************************
    tilemap callbacks
 ********************************************/

static TILEMAP_MAPPER( fgvideoram_scan_cols )
{
	/* logical (col,row) -> memory offset */
	return (num_cols - 1 - col) * num_rows + row;
}

static TILEMAP_MAPPER( bgvideoram_scan_cols )
{
	/* logical (col,row) -> memory offset */
	return tile_offset[col * num_rows + row];
}

static TILE_GET_INFO( get_bg_l_tile_info )
{
	decocass_state *state = machine.driver_data<decocass_state>();
	int color = (state->m_color_center_bot >> 7) & 1;
	SET_TILE_INFO(
			2,
			(0x80 == (tile_index & 0x80)) ? 16 : state->m_bgvideoram[tile_index] >> 4,
			color,
			0);
}

static TILE_GET_INFO( get_bg_r_tile_info )
{
	decocass_state *state = machine.driver_data<decocass_state>();
	int color = (state->m_color_center_bot >> 7) & 1;
	SET_TILE_INFO(
			2,
			(0x00 == (tile_index & 0x80)) ? 16 : state->m_bgvideoram[tile_index] >> 4,
			color,
			TILE_FLIPY);
}

static TILE_GET_INFO( get_fg_tile_info )
{
	decocass_state *state = machine.driver_data<decocass_state>();
	UINT8 code = state->m_fgvideoram[tile_index];
	UINT8 attr = state->m_colorram[tile_index];
	SET_TILE_INFO(
			0,
			256 * (attr & 3) + code,
			state->m_color_center_bot & 1,
			0);
}

/********************************************
    big object
 ********************************************/

static void draw_object( running_machine& machine, bitmap_ind16 &bitmap, const rectangle &cliprect )
{
	decocass_state *state = machine.driver_data<decocass_state>();
	int sx, sy, color;

	if (0 == (state->m_mode_set & 0x80))  /* part_h_enable off? */
		return;

	color = (state->m_color_center_bot >> 4) & 15;

	sy = 192 - (state->m_part_v_shift & 0x7f);

	if (state->m_part_h_shift & 0x80)
		sx = (state->m_part_h_shift & 0x7f) + 1;
	else
		sx = 91 - (state->m_part_h_shift & 0x7f);

	drawgfx_transpen(bitmap, cliprect, machine.gfx[3], 0, color, 0, 0, sx + 64, sy, 0);
	drawgfx_transpen(bitmap, cliprect, machine.gfx[3], 1, color, 0, 0, sx, sy, 0);
	drawgfx_transpen(bitmap, cliprect, machine.gfx[3], 0, color, 0, 1, sx + 64, sy - 64, 0);
	drawgfx_transpen(bitmap, cliprect, machine.gfx[3], 1, color, 0, 1, sx, sy - 64, 0);
}

static void draw_center( running_machine& machine, bitmap_ind16 &bitmap, const rectangle &cliprect )
{
	decocass_state *state = machine.driver_data<decocass_state>();
	int sx, sy, x, y, color;

	color = 0;
	if (state->m_color_center_bot & 0x10)
		color |= 4;
	if (state->m_color_center_bot & 0x20)
		color |= 2;
	if (state->m_color_center_bot & 0x40)
		color |= 1;
	if (state->m_color_center_bot & 0x80)
		color = (color & 4) + ((color << 1) & 2) + ((color >> 1) & 1);

	sy = state->m_center_v_shift;
	sx = (state->m_center_h_shift_space >> 2) & 0x3c;

	for (y = 0; y < 4; y++)
		if ((sy + y) >= cliprect.min_y && (sy + y) <= cliprect.max_y)
		{
			if (((sy + y) & state->m_color_center_bot & 3) == (sy & state->m_color_center_bot & 3))
				for (x = 0; x < 256; x++)
					if (0 != (x & 16) || 0 != (state->m_center_h_shift_space & 1))
						bitmap.pix16(sy + y, (sx + x) & 255) = color;
		}
}

/********************************************
    memory handlers
 ********************************************/

WRITE8_HANDLER( decocass_paletteram_w )
{
	/*
     * RGB output is inverted and A4 is inverted too
     * (ME/ input on 1st paletteram, inverter -> ME/ on 2nd)
     */
	offset = (offset & 31) ^ 16;
	colortable_palette_set_color(space->machine().colortable, offset, MAKE_RGB(pal3bit(~data >> 0), pal3bit(~data >> 3), pal2bit(~data >> 6)));
}

WRITE8_HANDLER( decocass_charram_w )
{
	decocass_state *state = space->machine().driver_data<decocass_state>();
	state->m_charram[offset] = data;
	/* dirty sprite */
	gfx_element_mark_dirty(space->machine().gfx[1], (offset >> 5) & 255);
	/* dirty char */
	gfx_element_mark_dirty(space->machine().gfx[0], (offset >> 3) & 1023);
}


WRITE8_HANDLER( decocass_fgvideoram_w )
{
	decocass_state *state = space->machine().driver_data<decocass_state>();
	state->m_fgvideoram[offset] = data;
	state->m_fg_tilemap->mark_tile_dirty(offset);
}

WRITE8_HANDLER( decocass_colorram_w )
{
	decocass_state *state = space->machine().driver_data<decocass_state>();
	state->m_colorram[offset] = data;
	state->m_fg_tilemap->mark_tile_dirty(offset);
}

static void mark_bg_tile_dirty( running_machine &machine, offs_t offset )
{
	decocass_state *state = machine.driver_data<decocass_state>();
	if (offset & 0x80)
		state->m_bg_tilemap_r->mark_tile_dirty(offset);
	else
		state->m_bg_tilemap_l->mark_tile_dirty(offset);
}

WRITE8_HANDLER( decocass_tileram_w )
{
	decocass_state *state = space->machine().driver_data<decocass_state>();
	state->m_tileram[offset] = data;
	/* dirty tile (64 bytes per tile) */
	gfx_element_mark_dirty(space->machine().gfx[2], (offset / 64) & 15);
	/* first 1KB of tile RAM is shared with tilemap RAM */
	if (offset < state->m_bgvideoram_size)
		mark_bg_tile_dirty(space->machine(), offset);
}

WRITE8_HANDLER( decocass_objectram_w )
{
	decocass_state *state = space->machine().driver_data<decocass_state>();
	state->m_objectram[offset] = data;
	/* dirty the object */
	gfx_element_mark_dirty(space->machine().gfx[3], 0);
	gfx_element_mark_dirty(space->machine().gfx[3], 1);
}

WRITE8_HANDLER( decocass_bgvideoram_w )
{
	decocass_state *state = space->machine().driver_data<decocass_state>();
	state->m_bgvideoram[offset] = data;
	mark_bg_tile_dirty(space->machine(), offset);
}

/* The watchdog is a 4bit counter counting down every frame */
WRITE8_HANDLER( decocass_watchdog_count_w )
{
	decocass_state *state = space->machine().driver_data<decocass_state>();
	LOG(1,("decocass_watchdog_count_w: $%02x\n", data));
	state->m_watchdog_count = data & 0x0f;
}

WRITE8_HANDLER( decocass_watchdog_flip_w )
{
	decocass_state *state = space->machine().driver_data<decocass_state>();
	LOG(1,("decocass_watchdog_flip_w: $%02x\n", data));
	state->m_watchdog_flip = data;
}

WRITE8_HANDLER( decocass_color_missiles_w )
{
	decocass_state *state = space->machine().driver_data<decocass_state>();
	LOG(1,("decocass_color_missiles_w: $%02x\n", data));
	/* only bits D0-D2 and D4-D6 are connected to
     * the color RAM demux:
     * D0-D2 to the IC0 inputs
     * D4-D6 to the IC1 inputs
     */
	state->m_color_missiles = data & 0x77;
}

/*
 * D0 - ??
 * D1 - ??
 * D2 - ptn 1/2
 * D3 - BKG ena
 * D4 - center L on
 * D5 - cross on
 * D6 - tunnel
 * D7 - part h enable
 */
WRITE8_HANDLER( decocass_mode_set_w )
{
	decocass_state *state = space->machine().driver_data<decocass_state>();
	if (data == state->m_mode_set)
		return;
	LOG(1,("decocass_mode_set_w: $%02x (%s%s%s%s%s%s%s%s)\n", data,
		(data & 0x01) ? "D0?" : "",
		(data & 0x02) ? " D1?" : "",
		(data & 0x04) ? " ptn1/2" : "",
		(data & 0x08) ? " bkg_ena" : "",
		(data & 0x10) ? " center_l_on" : "",
		(data & 0x20) ? " cross_on" : "",
		(data & 0x40) ? " tunnel" : "",
		(data & 0x80) ? " part_h_enable" : ""));

	state->m_mode_set = data;
}

WRITE8_HANDLER( decocass_color_center_bot_w )
{
	decocass_state *state = space->machine().driver_data<decocass_state>();
	if (data == state->m_color_center_bot)
		return;
	LOG(1,("decocass_color_center_bot_w: $%02x (color:%d, center_bot:%d)\n", data, data & 3, data >> 4));
	/*
     * D7   CL3/4 (swap 2+4)
     * D6   CL1
     * D5   CL2
     * D4   CL4
     * D3   nc
     * D2   nc
     * D1   CLD4
     * D0   CLD3
     */

	if ((state->m_color_center_bot ^ data) & 0x80)
	{
		state->m_bg_tilemap_r->mark_all_dirty();
		state->m_bg_tilemap_l->mark_all_dirty();
	}
	if ((state->m_color_center_bot ^ data) & 0x01)
		state->m_fg_tilemap->mark_all_dirty();
	state->m_color_center_bot = data;
}

WRITE8_HANDLER( decocass_back_h_shift_w )
{
	decocass_state *state = space->machine().driver_data<decocass_state>();
	if (data == state->m_back_h_shift)
		return;
	LOG(1,("decocass_back_h_shift_w: $%02x\n", data));
	state->m_back_h_shift = data;
}

WRITE8_HANDLER( decocass_back_vl_shift_w )
{
	decocass_state *state = space->machine().driver_data<decocass_state>();
	if (data == state->m_back_vl_shift)
		return;
	LOG(1,("decocass_back_vl_shift_w: $%02x\n", data));
	state->m_back_vl_shift = data;
}

WRITE8_HANDLER( decocass_back_vr_shift_w )
{
	decocass_state *state = space->machine().driver_data<decocass_state>();
	if (data == state->m_back_vr_shift)
		return;
	LOG(1,("decocass_back_vr_shift_w: $%02x\n", data));
	state->m_back_vr_shift = data;
}

WRITE8_HANDLER( decocass_part_h_shift_w )
{
	decocass_state *state = space->machine().driver_data<decocass_state>();
	if (data == state->m_part_v_shift )
		return;
	LOG(1,("decocass_part_h_shift_w: $%02x\n", data));
	state->m_part_h_shift = data;
}

WRITE8_HANDLER( decocass_part_v_shift_w )
{
	decocass_state *state = space->machine().driver_data<decocass_state>();
	if (data == state->m_part_v_shift )
		return;
	LOG(1,("decocass_part_v_shift_w: $%02x\n", data));
	state->m_part_v_shift = data;
}

WRITE8_HANDLER( decocass_center_h_shift_space_w )
{
	decocass_state *state = space->machine().driver_data<decocass_state>();
	if (data == state->m_center_h_shift_space)
		return;
	LOG(1,("decocass_center_h_shift_space_w: $%02x\n", data));
	state->m_center_h_shift_space = data;
}

WRITE8_HANDLER( decocass_center_v_shift_w )
{
	decocass_state *state = space->machine().driver_data<decocass_state>();
	LOG(1,("decocass_center_v_shift_w: $%02x\n", data));
	state->m_center_v_shift = data;
}

/********************************************
    memory handlers
 ********************************************/

static void draw_sprites(running_machine& machine, bitmap_ind16 &bitmap, const rectangle &cliprect, int color,
						int sprite_y_adjust, int sprite_y_adjust_flip_screen,
						UINT8 *sprite_ram, int interleave)
{
	int i,offs;

	/* Draw the sprites */
	for (i = 0, offs = 0; i < 8; i++, offs += 4 * interleave)
	{
		int sx, sy, flipx, flipy;

		if (!(sprite_ram[offs + 0] & 0x01))
			continue;

		sx = 240 - sprite_ram[offs + 3 * interleave];
		sy = 240 - sprite_ram[offs + 2 * interleave];

		flipx = sprite_ram[offs + 0] & 0x04;
		flipy = sprite_ram[offs + 0] & 0x02;

		if (flip_screen_get(machine))
		{
			sx = 240 - sx;
			sy = 240 - sy + sprite_y_adjust_flip_screen;

			flipx = !flipx;
			flipy = !flipy;
		}

		sy -= sprite_y_adjust;

		drawgfx_transpen(bitmap,cliprect, machine.gfx[1],
				sprite_ram[offs + interleave],
				color,
				flipx,flipy,
				sx,sy, 0);

		sy += (flip_screen_get(machine) ? -256 : 256);

		// Wrap around
		drawgfx_transpen(bitmap,cliprect, machine.gfx[1],
				sprite_ram[offs + interleave],
				color,
				flipx,flipy,
				sx,sy, 0);
	}
}


static void draw_missiles(running_machine &machine,bitmap_ind16 &bitmap, const rectangle &cliprect,
						int missile_y_adjust, int missile_y_adjust_flip_screen,
						UINT8 *missile_ram, int interleave)
{
	decocass_state *state = machine.driver_data<decocass_state>();
	int i, offs, x;

	/* Draw the missiles (16 of them) seemingly with alternating colors
     * from the E302 latch (color_missiles) */
	for (i = 0, offs = 0; i < 8; i++, offs += 4 * interleave)
	{
		int sx, sy;

		sy = 255 - missile_ram[offs + 0 * interleave];
		sx = 255 - missile_ram[offs + 2 * interleave];
		if (flip_screen_get(machine))
		{
			sx = 240 - sx;
			sy = 240 - sy + missile_y_adjust_flip_screen;
		}
		sy -= missile_y_adjust;
		if (sy >= cliprect.min_y && sy <= cliprect.max_y)
			for (x = 0; x < 4; x++)
			{
				if (sx >= cliprect.min_x && sx <= cliprect.max_x)
					bitmap.pix16(sy, sx) = (state->m_color_missiles >> 4) & 7;
				sx++;
			}

		sy = 255 - missile_ram[offs + 1 * interleave];
		sx = 255 - missile_ram[offs + 3 * interleave];
		if (flip_screen_get(machine))
		{
			sx = 240 - sx;
			sy = 240 - sy + missile_y_adjust_flip_screen;
		}
		sy -= missile_y_adjust;
		if (sy >= cliprect.min_y && sy <= cliprect.max_y)
			for (x = 0; x < 4; x++)
			{
				if (sx >= cliprect.min_x && sx <= cliprect.max_x)
					bitmap.pix16(sy, sx) = state->m_color_missiles & 7;
				sx++;
			}
	}
}


VIDEO_START( decocass )
{
	decocass_state *state = machine.driver_data<decocass_state>();
	state->m_bg_tilemap_l = tilemap_create(machine, get_bg_l_tile_info, bgvideoram_scan_cols, 16, 16, 32, 32);
	state->m_bg_tilemap_r = tilemap_create(machine, get_bg_r_tile_info, bgvideoram_scan_cols, 16, 16, 32, 32);
	state->m_fg_tilemap = tilemap_create(machine, get_fg_tile_info, fgvideoram_scan_cols, 8, 8, 32, 32);

	state->m_bg_tilemap_l->set_transparent_pen(0);
	state->m_bg_tilemap_r->set_transparent_pen(0);
	state->m_fg_tilemap->set_transparent_pen(0);

	state->m_bg_tilemap_l_clip = machine.primary_screen->visible_area();
	state->m_bg_tilemap_l_clip.max_y = 256 / 2 - 1;

	state->m_bg_tilemap_r_clip = machine.primary_screen->visible_area();
	state->m_bg_tilemap_r_clip.min_y = 256 / 2;

	/* background videroam bits D0-D3 are shared with the tileram */
	state->m_bgvideoram = state->m_tileram;
	state->m_bgvideoram_size = 0x0400;	/* d000-d3ff */

	gfx_element_set_source(machine.gfx[0], state->m_charram);
	gfx_element_set_source(machine.gfx[1], state->m_charram);
	gfx_element_set_source(machine.gfx[2], state->m_tileram);
	gfx_element_set_source(machine.gfx[3], state->m_objectram);

	/* This should ensure that the fake 17th tile is left blank
     * now that dirty-tile tracking is handled by the core */
	gfx_element_decode(machine.gfx[2], 16);
}

SCREEN_UPDATE_IND16( decocass )
{
	decocass_state *state = screen.machine().driver_data<decocass_state>();
	int scrollx, scrolly_l, scrolly_r;
	rectangle clip;

	if (0xc0 != (input_port_read(screen.machine(), "IN2") & 0xc0))  /* coin slots assert an NMI */
		device_set_input_line(state->m_maincpu, INPUT_LINE_NMI, ASSERT_LINE);

	if (0 == (state->m_watchdog_flip & 0x04))
		watchdog_reset(screen.machine());
	else if (state->m_watchdog_count-- > 0)
		watchdog_reset(screen.machine());

#ifdef MAME_DEBUG
	{
		if (screen.machine().input().code_pressed_once(KEYCODE_I))
			state->m_showmsg ^= 1;
		if (state->m_showmsg)
			popmessage("mode:$%02x cm:$%02x ccb:$%02x h:$%02x vl:$%02x vr:$%02x ph:$%02x pv:$%02x ch:$%02x cv:$%02x",
				state->m_mode_set,
				state->m_color_missiles,
				state->m_color_center_bot,
				state->m_back_h_shift,
				state->m_back_vl_shift,
				state->m_back_vr_shift,
				state->m_part_h_shift,
				state->m_part_v_shift,
				state->m_center_h_shift_space,
				state->m_center_v_shift);
	}
#endif

	bitmap.fill(0, cliprect);

	scrolly_l = state->m_back_vl_shift;
	scrolly_r = 256 - state->m_back_vr_shift;

	scrollx = 256 - state->m_back_h_shift;
	if (0 == (state->m_mode_set & 0x02))
		scrollx += 256;

#if 0
/* this is wrong */
	if (0 != state->m_back_h_shift && 0 == ((state->m_mode_set ^ (state->m_mode_set >> 1)) & 1))
		scrollx += 256;
#endif

	if (0 == (state->m_mode_set & 0x04))
		scrolly_r += 256;
	else
		scrolly_l += 256;

	state->m_bg_tilemap_l->set_scrollx(0, scrollx);
	state->m_bg_tilemap_l->set_scrolly(0, scrolly_l);

	state->m_bg_tilemap_r->set_scrollx(0, scrollx);
	state->m_bg_tilemap_r->set_scrolly(0, scrolly_r);

	if (state->m_mode_set & 0x08)	/* bkg_ena on ? */
	{
		clip = state->m_bg_tilemap_l_clip;
		clip &= cliprect;
		state->m_bg_tilemap_l->draw(bitmap, clip, TILEMAP_DRAW_OPAQUE, 0);

		clip = state->m_bg_tilemap_r_clip;
		clip &= cliprect;
		state->m_bg_tilemap_r->draw(bitmap, clip, TILEMAP_DRAW_OPAQUE, 0);
	}

	if (state->m_mode_set & 0x20)
	{
		draw_object(screen.machine(), bitmap, cliprect);
		draw_center(screen.machine(), bitmap, cliprect);
	}
	else
	{
		draw_object(screen.machine(), bitmap, cliprect);
		draw_center(screen.machine(), bitmap, cliprect);
		if (state->m_mode_set & 0x08)	/* bkg_ena on ? */
		{
			clip = state->m_bg_tilemap_l_clip;
			clip &= cliprect;
			state->m_bg_tilemap_l->draw(bitmap, clip, 0, 0);

			clip = state->m_bg_tilemap_r_clip;
			clip &= cliprect;
			state->m_bg_tilemap_r->draw(bitmap, clip, 0, 0);
		}
	}
	state->m_fg_tilemap->draw(bitmap, cliprect, 0, 0);
	draw_sprites(screen.machine(), bitmap, cliprect, (state->m_color_center_bot >> 1) & 1, 0, 0, state->m_fgvideoram, 0x20);
	draw_missiles(screen.machine(), bitmap, cliprect, 1, 0, state->m_colorram, 0x20);
	return 0;
}
