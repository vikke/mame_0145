/*
    Williams System 6
*/

#define ADDRESS_MAP_MODERN

#include "emu.h"
#include "cpu/m6800/m6800.h"

class williams_s6_state : public driver_device
{
public:
	williams_s6_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag),
		  m_maincpu(*this, "maincpu")
	{ }

protected:

	// devices
	required_device<cpu_device> m_maincpu;

	// driver_device overrides
	virtual void machine_reset();
};


static ADDRESS_MAP_START( williams_s6_map, AS_PROGRAM, 8, williams_s6_state )
	AM_RANGE(0x0000, 0xffff) AM_NOP
ADDRESS_MAP_END

static INPUT_PORTS_START( williams_s6 )
INPUT_PORTS_END

void williams_s6_state::machine_reset()
{
}

static DRIVER_INIT( williams_s6 )
{
}

static MACHINE_CONFIG_START( williams_s6, williams_s6_state )
	/* basic machine hardware */
	MCFG_CPU_ADD("maincpu", M6800, 1000000)
	MCFG_CPU_PROGRAM_MAP(williams_s6_map)
MACHINE_CONFIG_END

/*----------------------------
/ Blackout - Sys.6 (Game #495)
/---------------------------*/
ROM_START(blkou_l1)
	ROM_REGION(0x10000, "maincpu", 0)
	ROM_LOAD("gamerom.716", 0x6000, 0x0800, CRC(4b407ae2) SHA1(46a2afcfc2d969c5acae18b57a678265257a6102))
	ROM_RELOAD( 0xe000, 0x0800)
	ROM_LOAD("green2.716", 0x7800, 0x0800, CRC(1c978a4a) SHA1(1959184764643d58f1740c54bb74c2aad7d667d2))
	ROM_RELOAD( 0xf800, 0x0800)
	ROM_LOAD("green1.716", 0x7000, 0x0800, CRC(2145f8ab) SHA1(ddf63208559a3a08d4e88327c55426b0eed27654))
	ROM_RELOAD( 0xf000, 0x0800)
	ROM_REGION(0x10000, "cpu2", 0)
	ROM_LOAD("sound2.716", 0x7800, 0x0800, CRC(c9103a68) SHA1(cc77af54fdb192f0b334d9d1028210618c3f1d95))
	ROM_RELOAD( 0xf800, 0x0800)
	ROM_LOAD("v_ic7.532", 0x3000, 0x1000, CRC(87864071) SHA1(d03c71efc0431f30a07c8194c0614c96fb683710))
	ROM_RELOAD( 0xb000, 0x1000)
	ROM_LOAD("v_ic5.532", 0x4000, 0x1000, CRC(046a96d8) SHA1(879127a88b3640bbb202c64cbf8678869c964177))
	ROM_RELOAD( 0xc000, 0x1000)
	ROM_LOAD("v_ic6.532", 0x5000, 0x1000, CRC(0104e5c4) SHA1(c073cb4bdea189085ae074e9c16872752b6ffba0))
	ROM_RELOAD( 0xd000, 0x1000)
ROM_END

ROM_START(blkou_t1)
	ROM_REGION(0x10000, "maincpu", 0)
	ROM_LOAD("gamerom.716", 0x6000, 0x0800, CRC(4b407ae2) SHA1(46a2afcfc2d969c5acae18b57a678265257a6102))
	ROM_RELOAD( 0xe000, 0x0800)
	ROM_LOAD("green2a.716", 0x7800, 0x0800, CRC(16621eec) SHA1(14e1cf5f7227860a3219b2b79fa66dcf252dce98))
	ROM_RELOAD( 0xf800, 0x0800)
	ROM_LOAD("green1.716", 0x7000, 0x0800, CRC(2145f8ab) SHA1(ddf63208559a3a08d4e88327c55426b0eed27654))
	ROM_RELOAD( 0xf000, 0x0800)
	ROM_REGION(0x10000, "cpu2", 0)
	ROM_LOAD("sound2.716", 0x7800, 0x0800, CRC(c9103a68) SHA1(cc77af54fdb192f0b334d9d1028210618c3f1d95))
	ROM_RELOAD( 0xf800, 0x0800)
	ROM_LOAD("v_ic7.532", 0x3000, 0x1000, CRC(87864071) SHA1(d03c71efc0431f30a07c8194c0614c96fb683710))
	ROM_RELOAD( 0xb000, 0x1000)
	ROM_LOAD("v_ic5.532", 0x4000, 0x1000, CRC(046a96d8) SHA1(879127a88b3640bbb202c64cbf8678869c964177))
	ROM_RELOAD( 0xc000, 0x1000)
	ROM_LOAD("v_ic6.532", 0x5000, 0x1000, CRC(0104e5c4) SHA1(c073cb4bdea189085ae074e9c16872752b6ffba0))
	ROM_RELOAD( 0xd000, 0x1000)
ROM_END

ROM_START(blkou_f1)
	ROM_REGION(0x10000, "maincpu", 0)
	ROM_LOAD("gamerom.716", 0x6000, 0x0800, CRC(4b407ae2) SHA1(46a2afcfc2d969c5acae18b57a678265257a6102))
	ROM_RELOAD( 0xe000, 0x0800)
	ROM_LOAD("green2.716", 0x7800, 0x0800, CRC(1c978a4a) SHA1(1959184764643d58f1740c54bb74c2aad7d667d2))
	ROM_RELOAD( 0xf800, 0x0800)
	ROM_LOAD("green1.716", 0x7000, 0x0800, CRC(2145f8ab) SHA1(ddf63208559a3a08d4e88327c55426b0eed27654))
	ROM_RELOAD( 0xf000, 0x0800)
	ROM_REGION(0x10000, "cpu2", 0)
	ROM_LOAD("sound2.716", 0x7800, 0x0800, CRC(c9103a68) SHA1(cc77af54fdb192f0b334d9d1028210618c3f1d95))
	ROM_RELOAD( 0xf800, 0x0800)
	ROM_LOAD("speech7f.532", 0x3000, 0x1000, CRC(bdc1b0b1) SHA1(c78f8653dfe3ec58722a8a17da7924e4a76cf692))
	ROM_RELOAD( 0xb000, 0x1000)
	ROM_LOAD("speech6f.532", 0x4000, 0x1000, CRC(9b7e4ae9) SHA1(137b5ec871162329cb7ca3a62da3193382223d8a))
	ROM_RELOAD( 0xc000, 0x1000)
	ROM_LOAD("speech5f.532", 0x5000, 0x1000, CRC(9040f34a) SHA1(529eae0b58f3300f2b9bdf40c5ca7f4b29425dff))
	ROM_RELOAD( 0xd000, 0x1000)
	ROM_LOAD("speech4f.532", 0x6000, 0x1000, CRC(29c4abde) SHA1(b3af7b8d0c2548f5c0bb240aa1dc5cc59bb2af9a))
	ROM_RELOAD( 0xe000, 0x1000)
ROM_END

/*-------------------------------
/ Firepower - Sys.6 (Game #497)
/------------------------------*/
ROM_START(frpwr_l6)
	ROM_REGION(0x10000, "maincpu", 0)
	ROM_LOAD("gamerom.716", 0x6800, 0x0800, CRC(fdd3b983) SHA1(fb5d1eb01589311cf4b2ef16e25db03d40bca2f7))
	ROM_RELOAD( 0xe800, 0x0800)
	ROM_LOAD("green2.716", 0x7800, 0x0800, CRC(1c978a4a) SHA1(1959184764643d58f1740c54bb74c2aad7d667d2))
	ROM_RELOAD( 0xf800, 0x0800)
	ROM_LOAD("green1.716", 0x7000, 0x0800, CRC(2145f8ab) SHA1(ddf63208559a3a08d4e88327c55426b0eed27654))
	ROM_RELOAD( 0xf000, 0x0800)
	ROM_LOAD("prom1_6.474", 0x6000, 0x0200, CRC(af6eb0b9) SHA1(28f8366737e09ffd60cb5ea55a5734143cdb9663))
	ROM_RELOAD( 0xe000, 0x0200)
	ROM_LOAD("prom2.474", 0x6200, 0x0200, CRC(f75ade1a) SHA1(a5572c5c721dbcb82988b709f4ef2119118e37c2))
	ROM_RELOAD( 0xe000, 0x0200)
	ROM_LOAD("prom3.474", 0x6400, 0x0200, CRC(242ec687) SHA1(c3366c898a66c78034687e6a6000193d52be4141))
	ROM_RELOAD( 0xe000, 0x0200)
	ROM_REGION(0x10000, "cpu2", 0)
	ROM_LOAD("sound3.716", 0x7800, 0x0800, CRC(55a10d13) SHA1(521d4cdfb0ed8178b3594cedceae93b772a951a4))
	ROM_RELOAD( 0xf800, 0x0800)
	ROM_LOAD("v_ic7.532", 0x3000, 0x1000, CRC(94c5c0a7) SHA1(ff7c618d1666c1d5c3319fdd72c1af2846659290))
	ROM_RELOAD( 0xb000, 0x1000)
	ROM_LOAD("v_ic5.532", 0x4000, 0x1000, CRC(1737fdd2) SHA1(6307e0ae715e97294ee8aaaeb2e2bebb0cb590c2))
	ROM_RELOAD( 0xc000, 0x1000)
	ROM_LOAD("v_ic6.532", 0x5000, 0x1000, CRC(e56f7aa2) SHA1(cb922c3f4d91285dda4ccae880c2d798a82fd51b))
	ROM_RELOAD( 0xd000, 0x1000)
ROM_END

ROM_START(frpwr_t6)
	ROM_REGION(0x10000, "maincpu", 0)
	ROM_LOAD("gamerom.716", 0x6800, 0x0800, CRC(fdd3b983) SHA1(fb5d1eb01589311cf4b2ef16e25db03d40bca2f7))
	ROM_RELOAD( 0xe800, 0x0800)
	ROM_LOAD("green2a.716", 0x7800, 0x0800, CRC(16621eec) SHA1(14e1cf5f7227860a3219b2b79fa66dcf252dce98))
	ROM_RELOAD( 0xf800, 0x0800)
	ROM_LOAD("green1.716", 0x7000, 0x0800, CRC(2145f8ab) SHA1(ddf63208559a3a08d4e88327c55426b0eed27654))
	ROM_RELOAD( 0xf000, 0x0800)
	ROM_LOAD("prom1_6.474", 0x6000, 0x0200, CRC(af6eb0b9) SHA1(28f8366737e09ffd60cb5ea55a5734143cdb9663))
	ROM_RELOAD( 0xe000, 0x0200)
	ROM_LOAD("prom2.474", 0x6200, 0x0200, CRC(f75ade1a) SHA1(a5572c5c721dbcb82988b709f4ef2119118e37c2))
	ROM_RELOAD( 0xe000, 0x0200)
	ROM_LOAD("prom3.474", 0x6400, 0x0200, CRC(242ec687) SHA1(c3366c898a66c78034687e6a6000193d52be4141))
	ROM_RELOAD( 0xe000, 0x0200)
	ROM_REGION(0x10000, "cpu2", 0)
	ROM_LOAD("sound3.716", 0x7800, 0x0800, CRC(55a10d13) SHA1(521d4cdfb0ed8178b3594cedceae93b772a951a4))
	ROM_RELOAD( 0xf800, 0x0800)
	ROM_LOAD("v_ic7.532", 0x3000, 0x1000, CRC(94c5c0a7) SHA1(ff7c618d1666c1d5c3319fdd72c1af2846659290))
	ROM_RELOAD( 0xb000, 0x1000)
	ROM_LOAD("v_ic5.532", 0x4000, 0x1000, CRC(1737fdd2) SHA1(6307e0ae715e97294ee8aaaeb2e2bebb0cb590c2))
	ROM_RELOAD( 0xc000, 0x1000)
	ROM_LOAD("v_ic6.532", 0x5000, 0x1000, CRC(e56f7aa2) SHA1(cb922c3f4d91285dda4ccae880c2d798a82fd51b))
	ROM_RELOAD( 0xd000, 0x1000)
ROM_END

ROM_START(frpwr_l2)
	ROM_REGION(0x10000, "maincpu", 0)
	ROM_LOAD("gamerom.716", 0x6800, 0x0800, CRC(fdd3b983) SHA1(fb5d1eb01589311cf4b2ef16e25db03d40bca2f7))
	ROM_RELOAD( 0xe800, 0x0800)
	ROM_LOAD("green2.716", 0x7800, 0x0800, CRC(1c978a4a) SHA1(1959184764643d58f1740c54bb74c2aad7d667d2))
	ROM_RELOAD( 0xf800, 0x0800)
	ROM_LOAD("green1.716", 0x7000, 0x0800, CRC(2145f8ab) SHA1(ddf63208559a3a08d4e88327c55426b0eed27654))
	ROM_RELOAD( 0xf000, 0x0800)
	ROM_LOAD("prom1.474", 0x6000, 0x0200, CRC(fbb7299f) SHA1(0ae9dbdc6ed8315596bf755ece34691671dc8d44))
	ROM_RELOAD( 0xe000, 0x0200)
	ROM_LOAD("prom2.474", 0x6200, 0x0200, CRC(f75ade1a) SHA1(a5572c5c721dbcb82988b709f4ef2119118e37c2))
	ROM_RELOAD( 0xe000, 0x0200)
	ROM_LOAD("prom3.474", 0x6400, 0x0200, CRC(242ec687) SHA1(c3366c898a66c78034687e6a6000193d52be4141))
	ROM_RELOAD( 0xe000, 0x0200)
	ROM_REGION(0x10000, "cpu2", 0)
	ROM_LOAD("sound3.716", 0x7800, 0x0800, CRC(55a10d13) SHA1(521d4cdfb0ed8178b3594cedceae93b772a951a4))
	ROM_RELOAD( 0xf800, 0x0800)
	ROM_LOAD("v_ic7.532", 0x3000, 0x1000, CRC(94c5c0a7) SHA1(ff7c618d1666c1d5c3319fdd72c1af2846659290))
	ROM_RELOAD( 0xb000, 0x1000)
	ROM_LOAD("v_ic5.532", 0x4000, 0x1000, CRC(1737fdd2) SHA1(6307e0ae715e97294ee8aaaeb2e2bebb0cb590c2))
	ROM_RELOAD( 0xc000, 0x1000)
	ROM_LOAD("v_ic6.532", 0x5000, 0x1000, CRC(e56f7aa2) SHA1(cb922c3f4d91285dda4ccae880c2d798a82fd51b))
	ROM_RELOAD( 0xd000, 0x1000)
ROM_END

/*--------------------------
/ Gorgar - Sys.6 (Game #496)
/-------------------------*/
ROM_START(grgar_l1)
	ROM_REGION(0x10000, "maincpu", 0)
	ROM_LOAD("gamerom.716", 0x6000, 0x0800, CRC(1c6f3e48) SHA1(ba5536e6fbcaf3709277fe27827d7f75c1889ba3))
	ROM_RELOAD( 0xe000, 0x0800)
	ROM_LOAD("green2.716", 0x7800, 0x0800, CRC(1c978a4a) SHA1(1959184764643d58f1740c54bb74c2aad7d667d2))
	ROM_RELOAD( 0xf800, 0x0800)
	ROM_LOAD("green1.716", 0x7000, 0x0800, CRC(2145f8ab) SHA1(ddf63208559a3a08d4e88327c55426b0eed27654))
	ROM_RELOAD( 0xf000, 0x0800)
	ROM_REGION(0x10000, "cpu2", 0)
	ROM_LOAD("sound2.716", 0x7800, 0x0800, CRC(c9103a68) SHA1(cc77af54fdb192f0b334d9d1028210618c3f1d95))
	ROM_RELOAD( 0xf800, 0x0800)
	ROM_LOAD("v_ic7.532", 0x3000, 0x1000, CRC(0b1879e3) SHA1(2c34a815f598b4413e9229e8eb1322ec9e7cc9d6))
	ROM_RELOAD( 0xb000, 0x1000)
	ROM_LOAD("v_ic5.532", 0x4000, 0x1000, CRC(0ceaef37) SHA1(33b5f5286b8588162d56dbc5c9a8ccb70d3b9090))
	ROM_RELOAD( 0xc000, 0x1000)
	ROM_LOAD("v_ic6.532", 0x5000, 0x1000, CRC(218290b9) SHA1(6afeff1413895489e92a4bb1c05f6de5773dbb6a))
	ROM_RELOAD( 0xd000, 0x1000)
ROM_END

ROM_START(grgar_t1)
	ROM_REGION(0x10000, "maincpu", 0)
	ROM_LOAD("gamerom.716", 0x6000, 0x0800, CRC(1c6f3e48) SHA1(ba5536e6fbcaf3709277fe27827d7f75c1889ba3))
	ROM_RELOAD( 0xe000, 0x0800)
	ROM_LOAD("green2a.716", 0x7800, 0x0800, CRC(16621eec) SHA1(14e1cf5f7227860a3219b2b79fa66dcf252dce98))
	ROM_RELOAD( 0xf800, 0x0800)
	ROM_LOAD("green1.716", 0x7000, 0x0800, CRC(2145f8ab) SHA1(ddf63208559a3a08d4e88327c55426b0eed27654))
	ROM_RELOAD( 0xf000, 0x0800)
	ROM_REGION(0x10000, "cpu2", 0)
	ROM_LOAD("sound2.716", 0x7800, 0x0800, CRC(c9103a68) SHA1(cc77af54fdb192f0b334d9d1028210618c3f1d95))
	ROM_RELOAD( 0xf800, 0x0800)
	ROM_LOAD("v_ic7.532", 0x3000, 0x1000, CRC(0b1879e3) SHA1(2c34a815f598b4413e9229e8eb1322ec9e7cc9d6))
	ROM_RELOAD( 0xb000, 0x1000)
	ROM_LOAD("v_ic5.532", 0x4000, 0x1000, CRC(0ceaef37) SHA1(33b5f5286b8588162d56dbc5c9a8ccb70d3b9090))
	ROM_RELOAD( 0xc000, 0x1000)
	ROM_LOAD("v_ic6.532", 0x5000, 0x1000, CRC(218290b9) SHA1(6afeff1413895489e92a4bb1c05f6de5773dbb6a))
	ROM_RELOAD( 0xd000, 0x1000)
ROM_END

/*--------------------------------
/ Laser Ball - Sys.6 (Game #493)
/-------------------------------*/
ROM_START(lzbal_l2)
	ROM_REGION(0x10000, "maincpu", 0)
	ROM_LOAD("gamerom.716", 0x6000, 0x0800, CRC(9c5ffe2f) SHA1(f0db627abaeb8c023a3ccc75262e236c998a5d6f))
	ROM_RELOAD( 0xe000, 0x0800)
	ROM_LOAD("green2.716", 0x7800, 0x0800, CRC(1c978a4a) SHA1(1959184764643d58f1740c54bb74c2aad7d667d2))
	ROM_RELOAD( 0xf800, 0x0800)
	ROM_LOAD("green1.716", 0x7000, 0x0800, CRC(2145f8ab) SHA1(ddf63208559a3a08d4e88327c55426b0eed27654))
	ROM_RELOAD( 0xf000, 0x0800)
	ROM_REGION(0x10000, "cpu2", 0)
	ROM_LOAD("sound2.716", 0x7800, 0x0800, CRC(c9103a68) SHA1(cc77af54fdb192f0b334d9d1028210618c3f1d95))
	ROM_RELOAD( 0xf800, 0x0800)
ROM_END

ROM_START(lzbal_t2)
	ROM_REGION(0x10000, "maincpu", 0)
	ROM_LOAD("gamerom.716", 0x6000, 0x0800, CRC(9c5ffe2f) SHA1(f0db627abaeb8c023a3ccc75262e236c998a5d6f))
	ROM_RELOAD( 0xe000, 0x0800)
	ROM_LOAD("green2a.716", 0x7800, 0x0800, CRC(16621eec) SHA1(14e1cf5f7227860a3219b2b79fa66dcf252dce98))
	ROM_RELOAD( 0xf800, 0x0800)
	ROM_LOAD("green1.716", 0x7000, 0x0800, CRC(2145f8ab) SHA1(ddf63208559a3a08d4e88327c55426b0eed27654))
	ROM_RELOAD( 0xf000, 0x0800)
	ROM_REGION(0x10000, "cpu2", 0)
	ROM_LOAD("sound2.716", 0x7800, 0x0800, CRC(c9103a68) SHA1(cc77af54fdb192f0b334d9d1028210618c3f1d95))
	ROM_RELOAD( 0xf800, 0x0800)
ROM_END

/*--------------------------------
/ Time Warp - Sys.4 (Game #489)
/-------------------------------*/
ROM_START(tmwrp_l2)
	ROM_REGION(0x10000, "maincpu", 0)
	ROM_LOAD("gamerom.716", 0x6000, 0x0800, CRC(b168df09) SHA1(d4c97714636ce51be2e5f8cc5af89e10a2f82ac7))
	ROM_RELOAD( 0xe000, 0x0800)
	ROM_LOAD("green2.716", 0x7800, 0x0800, CRC(1c978a4a) SHA1(1959184764643d58f1740c54bb74c2aad7d667d2))
	ROM_RELOAD( 0xf800, 0x0800)
	ROM_LOAD("green1.716", 0x7000, 0x0800, CRC(2145f8ab) SHA1(ddf63208559a3a08d4e88327c55426b0eed27654))
	ROM_RELOAD( 0xf000, 0x0800)
	ROM_REGION(0x10000, "cpu2", 0)
	ROM_LOAD("sound1.716", 0x7800, 0x0800, CRC(f4190ca3) SHA1(ee234fb5c894fca5876ee6dc7ea8e89e7e0aec9c))
	ROM_RELOAD( 0xf800, 0x0800)
ROM_END

ROM_START(tmwrp_t2)
	ROM_REGION(0x10000, "maincpu", 0)
	ROM_LOAD("gamerom.716", 0x6000, 0x0800, CRC(b168df09) SHA1(d4c97714636ce51be2e5f8cc5af89e10a2f82ac7))
	ROM_RELOAD( 0xe000, 0x0800)
	ROM_LOAD("green2a.716", 0x7800, 0x0800, CRC(16621eec) SHA1(14e1cf5f7227860a3219b2b79fa66dcf252dce98))
	ROM_RELOAD( 0xf800, 0x0800)
	ROM_LOAD("green1.716", 0x7000, 0x0800, CRC(2145f8ab) SHA1(ddf63208559a3a08d4e88327c55426b0eed27654))
	ROM_RELOAD( 0xf000, 0x0800)
	ROM_REGION(0x10000, "cpu2", 0)
	ROM_LOAD("sound1.716", 0x7800, 0x0800, CRC(f4190ca3) SHA1(ee234fb5c894fca5876ee6dc7ea8e89e7e0aec9c))
	ROM_RELOAD( 0xf800, 0x0800)
ROM_END

/*-----------------------------
/ Scorpion - Sys.6 (Game #494)
/----------------------------*/
ROM_START(scrpn_l1)
	ROM_REGION(0x10000, "maincpu", 0)
	ROM_LOAD("gamerom.716", 0x6000, 0x0800, CRC(881109a9) SHA1(53d4275c76b47b68a74209fe660d943a51e90eca))
	ROM_RELOAD( 0xe000, 0x0800)
	ROM_LOAD("green2.716", 0x7800, 0x0800, CRC(1c978a4a) SHA1(1959184764643d58f1740c54bb74c2aad7d667d2))
	ROM_RELOAD( 0xf800, 0x0800)
	ROM_LOAD("green1.716", 0x7000, 0x0800, CRC(2145f8ab) SHA1(ddf63208559a3a08d4e88327c55426b0eed27654))
	ROM_RELOAD( 0xf000, 0x0800)
	ROM_REGION(0x10000, "cpu2", 0)
	ROM_LOAD("sound1.716", 0x7800, 0x0800, CRC(f4190ca3) SHA1(ee234fb5c894fca5876ee6dc7ea8e89e7e0aec9c))
	ROM_RELOAD( 0xf800, 0x0800)
ROM_END

ROM_START(scrpn_t1)
	ROM_REGION(0x10000, "maincpu", 0)
	ROM_LOAD("gamerom.716", 0x6000, 0x0800, CRC(881109a9) SHA1(53d4275c76b47b68a74209fe660d943a51e90eca))
	ROM_RELOAD( 0xe000, 0x0800)
	ROM_LOAD("green2a.716", 0x7800, 0x0800, CRC(16621eec) SHA1(14e1cf5f7227860a3219b2b79fa66dcf252dce98))
	ROM_RELOAD( 0xf800, 0x0800)
	ROM_LOAD("green1.716", 0x7000, 0x0800, CRC(2145f8ab) SHA1(ddf63208559a3a08d4e88327c55426b0eed27654))
	ROM_RELOAD( 0xf000, 0x0800)
	ROM_REGION(0x10000, "cpu2", 0)
	ROM_LOAD("sound1.716", 0x7800, 0x0800, CRC(f4190ca3) SHA1(ee234fb5c894fca5876ee6dc7ea8e89e7e0aec9c))
	ROM_RELOAD( 0xf800, 0x0800)
ROM_END

/*--------------------------------
/ Tri Zone - Sys.4 (Game #487)
/-------------------------------*/
ROM_START(trizn_l1)
	ROM_REGION(0x10000, "maincpu", 0)
	ROM_LOAD("gamerom.716", 0x6000, 0x0800, CRC(757091c5) SHA1(00dac6c19b08d2528ea293619c4a39499a1a02c2))
	ROM_RELOAD( 0xe000, 0x0800)
	ROM_LOAD("green2.716", 0x7800, 0x0800, CRC(1c978a4a) SHA1(1959184764643d58f1740c54bb74c2aad7d667d2))
	ROM_RELOAD( 0xf800, 0x0800)
	ROM_LOAD("green1.716", 0x7000, 0x0800, CRC(2145f8ab) SHA1(ddf63208559a3a08d4e88327c55426b0eed27654))
	ROM_RELOAD( 0xf000, 0x0800)
	ROM_REGION(0x10000, "cpu2", 0)
	ROM_LOAD("sound1.716", 0x7800, 0x0800, CRC(f4190ca3) SHA1(ee234fb5c894fca5876ee6dc7ea8e89e7e0aec9c))
	ROM_RELOAD( 0xf800, 0x0800)
ROM_END

ROM_START(trizn_t1)
	ROM_REGION(0x10000, "maincpu", 0)
	ROM_LOAD("gamerom.716", 0x6000, 0x0800, CRC(757091c5) SHA1(00dac6c19b08d2528ea293619c4a39499a1a02c2))
	ROM_RELOAD( 0xe000, 0x0800)
	ROM_LOAD("green2a.716", 0x7800, 0x0800, CRC(16621eec) SHA1(14e1cf5f7227860a3219b2b79fa66dcf252dce98))
	ROM_RELOAD( 0xf800, 0x0800)
	ROM_LOAD("green1.716", 0x7000, 0x0800, CRC(2145f8ab) SHA1(ddf63208559a3a08d4e88327c55426b0eed27654))
	ROM_RELOAD( 0xf000, 0x0800)
	ROM_REGION(0x10000, "cpu2", 0)
	ROM_LOAD("sound1.716", 0x7800, 0x0800, CRC(f4190ca3) SHA1(ee234fb5c894fca5876ee6dc7ea8e89e7e0aec9c))
	ROM_RELOAD( 0xf800, 0x0800)
ROM_END


GAME(1979,	blkou_l1,		0,			williams_s6,	williams_s6,	williams_s6,	ROT0,	"Williams",				"Blackout (L-1)",				GAME_IS_SKELETON_MECHANICAL)
GAME(1979,	blkou_t1,		blkou_l1,	williams_s6,	williams_s6,	williams_s6,	ROT0,	"Williams",				"Blackout (T-1)",				GAME_IS_SKELETON_MECHANICAL)
GAME(1979,	blkou_f1,		blkou_l1,	williams_s6,	williams_s6,	williams_s6,	ROT0,	"Williams",				"Blackout (L-1, French Speech)",				GAME_IS_SKELETON_MECHANICAL)
GAME(1980,	frpwr_l6,		0,			williams_s6,	williams_s6,	williams_s6,	ROT0,	"Williams",				"Firepower (L-6)",				GAME_IS_SKELETON_MECHANICAL)
GAME(1980,	frpwr_t6,		frpwr_l6,	williams_s6,	williams_s6,	williams_s6,	ROT0,	"Williams",				"Firepower (T-6)",				GAME_IS_SKELETON_MECHANICAL)
GAME(1980,	frpwr_l2,		frpwr_l6,	williams_s6,	williams_s6,	williams_s6,	ROT0,	"Williams",				"Firepower (L-2)",				GAME_IS_SKELETON_MECHANICAL)
GAME(1979,	grgar_l1,		0,			williams_s6,	williams_s6,	williams_s6,	ROT0,	"Williams",				"Gorgar (L-1)",					GAME_IS_SKELETON_MECHANICAL)
GAME(1979,	grgar_t1,		grgar_l1,	williams_s6,	williams_s6,	williams_s6,	ROT0,	"Williams",				"Gorgar (T-1)",					GAME_IS_SKELETON_MECHANICAL)
GAME(1979,	lzbal_l2,		0,			williams_s6,	williams_s6,	williams_s6,	ROT0,	"Williams",				"Laser Ball (L-2)",				GAME_IS_SKELETON_MECHANICAL)
GAME(1980,	lzbal_t2,		lzbal_l2,	williams_s6,	williams_s6,	williams_s6,	ROT0,	"Williams",				"Laser Ball (T-2)",				GAME_IS_SKELETON_MECHANICAL)
GAME(1979,	tmwrp_l2,		0,			williams_s6,	williams_s6,	williams_s6,	ROT0,	"Williams",				"Time Warp (L-2)",				GAME_IS_SKELETON_MECHANICAL)
GAME(1979,	tmwrp_t2,		tmwrp_l2,	williams_s6,	williams_s6,	williams_s6,	ROT0,	"Williams",				"Time Warp (T-2)",				GAME_IS_SKELETON_MECHANICAL)
GAME(1980,	scrpn_l1,		0,			williams_s6,	williams_s6,	williams_s6,	ROT0,	"Williams",				"Scorpion (L-1)",				GAME_IS_SKELETON_MECHANICAL)
GAME(1980,	scrpn_t1,		scrpn_l1,	williams_s6,	williams_s6,	williams_s6,	ROT0,	"Williams",				"Scorpion (T-1)",				GAME_IS_SKELETON_MECHANICAL)
GAME(1978,	trizn_l1,		0,			williams_s6,	williams_s6,	williams_s6,	ROT0,	"Williams",				"Tri Zone (L-1)",				GAME_IS_SKELETON_MECHANICAL)
GAME(1978,	trizn_t1,		trizn_l1,	williams_s6,	williams_s6,	williams_s6,	ROT0,	"Williams",				"Tri Zone (T-1)",				GAME_IS_SKELETON_MECHANICAL)
