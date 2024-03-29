/***************************************************************************

    Atari Cloud 9 (prototype) hardware

***************************************************************************/

#include "emu.h"
#include "includes/cloud9.h"
#include "video/resnet.h"


/*************************************
 *
 *  Video startup
 *
 *************************************/

VIDEO_START( cloud9 )
{
	cloud9_state *state = machine.driver_data<cloud9_state>();
	static const int resistances[3] = { 22000, 10000, 4700 };

	/* allocate second bank of videoram */
	state->m_videoram = auto_alloc_array(machine, UINT8, 0x8000);
	memory_set_bankptr(machine, "bank1", state->m_videoram);

	/* get pointers to our PROMs */
	state->m_syncprom = machine.region("proms")->base() + 0x000;
	state->m_wpprom = machine.region("proms")->base() + 0x200;
	state->m_priprom = machine.region("proms")->base() + 0x300;

	/* compute the color output resistor weights at startup */
	compute_resistor_weights(0,	255, -1.0,
			3,	resistances, state->m_rweights, 1000, 0,
			3,	resistances, state->m_gweights, 1000, 0,
			3,	resistances, state->m_bweights, 1000, 0);

	/* allocate a bitmap for drawing sprites */
	machine.primary_screen->register_screen_bitmap(state->m_spritebitmap);

	/* register for savestates */
	state->save_pointer(NAME(state->m_videoram), 0x8000);
	state->save_item(NAME(state->m_video_control));
	state->save_item(NAME(state->m_bitmode_addr));
}



/*************************************
 *
 *  Video control registers
 *
 *************************************/

WRITE8_HANDLER( cloud9_video_control_w )
{
	cloud9_state *state = space->machine().driver_data<cloud9_state>();

	/* only D7 matters */
	state->m_video_control[offset] = (data >> 7) & 1;
}



/*************************************
 *
 *  Palette RAM accesses
 *
 *************************************/

WRITE8_HANDLER( cloud9_paletteram_w )
{
	cloud9_state *state = space->machine().driver_data<cloud9_state>();
	int bit0, bit1, bit2;
	int r, g, b;

	/* extract the raw RGB bits */
	r = (data & 0xe0) >> 5;
	g = (data & 0x1c) >> 2;
	b = ((data & 0x03) << 1) | ((offset & 0x40) >> 6);

	/* red component (inverted) */
	bit0 = (~r >> 0) & 0x01;
	bit1 = (~r >> 1) & 0x01;
	bit2 = (~r >> 2) & 0x01;
	r = combine_3_weights(state->m_rweights, bit0, bit1, bit2);

	/* green component (inverted) */
	bit0 = (~g >> 0) & 0x01;
	bit1 = (~g >> 1) & 0x01;
	bit2 = (~g >> 2) & 0x01;
	g = combine_3_weights(state->m_gweights, bit0, bit1, bit2);

	/* blue component (inverted) */
	bit0 = (~b >> 0) & 0x01;
	bit1 = (~b >> 1) & 0x01;
	bit2 = (~b >> 2) & 0x01;
	b = combine_3_weights(state->m_bweights, bit0, bit1, bit2);

	palette_set_color(space->machine(), offset & 0x3f, MAKE_RGB(r, g, b));
}



/*************************************
 *
 *  Video RAM access via the write
 *  protect PROM
 *
 *************************************/

INLINE void cloud9_write_vram( running_machine &machine, UINT16 addr, UINT8 data, UINT8 bitmd, UINT8 pixba )
{
	cloud9_state *state = machine.driver_data<cloud9_state>();
	UINT8 *dest = &state->m_videoram[0x0000 | (addr & 0x3fff)];
	UINT8 *dest2 = &state->m_videoram[0x4000 | (addr & 0x3fff)];
	UINT8 promaddr = 0;
	UINT8 wpbits;

	/*
        Inputs to the write-protect PROM:

        Bit 7 = BITMD
        Bit 6 = video_control[4]
        Bit 5 = video_control[6]
        Bit 4 = 1 if (A15-A12 != 4)
        Bit 3 = !(A13 | A12 | A11)
        Bit 2 = A9 & A10
        Bit 1 = PIXB
        Bit 0 = PIXA
    */
	promaddr |= bitmd << 7;
	promaddr |= state->m_video_control[4] << 6;
	promaddr |= state->m_video_control[6] << 5;
	promaddr |= ((addr & 0xf000) != 0x4000) << 4;
	promaddr |= ((addr & 0x3800) == 0x0000) << 3;
	promaddr |= ((addr & 0x0600) == 0x0600) << 2;
	promaddr |= (pixba << 0);

	/* look up the PROM result */
	wpbits = state->m_wpprom[promaddr];

	/* write to the appropriate parts of VRAM depending on the result */
	if (!(wpbits & 1))
		dest2[0] = (dest2[0] & 0x0f) | (data & 0xf0);
	if (!(wpbits & 2))
		dest2[0] = (dest2[0] & 0xf0) | (data & 0x0f);
	if (!(wpbits & 4))
		dest[0] = (dest[0] & 0x0f) | (data & 0xf0);
	if (!(wpbits & 8))
		dest[0] = (dest[0] & 0xf0) | (data & 0x0f);
}



/*************************************
 *
 *  Autoincrement control for bit mode
 *
 *************************************/

INLINE void bitmode_autoinc( running_machine &machine )
{
	cloud9_state *state = machine.driver_data<cloud9_state>();

	/* auto increment in the x-direction if it's enabled */
	if (!state->m_video_control[0])	/* /AX */
		state->m_bitmode_addr[0]++;

	/* auto increment in the y-direction if it's enabled */
	if (!state->m_video_control[1])	/* /AY */
		state->m_bitmode_addr[1]++;
}



/*************************************
 *
 *  Standard video RAM access
 *
 *************************************/

WRITE8_HANDLER( cloud9_videoram_w )
{
	/* direct writes to VRAM go through the write protect PROM as well */
	cloud9_write_vram(space->machine(), offset, data, 0, 0);
}



/*************************************
 *
 *  Bit mode video RAM access
 *
 *************************************/

READ8_HANDLER( cloud9_bitmode_r )
{
	cloud9_state *state = space->machine().driver_data<cloud9_state>();

	/* in bitmode, the address comes from the autoincrement latches */
	UINT16 addr = (state->m_bitmode_addr[1] << 6) | (state->m_bitmode_addr[0] >> 2);

	/* the appropriate pixel is selected into the upper 4 bits */
	UINT8 result = state->m_videoram[((~state->m_bitmode_addr[0] & 2) << 13) | addr] << ((state->m_bitmode_addr[0] & 1) * 4);

	/* autoincrement because /BITMD was selected */
	bitmode_autoinc(space->machine());

	/* the upper 4 bits of the data lines are not driven so make them all 1's */
	return (result >> 4) | 0xf0;
}


WRITE8_HANDLER( cloud9_bitmode_w )
{
	cloud9_state *state = space->machine().driver_data<cloud9_state>();

	/* in bitmode, the address comes from the autoincrement latches */
	UINT16 addr = (state->m_bitmode_addr[1] << 6) | (state->m_bitmode_addr[0] >> 2);

	/* the lower 4 bits of data are replicated to the upper 4 bits */
	data = (data & 0x0f) | (data << 4);

	/* write through the generic VRAM routine, passing the low 2 X bits as PIXB/PIXA */
	cloud9_write_vram(space->machine(), addr, data, 1, state->m_bitmode_addr[0] & 3);

	/* autoincrement because /BITMD was selected */
	bitmode_autoinc(space->machine());
}


WRITE8_HANDLER( cloud9_bitmode_addr_w )
{
	cloud9_state *state = space->machine().driver_data<cloud9_state>();

	/* write through to video RAM and also to the addressing latches */
	cloud9_write_vram(space->machine(), offset, data, 0, 0);
	state->m_bitmode_addr[offset] = data;
}



/*************************************
 *
 *  Video updating
 *
 *************************************/

SCREEN_UPDATE_IND16( cloud9 )
{
	cloud9_state *state = screen.machine().driver_data<cloud9_state>();
	UINT8 *spriteaddr = state->m_spriteram;
	int flip = state->m_video_control[5] ? 0xff : 0x00;	/* PLAYER2 */
	pen_t black = get_black_pen(screen.machine());
	int x, y, offs;

	/* draw the sprites */
	state->m_spritebitmap.fill(0x00, cliprect);
	for (offs = 0; offs < 0x20; offs++)
		if (spriteaddr[offs + 0x00] != 0)
		{
			int x = spriteaddr[offs + 0x60];
			int y = 256 - 15 - spriteaddr[offs + 0x00];
			int xflip = spriteaddr[offs + 0x40] & 0x80;
			int yflip = spriteaddr[offs + 0x40] & 0x40;
			int which = spriteaddr[offs + 0x20];
			int color = 0;

			drawgfx_transpen(state->m_spritebitmap, cliprect, screen.machine().gfx[0], which, color, xflip, yflip, x, y, 0);
			if (x >= 256 - 16)
				drawgfx_transpen(state->m_spritebitmap, cliprect, screen.machine().gfx[0], which, color, xflip, yflip, x - 256, y, 0);
		}

	/* draw the bitmap to the screen, looping over Y */
	for (y = cliprect.min_y; y <= cliprect.max_y; y++)
	{
		UINT16 *dst = &bitmap.pix16(y);

		/* if we're in the VBLANK region, just fill with black */
		if (~state->m_syncprom[y] & 2)
		{
			for (x = cliprect.min_x; x <= cliprect.max_x; x++)
				dst[x] = black;
		}

		/* non-VBLANK region: merge the sprites and the bitmap */
		else
		{
			UINT16 *mosrc = &state->m_spritebitmap.pix16(y);
			int effy = y ^ flip;
			UINT8 *src[2];

			/* two videoram arrays */
			src[0] = &state->m_videoram[0x4000 | (effy * 64)];
			src[1] = &state->m_videoram[0x0000 | (effy * 64)];

			/* loop over X */
			for (x = cliprect.min_x; x <= cliprect.max_x; x++)
			{
				/* if we're in the HBLANK region, just store black */
				if (x >= 256)
					dst[x] = black;

				/* otherwise, process normally */
				else
				{
					int effx = x ^ flip;

					/* low 4 bits = left pixel, high 4 bits = right pixel */
					UINT8 pix = (src[(effx >> 1) & 1][effx / 4] >> ((~effx & 1) * 4)) & 0x0f;
					UINT8 mopix = mosrc[x];

					/* sprites have priority if sprite pixel != 0 or some other condition */
					if (mopix != 0)
						pix = mopix | 0x10;

					/* the high bit is the bank select */
					pix |= state->m_video_control[7] << 5;

					/* store the pixel value and also a priority value based on the topmost bit */
					dst[x] = pix;
				}
			}
		}
	}
	return 0;
}
