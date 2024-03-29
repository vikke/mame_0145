/*********************************************************************************************************************

Last Fighting  (C)2000 Subsino

driver by Luca Elia

PCB Layout
----------

|------------------------------------------------------|
|TDA1519A                                              |
|     VOL                                              |
|                HM86171                       ULN2003 |
|   LM324                                              |
|           S-1                                ULN2003 |
|                                                      |
|                                    |-----|   ULN2003 |
|                        |-----|     |U1   |           |
|              V100.U7   |U6   |     |     |           |
|J                       |     |     |-----|           |
|A                       |-----|                       |
|M                                                     |
|M                                                     |
|A          KM428C256      32MHz     CXK58257          |
|     |----ROM BOARD------|                            |
|     |                   |          V106.U16          |
|     |          U19      |                         SW1|
|     |       |-------|   |          |-----|           |
|     |       |SUBSINO|   |          |H8   |           |
|     |       |9623EX |   | |-----|  |3044 |           |
|     |       |008    |   | |EPM  |  |-----|           |
|     |       |-------|   | |7032 |                    |
|     |     CN2           | |-----|             3V_BATT|
|-----|-------------------|----------------------------|

Notes:
      H8/3044 - Subsino re-badged Hitachi H8/3044 HD6433044A22F Microcontroller (QFP100)
                The H8/3044 is a H8/3002 with 24bit address bus and has 32k MASKROM and 2k RAM, clock input is 16.000MHz [32/2]
                MD0,MD1 & MD2 are configured to MODE 6 16M-Byte Expanded Mode with the on-chip 32k MASKROM enabled.
      EPM7032 - Altera EPM7032LC44-15T CPLD (PLCC44)
     CXK58257 - Sony CXK58257 32k x8 SRAM (SOP28)
    KM428C256 - Samsung Semiconductor KM428C256 256k x8 Dual Port DRAM (SOJ40)
     ULKN2003 - Toshiba ULN2003 High Voltage High Current Darlington Transistor Array comprising 7 NPN Darlinton pairs (DIP16)
      HM86171 - Hualon Microelectronics HMC HM86171 VGA 256 colour RAMDAC (DIP28)
      3V_BATT - 3 Volt Coin Battery. This is tied to the CXK58257 SRAM. It appears to be used as an EEPROM, as the game
                has on-board settings in test mode and there's no DIPs and no EEPROM.
          S-1 - ?? Probably some kind of audio OP AMP or DAC? (DIP8)
     TDA1519A - Philips TDA1519A 22W BTL/Dual 11W Audio Power Amplifier IC (SIL9)
          CN2 - 70 pin connector for connection of ROM board
          SW1 - Push Button Test Switch
        HSync - 15.75kHz
        VSync - 60Hz
    ROM BOARD - Small Daughterboard containing positions for 8x 16MBit SOP44 MASKROMs. Only positions 1-4 are populated.
   Custom ICs -
                U19     - SUBSINO 9623EX008 (QFP208)
                H8/3044 - SUBSINO SS9689 6433044A22F, rebadged Hitachi H8/3044 MCU (QFP100)
                U1      - SUBSINO SS9802 9933 (QFP100)
                U6      - SUBSINO SS9804 0001 (QFP100)
         ROMs -
                V106.U16 - MX27C4000 4MBit DIP32 EPROM; Main Program
                V100.U7  - ST M27C801 8MBit DIP32 EPROM; Audio Samples?

*********************************************************************************************************************/

#include "emu.h"
#include "cpu/h83002/h8.h"
#include "machine/nvram.h"

class lastfght_state : public driver_device
{
public:
	lastfght_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag),
		m_maincpu(*this,"maincpu")
		{ }

	/* video-related */
	bitmap_ind16 m_bitmap[2];
	int m_clr_offset;
	int m_dest;
	int m_hi;
	int m_sx;
	int m_sx1;
	int m_dsx;
	int m_sy;
	int m_sy1;
	int m_dsy;
	int m_sp;
	int m_sr;
	int m_x;
	int m_y;
	int m_w;
	int m_h;
	unsigned m_base;
	int m_view_roms;

	/* misc */
	UINT16 m_c00006;

	/* devices */
	required_device<cpu_device> m_maincpu;

	/* memory */
	UINT8   m_colorram[256 * 3];
};


/***************************************************************************
                                Video Hardware
***************************************************************************/

static VIDEO_START( lastfght )
{
	lastfght_state *state = machine.driver_data<lastfght_state>();
	int i;
	for (i = 0; i < 2; i++)
		machine.primary_screen->register_screen_bitmap(state->m_bitmap[i]);

	state->save_item(NAME(state->m_bitmap[0]));
	state->save_item(NAME(state->m_bitmap[1]));
	state->save_item(NAME(state->m_colorram));
}


static SCREEN_UPDATE_IND16( lastfght )
{
	lastfght_state *state = screen.machine().driver_data<lastfght_state>();

#ifdef MAME_DEBUG
#if 1
	// gfx roms viewer (toggle with enter, use pgup/down to browse)
	int x, y, count = 0;
	UINT8 *gfxdata = screen.machine().region("gfx1")->base();
	UINT8 data;

	if (screen.machine().input().code_pressed_once(KEYCODE_ENTER))	state->m_view_roms ^= 1;
	if (state->m_view_roms)
	{
		if (screen.machine().input().code_pressed_once(KEYCODE_PGDN))	state->m_base += 512 * 256;
		if (screen.machine().input().code_pressed_once(KEYCODE_PGUP))	state->m_base -= 512 * 256;
		state->m_base %= screen.machine().region("gfx1")->bytes();

		count = state->m_base;

		bitmap.fill(get_black_pen(screen.machine()), cliprect );
		for (y = 0 ; y < 256; y++)
		{
			for (x = 0; x < 512; x++)
			{
				data = (((count & 0xf) == 0) && ((count & 0x1e00) == 0)) ? get_white_pen(screen.machine()) : gfxdata[count];	// white grid or data
				bitmap.pix16(y, x) = data;
				count++;
			}
		}
		popmessage("%x", state->m_base);
		return 0;
	}
#endif
#endif

	copybitmap(bitmap, state->m_bitmap[state->m_dest ^ 1], 0, 0, 0, 0, cliprect);

	return 0;
}

// Palette: HMC HM86171 VGA 256 colour RAMDAC

static WRITE16_HANDLER( colordac_w )
{
	lastfght_state *state = space->machine().driver_data<lastfght_state>();

	if (ACCESSING_BITS_0_7)
	{
		state->m_colorram[state->m_clr_offset] = data;
		palette_set_color_rgb(space->machine(), state->m_clr_offset / 3,
			pal6bit(state->m_colorram[(state->m_clr_offset / 3) * 3 + 0]),
			pal6bit(state->m_colorram[(state->m_clr_offset / 3) * 3 + 1]),
			pal6bit(state->m_colorram[(state->m_clr_offset / 3) * 3 + 2])
		);
		state->m_clr_offset = (state->m_clr_offset + 1) % (256 * 3);
	}
	if (ACCESSING_BITS_8_15)
	{
		state->m_clr_offset = (data >> 8) * 3;
	}
}

//  Blitter (supports zooming)

// high byte of a 16 bit register
static WRITE16_HANDLER( lastfght_hi_w )
{
	lastfght_state *state = space->machine().driver_data<lastfght_state>();

	if (ACCESSING_BITS_8_15)
		logerror("%06x: 600000.b = %02x\n", cpu_get_pc(&space->device()), data >> 8);
	if (ACCESSING_BITS_0_7)
	{
		state->m_hi = data << 8;
		//logerror("%06x: lastfght_hi  = %02x\n", cpu_get_pc(&space->device()), data);
	}
}

// screen x
static WRITE16_HANDLER( lastfght_x_w )
{
	lastfght_state *state = space->machine().driver_data<lastfght_state>();

	if (ACCESSING_BITS_8_15)
		logerror("%06x: 800008.b = %02x\n", cpu_get_pc(&space->device()), data >> 8);
	if (ACCESSING_BITS_0_7)
	{
		state->m_x = state->m_hi | data;
		//logerror("%06x: lastfght_x   = %02x\n", cpu_get_pc(&space->device()),data);
	}
}

// screen y, screen width - 1
static WRITE16_HANDLER( lastfght_yw_w )
{
	lastfght_state *state = space->machine().driver_data<lastfght_state>();

	if (ACCESSING_BITS_8_15)
	{
		state->m_y = state->m_hi | (data >> 8);
		//logerror("%06x: lastfght_y   = %02x\n", cpu_get_pc(&space->device()), data >> 8);
	}
	if (ACCESSING_BITS_0_7)
	{
		state->m_w = state->m_hi | data;
		//logerror("%06x: lastfght_w   = %02x\n", cpu_get_pc(&space->device()), data);
	}
}

// screen height - 1
static WRITE16_HANDLER( lastfght_h_w )
{
	lastfght_state *state = space->machine().driver_data<lastfght_state>();

	if (ACCESSING_BITS_8_15)
	{
		state->m_h = state->m_hi | (data >> 8);
		//logerror("%06x: lastfght_h   = %02x\n", cpu_get_pc(&space->device()), data >> 8);
	}
	if (ACCESSING_BITS_0_7)
		logerror("%06x: 80000d.b = %02x\n", cpu_get_pc(&space->device()), data);
}

// source delta x << 6, source x << 6
static WRITE16_HANDLER( lastfght_sx_w )
{
	lastfght_state *state = space->machine().driver_data<lastfght_state>();

	if (ACCESSING_BITS_8_15)
	{
		state->m_dsx = state->m_hi | (data >> 8);
		//logerror("%06x: lastfght_dsx = %02x\n", cpu_get_pc(&space->device()), data >> 8);
	}
	if (ACCESSING_BITS_0_7)
	{
		state->m_sx = state->m_hi | data;
		//logerror("%06x: lastfght_sx  = %02x\n", cpu_get_pc(&space->device()), data);
	}
}

// source y << 6, source y1 << 6
static WRITE16_HANDLER( lastfght_sy_w )
{
	lastfght_state *state = space->machine().driver_data<lastfght_state>();

	if (ACCESSING_BITS_8_15)
	{
		state->m_sy = state->m_hi | (data >> 8);
		//logerror("%06x: lastfght_sy  = %02x\n", cpu_get_pc(&space->device()), data >> 8);
	}
	if (ACCESSING_BITS_0_7)
	{
		state->m_sy1 = state->m_hi | data;
		//logerror("%06x: lastfght_sy1 = %02x\n", cpu_get_pc(&space->device()), data);
	}
}

// source rom (0x200000 bytes), source page (512x256 bytes)
static WRITE16_HANDLER( lastfght_sr_w )
{
	lastfght_state *state = space->machine().driver_data<lastfght_state>();

	if (ACCESSING_BITS_8_15)
	{
		state->m_sp = (state->m_hi >> 8) >> 4;
		//logerror("%06x: lastfght_sp  = %02x\n", cpu_get_pc(&space->device()), data >> 8);
	}
	if (ACCESSING_BITS_0_7)
	{
		state->m_sr = data;
		//logerror("%06x: lastfght_sr  = %02x\n", cpu_get_pc(&space->device()), data);
	}
}

// source x1 << 6, source delta y << 6
static WRITE16_HANDLER( lastfght_sd_w )
{
	lastfght_state *state = space->machine().driver_data<lastfght_state>();

	if (ACCESSING_BITS_8_15)
	{
		state->m_sx1 = state->m_hi | (data >> 8);
		//logerror("%06x: lastfght_sx1 = %02x\n", cpu_get_pc(&space->device()), data >> 8);
	}
	if (ACCESSING_BITS_0_7)
	{
		state->m_dsy = state->m_hi | data;
		//logerror("%06x: lastfght_dsy = %02x\n", cpu_get_pc(&space->device()), data);
	}
}

// start blit
static WRITE16_HANDLER( lastfght_blit_w )
{
	lastfght_state *state = space->machine().driver_data<lastfght_state>();

	if (ACCESSING_BITS_8_15)
	{
		int x, y, addr;
		UINT8 *gfxdata = space->machine().region( "gfx1" )->base();
		bitmap_ind16 &dest = state->m_bitmap[state->m_dest];

#if 0
		logerror("%06x: blit x %03x, y %03x, w %03x, h %03x, sx %03x.%02x, sx1 %03x.%02x, dsx %03x.%02x, sy %03x.%02x, sy1 %03x.%02x, dsy %03x.%02x, sp %02x, sr %02x, data %02x\n", cpu_get_pc(&space->device()),
				state->m_x, state->m_y, state->m_w + 1, state->m_h + 1,
				state->m_sx >> 6, state->m_sx & 0x3f, state->m_sx1 >> 6, state->m_dsx & 0x3f, state->m_sx1 >> 6, state->m_sx1 & 0x3f,
				state->m_sy >> 6, state->m_sy & 0x3f, state->m_sy1 >> 6, state->m_dsy & 0x3f, state->m_sy1 >> 6, state->m_sy1 & 0x3f,
				state->m_sp, state->m_sr,
				data >> 8);
#endif

		for (y = 0; y <= state->m_h; y++)
		{
			for (x = 0; x <= state->m_w; x++)
			{
				addr = (((state->m_sx + state->m_sx1 + state->m_dsx * x) >> 6) & 0x1ff) +
							(((state->m_sy + state->m_sy1 + state->m_dsy * y) >> 6) & 0xff) * 0x200 +
							state->m_sp * 0x200 * 0x100 + state->m_sr * 0x200000;

				data = gfxdata[addr];

				if (data && (state->m_x + x >= 0) && (state->m_x + x < 512) && (state->m_y + y >= 0) && (state->m_y + y < 256))
					dest.pix16(state->m_y + y, state->m_x + x) = data;
			}
		}
	}
	if (ACCESSING_BITS_0_7)
		logerror("%06x: 600007.b = %02x\n", cpu_get_pc(&space->device()), data);
}

// toggle framebuffer
static WRITE16_HANDLER( lastfght_dest_w )
{
	lastfght_state *state = space->machine().driver_data<lastfght_state>();

	if (ACCESSING_BITS_0_7)
		state->m_dest ^= 1;
}

static READ16_HANDLER( lastfght_c00000_r )
{
	// high byte:
	// bit 7 = blitter busy
	// bit 6 = blitter?
	return 0x4000;

}

static READ16_HANDLER( lastfght_c00002_r )
{
	// high byte:
	// mask 0x1c: from sound?
	return (space->machine().rand() & 0x1c00) | input_port_read(space->machine(), "IN0");
}

static READ16_HANDLER( lastfght_c00004_r )
{
	return input_port_read(space->machine(), "IN1");
}

static READ16_HANDLER( lastfght_c00006_r )
{
	// low byte:
	// bit 7 = protection?
	// bit 5 = blitter?
	return input_port_read(space->machine(), "IN2");
}

static WRITE16_HANDLER( lastfght_c00006_w )
{
	lastfght_state *state = space->machine().driver_data<lastfght_state>();
	COMBINE_DATA(&state->m_c00006);
	//  popmessage("%04x", state->m_c00006);
}

static READ16_HANDLER( lastfght_sound_r )
{
	// low byte:
	// bit 3
	return 8;
}

static WRITE16_HANDLER( lastfght_sound_w )
{
	if (ACCESSING_BITS_8_15)
		logerror("%06x: sound_w msb = %02x\n", cpu_get_pc(&space->device()), data >> 8);
	if (ACCESSING_BITS_0_7)
		logerror("%06x: sound_w lsb = %02x\n", cpu_get_pc(&space->device()), data);
}

/***************************************************************************
                                Memory Maps
***************************************************************************/

static ADDRESS_MAP_START( lastfght_map, AS_PROGRAM, 16 )
	ADDRESS_MAP_GLOBAL_MASK(0xffffff)

	AM_RANGE( 0x000000, 0x07ffff ) AM_ROM AM_REGION("maincpu", 0)
	AM_RANGE( 0x080000, 0x0fffff ) AM_ROM AM_REGION("maincpu", 0)

	AM_RANGE( 0x200000, 0x20ffff ) AM_RAM AM_SHARE("nvram")	// battery

	AM_RANGE( 0x600000, 0x600001 ) AM_WRITE( lastfght_hi_w )
	AM_RANGE( 0x600002, 0x600003 ) AM_READWRITE( lastfght_sound_r, lastfght_sound_w )
	AM_RANGE( 0x600006, 0x600007 ) AM_WRITE( lastfght_blit_w )
	AM_RANGE( 0x600008, 0x600009 ) AM_WRITE( colordac_w )
	AM_RANGE( 0x60000a, 0x60000b ) AM_WRITENOP	// colordac?

	AM_RANGE( 0x800000, 0x800001 ) AM_WRITE( lastfght_sx_w )
	AM_RANGE( 0x800002, 0x800003 ) AM_WRITE( lastfght_sd_w )
	AM_RANGE( 0x800004, 0x800005 ) AM_WRITE( lastfght_sy_w )
	AM_RANGE( 0x800006, 0x800007 ) AM_WRITE( lastfght_sr_w )
	AM_RANGE( 0x800008, 0x800009 ) AM_WRITE( lastfght_x_w )
	AM_RANGE( 0x80000a, 0x80000b ) AM_WRITE( lastfght_yw_w )
	AM_RANGE( 0x80000c, 0x80000d ) AM_WRITE( lastfght_h_w )

	AM_RANGE( 0x800014, 0x800015 ) AM_WRITE( lastfght_dest_w )

	AM_RANGE( 0xc00000, 0xc00001 ) AM_READ( lastfght_c00000_r )
	AM_RANGE( 0xc00002, 0xc00003 ) AM_READ( lastfght_c00002_r )
	AM_RANGE( 0xc00004, 0xc00005 ) AM_READ( lastfght_c00004_r )
	AM_RANGE( 0xc00006, 0xc00007 ) AM_READWRITE( lastfght_c00006_r, lastfght_c00006_w )

	AM_RANGE( 0xff0000, 0xffffff ) AM_RAM
ADDRESS_MAP_END


/***************************************************************************
                                Input Ports
***************************************************************************/

static INPUT_PORTS_START( lastfght )
	PORT_START("IN0")	/* IN0 - c00002&3 */
	PORT_BIT( 0x0001, IP_ACTIVE_LOW, IPT_SERVICE1		)
	PORT_BIT( 0x0002, IP_ACTIVE_LOW, IPT_UNKNOWN		)
	PORT_BIT( 0x0004, IP_ACTIVE_LOW, IPT_OTHER			) PORT_NAME("Reset") PORT_CODE(KEYCODE_F1)
	PORT_BIT( 0x0008, IP_ACTIVE_LOW, IPT_UNKNOWN		)
	PORT_BIT( 0x0010, IP_ACTIVE_LOW, IPT_UNKNOWN		)
	PORT_BIT( 0x0020, IP_ACTIVE_LOW, IPT_UNKNOWN		)
	PORT_BIT( 0x0040, IP_ACTIVE_LOW, IPT_SERVICE		)
	PORT_BIT( 0x0080, IP_ACTIVE_LOW, IPT_UNKNOWN		)

	PORT_BIT( 0x0100, IP_ACTIVE_LOW, IPT_UNKNOWN		)
	PORT_BIT( 0x0200, IP_ACTIVE_LOW, IPT_UNKNOWN		)
	PORT_BIT( 0x0400, IP_ACTIVE_HIGH,IPT_SPECIAL		)
	PORT_BIT( 0x0800, IP_ACTIVE_HIGH,IPT_SPECIAL		)
	PORT_BIT( 0x1000, IP_ACTIVE_HIGH,IPT_SPECIAL		)
	PORT_BIT( 0x2000, IP_ACTIVE_LOW, IPT_UNKNOWN		)
	PORT_BIT( 0x4000, IP_ACTIVE_LOW, IPT_UNKNOWN		)
	PORT_BIT( 0x8000, IP_ACTIVE_LOW, IPT_UNKNOWN		)

	PORT_START("IN1")	/* IN1 - c00004&5 */
	PORT_BIT( 0x0001, IP_ACTIVE_LOW, IPT_BUTTON1		)
	PORT_BIT( 0x0002, IP_ACTIVE_LOW, IPT_BUTTON2		)
	PORT_BIT( 0x0004, IP_ACTIVE_LOW, IPT_UNKNOWN		)
	PORT_BIT( 0x0008, IP_ACTIVE_LOW, IPT_JOYSTICK_UP	)
	PORT_BIT( 0x0010, IP_ACTIVE_LOW, IPT_UNKNOWN		)
	PORT_BIT( 0x0020, IP_ACTIVE_LOW, IPT_START1			)
	PORT_BIT( 0x0040, IP_ACTIVE_LOW, IPT_UNKNOWN		)
	PORT_BIT( 0x0080, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN	)

	PORT_BIT( 0x0100, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT	)
	PORT_BIT( 0x0200, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT	)
	PORT_BIT( 0x0400, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT	) PORT_PLAYER(2)
	PORT_BIT( 0x0800, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT ) PORT_PLAYER(2)
	PORT_BIT( 0x1000, IP_ACTIVE_LOW, IPT_UNKNOWN		)
	PORT_BIT( 0x2000, IP_ACTIVE_LOW, IPT_UNKNOWN		)
	PORT_BIT( 0x4000, IP_ACTIVE_LOW, IPT_COIN1			)
	PORT_BIT( 0x8000, IP_ACTIVE_LOW, IPT_COIN2			)

	PORT_START("IN2")	/* IN2 - c00006&7 */
	PORT_BIT( 0x0001, IP_ACTIVE_LOW, IPT_UNKNOWN		)
	PORT_BIT( 0x0002, IP_ACTIVE_LOW, IPT_UNKNOWN		)
	PORT_BIT( 0x0004, IP_ACTIVE_LOW, IPT_UNKNOWN		)
	PORT_BIT( 0x0008, IP_ACTIVE_LOW, IPT_UNKNOWN		)
	PORT_BIT( 0x0010, IP_ACTIVE_LOW, IPT_UNKNOWN		)
	PORT_BIT( 0x0020, IP_ACTIVE_LOW, IPT_UNKNOWN		)
	PORT_BIT( 0x0040, IP_ACTIVE_LOW, IPT_UNKNOWN		)
	PORT_BIT( 0x0080, IP_ACTIVE_LOW, IPT_UNKNOWN		)

	PORT_BIT( 0x0100, IP_ACTIVE_LOW, IPT_BUTTON1		) PORT_PLAYER(2)
	PORT_BIT( 0x0200, IP_ACTIVE_LOW, IPT_BUTTON2		) PORT_PLAYER(2)
	PORT_BIT( 0x0400, IP_ACTIVE_LOW, IPT_UNKNOWN		)
	PORT_BIT( 0x0800, IP_ACTIVE_LOW, IPT_JOYSTICK_UP	) PORT_PLAYER(2)
	PORT_BIT( 0x1000, IP_ACTIVE_LOW, IPT_UNKNOWN		)
	PORT_BIT( 0x2000, IP_ACTIVE_LOW, IPT_START2			)
	PORT_BIT( 0x4000, IP_ACTIVE_LOW, IPT_UNKNOWN		)
	PORT_BIT( 0x8000, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN	) PORT_PLAYER(2)
INPUT_PORTS_END


/***************************************************************************
                                Machine Drivers
***************************************************************************/

static INTERRUPT_GEN( unknown_interrupt )
{
	lastfght_state *state = device->machine().driver_data<lastfght_state>();

	device_set_input_line(state->m_maincpu, H8_METRO_TIMER_HACK, HOLD_LINE);
}

static MACHINE_START( lastfght )
{
	lastfght_state *state = machine.driver_data<lastfght_state>();

	state->save_item(NAME(state->m_clr_offset));
	state->save_item(NAME(state->m_dest));
	state->save_item(NAME(state->m_hi));
	state->save_item(NAME(state->m_sx));
	state->save_item(NAME(state->m_sx1));
	state->save_item(NAME(state->m_dsx));
	state->save_item(NAME(state->m_sy));
	state->save_item(NAME(state->m_sy1));
	state->save_item(NAME(state->m_dsy));
	state->save_item(NAME(state->m_sp));
	state->save_item(NAME(state->m_sr));
	state->save_item(NAME(state->m_x));
	state->save_item(NAME(state->m_y));
	state->save_item(NAME(state->m_w));
	state->save_item(NAME(state->m_h));
	state->save_item(NAME(state->m_c00006));
}

static MACHINE_RESET( lastfght )
{
	lastfght_state *state = machine.driver_data<lastfght_state>();

	state->m_clr_offset = 0;
	state->m_dest = 0;
	state->m_hi = 0;
	state->m_sx = 0;
	state->m_sx1 = 0;
	state->m_dsx = 0;
	state->m_sy = 0;
	state->m_sy1 = 0;
	state->m_dsy = 0;
	state->m_sp = 0;
	state->m_sr = 0;
	state->m_x = 0;
	state->m_y = 0;
	state->m_w = 0;
	state->m_h = 0;
	state->m_c00006 = 0;
}

static MACHINE_CONFIG_START( lastfght, lastfght_state )

	/* basic machine hardware */
	MCFG_CPU_ADD("maincpu", H83044, 32000000/2)
	MCFG_CPU_PROGRAM_MAP( lastfght_map)
	MCFG_CPU_VBLANK_INT("screen",irq0_line_hold)
	MCFG_CPU_PERIODIC_INT(unknown_interrupt,60)

	MCFG_NVRAM_ADD_0FILL("nvram")

	MCFG_MACHINE_START(lastfght)
	MCFG_MACHINE_RESET(lastfght)

	/* video hardware */
	MCFG_PALETTE_LENGTH( 256 )

	MCFG_SCREEN_ADD("screen", RASTER)
	MCFG_SCREEN_SIZE( 512, 256 )
	MCFG_SCREEN_VISIBLE_AREA( 0, 512-1, 0, 256-16-1 )
	MCFG_SCREEN_REFRESH_RATE( 60 )
	MCFG_SCREEN_UPDATE_STATIC( lastfght )

	MCFG_VIDEO_START( lastfght )
MACHINE_CONFIG_END


/***************************************************************************
                                ROMs Loading
***************************************************************************/

ROM_START( lastfght )
	ROM_REGION( 0x100000, "maincpu", 0 )		// H8/3044 program
	ROM_LOAD( "v106.u16", 0x000000, 0x080000, CRC(7aec89f4) SHA1(7cff00844ad82a0f8d19b1bd07ba3a2bced69d66) )

	ROM_REGION( 0x800000, "gfx1", 0 )		// Blitter data
	ROM_LOAD( "1.b1", 0x000000, 0x200000, CRC(6c438136) SHA1(138934e948bbd6bd80f354f037badedef6cd8cb1) )
	ROM_LOAD( "2.b2", 0x200000, 0x200000, CRC(9710bcff) SHA1(0291385489a065ed895c99ae7197fdeac0a0e2a0) )
	ROM_LOAD( "3.b3", 0x400000, 0x200000, CRC(4236c79a) SHA1(94f093d12c096d38d1e7278796f6d58e4ba14e2e) )
	ROM_LOAD( "4.b4", 0x600000, 0x200000, CRC(68153b0f) SHA1(46ddf37d5885f411e0e6de9c7e8969ba3a00f17f) )

	ROM_REGION( 0x100000, "samples", 0 )	// Samples
	ROM_LOAD( "v100.u7", 0x000000, 0x100000, CRC(c134378c) SHA1(999c75f3a7890421cfd904a926ca377ee43a6825) )
ROM_END

static DRIVER_INIT(lastfght)
{
	UINT16 *rom = (UINT16*)machine.region("maincpu")->base();

	// pass initial check (protection ? hw?)
	rom[0x00354 / 2] = 0x403e;

	// rts -> rte
	rom[0x01b86 / 2] = 0x5670;
}

GAME( 2000, lastfght, 0, lastfght, lastfght, lastfght, ROT0, "Subsino", "Last Fighting", GAME_NO_SOUND | GAME_SUPPORTS_SAVE )
