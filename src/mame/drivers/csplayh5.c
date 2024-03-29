/***************************************************************************

    'High Rate DVD' HW (c) 1998 Nichibutsu

    preliminary driver by Angelo Salese

    TODO:
    - rewrite v9938/v9958 video chip;
    - fix h8 CPU core bugs, it trips various unhandled opcodes
    - Implement DVD routing and YUV decoding;
    - game timings seem busted, could be due of missing DVD hook-up

***************************************************************************/

#include "emu.h"
#include "cpu/m68000/m68000.h"
#include "machine/tmp68301.h"
#include "video/v9938.h"
#include "cpu/z80/z80.h"
#include "machine/z80ctc.h"
#include "sound/dac.h"
#include "sound/3812intf.h"
#include "cpu/z80/z80daisy.h"
#include "machine/nvram.h"
#include "cpu/h83002/h8.h"


class csplayh5_state : public driver_device
{
public:
	csplayh5_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag),
		m_maincpu(*this,"maincpu"),
		m_v9958(*this,"v9958")
		{ }

	UINT16 m_mux_data;
	UINT8 m_pio_dir[5];
	UINT8 m_pio_latch[5];

	required_device<cpu_device> m_maincpu;
	required_device<v9958_device> m_v9958;
};



#define USE_H8 0

// from MSX2 driver, may be not accurate for this HW
#define MSX2_XBORDER_PIXELS		16
#define MSX2_YBORDER_PIXELS		28
#define MSX2_TOTAL_XRES_PIXELS		256 * 2 + (MSX2_XBORDER_PIXELS * 2)
#define MSX2_TOTAL_YRES_PIXELS		212 * 2 + (MSX2_YBORDER_PIXELS * 2)
#define MSX2_VISIBLE_XBORDER_PIXELS	8 * 2
#define MSX2_VISIBLE_YBORDER_PIXELS	14 * 2

static void csplayh5_vdp0_interrupt(device_t *, v99x8_device &device, int i)
{
	/* this is not used as the v9938 interrupt callbacks are broken
       interrupts seem to be fired quite randomly */
}

static READ16_HANDLER( csplayh5_mux_r )
{
	csplayh5_state *state = space->machine().driver_data<csplayh5_state>();
	switch(state->m_mux_data)
	{
		case 0x01: return input_port_read(space->machine(), "KEY0");
		case 0x02: return input_port_read(space->machine(), "KEY1");
		case 0x04: return input_port_read(space->machine(), "KEY2");
		case 0x08: return input_port_read(space->machine(), "KEY3");
		case 0x10: return input_port_read(space->machine(), "KEY4");
	}

	return 0xffff;
}

static WRITE16_HANDLER( csplayh5_mux_w )
{
	csplayh5_state *state = space->machine().driver_data<csplayh5_state>();
	state->m_mux_data = (~data & 0x1f);
}

static WRITE16_HANDLER( csplayh5_sound_w )
{
	soundlatch_w(space, 0, ((data >> 8) & 0xff));
}


static ADDRESS_MAP_START( csplayh5_map, AS_PROGRAM, 16 )
	AM_RANGE(0x000000, 0x03ffff) AM_ROM

	AM_RANGE(0x200000, 0x200001) AM_READ_PORT("DSW") AM_WRITE(csplayh5_sound_w)
	AM_RANGE(0x200200, 0x200201) AM_READWRITE(csplayh5_mux_r,csplayh5_mux_w)
	AM_RANGE(0x200400, 0x200401) AM_READ_PORT("SYSTEM")

	AM_RANGE(0x200600, 0x200607) AM_DEVREADWRITE8_MODERN("v9958", v9958_device, read, write, 0x00ff)

	AM_RANGE(0x800000, 0xbfffff) AM_ROM AM_REGION("blit_gfx",0) // GFX ROM routes here

	AM_RANGE(0xc00000, 0xc7ffff) AM_RAM AM_SHARE("nvram") AM_MIRROR(0x380000) // work RAM

	AM_RANGE(0xfffc00, 0xffffff) AM_READWRITE(tmp68301_regs_r, tmp68301_regs_w)	// TMP68301 Registers
ADDRESS_MAP_END

#if USE_H8
static READ16_HANDLER( test_r )
{
	return space->machine().rand();
}

static ADDRESS_MAP_START( csplayh5_sub_map, AS_PROGRAM, 16 )
	AM_RANGE(0x000000, 0x01ffff) AM_ROM

	AM_RANGE(0x04002a, 0x04002b) AM_READ(test_r)
	AM_RANGE(0x040036, 0x040037) AM_READ(test_r)

	AM_RANGE(0x078000, 0x07ffff) AM_RAM AM_SHARE("nvram")
	AM_RANGE(0x080000, 0x0fffff) AM_RAM
ADDRESS_MAP_END


static ADDRESS_MAP_START( csplayh5_sub_io_map, AS_IO, 16 )

ADDRESS_MAP_END
#endif


/*
sound HW is identical to Niyanpai
*/

/* TMPZ84C011 PIO emulation */

#define SIGNED_DAC	0		// 0:unsigned DAC, 1:signed DAC
#if SIGNED_DAC
#define DAC_WRITE	dac_signed_w
#else
#define DAC_WRITE	dac_w
#endif

static void csplayh5_soundbank_w(running_machine &machine, int data)
{
	UINT8 *SNDROM = machine.region("audiocpu")->base();

	memory_set_bankptr(machine, "bank1", &SNDROM[0x08000 + (0x8000 * (data & 0x03))]);
}

static READ8_HANDLER( csplayh5_sound_r )
{
	return soundlatch_r(space, 0);
}

static WRITE8_HANDLER( csplayh5_soundclr_w )
{
	soundlatch_clear_w(space, 0, 0);
}

static READ8_HANDLER( tmpz84c011_pio_r )
{
	int portdata;

	switch (offset)
	{
		case 0:			/* PA_0 */
			portdata = 0xff;
			break;
		case 1:			/* PB_0 */
			portdata = 0xff;
			break;
		case 2:			/* PC_0 */
			portdata = 0xff;
			break;
		case 3:			/* PD_0 */
			portdata = csplayh5_sound_r(space, 0);
			break;
		case 4:			/* PE_0 */
			portdata = 0xff;
			break;

		default:
			logerror("%s: TMPZ84C011_PIO Unknown Port Read %02X\n", space->machine().describe_context(), offset);
			portdata = 0xff;
			break;
	}

	return portdata;
}

static WRITE8_HANDLER( tmpz84c011_pio_w)
{
	switch (offset)
	{
		case 0:			/* PA_0 */
			csplayh5_soundbank_w(space->machine(), data & 0x03);
			break;
		case 1:			/* PB_0 */
			DAC_WRITE(space->machine().device("dac2"), 0, data);
			break;
		case 2:			/* PC_0 */
			DAC_WRITE(space->machine().device("dac1"), 0, data);
			break;
		case 3:			/* PD_0 */
			break;
		case 4:			/* PE_0 */
			if (!(data & 0x01)) csplayh5_soundclr_w(space, 0, 0);
			break;

		default:
			logerror("%s: TMPZ84C011_PIO Unknown Port Write %02X, %02X\n", space->machine().describe_context(), offset, data);
			break;
	}
}


/* CPU interface */
static READ8_HANDLER( tmpz84c011_0_pa_r )
{
	csplayh5_state *state = space->machine().driver_data<csplayh5_state>();
	return (tmpz84c011_pio_r(space,0) & ~state->m_pio_dir[0]) | (state->m_pio_latch[0] & state->m_pio_dir[0]);
}

static READ8_HANDLER( tmpz84c011_0_pb_r )
{
	csplayh5_state *state = space->machine().driver_data<csplayh5_state>();
	return (tmpz84c011_pio_r(space,1) & ~state->m_pio_dir[1]) | (state->m_pio_latch[1] & state->m_pio_dir[1]);
}

static READ8_HANDLER( tmpz84c011_0_pc_r )
{
	csplayh5_state *state = space->machine().driver_data<csplayh5_state>();
	return (tmpz84c011_pio_r(space,2) & ~state->m_pio_dir[2]) | (state->m_pio_latch[2] & state->m_pio_dir[2]);
}

static READ8_HANDLER( tmpz84c011_0_pd_r )
{
	csplayh5_state *state = space->machine().driver_data<csplayh5_state>();
	return (tmpz84c011_pio_r(space,3) & ~state->m_pio_dir[3]) | (state->m_pio_latch[3] & state->m_pio_dir[3]);
}

static READ8_HANDLER( tmpz84c011_0_pe_r )
{
	csplayh5_state *state = space->machine().driver_data<csplayh5_state>();
	return (tmpz84c011_pio_r(space,4) & ~state->m_pio_dir[4]) | (state->m_pio_latch[4] & state->m_pio_dir[4]);
}

static WRITE8_HANDLER( tmpz84c011_0_pa_w )
{
	csplayh5_state *state = space->machine().driver_data<csplayh5_state>();
	state->m_pio_latch[0] = data;
	tmpz84c011_pio_w(space, 0, data);
}

static WRITE8_HANDLER( tmpz84c011_0_pb_w )
{
	csplayh5_state *state = space->machine().driver_data<csplayh5_state>();
	state->m_pio_latch[1] = data;
	tmpz84c011_pio_w(space, 1, data);
}

static WRITE8_HANDLER( tmpz84c011_0_pc_w )
{
	csplayh5_state *state = space->machine().driver_data<csplayh5_state>();
	state->m_pio_latch[2] = data;
	tmpz84c011_pio_w(space, 2, data);
}

static WRITE8_HANDLER( tmpz84c011_0_pd_w )
{
	csplayh5_state *state = space->machine().driver_data<csplayh5_state>();
	state->m_pio_latch[3] = data;
	tmpz84c011_pio_w(space, 3, data);
}

static WRITE8_HANDLER( tmpz84c011_0_pe_w )
{
	csplayh5_state *state = space->machine().driver_data<csplayh5_state>();
	state->m_pio_latch[4] = data;
	tmpz84c011_pio_w(space, 4, data);
}


static READ8_HANDLER( tmpz84c011_0_dir_pa_r )
{
	csplayh5_state *state = space->machine().driver_data<csplayh5_state>();
	return state->m_pio_dir[0];
}

static READ8_HANDLER( tmpz84c011_0_dir_pb_r )
{
	csplayh5_state *state = space->machine().driver_data<csplayh5_state>();
	return state->m_pio_dir[1];
}

static READ8_HANDLER( tmpz84c011_0_dir_pc_r )
{
	csplayh5_state *state = space->machine().driver_data<csplayh5_state>();
	return state->m_pio_dir[2];
}

static READ8_HANDLER( tmpz84c011_0_dir_pd_r )
{
	csplayh5_state *state = space->machine().driver_data<csplayh5_state>();
	return state->m_pio_dir[3];
}

static READ8_HANDLER( tmpz84c011_0_dir_pe_r )
{
	csplayh5_state *state = space->machine().driver_data<csplayh5_state>();
	return state->m_pio_dir[4];
}


static WRITE8_HANDLER( tmpz84c011_0_dir_pa_w )
{
	csplayh5_state *state = space->machine().driver_data<csplayh5_state>();
	state->m_pio_dir[0] = data;
}

static WRITE8_HANDLER( tmpz84c011_0_dir_pb_w )
{
	csplayh5_state *state = space->machine().driver_data<csplayh5_state>();
	state->m_pio_dir[1] = data;
}

static WRITE8_HANDLER( tmpz84c011_0_dir_pc_w )
{
	csplayh5_state *state = space->machine().driver_data<csplayh5_state>();
	state->m_pio_dir[2] = data;
}

static WRITE8_HANDLER( tmpz84c011_0_dir_pd_w )
{
	csplayh5_state *state = space->machine().driver_data<csplayh5_state>();
	state->m_pio_dir[3] = data;
}

static WRITE8_HANDLER( tmpz84c011_0_dir_pe_w )
{
	csplayh5_state *state = space->machine().driver_data<csplayh5_state>();
	state->m_pio_dir[4] = data;
}


static ADDRESS_MAP_START( csplayh5_sound_map, AS_PROGRAM, 8 )
	AM_RANGE(0x0000, 0x77ff) AM_ROM
	AM_RANGE(0x7800, 0x7fff) AM_RAM
	AM_RANGE(0x8000, 0xffff) AM_ROMBANK("bank1")
ADDRESS_MAP_END

static ADDRESS_MAP_START( csplayh5_sound_io_map, AS_IO, 8 )
	ADDRESS_MAP_GLOBAL_MASK(0xff)
	AM_RANGE(0x10, 0x13) AM_DEVREADWRITE("ctc", z80ctc_r, z80ctc_w)
	AM_RANGE(0x50, 0x50) AM_READWRITE(tmpz84c011_0_pa_r, tmpz84c011_0_pa_w)
	AM_RANGE(0x51, 0x51) AM_READWRITE(tmpz84c011_0_pb_r, tmpz84c011_0_pb_w)
	AM_RANGE(0x52, 0x52) AM_READWRITE(tmpz84c011_0_pc_r, tmpz84c011_0_pc_w)
	AM_RANGE(0x30, 0x30) AM_READWRITE(tmpz84c011_0_pd_r, tmpz84c011_0_pd_w)
	AM_RANGE(0x40, 0x40) AM_READWRITE(tmpz84c011_0_pe_r, tmpz84c011_0_pe_w)
	AM_RANGE(0x54, 0x54) AM_READWRITE(tmpz84c011_0_dir_pa_r, tmpz84c011_0_dir_pa_w)
	AM_RANGE(0x55, 0x55) AM_READWRITE(tmpz84c011_0_dir_pb_r, tmpz84c011_0_dir_pb_w)
	AM_RANGE(0x56, 0x56) AM_READWRITE(tmpz84c011_0_dir_pc_r, tmpz84c011_0_dir_pc_w)
	AM_RANGE(0x34, 0x34) AM_READWRITE(tmpz84c011_0_dir_pd_r, tmpz84c011_0_dir_pd_w)
	AM_RANGE(0x44, 0x44) AM_READWRITE(tmpz84c011_0_dir_pe_r, tmpz84c011_0_dir_pe_w)
	AM_RANGE(0x80, 0x81) AM_DEVWRITE("ymsnd", ym3812_w)
ADDRESS_MAP_END


static INPUT_PORTS_START( csplayh5 )
	PORT_START("KEY0")
	PORT_BIT( 0x0001, IP_ACTIVE_LOW, IPT_START1 )
	PORT_BIT( 0x0002, IP_ACTIVE_LOW, IPT_MAHJONG_KAN ) PORT_PLAYER(1)
	PORT_BIT( 0x0004, IP_ACTIVE_LOW, IPT_MAHJONG_M ) PORT_PLAYER(1)
	PORT_BIT( 0x0008, IP_ACTIVE_LOW, IPT_MAHJONG_I ) PORT_PLAYER(1)
	PORT_BIT( 0x0010, IP_ACTIVE_LOW, IPT_MAHJONG_E ) PORT_PLAYER(1)
	PORT_BIT( 0x0020, IP_ACTIVE_LOW, IPT_MAHJONG_A ) PORT_PLAYER(1)
	PORT_BIT( 0x0040, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x0080, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x0100, IP_ACTIVE_LOW, IPT_START2 )
	PORT_BIT( 0x0200, IP_ACTIVE_LOW, IPT_MAHJONG_KAN ) PORT_PLAYER(2)
	PORT_BIT( 0x0400, IP_ACTIVE_LOW, IPT_MAHJONG_M ) PORT_PLAYER(2)
	PORT_BIT( 0x0800, IP_ACTIVE_LOW, IPT_MAHJONG_I ) PORT_PLAYER(2)
	PORT_BIT( 0x1000, IP_ACTIVE_LOW, IPT_MAHJONG_E ) PORT_PLAYER(2)
	PORT_BIT( 0x2000, IP_ACTIVE_LOW, IPT_MAHJONG_A ) PORT_PLAYER(2)
	PORT_BIT( 0x4000, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x8000, IP_ACTIVE_LOW, IPT_UNKNOWN )

	PORT_START("KEY1")
	PORT_BIT( 0x0001, IP_ACTIVE_LOW, IPT_MAHJONG_BET ) PORT_PLAYER(1)
	PORT_BIT( 0x0002, IP_ACTIVE_LOW, IPT_MAHJONG_REACH ) PORT_PLAYER(1)
	PORT_BIT( 0x0004, IP_ACTIVE_LOW, IPT_MAHJONG_N ) PORT_PLAYER(1)
	PORT_BIT( 0x0008, IP_ACTIVE_LOW, IPT_MAHJONG_J ) PORT_PLAYER(1)
	PORT_BIT( 0x0010, IP_ACTIVE_LOW, IPT_MAHJONG_F ) PORT_PLAYER(1)
	PORT_BIT( 0x0020, IP_ACTIVE_LOW, IPT_MAHJONG_B ) PORT_PLAYER(1)
	PORT_BIT( 0x0040, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x0080, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x0100, IP_ACTIVE_LOW, IPT_MAHJONG_BET ) PORT_PLAYER(2)
	PORT_BIT( 0x0200, IP_ACTIVE_LOW, IPT_MAHJONG_REACH ) PORT_PLAYER(2)
	PORT_BIT( 0x0400, IP_ACTIVE_LOW, IPT_MAHJONG_N ) PORT_PLAYER(2)
	PORT_BIT( 0x0800, IP_ACTIVE_LOW, IPT_MAHJONG_J ) PORT_PLAYER(2)
	PORT_BIT( 0x1000, IP_ACTIVE_LOW, IPT_MAHJONG_F ) PORT_PLAYER(2)
	PORT_BIT( 0x2000, IP_ACTIVE_LOW, IPT_MAHJONG_B ) PORT_PLAYER(2)
	PORT_BIT( 0x4000, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x8000, IP_ACTIVE_LOW, IPT_UNKNOWN )

	PORT_START("KEY2")
	PORT_BIT( 0x0001, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x0002, IP_ACTIVE_LOW, IPT_MAHJONG_RON ) PORT_PLAYER(1)
	PORT_BIT( 0x0004, IP_ACTIVE_LOW, IPT_MAHJONG_CHI ) PORT_PLAYER(1)
	PORT_BIT( 0x0008, IP_ACTIVE_LOW, IPT_MAHJONG_K ) PORT_PLAYER(1)
	PORT_BIT( 0x0010, IP_ACTIVE_LOW, IPT_MAHJONG_G ) PORT_PLAYER(1)
	PORT_BIT( 0x0020, IP_ACTIVE_LOW, IPT_MAHJONG_C ) PORT_PLAYER(1)
	PORT_BIT( 0x0040, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x0080, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x0100, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x0200, IP_ACTIVE_LOW, IPT_MAHJONG_RON ) PORT_PLAYER(2)
	PORT_BIT( 0x0400, IP_ACTIVE_LOW, IPT_MAHJONG_CHI ) PORT_PLAYER(2)
	PORT_BIT( 0x0800, IP_ACTIVE_LOW, IPT_MAHJONG_K ) PORT_PLAYER(2)
	PORT_BIT( 0x1000, IP_ACTIVE_LOW, IPT_MAHJONG_G ) PORT_PLAYER(2)
	PORT_BIT( 0x2000, IP_ACTIVE_LOW, IPT_MAHJONG_C ) PORT_PLAYER(2)
	PORT_BIT( 0x4000, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x8000, IP_ACTIVE_LOW, IPT_UNKNOWN )

	PORT_START("KEY3")
	PORT_BIT( 0x0001, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x0002, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x0004, IP_ACTIVE_LOW, IPT_MAHJONG_PON ) PORT_PLAYER(1)
	PORT_BIT( 0x0008, IP_ACTIVE_LOW, IPT_MAHJONG_L ) PORT_PLAYER(1)
	PORT_BIT( 0x0010, IP_ACTIVE_LOW, IPT_MAHJONG_H ) PORT_PLAYER(1)
	PORT_BIT( 0x0020, IP_ACTIVE_LOW, IPT_MAHJONG_D ) PORT_PLAYER(1)
	PORT_BIT( 0x0040, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x0080, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x0100, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x0200, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x0400, IP_ACTIVE_LOW, IPT_MAHJONG_PON ) PORT_PLAYER(2)
	PORT_BIT( 0x0800, IP_ACTIVE_LOW, IPT_MAHJONG_L ) PORT_PLAYER(2)
	PORT_BIT( 0x1000, IP_ACTIVE_LOW, IPT_MAHJONG_H ) PORT_PLAYER(2)
	PORT_BIT( 0x2000, IP_ACTIVE_LOW, IPT_MAHJONG_D ) PORT_PLAYER(2)
	PORT_BIT( 0x4000, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x8000, IP_ACTIVE_LOW, IPT_UNKNOWN )

	PORT_START("KEY4")
	PORT_BIT( 0x0001, IP_ACTIVE_LOW, IPT_MAHJONG_SMALL ) PORT_PLAYER(1)
	PORT_BIT( 0x0002, IP_ACTIVE_LOW, IPT_MAHJONG_BIG ) PORT_PLAYER(1)
	PORT_BIT( 0x0004, IP_ACTIVE_LOW, IPT_MAHJONG_FLIP_FLOP ) PORT_PLAYER(1)
	PORT_BIT( 0x0008, IP_ACTIVE_LOW, IPT_MAHJONG_DOUBLE_UP ) PORT_PLAYER(1)
	PORT_BIT( 0x0010, IP_ACTIVE_LOW, IPT_MAHJONG_SCORE ) PORT_PLAYER(1)
	PORT_BIT( 0x0020, IP_ACTIVE_LOW, IPT_MAHJONG_LAST_CHANCE ) PORT_PLAYER(1)
	PORT_BIT( 0x0040, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x0080, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x0100, IP_ACTIVE_LOW, IPT_MAHJONG_SMALL ) PORT_PLAYER(2)
	PORT_BIT( 0x0200, IP_ACTIVE_LOW, IPT_MAHJONG_BIG ) PORT_PLAYER(2)
	PORT_BIT( 0x0400, IP_ACTIVE_LOW, IPT_MAHJONG_FLIP_FLOP ) PORT_PLAYER(2)
	PORT_BIT( 0x0800, IP_ACTIVE_LOW, IPT_MAHJONG_DOUBLE_UP ) PORT_PLAYER(2)
	PORT_BIT( 0x1000, IP_ACTIVE_LOW, IPT_MAHJONG_SCORE ) PORT_PLAYER(2)
	PORT_BIT( 0x2000, IP_ACTIVE_LOW, IPT_MAHJONG_LAST_CHANCE ) PORT_PLAYER(2)
	PORT_BIT( 0x4000, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x8000, IP_ACTIVE_LOW, IPT_UNKNOWN )

	PORT_START("DSW")
	PORT_DIPNAME( 0x0001, 0x0000, "DSWA" )
	PORT_DIPSETTING(      0x0000, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x0001, DEF_STR( On ) )
	PORT_DIPNAME( 0x0002, 0x0000, DEF_STR( Unknown ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x0002, DEF_STR( On ) )
	PORT_DIPNAME( 0x0004, 0x0000, DEF_STR( Unknown ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x0004, DEF_STR( On ) )
	PORT_DIPNAME( 0x0008, 0x0000, DEF_STR( Unknown ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x0008, DEF_STR( On ) )
	PORT_DIPNAME( 0x0010, 0x0000, DEF_STR( Unknown ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x0010, DEF_STR( On ) )
	PORT_DIPNAME( 0x0020, 0x0000, DEF_STR( Unknown ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x0020, DEF_STR( On ) )
	PORT_DIPNAME( 0x0040, 0x0000, DEF_STR( Unknown ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x0040, DEF_STR( On ) )
	PORT_DIPNAME( 0x0080, 0x0000, DEF_STR( Unknown ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x0080, DEF_STR( On ) )
	PORT_DIPNAME( 0x0100, 0x0000, "DSWB" )
	PORT_DIPSETTING(      0x0000, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x0100, DEF_STR( On ) )
	PORT_DIPNAME( 0x0200, 0x0000, DEF_STR( Unknown ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x0200, DEF_STR( On ) )
	PORT_DIPNAME( 0x0400, 0x0000, DEF_STR( Unknown ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x0400, DEF_STR( On ) )
	PORT_DIPNAME( 0x0800, 0x0000, DEF_STR( Unknown ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x0800, DEF_STR( On ) )
	PORT_DIPNAME( 0x1000, 0x0000, DEF_STR( Unknown ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x1000, DEF_STR( On ) )
	PORT_DIPNAME( 0x2000, 0x2000, DEF_STR( Unknown ) ) //enters into analyzer in some games otherwise
	PORT_DIPSETTING(      0x0000, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x2000, DEF_STR( On ) )
	PORT_DIPNAME( 0x4000, 0x0000, DEF_STR( Unknown ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x4000, DEF_STR( On ) )
	PORT_DIPNAME( 0x8000, 0x0000, DEF_STR( Unknown ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x8000, DEF_STR( On ) )

	PORT_START("SYSTEM")
	PORT_BIT( 0x0001, IP_ACTIVE_LOW, IPT_COIN1 )			// COIN1
	PORT_BIT( 0x0002, IP_ACTIVE_LOW, IPT_COIN2 )			// COIN2
	PORT_DIPNAME( 0x0004, 0x0004, "SYSA" )
	PORT_DIPSETTING(      0x0000, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x0004, DEF_STR( On ) )
	PORT_DIPNAME( 0x0008, 0x0008, DEF_STR( Unknown ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x0008, DEF_STR( On ) )
	PORT_DIPNAME( 0x0010, 0x0010, DEF_STR( Unknown ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x0010, DEF_STR( On ) )
	PORT_DIPNAME( 0x0020, 0x0020, DEF_STR( Unknown ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x0020, DEF_STR( On ) )
	PORT_DIPNAME( 0x0040, 0x0040, DEF_STR( Unknown ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x0040, DEF_STR( On ) )
	PORT_DIPNAME( 0x0080, 0x0080, DEF_STR( Unknown ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x0080, DEF_STR( On ) )
	PORT_DIPNAME( 0x0100, 0x0100, "SYSB" )
	PORT_DIPSETTING(      0x0000, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x0100, DEF_STR( On ) )
	PORT_DIPNAME( 0x0200, 0x0200, DEF_STR( Unknown ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x0200, DEF_STR( On ) )
	PORT_DIPNAME( 0x0400, 0x0400, DEF_STR( Unknown ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x0400, DEF_STR( On ) )
	PORT_DIPNAME( 0x0800, 0x0800, DEF_STR( Unknown ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x0800, DEF_STR( On ) )
	PORT_DIPNAME( 0x1000, 0x1000, DEF_STR( Unknown ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x1000, DEF_STR( On ) )
	PORT_DIPNAME( 0x2000, 0x2000, DEF_STR( Unknown ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x2000, DEF_STR( On ) )
	PORT_DIPNAME( 0x4000, 0x4000, DEF_STR( Unknown ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x4000, DEF_STR( On ) )
	PORT_DIPNAME( 0x8000, 0x8000, DEF_STR( Unknown ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x8000, DEF_STR( On ) )
INPUT_PORTS_END

static GFXDECODE_START( csplayh5 )
GFXDECODE_END

static Z80CTC_INTERFACE( ctc_intf )
{
	0,							/* timer disables */
	DEVCB_CPU_INPUT_LINE("audiocpu", INPUT_LINE_IRQ0),/* interrupt handler */
	DEVCB_LINE(z80ctc_trg3_w),	/* ZC/TO0 callback ctc1.zc0 -> ctc1.trg3 */
	DEVCB_NULL,					/* ZC/TO1 callback */
	DEVCB_NULL					/* ZC/TO2 callback */
};


static MACHINE_RESET( csplayh5 )
{
	csplayh5_state *state = machine.driver_data<csplayh5_state>();
	address_space *space = machine.device("maincpu")->memory().space(AS_PROGRAM);
	int i;

	// initialize TMPZ84C011 PIO
	for (i = 0; i < 5; i++)
	{
		state->m_pio_dir[i] = state->m_pio_latch[i] = 0;
		tmpz84c011_pio_w(space, i, 0);
	}
}

static TIMER_DEVICE_CALLBACK( csplayh5_irq )
{
	csplayh5_state *state = timer.machine().driver_data<csplayh5_state>();
	int scanline = param;

	if(scanline == 212*2)
		device_set_input_line_and_vector(state->m_maincpu, 1, HOLD_LINE,0x100/4);

	if((scanline % 2) == 0)
	{
		state->m_v9958->set_sprite_limit(0);
		state->m_v9958->set_resolution(RENDER_HIGH);
		state->m_v9958->interrupt();
	}
}

static const z80_daisy_config daisy_chain_sound[] =
{
	{ "ctc" },
	{ NULL }
};


static MACHINE_CONFIG_START( csplayh5, csplayh5_state )

	/* basic machine hardware */
	MCFG_CPU_ADD("maincpu",M68000,16000000) /* TMP68301-16 */
	MCFG_CPU_PROGRAM_MAP(csplayh5_map)
	MCFG_TIMER_ADD_SCANLINE("scantimer", csplayh5_irq, "screen", 0, 1)

#if USE_H8
	MCFG_CPU_ADD("subcpu", H83002, 16000000)	/* unknown clock */
	MCFG_CPU_PROGRAM_MAP(csplayh5_sub_map)
	MCFG_CPU_IO_MAP(csplayh5_sub_io_map)
#endif

	MCFG_CPU_ADD("audiocpu", Z80, 8000000)	/* TMPZ84C011, unknown clock */
	MCFG_CPU_CONFIG(daisy_chain_sound)
	MCFG_CPU_PROGRAM_MAP(csplayh5_sound_map)
	MCFG_CPU_IO_MAP(csplayh5_sound_io_map)

	MCFG_Z80CTC_ADD("ctc", 8000000, ctc_intf)

	MCFG_MACHINE_RESET(csplayh5)

	MCFG_NVRAM_ADD_0FILL("nvram")

	/* video hardware */
	MCFG_VIDEO_ATTRIBUTES(VIDEO_UPDATE_BEFORE_VBLANK)

	MCFG_V9958_ADD("v9958", "screen", 0x20000)
	MCFG_V99X8_INTERRUPT_CALLBACK_STATIC(csplayh5_vdp0_interrupt)

	MCFG_SCREEN_ADD("screen",RASTER)
	MCFG_SCREEN_REFRESH_RATE(60)
	MCFG_SCREEN_VBLANK_TIME(ATTOSECONDS_IN_USEC(2500))
	MCFG_SCREEN_SIZE(MSX2_TOTAL_XRES_PIXELS, 262*2)
	MCFG_SCREEN_VISIBLE_AREA(MSX2_XBORDER_PIXELS - MSX2_VISIBLE_XBORDER_PIXELS, MSX2_TOTAL_XRES_PIXELS - MSX2_XBORDER_PIXELS + MSX2_VISIBLE_XBORDER_PIXELS - 1, MSX2_YBORDER_PIXELS - MSX2_VISIBLE_YBORDER_PIXELS, MSX2_TOTAL_YRES_PIXELS - MSX2_YBORDER_PIXELS + MSX2_VISIBLE_YBORDER_PIXELS - 1)
	MCFG_SCREEN_UPDATE_DEVICE("v9958", v9958_device, screen_update)

	MCFG_PALETTE_LENGTH(512)
	MCFG_PALETTE_INIT( v9958 )

	/* sound hardware */
	MCFG_SPEAKER_STANDARD_MONO("mono")

	MCFG_SOUND_ADD("ymsnd", YM3812, 4000000)
	MCFG_SOUND_ROUTE(ALL_OUTPUTS, "mono", 0.70)

	MCFG_SOUND_ADD("dac1", DAC, 0)
	MCFG_SOUND_ROUTE(ALL_OUTPUTS, "mono", 0.50)

	MCFG_SOUND_ADD("dac2", DAC, 0)
	MCFG_SOUND_ROUTE(ALL_OUTPUTS, "mono", 0.75)
MACHINE_CONFIG_END

/***************************************************************************

  Game driver(s)

***************************************************************************/

void general_init(running_machine &machine, int patchaddress)
{
	UINT16 *MAINROM = (UINT16 *)machine.region("maincpu")->base();
	UINT8 *SNDROM = machine.region("audiocpu")->base();

	// initialize sound rom bank
	csplayh5_soundbank_w(machine, 0);

	/* patch DVD comms check */
	MAINROM[patchaddress] = 0x6018;

	/* patch sound program */
	SNDROM[0x0213] = 0x00;			// DI -> NOP

}

static DRIVER_INIT( junai )    { general_init(machine, 0x679c/2); }
static DRIVER_INIT( mjmania )  { general_init(machine, 0x6b96/2); }
static DRIVER_INIT( junai2 )   { general_init(machine, 0x6588/2); }
static DRIVER_INIT( csplayh5 ) { general_init(machine, 0x4cb4/2); }
static DRIVER_INIT( bikiniko ) { general_init(machine, 0x585c/2); }
static DRIVER_INIT( thenanpa ) { general_init(machine, 0x69ec/2); }
static DRIVER_INIT( csplayh7 ) { general_init(machine, 0x7a20/2); }
static DRIVER_INIT( fuudol )   { general_init(machine, 0x9166/2); }

ROM_START( junai )
	ROM_REGION( 0x40000, "maincpu", 0 ) // tmp68301 prg
	ROM_LOAD16_BYTE( "2.ic3",   0x00000, 0x20000, CRC(5923bf2e) SHA1(8fc7b95a44eb792ce03c1bffb9ad56f82d34b470) )
	ROM_LOAD16_BYTE( "1.ic2",   0x00001, 0x20000, CRC(4ac649ee) SHA1(f5b5bccecb6eba5addcf6a57e54deff7f29f6381) )

	ROM_REGION( 0x20000, "subcpu", 0 ) // h8, dvd player
	ROM_LOAD16_WORD_SWAP( "u2.va1b102",   0x00000, 0x20000, CRC(67374715) SHA1(8767cbd81614c2686a1adb70159f909e8ffd634d) )

	ROM_REGION( 0x20000, "audiocpu", 0 ) // z80
	ROM_LOAD( "11.ic51",   0x00000, 0x20000, CRC(a0472ea5) SHA1(0fd04941ff595cffe64357f3a1a9dc1170db8703) )

	ROM_REGION( 0x400000, "blit_gfx", ROMREGION_ERASEFF ) // blitter based gfxs
	ROM_LOAD16_BYTE( "3.ic40",   0x00000, 0x80000, CRC(f17fa4c4) SHA1(fd8b69b18f9ac00f468d751bf1ea0715498ea742) )
	ROM_LOAD16_BYTE( "4.ic41",   0x00001, 0x80000, CRC(4182dc30) SHA1(89601c62b74aff3d65b075d4b5cd1eb2ccf4e386) )
	// 0x100000 - 0x3fffff empty sockets

	DISK_REGION( "dvd" )
	DISK_IMAGE( "junai", 0, SHA1(0491533e0ce3e4d2af608ea0b9d9646316b512bd) )
ROM_END

ROM_START( csplayh5 )
	ROM_REGION( 0x40000, "maincpu", 0 ) // tmp68301 prg
	ROM_LOAD16_BYTE( "2.ic3",   0x00000, 0x20000, CRC(980bf3b0) SHA1(89da7354552f30aaa9d46442972c060b4b0f8979) )
	ROM_LOAD16_BYTE( "1.ic2",   0x00001, 0x20000, CRC(81ca49a4) SHA1(601b6802ab85be61f45a64f5b4c7e1f1ae5ee887) )

	ROM_REGION( 0x20000, "subcpu", 0 ) // h8, dvd player
	ROM_LOAD16_WORD_SWAP( "u2.va1a101",   0x00000, 0x20000, CRC(36135792) SHA1(1b9c50bd02df8227b228b35cc485efd5a13ec639) )

	ROM_REGION( 0x20000, "audiocpu", 0 ) // z80
	ROM_LOAD( "11.ic51",   0x00000, 0x20000, CRC(0b920806) SHA1(95f50ebfb296ba29aaa8079a41f5362cb9e879cc) )

	ROM_REGION( 0x400000, "blit_gfx", ROMREGION_ERASEFF ) // blitter based gfxs
	ROM_LOAD16_BYTE( "3.ic40",   0x00000, 0x80000, CRC(895b5e1f) SHA1(9398ee95d391f74d62fe641cb75311f31d4d1c8d) )
	ROM_LOAD16_BYTE( "4.ic41",   0x00001, 0x80000, CRC(113d7e96) SHA1(f3fb9c719544417a6a018b82f07c65bf73de21ff) )
	// 0x100000 - 0x3fffff empty sockets

	DISK_REGION( "dvd" )
	DISK_IMAGE( "csplayh5", 0, SHA1(ce4883ce1351ce5299e41bfbd9a5ae8078b82b8c) )
ROM_END

ROM_START( junai2 )
	ROM_REGION( 0x40000, "maincpu", 0 ) // tmp68301 prg
	ROM_LOAD16_BYTE( "2.ic3",   0x00000, 0x20000, CRC(b0ce71d6) SHA1(35cff8f8b18312808e2f6b96f02d952b0d1f04a1) )
	ROM_LOAD16_BYTE( "1.ic2",   0x00001, 0x20000, CRC(5a428e91) SHA1(dffce6f0a48cc4110970f124684dcaa267fe1b7f) )

	ROM_REGION( 0x20000, "subcpu", 0 ) // h8, dvd player
	ROM_LOAD16_WORD_SWAP( "u2",   0x00000, 0x20000, NO_DUMP )

	ROM_REGION( 0x20000, "audiocpu", 0 ) // z80
	ROM_LOAD( "11.ic51",   0x00000, 0x20000, CRC(a4b07757) SHA1(5010f28d7a80af0cc3f4fd135f777950fb2cf679) )

	ROM_REGION( 0x400000, "blit_gfx", ROMREGION_ERASEFF ) // blitter based gfxs
	ROM_LOAD16_BYTE( "3.ic40",   0x00000, 0x80000, CRC(95ecb29d) SHA1(e07bb0ff15aaee9fb26d8ef7f4644b47045c81a8) )
	ROM_LOAD16_BYTE( "4.ic41",   0x00001, 0x80000, CRC(5b37c8dd) SHA1(8de5e2f92721c6679c6506850a442cafff89653f) )
	// 0x100000 - 0x3fffff empty sockets

	DISK_REGION( "dvd" )
	DISK_IMAGE( "junai2", 0, SHA1(dc9633a101f20f03fd9b4414c10274d2539fb7c2) )

	ROM_REGION( 0x1000, "gal", ROMREGION_ERASE00 )
	ROM_LOAD( "gal16v8b.ic8", 0x000000, 0x0008c1, BAD_DUMP CRC(01c2895a) SHA1(782166a60fa14d5faa5a92629f7ca65a878ad7fe) )
ROM_END


ROM_START( mjmania )
	ROM_REGION( 0x40000, "maincpu", 0 ) // tmp68301 prg
	ROM_LOAD16_BYTE( "2.ic3", 0x000000, 0x020000, CRC(7b0f79c5) SHA1(73f23f68db4426b32583a7922abf773d67c76862) )
	ROM_LOAD16_BYTE( "1.ic2", 0x000001, 0x020000, CRC(19192ae7) SHA1(4e9fca04b567c8ef9136a3ab87b21207a44a24c4) )

	ROM_REGION( 0x20000, "subcpu", 0 ) // h8, dvd player
	ROM_LOAD16_WORD_SWAP( "u2",   0x00000, 0x20000, NO_DUMP )

	ROM_REGION( 0x20000, "audiocpu", 0 ) // z80
	ROM_LOAD( "11.ic51", 0x000000, 0x020000, CRC(f0c3bb11) SHA1(691a0ff53a9417e69051e9e2bdee7500bc6a746b) )

	ROM_REGION( 0x400000, "blit_gfx", ROMREGION_ERASEFF ) // blitter based gfxs
    ROM_LOAD16_BYTE( "3.ic40", 0x000000, 0x080000, CRC(37dde764) SHA1(0530b63d8e682cdf01128057fdc3a8c23262afc9) )
    ROM_LOAD16_BYTE( "4.ic41", 0x000001, 0x080000, CRC(dea4a2d2) SHA1(0118eb1330c9da8fead99f64fc015fd343fed79b) )

	DISK_REGION( "dvd" )
	DISK_IMAGE( "mjmania", 0, SHA1(7117f2045fd04a3d8f8e06a6a98e8f585c4da301) )

	ROM_REGION( 0x1000, "gal", ROMREGION_ERASE00 )
	ROM_LOAD( "gal16v8b.ic8", 0x000000, 0x0008c1, BAD_DUMP CRC(6a92b563) SHA1(a6c4305cf021f37845f99713427daa9394b6ec7d) )
ROM_END

ROM_START( bikiniko )
	ROM_REGION( 0x40000, "maincpu", 0 ) // tmp68301 prg
	ROM_LOAD16_BYTE( "2.ic3",   0x00000, 0x20000, CRC(b80b5484) SHA1(35769d9502cbe587dad6380c35e535cea1578227) )
	ROM_LOAD16_BYTE( "1.ic2",   0x00001, 0x20000, CRC(13a885af) SHA1(ba8221fab1a37f1937e4399eabe3eaa9093884d3) )

	ROM_REGION( 0x20000, "subcpu", 0 ) // h8, dvd player
	ROM_LOAD16_WORD_SWAP( "u2.va1b101",   0x00000, 0x20000, CRC(b92a83c8) SHA1(bd6d9adaa74cf7512478838d1bd5a79dbd0c4aa1) )

	ROM_REGION( 0x20000, "audiocpu", 0 ) // z80
	ROM_LOAD( "11.ic51",   0x00000, 0x20000, CRC(4a2142d6) SHA1(3a762f7b7cccdb6715b5f59524b04b12694fc130) )

	ROM_REGION( 0x400000, "blit_gfx", ROMREGION_ERASEFF ) // blitter based gfxs
	ROM_LOAD16_BYTE( "3.ic40",   0x00000, 0x80000, CRC(12914d3b) SHA1(de0cdb47ee5cbf8bd19ab19b1b8d8afe103dcedf) )
	ROM_LOAD16_BYTE( "4.ic41",   0x00001, 0x80000, CRC(1e2e1cf3) SHA1(f71b5dedf4f897644d519e412651152d0d81edb8) )
	// 0x100000 - 0x3fffff empty sockets

	DISK_REGION( "dvd" )
	DISK_IMAGE( "bikiniko", 0, SHA1(2189b676746dd848b9b5eb69f9663d6dccd63787) )
ROM_END

ROM_START( thenanpa )
	ROM_REGION( 0x40000, "maincpu", 0 ) // tmp68301 prg
    ROM_LOAD16_BYTE( "2.ic3", 0x000000, 0x020000, CRC(ab0b686f) SHA1(a5681dbacbc60f3eb40e079779967cf69d9cb292) )
    ROM_LOAD16_BYTE( "1.ic2", 0x000001, 0x020000, CRC(48b65f9a) SHA1(ce35475d3b0e9e8dc69892428f3957d8d3d5f22c) )

	ROM_REGION( 0x20000, "subcpu", 0 ) // h8, dvd player
	ROM_LOAD16_WORD_SWAP( "u2",   0x00000, 0x20000, NO_DUMP )

	ROM_REGION( 0x20000, "audiocpu", 0 ) // z80
    ROM_LOAD( "11.ic51", 0x000000, 0x020000, CRC(f44c4095) SHA1(d43e464bd6d614c34791445f8fd4af2f62a4dfc2) )

	ROM_REGION( 0x400000, "blit_gfx", ROMREGION_ERASEFF ) // blitter based gfxs
    ROM_LOAD16_BYTE( "3.ic40", 0x000000, 0x080000, CRC(ee6b88c4) SHA1(64ae66a24f1639801c7bdda7faa0d604bb97ceb1) )
    ROM_LOAD16_BYTE( "4.ic41", 0x000001, 0x080000, CRC(ce987845) SHA1(2f7dca32a79ad6afbc55ca1d492b582f952688ff) )

	DISK_REGION( "dvd" )
	DISK_IMAGE( "thenanpa", 0,  SHA1(72bf8c75189e877508c5a64d5591738d23ed7e96) )

	ROM_REGION( 0x1000, "gal", ROMREGION_ERASE00 )
    ROM_LOAD( "gal16v8b.ic8", 0x000000, 0x0008c1, BAD_DUMP CRC(daffd0ac)SHA1(cbeff914163d425a9cb30fe8d62f91fca281b11f) )
ROM_END

ROM_START( csplayh7 )
	ROM_REGION( 0x40000, "maincpu", 0 ) // tmp68301 prg
	ROM_LOAD16_BYTE( "2.ic3", 0x000000, 0x020000, CRC(c5ce76a6) SHA1(f8878285d2318c1ec50ba98607eb3f15a7f69913) )
	ROM_LOAD16_BYTE( "1.ic2", 0x000001, 0x020000, CRC(162f8cff) SHA1(8aa185fd1daa943d0b21fdf6e692f7782bc6dac4) )

	ROM_REGION( 0x20000, "subcpu", 0 ) // h8, dvd player
    ROM_LOAD16_WORD_SWAP( "va1b102.u2", 0x000000, 0x020000, CRC(67374715) SHA1(8767cbd81614c2686a1adb70159f909e8ffd634d) )

	ROM_REGION( 0x20000, "audiocpu", 0 ) // z80
    ROM_LOAD( "11.ic51", 0x000000, 0x020000, CRC(5905b199) SHA1(9155455bc21d23d439c4732549ff1143ee17b9d3) )

	ROM_REGION( 0x400000, "blit_gfx", ROMREGION_ERASEFF ) // blitter based gfxs
    ROM_LOAD16_BYTE( "3.ic40", 0x000000, 0x080000, CRC(1d67ca95) SHA1(9b45045b6fa67308bade324f91c21010aa8d121e) )
    ROM_LOAD16_BYTE( "4.ic41", 0x000001, 0x080000, CRC(b4f5f990) SHA1(88cccae04f89fef43d88f4e82b65de3de946e9af) )

	DISK_REGION( "dvd" )
	DISK_IMAGE( "csplayh7", 0, SHA1(f81e772745b0c62b17d91bd294993e49fe8da4d9) )

	ROM_REGION( 0x1000, "gal", ROMREGION_ERASE00 )
    ROM_LOAD( "mjdvd12.gal16v8b.ic8.bin", 0x000000, 0x0008c1, BAD_DUMP CRC(6a92b563)SHA1(a6c4305cf021f37845f99713427daa9394b6ec7d) )
ROM_END

ROM_START( fuudol )
	ROM_REGION( 0x40000, "maincpu", 0 ) // tmp68301 prg
    ROM_LOAD16_BYTE( "1.ic2", 0x000001, 0x020000, CRC(0cab2a72) SHA1(32d098bdd693a11f3cea6bbed3515c4217f40e23) )
	ROM_LOAD16_BYTE( "2.ic3", 0x000000, 0x020000, CRC(b1fa335e) SHA1(8a881c9c511fb63b00a3a7e433bae12aa9c2c262) )

	ROM_REGION( 0x20000, "subcpu", 0 ) // h8, dvd player
    ROM_LOAD16_WORD_SWAP( "va1b102.u2", 0x000000, 0x020000, CRC(67374715) SHA1(8767cbd81614c2686a1adb70159f909e8ffd634d) )

	ROM_REGION( 0x20000, "audiocpu", 0 ) // z80
	ROM_LOAD( "11.ic51", 0x000000, 0x020000, CRC(f6442026) SHA1(f49ddeeeaf6fffdccea9ba73bce3ca60c07a7647) )

	ROM_REGION( 0x400000, "blit_gfx", ROMREGION_ERASEFF ) // blitter based gfxs
	ROM_LOAD16_BYTE( "3.ic40", 0x000000, 0x080000, CRC(5c9e8665) SHA1(2a1b040e5c72d4400d4b5c467c75ae99e9bb01e2) )
	ROM_LOAD16_BYTE( "4.ic41", 0x000001, 0x080000, CRC(fdd79d8f) SHA1(f8bb82afaa28affb04b83270eb407129f1c7e611) )

	DISK_REGION( "dvd" )
	DISK_IMAGE( "fuudol", 0, SHA1(fabab43543ed14da4fe7c63a2a2cc4e68936938a) )

	ROM_REGION( 0x1000, "gal", ROMREGION_ERASE00 )
	ROM_LOAD( "gal16v8b.ic8", 0x000000, 0x0008c1, CRC(30719630) SHA1(a8c7b6d0304c38691775c5af6c32fbeeefd9f9fa) )
ROM_END

// 1998
// 01 : Mahjong Gal-pri - World Gal-con Grandprix : Nichibutsu/Just&Just
// 02 : Sengoku Mahjong Kurenai Otome-tai : Nichibutsu/Just&Just
/* 03 */ GAME( 1998, junai,     0,   csplayh5,  csplayh5,  junai,   		ROT0, "Nichibutsu/eic",   "Junai - Manatsu no First Kiss (Japan)", GAME_NOT_WORKING )
/* 04 */ GAME( 1998, csplayh5,  0,   csplayh5,  csplayh5,  csplayh5,		ROT0, "Nichibutsu",       "Mahjong Hanafuda Cosplay Tengoku 5 (Japan)", GAME_NOT_WORKING )
/* 05 */ GAME( 1998, junai2,    0,   csplayh5,  csplayh5,  junai2,  		ROT0, "Nichibutsu/eic",   "Junai 2 - White Love Story (Japan)", GAME_NOT_WORKING )
// 06 : Mahjong Mogitate : Nichibutsu/Just&Just/NVS/Astro System/AV Japan


// 1999
/* 07 */ GAME( 1999, mjmania,   0,   csplayh5,  csplayh5,  mjmania,     	ROT0, "Sphinx/Just&Just", "Mahjong Mania - Kairakukan e Youkoso (Japan)", GAME_NOT_WORKING )
// 08 : Renai Mahjong Idol Gakuen : Nichibutsu/eic
/* 09 */ GAME( 1999, bikiniko,  0,   csplayh5,  csplayh5,  bikiniko,		ROT0, "Nichibutsu/eic",   "BiKiNikko - Okinawa de Ippai Shichaimashita (Japan)", GAME_NOT_WORKING )
// 10 : Mahjong Hanafuda Cosplay Tengoku 6 - Junai hen : Nichibutsu/eic
/* 11 */ GAME( 1999, thenanpa,  0,   csplayh5,  csplayh5,  thenanpa,        ROT0, "Nichibutsu/Love Factory/eic", "The Nanpa (Japan)", GAME_NOT_WORKING )
// 12 GAME( 1999, pokoachu,  0,   csplayh5,  csplayh5,  0,        ROT0, "Nichibutsu/eic", "PokoaPoka Onsen de CHU - Bijin 3 Shimai ni Kiotsukete! (Japan)", GAME_NOT_WORKING )
/* 13 */ GAME( 1999, csplayh7,  0,   csplayh5,  csplayh5,  csplayh7,        ROT0, "Nichibutsu/eic", "Cosplay Tengoku 7 - Super Kogal Grandprix (Japan)", GAME_NOT_WORKING )
// 14 : Ai-mode - Pet Shiiku : Nichibutsu/eic

// 2000
/* 15 */ GAME( 2000, fuudol,    0,   csplayh5,  csplayh5,  fuudol,      	ROT0, "Nichibutsu/eic", "Fuudol (Japan)", GAME_NOT_WORKING )
// 16 : Nurete Mitaino... - Net Idol Hen : Nichibutsu/Love Factory
// 17 : Tsuugakuro no Yuuwaku : Nichibutsu/Love Factory/Just&Just
// 18 : Torarechattano - AV Kantoku Hen : Nichibutsu/Love Factory/M Friend

// 2001
// 19 : Konnano Hajimete! : Nichibutsu/Love Factory
// 20 : Uwasa no Deaikei Site : Nichibutsu/Love Factory/eic
