/*************************************************************************************

  /\/\<< Kasino '89 >>/\/\

  6-players electronic roulette.
  Video field + phisical LEDs roulette.

  Driver by Roberto Fresca.


  Running in this hardware:

  Kasino '89,  SFC SRL, 1989.
  Osel '93,    SFC SRL, 1993.


  Special thanks to Rob Ragon and Tomasz Slanina for their invaluable help.
  Also big thanks to Iris Falbala and Sofia Kiara for the beta-testing stage.

  For better results, use the associated artwork. Will allow you to see
  the roulette with all the working LEDs and lamps...


**************************************************************************************

  Hardware specs:

  Seems to be a MSX2 system on steroids...

  1x Potted CPU box (1x Z80 @ 3.578 MHz. + encrypted program ROM inside).
  1x Z80 (audio) @ 3.578 MHz.
  AY-3-8910 @ 1789 kHz.

  1x Unknown metallick brick (Yamaha MSX2 VDP inside).

  1x 8 DIP switches bank.

  1x Xtal: 21.477 MHz.

  1x Plastic roulette with 37 LEDs


  NOTES:

  - Addressing and data lines are fully scrambled inside the epoxy CPU box.
    Also there are PLDs connected between them.

  - Another board has both Z80's running at 3.336 MHz., but we strongly think
    that they tied a 20MHz crystal due to lack of 21.477MHz ones stock.

  - You can see the scrambling stuff inside the CPU box through a X-RAY picture,
    and other reverse-engineering steps here: http://www.robertofresca.com.ar/


**************************************************************************************

  Tech Notes...


  - NMI of main Z80 is connected to a 138Hz. oscillator AND bit6 of port $82.
    That means NMI should be triggered each 138Hz. *only* if bit6 of port $82
    remains active.

  - NMI of second Z80 is connected directly to a 138Hz. oscillator.

  - INT of second Z80 is coming from 2 places:
    The HOLD/ASSERT (flanco descendente) is done by the main Z80 through IO port &85,
    and the CLEAR part (flanco ascendente) is autogenerated using the IO port $00.

  - The AY-8910 is clocked 1789 kHz, and has nothing connected at simple sight.


**************************************************************************************

  Roulette's LEDs Support.
  -----------------------

  Ports $83/$84 handle the roulette's LEDs in a multiplexed way...
  Port $84 is the selector, while port $83 carry the LEDs set.


  Checks:

  00: OK 40-02       10: OK 08-10       20: OK 10-08       30: OK 40-10
  01: OK 20-08       11: OK 01-01       21: OK 02-02       31: OK 04-08
  02: OK 01-02       12: OK 04-04       22: OK 01-08       32: OK 20-02
  03: OK 01-04       13: OK 04-01       23: OK 10-10       33: OK 40-08
  04: OK 04-02       14: OK 08-08       24: OK 02-10       34: OK 20-01
  05: OK 04-10       15: OK 10-02       25: OK 80-01       35: OK 02-04
  06: OK 10-01       16: OK 01-10       26: OK 80-02       36: OK 02-01
  07: OK 10-04       17: OK 40-01       27: OK 08-01
  08: OK 20-10       18: OK 40-04       28: OK 08-04
  09: OK 02-08       19: OK 08-02       29: OK 20-04


  .------------------++----+----+----+----+----+----+----+----.
  |  Set \ Writes... || 80 | 40 | 20 | 10 | 08 | 04 | 02 | 01 |
  +------------------++----+----+----+----+----+----+----+----+
  +------------------++----+----+----+----+----+----+----+----+
  |  Set 01 numbers  || 25 | 17 | 34 | 06 | 27 | 13 | 36 | 11 |
  +------------------++----+----+----+----+----+----+----+----+
  |  Set 02 numbers  || 26 | 00 | 32 | 15 | 19 | 04 | 21 | 02 |
  +------------------++----+----+----+----+----+----+----+----+
  |  Set 04 numbers  || -- | 18 | 29 | 07 | 28 | 12 | 35 | 03 |
  +------------------++----+----+----+----+----+----+----+----+
  |  Set 08 numbers  || -- | 33 | 01 | 20 | 14 | 31 | 09 | 22 |
  +------------------++----+----+----+----+----+----+----+----+
  |  Set 10 numbers  || -- | 30 | 08 | 23 | 10 | 05 | 24 | 16 |
  '------------------++----+----+----+----+----+----+----+----'


**************************************************************************************

  Inputs...

  Scanned BUS:
  .-.
  | |-D0--- BET Button.
  | |-D1--- DOWN Button.
  | |-D2--- RIGHT Button.
  | |-D3--- LEFT Button.
  | |-D4--- UP Button.
  | |-D5--- OUT Credits.
  | |-D6--- IN Credits.
  | |-D7--- Audit/Test.
  '-'

  Whilest in Audit/Test mode, pressing the ADVANCE button, you can enter into
  screen and inputs test mode.

  This ADVANCE button, is routed into a latch with D0, sharing with BET button.


**************************************************************************************

  Game Notes:
  -----------

  You have 3 controls for operators.
  The Operator/Habilitation Key, the Audit/Test Mode switch and the Advance button.

  To enter the game, you should turn the Audit/Test switch ON, then OFF, otherwise
  a louder and annoying alarm will warn you about.

  Once in game, you need to turn ON the Operator/Habilitation Key to allow credit IN
  and OUT all 6 players. If the key is OFF, the IN/OUT credit system will not work,
  getting the game only in attract mode.


  * Audit/Test Mode

  There are 2 controls inside the cab:
  1x Audit/Test Switch.
  1x Avance Pantalla (Advance) button.

  In the disclaimer, turn the Audit/Test Switch ON.
  You can see a screen with all the bookkeepings.

  While in Audit mode, you need to press the ADVANCE button to enter the first test.

  Black screen. All roulette LEDs ON.

  then...

  White screen. Test all sequenced LEDs in both ways, when pressing the CREDIT IN/OUT
  buttons. You need the Habilitation key ON to do this test.

  Press ADVANCE again, and you'll see the color palette screen.

  Press ADVANCE again, and you'll see the grid pattern screen.

  Press ADVANCE again, and you'll enter the input test mode. All control buttons from
  each player, including Credits IN/OUT will be shown here.

  Press ADVANCE again, and you'll arrive to the Audit screen again.

  To exit the Audit/Test mode, turn OFF the Audit/Test switch. you'll enter the game.


**************************************************************************************

  TODO:

  - Check why some sound events are louder than others (volume balance?).
  - Fix NVRAM.
  - PCB layout.


*************************************************************************************/


#define MASTER_CLOCK		XTAL_21_4772MHz

#include "emu.h"
#include "cpu/z80/z80.h"
#include "sound/ay8910.h"
#include "video/v9938.h"
#include "machine/nvram.h"
#include "kas89.lh"


class kas89_state : public driver_device
{
public:
	kas89_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag),
		  m_v9938(*this, "v9938")
		{ }

	UINT8 m_mux_data;
	UINT8 m_main_nmi_enable;

	UINT8 m_leds_mux_selector;
	UINT8 m_leds_mux_data;
	UINT8 m_outdata;			/* Muxed with the sound latch. Output to a sign? */

	device_t *m_maincpu;
	device_t *m_audiocpu;

	required_device<v9938_device> m_v9938;
};

#define VDP_MEM             0x40000


/***************************************
*      Interrupt handling & Video      *
***************************************/

static void kas89_vdp_interrupt(device_t *, v99x8_device &device, int i)
{
	cputag_set_input_line (device.machine(), "maincpu", 0, (i ? ASSERT_LINE : CLEAR_LINE));
}

static TIMER_DEVICE_CALLBACK( kas89_interrupt )
{
	kas89_state *state = timer.machine().driver_data<kas89_state>();
	int scanline = param;

	if((scanline % 2) == 0)
		state->m_v9938->interrupt();
}


/*************************************
*       Machine Start & Reset        *
*************************************/

static MACHINE_START( kas89 )
{
	kas89_state *state = machine.driver_data<kas89_state>();
	state->m_maincpu = machine.device("maincpu");
	state->m_audiocpu = machine.device("audiocpu");

	output_set_lamp_value(37, 0);	/* turning off the operator led */
}

static MACHINE_RESET(kas89)
{
	kas89_state *state = machine.driver_data<kas89_state>();

	state->m_main_nmi_enable = 0;
}


/****************************************
*  Input Ports Demux & Common Routines  *
****************************************/

static WRITE8_HANDLER( mux_w )
{
/*  - bits -
    7654 3210
    xxxx xxxx   Input selector.

    Writes: 1-2-4-8-10-20-40-3f. Only 80 (for DSW) at very begining...
*/
	kas89_state *state = space->machine().driver_data<kas89_state>();
	state->m_mux_data = data;
}

static READ8_HANDLER( mux_r )
{
	kas89_state *state = space->machine().driver_data<kas89_state>();
	switch(state->m_mux_data)
	{
		case 0x01: return input_port_read(space->machine(), "PL1");
		case 0x02: return input_port_read(space->machine(), "PL2");
		case 0x04: return input_port_read(space->machine(), "PL3");
		case 0x08: return input_port_read(space->machine(), "PL4");
		case 0x10: return input_port_read(space->machine(), "PL5");
		case 0x20: return input_port_read(space->machine(), "PL6");
		case 0x40:
		{
			output_set_lamp_value(37, 1 - ((input_port_read(space->machine(), "SVC") >> 5) & 1));	/* Operator Key LAMP */
			return input_port_read(space->machine(), "SVC");
		}
		case 0x80: return input_port_read(space->machine(), "DSW");	/* Polled at $162a through NMI routine */
		case 0x3f: return input_port_read(space->machine(), "UNK");
	}

	logerror("Mux_data %02X\n", state->m_mux_data);
	return state->m_mux_data;
}

static TIMER_DEVICE_CALLBACK ( kas89_nmi_cb )
{
	kas89_state *state = timer.machine().driver_data<kas89_state>();

	if (state->m_main_nmi_enable)
		device_set_input_line(state->m_maincpu, INPUT_LINE_NMI, PULSE_LINE);
}

static TIMER_DEVICE_CALLBACK ( kas89_sound_nmi_cb )
{
	kas89_state *state = timer.machine().driver_data<kas89_state>();

	device_set_input_line(state->m_audiocpu, INPUT_LINE_NMI, PULSE_LINE);
}


static WRITE8_HANDLER( control_w )
{
/*  - bits -
    7654 3210
    ---- ---x   Credits In counter (x10). Active low.
    ---- --x-   Credits Out counter (x1). Active low.
    ---- -x--   Unknown. Active low.
    -x-- ----   NMI enable. Active high.
    x-xx x---   Unused.
*/
	kas89_state *state = space->machine().driver_data<kas89_state>();
	state->m_main_nmi_enable = data & 0x40;

	coin_counter_w(space->machine(), 0, (data ^ 0xff) & 0x01);	/* Credits In counter */
	coin_counter_w(space->machine(), 1, (data ^ 0xff) & 0x02);	/* Credits Out counter */
}

static WRITE8_HANDLER( sound_comm_w )
{
/*  This port is used mainly for sound latch, but bit6 activates a
    sort of output port (maybe for a sign?)

    bit6 = 0 ; sound latch.
    bit6 = 1 ; outport data.

    Once the ball is landed, the outport writes the winner number
    14 times (as an intermitent way).

    When the attract starts, just before the game title appear, $3f
    is written to the outport... (maybe to clear the possible sign).

*/
	kas89_state *state = space->machine().driver_data<kas89_state>();

	if (data & 0x40)
	{
		state->m_outdata = (data ^ 0x40);	/* Pure data. Without the activator bit */

		if (state->m_outdata == 0x3f)
		{
			UINT8 i;
			for ( i = 0; i < 37; i++ )
			{
				output_set_lamp_value(i, 0);	/* All roulette LEDs OFF */
			}
		}

		logerror("Outdata: Write %02x\n", state->m_outdata);
	}

	else
	{
		soundlatch_w(space, 0, data);
		device_set_input_line(state->m_audiocpu, 0, ASSERT_LINE );
	}
}

static WRITE8_HANDLER( int_ack_w )
{
	kas89_state *state = space->machine().driver_data<kas89_state>();
	device_set_input_line(state->m_audiocpu, 0, CLEAR_LINE );
}


/****************************************
*   Output Ports Demux & LEDs Support   *
****************************************/

static WRITE8_HANDLER( led_mux_data_w )
{
/*  - bits -
    7654 3210
    xxxx xxxx   LEDs data.
*/
	kas89_state *state = space->machine().driver_data<kas89_state>();
	state->m_leds_mux_data = data;
}

static WRITE8_HANDLER( led_mux_select_w )
{
/*  - bits -
    7654 3210
    ---x xxxx   LEDs Set selector.
*/
	kas89_state *state = space->machine().driver_data<kas89_state>();
	state->m_leds_mux_selector = data;

	UINT8 i;
	for ( i = 0; i < 37; i++ )
	{
		output_set_lamp_value(i, 0);	/* All LEDs OFF */
	}

	switch(data)
	{
		case 0x00:
		{
			for ( i = 0; i < 37; i++ )
			{
				output_set_lamp_value(i, 0);	/* All LEDs OFF */
			}
		}

		case 0x01:
		{
			output_set_lamp_value(11, (state->m_leds_mux_data >> 0) & 1);	/* Number 11 LED */
			output_set_lamp_value(36, (state->m_leds_mux_data >> 1) & 1);	/* Number 36 LED */
			output_set_lamp_value(13, (state->m_leds_mux_data >> 2) & 1);	/* Number 13 LED */
			output_set_lamp_value(27, (state->m_leds_mux_data >> 3) & 1);	/* Number 27 LED */
			output_set_lamp_value(06, (state->m_leds_mux_data >> 4) & 1);	/* Number  6 LED */
			output_set_lamp_value(34, (state->m_leds_mux_data >> 5) & 1);	/* Number 34 LED */
			output_set_lamp_value(17, (state->m_leds_mux_data >> 6) & 1);	/* Number 17 LED */
			output_set_lamp_value(25, (state->m_leds_mux_data >> 7) & 1);	/* Number 25 LED */
			break;
		}

		case 0x02:
		{
			output_set_lamp_value( 2, (state->m_leds_mux_data >> 0) & 1);	/* Number  2 LED */
			output_set_lamp_value(21, (state->m_leds_mux_data >> 1) & 1);	/* Number 21 LED */
			output_set_lamp_value( 4, (state->m_leds_mux_data >> 2) & 1);	/* Number  4 LED */
			output_set_lamp_value(19, (state->m_leds_mux_data >> 3) & 1);	/* Number 19 LED */
			output_set_lamp_value(15, (state->m_leds_mux_data >> 4) & 1);	/* Number 15 LED */
			output_set_lamp_value(32, (state->m_leds_mux_data >> 5) & 1);	/* Number 32 LED */
			output_set_lamp_value( 0, (state->m_leds_mux_data >> 6) & 1);	/* Number  0 LED */
			output_set_lamp_value(26, (state->m_leds_mux_data >> 7) & 1);	/* Number 26 LED */
			break;
		}

		case 0x04:
		{
			output_set_lamp_value( 3, (state->m_leds_mux_data >> 0) & 1);	/* Number  3 LED */
			output_set_lamp_value(35, (state->m_leds_mux_data >> 1) & 1);	/* Number 35 LED */
			output_set_lamp_value(12, (state->m_leds_mux_data >> 2) & 1);	/* Number 12 LED */
			output_set_lamp_value(28, (state->m_leds_mux_data >> 3) & 1);	/* Number 28 LED */
			output_set_lamp_value( 7, (state->m_leds_mux_data >> 4) & 1);	/* Number  7 LED */
			output_set_lamp_value(29, (state->m_leds_mux_data >> 5) & 1);	/* Number 29 LED */
			output_set_lamp_value(18, (state->m_leds_mux_data >> 6) & 1);	/* Number 18 LED */
			break;
		}

		case 0x08:
		{
			output_set_lamp_value(22, (state->m_leds_mux_data >> 0) & 1);	/* Number 22 LED */
			output_set_lamp_value( 9, (state->m_leds_mux_data >> 1) & 1);	/* Number  9 LED */
			output_set_lamp_value(31, (state->m_leds_mux_data >> 2) & 1);	/* Number 31 LED */
			output_set_lamp_value(14, (state->m_leds_mux_data >> 3) & 1);	/* Number 14 LED */
			output_set_lamp_value(20, (state->m_leds_mux_data >> 4) & 1);	/* Number 20 LED */
			output_set_lamp_value( 1, (state->m_leds_mux_data >> 5) & 1);	/* Number  1 LED */
			output_set_lamp_value(33, (state->m_leds_mux_data >> 6) & 1);	/* Number 33 LED */
			break;
		}

		case 0x10:
		{
			output_set_lamp_value(16, (state->m_leds_mux_data >> 0) & 1);	/* Number 16 LED */
			output_set_lamp_value(24, (state->m_leds_mux_data >> 1) & 1);	/* Number 24 LED */
			output_set_lamp_value( 5, (state->m_leds_mux_data >> 2) & 1);	/* Number  5 LED */
			output_set_lamp_value(10, (state->m_leds_mux_data >> 3) & 1);	/* Number 10 LED */
			output_set_lamp_value(23, (state->m_leds_mux_data >> 4) & 1);	/* Number 23 LED */
			output_set_lamp_value( 8, (state->m_leds_mux_data >> 5) & 1);	/* Number  8 LED */
			output_set_lamp_value(30, (state->m_leds_mux_data >> 6) & 1);	/* Number 30 LED */
			break;
		}

		case 0xff:
		{
			for ( i = 0; i < 37; i++ )
			{
				output_set_lamp_value(i, 1);	/* All LEDs ON */
			}
		}
	}
}


/**************************************
*             Memory Map              *
**************************************/

static ADDRESS_MAP_START( kas89_map, AS_PROGRAM, 8 )
	AM_RANGE(0x0000, 0x7fff) AM_ROM
	AM_RANGE(0x0037, 0x0037) AM_WRITENOP	/* WTF? */
	AM_RANGE(0x8000, 0x81ff) AM_RAM		/* empty */
	AM_RANGE(0x8200, 0x83ff) AM_RAM		/* 8200-83ff (PL1) */
	AM_RANGE(0x8400, 0x85ff) AM_RAM		/* 8400-85ff (PL2) */
	AM_RANGE(0x8600, 0x87ff) AM_RAM		/* 8600-87ff (PL3) */
	AM_RANGE(0x8800, 0x89ff) AM_RAM		/* 8800-89ff (PL4) */
	AM_RANGE(0x8a00, 0x8bff) AM_RAM		/* 8a00-8bff (PL5) */
	AM_RANGE(0x8c00, 0x8dff) AM_RAM		/* 8c00-8dff (PL6) */
	AM_RANGE(0x8e00, 0x8fff) AM_RAM		/* empty */
	AM_RANGE(0x9000, 0x97ff) AM_RAM
	AM_RANGE(0x9800, 0x9fff) AM_RAM AM_SHARE("nvram")
ADDRESS_MAP_END

static ADDRESS_MAP_START( kas89_io, AS_IO, 8 )
	ADDRESS_MAP_GLOBAL_MASK(0xff)
	AM_RANGE(0x40, 0x43) AM_DEVREADWRITE_MODERN("v9938", v9938_device, read, write)
	AM_RANGE(0x80, 0x80) AM_WRITE(mux_w)
	AM_RANGE(0x81, 0x81) AM_READ(mux_r)
	AM_RANGE(0x82, 0x82) AM_WRITE(control_w)	/* Bit6 trigger the 138Hz osc. tied to main Z80's NMI.*/
	AM_RANGE(0x83, 0x83) AM_WRITE(led_mux_data_w)
	AM_RANGE(0x84, 0x84) AM_WRITE(led_mux_select_w)
	AM_RANGE(0x85, 0x85) AM_WRITE(sound_comm_w)
ADDRESS_MAP_END

/*
  900d --- (RAM) NMI stores the read from / write to port $82

  Write to $82 -> 07  ; Disable main CPU NMI
  Write to $82 -> 47  ; Enable main CPU NMI
  Write to $83 -> 02  ; LEDs data
  Write to $84 -> 01  ; LEDs selector
  Write to $85 -> 64  ; Write winner number (+ bit6 active)
  Write to $85 -> 04  ; Write to audio CPU.
  Write to $82 -> 07  ; Disable main CPU NMI

  Write to $82 -> 07  ; Disable main CPU NMI
  Write to $82 -> 47  ; Enable main CPU NMI
  Write to $83 -> 00  ; LEDs data
  Write to $84 -> 00  ; LEDs selector
  Write to $85 -> 64  ; Write winner number (+ bit6 active)
  Write to $85 -> 03  ; Write to audio CPU.
  Write to $82 -> 07  ; Disable main CPU NMI

  NMI enable/disable:

  4a88 : push af
  4a89 : ld   a,($900d)
  4a8c : set  6,a
  4a8e : ld   ($900d),a
  4a91 : out  ($82),a
  4a93 : pop  af
  4a94 : ret

  4a95 : push af
  4a96 : ld   a,($900d)
  4a99 : res  6,a
  4a9b : ld   ($900d),a
  4a9e : out  ($82),a
  4aa0 : pop  af
  4aa1 : ret
*/

static ADDRESS_MAP_START( audio_map, AS_PROGRAM, 8 )
	AM_RANGE(0x0000, 0x0fff) AM_ROM
	AM_RANGE(0xc000, 0xc7ff) AM_RAM
ADDRESS_MAP_END

static ADDRESS_MAP_START( audio_io, AS_IO, 8 )
	ADDRESS_MAP_GLOBAL_MASK(0xff)
	AM_RANGE(0x00, 0x00) AM_WRITE(int_ack_w)	// comm out (1st Z80). seems to write here the value previously read through soundlatch (port 0x02).
	AM_RANGE(0x02, 0x02) AM_READ(soundlatch_r)
	AM_RANGE(0x04, 0x04) AM_DEVREAD("aysnd", ay8910_r)
	AM_RANGE(0x04, 0x05) AM_DEVWRITE("aysnd", ay8910_data_address_w)
ADDRESS_MAP_END


/**************************************
*            Input Ports              *
**************************************/

static INPUT_PORTS_START( kas89 )

	PORT_START("PL1")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_OTHER ) PORT_CODE(KEYCODE_LCONTROL) PORT_NAME("Player 1 - Bet")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_OTHER ) PORT_CODE(KEYCODE_DOWN)     PORT_NAME("Player 1 - Down")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_OTHER ) PORT_CODE(KEYCODE_LEFT)     PORT_NAME("Player 1 - Left")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_OTHER ) PORT_CODE(KEYCODE_RIGHT)    PORT_NAME("Player 1 - Right")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_OTHER ) PORT_CODE(KEYCODE_UP)       PORT_NAME("Player 1 - Up")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_OTHER ) PORT_CODE(KEYCODE_Z)        PORT_NAME("Player 1 - Credits OUT")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_OTHER ) PORT_CODE(KEYCODE_1)        PORT_NAME("Player 1 - Credits IN")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_OTHER ) PORT_CODE(KEYCODE_LALT)     PORT_NAME("Player 1 - Unknown")

	PORT_START("PL2")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_OTHER ) PORT_CODE(KEYCODE_7_PAD) PORT_NAME("Player 2 - Bet")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_OTHER ) PORT_CODE(KEYCODE_2_PAD) PORT_NAME("Player 2 - Down")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_OTHER ) PORT_CODE(KEYCODE_4_PAD) PORT_NAME("Player 2 - Left")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_OTHER ) PORT_CODE(KEYCODE_6_PAD) PORT_NAME("Player 2 - Right")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_OTHER ) PORT_CODE(KEYCODE_8_PAD) PORT_NAME("Player 2 - Up")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_OTHER ) PORT_CODE(KEYCODE_X)     PORT_NAME("Player 2 - Credits OUT")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_OTHER ) PORT_CODE(KEYCODE_2)     PORT_NAME("Player 2 - Credits IN")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_OTHER ) PORT_CODE(KEYCODE_5_PAD) PORT_NAME("Player 2 - Unknown")

	PORT_START("PL3")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_OTHER ) PORT_CODE(KEYCODE_Q) PORT_NAME("Player 3 - Bet")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_OTHER ) PORT_CODE(KEYCODE_S) PORT_NAME("Player 3 - Down")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_OTHER ) PORT_CODE(KEYCODE_A) PORT_NAME("Player 3 - Left")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_OTHER ) PORT_CODE(KEYCODE_D) PORT_NAME("Player 3 - Right")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_OTHER ) PORT_CODE(KEYCODE_W) PORT_NAME("Player 3 - Up")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_OTHER ) PORT_CODE(KEYCODE_C) PORT_NAME("Player 3 - Credits OUT")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_OTHER ) PORT_CODE(KEYCODE_3) PORT_NAME("Player 3 - Credits IN")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_OTHER ) PORT_CODE(KEYCODE_E) PORT_NAME("Player 3 - Unknown")

	PORT_START("PL4")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_OTHER ) PORT_CODE(KEYCODE_R) PORT_NAME("Player 4 - Bet")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_OTHER ) PORT_CODE(KEYCODE_T) PORT_NAME("Player 4 - Up")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_OTHER ) PORT_CODE(KEYCODE_H) PORT_NAME("Player 4 - Right")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_OTHER ) PORT_CODE(KEYCODE_F) PORT_NAME("Player 4 - Left")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_OTHER ) PORT_CODE(KEYCODE_G) PORT_NAME("Player 4 - Down")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_OTHER ) PORT_CODE(KEYCODE_V) PORT_NAME("Player 4 - Credits OUT")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_OTHER ) PORT_CODE(KEYCODE_4) PORT_NAME("Player 4 - Credits IN")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_OTHER ) PORT_CODE(KEYCODE_Y) PORT_NAME("Player 4 - Unknown")

	PORT_START("PL5")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_OTHER ) PORT_CODE(KEYCODE_U) PORT_NAME("Player 5 - Bet")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_OTHER ) PORT_CODE(KEYCODE_I) PORT_NAME("Player 5 - Up")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_OTHER ) PORT_CODE(KEYCODE_L) PORT_NAME("Player 5 - Right")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_OTHER ) PORT_CODE(KEYCODE_J) PORT_NAME("Player 5 - Left")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_OTHER ) PORT_CODE(KEYCODE_K) PORT_NAME("Player 5 - Down")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_OTHER ) PORT_CODE(KEYCODE_B) PORT_NAME("Player 5 - Credits OUT")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_OTHER ) PORT_CODE(KEYCODE_5) PORT_NAME("Player 5 - Credits IN")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_OTHER ) PORT_CODE(KEYCODE_O) PORT_NAME("Player 5 - Unknown")

	PORT_START("PL6")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_OTHER ) PORT_CODE(KEYCODE_ENTER) PORT_NAME("Player 6 - Bet")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_OTHER ) PORT_CODE(KEYCODE_HOME)  PORT_NAME("Player 6 - Up")
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_OTHER ) PORT_CODE(KEYCODE_PGDN)  PORT_NAME("Player 6 - Right")
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_OTHER ) PORT_CODE(KEYCODE_DEL)   PORT_NAME("Player 6 - Left")
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_OTHER ) PORT_CODE(KEYCODE_END)   PORT_NAME("Player 6 - Down")
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_OTHER ) PORT_CODE(KEYCODE_N)     PORT_NAME("Player 6 - Credits OUT")
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_OTHER ) PORT_CODE(KEYCODE_6)     PORT_NAME("Player 6 - Credits IN")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_OTHER ) PORT_CODE(KEYCODE_PGUP)  PORT_NAME("Player 6 - Unknown")

	PORT_START("SVC")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_SERVICE ) PORT_CODE(KEYCODE_0) PORT_NAME("Avance Pantalla (Advance)")
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_UNKNOWN )	/* Blank the screen. Disclaimer isn't shown.*/
	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_UNKNOWN )	/* Otherwise hang the game when insert credits.*/
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_SERVICE ) PORT_CODE(KEYCODE_8)  PORT_TOGGLE PORT_NAME("Operator/Habilitation Key")
//  PORT_DIPNAME( 0x20, 0x00, "Habilitation Key")
//  PORT_DIPSETTING(    0x20, DEF_STR( Off ) )
//  PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_SERVICE ) PORT_CODE(KEYCODE_9)  PORT_TOGGLE PORT_NAME("Audit/Test Mode Switch")
//  PORT_DIPNAME( 0x80, 0x80, "Audit/Test Mode Switch" )    PORT_DIPLOCATION("SVC:1")
//  PORT_DIPSETTING(    0x80, DEF_STR( Off ) )
//  PORT_DIPSETTING(    0x00, DEF_STR( On ) )


	PORT_START("DSW")
/*
  .-------------------------------+-----+-----+-----+-----+-----+-----+-----+-----.
  | DIP Switches                  |  1  |  2  |  3  |  4  |  5  |  6  |  7  |  8  |  Translation
  +-------------------------------+-----+-----+-----+-----+-----+-----+-----+-----+  -----------
  +-------------------------------+-----+-----+-----------------------------------+  [Game Mode]
  | Modo de Juego      Facil      | OFF | OFF |                                   |    Easy
  |                    Normal     | ON  | OFF |                                   |    Normal
  |                    Dura       | OFF | ON  |                                   |    Hard
  |                    Muy Dura   | ON  | ON  |                                   |    Very Hard
  +-------------------------------+-----+-----+-----+-----+-----------------------+  [Heart Appeareance]
  | Aparicion          No         |           | OFF | OFF |                       |    No
  |    del             Poco       |           | ON  | OFF |                       |    Few
  |  Corazon           (n/c)      |           | OFF | ON  |                       |    n/c
  |                    Mucho      |           | ON  | ON  |                       |    Often
  +-------------------------------+-----------+-----+-----+-----+-----------------+  [RAM Clear]
  | Borrado de RAM     No         |                       | OFF |                 |    No
  |                    Si         |                       | ON  |                 |    Yes
  +-------------------------------+-----------------------+-----+-----+-----------+  [Leveling]
  | Nivelacion     Creditos IN/OUT|                             | OFF |           |    Credits IN/OUT
  |                Pagos/Apuestas |                             | ON  |           |    Payments/Bets
  +-------------------------------+-----------------------------+-----+-----+-----+  [Jackpot Limit]
  | Limite del Pozo    Alto       |                                   | OFF | OFF |    High
  |                    Normal     |                                   | ON  | OFF |    Normal
  |                    Bajo       |                                   | OFF | ON  |    Low
  |                    Muy Bajo   |                                   | ON  | ON  |    Very Low
  '-------------------------------+-----------------------------------+-----+-----'

  .------------------------+---+---+---+---+---+---+---+---.
  | Suggested DIP Switches | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |
  +------------------------+---+---+---+---+---+---+---+---+
  | ON                     | * |   |   |   |   |   | * |   |
  +------------------------+---+---+---+---+---+---+---+---+
  | OFF                    |   | * | * | * | * | * |   | * |
  '------------------------+---+---+---+---+---+---+---+---'

   DSW Routing...

   BIT0 = DSW1-3
   BIT1 = DSW1-6
   BIT2 = DSW1-5
   BIT3 = DSW1-1
   BIT4 = DSW1-8
   BIT5 = DSW1-4
   BIT6 = DSW1-7
   BIT7 = DSW1-2

*/
	PORT_DIPNAME( 0x88, 0x80, "Modo de Juego (Game Mode)" )			PORT_DIPLOCATION("DSW1:1,2")
	PORT_DIPSETTING(    0x88, "Facil (easy)" )
	PORT_DIPSETTING(    0x80, "Normal (Normal)" )
	PORT_DIPSETTING(    0x08, "Dura (Hard)" )
	PORT_DIPSETTING(    0x00, "Muy Dura (Very Hard)" )

	PORT_DIPNAME( 0x21, 0x21, "Aparicion del Corazon (Heart Appeareance)" )	PORT_DIPLOCATION("DSW1:3,4")
	PORT_DIPSETTING(    0x01, "n/c" )
	PORT_DIPSETTING(    0x21, "No (No)" )
	PORT_DIPSETTING(    0x20, "Poco (Few)" )
	PORT_DIPSETTING(    0x00, "Mucho (Often)" )

	PORT_DIPNAME( 0x04, 0x04, "Borrado de RAM (RAM Clear)" )		PORT_DIPLOCATION("DSW1:5")
	PORT_DIPSETTING(    0x04, DEF_STR( No ) )
	PORT_DIPSETTING(    0x00, DEF_STR( Yes ) )

	PORT_DIPNAME( 0x02, 0x02, "Nivelacion (Leveling)" )				PORT_DIPLOCATION("DSW1:6")
	PORT_DIPSETTING(    0x02, "Creditos IN/OUT (Credits IN/OUT)" )
	PORT_DIPSETTING(    0x00, "Pagos/Apuestas (Payments/Bets)" )

	PORT_DIPNAME( 0x50, 0x10, "Limite del Pozo (Jackpot Limit)" )	PORT_DIPLOCATION("DSW1:8,7")
	PORT_DIPSETTING(    0x00, "Muy Bajo (Very Low)" )
	PORT_DIPSETTING(    0x40, "Bajo (Low)" )
	PORT_DIPSETTING(    0x10, "Normal (Normal)" )
	PORT_DIPSETTING(    0x50, "Alto (High)" )


	PORT_START("UNK")	/* Unknown input set */
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_UNKNOWN )	/* Should be active for Audit/Test Mode, also for boot...*/

INPUT_PORTS_END


/**********************************
*       AY-3-8910 Interface       *
**********************************/

static const ay8910_interface ay8910_config =
{
	AY8910_LEGACY_OUTPUT,
	AY8910_DEFAULT_LOADS,
	DEVCB_NULL,		// Nothing connected here.
	DEVCB_NULL,		// Nothing connected here.
	DEVCB_NULL,		// Nothing connected here.
	DEVCB_NULL		// Nothing connected here.
};


/**************************************
*           Machine Driver            *
**************************************/

static MACHINE_CONFIG_START( kas89, kas89_state )

	/* basic machine hardware */
	MCFG_CPU_ADD("maincpu", Z80, MASTER_CLOCK/6)	/* Confirmed */
	MCFG_CPU_PROGRAM_MAP(kas89_map)
	MCFG_CPU_IO_MAP(kas89_io)
	MCFG_TIMER_ADD_SCANLINE("scantimer", kas89_interrupt, "screen", 0, 1)
	MCFG_TIMER_ADD_PERIODIC("kas89_nmi", kas89_nmi_cb, attotime::from_hz(138)) /* Connected to a 138Hz osc. *AND* bit6 of port $82 */

	MCFG_CPU_ADD("audiocpu", Z80, MASTER_CLOCK/6)	/* Confirmed */
	MCFG_CPU_PROGRAM_MAP(audio_map)
	MCFG_CPU_IO_MAP(audio_io)
	MCFG_TIMER_ADD_PERIODIC("kas89_snmi", kas89_sound_nmi_cb, attotime::from_hz(138)) /* Connected to a 138Hz osc.*/

	MCFG_MACHINE_START(kas89)
	MCFG_MACHINE_RESET(kas89)

	MCFG_NVRAM_ADD_0FILL("nvram")

	/* video hardware */
	MCFG_V9938_ADD("v9938", "screen", VDP_MEM)
	MCFG_V99X8_INTERRUPT_CALLBACK_STATIC(kas89_vdp_interrupt)

	MCFG_SCREEN_ADD("screen", RASTER)
	MCFG_SCREEN_REFRESH_RATE(60)
	MCFG_SCREEN_VBLANK_TIME(ATTOSECONDS_IN_USEC(0))
	MCFG_SCREEN_UPDATE_DEVICE("v9938", v9938_device, screen_update)
	MCFG_SCREEN_SIZE(544, 524)
	MCFG_SCREEN_VISIBLE_AREA(0, 544 - 1, 0, 480 - 1)

	MCFG_PALETTE_LENGTH(512)
	MCFG_PALETTE_INIT(v9938)

	/* sound hardware */
	MCFG_SPEAKER_STANDARD_MONO("mono")
	MCFG_SOUND_ADD("aysnd", AY8910, MASTER_CLOCK/12)	/* Confirmed */
	MCFG_SOUND_CONFIG(ay8910_config)
	MCFG_SOUND_ROUTE(ALL_OUTPUTS, "mono", 1.0)
MACHINE_CONFIG_END


/**************************************
*              ROM Load               *
**************************************/

ROM_START( kas89 )
	ROM_REGION( 0x10000, "maincpu", 0 )
	ROM_LOAD( "k89_original_program.bin", 0x0000, 0x8000, CRC(a02b3063) SHA1(f6b05f7c99da2c6719c21687521f841d357d0eb4) )

	ROM_REGION( 0x10000, "audiocpu", 0 )
	ROM_LOAD( "k89_27256_son_2a58.bin", 0x0000, 0x8000, CRC(26636631) SHA1(95fad6a047bb9145d679841aa3ab4d013895b1f7) )
ROM_END


/**************************************
*            Driver Init              *
**************************************/

/* This reflect how was connected the bus to the EPROM
   inside the epoxy CPU block. They used ultra-thin wires,
   just to melt down with the epoxy in case someone try to
   use a heat gun for epoxy removal purposes...

   Bus / Eprom

    D0-> D5
    D1-> D6
    D2-> D0
    D3-> D7
    D4-> D2
    D5-> D4
    D6-> D3
    D7-> D1

   A00-> A10
   A01-> A08
   A02-> A01
   A03-> A11
   A04-> A05
   A05-> A13
   A06-> A12
   A07-> A04
   A08-> A02
   A09-> A07
   A10-> A03
   A11-> A00
   A12-> A09
   A13-> A06
   A14-> A14
*/
static DRIVER_INIT(kas89)
{
	int i;
	UINT8 *mem = machine.region("maincpu")->base();
	int memsize = machine.region("maincpu")->bytes();
	UINT8 *buf;

	/* Unscrambling data lines */
	for ( i = 0; i < memsize; i++ )
	{
		mem[i] = BITSWAP8(mem[i],3,1,0,5,6,4,7,2);
	}

	/* Unscrambling address lines */
	buf = auto_alloc_array(machine, UINT8, memsize);
	memcpy(buf, mem, memsize);
	for ( i = 0; i < memsize; i++ )
	{
		mem[BITSWAP16(i,15,14,5,6,3,0,12,1,9,13,4,7,10,8,2,11)] = buf[i];
	}

	auto_free(machine, buf);
}


/**************************************
*           Game Driver(s)            *
**************************************/

/*     YEAR  NAME      PARENT  MACHINE   INPUT     INIT    ROT     COMPANY       FULLNAME     FLAGS                 LAYOUT */
GAMEL( 1989, kas89,    0,      kas89,    kas89,    kas89,  ROT90, "SFC S.R.L.", "Kasino '89", GAME_IMPERFECT_SOUND, layout_kas89 )
