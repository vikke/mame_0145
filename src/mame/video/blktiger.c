#include "emu.h"
#include "includes/blktiger.h"


#define BGRAM_BANK_SIZE 0x1000
#define BGRAM_BANKS 4


/***************************************************************************

  Callbacks for the TileMap code

***************************************************************************/

static TILEMAP_MAPPER( bg8x4_scan )
{
	/* logical (col,row) -> memory offset */
	return (col & 0x0f) + ((row & 0x0f) << 4) + ((col & 0x70) << 4) + ((row & 0x30) << 7);
}

static TILEMAP_MAPPER( bg4x8_scan )
{
	/* logical (col,row) -> memory offset */
	return (col & 0x0f) + ((row & 0x0f) << 4) + ((col & 0x30) << 4) + ((row & 0x70) << 6);
}

static TILE_GET_INFO( get_bg_tile_info )
{
	blktiger_state *state = machine.driver_data<blktiger_state>();
	/* the tile priority table is a guess compiled by looking at the game. It
       was not derived from a PROM so it could be wrong. */
	static const UINT8 split_table[16] =
	{
		3,3,0,0,
		0,0,0,0,
		0,0,0,0,
		0,0,0,0
	};
	UINT8 attr = state->m_scroll_ram[2 * tile_index + 1];
	int color = (attr & 0x78) >> 3;
	SET_TILE_INFO(
			1,
			state->m_scroll_ram[2 * tile_index] + ((attr & 0x07) << 8),
			color,
			(attr & 0x80) ? TILE_FLIPX : 0);
	tileinfo.group = split_table[color];
}

static TILE_GET_INFO( get_tx_tile_info )
{
	blktiger_state *state = machine.driver_data<blktiger_state>();
	UINT8 attr = state->m_txvideoram[tile_index + 0x400];
	SET_TILE_INFO(
			0,
			state->m_txvideoram[tile_index] + ((attr & 0xe0) << 3),
			attr & 0x1f,
			0);
}


/***************************************************************************

  Start the video hardware emulation.

***************************************************************************/

VIDEO_START( blktiger )
{
	blktiger_state *state = machine.driver_data<blktiger_state>();

	state->m_chon = 1;
	state->m_bgon = 1;
	state->m_objon = 1;
	state->m_screen_layout = 0;

	state->m_scroll_ram = auto_alloc_array(machine, UINT8, BGRAM_BANK_SIZE * BGRAM_BANKS);

	state->m_tx_tilemap =    tilemap_create(machine, get_tx_tile_info, tilemap_scan_rows, 8, 8, 32, 32);
	state->m_bg_tilemap8x4 = tilemap_create(machine, get_bg_tile_info, bg8x4_scan, 16, 16, 128, 64);
	state->m_bg_tilemap4x8 = tilemap_create(machine, get_bg_tile_info, bg4x8_scan, 16, 16, 64, 128);

	state->m_tx_tilemap->set_transparent_pen(3);

	state->m_bg_tilemap8x4->set_transmask(0, 0xffff, 0x8000);	/* split type 0 is totally transparent in front half */
	state->m_bg_tilemap8x4->set_transmask(1, 0xfff0, 0x800f);	/* split type 1 has pens 4-15 transparent in front half */
	state->m_bg_tilemap8x4->set_transmask(2, 0xff00, 0x80ff);	/* split type 1 has pens 8-15 transparent in front half */
	state->m_bg_tilemap8x4->set_transmask(3, 0xf000, 0x8fff);	/* split type 1 has pens 12-15 transparent in front half */
	state->m_bg_tilemap4x8->set_transmask(0, 0xffff, 0x8000);
	state->m_bg_tilemap4x8->set_transmask(1, 0xfff0, 0x800f);
	state->m_bg_tilemap4x8->set_transmask(2, 0xff00, 0x80ff);
	state->m_bg_tilemap4x8->set_transmask(3, 0xf000, 0x8fff);

	state->save_pointer(NAME(state->m_scroll_ram), BGRAM_BANK_SIZE * BGRAM_BANKS);
}



/***************************************************************************

  Memory handlers

***************************************************************************/

WRITE8_HANDLER( blktiger_txvideoram_w )
{
	blktiger_state *state = space->machine().driver_data<blktiger_state>();
	state->m_txvideoram[offset] = data;
	state->m_tx_tilemap->mark_tile_dirty(offset & 0x3ff);
}

READ8_HANDLER( blktiger_bgvideoram_r )
{
	blktiger_state *state = space->machine().driver_data<blktiger_state>();
	return state->m_scroll_ram[offset + state->m_scroll_bank];
}

WRITE8_HANDLER( blktiger_bgvideoram_w )
{
	blktiger_state *state = space->machine().driver_data<blktiger_state>();
	offset += state->m_scroll_bank;

	state->m_scroll_ram[offset] = data;
	state->m_bg_tilemap8x4->mark_tile_dirty(offset / 2);
	state->m_bg_tilemap4x8->mark_tile_dirty(offset / 2);
}

WRITE8_HANDLER( blktiger_bgvideoram_bank_w )
{
	blktiger_state *state = space->machine().driver_data<blktiger_state>();
	state->m_scroll_bank = (data % BGRAM_BANKS) * BGRAM_BANK_SIZE;
}


WRITE8_HANDLER( blktiger_scrolly_w )
{
	blktiger_state *state = space->machine().driver_data<blktiger_state>();
	int scrolly;

	state->m_scroll_y[offset] = data;
	scrolly = state->m_scroll_y[0] | (state->m_scroll_y[1] << 8);
	state->m_bg_tilemap8x4->set_scrolly(0, scrolly);
	state->m_bg_tilemap4x8->set_scrolly(0, scrolly);
}

WRITE8_HANDLER( blktiger_scrollx_w )
{
	blktiger_state *state = space->machine().driver_data<blktiger_state>();
	int scrollx;

	state->m_scroll_x[offset] = data;
	scrollx = state->m_scroll_x[0] | (state->m_scroll_x[1] << 8);
	state->m_bg_tilemap8x4->set_scrollx(0, scrollx);
	state->m_bg_tilemap4x8->set_scrollx(0, scrollx);
}


WRITE8_HANDLER( blktiger_video_control_w )
{
	blktiger_state *state = space->machine().driver_data<blktiger_state>();
	/* bits 0 and 1 are coin counters */
	coin_counter_w(space->machine(), 0,data & 1);
	coin_counter_w(space->machine(), 1,data & 2);

	/* bit 5 resets the sound CPU */
	device_set_input_line(state->m_audiocpu, INPUT_LINE_RESET, (data & 0x20) ? ASSERT_LINE : CLEAR_LINE);

	/* bit 6 flips screen */
	flip_screen_set(space->machine(), data & 0x40);

	/* bit 7 enables characters? Just a guess */
	state->m_chon = ~data & 0x80;
}

WRITE8_HANDLER( blktiger_video_enable_w )
{
	blktiger_state *state = space->machine().driver_data<blktiger_state>();

	/* not sure which is which, but I think that bit 1 and 2 enable background and sprites */
	/* bit 1 enables bg ? */
	state->m_bgon = ~data & 0x02;

	/* bit 2 enables sprites ? */
	state->m_objon = ~data & 0x04;
}

WRITE8_HANDLER( blktiger_screen_layout_w )
{
	blktiger_state *state = space->machine().driver_data<blktiger_state>();
	state->m_screen_layout = data;
	state->m_bg_tilemap8x4->enable(state->m_screen_layout);
	state->m_bg_tilemap4x8->enable(!state->m_screen_layout);
}



/***************************************************************************

  Display refresh

***************************************************************************/

static void draw_sprites( running_machine &machine, bitmap_ind16 &bitmap, const rectangle &cliprect )
{
//  blktiger_state *state = machine.driver_data<blktiger_state>();
	UINT8 *buffered_spriteram = machine.generic.buffered_spriteram.u8;
	int offs;

	/* Draw the sprites. */
	for (offs = machine.generic.spriteram_size - 4;offs >= 0;offs -= 4)
	{
		int attr = buffered_spriteram[offs+1];
		int sx = buffered_spriteram[offs + 3] - ((attr & 0x10) << 4);
		int sy = buffered_spriteram[offs + 2];
		int code = buffered_spriteram[offs] | ((attr & 0xe0) << 3);
		int color = attr & 0x07;
		int flipx = attr & 0x08;

		if (flip_screen_get(machine))
		{
			sx = 240 - sx;
			sy = 240 - sy;
			flipx = !flipx;
		}

		drawgfx_transpen(bitmap,cliprect,machine.gfx[2],
				code,
				color,
				flipx,flip_screen_get(machine),
				sx,sy,15);
	}
}

SCREEN_UPDATE_IND16( blktiger )
{
	blktiger_state *state = screen.machine().driver_data<blktiger_state>();

	bitmap.fill(1023, cliprect);

	if (state->m_bgon)
		(state->m_screen_layout ? state->m_bg_tilemap8x4 : state->m_bg_tilemap4x8)->draw(bitmap, cliprect, TILEMAP_DRAW_LAYER1, 0);

	if (state->m_objon)
		draw_sprites(screen.machine(), bitmap, cliprect);

	if (state->m_bgon)
		(state->m_screen_layout ? state->m_bg_tilemap8x4 : state->m_bg_tilemap4x8)->draw(bitmap, cliprect, TILEMAP_DRAW_LAYER0, 0);

	if (state->m_chon)
		state->m_tx_tilemap->draw(bitmap, cliprect, 0, 0);

	return 0;
}

SCREEN_VBLANK( blktiger )
{
	// rising edge
	if (vblank_on)
	{
		address_space *space = screen.machine().device("maincpu")->memory().space(AS_PROGRAM);

		buffer_spriteram_w(space, 0, 0);
	}
}
