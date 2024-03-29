/***************************************************************************

    JPM IMPACT with Video hardware

****************************************************************************/

struct duart_t
{
	UINT8 MR1A, MR2A;
	UINT8 SRA, CSRA;
	UINT8 CRA;
	UINT8 RBA, TBA;

	UINT8 IPCR;
	UINT8 ACR;
	UINT8 ISR, IMR;

	union
	{
		UINT8 CUR, CLR;
		UINT16 CR;
	};
	union
	{
		UINT8 CTUR, CTLR;
		UINT16 CT;
	};

	int tc;

	UINT8 MR1B, MR2B;
	UINT8 SRB, CSRB;
	UINT8 CRB;
	UINT8 RBB, TBB;

	UINT8 IVR;
	UINT8 IP;
	UINT8 OP;
	UINT8 OPR;
	UINT8 OPCR;
};

struct bt477_t
{
	UINT8 address;
	UINT8 addr_cnt;
	UINT8 pixmask;
	UINT8 command;
	rgb_t color;
};

class jpmimpct_state : public driver_device
{
public:
	jpmimpct_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag) { }

	UINT8 m_tms_irq;
	UINT8 m_duart_1_irq;
	struct duart_t m_duart_1;
	UINT8 m_touch_cnt;
	UINT8 m_touch_data[3];
	int m_lamp_strobe;
	UINT8 m_Lamps[256];
	int m_optic_pattern;
	int m_payen;
	int m_alpha_data_line;
	int m_alpha_clock;
	int m_hopinhibit;
	int m_slidesout;
	int m_hopper[3];
	int m_motor[3];
	UINT16 *m_vram;
	struct bt477_t m_bt477;
};


/*----------- defined in video/jpmimpct.c -----------*/

READ16_HANDLER( jpmimpct_bt477_r );
WRITE16_HANDLER( jpmimpct_bt477_w );

void jpmimpct_to_shiftreg(address_space *space, UINT32 address, UINT16 *shiftreg);
void jpmimpct_from_shiftreg(address_space *space, UINT32 address, UINT16 *shiftreg);
void jpmimpct_scanline_update(screen_device &screen, bitmap_rgb32 &bitmap, int scanline, const tms34010_display_params *params);

VIDEO_START( jpmimpct );
