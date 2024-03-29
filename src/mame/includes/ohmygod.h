/*************************************************************************

    Oh My God!

*************************************************************************/

class ohmygod_state : public driver_device
{
public:
	ohmygod_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag) { }


	/* memory pointers */
	UINT16 *    m_videoram;
	UINT16 *    m_spriteram;
	size_t      m_spriteram_size;

	/* video-related */
	tilemap_t    *m_bg_tilemap;
	int m_spritebank;
	UINT16 m_scrollx;
	UINT16 m_scrolly;

	/* misc */
	int m_adpcm_bank_shift;
	int m_sndbank;
};


/*----------- defined in video/ohmygod.c -----------*/

WRITE16_HANDLER( ohmygod_videoram_w );
WRITE16_HANDLER( ohmygod_spritebank_w );
WRITE16_HANDLER( ohmygod_scrollx_w );
WRITE16_HANDLER( ohmygod_scrolly_w );

VIDEO_START( ohmygod );
SCREEN_UPDATE_IND16( ohmygod );
