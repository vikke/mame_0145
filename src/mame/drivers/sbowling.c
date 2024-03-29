/**********************************************************
Strike Bowling  (c)1982 Taito

driver by Jaroslaw Burczynski
          Tomasz Slanina

Todo:
 - analog sound
 - colors

***********************************************************

Runs on 3 board (color) hardware, similar to Space Invaders,
but enhanced slightly (more ram, updated sound hardware etc.)

Top Board
---------
PCB No: KBO70001  KBN00001
DIPSW : 8 position x2
SOUND : AY-3-8910
Volume POTs x4 (Master volume + 3 for separate sound levels)

Middle Board
------------
PCB No: KBO70002  KBN00002
CPU   : 8080
XTAL  : 19.968MHz
RAM   : 2114 x2
ROMs  : 2732 x3 (main program)

Bottom Board
------------
PCB No: KBO70003  KBN00003
RAM   : TMS4060 x32
ROMs  : 2716 x3, 2732 x1
PROMs : NEC B406 (1kx4) x2

***********************************************************/

#include "emu.h"
#include "cpu/i8085/i8085.h"
#include "cpu/mcs48/mcs48.h"
#include "video/resnet.h"
#include "sound/ay8910.h"


class sbowling_state : public driver_device
{
public:
	sbowling_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag),
		m_maincpu(*this, "maincpu")
		{ }

	int m_bgmap;
	UINT8 *m_videoram;

	int m_sbw_system;
	tilemap_t *m_sb_tilemap;
	bitmap_ind16 *m_tmpbitmap;
	UINT32 m_color_prom_address;
	UINT8 m_pix_sh;
	UINT8 m_pix[2];

	required_device<cpu_device> m_maincpu;
};

static TILE_GET_INFO( get_sb_tile_info )
{
	sbowling_state *state = machine.driver_data<sbowling_state>();
	UINT8 *rom = machine.region("user1")->base();
	int tileno = rom[tile_index + state->m_bgmap * 1024];

	SET_TILE_INFO(0, tileno, 0, 0);
}

static void plot_pixel_sbw(bitmap_ind16 *tmpbitmap, int x, int y, int col, int flip)
{
	if (flip)
	{
		y = 255-y;
		x = 247-x;
	}
	tmpbitmap->pix16(y, x) = col;
}

static WRITE8_HANDLER( sbw_videoram_w )
{
	sbowling_state *state = space->machine().driver_data<sbowling_state>();
	int flip = flip_screen_get(space->machine());
	int x,y,i,v1,v2;

	state->m_videoram[offset] = data;

	offset &= 0x1fff;

	y = offset / 32;
	x = (offset % 32) * 8;

	v1 = state->m_videoram[offset];
	v2 = state->m_videoram[offset+0x2000];

	for (i = 0; i < 8; i++)
	{
		plot_pixel_sbw(state->m_tmpbitmap, x++, y, state->m_color_prom_address | ( ((v1&1)*0x20) | ((v2&1)*0x40) ), flip);
		v1 >>= 1;
		v2 >>= 1;
	}
}

static SCREEN_UPDATE_IND16(sbowling)
{
	sbowling_state *state = screen.machine().driver_data<sbowling_state>();

	bitmap.fill(0x18, cliprect);
	state->m_sb_tilemap->draw(bitmap, cliprect, 0, 0);
	copybitmap_trans(bitmap, *state->m_tmpbitmap, 0, 0, 0, 0, cliprect, state->m_color_prom_address);
	return 0;
}

static VIDEO_START(sbowling)
{
	sbowling_state *state = machine.driver_data<sbowling_state>();

	state->m_tmpbitmap = auto_bitmap_ind16_alloc(machine,32*8,32*8);
	state->m_sb_tilemap = tilemap_create(machine, get_sb_tile_info, tilemap_scan_rows, 8, 8, 32, 32);
}

static WRITE8_HANDLER( pix_shift_w )
{
	sbowling_state *state = space->machine().driver_data<sbowling_state>();

	state->m_pix_sh = data;
}
static WRITE8_HANDLER( pix_data_w )
{
	sbowling_state *state = space->machine().driver_data<sbowling_state>();

	state->m_pix[0] = state->m_pix[1];
	state->m_pix[1] = data;
}
static READ8_HANDLER( pix_data_r )
{
	sbowling_state *state = space->machine().driver_data<sbowling_state>();
	UINT32 p1, p0;
	int res;
	int sh = state->m_pix_sh & 7;

	p1 = state->m_pix[1];
	p0 = state->m_pix[0];

	res = (((p1 << (sh+8)) | (p0 << sh)) & 0xff00) >> 8;

	return res;
}



static TIMER_DEVICE_CALLBACK( sbw_interrupt )
{
	sbowling_state *state = timer.machine().driver_data<sbowling_state>();
	int scanline = param;

	if(scanline == 256)
		device_set_input_line_and_vector(state->m_maincpu, 0, HOLD_LINE, 0xcf); /* RST 08h */

	if(scanline == 128)
		device_set_input_line_and_vector(state->m_maincpu, 0, HOLD_LINE, 0xd7); /* RST 10h */

}

static WRITE8_HANDLER (system_w)
{
	/*
        76543210
        -------x flip screen/controls?
        ------x- trackball x/y  select
        -----x-- 1 ?
        ----x--- flip screen/controls
    */
	sbowling_state *state = space->machine().driver_data<sbowling_state>();

	flip_screen_set(space->machine(), data&1);

	if ((state->m_sbw_system^data)&1)
	{
		int offs;
		for (offs = 0;offs < 0x4000; offs++)
			sbw_videoram_w(space, offs, state->m_videoram[offs]);
	}
	state->m_sbw_system = data;
}

static WRITE8_HANDLER(graph_control_w)
{
	/*
        76543210
        -----xxx color PROM address lines A9,A8,A7
        ----?--- nc ?
        --xx---- background image select (address lines on tilemap rom)
        xx------ color PROM address lines A4,A3
    */

	sbowling_state *state = space->machine().driver_data<sbowling_state>();

	state->m_color_prom_address = ((data&0x07)<<7) | ((data&0xc0)>>3);

	state->m_bgmap = ((data>>4)^3) & 0x3;
	state->m_sb_tilemap->mark_all_dirty();
}

static READ8_HANDLER (controls_r)
{
	sbowling_state *state = space->machine().driver_data<sbowling_state>();

	if (state->m_sbw_system & 2)
		return input_port_read(space->machine(), "TRACKY");
	else
		return input_port_read(space->machine(), "TRACKX");
}

static ADDRESS_MAP_START( main_map, AS_PROGRAM, 8 )
	AM_RANGE(0x0000, 0x2fff) AM_ROM
	AM_RANGE(0x8000, 0xbfff) AM_RAM_WRITE(sbw_videoram_w) AM_BASE_MEMBER(sbowling_state,m_videoram)
	AM_RANGE(0xf800, 0xf801) AM_DEVWRITE("aysnd", ay8910_address_data_w)
	AM_RANGE(0xf801, 0xf801) AM_DEVREAD("aysnd", ay8910_r)
	AM_RANGE(0xfc00, 0xffff) AM_RAM
ADDRESS_MAP_END


static ADDRESS_MAP_START( port_map, AS_IO, 8 )
	AM_RANGE(0x00, 0x00) AM_READ_PORT("IN0") AM_WRITE(watchdog_reset_w)
	AM_RANGE(0x01, 0x01) AM_READWRITE(controls_r, pix_data_w)
	AM_RANGE(0x02, 0x02) AM_READWRITE(pix_data_r, pix_shift_w)
	AM_RANGE(0x03, 0x03) AM_READ_PORT("IN1") AM_WRITENOP
	AM_RANGE(0x04, 0x04) AM_READ_PORT("DSW0") AM_WRITE(system_w)
	AM_RANGE(0x05, 0x05) AM_READ_PORT("DSW1") AM_WRITE(graph_control_w)
ADDRESS_MAP_END



static INPUT_PORTS_START( sbowling )
	PORT_START("IN0")
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_UNKNOWN )
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_UNKNOWN )
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_UNKNOWN )
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_UNKNOWN )
	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_COIN1   )
	PORT_BIT( 0x20, IP_ACTIVE_HIGH,	IPT_TILT )
	PORT_BIT( 0x40, IP_ACTIVE_HIGH, IPT_UNKNOWN )
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_UNKNOWN )

	PORT_START("IN1")
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_COIN2 )
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_UNKNOWN )
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_UNKNOWN )
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_UNKNOWN )
	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_BUTTON1 )
	PORT_BIT( 0x20, IP_ACTIVE_HIGH,	IPT_UNKNOWN )
	PORT_BIT( 0x40, IP_ACTIVE_HIGH, IPT_START1 )
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_START2 )

	PORT_START("TRACKY")
	PORT_BIT( 0xff, 0, IPT_TRACKBALL_Y ) PORT_SENSITIVITY(30) PORT_KEYDELTA(30)

	PORT_START("TRACKX")
	PORT_BIT( 0xff, 0, IPT_TRACKBALL_X ) PORT_SENSITIVITY(30) PORT_KEYDELTA(30) PORT_REVERSE

	PORT_START("DSW0")	/* coin slots: A 4 LSB, B 4 MSB */
	PORT_DIPNAME( 0x0f, 0x00, DEF_STR( Coin_A ) )  PORT_DIPLOCATION("SW1:!1,!2,!3,!4")
	PORT_DIPSETTING(    0x0f, DEF_STR( 9C_1C ) )
	PORT_DIPSETTING(    0x0e, DEF_STR( 8C_1C ) )
	PORT_DIPSETTING(    0x0d, DEF_STR( 7C_1C ) )
	PORT_DIPSETTING(    0x0c, DEF_STR( 6C_1C ) )
	PORT_DIPSETTING(    0x0b, DEF_STR( 5C_1C ) )
	PORT_DIPSETTING(    0x0a, DEF_STR( 4C_1C ) )
	PORT_DIPSETTING(    0x09, DEF_STR( 3C_1C ) )
	PORT_DIPSETTING(    0x08, DEF_STR( 2C_1C ) )
	PORT_DIPSETTING(    0x00, DEF_STR( 1C_1C ) )
	PORT_DIPSETTING(    0x01, DEF_STR( 1C_2C ) )
	PORT_DIPSETTING(    0x02, DEF_STR( 1C_3C ) )
	PORT_DIPSETTING(    0x03, DEF_STR( 1C_4C ) )
	PORT_DIPSETTING(    0x04, DEF_STR( 1C_5C ) )
	PORT_DIPSETTING(    0x05, DEF_STR( 1C_6C ) )
	PORT_DIPSETTING(    0x06, DEF_STR( 1C_7C ) )
	PORT_DIPSETTING(    0x07, DEF_STR( 1C_8C ) )

	PORT_DIPNAME( 0xf0, 0x00, DEF_STR( Coin_B ) )  PORT_DIPLOCATION("SW1:!5,!6,!7,!8")
	PORT_DIPSETTING(    0xf0, DEF_STR( 9C_1C ) )
	PORT_DIPSETTING(    0xe0, DEF_STR( 8C_1C ) )
	PORT_DIPSETTING(    0xd0, DEF_STR( 7C_1C ) )
	PORT_DIPSETTING(    0xc0, DEF_STR( 6C_1C ) )
	PORT_DIPSETTING(    0xb0, DEF_STR( 5C_1C ) )
	PORT_DIPSETTING(    0xa0, DEF_STR( 4C_1C ) )
	PORT_DIPSETTING(    0x90, DEF_STR( 3C_1C ) )
	PORT_DIPSETTING(    0x80, DEF_STR( 2C_1C ) )
	PORT_DIPSETTING(    0x00, DEF_STR( 1C_1C ) )
	PORT_DIPSETTING(    0x10, DEF_STR( 1C_2C ) )
	PORT_DIPSETTING(    0x20, DEF_STR( 1C_3C ) )
	PORT_DIPSETTING(    0x30, DEF_STR( 1C_4C ) )
	PORT_DIPSETTING(    0x40, DEF_STR( 1C_5C ) )
	PORT_DIPSETTING(    0x50, DEF_STR( 1C_6C ) )
	PORT_DIPSETTING(    0x60, DEF_STR( 1C_7C ) )
	PORT_DIPSETTING(    0x70, DEF_STR( 1C_8C ) )

	PORT_START("DSW1")
	PORT_DIPNAME( 0x01, 0x01, DEF_STR( Cabinet ) )  PORT_DIPLOCATION("SW2:!1")
	PORT_DIPSETTING(    0x01, DEF_STR( Upright ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Cocktail ) )
	PORT_DIPUNKNOWN_DIPLOC( 0x02, 0x00, "SW2:!2" )
	PORT_DIPNAME( 0x04, 0x00, "Year Display" )  PORT_DIPLOCATION("SW2:!3")
	PORT_DIPSETTING(    0x04, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPUNKNOWN_DIPLOC( 0x08, 0x00, "SW2:!4" )
	PORT_DIPUNKNOWN_DIPLOC( 0x10, 0x00, "SW2:!5" )
	PORT_DIPUNKNOWN_DIPLOC( 0x20, 0x00, "SW2:!6" )
	PORT_DIPNAME( 0x40, 0x00, "Ball Control Check" )  PORT_DIPLOCATION("SW2:!7")
	PORT_DIPSETTING(    0x00, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x40, DEF_STR( On ) )
	PORT_DIPNAME( 0x80, 0x00, "Video Test" )  PORT_DIPLOCATION("SW2:!8")
	PORT_DIPSETTING(    0x00, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x80, DEF_STR( On ) )
INPUT_PORTS_END

static const gfx_layout charlayout =
{
	8,8,
	RGN_FRAC(1,3),
	3,
	{ RGN_FRAC(0,3), RGN_FRAC(1,3), RGN_FRAC(2,3) },
	{ 7, 6, 5, 4, 3, 2, 1, 0 },
	{ 0*8, 1*8, 2*8, 3*8, 4*8, 5*8, 6*8, 7*8 },
	8*8
};

static GFXDECODE_START( sbowling )
	GFXDECODE_ENTRY( "gfx1", 0, charlayout,   0x18, 1 )
GFXDECODE_END


static PALETTE_INIT( sbowling )
{
	int i;

	static const int resistances_rg[3] = { 470, 270, 100 };
	static const int resistances_b[2]  = { 270, 100 };
	double outputs_r[1<<3], outputs_g[1<<3], outputs_b[1<<2];

	/* the game uses output collector PROMs type: NEC B406  */
	compute_resistor_net_outputs(0, 255,	-1.0,
		3,	resistances_rg, outputs_r,	0,	100,
		3,	resistances_rg, outputs_g,	0,	100,
		2,	resistances_b,  outputs_b,	0,	100);

	for (i = 0;i < machine.total_colors();i++)
	{
		int bit0,bit1,bit2,r,g,b;

		/* blue component */
		bit0 = (color_prom[i] >> 0) & 0x01;
		bit1 = (color_prom[i] >> 1) & 0x01;
		b = (int)(outputs_b[ (bit0<<0) | (bit1<<1) ] + 0.5);

		/* green component */
		bit0 = (color_prom[i] >> 2) & 0x01;
		bit1 = (color_prom[i] >> 3) & 0x01;
		bit2 = (color_prom[i+0x400] >> 0) & 0x01;
		g = (int)(outputs_g[ (bit0<<0) | (bit1<<1) | (bit2<<2) ] + 0.5);

		/* red component */
		bit0 = (color_prom[i+0x400] >> 1) & 0x01;
		bit1 = (color_prom[i+0x400] >> 2) & 0x01;
		bit2 = (color_prom[i+0x400] >> 3) & 0x01;
		r = (int)(outputs_r[ (bit0<<0) | (bit1<<1) | (bit2<<2) ] + 0.5);

		palette_set_color(machine,i,MAKE_RGB(r,g,b));
	}
}

static MACHINE_CONFIG_START( sbowling, sbowling_state )
	MCFG_CPU_ADD("maincpu", I8080, XTAL_19_968MHz/10)	/* ? */
	MCFG_CPU_PROGRAM_MAP(main_map)
	MCFG_CPU_IO_MAP(port_map)
	MCFG_TIMER_ADD_SCANLINE("scantimer", sbw_interrupt, "screen", 0, 1)

	/* video hardware */
	MCFG_SCREEN_ADD("screen", RASTER)
	MCFG_SCREEN_REFRESH_RATE(60)
	MCFG_SCREEN_SIZE(32*8, 262)		/* vert size taken from mw8080bw */
	MCFG_SCREEN_VISIBLE_AREA(1*8, 31*8-1, 4*8, 32*8-1)
	MCFG_SCREEN_UPDATE_STATIC(sbowling)

	MCFG_GFXDECODE(sbowling)

	MCFG_PALETTE_LENGTH(0x400)
	MCFG_PALETTE_INIT(sbowling)
	MCFG_VIDEO_START(sbowling)

	/* sound hardware */
	MCFG_SPEAKER_STANDARD_MONO("mono")

	MCFG_SOUND_ADD("aysnd", AY8910, XTAL_19_968MHz/16)	/* ? */
	MCFG_SOUND_ROUTE(ALL_OUTPUTS, "mono", 0.33)
MACHINE_CONFIG_END

ROM_START( sbowling )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "kb01.6h",        0x0000, 0x1000, CRC(dd5d411a) SHA1(ca15676d234353bc47f642be13d58f3d6d880126))
	ROM_LOAD( "kb02.5h",        0x1000, 0x1000, CRC(75d3c45f) SHA1(af6e6237b7b28efaac258e6ddd85518c3406b24a))
	ROM_LOAD( "kb03.3h",        0x2000, 0x1000, CRC(955fbfb8) SHA1(05d501f924adc5b816670f6f5e58a98a0c1bc962))

	ROM_REGION( 0x1800, "gfx1", 0 )
	ROM_LOAD( "kb05.9k",        0x0000, 0x800,  CRC(4b4d9569) SHA1(d69e69add69ec11724090e34838ec8c61de81f4e))
	ROM_LOAD( "kb06.7k",        0x0800, 0x800,  CRC(d89ba78b) SHA1(9e01be976e1e14feb8f7bd9f699a977a15a72e0d))
	ROM_LOAD( "kb07.6k",        0x1000, 0x800,  CRC(9fb5db1a) SHA1(0b28ca5277ebe0d78d1a3f2d414efb5fd7c6e9ee))

	ROM_REGION( 0x01000, "user1", 0 )
	ROM_LOAD( "kb04.10k",       0x0000, 0x1000, CRC(1c27adc1) SHA1(a68748fbdbd8fb48f20b3675d793e5c156d1bd02))

	ROM_REGION( 0x0800, "proms", 0 )
	ROM_LOAD( "kb08.7m",        0x0000, 0x0400, CRC(e949e441) SHA1(8e0fe71ed6d4e6f94a703c27a8364da27b443730))
	ROM_LOAD( "kb09.6m",        0x0400, 0x0400, CRC(e29191a6) SHA1(9a2c78a96ef6d118f4dacbea0b7d454b66a452ae))
ROM_END

GAME( 1982, sbowling, 0, sbowling, sbowling, 0, ROT90, "Taito Corporation", "Strike Bowling",GAME_IMPERFECT_SOUND)
