/*************************************************************************

    Ultraman

*************************************************************************/

class ultraman_state : public driver_device
{
public:
	ultraman_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag) { }

	/* memory pointers */
//  UINT16 *   m_paletteram;    // currently this uses generic palette handling

	/* video-related */
	int        m_sprite_colorbase;
	int        m_zoom_colorbase[3];
	int        m_bank0;
	int        m_bank1;
	int        m_bank2;

	/* devices */
	device_t *m_maincpu;
	device_t *m_audiocpu;
	device_t *m_k051316_1;
	device_t *m_k051316_2;
	device_t *m_k051316_3;
	device_t *m_k051960;
};



/*----------- defined in video/ultraman.c -----------*/

extern void ultraman_sprite_callback(running_machine &machine, int *code,int *color,int *priority,int *shadow);
extern void ultraman_zoom_callback_0(running_machine &machine, int *code,int *color,int *flags);
extern void ultraman_zoom_callback_1(running_machine &machine, int *code,int *color,int *flags);
extern void ultraman_zoom_callback_2(running_machine &machine, int *code,int *color,int *flags);

WRITE16_HANDLER( ultraman_gfxctrl_w );

VIDEO_START( ultraman );
SCREEN_UPDATE_IND16( ultraman );
