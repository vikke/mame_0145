/*************************************************************************

    Atari Subs hardware

*************************************************************************/

#include "sound/discrete.h"

/* Discrete Sound Input Nodes */
#define SUBS_SONAR1_EN			NODE_01
#define SUBS_SONAR2_EN			NODE_02
#define SUBS_LAUNCH_DATA		NODE_03
#define SUBS_CRASH_DATA			NODE_04
#define SUBS_CRASH_EN			NODE_05
#define SUBS_EXPLODE_EN			NODE_06
#define SUBS_NOISE_RESET		NODE_07


class subs_state : public driver_device
{
public:
	subs_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag) { }

	UINT8 *m_videoram;
	UINT8 *m_spriteram;
	int m_steering_buf1;
	int m_steering_buf2;
	int m_steering_val1;
	int m_steering_val2;
	int m_last_val_1;
	int m_last_val_2;
};


/*----------- defined in machine/subs.c -----------*/

MACHINE_RESET( subs );
INTERRUPT_GEN( subs_interrupt );
WRITE8_HANDLER( subs_steer_reset_w );
READ8_HANDLER( subs_control_r );
READ8_HANDLER( subs_coin_r );
READ8_HANDLER( subs_options_r );
WRITE8_HANDLER( subs_lamp1_w );
WRITE8_HANDLER( subs_lamp2_w );


/*----------- defined in audio/subs.c -----------*/

WRITE8_DEVICE_HANDLER( subs_noise_reset_w );
WRITE8_DEVICE_HANDLER( subs_sonar2_w );
WRITE8_DEVICE_HANDLER( subs_sonar1_w );
WRITE8_DEVICE_HANDLER( subs_crash_w );
WRITE8_DEVICE_HANDLER( subs_explode_w );

DISCRETE_SOUND_EXTERN( subs );


/*----------- defined in video/subs.c -----------*/

SCREEN_UPDATE_IND16( subs_left );
SCREEN_UPDATE_IND16( subs_right );

WRITE8_HANDLER( subs_invert1_w );
WRITE8_HANDLER( subs_invert2_w );
