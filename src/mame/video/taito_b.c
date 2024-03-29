#include "emu.h"
#include "profiler.h"
#include "video/hd63484.h"
#include "video/taitoic.h"
#include "includes/taito_b.h"

WRITE16_HANDLER( hitice_pixelram_w )
{
	taitob_state *state = space->machine().driver_data<taitob_state>();
	int sy = offset >> 9;
	int sx = offset & 0x1ff;

	COMBINE_DATA(&state->m_pixelram[offset]);

	if (ACCESSING_BITS_0_7)
	{
		/* bit 15 of pixel_scroll[0] is probably flip screen */
		state->m_pixel_bitmap->pix16(sy, 2 * sx + 0) = state->m_b_fg_color_base * 16 + (data & 0xff);
		state->m_pixel_bitmap->pix16(sy, 2 * sx + 1) = state->m_b_fg_color_base * 16 + (data & 0xff);
	}
}

WRITE16_HANDLER( hitice_pixel_scroll_w )
{
	taitob_state *state = space->machine().driver_data<taitob_state>();
	COMBINE_DATA(&state->m_pixel_scroll[offset]);
}

static void hitice_clear_pixel_bitmap( running_machine &machine )
{
	int i;
	address_space *space = machine.device("maincpu")->memory().space(AS_PROGRAM);

	for (i = 0; i < 0x40000; i++)
		hitice_pixelram_w(space, i, 0, 0xffff);
}

WRITE16_HANDLER( realpunc_video_ctrl_w )
{
	taitob_state *state = space->machine().driver_data<taitob_state>();
	COMBINE_DATA(&state->m_realpunc_video_ctrl);
}

static VIDEO_START( taitob_core )
{
	taitob_state *state = machine.driver_data<taitob_state>();

	state->m_framebuffer[0] = auto_bitmap_ind16_alloc(machine, 512, 256);
	state->m_framebuffer[1] = auto_bitmap_ind16_alloc(machine, 512, 256);
	state->m_pixel_bitmap = NULL;  /* only hitice needs this */

	state->save_item(NAME(state->m_pixel_scroll));

	state->save_item(NAME(*state->m_framebuffer[0]));
	state->save_item(NAME(*state->m_framebuffer[1]));
}

VIDEO_START( taitob_color_order0 )
{
	/*graphics are shared, only that they use different palette*/
	/*this is the basic layout used in: Nastar, Ashura Blaster, Hit the Ice, Rambo3, Tetris*/

	/*Note that in both this and color order 1 pixel_color_base/color_granularity is equal to sprites color base. Pure coincidence? */

	taitob_state *state = machine.driver_data<taitob_state>();
	state->m_b_sp_color_base = 0x40 * 16;	/*sprites   */

	/* bg, fg, tx color_base are set in the tc0180vcu interface */

	VIDEO_START_CALL(taitob_core);
}

VIDEO_START( taitob_color_order1 )
{
	/* this is the reversed layout used in: Crime City, Puzzle Bobble */
	taitob_state *state = machine.driver_data<taitob_state>();
	state->m_b_sp_color_base = 0x80 * 16;

	VIDEO_START_CALL(taitob_core);
}

VIDEO_START( taitob_color_order2 )
{
	/*this is used in: rambo3a, masterw, silentd, selfeena, ryujin */
	taitob_state *state = machine.driver_data<taitob_state>();
	state->m_b_sp_color_base = 0x10 * 16;

	VIDEO_START_CALL(taitob_core);
}


VIDEO_START( hitice )
{
	taitob_state *state = machine.driver_data<taitob_state>();

	VIDEO_START_CALL(taitob_color_order0);

	state->m_b_fg_color_base = 0x80;		/* hitice also uses this for the pixel_bitmap */

	state->m_pixel_bitmap = auto_bitmap_ind16_alloc(machine, 1024, 512);

	state->save_item(NAME(*state->m_pixel_bitmap));
}

VIDEO_RESET( hitice )
{
	/* kludge: clear the bitmap on startup */
	hitice_clear_pixel_bitmap(machine);
}


VIDEO_START( realpunc )
{
	taitob_state *state = machine.driver_data<taitob_state>();

	state->m_realpunc_bitmap = auto_bitmap_ind16_alloc(machine, machine.primary_screen->width(), machine.primary_screen->height());

	VIDEO_START_CALL(taitob_color_order0);
}


READ16_HANDLER( tc0180vcu_framebuffer_word_r )
{
	taitob_state *state = space->machine().driver_data<taitob_state>();
	int sy = offset >> 8;
	int sx = 2 * (offset & 0xff);

	return (state->m_framebuffer[sy >> 8]->pix16(sy & 0xff, sx + 0) << 8) | state->m_framebuffer[sy >> 8]->pix16(sy & 0xff, sx + 1);
}

WRITE16_HANDLER( tc0180vcu_framebuffer_word_w )
{
	taitob_state *state = space->machine().driver_data<taitob_state>();
	int sy = offset >> 8;
	int sx = 2 * (offset & 0xff);

	if (ACCESSING_BITS_8_15)
		state->m_framebuffer[sy >> 8]->pix16(sy & 0xff, sx + 0) = data >> 8;
	if (ACCESSING_BITS_0_7)
		state->m_framebuffer[sy >> 8]->pix16(sy & 0xff, sx + 1) = data & 0xff;
}


static void draw_sprites( running_machine &machine, bitmap_ind16 &bitmap, const rectangle &cliprect )
{
/*  Sprite format: (16 bytes per sprite)
  offs:             bits:
  0000: 0xxxxxxxxxxxxxxx: tile code - 0x0000 to 0x7fff in qzshowby
  0002: 0000000000xxxxxx: color (0x00 - 0x3f)
        x000000000000000: flipy
        0x00000000000000: flipx
        00????????000000: unused ?
  0004: xxxxxx0000000000: doesn't matter - some games (eg nastar) fill this with sign bit, some (eg ashura) do not
        000000xxxxxxxxxx: x-coordinate 10 bits signed (all zero for sprites forming up a big sprite, except for the first one)
  0006: xxxxxx0000000000: doesn't matter - same as x
        000000xxxxxxxxxx: y-coordinate 10 bits signed (same as x)
  0008: xxxxxxxx00000000: sprite x-zoom level
        00000000xxxxxxxx: sprite y-zoom level
      0x00 - non scaled = 100%
      0x80 - scaled to 50%
      0xc0 - scaled to 25%
      0xe0 - scaled to 12.5%
      0xff - scaled to zero pixels size (off)
      Sprite zoom is used in Ashura Blaster just in the beginning
      where you can see a big choplifter and a japanese title.
      This japanese title is a scaled sprite.
      It is used in Crime City also at the end of the third level (in the garage)
      where there are four columns on the sides of the screen
      Heaviest usage is in Rambo 3 - almost every sprite in game is scaled
  000a: xxxxxxxx00000000: x-sprites number (big sprite) decremented by one
        00000000xxxxxxxx: y-sprites number (big sprite) decremented by one
  000c - 000f: unused
*/

	taitob_state *state = machine.driver_data<taitob_state>();
	int x, y, xlatch = 0, ylatch = 0, x_no = 0, y_no = 0, x_num = 0, y_num = 0, big_sprite = 0;
	int offs, code, color, flipx, flipy;
	UINT32 data, zoomx, zoomy, zx, zy, zoomxlatch = 0, zoomylatch = 0;

	for (offs = (0x1980 - 16) / 2; offs >=0; offs -= 8)
	{
		code = state->m_spriteram[offs];

		color = state->m_spriteram[offs + 1];
		flipx = color & 0x4000;
		flipy = color & 0x8000;
#if 0
		/*check the unknown bits*/
		if (color & 0x3fc0)
		{
			logerror("sprite color (taitob)=%4x ofs=%4x\n", color, offs);
			color = rand() & 0x3f;
		}
#endif
		color = (color & 0x3f) * 16;

		x = state->m_spriteram[offs + 2] & 0x3ff;
		y = state->m_spriteram[offs + 3] & 0x3ff;
		if (x >= 0x200)  x -= 0x400;
		if (y >= 0x200)  y -= 0x400;

		data = state->m_spriteram[offs + 5];
		if (data)
		{
			if (!big_sprite)
			{
				x_num = (data >> 8) & 0xff;
				y_num = (data >> 0) & 0xff;
				x_no  = 0;
				y_no  = 0;
				xlatch = x;
				ylatch = y;
				data = state->m_spriteram[offs + 4];
				zoomxlatch = (data >> 8) & 0xff;
				zoomylatch = (data >> 0) & 0xff;
				big_sprite = 1;
			}
		}

		data = state->m_spriteram[offs + 4];
		zoomx = (data >> 8) & 0xff;
		zoomy = (data >> 0) & 0xff;
		zx = (0x100 - zoomx) / 16;
		zy = (0x100 - zoomy) / 16;

		if (big_sprite)
		{
			zoomx = zoomxlatch;
			zoomy = zoomylatch;

			/* Note: like taito_f2.c, this zoom implementation is wrong,
            chopped up into 16x16 sections instead of one sprite. This
            is especially visible in rambo3. */

			x = xlatch + (x_no * (0xff - zoomx) + 15) / 16;
			y = ylatch + (y_no * (0xff - zoomy) + 15) / 16;
			zx = xlatch + ((x_no + 1) * (0xff - zoomx) + 15) / 16 - x;
			zy = ylatch + ((y_no + 1) * (0xff - zoomy) + 15) / 16 - y;
			y_no++;

			if (y_no > y_num)
			{
				y_no = 0;
				x_no++;

				if (x_no > x_num)
					big_sprite = 0;
			}
		}

		if ( zoomx || zoomy )
		{
			drawgfxzoom_transpen_raw(bitmap, cliprect, machine.gfx[1],
				code,
				color,
				flipx,flipy,
				x,y,
				(zx << 16) / 16,(zy << 16) / 16,0);
		}
		else
		{
			drawgfx_transpen_raw (bitmap,cliprect,machine.gfx[1],
				code,
				color,
				flipx,flipy,
				x,y,
				0);
		}
	}
}


static void draw_framebuffer( running_machine &machine, bitmap_ind16 &bitmap, const rectangle &cliprect, int priority )
{
	taitob_state *state = machine.driver_data<taitob_state>();
	rectangle myclip = cliprect;
	int x, y;
	UINT8 video_control = tc0180vcu_get_videoctrl(state->m_tc0180vcu, 0);
	UINT8 framebuffer_page = tc0180vcu_get_fb_page(state->m_tc0180vcu, 0);

g_profiler.start(PROFILER_USER1);

	priority <<= 4;

	if (video_control & 0x08)
	{
		if (priority)
		{
			g_profiler.stop();
			return;
		}

		if (video_control & 0x10)	/*flip screen*/
		{
			/*popmessage("1. X[%3i;%3i] Y[%3i;%3i]", myclip.min_x, myclip.max_x, myclip.min_y, myclip.max_y);*/
			for (y = myclip.min_y; y <= myclip.max_y; y++)
			{
				UINT16 *src = &state->m_framebuffer[framebuffer_page]->pix16(y, myclip.min_x);
				UINT16 *dst;

				dst = &bitmap.pix16(bitmap.height()-1-y, myclip.max_x);

				for (x = myclip.min_x; x <= myclip.max_x; x++)
				{
					UINT16 c = *src++;

					if (c != 0)
						*dst = state->m_b_sp_color_base + c;

					dst--;
				}
			}
		}
		else
		{
			for (y = myclip.min_y; y <= myclip.max_y; y++)
			{
				UINT16 *src = &state->m_framebuffer[framebuffer_page]->pix16(y, myclip.min_x);
				UINT16 *dst = &bitmap.pix16(y, myclip.min_x);

				for (x = myclip.min_x; x <= myclip.max_x; x++)
				{
					UINT16 c = *src++;

					if (c != 0)
						*dst = state->m_b_sp_color_base + c;

					dst++;
				}
			}
		}
	}
	else
	{
		if (video_control & 0x10)   /*flip screen*/
		{
			/*popmessage("3. X[%3i;%3i] Y[%3i;%3i]", myclip.min_x, myclip.max_x, myclip.min_y, myclip.max_y);*/
			for (y = myclip.min_y ;y <= myclip.max_y; y++)
			{
				UINT16 *src = &state->m_framebuffer[framebuffer_page]->pix16(y, myclip.min_x);
				UINT16 *dst;

				dst = &bitmap.pix16(bitmap.height()-1-y, myclip.max_x);

				for (x = myclip.min_x; x <= myclip.max_x; x++)
				{
					UINT16 c = *src++;

					if (c != 0 && (c & 0x10) == priority)
						*dst = state->m_b_sp_color_base + c;

					dst--;
				}
			}
		}
    	else
	    {
	        for (y = myclip.min_y; y <= myclip.max_y; y++)
			{
				UINT16 *src = &state->m_framebuffer[framebuffer_page]->pix16(y, myclip.min_x);
				UINT16 *dst = &bitmap.pix16(y, myclip.min_x);

				for (x = myclip.min_x; x <= myclip.max_x; x++)
				{
					UINT16 c = *src++;

					if (c != 0 && (c & 0x10) == priority)
						*dst = state->m_b_sp_color_base + c;

					dst++;
				}
			}
		}
	}
g_profiler.stop();
}

SCREEN_UPDATE_IND16( taitob )
{
	taitob_state *state = screen.machine().driver_data<taitob_state>();
	UINT8 video_control = tc0180vcu_get_videoctrl(state->m_tc0180vcu, 0);

	if ((video_control & 0x20) == 0)
	{
		bitmap.fill(0, cliprect);
		return 0;
	}

	/* Draw playfields */
	tc0180vcu_tilemap_draw(state->m_tc0180vcu, bitmap, cliprect, 0, 1);

	draw_framebuffer(screen.machine(), bitmap, cliprect, 1);

	tc0180vcu_tilemap_draw(state->m_tc0180vcu, bitmap, cliprect, 1, 0);

	if (state->m_pixel_bitmap)  /* hitice only */
	{
		int scrollx = -2 * state->m_pixel_scroll[0]; //+320;
		int scrolly = - state->m_pixel_scroll[1]; //+240;
		/* bit 15 of pixel_scroll[0] is probably flip screen */

		copyscrollbitmap_trans(bitmap, *state->m_pixel_bitmap, 1, &scrollx, 1, &scrolly, cliprect, state->m_b_fg_color_base * 16);
	}

	draw_framebuffer(screen.machine(), bitmap, cliprect, 0);

	tc0180vcu_tilemap_draw(state->m_tc0180vcu, bitmap, cliprect, 2, 0);

	return 0;
}



SCREEN_UPDATE_RGB32( realpunc )
{
	taitob_state *state = screen.machine().driver_data<taitob_state>();
	const rgb_t *palette = palette_entry_list_adjusted(screen.machine().palette);
	UINT8 video_control = tc0180vcu_get_videoctrl(state->m_tc0180vcu, 0);
	int x, y;

	/* Video blanked? */
	if (!(video_control & 0x20))
	{
		bitmap.fill(0, cliprect);
		return 0;
	}

	/* Draw the palettized playfields to an indexed bitmap */
	tc0180vcu_tilemap_draw(state->m_tc0180vcu, *state->m_realpunc_bitmap, cliprect, 0, 1);

	draw_framebuffer(screen.machine(), *state->m_realpunc_bitmap, cliprect, 1);

	tc0180vcu_tilemap_draw(state->m_tc0180vcu, *state->m_realpunc_bitmap, cliprect, 1, 0);

	if (state->m_realpunc_video_ctrl & 0x0001)
		draw_framebuffer(screen.machine(), *state->m_realpunc_bitmap, cliprect, 0);

	/* Copy the intermediate bitmap to the output bitmap, applying the palette */
	for (y = 0; y <= cliprect.max_y; y++)
		for (x = 0; x <= cliprect.max_x; x++)
			bitmap.pix32(y, x) = palette[state->m_realpunc_bitmap->pix16(y, x)];

	/* Draw the 15bpp raw CRTC frame buffer directly to the output bitmap */
	if (state->m_realpunc_video_ctrl & 0x0002)
	{
		device_t *hd63484 = screen.machine().device("hd63484");

		int base = (hd63484_regs_r(hd63484, 0xcc/2, 0xffff) << 16) + hd63484_regs_r(hd63484, 0xce/2, 0xffff);
		int stride = hd63484_regs_r(hd63484, 0xca/2, 0xffff);

//      scrollx = taitob_scroll[0];
//      scrolly = taitob_scroll[1];

		for (y = 0; y <= cliprect.max_y; y++)
		{
			int addr = base + (y*stride);
			for (x = 0; x <= cliprect.max_x; x++)
			{
				int r, g, b;
				UINT16 srcpix = hd63484_ram_r(hd63484, addr++, 0xffff);

				r = (BIT(srcpix, 1)) | ((srcpix >> 11) & 0x1e);
				g = (BIT(srcpix, 2)) | ((srcpix >> 7) & 0x1e);
				b = (BIT(srcpix, 3)) | ((srcpix >> 3) & 0x1e);

				if (srcpix)
					bitmap.pix32(y, x) = MAKE_RGB(pal5bit(r), pal5bit(g), pal5bit(b));
			}

			addr += stride;
		}
	}
	/* Draw the 15bpp raw output of the camera ADCs (TODO) */
	else if (state->m_realpunc_video_ctrl & 0x0004)
	{
		for (y = 0; y <= cliprect.max_y; y++)
		{
			for (x = 0; x <= cliprect.max_x; x++)
				bitmap.pix32(y, x) = MAKE_RGB(0x00, 0x00, 0x00);
		}
	}

	/* Clear the indexed bitmap and draw the final indexed layers */
	state->m_realpunc_bitmap->fill(0, cliprect);

	if (!(state->m_realpunc_video_ctrl & 0x0001))
		draw_framebuffer(screen.machine(), *state->m_realpunc_bitmap, cliprect, 0);

	tc0180vcu_tilemap_draw(state->m_tc0180vcu, *state->m_realpunc_bitmap, cliprect, 2, 0);

	/* Merge the indexed layers with the output bitmap */
	for (y = 0; y <= cliprect.max_y; y++)
	{
		for (x = 0; x <= cliprect.max_x; x++)
		{
			if (state->m_realpunc_bitmap->pix16(y, x))
				bitmap.pix32(y, x) = palette[state->m_realpunc_bitmap->pix16(y, x)];
		}
	}

	return 0;
}



SCREEN_VBLANK( taitob )
{
	// rising edge
	if (vblank_on)
	{
		taitob_state *state = screen.machine().driver_data<taitob_state>();
		UINT8 video_control = tc0180vcu_get_videoctrl(state->m_tc0180vcu, 0);
		UINT8 framebuffer_page = tc0180vcu_get_fb_page(state->m_tc0180vcu, 0);

		if (~video_control & 0x01)
			state->m_framebuffer[framebuffer_page]->fill(0, screen.machine().primary_screen->visible_area());

		if (~video_control & 0x80)
		{
			framebuffer_page ^= 1;
			tc0180vcu_set_fb_page(state->m_tc0180vcu, 0, framebuffer_page);
		}

		draw_sprites(screen.machine(), *state->m_framebuffer[framebuffer_page], screen.machine().primary_screen->visible_area());
	}
}
