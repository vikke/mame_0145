/*************************************************************************

    Block Hole

*************************************************************************/

class blockhl_state : public driver_device
{
public:
	blockhl_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag) { }

	/* memory pointers */
	UINT8 *    m_ram;
//  UINT8 *    m_paletteram;    // currently this uses generic palette handling

	/* video-related */
	int        m_layer_colorbase[3];
	int        m_sprite_colorbase;

	/* misc */
	int        m_palette_selected;
	int        m_rombank;

	/* devices */
	device_t *m_maincpu;
	device_t *m_audiocpu;
	device_t *m_k052109;
	device_t *m_k051960;
};

/*----------- defined in video/blockhl.c -----------*/

extern void blockhl_tile_callback(running_machine &machine, int layer,int bank,int *code,int *color, int *flags, int *priority);
extern void blockhl_sprite_callback(running_machine &machine, int *code,int *color,int *priority,int *shadow);

VIDEO_START( blockhl );
SCREEN_UPDATE_IND16( blockhl );
