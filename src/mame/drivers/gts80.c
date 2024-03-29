/*

    Gottlieb System 80

*/

#define ADDRESS_MAP_MODERN

#include "emu.h"
#include "cpu/m6502/m6502.h"

class gts80_state : public driver_device
{
public:
	gts80_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag),
		  m_maincpu(*this, "maincpu")
	{ }

protected:

	// devices
	required_device<cpu_device> m_maincpu;

	// driver_device overrides
	virtual void machine_reset();
};

static ADDRESS_MAP_START( gts80_map, AS_PROGRAM, 8, gts80_state )
	AM_RANGE(0x0000, 0xffff) AM_NOP
	AM_RANGE(0x1000, 0x17ff) AM_MIRROR(0xc000) AM_ROM
	AM_RANGE(0x2000, 0x2fff) AM_MIRROR(0xc000) AM_ROM
	AM_RANGE(0x3000, 0x3fff) AM_MIRROR(0xc000) AM_ROM
ADDRESS_MAP_END


static INPUT_PORTS_START( gts80 )
INPUT_PORTS_END

void gts80_state::machine_reset()
{
}

static DRIVER_INIT( gts80 )
{
}

/* with Sound Board */
static MACHINE_CONFIG_START( gts80_s, gts80_state )
	/* basic machine hardware */
	MCFG_CPU_ADD("maincpu", M6502, 850000)
	MCFG_CPU_PROGRAM_MAP(gts80_map)

	/* related to src/mame/audio/gottlieb.c */
//  MCFG_IMPORT_FROM(gts80s_s)
MACHINE_CONFIG_END

/* with Sound & Speech Board */
/* Note: hh uses this but it does not have the Votrax chip (?) */
static MACHINE_CONFIG_START( gts80_ss, gts80_state )
	/* basic machine hardware */
	MCFG_CPU_ADD("maincpu", M6502, 850000)
	MCFG_CPU_PROGRAM_MAP(gts80_map)

	/* related to src/mame/audio/gottlieb.c */
//  MCFG_IMPORT_FROM(gts80s_ss)
MACHINE_CONFIG_END

/*-------------------------------------------------------------------
/ Black Hole
/-------------------------------------------------------------------*/
ROM_START(blckhole)
	ROM_REGION(0x10000, "maincpu", 0)
	ROM_LOAD("u2_80.bin", 0x2000, 0x1000, CRC(4f0bc7b1) SHA1(612cbacdca5cfa6ad23940796df3b7c385be79fe))
	ROM_RELOAD(0x6000, 0x1000)
	ROM_RELOAD(0xa000, 0x1000)
	ROM_RELOAD(0xe000, 0x1000)
	ROM_LOAD("u3_80.bin", 0x3000, 0x1000, CRC(1e69f9d0) SHA1(ad738cac2555830257b531e5e533b15362f624b9))
	ROM_RELOAD(0x7000, 0x1000)
	ROM_RELOAD(0xb000, 0x1000)
	ROM_RELOAD(0xf000, 0x1000)
	ROM_LOAD("668-4.cpu", 0x1000, 0x0800, CRC(01b53045) SHA1(72d73bbb09358b331696cd1cc44fc4958feffbe2))
	ROM_RELOAD(0x5000, 0x0800)
	ROM_RELOAD(0x9000, 0x0800)
	ROM_RELOAD(0xd000, 0x0800)

	ROM_REGION(0x10000, "cpu2", 0)
	ROM_LOAD("668-s1.snd", 0x7000, 0x0800, CRC(23d5045d) SHA1(a20bf02ece97e8238d1dbe8d35ca63d82b62431e))
	ROM_LOAD("668-s2.snd", 0x7800, 0x0800, CRC(d63da498) SHA1(84dd87783f47fbf64b1830284c168501f9b455e2))
ROM_END

ROM_START(blckhole2)
	ROM_REGION(0x10000, "maincpu", 0)
	ROM_LOAD("u2_80.bin", 0x2000, 0x1000, CRC(4f0bc7b1) SHA1(612cbacdca5cfa6ad23940796df3b7c385be79fe))
	ROM_RELOAD(0x6000, 0x1000)
	ROM_RELOAD(0xa000, 0x1000)
	ROM_RELOAD(0xe000, 0x1000)
	ROM_LOAD("u3_80.bin", 0x3000, 0x1000, CRC(1e69f9d0) SHA1(ad738cac2555830257b531e5e533b15362f624b9))
	ROM_RELOAD(0x7000, 0x1000)
	ROM_RELOAD(0xb000, 0x1000)
	ROM_RELOAD(0xf000, 0x1000)
	ROM_LOAD("668-2.cpu", 0x1000, 0x0800, CRC(df03ffea) SHA1(7ca8fc321f74b9193104c282c7b4b92af93694c9))
	ROM_RELOAD(0x5000, 0x0800)
	ROM_RELOAD(0x9000, 0x0800)
	ROM_RELOAD(0xd000, 0x0800)

	ROM_REGION(0x10000, "cpu2", 0)
	ROM_LOAD("668-s1.snd", 0x7000, 0x0800, CRC(23d5045d) SHA1(a20bf02ece97e8238d1dbe8d35ca63d82b62431e))
	ROM_LOAD("668-s2.snd", 0x7800, 0x0800, CRC(d63da498) SHA1(84dd87783f47fbf64b1830284c168501f9b455e2))
ROM_END

ROM_START(blckhols)
	ROM_REGION(0x10000, "maincpu", 0)
	ROM_LOAD("u2_80.bin", 0x2000, 0x1000, CRC(4f0bc7b1) SHA1(612cbacdca5cfa6ad23940796df3b7c385be79fe))
	ROM_RELOAD(0x6000, 0x1000)
	ROM_RELOAD(0xa000, 0x1000)
	ROM_RELOAD(0xe000, 0x1000)
	ROM_LOAD("u3_80.bin", 0x3000, 0x1000, CRC(1e69f9d0) SHA1(ad738cac2555830257b531e5e533b15362f624b9))
	ROM_RELOAD(0x7000, 0x1000)
	ROM_RELOAD(0xb000, 0x1000)
	ROM_RELOAD(0xf000, 0x1000)
	ROM_LOAD("668-a2.cpu", 0x1000, 0x0800, CRC(df56f896) SHA1(1ec945a7ed8d25064476791adab2b554371dadbe))
	ROM_RELOAD(0x5000, 0x0800)
	ROM_RELOAD(0x9000, 0x0800)
	ROM_RELOAD(0xd000, 0x0800)

	ROM_REGION(0x10000, "cpu2", 0)
	ROM_LOAD("668-a-s.snd", 0x0400, 0x0400, CRC(5175f307) SHA1(97be8f2bbc393cc45a07fa43daec4bbba2336af8))
	ROM_RELOAD( 0x0800, 0x0400)
	ROM_LOAD("6530sy80.bin", 0x0c00, 0x0400, CRC(c8ba951d) SHA1(e4aa152b36695a0205c19a8914e4d77373f64c6c))
	ROM_RELOAD( 0xfc00, 0x0400)
ROM_END

/*-------------------------------------------------------------------
/ Circus
/-------------------------------------------------------------------*/
ROM_START(circusp)
	ROM_REGION(0x10000, "maincpu", 0)
	ROM_LOAD("u2_80.bin", 0x2000, 0x1000, CRC(4f0bc7b1) SHA1(612cbacdca5cfa6ad23940796df3b7c385be79fe))
	ROM_RELOAD(0x6000, 0x1000)
	ROM_RELOAD(0xa000, 0x1000)
	ROM_RELOAD(0xe000, 0x1000)
	ROM_LOAD("u3_80.bin", 0x3000, 0x1000, CRC(1e69f9d0) SHA1(ad738cac2555830257b531e5e533b15362f624b9))
	ROM_RELOAD(0x7000, 0x1000)
	ROM_RELOAD(0xb000, 0x1000)
	ROM_RELOAD(0xf000, 0x1000)
	ROM_LOAD("654-1.cpu", 0x1000, 0x0200, CRC(0eeb2731) SHA1(087cd6400bf0775bda0264422b3f790a77852bc4))
	ROM_RELOAD(0x1400, 0x0200)
	ROM_RELOAD(0x5000, 0x0200)
	ROM_RELOAD(0x5400, 0x0200)
	ROM_RELOAD(0x9000, 0x0200)
	ROM_RELOAD(0x9400, 0x0200)
	ROM_RELOAD(0xd000, 0x0200)
	ROM_RELOAD(0xd400, 0x0200)
	ROM_LOAD("654-2.cpu", 0x1200, 0x0200, CRC(01e23569) SHA1(47088421254e487aa1d1e87ea911dc1634e7d9ad))
	ROM_RELOAD(0x1600, 0x0200)
	ROM_RELOAD(0x5000, 0x0200)
	ROM_RELOAD(0x5600, 0x0200)
	ROM_RELOAD(0x9000, 0x0200)
	ROM_RELOAD(0x9600, 0x0200)
	ROM_RELOAD(0xd000, 0x0200)
	ROM_RELOAD(0xd600, 0x0200)

	ROM_REGION(0x10000, "cpu2", 0)
	ROM_LOAD("654.snd", 0x0400, 0x0400, CRC(75c3ad67) SHA1(4f59c451b8659d964d5242728814c2d97f68445b))
	ROM_RELOAD( 0x0800, 0x0400)
	ROM_LOAD("6530sy80.bin", 0x0c00, 0x0400, CRC(c8ba951d) SHA1(e4aa152b36695a0205c19a8914e4d77373f64c6c))
	ROM_RELOAD( 0xfc00, 0x0400)
ROM_END

/*-------------------------------------------------------------------
/ Counterforce
/-------------------------------------------------------------------*/
ROM_START(cntforce)
	ROM_REGION(0x10000, "maincpu", 0)
	ROM_LOAD("u2_80.bin", 0x2000, 0x1000, CRC(4f0bc7b1) SHA1(612cbacdca5cfa6ad23940796df3b7c385be79fe))
	ROM_RELOAD(0x6000, 0x1000)
	ROM_RELOAD(0xa000, 0x1000)
	ROM_RELOAD(0xe000, 0x1000)
	ROM_LOAD("u3_80.bin", 0x3000, 0x1000, CRC(1e69f9d0) SHA1(ad738cac2555830257b531e5e533b15362f624b9))
	ROM_RELOAD(0x7000, 0x1000)
	ROM_RELOAD(0xb000, 0x1000)
	ROM_RELOAD(0xf000, 0x1000)
	ROM_LOAD("656-1.cpu", 0x1000, 0x0200, CRC(42baf51d) SHA1(6c7947df6e4d7ed2fd48410705018bde91db3356))
	ROM_RELOAD(0x1400, 0x0200)
	ROM_RELOAD(0x5000, 0x0200)
	ROM_RELOAD(0x5400, 0x0200)
	ROM_RELOAD(0x9000, 0x0200)
	ROM_RELOAD(0x9400, 0x0200)
	ROM_RELOAD(0xd000, 0x0200)
	ROM_RELOAD(0xd400, 0x0200)
	ROM_LOAD("656-2.cpu", 0x1200, 0x0200, CRC(0e185c30) SHA1(01d9fb5d335c24bed9f747d6e23f57adb6ef09a5))
	ROM_RELOAD(0x1600, 0x0200)
	ROM_RELOAD(0x5000, 0x0200)
	ROM_RELOAD(0x5600, 0x0200)
	ROM_RELOAD(0x9000, 0x0200)
	ROM_RELOAD(0x9600, 0x0200)
	ROM_RELOAD(0xd000, 0x0200)
	ROM_RELOAD(0xd600, 0x0200)

	ROM_REGION(0x10000, "cpu2", 0)
	ROM_LOAD("656.snd", 0x0400, 0x0400, CRC(0be2cbe9) SHA1(306a3e7d93733562360285de35b331b5daae7250))
	ROM_RELOAD( 0x0800, 0x0400)
	ROM_LOAD("6530sy80.bin", 0x0c00, 0x0400, CRC(c8ba951d) SHA1(e4aa152b36695a0205c19a8914e4d77373f64c6c))
	ROM_RELOAD( 0xfc00, 0x0400)
ROM_END

/*-------------------------------------------------------------------
/ Critical Mass
/-------------------------------------------------------------------*/

/*-------------------------------------------------------------------
/ Eclipse
/-------------------------------------------------------------------*/
ROM_START(eclipse)
	ROM_REGION(0x10000, "maincpu", 0)
	ROM_LOAD("u2_80.bin", 0x2000, 0x1000, CRC(4f0bc7b1) SHA1(612cbacdca5cfa6ad23940796df3b7c385be79fe))
	ROM_RELOAD(0x6000, 0x1000)
	ROM_RELOAD(0xa000, 0x1000)
	ROM_RELOAD(0xe000, 0x1000)
	ROM_LOAD("u3_80.bin", 0x3000, 0x1000, CRC(1e69f9d0) SHA1(ad738cac2555830257b531e5e533b15362f624b9))
	ROM_RELOAD(0x7000, 0x1000)
	ROM_RELOAD(0xb000, 0x1000)
	ROM_RELOAD(0xf000, 0x1000)
	ROM_LOAD("671-a.cpu", 0x1000, 0x0800, CRC(efad7312) SHA1(fcfd5e5c7924d65ac42561994797156a80018667))
	ROM_RELOAD(0x5000, 0x0800)
	ROM_RELOAD(0x9000, 0x0800)
	ROM_RELOAD(0xd000, 0x0800)

	ROM_REGION(0x10000, "cpu2", 0)
	ROM_LOAD("671-a-s.snd", 0x0400, 0x0400, CRC(5175f307) SHA1(97be8f2bbc393cc45a07fa43daec4bbba2336af8))
	ROM_RELOAD( 0x0800, 0x0400)
	ROM_LOAD("6530sy80.bin", 0x0c00, 0x0400, CRC(c8ba951d) SHA1(e4aa152b36695a0205c19a8914e4d77373f64c6c))
	ROM_RELOAD( 0xfc00, 0x0400)
ROM_END

/*-------------------------------------------------------------------
/ Force II
/-------------------------------------------------------------------*/
ROM_START(forceii)
	ROM_REGION(0x10000, "maincpu", 0)
	ROM_LOAD("u2_80.bin", 0x2000, 0x1000, CRC(4f0bc7b1) SHA1(612cbacdca5cfa6ad23940796df3b7c385be79fe))
	ROM_RELOAD(0x6000, 0x1000)
	ROM_RELOAD(0xa000, 0x1000)
	ROM_RELOAD(0xe000, 0x1000)
	ROM_LOAD("u3_80.bin", 0x3000, 0x1000, CRC(1e69f9d0) SHA1(ad738cac2555830257b531e5e533b15362f624b9))
	ROM_RELOAD(0x7000, 0x1000)
	ROM_RELOAD(0xb000, 0x1000)
	ROM_RELOAD(0xf000, 0x1000)
	ROM_LOAD("661-2.cpu", 0x1000, 0x0800, CRC(a4fa42a4) SHA1(c17af4f0da6d5630e43db44655bece0e26b0112a))
	ROM_RELOAD(0x5000, 0x0800)
	ROM_RELOAD(0x9000, 0x0800)
	ROM_RELOAD(0xd000, 0x0800)

	ROM_REGION(0x10000, "cpu2", 0)
	ROM_LOAD("661.snd", 0x0400, 0x0400, CRC(650158a7) SHA1(c7a9d521d1e7de1e00e7abc3a97aaaee04f8052e))
	ROM_RELOAD( 0x0800, 0x0400)
	ROM_LOAD("6530sy80.bin", 0x0c00, 0x0400, CRC(c8ba951d) SHA1(e4aa152b36695a0205c19a8914e4d77373f64c6c))
	ROM_RELOAD( 0xfc00, 0x0400)
ROM_END

/*-------------------------------------------------------------------
/ Haunted House, since serial no. 5000
/-------------------------------------------------------------------*/
ROM_START(hh)
	ROM_REGION(0x10000, "maincpu", 0)
	ROM_LOAD("u2_80.bin", 0x2000, 0x1000, CRC(4f0bc7b1) SHA1(612cbacdca5cfa6ad23940796df3b7c385be79fe))
	ROM_RELOAD(0x6000, 0x1000)
	ROM_RELOAD(0xa000, 0x1000)
	ROM_RELOAD(0xe000, 0x1000)
	ROM_LOAD("u3_80.bin", 0x3000, 0x1000, CRC(1e69f9d0) SHA1(ad738cac2555830257b531e5e533b15362f624b9))
	ROM_RELOAD(0x7000, 0x1000)
	ROM_RELOAD(0xb000, 0x1000)
	ROM_RELOAD(0xf000, 0x1000)
	ROM_LOAD("669-2.cpu", 0x1000, 0x0800, CRC(f3085f77) SHA1(ebd43588401a735d9c941d06d67ac90183139e90))
	ROM_RELOAD(0x5000, 0x0800)
	ROM_RELOAD(0x9000, 0x0800)
	ROM_RELOAD(0xd000, 0x0800)

	ROM_REGION(0x10000, "cpu2", 0)
	ROM_LOAD("669-s1.snd", 0x7000, 0x0800, CRC(52ec7335) SHA1(2b08dd8a89057c9c8c184d5b723ecad01572129f))
	ROM_LOAD("669-s2.snd", 0x7800, 0x0800, CRC(a3317b4b) SHA1(c3b14aa58fd4588c8b8fa3540ea6331a9ee40f1f))
ROM_END

ROM_START(hh_1)
	ROM_REGION(0x10000, "maincpu", 0)
	ROM_LOAD("u2_80.bin", 0x2000, 0x1000, CRC(4f0bc7b1) SHA1(612cbacdca5cfa6ad23940796df3b7c385be79fe))
	ROM_RELOAD(0x6000, 0x1000)
	ROM_RELOAD(0xa000, 0x1000)
	ROM_RELOAD(0xe000, 0x1000)
	ROM_LOAD("u3_80.bin", 0x3000, 0x1000, CRC(1e69f9d0) SHA1(ad738cac2555830257b531e5e533b15362f624b9))
	ROM_RELOAD(0x7000, 0x1000)
	ROM_RELOAD(0xb000, 0x1000)
	ROM_RELOAD(0xf000, 0x1000)
	ROM_LOAD("669-1.cpu", 0x1000, 0x0800, CRC(96e72b93) SHA1(3eb3d3e064ba2fe637bba2a93ffd07f00edfa0f2))
	ROM_RELOAD(0x5000, 0x0800)
	ROM_RELOAD(0x9000, 0x0800)
	ROM_RELOAD(0xd000, 0x0800)

	ROM_REGION(0x10000, "cpu2", 0)
	ROM_LOAD("669-s1.snd", 0x7000, 0x0800, CRC(52ec7335) SHA1(2b08dd8a89057c9c8c184d5b723ecad01572129f))
	ROM_LOAD("669-s2.snd", 0x7800, 0x0800, CRC(a3317b4b) SHA1(c3b14aa58fd4588c8b8fa3540ea6331a9ee40f1f))
ROM_END

/*-------------------------------------------------------------------
/ James Bond
/-------------------------------------------------------------------*/
ROM_START(jamesb)
	ROM_REGION(0x10000, "maincpu", 0)
	ROM_LOAD("u2_80.bin", 0x2000, 0x1000, CRC(4f0bc7b1) SHA1(612cbacdca5cfa6ad23940796df3b7c385be79fe))
	ROM_RELOAD(0x6000, 0x1000)
	ROM_RELOAD(0xa000, 0x1000)
	ROM_RELOAD(0xe000, 0x1000)
	ROM_LOAD("u3_80.bin", 0x3000, 0x1000, CRC(1e69f9d0) SHA1(ad738cac2555830257b531e5e533b15362f624b9))
	ROM_RELOAD(0x7000, 0x1000)
	ROM_RELOAD(0xb000, 0x1000)
	ROM_RELOAD(0xf000, 0x1000)
	ROM_LOAD("658-1.cpu", 0x1000, 0x0800, CRC(b841ad7a) SHA1(3396e82351c975781cac9112bfa341a3b799f296))
	ROM_RELOAD(0x5000, 0x0800)
	ROM_RELOAD(0x9000, 0x0800)
	ROM_RELOAD(0xd000, 0x0800)

	ROM_REGION(0x10000, "cpu2", 0)
	ROM_LOAD("658.snd", 0x0400, 0x0400, CRC(962c03df) SHA1(e8ff5d502a038531a921380b75c27ef79b6feac8))
	ROM_RELOAD( 0x0800, 0x0400)
	ROM_LOAD("6530sy80.bin", 0x0c00, 0x0400, CRC(c8ba951d) SHA1(e4aa152b36695a0205c19a8914e4d77373f64c6c))
	ROM_RELOAD( 0xfc00, 0x0400)
ROM_END

ROM_START(jamesb2)
	ROM_REGION(0x10000, "maincpu", 0)
	ROM_LOAD("u2_80.bin", 0x2000, 0x1000, CRC(4f0bc7b1) SHA1(612cbacdca5cfa6ad23940796df3b7c385be79fe))
	ROM_RELOAD(0x6000, 0x1000)
	ROM_RELOAD(0xa000, 0x1000)
	ROM_RELOAD(0xe000, 0x1000)
	ROM_LOAD("u3_80.bin", 0x3000, 0x1000, CRC(1e69f9d0) SHA1(ad738cac2555830257b531e5e533b15362f624b9))
	ROM_RELOAD(0x7000, 0x1000)
	ROM_RELOAD(0xb000, 0x1000)
	ROM_RELOAD(0xf000, 0x1000)
	ROM_LOAD("658-x.cpu", 0x1000, 0x0800, CRC(e7e0febf) SHA1(2c101a88b61229f30ed15d38f395bc538999d766))
	ROM_RELOAD(0x5000, 0x0800)
	ROM_RELOAD(0x9000, 0x0800)
	ROM_RELOAD(0xd000, 0x0800)

	ROM_REGION(0x10000, "cpu2", 0)
	ROM_LOAD("658.snd", 0x0400, 0x0400, CRC(962c03df) SHA1(e8ff5d502a038531a921380b75c27ef79b6feac8))
	ROM_RELOAD( 0x0800, 0x0400)
	ROM_LOAD("6530sy80.bin", 0x0c00, 0x0400, CRC(c8ba951d) SHA1(e4aa152b36695a0205c19a8914e4d77373f64c6c))
	ROM_RELOAD( 0xfc00, 0x0400)
ROM_END

/*-------------------------------------------------------------------
/ Mars - God of War
/-------------------------------------------------------------------*/
ROM_START(marsp)
	ROM_REGION(0x10000, "maincpu", 0)
	ROM_LOAD("u2_80.bin", 0x2000, 0x1000, CRC(4f0bc7b1) SHA1(612cbacdca5cfa6ad23940796df3b7c385be79fe))
	ROM_RELOAD(0x6000, 0x1000)
	ROM_RELOAD(0xa000, 0x1000)
	ROM_RELOAD(0xe000, 0x1000)
	ROM_LOAD("u3_80.bin", 0x3000, 0x1000, CRC(1e69f9d0) SHA1(ad738cac2555830257b531e5e533b15362f624b9))
	ROM_RELOAD(0x7000, 0x1000)
	ROM_RELOAD(0xb000, 0x1000)
	ROM_RELOAD(0xf000, 0x1000)
	ROM_LOAD("666-1.cpu", 0x1000, 0x0800, CRC(bb7d476a) SHA1(22d5d7f0e52c5180f73a1ca0b3c6bd4b7d0843d6))
	ROM_RELOAD(0x5000, 0x0800)
	ROM_RELOAD(0x9000, 0x0800)
	ROM_RELOAD(0xd000, 0x0800)

	ROM_REGION(0x10000, "cpu2", 0)
	ROM_LOAD("666-s1.snd", 0x7000, 0x0800, CRC(d33dc8a5) SHA1(8d071c392996a74c3cdc2cf5ea3be3c86553ce89))
	ROM_LOAD("666-s2.snd", 0x7800, 0x0800, CRC(e5616f3e) SHA1(a6b5ebd0b456a555db0889cd63ce79aafc64dbe5))
ROM_END

/*-------------------------------------------------------------------
/ Panthera
/-------------------------------------------------------------------*/
ROM_START(panthera)
	ROM_REGION(0x10000, "maincpu", 0)
	ROM_LOAD("u2_80.bin", 0x2000, 0x1000, CRC(4f0bc7b1) SHA1(612cbacdca5cfa6ad23940796df3b7c385be79fe))
	ROM_RELOAD(0x6000, 0x1000)
	ROM_RELOAD(0xa000, 0x1000)
	ROM_RELOAD(0xe000, 0x1000)
	ROM_LOAD("u3_80.bin", 0x3000, 0x1000, CRC(1e69f9d0) SHA1(ad738cac2555830257b531e5e533b15362f624b9))
	ROM_RELOAD(0x7000, 0x1000)
	ROM_RELOAD(0xb000, 0x1000)
	ROM_RELOAD(0xf000, 0x1000)
	ROM_LOAD("652.cpu", 0x1000, 0x0800, CRC(5386e5fb) SHA1(822f47951b702f9c6a1ce674baaab0a596f34413))
	ROM_RELOAD(0x5000, 0x0800)
	ROM_RELOAD(0x9000, 0x0800)
	ROM_RELOAD(0xd000, 0x0800)

	ROM_REGION(0x10000, "cpu2", 0)
	ROM_LOAD("652.snd", 0x0400, 0x0400, CRC(4d0cf2c0) SHA1(0da5d118ffd19b1e78dfaaee3e31c43750d45c8d))
	ROM_RELOAD( 0x0800, 0x0400)
	ROM_LOAD("6530sy80.bin", 0x0c00, 0x0400, CRC(c8ba951d) SHA1(e4aa152b36695a0205c19a8914e4d77373f64c6c))
	ROM_RELOAD( 0xfc00, 0x0400)
ROM_END

ROM_START(panther7)
	ROM_REGION(0x10000, "maincpu", 0)
	ROM_LOAD("u2g807dc.bin", 0x2000, 0x1000, CRC(f8a687b3) SHA1(ba7747c04a5967df760ace102e47c91d42e07a12))
	ROM_RELOAD(0x6000, 0x1000)
	ROM_RELOAD(0xa000, 0x1000)
	ROM_RELOAD(0xe000, 0x1000)
	ROM_LOAD("u3g807dc.bin", 0x3000, 0x1000, CRC(6e31242e) SHA1(14e371a0352a6068dec20af1f2b344e34a5b9011))
	ROM_RELOAD(0x7000, 0x1000)
	ROM_RELOAD(0xb000, 0x1000)
	ROM_RELOAD(0xf000, 0x1000)
	ROM_LOAD("652.cpu", 0x1000, 0x0800, CRC(5386e5fb) SHA1(822f47951b702f9c6a1ce674baaab0a596f34413))
	ROM_RELOAD(0x5000, 0x0800)
	ROM_RELOAD(0x9000, 0x0800)
	ROM_RELOAD(0xd000, 0x0800)
	ROM_REGION(0x10000, "cpu2", 0)
	ROM_LOAD("652.snd", 0x0400, 0x0400, CRC(4d0cf2c0) SHA1(0da5d118ffd19b1e78dfaaee3e31c43750d45c8d))
	ROM_RELOAD( 0x0800, 0x0400)
	ROM_LOAD("6530sy80.bin", 0x0c00, 0x0400, CRC(c8ba951d) SHA1(e4aa152b36695a0205c19a8914e4d77373f64c6c))
	ROM_RELOAD( 0xfc00, 0x0400)
ROM_END

/*-------------------------------------------------------------------
/ Pink Panther
/-------------------------------------------------------------------*/
ROM_START(pnkpnthr)
	ROM_REGION(0x10000, "maincpu", 0)
	ROM_LOAD("u2_80.bin", 0x2000, 0x1000, CRC(4f0bc7b1) SHA1(612cbacdca5cfa6ad23940796df3b7c385be79fe))
	ROM_RELOAD(0x6000, 0x1000)
	ROM_RELOAD(0xa000, 0x1000)
	ROM_RELOAD(0xe000, 0x1000)
	ROM_LOAD("u3_80.bin", 0x3000, 0x1000, CRC(1e69f9d0) SHA1(ad738cac2555830257b531e5e533b15362f624b9))
	ROM_RELOAD(0x7000, 0x1000)
	ROM_RELOAD(0xb000, 0x1000)
	ROM_RELOAD(0xf000, 0x1000)
	ROM_LOAD("664-1.cpu", 0x1000, 0x0800, CRC(a0d3e69a) SHA1(590e68dc28067e61832927cd4b3eefcc066f0a92))
	ROM_RELOAD(0x5000, 0x0800)
	ROM_RELOAD(0x9000, 0x0800)
	ROM_RELOAD(0xd000, 0x0800)

	ROM_REGION(0x10000, "cpu2", 0)
	ROM_LOAD("664.snd", 0x0400, 0x0400, CRC(18f4abfd) SHA1(9e85eb7e9b1e2fe71be828ff1b5752424ed42588))
	ROM_RELOAD( 0x0800, 0x0400)
	ROM_LOAD("6530sy80.bin", 0x0c00, 0x0400, CRC(c8ba951d) SHA1(e4aa152b36695a0205c19a8914e4d77373f64c6c))
	ROM_RELOAD( 0xfc00, 0x0400)
ROM_END

ROM_START(pnkpntr7)
	ROM_REGION(0x10000, "maincpu", 0)
	ROM_LOAD("u2g807dc.bin", 0x2000, 0x1000, CRC(f8a687b3) SHA1(ba7747c04a5967df760ace102e47c91d42e07a12))
	ROM_RELOAD(0x6000, 0x1000)
	ROM_RELOAD(0xa000, 0x1000)
	ROM_RELOAD(0xe000, 0x1000)
	ROM_LOAD("u3g807dc.bin", 0x3000, 0x1000, CRC(6e31242e) SHA1(14e371a0352a6068dec20af1f2b344e34a5b9011))
	ROM_RELOAD(0x7000, 0x1000)
	ROM_RELOAD(0xb000, 0x1000)
	ROM_RELOAD(0xf000, 0x1000)
	ROM_LOAD("664-1.cpu", 0x1000, 0x0800, CRC(a0d3e69a) SHA1(590e68dc28067e61832927cd4b3eefcc066f0a92))
	ROM_RELOAD(0x5000, 0x0800)
	ROM_RELOAD(0x9000, 0x0800)
	ROM_RELOAD(0xd000, 0x0800)
	ROM_REGION(0x10000, "cpu2", 0)
	ROM_LOAD("664.snd", 0x0400, 0x0400, CRC(18f4abfd) SHA1(9e85eb7e9b1e2fe71be828ff1b5752424ed42588))
	ROM_RELOAD( 0x0800, 0x0400)
	ROM_LOAD("6530sy80.bin", 0x0c00, 0x0400, CRC(c8ba951d) SHA1(e4aa152b36695a0205c19a8914e4d77373f64c6c))
	ROM_RELOAD( 0xfc00, 0x0400)
ROM_END

/*-------------------------------------------------------------------
/ Star Race
/-------------------------------------------------------------------*/
ROM_START(starrace)
	ROM_REGION(0x10000, "maincpu", 0)
	ROM_LOAD("u2_80.bin", 0x2000, 0x1000, CRC(4f0bc7b1) SHA1(612cbacdca5cfa6ad23940796df3b7c385be79fe))
	ROM_RELOAD(0x6000, 0x1000)
	ROM_RELOAD(0xa000, 0x1000)
	ROM_RELOAD(0xe000, 0x1000)
	ROM_LOAD("u3_80.bin", 0x3000, 0x1000, CRC(1e69f9d0) SHA1(ad738cac2555830257b531e5e533b15362f624b9))
	ROM_RELOAD(0x7000, 0x1000)
	ROM_RELOAD(0xb000, 0x1000)
	ROM_RELOAD(0xf000, 0x1000)
	ROM_LOAD("657-1.cpu", 0x1000, 0x0200, CRC(27081372) SHA1(2d9cd81ffa44c389c4895043fa1e93b899544158))
	ROM_RELOAD(0x1400, 0x0200)
	ROM_RELOAD(0x5000, 0x0200)
	ROM_RELOAD(0x5400, 0x0200)
	ROM_RELOAD(0x9000, 0x0200)
	ROM_RELOAD(0x9400, 0x0200)
	ROM_RELOAD(0xd000, 0x0200)
	ROM_RELOAD(0xd400, 0x0200)
	ROM_LOAD("657-2.cpu", 0x1200, 0x0200, CRC(c56e31c8) SHA1(1e129fb6309e015a16f2bdb1e389cbc85d1919a7))
	ROM_RELOAD(0x1600, 0x0200)
	ROM_RELOAD(0x5000, 0x0200)
	ROM_RELOAD(0x5600, 0x0200)
	ROM_RELOAD(0x9000, 0x0200)
	ROM_RELOAD(0x9600, 0x0200)
	ROM_RELOAD(0xd000, 0x0200)
	ROM_RELOAD(0xd600, 0x0200)

	ROM_REGION(0x10000, "cpu2", 0)
	ROM_LOAD("657.snd", 0x0400, 0x0400, CRC(3a1d3995) SHA1(6f0bdb34c4fa11d5f8ecbb98ae55bafeb5d62c9e))
	ROM_RELOAD( 0x0800, 0x0400)
	ROM_LOAD("6530sy80.bin", 0x0c00, 0x0400, CRC(c8ba951d) SHA1(e4aa152b36695a0205c19a8914e4d77373f64c6c))
	ROM_RELOAD( 0xfc00, 0x0400)
ROM_END

ROM_START(starrac7)
	ROM_REGION(0x10000, "maincpu", 0)
	ROM_LOAD("u2g807dc.bin", 0x2000, 0x1000, CRC(f8a687b3) SHA1(ba7747c04a5967df760ace102e47c91d42e07a12))
	ROM_RELOAD(0x6000, 0x1000)
	ROM_RELOAD(0xa000, 0x1000)
	ROM_RELOAD(0xe000, 0x1000)
	ROM_LOAD("u3g807dc.bin", 0x3000, 0x1000, CRC(6e31242e) SHA1(14e371a0352a6068dec20af1f2b344e34a5b9011))
	ROM_RELOAD(0x7000, 0x1000)
	ROM_RELOAD(0xb000, 0x1000)
	ROM_RELOAD(0xf000, 0x1000)
	ROM_LOAD("657-1.cpu", 0x1000, 0x0200, CRC(27081372) SHA1(2d9cd81ffa44c389c4895043fa1e93b899544158))
	ROM_RELOAD(0x1400, 0x0200)
	ROM_RELOAD(0x5000, 0x0200)
	ROM_RELOAD(0x5400, 0x0200)
	ROM_RELOAD(0x9000, 0x0200)
	ROM_RELOAD(0x9400, 0x0200)
	ROM_RELOAD(0xd000, 0x0200)
	ROM_RELOAD(0xd400, 0x0200)
	ROM_LOAD("657-2.cpu", 0x1200, 0x0200, CRC(c56e31c8) SHA1(1e129fb6309e015a16f2bdb1e389cbc85d1919a7))
	ROM_RELOAD(0x1600, 0x0200)
	ROM_RELOAD(0x5000, 0x0200)
	ROM_RELOAD(0x5600, 0x0200)
	ROM_RELOAD(0x9000, 0x0200)
	ROM_RELOAD(0x9600, 0x0200)
	ROM_RELOAD(0xd000, 0x0200)
	ROM_RELOAD(0xd600, 0x0200)
	ROM_REGION(0x10000, "cpu2", 0)
	ROM_LOAD("657.snd", 0x0400, 0x0400, CRC(3a1d3995) SHA1(6f0bdb34c4fa11d5f8ecbb98ae55bafeb5d62c9e))
	ROM_RELOAD( 0x0800, 0x0400)
	ROM_LOAD("6530sy80.bin", 0x0c00, 0x0400, CRC(c8ba951d) SHA1(e4aa152b36695a0205c19a8914e4d77373f64c6c))
	ROM_RELOAD( 0xfc00, 0x0400)
ROM_END

/*-------------------------------------------------------------------
/ The Amazing Spider-Man
/-------------------------------------------------------------------*/
ROM_START(spidermn)
	ROM_REGION(0x10000, "maincpu", 0)
	ROM_LOAD("u2_80.bin", 0x2000, 0x1000, CRC(4f0bc7b1) SHA1(612cbacdca5cfa6ad23940796df3b7c385be79fe))
	ROM_RELOAD(0x6000, 0x1000)
	ROM_RELOAD(0xa000, 0x1000)
	ROM_RELOAD(0xe000, 0x1000)
	ROM_LOAD("u3_80.bin", 0x3000, 0x1000, CRC(1e69f9d0) SHA1(ad738cac2555830257b531e5e533b15362f624b9))
	ROM_RELOAD(0x7000, 0x1000)
	ROM_RELOAD(0xb000, 0x1000)
	ROM_RELOAD(0xf000, 0x1000)
	ROM_LOAD("653-1.cpu", 0x1000, 0x0200, CRC(674ddc58) SHA1(c9be45391b8dd58a0836801807d593d4c7da9904))
	ROM_RELOAD(0x1400, 0x0200)
	ROM_RELOAD(0x5000, 0x0200)
	ROM_RELOAD(0x5400, 0x0200)
	ROM_RELOAD(0x9000, 0x0200)
	ROM_RELOAD(0x9400, 0x0200)
	ROM_RELOAD(0xd000, 0x0200)
	ROM_RELOAD(0xd400, 0x0200)
	ROM_LOAD("653-2.cpu", 0x1200, 0x0200, CRC(ff1ddfd7) SHA1(dd7b98e491045916153b760f36432506277a4093))
	ROM_RELOAD(0x1600, 0x0200)
	ROM_RELOAD(0x5000, 0x0200)
	ROM_RELOAD(0x5600, 0x0200)
	ROM_RELOAD(0x9000, 0x0200)
	ROM_RELOAD(0x9600, 0x0200)
	ROM_RELOAD(0xd000, 0x0200)
	ROM_RELOAD(0xd600, 0x0200)

	ROM_REGION(0x10000, "cpu2", 0)
	ROM_LOAD("653.snd", 0x0400, 0x0400, CRC(f5650c46) SHA1(2d0e50fa2f4b3d633daeaa7454630e3444453cb2))
	ROM_RELOAD( 0x0800, 0x0400)
	ROM_LOAD("6530sy80.bin", 0x0c00, 0x0400, CRC(c8ba951d) SHA1(e4aa152b36695a0205c19a8914e4d77373f64c6c))
	ROM_RELOAD( 0xfc00, 0x0400)
ROM_END

/*-------------------------------------------------------------------
/ The Incredible Hulk
/-------------------------------------------------------------------*/

/*-------------------------------------------------------------------
/ Time Line
/-------------------------------------------------------------------*/
ROM_START(timeline)
	ROM_REGION(0x10000, "maincpu", 0)
	ROM_LOAD("u2_80.bin", 0x2000, 0x1000, CRC(4f0bc7b1) SHA1(612cbacdca5cfa6ad23940796df3b7c385be79fe))
	ROM_RELOAD(0x6000, 0x1000)
	ROM_RELOAD(0xa000, 0x1000)
	ROM_RELOAD(0xe000, 0x1000)
	ROM_LOAD("u3_80.bin", 0x3000, 0x1000, CRC(1e69f9d0) SHA1(ad738cac2555830257b531e5e533b15362f624b9))
	ROM_RELOAD(0x7000, 0x1000)
	ROM_RELOAD(0xb000, 0x1000)
	ROM_RELOAD(0xf000, 0x1000)
	ROM_LOAD("659.cpu", 0x1000, 0x0800, CRC(d6950e3b) SHA1(939b45a9ee4bb122fbea534ad728ec6b85120416))
	ROM_RELOAD(0x5000, 0x0800)
	ROM_RELOAD(0x9000, 0x0800)
	ROM_RELOAD(0xd000, 0x0800)

	ROM_REGION(0x10000, "cpu2", 0)
	ROM_LOAD("659.snd", 0x0400, 0x0400, CRC(28185568) SHA1(2fd26e7e0a8f050d67159f17634df2b1fc47cbd3))
	ROM_RELOAD( 0x0800, 0x0400)
	ROM_LOAD("6530sy80.bin", 0x0c00, 0x0400, CRC(c8ba951d) SHA1(e4aa152b36695a0205c19a8914e4d77373f64c6c))
	ROM_RELOAD( 0xfc00, 0x0400)
ROM_END

/*-------------------------------------------------------------------
/ Volcano (Sound and Speech)
/-------------------------------------------------------------------*/
ROM_START(vlcno_ax)
	ROM_REGION(0x10000, "maincpu", 0)
	ROM_LOAD("u2_80.bin", 0x2000, 0x1000, CRC(4f0bc7b1) SHA1(612cbacdca5cfa6ad23940796df3b7c385be79fe))
	ROM_RELOAD(0x6000, 0x1000)
	ROM_RELOAD(0xa000, 0x1000)
	ROM_RELOAD(0xe000, 0x1000)
	ROM_LOAD("u3_80.bin", 0x3000, 0x1000, CRC(1e69f9d0) SHA1(ad738cac2555830257b531e5e533b15362f624b9))
	ROM_RELOAD(0x7000, 0x1000)
	ROM_RELOAD(0xb000, 0x1000)
	ROM_RELOAD(0xf000, 0x1000)
	ROM_LOAD("667-a-x.cpu", 0x1000, 0x0800, CRC(1f51c351) SHA1(8e1850808faab843ac324040ca665a83809cdc7b))
	ROM_RELOAD(0x5000, 0x0800)
	ROM_RELOAD(0x9000, 0x0800)
	ROM_RELOAD(0xd000, 0x0800)

	ROM_REGION(0x10000, "cpu2", 0)
	ROM_LOAD("667-s1.snd", 0x7000, 0x0800, CRC(ba9d40b7) SHA1(3d6640b259cd8ae87b998cbf1ae2dc13a2913e4f))
	ROM_LOAD("667-s2.snd", 0x7800, 0x0800, CRC(b54bd123) SHA1(3522ccdcb28bfacff2287f5537d52f22879249ab))
ROM_END

ROM_START(vlcno_1b)
	ROM_REGION(0x10000, "maincpu", 0)
	ROM_LOAD("u2_80.bin", 0x2000, 0x1000, CRC(4f0bc7b1) SHA1(612cbacdca5cfa6ad23940796df3b7c385be79fe))
	ROM_RELOAD(0x6000, 0x1000)
	ROM_RELOAD(0xa000, 0x1000)
	ROM_RELOAD(0xe000, 0x1000)
	ROM_LOAD("u3_80.bin", 0x3000, 0x1000, CRC(1e69f9d0) SHA1(ad738cac2555830257b531e5e533b15362f624b9))
	ROM_RELOAD(0x7000, 0x1000)
	ROM_RELOAD(0xb000, 0x1000)
	ROM_RELOAD(0xf000, 0x1000)
	ROM_LOAD("667-1b.cpu", 0x1000, 0x0800, CRC(a422d862) SHA1(2785388eb43c08405774a9413ffa52c1591a84f2))
	ROM_RELOAD(0x5000, 0x0800)
	ROM_RELOAD(0x9000, 0x0800)
	ROM_RELOAD(0xd000, 0x0800)

	ROM_REGION(0x10000, "cpu2", 0)
	ROM_LOAD("667-a-s.snd", 0x0400, 0x0400, CRC(894b4e2e) SHA1(d888f8e00b2b50cef5cc916d46e4c5e6699914a1))
	ROM_RELOAD( 0x0800, 0x0400)
	ROM_LOAD("6530sy80.bin", 0x0c00, 0x0400, CRC(c8ba951d) SHA1(e4aa152b36695a0205c19a8914e4d77373f64c6c))
	ROM_RELOAD( 0xfc00, 0x0400)
ROM_END

ROM_START(vlcno_1a)
	ROM_REGION(0x10000, "maincpu", 0)
	ROM_LOAD("u2_80.bin", 0x2000, 0x1000, CRC(4f0bc7b1) SHA1(612cbacdca5cfa6ad23940796df3b7c385be79fe))
	ROM_RELOAD(0x6000, 0x1000)
	ROM_RELOAD(0xa000, 0x1000)
	ROM_RELOAD(0xe000, 0x1000)
	ROM_LOAD("u3_80.bin", 0x3000, 0x1000, CRC(1e69f9d0) SHA1(ad738cac2555830257b531e5e533b15362f624b9))
	ROM_RELOAD(0x7000, 0x1000)
	ROM_RELOAD(0xb000, 0x1000)
	ROM_RELOAD(0xf000, 0x1000)
	ROM_LOAD("667-1a.cpu", 0x1000, 0x0800, CRC(5931c6f7) SHA1(e104a6c3ca2175bb49199e06963e26185dd563d2))
	ROM_RELOAD(0x5000, 0x0800)
	ROM_RELOAD(0x9000, 0x0800)
	ROM_RELOAD(0xd000, 0x0800)

	ROM_REGION(0x10000, "cpu2", 0)
	ROM_LOAD("667-a-s.snd", 0x0400, 0x0400, CRC(894b4e2e) SHA1(d888f8e00b2b50cef5cc916d46e4c5e6699914a1))
	ROM_RELOAD( 0x0800, 0x0400)
	ROM_LOAD("6530sy80.bin", 0x0c00, 0x0400, CRC(c8ba951d) SHA1(e4aa152b36695a0205c19a8914e4d77373f64c6c))
	ROM_RELOAD( 0xfc00, 0x0400)
ROM_END

/*-------------------------------------------------------------------
/ System 80 Test Fixture
/-------------------------------------------------------------------*/
ROM_START(s80tst)
	ROM_REGION(0x10000, "maincpu", 0)
	ROM_LOAD("u2_80.bin", 0x2000, 0x1000, CRC(4f0bc7b1) SHA1(612cbacdca5cfa6ad23940796df3b7c385be79fe))
	ROM_RELOAD(0x6000, 0x1000)
	ROM_RELOAD(0xa000, 0x1000)
	ROM_RELOAD(0xe000, 0x1000)
	ROM_LOAD("u3_80.bin", 0x3000, 0x1000, CRC(1e69f9d0) SHA1(ad738cac2555830257b531e5e533b15362f624b9))
	ROM_RELOAD(0x7000, 0x1000)
	ROM_RELOAD(0xb000, 0x1000)
	ROM_RELOAD(0xf000, 0x1000)
	ROM_LOAD("80tst.cpu", 0x1000, 0x0800, CRC(a0f9e56b) SHA1(5146745ab61fea4b3070c6cf4324a9e77a7cee36))
	ROM_RELOAD(0x5000, 0x0800)
	ROM_RELOAD(0x9000, 0x0800)
	ROM_RELOAD(0xd000, 0x0800)

	ROM_REGION(0x10000, "cpu2", 0)
	ROM_LOAD("80tst-s1.snd", 0x7000, 0x0800, CRC(b9dbdd21) SHA1(dfe42c9e6e02f82ffd0cafe164df3211cdc2d966))
	ROM_LOAD("80tst-s2.snd", 0x7800, 0x0800, CRC(1a4b1e9d) SHA1(18e7ffbdbdaf83ab1c8daa5fa5201d9f54390758))
ROM_END

/* disp2 */GAME(1981,	blckhole,	0,		gts80_ss,	gts80,	gts80,	ROT0,	"Gottlieb",		"Black Hole (Rev. 4)",			GAME_IS_SKELETON_MECHANICAL)
/* disp2 */GAME(1981,	blckhole2,	blckhole,	gts80_ss,	gts80,	gts80,	ROT0,	"Gottlieb",		"Black Hole (Rev. 2)",		GAME_IS_SKELETON_MECHANICAL)
/* disp2 */GAME(1981,	blckhols,	0,			gts80_s,	gts80,	gts80,	ROT0,	"Gottlieb",		"Black Hole (Sound Only)",	GAME_IS_SKELETON_MECHANICAL)
/* disp1 */GAME(1980,	circusp,	0,		gts80_s,	gts80,	gts80,	ROT0,	"Gottlieb",		"Circus",				GAME_IS_SKELETON_MECHANICAL)
/* disp1 */GAME(1980,	cntforce,	0,		gts80_s,	gts80,	gts80,	ROT0,	"Gottlieb",		"Counterforce",			GAME_IS_SKELETON_MECHANICAL)
/* disp2 */GAME(1981,	eclipse,	0,		gts80_s,	gts80,	gts80,	ROT0,	"Gottlieb",		"Eclipse",				GAME_IS_SKELETON_MECHANICAL)
/* disp1 */GAME(1981,	forceii,	0,		gts80_s,	gts80,	gts80,	ROT0,	"Gottlieb",		"Force II",				GAME_IS_SKELETON_MECHANICAL)
/* disp2 */GAME(1982,	hh,		0,		gts80_ss,	gts80,	gts80,	ROT0,	"Gottlieb",		"Haunted House (Rev. 2)",	GAME_IS_SKELETON_MECHANICAL)
/* disp2 */GAME(1982,	hh_1,		hh,		gts80_ss,	gts80,	gts80,	ROT0,	"Gottlieb",		"Haunted House (Rev. 1)",	GAME_IS_SKELETON_MECHANICAL)
/* disp2 */GAME(1980,	jamesb,	0,		gts80_s,	gts80,	gts80,	ROT0,	"Gottlieb",		"James Bond (Timed Play)",	GAME_IS_SKELETON_MECHANICAL)
/* disp2 */GAME(1980,	jamesb2,	jamesb,	gts80_s,	gts80,	gts80,	ROT0,	"Gottlieb",		"James Bond (3/5-Ball)",	GAME_IS_SKELETON_MECHANICAL)
/* disp1 */GAME(1981,	marsp,		0,		gts80_ss,	gts80,	gts80,	ROT0,	"Gottlieb",		"Mars - God of War",		GAME_IS_SKELETON_MECHANICAL)
/* disp1 */GAME(1980,	panthera,	0,		gts80_s,	gts80,	gts80,	ROT0,	"Gottlieb",		"Panthera",				GAME_IS_SKELETON_MECHANICAL)
/* cust  */GAME(1981,	pnkpnthr,	0,		gts80_s,	gts80,	gts80,	ROT0,	"Gottlieb",		"Pink Panther",			GAME_IS_SKELETON_MECHANICAL)
/* disp1 */GAME(1980,	starrace,	0,		gts80_s,	gts80,	gts80,	ROT0,	"Gottlieb",		"Star Race",			GAME_IS_SKELETON_MECHANICAL)
/* disp1 */GAME(1980,	spidermn,	0,		gts80_s,	gts80,	gts80,	ROT0,	"Gottlieb",		"The Amazing Spider-Man",	GAME_IS_SKELETON_MECHANICAL)
/* cust  */GAME(1980,	timeline,	0,		gts80_s,	gts80,	gts80,	ROT0,	"Gottlieb",		"Time Line",			GAME_IS_SKELETON_MECHANICAL)
/* disp1 */GAME(1981,	vlcno_ax,	0,		gts80_ss,	gts80,	gts80,	ROT0,	"Gottlieb",		"Volcano",				GAME_IS_SKELETON_MECHANICAL)
/* disp1 */GAME(1981,	vlcno_1b,	0,			gts80_s,	gts80,	gts80,	ROT0,	"Gottlieb",		"Volcano (Sound Only)",	GAME_IS_SKELETON_MECHANICAL)
/* disp1 */GAME(1981,	vlcno_1a,	vlcno_1b,	gts80_s,	gts80,	gts80,	ROT0,	"Gottlieb",		"Volcano (Sound Only, alternate version)",		GAME_IS_SKELETON_MECHANICAL)
/* disp1 */GAME(1981,	s80tst,	0,		gts80_ss,	gts80,	gts80,	ROT0,	"Gottlieb",		"System 80 Test",			GAME_IS_SKELETON_MECHANICAL)
