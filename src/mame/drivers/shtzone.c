/*

"Sega Shooting Zone" aka "Sega Sharp Shooter"

This is an SMS with a timer system, official Sega product.  Cabinet has a lightgun, it runs the SMS light gun games.

It has 2 IO controllers, and 1 VDP, so I'm guessing the BIOS just displays to some kind of Segment LED display.

---------------------------------

Shooting Zone by SEGA 1987

834-6294

CPU(s) : D780C (x2)

Xtal : 10.7380 Mhz

RAMS(s) : D4168C (x3)
    : MB8464-12L

Eprom : Epr10894A.20

PAL : 315-5287

Customs IC's :  315-5216 (x2)

        315-5124

GAMES for this system :

Black Belt (mpr10150.ic1)

Shooting Gallery

Gangster Town

Marksman Shooting / Trap Shooting / Safari Hunt (315-5028.ic1 + Mpr10157.ic2)

Fantasy Zone(1)

---------------------------------

Notes:
(1) apparently.... seems a bit odd, because it's not a gun game


*/

#include "emu.h"
#include "cpu/z80/z80.h"

class shtzone_state : public driver_device
{
public:
	shtzone_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag) { }
};

static ADDRESS_MAP_START( shtzone_map, AS_PROGRAM, 8 )
	AM_RANGE(0x0000, 0x3fff) AM_ROM
	AM_RANGE(0x4000, 0x5fff) AM_RAM
ADDRESS_MAP_END


static INPUT_PORTS_START( shtzone )
INPUT_PORTS_END


static MACHINE_START( shtzone )
{

}

static MACHINE_RESET( shtzone )
{

}

VIDEO_START( shtzone )
{

}


SCREEN_UPDATE_IND16( shtzone )
{

	return 0;
}


static MACHINE_CONFIG_START( shtzone, shtzone_state )

	/* basic machine hardware */
	MCFG_CPU_ADD("timercpu", Z80,10738000/4)
	MCFG_CPU_PROGRAM_MAP(shtzone_map)

	/* + SMS CPU */

	MCFG_MACHINE_START(shtzone)
	MCFG_MACHINE_RESET(shtzone)

	/* video hardware */
	MCFG_SCREEN_ADD("screen", RASTER)
	MCFG_SCREEN_REFRESH_RATE(60)
	MCFG_SCREEN_VBLANK_TIME(ATTOSECONDS_IN_USEC(0))
	MCFG_SCREEN_SIZE(256, 256)
	MCFG_SCREEN_VISIBLE_AREA(0, 256-1, 0, 256-1)
	MCFG_SCREEN_UPDATE_STATIC(shtzone)

	MCFG_PALETTE_LENGTH(0x100)

	MCFG_VIDEO_START(shtzone)
MACHINE_CONFIG_END


ROM_START( shtzone )
	ROM_REGION( 0x4000, "timercpu", 0 )
	ROM_LOAD( "epr10894a.20", 0x00000, 0x04000, CRC(ea8901d9) SHA1(43fd8bfc395e3b2e3fbe9645d692a5eb04783d9c) )
ROM_END

GAME( 1987, shtzone,  0,    shtzone, shtzone,  0, ROT0, "Sega", "Shooting Zone System BIOS", GAME_IS_SKELETON | GAME_NO_SOUND | GAME_NOT_WORKING | GAME_IS_BIOS_ROOT )
