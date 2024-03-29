/***************************************************************************

    World Rally

    Functions to emulate general aspects of the machine (RAM, ROM, interrupts,
    I/O ports)

***************************************************************************/

#include "emu.h"
#include "cpu/mcs51/mcs51.h"
#include "includes/gaelcrpt.h"
#include "includes/wrally.h"

/***************************************************************************

    World Rally memory handlers

***************************************************************************/

WRITE16_HANDLER( wrally_vram_w )
{
	wrally_state *state = space->machine().driver_data<wrally_state>();
	data = gaelco_decrypt(space, offset, data, 0x1f, 0x522a);
	COMBINE_DATA(&state->m_videoram[offset]);

	state->m_pant[(offset & 0x1fff) >> 12]->mark_tile_dirty(((offset << 1) & 0x1fff) >> 2);
}

WRITE16_HANDLER( wrally_flipscreen_w )
{
	flip_screen_set(space->machine(), data & 0x01);
}

WRITE16_HANDLER( OKIM6295_bankswitch_w )
{
	UINT8 *RAM = space->machine().region("oki")->base();

	if (ACCESSING_BITS_0_7){
		memcpy(&RAM[0x30000], &RAM[0x40000 + (data & 0x0f)*0x10000], 0x10000);
	}
}

WRITE16_HANDLER( wrally_coin_counter_w )
{
	coin_counter_w( space->machine(), (offset >> 3) & 0x01, data & 0x01);
}

WRITE16_HANDLER( wrally_coin_lockout_w )
{
	coin_lockout_w( space->machine(), (offset >> 3) & 0x01, ~data & 0x01);
}

