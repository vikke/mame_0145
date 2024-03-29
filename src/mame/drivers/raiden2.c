/* Seibu Protected 1993-94 era hardware, V30 based (sequel to the 68k based hardware)
    TODO: figure out the rest of the protection
    TODO: Zero team needs COIN3 and COIN4 found and hooked up, they may be on the sound cpu
*/

/* raiden 2 board test note 17/04/08 (based on test by dox)

 rom banking is at 6c9, bit 0x80
  -- the game only writes this directly at startup, must be written indirectly by
     one of the protection commands? or mirrored?
  value of 0x80 puts 0x00000-0x1ffff at 0x20000 - 0x3ffff
  value of 0x00 puts 0x20000-0x3ffff at 0x20000 - 0x3ffff

*/

/*
Raiden DX
Seibu Kaihatsu, 1994

This readme covers Raiden DX and to some extent Raiden II
which uses an almost identical PCB.

PCB Layout
----------

(C) 1993 RAIDEN II DX SEIBU KAIHATSU INC.,o
|----------------------------------------------------------|
|      1    2   3   4   5   6    7      8      9     10    |
|LA4460    M6295  PCM  Z80     6116                       A|
|   YM2151 M6295   6    5      6116    28.63636MHz        B|
|     VOL   YM3012                                         |
|HB-45A            |------|                               C|
|HB-2       4560   |SIE150| 6116      |---------|          |
|RC220             |      | 6116      | SEI252  |         D|
|                  |------| 6116      |SB05-106 |          |
|                           6116      |(QFP208) |         E|
|J                                    |         |         F|
|A    DSW2(8)                         |---------|          |
|M                                                        G|
|M    DSW1(8)                                   CXK58258   |
|A          |---------|OBJ-1    OBJ-2           CXK58258  H|
|           | SEI360  |                         CXK58258  J|
|           |SB06-1937|DX_OBJ-3 DX_OBJ-4        CXK58258  K|
|           |(QFP160) |  PAL1               |---------|   L|
|           |         |                     |SEI1000  |   M|
| |------|  |---------|  1H      3H         |SB01-001 |   N|
| |SEI200|         32MHz                    |(QFP184) |    |
| |      |CY7C185        2H      4H         |         |   P|
| |------|CY7C185                           |---------|    |
|                                                         Q|
|                        PAL2 PAL3             |----|     R|
|                                              |V30 |      |
| DX_BACK-1  DX_BACK-2   7   COPX-D2           |----|     S|
|----------------------------------------------------------|
Notes:
      V30 clock    - 16.000MHz [32/2]. Chip is stamped "NEC D70116HG-16 V30 NEC '84" (QFP52)
      Z80 clock    - 3.579545MHz [28.63636/8]
      YM2151 clock - 3.579545MHz [28.63636/8]
      M6295 clocks - 1.022MHz [28.63636/28] and pin 7 HIGH (both)
      CXK58258     - Sony CXK58258 32k x8 SRAM (= 62256)
      CY7C185      - Cypress CY7C185 8k x8 SRAM (= 6264)
      6116         - 2k x8 SRAM
      HB-45A       - Seibu custom ceramic module sound DAC (SIP20)
      HB-2         - Seibu custom ceramic module connected to coin counters (SIP10)
      RC220        - Custom resistor network module used for inputs (SIP14)
      VSync        - 55.4859Hz  \
      HSync        - 15.5586kHz / measured via EL4583
      PAL1         - AMI 18CV8 stamped 'JJ5004' (DIP20)
      PAL2         - AMI 18CV8 stamped 'JJ5002' (DIP20)
      PAL3         - AMI 18CV8 stamped 'JJ5001' (DIP20)
      ROMs         - *PCM      - 2M MaskROM stamped 'RAIDEN 2 PCM' at location U1018 (DIP32)
                     6         - 27C020 EPROM labelled 'SEIBU 6' at location U1017 (DIP32)
                     5         - 27C512 EPROM labelled 'SEIBU 5' at location U1110 (DIP28)
                     *OBJ-1    - 16M MaskROM stamped 'RAIDEN 2 OBJ-1' at location U0811 (DIP42)
                     *OBJ-2    - 16M MaskROM stamped 'RAIDEN 2 OBJ-2' at location U082 (DIP42)
                     DX_OBJ-3  - 16M MaskROM stamped 'DX OBJ-3' at location U0837 (DIP42)
                     DX_OBJ-4  - 16M MaskROM stamped 'DX OBJ-4' at location U0836 (DIP42)
                     1H        - 27C4001 EPROM labelled 'SEIBU 1H' at location U1210 (DIP32)
                     2H        - 27C4001 EPROM labelled 'SEIBU 2H' at location U1211 (DIP32)
                     3H        - 27C4001 EPROM labelled 'SEIBU 3H' at location U129 (DIP32)
                     4H        - 27C4001 EPROM labelled 'SEIBU 4H' at location U1212 (DIP32)
                     DX_BACK-1 - 16M MaskROM stamped 'DX BACK-1' at location U075 (DIP42)
                     DX_BACK-2 - 16M MaskROM stamped 'DX BACK-2' at location U0714 (DIP42)
                     7         - 27C210 EPROM labelled 'SEIBU 7' at location U0724 (DIP40)
                     *COPX-D2  - 2M MaskROM stamped 'COPX-D2' at location U0313 (DIP40)

                     * = these ROMs are soldered-in and match ROMs from the original Raiden II PCB

      SEIBU Custom ICs -
                        SIE150 (QFP100)
                        SEI252 SB05-106 (QFP208)
                        SEI0200 TC110G21AF 0076 (QFP100)
                        SEI360 SB06-1937 (QFP160)
                        SEI1000 SB01-001 (QFP184)


Games on this PCB / Similar PCBs
 Raiden 2
 Raiden DX
 Zero Team
 X Se Dae Quiz

 + variants

Some of these games were also released on updated PCBs
which usually featured vastly inferior sound hardware
 (see the V33 based version of Raiden II/DX New)


Protection Notes:
 These games use the 2nd (and 3rd) generation of Seibu's 'COP' protection,
 utilizing the external 'COPX_D2' and 'COPX_D3' lookup roms (probably for
 math operations)  These chips, marked (c)1992 RISE Corp. are not thought
 to be the actual MCU which is probably internal to one of the Seibu
 customs.

 The games in legionna.c use the same protection chips.

Current Problem(s) - in order of priority

 High Priority

 ROM banking - we don't know where the ROM bank registers are, this causes
 serious problems as it's hard to see which glitches are caused by
 protection, and which are caused by a lack of ROM banking.

 Protection - it isn't emulated, until it is the games will never work.

 Sprite Encryption - this is 99% complete for Raiden 2 / DX, just a few bad
 bits remain.  No decryption support for Zero Team yet.

 Video emulation - used to be more complete than it is now, tile banking is
 currently broken.

 Low Priority

*/

#define ADDRESS_MAP_MODERN
#include "emu.h"
#include "cpu/nec/nec.h"
#include "cpu/z80/z80.h"
#include "audio/seibu.h"
#include "machine/eeprom.h"
#include "sound/okim6295.h"
#include "includes/raiden2.h"

static UINT16 rps(running_machine &machine)
{
	return cpu_get_reg(machine.device("maincpu"), NEC_CS);
}

static UINT16 rpc(running_machine &machine)
{
	return cpu_get_reg(machine.device("maincpu"), NEC_IP);
}

WRITE16_MEMBER(raiden2_state::cop_pgm_data_w)
{
	assert(ACCESSING_BITS_0_7 && ACCESSING_BITS_8_15);
	cop_program[cop_latch_addr] = data;
	int idx = cop_latch_addr >> 3;
	cop_func_trigger[idx] = cop_latch_trigger;
	cop_func_value[idx]   = cop_latch_value;
	cop_func_mask[idx]    = cop_latch_mask;

	if(data) {
		int off = data & 31;
		int reg = (data >> 5) & 3;
		int op = (data >> 7) & 31;

		logerror("COPDIS: %04x s=%02x f1=%x l=%x f2=%02x %x %04x %02x %03x %02x.%x.%02x ", cop_latch_trigger,  (cop_latch_trigger >> 11) << 3, (cop_latch_trigger >> 10) & 1, ((cop_latch_trigger >> 7) & 7)+1, cop_latch_trigger & 0x7f, cop_latch_value, cop_latch_mask, cop_latch_addr, data, op, reg, off);

		off *= 2;

		// COPDIS: 0205 s=00 f1=0 l=5 f2=05 6 ffeb 00 188 03.0.08 read32 10(r0)
		// COPDIS: 0205 s=00 f1=0 l=5 f2=05 6 ffeb 01 282 05.0.02 add32 4(r0)
		// COPDIS: 0205 s=00 f1=0 l=5 f2=05 6 ffeb 02 082 01.0.02 write32 4(r0)
		// COPDIS: 0205 s=00 f1=0 l=5 f2=05 6 ffeb 03 b8e 17.0.0e add16h 1c(r0)
		// COPDIS: 0205 s=00 f1=0 l=5 f2=05 6 ffeb 04 98e 13.0.0e write16h 1c(r0)

		// 188 182 082 b8e 98e -> 04  = 04+04    1ch = 1c+04
		// 188 188 082 b8e 98e -> 04  = 04+10    1ch = 1c+10
		// 188 18e 082 b8e 98e -> 04  = 04+1c    1ch = 1c+1c
		// 188 282 082 b8e 98e -> 04  = 04+10    1ch = 1c+10
		// 188 288 082 b8e 98e -> 04  = 10+10    1ch = 1c+10
		// 188 28e 082 b8e 98e -> 04  = 1c+10    1ch = 1c+10
		// 188 282 282 282 082 -> 04  = 04+04+10 10h = 04+10
		// 188 188 188 188 082 -> 04h = 04+10    04l = 04+10+10
		// 188 188 188 188 082 -> 04  = 04+10    04l = 04+10+10  10h = 04+10 (same, but trigger = 020b)

		switch(op) {
		case 0x01:
			if(off)
				logerror("addmem32 %x(r%x)\n", off, reg);
			else
				logerror("addmem32 (r%x)\n", reg);
			break;
		case 0x03:
			if(off)
				logerror("read32 %x(r%x)\n", off, reg);
			else
				logerror("read32 (r%x)\n", reg);
			break;
		case 0x05:
			if(off)
				logerror("add32 %x(r%x)\n", off, reg);
			else
				logerror("add32 (r%x)\n", reg);
			break;
		case 0x13:
			if(off)
				logerror("write16h %x(r%x)\n", off, reg);
			else
				logerror("write16h (r%x)\n", reg);
			break;
		case 0x15:
			if(off)
				logerror("sub32 %x(r%x)\n", off, reg);
			else
				logerror("sub32 (r%x)\n", reg);
			break;
			break;
		case 0x17:
			if(off)
				logerror("addmem16 %x(r%x)\n", off, reg);
			else
				logerror("addmem16 (r%x)\n", reg);
			break;
		default:
			logerror("?\n");
			break;
		}
	}
}

WRITE16_MEMBER(raiden2_state::cop_pgm_addr_w)
{
	assert(ACCESSING_BITS_0_7 && ACCESSING_BITS_8_15);
	assert(data < 0x100);
	cop_latch_addr = data;
}

WRITE16_MEMBER(raiden2_state::cop_pgm_value_w)
{
	assert(ACCESSING_BITS_0_7 && ACCESSING_BITS_8_15);
	cop_latch_value = data;
}

WRITE16_MEMBER(raiden2_state::cop_pgm_mask_w)
{
	assert(ACCESSING_BITS_0_7 && ACCESSING_BITS_8_15);
	cop_latch_mask = data;
}

WRITE16_MEMBER(raiden2_state::cop_pgm_trigger_w)
{
	assert(ACCESSING_BITS_0_7 && ACCESSING_BITS_8_15);
	cop_latch_trigger = data;
}

WRITE16_MEMBER(raiden2_state::cop_dma_adr_rel_w)
{
	COMBINE_DATA(&cop_dma_adr_rel);
}

WRITE16_MEMBER(raiden2_state::cop_dma_v1_w)
{
	COMBINE_DATA(&cop_dma_v1);
}

WRITE16_MEMBER(raiden2_state::cop_dma_v2_w)
{
	COMBINE_DATA(&cop_dma_v2);
}

WRITE16_MEMBER(raiden2_state::cop_dma_dst_w)
{
	COMBINE_DATA(&cop_dma_dst[cop_dma_mode]);
}

READ16_MEMBER(raiden2_state::cop_dma_mode_r)
{
	return cop_dma_mode;
}

WRITE16_MEMBER(raiden2_state::cop_dma_mode_w)
{
	COMBINE_DATA(&cop_dma_mode);
}

WRITE16_MEMBER(raiden2_state::cop_dma_src_w)
{
	COMBINE_DATA(&cop_dma_src[cop_dma_mode]);
}

WRITE16_MEMBER(raiden2_state::cop_dma_size_w)
{
	COMBINE_DATA(&cop_dma_size[cop_dma_mode]);
}

WRITE16_MEMBER(raiden2_state::cop_dma_trigger_w)
{
	//  logerror("COP DMA mode=%x adr=%x size=%x vals=%x %x %x\n", cop_dma_mode, cop_dma_src[cop_dma_mode], cop_dma_size[cop_dma_mode], cop_dma_v1[cop_dma_mode], cop_dma_v2[cop_dma_mode], cop_dma_dst[cop_dma_mode]);

	switch(cop_dma_mode) {
	case 0x14: {
		/* TODO: this transfers the whole VRAM, not only spriteram!
           For whatever reason, this stopped working as soon as I've implemented DMA slot concept.
           Raiden 2 uses this DMA with cop_dma_dst == 0xfffe, effectively changing the order of the uploaded VRAMs.
           Also the size is used for doing a sprite limit trickery.
        */
		static int rsize = ((0x80 - cop_dma_size[cop_dma_mode]) & 0x7f) +1;

		sprites_cur_start = 0x1000 - (rsize << 5);
		#if 0
		int rsize = 32*(0x7f-cop_dma_size);
		int radr = 64*cop_dma_adr - rsize;
		for(int i=0; i<rsize; i+=2)
			sprites[i/2] = space.read_word(radr+i);
		sprites_cur_start = rsize;
		#endif
		break;
	}
	case 0x82: {
		UINT32 src,dst,size;
		int i;

		src = (cop_dma_src[cop_dma_mode] << 6) + (cop_dma_adr_rel * 0x400);
		dst = (cop_dma_dst[cop_dma_mode] << 6);
		size = ((cop_dma_size[cop_dma_mode] << 5) - (cop_dma_dst[cop_dma_mode] << 6) + 0x20)/2;

		//printf("%08x %08x %08x\n",src,dst,size);
		/* TODO: palette brightness */

		for(i = 0;i < size;i++)
		{
			space.write_word(dst, space.read_word(src));
			src+=2;
			dst+=2;
		}

		break;
	}
	case 0x09: {
		UINT32 src,dst,size;
		int i;

		src = (cop_dma_src[cop_dma_mode] << 6);
		dst = (cop_dma_dst[cop_dma_mode] << 6);
		size = ((cop_dma_size[cop_dma_mode] << 5) - (cop_dma_dst[cop_dma_mode] << 6) + 0x20)/2;

//      printf("%08x %08x %08x\n",src,dst,size);

		for(i = 0;i < size;i++)
		{
			space.write_word(dst, space.read_word(src));
			src+=2;
			dst+=2;
		}

		break;
	}
	case 0x118:
	case 0x11f: {
		UINT32 length, address;
		int i;
		if(cop_dma_dst[cop_dma_mode] != 0x0000) // Invalid?
			return;

		address = (cop_dma_src[cop_dma_mode] << 6);
		length = (cop_dma_size[cop_dma_mode]+1) << 5;

		//printf("%08x %08x\n",address,length);

		for (i=address;i<address+length;i+=4)
		{
			space.write_dword(i, (cop_dma_v1) | (cop_dma_v2 << 16));
		}
	}
	}
}

WRITE16_MEMBER(raiden2_state::cop_itoa_low_w)
{
	cop_itoa = (cop_itoa & ~UINT32(mem_mask)) | (data & mem_mask);

	int digits = 1 << cop_itoa_digit_count*2;
	UINT32 val = cop_itoa;

	if(digits > 9)
		digits = 9;
	for(int i=0; i<digits; i++)
		if(!val && i)
			cop_itoa_digits[i] = 0x20;
		else {
			cop_itoa_digits[i] = 0x30 | (val % 10);
			val = val / 10;
		}
	cop_itoa_digits[9] = 0;
}

WRITE16_MEMBER(raiden2_state::cop_itoa_high_w)
{
	cop_itoa = (cop_itoa & ~(mem_mask << 16)) | ((data & mem_mask) << 16);
}

WRITE16_MEMBER(raiden2_state::cop_itoa_digit_count_w)
{
	COMBINE_DATA(&cop_itoa_digit_count);
}

READ16_MEMBER(raiden2_state::cop_itoa_digits_r)
{
	return cop_itoa_digits[offset*2] | (cop_itoa_digits[offset*2+1] << 8);
}

READ16_MEMBER(raiden2_state::cop_status_r)
{
	return cop_status;
}

READ16_MEMBER(raiden2_state::cop_angle_r)
{
	return cop_angle;
}

READ16_MEMBER(raiden2_state::cop_dist_r)
{
	return cop_dist;
}

WRITE16_MEMBER(raiden2_state::cop_scale_w)
{
	COMBINE_DATA(&cop_scale);
}

READ16_MEMBER(raiden2_state::cop_reg_high_r)
{
	return cop_regs[offset] >> 16;
}

WRITE16_MEMBER(raiden2_state::cop_reg_high_w)
{
	cop_regs[offset] = (cop_regs[offset] & ~(mem_mask << 16)) | ((data & mem_mask) << 16);
}

READ16_MEMBER(raiden2_state::cop_reg_low_r)
{
	return cop_regs[offset];
}

WRITE16_MEMBER(raiden2_state::cop_reg_low_w)
{
	cop_regs[offset] = (cop_regs[offset] & ~UINT32(mem_mask)) | (data & mem_mask);
}

UINT8 raiden2_state::cop_calculate_collsion_detection(running_machine &machine)
{
	static UINT8 res;

	res = 3;

	/* outbound X check */
	if(cop_collision_info[0].max_x >= cop_collision_info[1].min_x && cop_collision_info[0].min_x <= cop_collision_info[1].max_x)
		res &= ~1;

	/* outbound Y check */
	if(cop_collision_info[0].max_y >= cop_collision_info[1].min_y && cop_collision_info[0].min_y <= cop_collision_info[1].max_y)
		res &= ~2;

	/* TODO: special collision detection for Zero Team */

	return res;
}

WRITE16_MEMBER(raiden2_state::cop_cmd_w)
{
	cop_status &= 0x7fff;

	switch(data) {
	case 0x0205:   // 0205 0006 ffeb 0000 - 0188 0282 0082 0b8e 098e 0000 0000 0000
		space.write_dword(cop_regs[0] + 4 + offset*4, space.read_dword(cop_regs[0] + 4 + offset*4) + space.read_dword(cop_regs[0] + 16 + offset*4));
		space.write_word(cop_regs[0] + 0x1c + offset*4, space.read_word(cop_regs[0] + 0x1c + offset*4) + space.read_word(cop_regs[0] + 16 + offset*4));
		break;

	case 0x0904: { /* X Se Dae and Zero Team uses this variant */
		space.write_dword(cop_regs[0] + 16 + offset*4, space.read_dword(cop_regs[0] + 16 + offset*4) - space.read_dword(cop_regs[0] + 0x28 + offset*4));
		break;
	}
	case 0x0905: // 194 288 088
		space.write_dword(cop_regs[0] + 16 + offset*4, space.read_dword(cop_regs[0] + 16 + offset*4) + space.read_dword(cop_regs[0] + 0x28 + offset*4));
		break;

	case 0x130e:
	case 0x138e: { // 130e 0005 bf7f 0010 - 0984 0aa4 0d82 0aa2 039b 0b9a 0b9a 0a9a
		int dx = space.read_dword(cop_regs[1]+4) - space.read_dword(cop_regs[0]+4);
		int dy = space.read_dword(cop_regs[1]+8) - space.read_dword(cop_regs[0]+8);

		if(!dy) {
			cop_status |= 0x8000;
			cop_angle = 0;
		} else {
			cop_angle = atan(double(dx)/double(dy)) * 128 / M_PI;
			if(dy<0)
				cop_angle += 0x80;
		}
		dx = dx >> 16;
		dy = dy >> 16;
		cop_dist = sqrt((double)(dx*dx+dy*dy));

		if(data & 0x0080) {
			space.write_byte(cop_regs[0]+0x34, cop_angle);
			space.write_word(cop_regs[0]+0x38, cop_dist);
		}
		break;
	}

	case 0x3bb0: { // 3bb0 0004 007f 0038 - 0f9c 0b9c 0b9c 0b9c 0b9c 0b9c 0b9c 099c
		// called systematically after 130e/138e, no results expected it seems
		break;
	}

	case 0x42c2: { // 42c2 0005 fcdd 0040 - 0f9a 0b9a 0b9c 0b9c 0b9c 029c 0000 0000
		int div = space.read_word(cop_regs[0]+0x36);
		if(!div)
			div = 1;
		space.write_word(cop_regs[0]+0x38, (space.read_word(cop_regs[0]+0x38) << (5-cop_scale)) / div);
		break;
	}

	case 0x8100: { // 8100 0007 fdfb 0080 - 0b9a 0b88 0888 0000 0000 0000 0000 0000
		int raw_angle = (space.read_word(cop_regs[0]+0x34) & 0xff);
		double angle = raw_angle * M_PI / 128;
		double amp = 65536*(space.read_word(cop_regs[0]+0x36) & 0xff);
		/* TODO: up direction, why? (check machine/seicop.c) */
		if(raw_angle == 0xc0)
			amp*=2;
		space.write_dword(cop_regs[0] + 16, int(amp*sin(angle)) >> (5-cop_scale));
		break;
	}

	case 0x8900: { // 8900 0007 fdfb 0088 - 0b9a 0b8a 088a 0000 0000 0000 0000 0000
		int raw_angle = (space.read_word(cop_regs[0]+0x34) & 0xff);
		double angle = raw_angle * M_PI / 128;
		double amp = 65536*(space.read_word(cop_regs[0]+0x36) & 0xff);
		/* TODO: left direction, why? (check machine/seicop.c) */
		if(raw_angle == 0x80)
			amp*=2;
		space.write_dword(cop_regs[0] + 20, int(amp*cos(angle)) >> (5-cop_scale));
		break;
	}

	case 0x5205:   // 5205 0006 fff7 0050 - 0180 02e0 03a0 00a0 03a0 0000 0000 0000
		//      fprintf(stderr, "sprcpt 5205 %04x %04x %04x %08x %08x\n", cop_regs[0], cop_regs[1], cop_regs[3], space.read_dword(cop_regs[0]), space.read_dword(cop_regs[3]));
		space.write_dword(cop_regs[1], space.read_dword(cop_regs[0]));
		break;

	case 0x5a05:   // 5a05 0006 fff7 0058 - 0180 02e0 03a0 00a0 03a0 0000 0000 0000
		//      fprintf(stderr, "sprcpt 5a05 %04x %04x %04x %08x %08x\n", cop_regs[0], cop_regs[1], cop_regs[3], space.read_dword(cop_regs[0]), space.read_dword(cop_regs[3]));
		space.write_dword(cop_regs[1], space.read_dword(cop_regs[0]));
		break;

	case 0xf205:   // f205 0006 fff7 00f0 - 0182 02e0 03c0 00c0 03c0 0000 0000 0000
		//      fprintf(stderr, "sprcpt f205 %04x %04x %04x %08x %08x\n", cop_regs[0]+4, cop_regs[1], cop_regs[3], space.read_dword(cop_regs[0]+4), space.read_dword(cop_regs[3]));
		space.write_dword(cop_regs[2], space.read_dword(cop_regs[0]+4));
		break;

		// raidndx only
	case 0x7e05:
		space.write_dword(0x470, (space.read_dword(cop_regs[4]) & 0x30) << 6);
		// Actually, wherever the bank selection actually is
		// And probably 8 bytes too, but they zero all the rest
		break;

	case 0xa100:
		cop_collision_info[0].y = (space.read_dword(cop_regs[0]+4));
		cop_collision_info[0].x = (space.read_dword(cop_regs[0]+8));
		break;

	case 0xa900:
		cop_collision_info[1].y = (space.read_dword(cop_regs[1]+4));
		cop_collision_info[1].x = (space.read_dword(cop_regs[1]+8));
		break;

	case 0xb100:
		/* Take hitbox param, TODO */
		cop_collision_info[0].hitbox = space.read_word(cop_regs[2]);

		cop_collision_info[0].min_x = cop_collision_info[0].x + (0 << 16);
		cop_collision_info[0].min_y = cop_collision_info[0].y + (0 << 16);
		cop_collision_info[0].max_x = cop_collision_info[0].x + (0x10 << 16);
		cop_collision_info[0].max_y = cop_collision_info[0].y + (0x10 << 16);

		/* do the math */
		cop_hit_status = cop_calculate_collsion_detection(space.machine());
		break;

	case 0xb900:
		/* Take hitbox param, TODO */
		cop_collision_info[1].hitbox = space.read_word(cop_regs[3]);

		cop_collision_info[1].min_x = cop_collision_info[1].x + (0 << 16);
		cop_collision_info[1].min_y = cop_collision_info[1].y + (0 << 16);
		cop_collision_info[1].max_x = cop_collision_info[1].x + (0x10 << 16);
		cop_collision_info[1].max_y = cop_collision_info[1].y + (0x10 << 16);

		/* do the math */
		cop_hit_status = cop_calculate_collsion_detection(space.machine());
		break;

	default:
		logerror("pcall %04x (%04x:%04x) [%x %x %x %x]\n", data, rps(space.machine()), rpc(space.machine()), cop_regs[0], cop_regs[1], cop_regs[2], cop_regs[3]);
	}
}

//  case 0x6ca:
//      logerror("select bank %d %04x\n", (data >> 15) & 1, data);
//      memory_set_bank(space.machine(), "bank1", (data >> 15) & 1);


static void combine32(UINT32 *val, int offset, UINT16 data, UINT16 mem_mask)
{
	UINT16 *dest = (UINT16 *)val + BYTE_XOR_LE(offset);
	COMBINE_DATA(dest);
}



/* SPRITE DRAWING (move to video file) */

void raiden2_state::draw_sprites(running_machine &machine, bitmap_ind16 &bitmap, const rectangle &cliprect ,int pri_mask )
{
	UINT16 *source = sprites + sprites_cur_start/2 - 4;

	const gfx_element *gfx = machine.gfx[2];

//  static int ytlim = 1;
//  static int xtlim = 1;

//  if ( machine.input().code_pressed_once(KEYCODE_Q) ) ytlim--;
//  if ( machine.input().code_pressed_once(KEYCODE_W) ) ytlim++;

//  if ( machine.input().code_pressed_once(KEYCODE_A) ) xtlim--;
//  if ( machine.input().code_pressed_once(KEYCODE_S) ) xtlim++;


	/*00 ???? ????  (colour / priority?)
      01 fhhh Fwww   h = height f=flipy w = width F = flipx
      02 nnnn nnnn   n = tileno
      03 nnnn nnnn   n = tile no
      04 xxxx xxxx   x = xpos
      05 xxxx xxxx   x = xpos
      06 yyyy yyyy   y = ypos
      07 yyyy yyyy   y = ypos

     */


	while( source>sprites ){
		int tile_number = source[1];
		int sx = source[2];
		int sy = source[3];
		int colr;
		int xtiles, ytiles;
		int ytlim, xtlim;
		int xflip, yflip;
		int xstep, ystep;

		if (sx & 0x8000) sx -= 0x10000;
		if (sy & 0x8000) sy -= 0x10000;


		ytlim = (source[0] >> 12) & 0x7;
		xtlim = (source[0] >> 8) & 0x7;

		xflip = (source[0] >> 15) & 0x1;
		yflip = (source[0] >> 11) & 0x1;

		colr = source[0] & 0x3f;

		ytlim += 1;
		xtlim += 1;

		xstep = 16;
		ystep = 16;

		if (xflip)
		{
			ystep = -16;
			sy += ytlim*16-16;
		}

		if (yflip)
		{
			xstep = -16;
			sx += xtlim*16-16;
		}

		for (xtiles = 0; xtiles < xtlim; xtiles++)
		{
			for (ytiles = 0; ytiles < ytlim; ytiles++)
			{
				drawgfx_transpen(
						bitmap,
						cliprect,
						gfx,
						tile_number,
						colr,
						yflip,xflip,
						sx+xstep*xtiles,sy+ystep*ytiles,15);

				tile_number++;
			}
		}

		source-=4;
	}

}

/* VIDEO RELATED WRITE HANDLERS (move to video file) */

WRITE16_MEMBER(raiden2_state::raiden2_background_w)
{
	COMBINE_DATA(&back_data[offset]);
	background_layer->mark_tile_dirty(offset);
}

WRITE16_MEMBER(raiden2_state::raiden2_midground_w)
{
	COMBINE_DATA(&mid_data[offset]);
	midground_layer->mark_tile_dirty(offset);
}

WRITE16_MEMBER(raiden2_state::raiden2_foreground_w)
{
	COMBINE_DATA(&fore_data[offset]);
	foreground_layer->mark_tile_dirty(offset);
}

WRITE16_MEMBER(raiden2_state::raiden2_text_w)
{
	COMBINE_DATA(&text_data[offset]);
	text_layer->mark_tile_dirty(offset);
}

WRITE16_MEMBER(raiden2_state::tilemap_enable_w)
{
	COMBINE_DATA(&raiden2_tilemap_enable);
}

WRITE16_MEMBER(raiden2_state::tile_scroll_w)
{
	COMBINE_DATA(scrollvals + offset);
	data = scrollvals[offset];

	tilemap_t *tm = 0;
	switch(offset/2) {
	case 0: tm = background_layer; break;
	case 1: tm = midground_layer; break;
	case 2: tm = foreground_layer; break;
	}
	if(offset & 1)
		tm->set_scrolly(0, data);
	else
		tm->set_scrollx(0, data);
}

WRITE16_MEMBER(raiden2_state::tile_bank_01_w)
{
	if(ACCESSING_BITS_0_7) {
		int new_bank;
		new_bank = 0 | ((data & 1)<<1);
		if(new_bank != bg_bank) {
			bg_bank = new_bank;
			background_layer->mark_all_dirty();
		}

		new_bank = 1 | (data & 2);
		if(new_bank != mid_bank) {
			mid_bank = new_bank;
			midground_layer->mark_all_dirty();
		}
	}
}

READ16_MEMBER(raiden2_state::cop_tile_bank_2_r)
{
	return cop_bank;
}

WRITE16_MEMBER(raiden2_state::cop_tile_bank_2_w)
{
	COMBINE_DATA(&cop_bank);

	if(ACCESSING_BITS_8_15) {
		int new_bank = 4 | (data >> 14);
		if(new_bank != fg_bank) {
			fg_bank = new_bank;
			foreground_layer->mark_all_dirty();
		}
	}
}

WRITE16_MEMBER(raiden2_state::raidendx_cop_bank_2_w)
{
	COMBINE_DATA(&cop_bank);

	if(ACCESSING_BITS_8_15) {
		int new_bank = 4 | ((cop_bank >> 10) & 3);
		if(new_bank != fg_bank) {
			fg_bank = new_bank;
			foreground_layer->mark_all_dirty();
		}

		/* probably bit 3 is from 6c9 */
		/* TODO: this doesn't work! */
		memory_set_bank(space.machine(), "mainbank", 8 | (cop_bank & 0x7000) >> 12);
	}
}



/* TILEMAP RELATED (move to video file) */

static TILE_GET_INFO( get_back_tile_info )
{
	raiden2_state *state = machine.driver_data<raiden2_state>();
	int tile = state->back_data[tile_index];
	int color = (tile >> 12) | (0 << 4);

	tile = (tile & 0xfff) | (state->bg_bank << 12);

	SET_TILE_INFO(1,tile+0x0000,color,0);
}

static TILE_GET_INFO( get_mid_tile_info )
{
	raiden2_state *state = machine.driver_data<raiden2_state>();
	int tile = state->mid_data[tile_index];
	int color = (tile >> 12) | (2 << 4);

	tile = (tile & 0xfff) | (state->mid_bank << 12);

	SET_TILE_INFO(1,tile,color,0);
}

static TILE_GET_INFO( get_fore_tile_info )
{
	raiden2_state *state = machine.driver_data<raiden2_state>();
	int tile = state->fore_data[tile_index];
	int color = (tile >> 12) | (1 << 4);

	tile = (tile & 0xfff) | (state->fg_bank << 12);

	SET_TILE_INFO(1,tile,color,0);
}

static TILE_GET_INFO( get_text_tile_info )
{
	raiden2_state *state = machine.driver_data<raiden2_state>();
	int tile = state->text_data[tile_index];
	int color = (tile>>12)&0xf;

	tile &= 0xfff;

	SET_TILE_INFO(0,tile,color,0);
}

/* VIDEO START (move to video file) */


static VIDEO_START( raiden2 )
{
	raiden2_state *state = machine.driver_data<raiden2_state>();
	state->text_layer       = tilemap_create(machine, get_text_tile_info, tilemap_scan_rows,  8, 8, 64,32 );
	state->background_layer = tilemap_create(machine, get_back_tile_info, tilemap_scan_rows, 16,16, 32,32 );
	state->midground_layer  = tilemap_create(machine, get_mid_tile_info,  tilemap_scan_rows, 16,16, 32,32 );
	state->foreground_layer = tilemap_create(machine, get_fore_tile_info, tilemap_scan_rows, 16,16, 32,32 );

	state->midground_layer->set_transparent_pen(15);
	state->foreground_layer->set_transparent_pen(15);
	state->text_layer->set_transparent_pen(15);
}

/* SCREEN_UPDATE_IND16 (move to video file) */

static SCREEN_UPDATE_IND16( raiden2 )
{
	raiden2_state *state = screen.machine().driver_data<raiden2_state>();
	bitmap.fill(get_black_pen(screen.machine()), cliprect);

	//if (!screen.machine().input().code_pressed(KEYCODE_Q))
	{
		if (!(state->raiden2_tilemap_enable & 1))
			state->background_layer->draw(bitmap, cliprect, 0, 0);
	}

	//if (!screen.machine().input().code_pressed(KEYCODE_W))
	{
		if (!(state->raiden2_tilemap_enable & 2))
			state->midground_layer->draw(bitmap, cliprect, 0, 0);
	}

	//if (!screen.machine().input().code_pressed(KEYCODE_E))
	{
		if (!(state->raiden2_tilemap_enable & 4))
			state->foreground_layer->draw(bitmap, cliprect, 0, 0);
	}

	//if (!screen.machine().input().code_pressed(KEYCODE_S))
	{
		//if (!(raiden2_tilemap_enable & 0x10))
			state->draw_sprites(screen.machine(), bitmap, cliprect, 0);
	}

	//if (!screen.machine().input().code_pressed(KEYCODE_A))
	{
		if (!(state->raiden2_tilemap_enable & 8))
			state->text_layer->draw(bitmap, cliprect, 0, 0);
	}

	return 0;
}




/*************************************
 *
 *  Interrupts
 *
 *************************************/

static INTERRUPT_GEN( raiden2_interrupt )
{
	device_set_input_line_and_vector(device, 0, HOLD_LINE, 0xc0/4);	/* VBL */
}




// Sprite encryption key upload

static UINT32 sprcpt_adr, sprcpt_idx;

static UINT16 sprcpt_flags2;
static UINT32 sprcpt_val[2], sprcpt_flags1;
static UINT32 sprcpt_data_1[0x100], sprcpt_data_2[0x40], sprcpt_data_3[6], sprcpt_data_4[4];

static void sprcpt_init(void)
{
	memset(sprcpt_data_1, 0, sizeof(sprcpt_data_1));
	memset(sprcpt_data_2, 0, sizeof(sprcpt_data_2));
	memset(sprcpt_data_3, 0, sizeof(sprcpt_data_3));
	memset(sprcpt_data_4, 0, sizeof(sprcpt_data_4));

	sprcpt_adr = 0;
	sprcpt_idx = 0;
}


WRITE16_MEMBER(raiden2_state::sprcpt_adr_w)
{
	combine32(&sprcpt_adr, offset, data, mem_mask);
}

WRITE16_MEMBER(raiden2_state::sprcpt_data_1_w)
{
	combine32(sprcpt_data_1+sprcpt_adr, offset, data, mem_mask);
}

WRITE16_MEMBER(raiden2_state::sprcpt_data_2_w)
{
	combine32(sprcpt_data_2+sprcpt_adr, offset, data, mem_mask);
}

WRITE16_MEMBER(raiden2_state::sprcpt_data_3_w)
{
	combine32(sprcpt_data_3+sprcpt_idx, offset, data, mem_mask);
	if(offset == 1) {
		sprcpt_idx ++;
		if(sprcpt_idx == 6)
			sprcpt_idx = 0;
	}
}

WRITE16_MEMBER(raiden2_state::sprcpt_data_4_w)
{
	combine32(sprcpt_data_4+sprcpt_idx, offset, data, mem_mask);
	if(offset == 1) {
		sprcpt_idx ++;
		if(sprcpt_idx == 4)
			sprcpt_idx = 0;
	}
}

WRITE16_MEMBER(raiden2_state::sprcpt_val_1_w)
{
	combine32(sprcpt_val+0, offset, data, mem_mask);
}

WRITE16_MEMBER(raiden2_state::sprcpt_val_2_w)
{
	combine32(sprcpt_val+1, offset, data, mem_mask);
}

WRITE16_MEMBER(raiden2_state::sprcpt_flags_1_w)
{
	combine32(&sprcpt_flags1, offset, data, mem_mask);
	if(offset == 1) {
		// bit 31: 1 = allow write on sprcpt data

		if(!(sprcpt_flags1 & 0x80000000U)) {
			// Upload finished
			if(1) {
				int i;
				logerror("sprcpt_val 1: %08x\n", sprcpt_val[0]);
				logerror("sprcpt_val 2: %08x\n", sprcpt_val[1]);
				logerror("sprcpt_data 1:\n");
				for(i=0; i<0x100; i++) {
					logerror(" %08x", sprcpt_data_1[i]);
					if(!((i+1) & 7))
						logerror("\n");
				}
				logerror("sprcpt_data 2:\n");
				for(i=0; i<0x40; i++) {
					logerror(" %08x", sprcpt_data_2[i]);
					if(!((i+1) & 7))
						logerror("\n");
				}
			}
		}
	}
}

WRITE16_MEMBER(raiden2_state::sprcpt_flags_2_w)
{
	COMBINE_DATA(&sprcpt_flags2);
	if(offset == 0) {
		if(sprcpt_flags2 & 0x8000) {
			// Reset decryption -> redo it
		}
	}
}



void raiden2_state::common_reset()
{
	bg_bank=0;
	fg_bank=6;
	mid_bank=1;
	cop_itoa_digit_count = 4; //TODO: Raiden 2 never inits the BCD register, value here is a guess (8 digits, as WR is 10.000.000 + a)
}

static MACHINE_RESET(raiden2)
{
	raiden2_state *state = machine.driver_data<raiden2_state>();
	state->common_reset();
	sprcpt_init();
	MACHINE_RESET_CALL(seibu_sound);

	memory_set_bank(machine, "mainbank", 1);

	state->prg_bank = 0;
	//cop_init();
}

static MACHINE_RESET(raidendx)
{
	raiden2_state *state = machine.driver_data<raiden2_state>();
	state->common_reset();
	sprcpt_init();
	MACHINE_RESET_CALL(seibu_sound);

	memory_set_bank(machine, "mainbank", 8);

	state->prg_bank = 0x08;

	//cop_init();
}

static MACHINE_RESET(zeroteam)
{
	raiden2_state *state = machine.driver_data<raiden2_state>();
	state->bg_bank = 0;
	state->fg_bank = 2;
	state->mid_bank = 1;
	sprcpt_init();
	MACHINE_RESET_CALL(seibu_sound);

	memory_set_bank(machine, "mainbank", 1);

	state->prg_bank = 0;
	//cop_init();
}

static MACHINE_RESET(xsedae)
{
	raiden2_state *state = machine.driver_data<raiden2_state>();
	state->bg_bank = 0;
	state->fg_bank = 2;
	state->mid_bank = 1;
	sprcpt_init();
	MACHINE_RESET_CALL(seibu_sound);

	//memory_set_bank(machine, "mainbank", 1);

	//cop_init();
}

READ16_MEMBER(raiden2_state::raiden2_sound_comms_r)
{
	return seibu_main_word_r(&space,(offset >> 1) & 7,0xffff);
}

WRITE16_MEMBER(raiden2_state::raiden2_sound_comms_w)
{
	seibu_main_word_w(&space,(offset >> 1) & 7,data,0x00ff);
}

WRITE16_MEMBER(raiden2_state::raiden2_bank_w)
{
	if(ACCESSING_BITS_8_15) {
		logerror("select bank %d %04x\n", (data >> 15) & 1, data);
		memory_set_bank(space.machine(), "mainbank", !((data >> 15) & 1));
		prg_bank = ((data >> 15) & 1);
	}
}

READ16_MEMBER(raiden2_state::cop_collision_status_r)
{
	return cop_hit_status;
}

WRITE16_MEMBER(raiden2_state::sprite_prot_x_w)
{
	sprite_prot_x = data;
	popmessage("%04x %04x",sprite_prot_x,sprite_prot_y);
}

WRITE16_MEMBER(raiden2_state::sprite_prot_y_w)
{
	sprite_prot_y = data;
	popmessage("%04x %04x",sprite_prot_x,sprite_prot_y);
}

WRITE16_MEMBER(raiden2_state::sprite_prot_src_seg_w)
{
	sprite_prot_src_addr[0] = data;
}

WRITE16_MEMBER(raiden2_state::sprite_prot_src_w)
{
	int dx;
	int dy;
	UINT32 src;

	sprite_prot_src_addr[1] = data;
	src = (sprite_prot_src_addr[0]<<4)+sprite_prot_src_addr[1];

	dx = ((space.read_dword(src+0x08) >> 16) - (sprite_prot_x)) & 0xffff;
	dy = ((space.read_dword(src+0x04) >> 16) - (sprite_prot_y)) & 0xffff;

	space.write_word(src,(dx < 0x140 && dy < 256) ? 0x0001 : 0x0000);

	//printf("[%08x] %08x %08x %04x %04x\n",src,dx,dy,dst1,dst2);
}

READ16_MEMBER(raiden2_state::sprite_prot_dst1_r)
{
	return dst1;
}

READ16_MEMBER(raiden2_state::sprite_prot_dst2_r)
{
	return dst2;
}

WRITE16_MEMBER(raiden2_state::sprite_prot_dst1_w)
{
	dst1 = data;
}

WRITE16_MEMBER(raiden2_state::sprite_prot_dst2_w)
{
	dst2 = data;
}

/* MEMORY MAPS */
static ADDRESS_MAP_START( raiden2_cop_mem, AS_PROGRAM, 16, raiden2_state )
//  AM_RANGE(0x0041c, 0x0041d) AM_WRITENOP // angle compare (for 0x6200 COP macro)
//  AM_RANGE(0x0041e, 0x0041f) AM_WRITENOP // angle mod value (for 0x6200 COP macro)
	AM_RANGE(0x00420, 0x00421) AM_WRITE(cop_itoa_low_w)
	AM_RANGE(0x00422, 0x00423) AM_WRITE(cop_itoa_high_w)
	AM_RANGE(0x00424, 0x00425) AM_WRITE(cop_itoa_digit_count_w)
	AM_RANGE(0x00428, 0x00429) AM_WRITE(cop_dma_v1_w)
	AM_RANGE(0x0042a, 0x0042b) AM_WRITE(cop_dma_v2_w)
	AM_RANGE(0x00432, 0x00433) AM_WRITE(cop_pgm_data_w)
	AM_RANGE(0x00434, 0x00435) AM_WRITE(cop_pgm_addr_w)
	AM_RANGE(0x00438, 0x00439) AM_WRITE(cop_pgm_value_w)
	AM_RANGE(0x0043a, 0x0043b) AM_WRITE(cop_pgm_mask_w)
	AM_RANGE(0x0043c, 0x0043d) AM_WRITE(cop_pgm_trigger_w)
	AM_RANGE(0x00444, 0x00445) AM_WRITE(cop_scale_w)
	AM_RANGE(0x00450, 0x00459) AM_WRITENOP //sort-DMA params, Zero Team uses it
	AM_RANGE(0x0045a, 0x0045b) AM_WRITENOP //palette DMA brightness val, used by X Se Dae / Zero Team
	AM_RANGE(0x0045c, 0x0045d) AM_WRITENOP //palette DMA brightness mode, used by X Se Dae / Zero Team (sets to 5)
	AM_RANGE(0x00470, 0x00471) AM_READWRITE(cop_tile_bank_2_r,cop_tile_bank_2_w)

	AM_RANGE(0x00476, 0x00477) AM_WRITE(cop_dma_adr_rel_w)
	AM_RANGE(0x00478, 0x00479) AM_WRITE(cop_dma_src_w)
	AM_RANGE(0x0047a, 0x0047b) AM_WRITE(cop_dma_size_w)
	AM_RANGE(0x0047c, 0x0047d) AM_WRITE(cop_dma_dst_w)
	AM_RANGE(0x0047e, 0x0047f) AM_READWRITE(cop_dma_mode_r, cop_dma_mode_w)
	AM_RANGE(0x004a0, 0x004a9) AM_READWRITE(cop_reg_high_r, cop_reg_high_w)
	AM_RANGE(0x004c0, 0x004c9) AM_READWRITE(cop_reg_low_r, cop_reg_low_w)
	AM_RANGE(0x00500, 0x00505) AM_WRITE(cop_cmd_w)
	AM_RANGE(0x00580, 0x00581) AM_READ(cop_collision_status_r)
//  AM_RANGE(0x00588, 0x00589) AM_READ(cop_collision_status2_r) // used by Zero Team (only this, why?)
	AM_RANGE(0x00590, 0x00599) AM_READ(cop_itoa_digits_r)
	AM_RANGE(0x005b0, 0x005b1) AM_READ(cop_status_r)
	AM_RANGE(0x005b2, 0x005b3) AM_READ(cop_dist_r)
	AM_RANGE(0x005b4, 0x005b5) AM_READ(cop_angle_r)

	AM_RANGE(0x0061c, 0x0061d) AM_WRITE(tilemap_enable_w)
	AM_RANGE(0x00620, 0x0062b) AM_WRITE(tile_scroll_w)
	AM_RANGE(0x006a0, 0x006a3) AM_WRITE(sprcpt_val_1_w)
	AM_RANGE(0x006a4, 0x006a7) AM_WRITE(sprcpt_data_3_w)
	AM_RANGE(0x006a8, 0x006ab) AM_WRITE(sprcpt_data_4_w)
	AM_RANGE(0x006ac, 0x006af) AM_WRITE(sprcpt_flags_1_w)
	AM_RANGE(0x006b0, 0x006b3) AM_WRITE(sprcpt_data_1_w)
	AM_RANGE(0x006b4, 0x006b7) AM_WRITE(sprcpt_data_2_w)
	AM_RANGE(0x006b8, 0x006bb) AM_WRITE(sprcpt_val_2_w)
	AM_RANGE(0x006bc, 0x006bf) AM_WRITE(sprcpt_adr_w)
	AM_RANGE(0x006c2, 0x006c3) AM_WRITE(sprite_prot_src_seg_w)
	AM_RANGE(0x006c6, 0x006c7) AM_WRITE(sprite_prot_dst1_w)
	AM_RANGE(0x006ca, 0x006cb) AM_WRITE(raiden2_bank_w)
	AM_RANGE(0x006cc, 0x006cd) AM_WRITE(tile_bank_01_w)
	AM_RANGE(0x006ce, 0x006cf) AM_WRITE(sprcpt_flags_2_w)
	AM_RANGE(0x006d8, 0x006d9) AM_WRITE(sprite_prot_x_w)
	AM_RANGE(0x006da, 0x006db) AM_WRITE(sprite_prot_y_w)
	AM_RANGE(0x006dc, 0x006dd) AM_READ(sprite_prot_dst2_r) AM_WRITE(sprite_prot_dst2_w)
	AM_RANGE(0x006de, 0x006df) AM_WRITE(sprite_prot_src_w)
	AM_RANGE(0x006fc, 0x006fd) AM_WRITE(cop_dma_trigger_w)
	AM_RANGE(0x006fe, 0x006ff) AM_WRITENOP // sort-DMA trigger

	AM_RANGE(0x00762, 0x00763) AM_READ(sprite_prot_dst1_r)
ADDRESS_MAP_END

static ADDRESS_MAP_START( raiden2_mem, AS_PROGRAM, 16, raiden2_state )
	AM_RANGE(0x00000, 0x003ff) AM_RAM

	AM_RANGE(0x0068e, 0x0068f) AM_WRITENOP //irq ack / sprite buffering?

	AM_IMPORT_FROM( raiden2_cop_mem )

	AM_RANGE(0x00700, 0x0071f) AM_READWRITE(raiden2_sound_comms_r,raiden2_sound_comms_w)

	AM_RANGE(0x00740, 0x00741) AM_READ_PORT("DSW")
	AM_RANGE(0x00744, 0x00745) AM_READ_PORT("P1_P2")
	AM_RANGE(0x0074c, 0x0074d) AM_READ_PORT("SYSTEM")

	AM_RANGE(0x00800, 0x0bfff) AM_RAM

	AM_RANGE(0x0c000, 0x0cfff) AM_RAM AM_BASE(sprites)
	AM_RANGE(0x0d000, 0x0d7ff) AM_RAM_WRITE(raiden2_background_w) AM_BASE(back_data)
	AM_RANGE(0x0d800, 0x0dfff) AM_RAM_WRITE(raiden2_foreground_w) AM_BASE(fore_data)
    AM_RANGE(0x0e000, 0x0e7ff) AM_RAM_WRITE(raiden2_midground_w)  AM_BASE(mid_data)
    AM_RANGE(0x0e800, 0x0f7ff) AM_RAM_WRITE(raiden2_text_w) AM_BASE(text_data)
	AM_RANGE(0x0f800, 0x0ffff) AM_RAM /* Stack area */

	AM_RANGE(0x10000, 0x1efff) AM_RAM
	AM_RANGE(0x1f000, 0x1ffff) AM_RAM AM_WRITE_LEGACY(paletteram16_xBBBBBGGGGGRRRRR_word_w) AM_BASE_GENERIC(paletteram)

	AM_RANGE(0x20000, 0x3ffff) AM_ROMBANK("mainbank")
	AM_RANGE(0x40000, 0xfffff) AM_ROM AM_REGION("mainprg", 0x40000)
ADDRESS_MAP_END

static ADDRESS_MAP_START( raidendx_mem, AS_PROGRAM, 16, raiden2_state )
	AM_RANGE(0x00470, 0x00471) AM_READWRITE(cop_tile_bank_2_r,raidendx_cop_bank_2_w)
	AM_RANGE(0x004d0, 0x004d7) AM_RAM //???
	AM_RANGE(0x0062c, 0x0062d) AM_WRITE(tilemap_enable_w)
	AM_RANGE(0x00610, 0x0061b) AM_WRITE(tile_scroll_w)
//  AM_RANGE(0x006ca, 0x006cb) AM_WRITENOP
	AM_IMPORT_FROM( raiden2_mem )
ADDRESS_MAP_END

static ADDRESS_MAP_START( zeroteam_mem, AS_PROGRAM, 16, raiden2_state )
	AM_RANGE(0x00000, 0x003ff) AM_RAM

	AM_RANGE(0x00470, 0x00471) AM_WRITENOP
	AM_RANGE(0x006cc, 0x006cd) AM_WRITENOP

	AM_RANGE(0x0068e, 0x0068f) AM_WRITENOP // irq ack / sprite buffering?

	AM_IMPORT_FROM( raiden2_cop_mem )

	AM_RANGE(0x00700, 0x0071f) AM_READWRITE(raiden2_sound_comms_r,raiden2_sound_comms_w)

	AM_RANGE(0x00740, 0x00741) AM_READ_PORT("DSW")
	AM_RANGE(0x00744, 0x00745) AM_READ_PORT("P1_P2")
	AM_RANGE(0x00748, 0x00749) AM_READ_PORT("P3_P4")
	AM_RANGE(0x0074c, 0x0074d) AM_READ_PORT("SYSTEM")

	AM_RANGE(0x00800, 0x0b7ff) AM_RAM
	AM_RANGE(0x0b800, 0x0bfff) AM_RAM_WRITE(raiden2_background_w) AM_BASE(back_data)
	AM_RANGE(0x0c000, 0x0c7ff) AM_RAM_WRITE(raiden2_foreground_w) AM_BASE(fore_data)
	AM_RANGE(0x0c800, 0x0cfff) AM_RAM_WRITE(raiden2_midground_w) AM_BASE(mid_data)
    AM_RANGE(0x0d000, 0x0dfff) AM_RAM_WRITE(raiden2_text_w) AM_BASE(text_data)
	AM_RANGE(0x0e000, 0x0efff) AM_RAM AM_WRITE_LEGACY(paletteram16_xBBBBBGGGGGRRRRR_word_w) AM_BASE_GENERIC(paletteram)
	AM_RANGE(0x0f000, 0x0ffff) AM_RAM AM_BASE(sprites)
	AM_RANGE(0x10000, 0x1ffff) AM_RAM

	AM_RANGE(0x20000, 0x3ffff) AM_ROMBANK("mainbank")
	AM_RANGE(0x40000, 0xfffff) AM_ROM AM_REGION("mainprg", 0x40000)
ADDRESS_MAP_END

static ADDRESS_MAP_START( xsedae_mem, AS_PROGRAM, 16, raiden2_state )
	AM_RANGE(0x00000, 0x003ff) AM_RAM

	AM_RANGE(0x00470, 0x00471) AM_WRITENOP
	AM_RANGE(0x006cc, 0x006cd) AM_WRITENOP

	AM_RANGE(0x0068e, 0x0068f) AM_WRITENOP //irq ack / sprite buffering?

	AM_IMPORT_FROM( raiden2_cop_mem )

	AM_RANGE(0x00700, 0x0071f) AM_READWRITE(raiden2_sound_comms_r,raiden2_sound_comms_w)

	AM_RANGE(0x00740, 0x00741) AM_READ_PORT("DSW")
	AM_RANGE(0x00744, 0x00745) AM_READ_PORT("P1_P2")
	AM_RANGE(0x00748, 0x00749) AM_READ_PORT("P3_P4")
	AM_RANGE(0x0074c, 0x0074d) AM_READ_PORT("SYSTEM")

	AM_RANGE(0x00800, 0x0b7ff) AM_RAM
	AM_RANGE(0x0b800, 0x0bfff) AM_RAM_WRITE(raiden2_background_w) AM_BASE(back_data)
	AM_RANGE(0x0c000, 0x0c7ff) AM_RAM_WRITE(raiden2_foreground_w) AM_BASE(fore_data)
	AM_RANGE(0x0c800, 0x0cfff) AM_RAM_WRITE(raiden2_midground_w) AM_BASE(mid_data)
    AM_RANGE(0x0d000, 0x0dfff) AM_RAM_WRITE(raiden2_text_w) AM_BASE(text_data)
	AM_RANGE(0x0e000, 0x0efff) AM_RAM AM_WRITE_LEGACY(paletteram16_xBBBBBGGGGGRRRRR_word_w) AM_BASE_GENERIC(paletteram)
	AM_RANGE(0x0f000, 0x0ffff) AM_RAM AM_BASE(sprites)

	AM_RANGE(0x10000, 0x1ffff) AM_RAM

	AM_RANGE(0x20000, 0xfffff) AM_ROM AM_REGION("mainprg", 0x20000)
ADDRESS_MAP_END


/* INPUT PORTS */

static INPUT_PORTS_START( raiden2 )
	SEIBU_COIN_INPUTS	/* coin inputs read through sound cpu */

	PORT_START("P1_P2")	/* IN0/1 */
	PORT_BIT( 0x0001, IP_ACTIVE_LOW, IPT_JOYSTICK_UP ) PORT_8WAY PORT_PLAYER(1)
	PORT_BIT( 0x0002, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN ) PORT_8WAY PORT_PLAYER(1)
	PORT_BIT( 0x0004, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT ) PORT_8WAY PORT_PLAYER(1)
	PORT_BIT( 0x0008, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT ) PORT_8WAY PORT_PLAYER(1)
	PORT_BIT( 0x0010, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_PLAYER(1)
	PORT_BIT( 0x0020, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_PLAYER(1)
	PORT_BIT( 0x0040, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT( 0x0080, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT( 0x0100, IP_ACTIVE_LOW, IPT_JOYSTICK_UP ) PORT_8WAY PORT_PLAYER(2)
	PORT_BIT( 0x0200, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN ) PORT_8WAY PORT_PLAYER(2)
	PORT_BIT( 0x0400, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT ) PORT_8WAY PORT_PLAYER(2)
	PORT_BIT( 0x0800, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT ) PORT_8WAY PORT_PLAYER(2)
	PORT_BIT( 0x1000, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_PLAYER(2)
	PORT_BIT( 0x2000, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_PLAYER(2)
	PORT_BIT( 0x4000, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT( 0x8000, IP_ACTIVE_LOW, IPT_UNUSED )

	PORT_START("DSW")	/* Dip switches  */
	PORT_DIPNAME( 0x0007, 0x0007, DEF_STR( Coin_A ) ) PORT_DIPLOCATION("SW1:!1,!2,!3")
	PORT_DIPSETTING(      0x0001, DEF_STR( 4C_1C ) )
	PORT_DIPSETTING(      0x0002, DEF_STR( 3C_1C ) )
	PORT_DIPSETTING(      0x0004, DEF_STR( 2C_1C ) )
	PORT_DIPSETTING(      0x0007, DEF_STR( 1C_1C ) )
	PORT_DIPSETTING(      0x0006, DEF_STR( 1C_2C ) )
	PORT_DIPSETTING(      0x0005, DEF_STR( 1C_3C ) )
	PORT_DIPSETTING(      0x0003, DEF_STR( 1C_4C ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( Free_Play ) )
	PORT_DIPNAME( 0x0038, 0x0038, DEF_STR( Coin_B ) ) PORT_DIPLOCATION("SW1:!4,!5,!6")
	PORT_DIPSETTING(      0x0008, DEF_STR( 4C_1C ) )
	PORT_DIPSETTING(      0x0010, DEF_STR( 3C_1C ) )
	PORT_DIPSETTING(      0x0020, DEF_STR( 2C_1C ) )
	PORT_DIPSETTING(      0x0038, DEF_STR( 1C_1C ) )
	PORT_DIPSETTING(      0x0030, DEF_STR( 1C_2C ) )
	PORT_DIPSETTING(      0x0028, DEF_STR( 1C_3C ) )
	PORT_DIPSETTING(      0x0018, DEF_STR( 1C_4C ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( Free_Play ) )
	PORT_DIPNAME( 0x0040, 0x0040, "Starting Coin" ) PORT_DIPLOCATION("SW1:!7")
	PORT_DIPSETTING(      0x0040, DEF_STR( Normal ) )
	PORT_DIPSETTING(      0x0000, "X 2" )
	PORT_DIPNAME( 0x0080, 0x0080, DEF_STR( Flip_Screen ) ) PORT_DIPLOCATION("SW1:!8")
	PORT_DIPSETTING(      0x0080, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( On ) )
	PORT_DIPNAME( 0x0300, 0x0300, DEF_STR( Difficulty ) ) PORT_DIPLOCATION("SW2:!1,!2")
	PORT_DIPSETTING(      0x0300, DEF_STR( Normal ) )
	PORT_DIPSETTING(      0x0200, DEF_STR( Easy ) ) // dipsw sheets say this is hard but service mode says easy
	PORT_DIPSETTING(      0x0100, DEF_STR( Hard ) ) // vice versa of above
	PORT_DIPSETTING(      0x0000, DEF_STR( Very_Hard ) )
	PORT_DIPNAME( 0x0c00, 0x0c00, DEF_STR( Lives ) ) PORT_DIPLOCATION("SW2:!3,!4")
	PORT_DIPSETTING(      0x0000, "1" )
	PORT_DIPSETTING(      0x0400, "4" )
	PORT_DIPSETTING(      0x0800, "2" )
	PORT_DIPSETTING(      0x0c00, "3" )
	PORT_DIPNAME( 0x3000, 0x3000, DEF_STR( Bonus_Life ) ) PORT_DIPLOCATION("SW2:!5,!6")
	PORT_DIPSETTING(      0x3000, "200000 500000" )
	PORT_DIPSETTING(      0x2000, "400000 1000000" )
	PORT_DIPSETTING(      0x1000, "1000000 3000000" )
	PORT_DIPSETTING(      0x0000, "No Extend" )
	PORT_DIPNAME( 0x4000, 0x4000, "Demo Sound" ) PORT_DIPLOCATION("SW2:!7")
	PORT_DIPSETTING(      0x0000, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x4000, DEF_STR( On ) )
	PORT_SERVICE( 0x8000, IP_ACTIVE_LOW ) PORT_DIPLOCATION("SW2:!8") /* Test Mode */

	PORT_START("SYSTEM")	/* START BUTTONS */
	PORT_BIT( 0x0001, IP_ACTIVE_LOW, IPT_START1 )
	PORT_BIT( 0x0002, IP_ACTIVE_LOW, IPT_START2 )
	PORT_BIT( 0x0004, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT( 0x0008, IP_ACTIVE_LOW, IPT_SERVICE1 )
	PORT_BIT( 0xfff0, IP_ACTIVE_LOW, IPT_UNUSED )
INPUT_PORTS_END

static INPUT_PORTS_START( raidendx )
	PORT_INCLUDE( raiden2 )

	PORT_MODIFY("DSW")	/* Dip switches  */
	PORT_DIPNAME( 0x1000, 0x1000, DEF_STR( Unknown ) ) PORT_DIPLOCATION("SW2:!5") /* Manual shows "Not Used" */
	PORT_DIPSETTING(      0x1000, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( On ) )
	PORT_DIPNAME( 0x2000, 0x2000, DEF_STR( Unknown ) ) PORT_DIPLOCATION("SW2:!6") /* Manual shows "Not Used" */
	PORT_DIPSETTING(      0x2000, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( On ) )

INPUT_PORTS_END

static INPUT_PORTS_START( zeroteam )
	PORT_INCLUDE( raiden2 )

	PORT_MODIFY("P1_P2")
	PORT_BIT( 0x0040, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_PLAYER(1)
	PORT_BIT( 0x4000, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_PLAYER(2)

	PORT_START("P3_P4")
	PORT_BIT( 0x0001, IP_ACTIVE_LOW, IPT_JOYSTICK_UP ) PORT_8WAY PORT_PLAYER(3)
	PORT_BIT( 0x0002, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN ) PORT_8WAY PORT_PLAYER(3)
	PORT_BIT( 0x0004, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT ) PORT_8WAY PORT_PLAYER(3)
	PORT_BIT( 0x0008, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT ) PORT_8WAY PORT_PLAYER(3)
	PORT_BIT( 0x0010, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_PLAYER(3)
	PORT_BIT( 0x0020, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_PLAYER(3)
	PORT_BIT( 0x0040, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_PLAYER(3)
	PORT_BIT( 0x0080, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT( 0x0100, IP_ACTIVE_LOW, IPT_JOYSTICK_UP ) PORT_8WAY PORT_PLAYER(4)
	PORT_BIT( 0x0200, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN ) PORT_8WAY PORT_PLAYER(4)
	PORT_BIT( 0x0400, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT ) PORT_8WAY PORT_PLAYER(4)
	PORT_BIT( 0x0800, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT ) PORT_8WAY PORT_PLAYER(4)
	PORT_BIT( 0x1000, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_PLAYER(4)
	PORT_BIT( 0x2000, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_PLAYER(4)
	PORT_BIT( 0x4000, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_PLAYER(4)
	PORT_BIT( 0x8000, IP_ACTIVE_LOW, IPT_UNUSED )

	PORT_MODIFY("DSW") // not the same as raiden2/dx: coins, difficulty, lives and bonus lives all differ!
	PORT_DIPNAME( 0x0007, 0x0007, DEF_STR( Coin_A ) ) PORT_DIPLOCATION("SW1:!1,!2,!3")
	PORT_DIPSETTING(      0x0000, DEF_STR( 4C_1C ) )
	PORT_DIPSETTING(      0x0001, DEF_STR( 3C_1C ) )
	PORT_DIPSETTING(      0x0002, DEF_STR( 2C_1C ) )
	PORT_DIPSETTING(      0x0007, DEF_STR( 1C_1C ) )
	PORT_DIPSETTING(      0x0006, DEF_STR( 1C_2C ) )
	PORT_DIPSETTING(      0x0005, DEF_STR( 1C_3C ) )
	PORT_DIPSETTING(      0x0003, DEF_STR( 1C_4C ) )
	PORT_DIPSETTING(      0x0004, DEF_STR( 1C_6C ) )
	PORT_DIPNAME( 0x0038, 0x0038, DEF_STR( Coin_B ) ) PORT_DIPLOCATION("SW1:!4,!5,!6")
	PORT_DIPSETTING(      0x0000, DEF_STR( 4C_1C ) )
	PORT_DIPSETTING(      0x0008, DEF_STR( 3C_1C ) )
	PORT_DIPSETTING(      0x0010, DEF_STR( 2C_1C ) )
	PORT_DIPSETTING(      0x0038, DEF_STR( 1C_1C ) )
	PORT_DIPSETTING(      0x0030, DEF_STR( 1C_2C ) )
	PORT_DIPSETTING(      0x0028, DEF_STR( 1C_3C ) )
	PORT_DIPSETTING(      0x0018, DEF_STR( 1C_4C ) )
	PORT_DIPSETTING(      0x0020, DEF_STR( 1C_6C ) )
	PORT_DIPNAME( 0x0040, 0x0040, "Starting Coin" ) PORT_DIPLOCATION("SW1:!7")
	PORT_DIPSETTING(      0x0040, DEF_STR( Normal ) )
	PORT_DIPSETTING(      0x0000, "X 2" )
	PORT_DIPNAME( 0x0080, 0x0080, DEF_STR( Flip_Screen ) ) PORT_DIPLOCATION("SW1:!8")
	PORT_DIPSETTING(      0x0080, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( On ) )
	PORT_DIPNAME( 0x0300, 0x0300, DEF_STR( Difficulty ) ) PORT_DIPLOCATION("SW2:!1,!2")
	PORT_DIPSETTING(      0x0300, DEF_STR( Normal ) )
	PORT_DIPSETTING(      0x0200, DEF_STR( Hard ) )
	PORT_DIPSETTING(      0x0100, DEF_STR( Easy ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( Very_Hard ) )
	PORT_DIPNAME( 0x0c00, 0x0c00, DEF_STR( Lives ) ) PORT_DIPLOCATION("SW2:!3,!4")
	PORT_DIPSETTING(      0x0c00, "2" )
	PORT_DIPSETTING(      0x0800, "4" )
	PORT_DIPSETTING(      0x0400, "3" )
	PORT_DIPSETTING(      0x0000, "1" )
	PORT_DIPNAME( 0x3000, 0x3000, DEF_STR( Bonus_Life ) ) PORT_DIPLOCATION("SW2:!5,!6")
	PORT_DIPSETTING(      0x3000, "1000000" )
	PORT_DIPSETTING(      0x2000, "2000000" )
	PORT_DIPSETTING(      0x1000, "Every 1000000" )
	PORT_DIPSETTING(      0x0000, "No Extend" )
	PORT_DIPNAME( 0x4000, 0x4000, "Demo Sound" ) PORT_DIPLOCATION("SW2:!7")
	PORT_DIPSETTING(      0x0000, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x4000, DEF_STR( On ) )
	PORT_DIPNAME( 0x8000, 0x8000, DEF_STR( Unknown ) ) PORT_DIPLOCATION("SW2:!8") // marked as unused
	PORT_DIPSETTING(      0x8000, DEF_STR( Off ) )
	PORT_DIPSETTING(      0x0000, DEF_STR( On ) )

	PORT_MODIFY("SYSTEM")
	PORT_BIT( 0x0010, IP_ACTIVE_LOW, IPT_START3 )
	PORT_BIT( 0x0020, IP_ACTIVE_LOW, IPT_START4 )
	PORT_BIT( 0x00c0, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_DIPNAME( 0x0700, 0x0700, "Cabinet Setting" ) PORT_DIPLOCATION("SW3:!1,!2,!3")
	PORT_DIPSETTING(    0x0700, "2P" )
	PORT_DIPSETTING(    0x0600, "3P 3Slot" ) // TODO: coin3 is not hooked up here
	PORT_DIPSETTING(    0x0500, "4P 4Slot" ) // TODO: coin3 and 4 are not hooked up here
	PORT_DIPSETTING(    0x0400, "3P 2Slot" )
	PORT_DIPSETTING(    0x0300, "2P x2" ) // TODO: coin3 and 4 are not hooked up here
	PORT_DIPSETTING(    0x0200, "4P 2Slot" )
	PORT_DIPSETTING(    0x0100, "2P Freeplay" )
	PORT_DIPSETTING(    0x0000, "4P Freeplay" )
	PORT_SERVICE( 0x0800, IP_ACTIVE_LOW ) PORT_DIPLOCATION("SW3:!4") // marked as test mode
	PORT_DIPNAME( 0x1000, 0x1000, DEF_STR( Unknown ) ) PORT_DIPLOCATION("SW3:!5") // marked as unused
	PORT_DIPSETTING(    0x1000, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x0000, DEF_STR( On ) )
	PORT_DIPNAME( 0x2000, 0x2000, DEF_STR( Unknown ) ) PORT_DIPLOCATION("SW3:!6") // marked as unused
	PORT_DIPSETTING(    0x2000, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x0000, DEF_STR( On ) )
	PORT_DIPNAME( 0x4000, 0x4000, DEF_STR( Unknown ) ) PORT_DIPLOCATION("SW3:!7") // marked as unused
	PORT_DIPSETTING(    0x4000, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x0000, DEF_STR( On ) )
	PORT_DIPNAME( 0x8000, 0x8000, DEF_STR( Unknown ) ) PORT_DIPLOCATION("SW3:!8") // marked as unused
	PORT_DIPSETTING(    0x8000, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x0000, DEF_STR( On ) )
INPUT_PORTS_END

static INPUT_PORTS_START( xsedae )
	PORT_INCLUDE( raiden2 )

	PORT_START("P3_P4")
	PORT_BIT( 0xffff, IP_ACTIVE_LOW, IPT_UNUSED )

	PORT_MODIFY("DSW")
	PORT_DIPNAME( 0x0001, 0x0001, "DSW0" )
	PORT_DIPSETTING(    0x0001, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x0000, DEF_STR( On ) )
	PORT_DIPNAME( 0x0002, 0x0002, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x0002, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x0000, DEF_STR( On ) )
	PORT_DIPNAME( 0x0004, 0x0004, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x0004, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x0000, DEF_STR( On ) )
	PORT_DIPNAME( 0x0008, 0x0008, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x0008, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x0000, DEF_STR( On ) )
	PORT_DIPNAME( 0x0010, 0x0010, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x0010, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x0000, DEF_STR( On ) )
	PORT_DIPNAME( 0x0020, 0x0020, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x0020, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x0000, DEF_STR( On ) )
	PORT_DIPNAME( 0x0040, 0x0040, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x0040, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x0000, DEF_STR( On ) )
	PORT_DIPNAME( 0x0080, 0x0080, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x0080, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x0000, DEF_STR( On ) )
	PORT_DIPNAME( 0x0100, 0x0100, "DSW1" )
	PORT_DIPSETTING(    0x0100, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x0000, DEF_STR( On ) )
	PORT_DIPNAME( 0x0200, 0x0200, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x0200, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x0000, DEF_STR( On ) )
	PORT_DIPNAME( 0x0400, 0x0400, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x0400, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x0000, DEF_STR( On ) )
	PORT_DIPNAME( 0x0800, 0x0800, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x0800, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x0000, DEF_STR( On ) )
	PORT_DIPNAME( 0x1000, 0x1000, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x1000, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x0000, DEF_STR( On ) )
	PORT_DIPNAME( 0x2000, 0x2000, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x2000, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x0000, DEF_STR( On ) )
	PORT_DIPNAME( 0x4000, 0x4000, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x4000, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x0000, DEF_STR( On ) )
	PORT_DIPNAME( 0x8000, 0x8000, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x8000, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x0000, DEF_STR( On ) )
INPUT_PORTS_END

/*************************************
 *
 *  Graphics layouts
 *
 *************************************/

static const gfx_layout raiden2_charlayout =
{
	8,8,
	4096,
	4,
	{ 8,12,0,4 },
	{ 3,2,1,0,19,18,17,16 },
	{ STEP8(0,32) },
	32*8
};


static const gfx_layout raiden2_tilelayout =
{
	16,16,
	0x8000,
	4,
	{ 8,12,0,4 },
	{
		3,2,1,0,
		19,18,17,16,
		3+64*8, 2+64*8, 1+64*8, 0+64*8,
		19+64*8,18+64*8,17+64*8,16+64*8,
	},
	{ STEP16(0,32) },
	128*8
};

static const gfx_layout raiden2_spritelayout =
{
	16, 16,
	0x10000,
	4,
	{ STEP4(0,1) },
	{ 4, 0, 12, 8, 20, 16, 28, 24, 36, 32, 44, 40, 52, 48, 60, 56 },
	{ STEP16(0,64) },
	16*16*4
};

static GFXDECODE_START( raiden2 )
	GFXDECODE_ENTRY( "gfx1", 0x00000, raiden2_charlayout,   0x700, 128 )
	GFXDECODE_ENTRY( "gfx2", 0x00000, raiden2_tilelayout,   0x400, 128 )
	GFXDECODE_ENTRY( "gfx3", 0x00000, raiden2_spritelayout, 0x000, 128 )
GFXDECODE_END


/* MACHINE DRIVERS */

static MACHINE_CONFIG_START( raiden2, raiden2_state )

	/* basic machine hardware */
	MCFG_CPU_ADD("maincpu", V30,XTAL_32MHz/2) /* verified on pcb */
	MCFG_CPU_PROGRAM_MAP(raiden2_mem)
	MCFG_CPU_VBLANK_INT("screen", raiden2_interrupt)

	MCFG_MACHINE_RESET(raiden2)

	SEIBU2_RAIDEN2_SOUND_SYSTEM_CPU(XTAL_28_63636MHz/8)

	/* video hardware */
	MCFG_VIDEO_ATTRIBUTES(VIDEO_UPDATE_AFTER_VBLANK)

	MCFG_SCREEN_ADD("screen", RASTER)
	MCFG_SCREEN_REFRESH_RATE(55.47)    /* verified on pcb */
	MCFG_SCREEN_VBLANK_TIME(ATTOSECONDS_IN_USEC(2500) /* not accurate *//2)
	MCFG_SCREEN_SIZE(64*8, 64*8)
	MCFG_SCREEN_VISIBLE_AREA(0*8, 40*8-1, 0, 30*8-1)
	MCFG_SCREEN_UPDATE_STATIC(raiden2)
	MCFG_GFXDECODE(raiden2)
	MCFG_PALETTE_LENGTH(2048)

	MCFG_VIDEO_START(raiden2)

	/* sound hardware */
	SEIBU_SOUND_SYSTEM_YM2151_RAIDEN2_INTERFACE(XTAL_28_63636MHz/8,XTAL_28_63636MHz/28,1,2)
	// the sound z80 has /NMI, /BUSREQ and /WAIT tied high/unused


/* Sound hardware infos: Z80 and YM2151 are clocked at XTAL_28_63636MHz/8 */
/* The 2 Oki M6295 are clocked at XTAL_28_63636MHz/28 and pin 7 is high for both */

MACHINE_CONFIG_END

static MACHINE_CONFIG_DERIVED( xsedae, raiden2 )
	MCFG_CPU_MODIFY("maincpu")
	MCFG_CPU_PROGRAM_MAP(xsedae_mem)

	MCFG_MACHINE_RESET(xsedae)

	MCFG_VIDEO_ATTRIBUTES(VIDEO_UPDATE_BEFORE_VBLANK)

	MCFG_SCREEN_MODIFY("screen")
	MCFG_SCREEN_VISIBLE_AREA(0*8, 40*8-1, 0, 32*8-1)

MACHINE_CONFIG_END

static MACHINE_CONFIG_DERIVED( raidendx, raiden2 )
	MCFG_CPU_MODIFY("maincpu")
	MCFG_CPU_PROGRAM_MAP(raidendx_mem)

	MCFG_MACHINE_RESET(raidendx)
MACHINE_CONFIG_END


static MACHINE_CONFIG_START( zeroteam, raiden2_state )

	/* basic machine hardware */
	MCFG_CPU_ADD("maincpu", V30,XTAL_32MHz/2) /* verified on pcb */
	MCFG_CPU_PROGRAM_MAP(zeroteam_mem)
	MCFG_CPU_VBLANK_INT("screen", raiden2_interrupt)

	MCFG_MACHINE_RESET(zeroteam)

	SEIBU_NEWZEROTEAM_SOUND_SYSTEM_CPU(XTAL_28_63636MHz/8)

	/* video hardware */
	MCFG_VIDEO_ATTRIBUTES(VIDEO_UPDATE_AFTER_VBLANK)

	MCFG_SCREEN_ADD("screen", RASTER)
	MCFG_SCREEN_REFRESH_RATE(55.47)    /* verified on pcb */
	MCFG_SCREEN_VBLANK_TIME(ATTOSECONDS_IN_USEC(2500) /* not accurate *//2)
	MCFG_SCREEN_SIZE(64*8, 64*8)
	MCFG_SCREEN_VISIBLE_AREA(0*8, 40*8-1, 0, 32*8-1)
	MCFG_SCREEN_UPDATE_STATIC(raiden2)
	MCFG_GFXDECODE(raiden2)
	MCFG_PALETTE_LENGTH(2048)

	MCFG_VIDEO_START(raiden2)

	/* sound hardware */
	SEIBU_SOUND_SYSTEM_YM3812_INTERFACE(XTAL_28_63636MHz/8, 1320000/* ? */)
MACHINE_CONFIG_END

/* ROM LOADING */
/*
Raiden II

(C) 1993 RAIDEN II SEIBU KAIHATSU INC.,o
|----------------------------------------------------------|
|      1    2   3   4   5   6    7      8      9     10    |
|LA4460    M6295  PCM  Z8400A  6116    BATTERY3.6v        A|
|   YM2151 M6295   6    5      6116    28.6360 MHz        B|
|     VOL   YM3014                                         |
|HB-45A     YM3012 |------|                               C|
|HB-2      NJM4560 |SIE150| LH5116    |---------|          |
|RC220             |      | LH5116    | SEI252  |         D|
|RC220             |------| LH5116    |SB05-106 |          |
|RC220                      LH5116    |(QFP208) |         E|
|J                                    |         |         F|
|A                                    |---------|          |
|M    DSW2(8)                                             G|
|M    DSW1(8)                                   LH522258   |
|A          |---------|OBJ-1    OBJ-2           LH522258  H|
|           | SEI360  |                         LH522258  J|
|           |SB06-1937|OBJ-3    OBJ-4           LH522258  K|
|           |(QFP160) |                     |---------|   L|
|           |         |      1              |SEI1000  |   M|
| |------|  |---------|  1x      3x         |SB01-001 |   N|
| |SEI200|         32MHz     2              |(QFP184) |    |
| |      |CXK5863        2x      4x         |         |   P|
| |------|CXK5863                           |---------|    |
|                                                         Q|
|                        PAL2 PAL1             |----|     R|
|                                              |V30 |      |
|      BG-1       BG-2   7   COPX-D2           |----|     S|
|----------------------------------------------------------|
Notes:
      V30 clock    - 16.000MHz [32/2]. Chip is stamped "NEC D70116HG-16 V30 NEC '84" (QFP52)
      Z80 clock    - 3.579545MHz [28.63636/8]
      YM2151 clock - 3.579545MHz [28.63636/8]
      Yamaha DAC   -
        early boards: ym3014 mono dac, no NJM4560
        later boards: ym3012 stereo dac plus NJM4560, each with a capacitor on top
      M6295 clocks - 1.022MHz [28.63636/28] and pin 7 HIGH (both)
      LH52258      - Sharp LH52258 32k x8 SRAM (= 62256)
      CXK5863      - Sony CXK5863 8k x8 SRAM (= 6264)
      6116         - 2k x8 SRAM
      LH5116       - 2k x8 SRAM
      HB-45A       - Seibu custom ceramic module sound DAC (SIP20)
      HB-2         - Seibu custom ceramic module connected to coin counters (SIP10)
      RC220        - Custom resistor network module used for inputs (SIP14)
      VSync        - 55.4859Hz  \
      HSync        - 15.5586kHz / not measured but assumed same as R2DX
      PAL1         - MMIPAL16L8B stamped 'JJ4B01' (DIP20)
      PAL2         - AMI 18CV8 stamped 'JJ4B02' (DIP20)
      ROMs         - *PCM      - 2M MaskROM stamped 'RAIDEN 2 PCM' at location U1018 (DIP32), pcb labeled VOI2
                     6         - 23C020 MASK ROM labelled 'SEIBU 6' at location U1017 (DIP32), pcb labeled VOI1
                     5         - 27C512 EPROM labelled 'SEIBU 5' at location U1110 (DIP28)
                     *OBJ-1     - 16M MaskROM stamped 'RAIDEN 2 OBJ-1' at location U0811 (DIP42)
                     *OBJ-2     - 16M MaskROM stamped 'RAIDEN 2 OBJ-2' at location U082 (DIP42)
                     *OBJ-3     - 16M MaskROM stamped 'RAIDEN 2 OBJ-3' at location U0837 (DIP42)
                     *OBJ-4     - 16M MaskROM stamped 'RAIDEN 2 OBJ-4' at location U0836 (DIP42)
                 /   1x        - 27C2001 EPROM labelled 'SEIBU 1' at location U1210 (DIP32)
     Early boards|   2x        - 27C2001 EPROM labelled 'SEIBU 2' at location U1211 (DIP32)
                 |   3x        - 27C2001 EPROM labelled 'SEIBU 3' at location U129 (DIP32)
                 \   4x        - 27C2001 EPROM labelled 'SEIBU 4' at location U1212 (DIP32)
     Later boards/   1         - 27C402 or 27C4096 EPROM labelled 'SEIBU 1' at location U0211 (DIP40)
                 \   2         - 27C402 or 27C4096 EPROM labelled 'SEIBU 2' at location U0212 (DIP40)
                     *BG-1      - 16M MaskROM stamped 'RAIDEN 2 BG-1' at location U075 (DIP42)
                     *BG-2      - 16M MaskROM stamped 'RAIDEN 2 BG-2' at location U0714 (DIP42)
                     7         - 27C210 EPROM labelled 'SEIBU 7' at location U0724 (DIP40)
                     *COPX-D2  - 2M MaskROM stamped 'COPX-D2' at location U0313 (DIP40)

                     * = these ROMs are soldered-in

      SEIBU Custom ICs -
                        SIE150 (QFP100) - z80 interface
                        SEI252 SB05-106 (QFP208) - fg/sprite gfx and its decryption
                        SEI0200 TC110G21AF 0076 (QFP100) - bg gfx
                        SEI360 SB06-1937 (QFP160) - logic and i/o array
                        SEI1000 SB01-001 (QFP184) - main protection

*/

/* Note: some raiden 2 fabtek usa boards (the one Hammad sent to LN and Balrog, at least) have the
    ROM_LOAD( "seibu5.u1110",  0x000000, 0x08000, CRC(8f130589) SHA1(e58c8beaf9f27f063ffbcb0ab4600123c25ce6f3) )
    z80 sound rom as used in raiden2a instead of the
    ROM_LOAD( "snd.u1110",  0x000000, 0x08000, CRC(f51a28f9) SHA1(7ae2e2ba0c8159a544a8fd2bb0c2c694ba849302) )
    rom from raiden2. Slight version difference, and I don't know which is older/newer. - LN

ROMSET organization:
Note: type numbers are NOT NECESSARILY in chronological version order YET.
SETNAME   LONGNAME       PRG TYPES   SND(u1110) TYPE   VOICE(u1017) TYPE  FX0(u0724) TYPE  Notes
raiden2   (set 1 fabtek) 1 1'        1(f51a28f9)       1(fb0fca23)        1(c9ec9469)      sepia fighter on hiscore
raiden2a  (set 2 metro)  1 2'        2(8f130589)       1(fb0fca23)        1(c9ec9469)      sepia fighter on hiscore
raiden2b  (set 3 japan)  1 3'        2(8f130589)       1(fb0fca23)        1(c9ec9469)      sepia fighter on hiscore
raiden2c  (set 4 italy)  2 4'        3(5db9f922)       1(fb0fca23)        1(c9ec9469)      sepia fighter on hiscore
(trap15: one of these four above has aama serial 0587600)
raiden2d  (set 5 easy)   3 5'        4(6bad0a3e)       2(488d050f)        2(c709bdf6)      red fighter on hiscore
raiden2e  (set 6 easy)   4 6'        5(f5f835af)       3(fab9f8e4)        3(c7aa4d00)      red fighter on hiscore
raiden2f  (set 7 easy fabtek) 4 7'   5(f5f835af)       3(fab9f8e4)        3(c7aa4d00)      red fighter on hiscore
raiden2g  (set 8 easy fabtek) 3 8'   6(6d362472)       3(fab9f8e4)        3(c7aa4d00)      red fighter on hiscore, sn 0003068, aama 0557135
^ this set has 4 prg roms: 1 and 3 correspond to seibu1/prg0 and 2 and 4 correspond to seibu2/prg1
balrog+ln (set x fabtek) 1 1'        2(8f130589)       1(fb0fca23)        1(c9ec9469)      sepia fighter on hiscore, sn 0012739, aama 0600565, not in mame yet due to roms matching mix of sets 1 and 2

differences amongst SND/u1110 roms:
   First half end, last byte before ff fill ending at 7fff
   |     Last byte before ff fill ending at 8fff
   |     |     Last byte before ff fill ending at ffff
   |     |     |
1: 62e8  8faf  f56b
2: 62b8  8faf  f56b
3: 62a9  8faf  f56b
4: 623e  8ee7  f4dd
5: 620a  8ee7  f4d7
6: 64b8  8e1f  f4db
<LordNlptp> btw my guess is the code versions go from newest to oldest, 1 to 6, though I need more serial numbers to be sure
<LordNlptp> 6 has a larger main code chunk because i think they accidentally included some stuff they didn't actually use, which was removed on later versions
<LordNlptp> and it would not surprise me in the least if the code/player data is ALMOST the same as the zt version but with support for the second msm6295

*/

ROM_START( raiden2 )
	ROM_REGION( 0x200000, "mainprg", 0 ) /* v30 main cpu */
	ROM_LOAD16_BYTE("prg0.u0211",   0x000000, 0x80000, CRC(09475ec4) SHA1(05027f2d8f9e11fcbd485659eda68ada286dae32) )
	ROM_RELOAD(0x100000, 0x80000)
	ROM_LOAD16_BYTE("prg1.u0212",   0x000001, 0x80000, CRC(4609b5f2) SHA1(272d2aa75b8ea4d133daddf42c4fc9089093df2e) )
	ROM_RELOAD(0x100001, 0x80000)

	ROM_REGION( 0x40000, "user2", 0 )	/* COPX */
	ROM_LOAD( "copx-d2.u0313", 0x00000, 0x40000, CRC(a6732ff9) SHA1(c4856ec77869d9098da24b1bb3d7d58bb74b4cda) ) /* Soldered MASK ROM */

	ROM_REGION( 0x20000, "audiocpu", ROMREGION_ERASEFF ) /* 64k code for sound Z80 */
	ROM_LOAD( "snd.u1110",  0x000000, 0x08000, CRC(f51a28f9) SHA1(7ae2e2ba0c8159a544a8fd2bb0c2c694ba849302) )
	ROM_CONTINUE(0x10000,0x8000)
	ROM_COPY( "audiocpu", 0, 0x018000, 0x08000 )

	ROM_REGION( 0x020000, "gfx1", 0 ) /* chars */
	ROM_LOAD( "seibu7.u0724", 0x000000, 0x020000, CRC(c9ec9469) SHA1(a29f480a1bee073be7a177096ef58e1887a5af24) ) /* PCB silkscreened FX0 */

	ROM_REGION( 0x400000, "gfx2", 0 ) /* background gfx */
	ROM_LOAD( "raiden_2_seibu_bg-1.u0714", 0x000000, 0x200000, CRC(e61ad38e) SHA1(63b06cd38db946ad3fc5c1482dc863ef80b58fec) ) /* Soldered MASK ROM */
	ROM_LOAD( "raiden_2_seibu_bg-2.u075",  0x200000, 0x200000, CRC(a694a4bb) SHA1(39c2614d0effc899fe58f735604283097769df77) ) /* Soldered MASK ROM */

	ROM_REGION( 0x800000, "gfx3", 0 ) /* sprite gfx (encrypted) */
	ROM_LOAD32_WORD( "raiden_2_seibu_obj-1.u0811", 0x000000, 0x200000, CRC(ff08ef0b) SHA1(a1858430e8171ca8bab785457ef60e151b5e5cf1) ) /* Soldered MASK ROM */
	ROM_LOAD32_WORD( "raiden_2_seibu_obj-2.u082",  0x000002, 0x200000, CRC(638eb771) SHA1(9774cc070e71668d7d1d20795502dccd21ca557b) ) /* Soldered MASK ROM */
	ROM_LOAD32_WORD( "raiden_2_seibu_obj-3.u0837", 0x400000, 0x200000, CRC(897a0322) SHA1(abb2737a2446da5b364fc2d96524b43d808f4126) ) /* Soldered MASK ROM */
	ROM_LOAD32_WORD( "raiden_2_seibu_obj-4.u0836", 0x400002, 0x200000, CRC(b676e188) SHA1(19cc838f1ccf9c4203cd0e5365e5d99ff3a4ff0f) ) /* Soldered MASK ROM */

	ROM_REGION( 0x100000, "oki1", 0 )	/* ADPCM samples */
	ROM_LOAD( "seibu6.u1017", 0x00000, 0x40000, CRC(fb0fca23) SHA1(4b2217b121a66c5ab6015537609cf908ffedaf86) ) /* PCB silkscreened VOICE1 */

	ROM_REGION( 0x100000, "oki2", 0 )	/* ADPCM samples */
	ROM_LOAD( "raiden_2_pcm.u1018", 0x00000, 0x40000, CRC(8cf0d17e) SHA1(0fbe0b1e1ca5360c7c8329331408e3d799b4714c) ) /* Soldered MASK ROM */

	ROM_REGION( 0x10000, "pals", 0 )	/* PALS */
	ROM_LOAD( "jj4b02__ami18cv8-15.u0342.jed", 0x0000, 0x288, NO_DUMP)
	ROM_LOAD( "jj4b01__mmipal16l8bcn.u0341.jed", 0x0000, 0x288, NO_DUMP)
ROM_END

/*

---------------------------------------
Raiden II by SEIBU KAIHATSU INC. (1993)
---------------------------------------
malcor

Location      Type      File ID    Checksum
-------------------------------------------
M6 U0211     27C240      ROM1        F9A9
M6 U0212     27C240      ROM2e       13B3    [ English  ]
M6 U0212     27C240      ROM2J       14BF    [ Japanese ]
B5 U1110     27C512      ROM5        1223
B3 U1017     27C2000     ROM6        DE25
S5 U0724     27C1024     ROM7        966D

*/

ROM_START( raiden2a )
	ROM_REGION( 0x200000, "mainprg", 0 ) /* v30 main cpu */
	ROM_LOAD16_BYTE("prg0.u0211",   0x000000, 0x80000, CRC(09475ec4) SHA1(05027f2d8f9e11fcbd485659eda68ada286dae32) ) // rom1
	ROM_RELOAD(0x100000, 0x80000)
	ROM_LOAD16_BYTE("rom2e.u0212",  0x000001, 0x80000, CRC(458d619c) SHA1(842bf0eeb5d192a6b188f4560793db8dad697683) )
	ROM_RELOAD(0x100001, 0x80000)

	ROM_REGION( 0x40000, "user2", 0 )	/* COPX */
	ROM_LOAD( "copx-d2.u0313", 0x00000, 0x40000, CRC(a6732ff9) SHA1(c4856ec77869d9098da24b1bb3d7d58bb74b4cda) ) /* Soldered MASK ROM */

	ROM_REGION( 0x20000, "audiocpu", ROMREGION_ERASEFF ) /* 64k code for sound Z80 */
	ROM_LOAD( "seibu5.u1110",  0x000000, 0x08000, CRC(8f130589) SHA1(e58c8beaf9f27f063ffbcb0ab4600123c25ce6f3) )
	ROM_CONTINUE(0x10000,0x8000)
	ROM_COPY( "audiocpu", 0, 0x018000, 0x08000 )

	ROM_REGION( 0x020000, "gfx1", 0 ) /* chars */
	ROM_LOAD( "seibu7.u0724", 0x000000, 0x020000, CRC(c9ec9469) SHA1(a29f480a1bee073be7a177096ef58e1887a5af24) ) /* PCB silkscreened FX0 */

	ROM_REGION( 0x400000, "gfx2", 0 ) /* background gfx */
	ROM_LOAD( "raiden_2_seibu_bg-1.u0714", 0x000000, 0x200000, CRC(e61ad38e) SHA1(63b06cd38db946ad3fc5c1482dc863ef80b58fec) ) /* Soldered MASK ROM */
	ROM_LOAD( "raiden_2_seibu_bg-2.u075",  0x200000, 0x200000, CRC(a694a4bb) SHA1(39c2614d0effc899fe58f735604283097769df77) ) /* Soldered MASK ROM */

	ROM_REGION( 0x800000, "gfx3", 0 ) /* sprite gfx (encrypted) */
	ROM_LOAD32_WORD( "raiden_2_seibu_obj-1.u0811", 0x000000, 0x200000, CRC(ff08ef0b) SHA1(a1858430e8171ca8bab785457ef60e151b5e5cf1) ) /* Soldered MASK ROM */
	ROM_LOAD32_WORD( "raiden_2_seibu_obj-2.u082",  0x000002, 0x200000, CRC(638eb771) SHA1(9774cc070e71668d7d1d20795502dccd21ca557b) ) /* Soldered MASK ROM */
	ROM_LOAD32_WORD( "raiden_2_seibu_obj-3.u0837", 0x400000, 0x200000, CRC(897a0322) SHA1(abb2737a2446da5b364fc2d96524b43d808f4126) ) /* Soldered MASK ROM */
	ROM_LOAD32_WORD( "raiden_2_seibu_obj-4.u0836", 0x400002, 0x200000, CRC(b676e188) SHA1(19cc838f1ccf9c4203cd0e5365e5d99ff3a4ff0f) ) /* Soldered MASK ROM */

	ROM_REGION( 0x100000, "oki1", 0 )	/* ADPCM samples */
	ROM_LOAD( "seibu6.u1017", 0x00000, 0x40000, CRC(fb0fca23) SHA1(4b2217b121a66c5ab6015537609cf908ffedaf86) ) /* PCB silkscreened VOICE1 */

	ROM_REGION( 0x100000, "oki2", 0 )	/* ADPCM samples */
	ROM_LOAD( "raiden_2_pcm.u1018", 0x00000, 0x40000, CRC(8cf0d17e) SHA1(0fbe0b1e1ca5360c7c8329331408e3d799b4714c) ) /* Soldered MASK ROM */

	ROM_REGION( 0x10000, "pals", 0 )	/* PALS */
	ROM_LOAD( "jj4b02__ami18cv8-15.u0342.jed", 0x0000, 0x288, NO_DUMP)
	ROM_LOAD( "jj4b01__mmipal16l8bcn.u0341.jed", 0x0000, 0x288, NO_DUMP)
	ROM_END

/*

Raiden II (Japan version)
(c) 1993 Seibu Kaihatsu Inc.,

CPU:          D70116HG-16 V30/Z8400AB1 Z80ACPU
SOUND:        YM2151
VOICE:        M6295 x2
OSC:          32.000/28.6364MHz
CUSTOM:       SEI150
              SEI252
              SEI360
              SEI1000
              SEI0200
              COPX-D2 ((c)1992 RISE CORP)

---------------------------------------------------
 filemanes          devices       kind
---------------------------------------------------
 RD2_1.211          27C4096       V30 main prg.
 RD2_2.212          27C4096       V30 main prg.
 RD2_5.110          27C512        Z80 sound prg.
 RD2_PCM.018        27C2001       M6295 data
 RD2_6.017          27C2001       M6295 data
 RD2_7.724          27C1024       fix chr.
 RD2_BG1.075        57C16200      bg  chr.
 RD2_BG2.714        57C16200      bg  chr.
 RD2_OBJ1.811       57C16200      obj chr.
 RD2_OBJ2.082       57C16200      obj chr.
 RD2_OBJ3.837       57C16200      obj chr.
 RD2_OBJ4.836       57C16200      obj chr.
---------------------------------------------------

*/

ROM_START( raiden2b )
	ROM_REGION( 0x200000, "mainprg", 0 ) /* v30 main cpu */
	ROM_LOAD16_BYTE("prg0.u0211",   0x000000, 0x80000, CRC(09475ec4) SHA1(05027f2d8f9e11fcbd485659eda68ada286dae32) ) // rom1
	ROM_RELOAD(0x100000, 0x80000)
	ROM_LOAD16_BYTE("rom2j.u0212",  0x000001, 0x80000, CRC(e4e4fb4c) SHA1(7ccf33fe9a1cddf0c7e80d7ed66d615a828b3bb9) )
	ROM_RELOAD(0x100001, 0x80000)

	ROM_REGION( 0x40000, "user2", 0 )	/* COPX */
	ROM_LOAD( "copx-d2.u0313", 0x00000, 0x40000, CRC(a6732ff9) SHA1(c4856ec77869d9098da24b1bb3d7d58bb74b4cda) ) /* Soldered MASK ROM */

	ROM_REGION( 0x20000, "audiocpu", ROMREGION_ERASEFF ) /* 64k code for sound Z80 */
	ROM_LOAD( "seibu5.u1110",  0x000000, 0x08000, CRC(8f130589) SHA1(e58c8beaf9f27f063ffbcb0ab4600123c25ce6f3) )
	ROM_CONTINUE(0x10000,0x8000)
	ROM_COPY( "audiocpu", 0, 0x018000, 0x08000 )

	ROM_REGION( 0x020000, "gfx1", 0 ) /* chars */
	ROM_LOAD( "seibu7.u0724", 0x000000, 0x020000, CRC(c9ec9469) SHA1(a29f480a1bee073be7a177096ef58e1887a5af24) ) /* PCB silkscreened FX0 */

	ROM_REGION( 0x400000, "gfx2", 0 ) /* background gfx */
	ROM_LOAD( "raiden_2_seibu_bg-1.u0714", 0x000000, 0x200000, CRC(e61ad38e) SHA1(63b06cd38db946ad3fc5c1482dc863ef80b58fec) ) /* Soldered MASK ROM */
	ROM_LOAD( "raiden_2_seibu_bg-2.u075",  0x200000, 0x200000, CRC(a694a4bb) SHA1(39c2614d0effc899fe58f735604283097769df77) ) /* Soldered MASK ROM */

	ROM_REGION( 0x800000, "gfx3", 0 ) /* sprite gfx (encrypted) */
	ROM_LOAD32_WORD( "raiden_2_seibu_obj-1.u0811", 0x000000, 0x200000, CRC(ff08ef0b) SHA1(a1858430e8171ca8bab785457ef60e151b5e5cf1) ) /* Soldered MASK ROM */
	ROM_LOAD32_WORD( "raiden_2_seibu_obj-2.u082",  0x000002, 0x200000, CRC(638eb771) SHA1(9774cc070e71668d7d1d20795502dccd21ca557b) ) /* Soldered MASK ROM */
	ROM_LOAD32_WORD( "raiden_2_seibu_obj-3.u0837", 0x400000, 0x200000, CRC(897a0322) SHA1(abb2737a2446da5b364fc2d96524b43d808f4126) ) /* Soldered MASK ROM */
	ROM_LOAD32_WORD( "raiden_2_seibu_obj-4.u0836", 0x400002, 0x200000, CRC(b676e188) SHA1(19cc838f1ccf9c4203cd0e5365e5d99ff3a4ff0f) ) /* Soldered MASK ROM */

	ROM_REGION( 0x100000, "oki1", 0 )	/* ADPCM samples */
	ROM_LOAD( "seibu6.u1017", 0x00000, 0x40000, CRC(fb0fca23) SHA1(4b2217b121a66c5ab6015537609cf908ffedaf86) ) /* PCB silkscreened VOICE1 */

	ROM_REGION( 0x100000, "oki2", 0 )	/* ADPCM samples */
	ROM_LOAD( "raiden_2_pcm.u1018", 0x00000, 0x40000, CRC(8cf0d17e) SHA1(0fbe0b1e1ca5360c7c8329331408e3d799b4714c) ) /* Soldered MASK ROM */

	ROM_REGION( 0x10000, "pals", 0 )	/* PALS */
	ROM_LOAD( "jj4b02__ami18cv8-15.u0342.jed", 0x0000, 0x288, NO_DUMP)
	ROM_LOAD( "jj4b01__mmipal16l8bcn.u0341.jed", 0x0000, 0x288, NO_DUMP)
	ROM_END

ROM_START( raiden2c )
	ROM_REGION( 0x200000, "mainprg", 0 ) /* v30 main cpu */
	ROM_LOAD16_BYTE("seibu1.u0211",   0x000000, 0x80000, CRC(c1fc70f5) SHA1(a054f5ae9583972c406d9cf871340d5e072d71a3) ) /* Italian set */
	ROM_RELOAD(0x100000, 0x80000)
	ROM_LOAD16_BYTE("seibu2.u0212",   0x000001, 0x80000, CRC(28d5365f) SHA1(21efe29c2d373229c2ff302d86e59c2c94fa6d03) )
	ROM_RELOAD(0x100001, 0x80000)

	ROM_REGION( 0x40000, "user2", 0 )	/* COPX */
	ROM_LOAD( "copx-d2.u0313", 0x00000, 0x40000, CRC(a6732ff9) SHA1(c4856ec77869d9098da24b1bb3d7d58bb74b4cda) ) /* Soldered MASK ROM */

	ROM_REGION( 0x20000, "audiocpu", ROMREGION_ERASEFF ) /* 64k code for sound Z80 */
	ROM_LOAD( "seibu5.c.u1110",  0x000000, 0x08000, CRC(5db9f922) SHA1(8257aab98657fe44df19d2a48d85fcf65b3d98c6) )
	ROM_CONTINUE(0x10000,0x8000)
	ROM_COPY( "audiocpu", 0, 0x018000, 0x08000 )

	ROM_REGION( 0x020000, "gfx1", 0 ) /* chars */
	ROM_LOAD( "seibu7.u0724", 0x000000, 0x020000, CRC(c9ec9469) SHA1(a29f480a1bee073be7a177096ef58e1887a5af24) ) /* PCB silkscreened FX0 */

	ROM_REGION( 0x400000, "gfx2", 0 ) /* background gfx */
	ROM_LOAD( "raiden_2_seibu_bg-1.u0714", 0x000000, 0x200000, CRC(e61ad38e) SHA1(63b06cd38db946ad3fc5c1482dc863ef80b58fec) ) /* Soldered MASK ROM */
	ROM_LOAD( "raiden_2_seibu_bg-2.u075",  0x200000, 0x200000, CRC(a694a4bb) SHA1(39c2614d0effc899fe58f735604283097769df77) ) /* Soldered MASK ROM */

	ROM_REGION( 0x800000, "gfx3", 0 ) /* sprite gfx (encrypted) */
	ROM_LOAD32_WORD( "raiden_2_seibu_obj-1.u0811", 0x000000, 0x200000, CRC(ff08ef0b) SHA1(a1858430e8171ca8bab785457ef60e151b5e5cf1) ) /* Soldered MASK ROM */
	ROM_LOAD32_WORD( "raiden_2_seibu_obj-2.u082",  0x000002, 0x200000, CRC(638eb771) SHA1(9774cc070e71668d7d1d20795502dccd21ca557b) ) /* Soldered MASK ROM */
	ROM_LOAD32_WORD( "raiden_2_seibu_obj-3.u0837", 0x400000, 0x200000, CRC(897a0322) SHA1(abb2737a2446da5b364fc2d96524b43d808f4126) ) /* Soldered MASK ROM */
	ROM_LOAD32_WORD( "raiden_2_seibu_obj-4.u0836", 0x400002, 0x200000, CRC(b676e188) SHA1(19cc838f1ccf9c4203cd0e5365e5d99ff3a4ff0f) ) /* Soldered MASK ROM */

	ROM_REGION( 0x100000, "oki1", 0 )	/* ADPCM samples */
	ROM_LOAD( "seibu6.u1017", 0x00000, 0x40000, CRC(fb0fca23) SHA1(4b2217b121a66c5ab6015537609cf908ffedaf86) ) /* PCB silkscreened VOICE1 */

	ROM_REGION( 0x100000, "oki2", 0 )	/* ADPCM samples */
	ROM_LOAD( "raiden_2_pcm.u1018", 0x00000, 0x40000, CRC(8cf0d17e) SHA1(0fbe0b1e1ca5360c7c8329331408e3d799b4714c) ) /* Soldered MASK ROM */

	ROM_REGION( 0x10000, "pals", 0 )	/* PALS */
	ROM_LOAD( "jj4b02__ami18cv8-15.u0342.jed", 0x0000, 0x288, NO_DUMP)
	ROM_LOAD( "jj4b01__mmipal16l8bcn.u0341.jed", 0x0000, 0x288, NO_DUMP)
	ROM_END


/*

Raiden 2, Seibu License, Easy Version

According to DragonKnight Zero's excellent Raiden 2
FAQ this PCB is the easy version.

The different versions may be identified by the high score
screen. The easy version has the Raiden MK-II in colour
on a black background whereas the hard version has a sepia shot
of an ascending fighter.

The entire FAQ is available here:
http://www.gamefaqs.com/coinop/arcade/game/10729.html

*/

ROM_START( raiden2d )
	ROM_REGION( 0x200000, "mainprg", 0 ) /* v30 main cpu */
	ROM_LOAD16_BYTE("r2_prg_0.u0211",   0x000000, 0x80000, CRC(2abc848c) SHA1(1df4276d0074fcf1267757fa0b525a980a520f3d) )
	ROM_RELOAD(0x100000, 0x80000)
	ROM_LOAD16_BYTE("r2_prg_1.u0212",   0x000001, 0x80000, CRC(509ade43) SHA1(7cdee7bb00a6a1c7899d10b96385d54c261f6f5a) )
	ROM_RELOAD(0x100001, 0x80000)

	ROM_REGION( 0x40000, "user2", 0 )	/* COPX */
	ROM_LOAD( "copx-d2.u0313", 0x00000, 0x40000, CRC(a6732ff9) SHA1(c4856ec77869d9098da24b1bb3d7d58bb74b4cda) ) /* Soldered MASK ROM */

	ROM_REGION( 0x20000, "audiocpu", ROMREGION_ERASEFF ) /* 64k code for sound Z80 */
	ROM_LOAD( "r2_snd.u1110", 0x000000, 0x08000, CRC(6bad0a3e) SHA1(eb7ae42353e1984cd60b569c26cdbc3b025a7da6) )
	ROM_CONTINUE(0x10000,0x8000)
	ROM_COPY( "audiocpu", 0, 0x018000, 0x08000 )

	ROM_REGION( 0x020000, "gfx1", 0 ) /* chars */
	ROM_LOAD( "r2_fx0.u0724",	0x000000, 0x020000, CRC(c709bdf6) SHA1(0468d90412b7590d67eaadc0a5e3537cd5e73943) )

	ROM_REGION( 0x400000, "gfx2", 0 ) /* background gfx */
	ROM_LOAD( "raiden_2_seibu_bg-1.u0714", 0x000000, 0x200000, CRC(e61ad38e) SHA1(63b06cd38db946ad3fc5c1482dc863ef80b58fec) ) /* Soldered MASK ROM */
	ROM_LOAD( "raiden_2_seibu_bg-2.u075",  0x200000, 0x200000, CRC(a694a4bb) SHA1(39c2614d0effc899fe58f735604283097769df77) ) /* Soldered MASK ROM */

	ROM_REGION( 0x800000, "gfx3", 0 ) /* sprite gfx (encrypted) */
	ROM_LOAD32_WORD( "raiden_2_seibu_obj-1.u0811", 0x000000, 0x200000, CRC(ff08ef0b) SHA1(a1858430e8171ca8bab785457ef60e151b5e5cf1) ) /* Soldered MASK ROM */
	ROM_LOAD32_WORD( "raiden_2_seibu_obj-2.u082",  0x000002, 0x200000, CRC(638eb771) SHA1(9774cc070e71668d7d1d20795502dccd21ca557b) ) /* Soldered MASK ROM */
	ROM_LOAD32_WORD( "raiden_2_seibu_obj-3.u0837", 0x400000, 0x200000, CRC(897a0322) SHA1(abb2737a2446da5b364fc2d96524b43d808f4126) ) /* Soldered MASK ROM */
	ROM_LOAD32_WORD( "raiden_2_seibu_obj-4.u0836", 0x400002, 0x200000, CRC(b676e188) SHA1(19cc838f1ccf9c4203cd0e5365e5d99ff3a4ff0f) ) /* Soldered MASK ROM */

	ROM_REGION( 0x100000, "oki1", 0 )	/* ADPCM samples */
	ROM_LOAD( "r2_voi1.u1017", 0x00000, 0x40000, CRC(488d050f) SHA1(fde2fd64fea6bc39e1a42885d21d362bc6be2ac2) )

	ROM_REGION( 0x100000, "oki2", 0 )	/* ADPCM samples */
	ROM_LOAD( "raiden_2_pcm.u1018", 0x00000, 0x40000, CRC(8cf0d17e) SHA1(0fbe0b1e1ca5360c7c8329331408e3d799b4714c) ) /* Soldered MASK ROM */

	ROM_REGION( 0x10000, "pals", 0 )	/* PALS */
	ROM_LOAD( "jj4b02__ami18cv8-15.u0342.jed", 0x0000, 0x288, NO_DUMP)
	ROM_LOAD( "jj4b01__mmipal16l8bcn.u0341.jed", 0x0000, 0x288, NO_DUMP)
ROM_END

ROM_START( raiden2e )
	ROM_REGION( 0x200000, "mainprg", 0 ) /* v30 main cpu */
	ROM_LOAD16_BYTE("r2.1.u0211",  0x000000, 0x80000, CRC(d7041be4) SHA1(3cf97132fba6f7b00c9059265f4e9f0bf1505b71) )
	ROM_RELOAD(0x100000, 0x80000)
	ROM_LOAD16_BYTE("r2.2.u0212",  0x000001, 0x80000, CRC(bf7577ec) SHA1(98576af78760b8aef1ef3efe1ba963977c89d225) )
	ROM_RELOAD(0x100001, 0x80000)

	ROM_REGION( 0x40000, "user2", 0 )	/* COPX */
	ROM_LOAD( "copx-d2.u0313", 0x00000, 0x40000, CRC(a6732ff9) SHA1(c4856ec77869d9098da24b1bb3d7d58bb74b4cda) ) /* Soldered MASK ROM */

	ROM_REGION( 0x20000, "audiocpu", 0 ) /* 64k code for sound Z80 */
	ROM_LOAD( "r2.5.u1110", 0x000000, 0x08000, CRC(f5f835af) SHA1(5be82ebc582d0da919e9ae1b9e64528bb295efc7) )
	ROM_CONTINUE(0x10000,0x8000)
	ROM_COPY( "audiocpu", 0, 0x018000, 0x08000 )

	ROM_REGION( 0x020000, "gfx1", 0 ) /* chars */
	ROM_LOAD( "r2.7.u0724", 0x000000, 0x020000, CRC(c7aa4d00) SHA1(9ad99d3891598c1ea3f12318400ee67666da56dd) )

	ROM_REGION( 0x400000, "gfx2", 0 ) /* background gfx */
	ROM_LOAD( "raiden_2_seibu_bg-1.u0714", 0x000000, 0x200000, CRC(e61ad38e) SHA1(63b06cd38db946ad3fc5c1482dc863ef80b58fec) ) /* Soldered MASK ROM */
	ROM_LOAD( "raiden_2_seibu_bg-2.u075",  0x200000, 0x200000, CRC(a694a4bb) SHA1(39c2614d0effc899fe58f735604283097769df77) ) /* Soldered MASK ROM */

	ROM_REGION( 0x800000, "gfx3", 0 ) /* sprite gfx (encrypted) */
	ROM_LOAD32_WORD( "raiden_2_seibu_obj-1.u0811", 0x000000, 0x200000, CRC(ff08ef0b) SHA1(a1858430e8171ca8bab785457ef60e151b5e5cf1) ) /* Soldered MASK ROM */
	ROM_LOAD32_WORD( "raiden_2_seibu_obj-2.u082",  0x000002, 0x200000, CRC(638eb771) SHA1(9774cc070e71668d7d1d20795502dccd21ca557b) ) /* Soldered MASK ROM */
	ROM_LOAD32_WORD( "raiden_2_seibu_obj-3.u0837", 0x400000, 0x200000, CRC(897a0322) SHA1(abb2737a2446da5b364fc2d96524b43d808f4126) ) /* Soldered MASK ROM */
	ROM_LOAD32_WORD( "raiden_2_seibu_obj-4.u0836", 0x400002, 0x200000, CRC(b676e188) SHA1(19cc838f1ccf9c4203cd0e5365e5d99ff3a4ff0f) ) /* Soldered MASK ROM */

	ROM_REGION( 0x100000, "oki1", 0 )	/* ADPCM samples */
	ROM_LOAD( "r2.6.u1017", 0x00000, 0x40000, CRC(fab9f8e4) SHA1(b1eff154c4f766b2d272ac6a57f8d54c9e39e3bb) )

	ROM_REGION( 0x100000, "oki2", 0 )	/* ADPCM samples */
	ROM_LOAD( "raiden_2_pcm.u1018", 0x00000, 0x40000, CRC(8cf0d17e) SHA1(0fbe0b1e1ca5360c7c8329331408e3d799b4714c) ) /* Soldered MASK ROM */

	ROM_REGION( 0x10000, "pals", 0 )	/* PALS */
	ROM_LOAD( "jj4b02__ami18cv8-15.u0342.jed", 0x0000, 0x288, NO_DUMP)
	ROM_LOAD( "jj4b01__mmipal16l8bcn.u0341.jed", 0x0000, 0x288, NO_DUMP)
	ROM_END

ROM_START( raiden2f ) // same as raiden2e, different region
	ROM_REGION( 0x200000, "mainprg", 0 ) /* v30 main cpu */
	ROM_LOAD16_BYTE("seibu_1.u0211",  0x000000, 0x80000, CRC(d7041be4) SHA1(3cf97132fba6f7b00c9059265f4e9f0bf1505b71) )
	ROM_RELOAD(0x100000, 0x80000)
	ROM_LOAD16_BYTE("seibu_2.u0212",  0x000001, 0x80000, CRC(beb71ddb) SHA1(471399ead1cdc27ac2a1139f9616f828efd14626) )
	ROM_RELOAD(0x100001, 0x80000)

	ROM_REGION( 0x40000, "user2", 0 )	/* COPX */
	ROM_LOAD( "copx-d2.u0313", 0x00000, 0x40000, CRC(a6732ff9) SHA1(c4856ec77869d9098da24b1bb3d7d58bb74b4cda) ) /* Soldered MASK ROM */

	ROM_REGION( 0x20000, "audiocpu", 0 ) /* 64k code for sound Z80 */
	ROM_LOAD( "r2.5.u1110", 0x000000, 0x08000, CRC(f5f835af) SHA1(5be82ebc582d0da919e9ae1b9e64528bb295efc7) )
	ROM_CONTINUE(0x10000,0x8000)
	ROM_COPY( "audiocpu", 0, 0x018000, 0x08000 )

	ROM_REGION( 0x020000, "gfx1", 0 ) /* chars */
	ROM_LOAD( "r2.7.u0724", 0x000000, 0x020000, CRC(c7aa4d00) SHA1(9ad99d3891598c1ea3f12318400ee67666da56dd) )

	ROM_REGION( 0x400000, "gfx2", 0 ) /* background gfx */
	ROM_LOAD( "raiden_2_seibu_bg-1.u0714", 0x000000, 0x200000, CRC(e61ad38e) SHA1(63b06cd38db946ad3fc5c1482dc863ef80b58fec) ) /* Soldered MASK ROM */
	ROM_LOAD( "raiden_2_seibu_bg-2.u075",  0x200000, 0x200000, CRC(a694a4bb) SHA1(39c2614d0effc899fe58f735604283097769df77) ) /* Soldered MASK ROM */

	ROM_REGION( 0x800000, "gfx3", 0 ) /* sprite gfx (encrypted) */
	ROM_LOAD32_WORD( "raiden_2_seibu_obj-1.u0811", 0x000000, 0x200000, CRC(ff08ef0b) SHA1(a1858430e8171ca8bab785457ef60e151b5e5cf1) ) /* Soldered MASK ROM */
	ROM_LOAD32_WORD( "raiden_2_seibu_obj-2.u082",  0x000002, 0x200000, CRC(638eb771) SHA1(9774cc070e71668d7d1d20795502dccd21ca557b) ) /* Soldered MASK ROM */
	ROM_LOAD32_WORD( "raiden_2_seibu_obj-3.u0837", 0x400000, 0x200000, CRC(897a0322) SHA1(abb2737a2446da5b364fc2d96524b43d808f4126) ) /* Soldered MASK ROM */
	ROM_LOAD32_WORD( "raiden_2_seibu_obj-4.u0836", 0x400002, 0x200000, CRC(b676e188) SHA1(19cc838f1ccf9c4203cd0e5365e5d99ff3a4ff0f) ) /* Soldered MASK ROM */

	ROM_REGION( 0x100000, "oki1", 0 )	/* ADPCM samples */
	ROM_LOAD( "r2.6.u1017", 0x00000, 0x40000, CRC(fab9f8e4) SHA1(b1eff154c4f766b2d272ac6a57f8d54c9e39e3bb) )

	ROM_REGION( 0x100000, "oki2", 0 )	/* ADPCM samples */
	ROM_LOAD( "raiden_2_pcm.u1018", 0x00000, 0x40000, CRC(8cf0d17e) SHA1(0fbe0b1e1ca5360c7c8329331408e3d799b4714c) ) /* Soldered MASK ROM */

	ROM_REGION( 0x10000, "pals", 0 )	/* PALS */
	ROM_LOAD( "jj4b02__ami18cv8-15.u0342.jed", 0x0000, 0x288, NO_DUMP)
	ROM_LOAD( "jj4b01__mmipal16l8bcn.u0341.jed", 0x0000, 0x288, NO_DUMP)
	ROM_END

ROM_START( raiden2g ) // sort of a mixture of raiden2d easy set with voice rom of raiden2e and 2f and a unique sound rom
	ROM_REGION( 0x200000, "mainprg", 0 ) /* v30 main cpu */
	ROM_LOAD32_BYTE("seibu__1.27c020j.u1210",   0x000000, 0x40000, CRC(ED1514E3) SHA1(296125BFE3C4F3033F7AA319DD8554BC978C4A00) )
	ROM_RELOAD(0x100000, 0x40000)
	ROM_LOAD32_BYTE("seibu__2.27c2001.u1211",   0x000001, 0x40000, CRC(BB6ECF2A) SHA1(D4F628E9D0ED2897654F05A8A2541E1ED3FAF8DD) )
	ROM_RELOAD(0x100001, 0x40000)
	ROM_LOAD32_BYTE("seibu__3.27c2001.u129",   0x000002, 0x40000, CRC(6A01D52C) SHA1(983B914592AB9D9C058BEBB5BCCF5C882E2B82DE) )
	ROM_RELOAD(0x100002, 0x40000)
	ROM_LOAD32_BYTE("seibu__4.27c2001.u1212",   0x000003, 0x40000, CRC(E54BFA37) SHA1(4FABB23503FD9245A10CDED15A6880415CA5FFD7) )
	ROM_RELOAD(0x100003, 0x40000)

	ROM_REGION( 0x40000, "user2", 0 )	/* COPX */
	ROM_LOAD( "copx-d2.u0313", 0x00000, 0x40000, CRC(a6732ff9) SHA1(c4856ec77869d9098da24b1bb3d7d58bb74b4cda) ) /* Soldered MASK ROM */

	ROM_REGION( 0x20000, "audiocpu", 0 ) /* 64k code for sound Z80 */
	ROM_LOAD( "seibu__5.27c512.u1110", 0x000000, 0x08000, CRC(6d362472) SHA1(a362e500bb9492affde1f7a4da7e08dd16e755df) )
	ROM_CONTINUE(0x10000,0x8000)
	ROM_COPY( "audiocpu", 0, 0x018000, 0x08000 )

	ROM_REGION( 0x020000, "gfx1", 0 ) /* chars */
	ROM_LOAD( "seibu__7.fx0.27c210.u0724", 0x000000, 0x020000, CRC(c7aa4d00) SHA1(9ad99d3891598c1ea3f12318400ee67666da56dd) )

	ROM_REGION( 0x400000, "gfx2", 0 ) /* background gfx */
	ROM_LOAD( "raiden_2_seibu_bg-1.u0714", 0x000000, 0x200000, CRC(e61ad38e) SHA1(63b06cd38db946ad3fc5c1482dc863ef80b58fec) ) /* Soldered MASK ROM */
	ROM_LOAD( "raiden_2_seibu_bg-2.u075",  0x200000, 0x200000, CRC(a694a4bb) SHA1(39c2614d0effc899fe58f735604283097769df77) ) /* Soldered MASK ROM */

	ROM_REGION( 0x800000, "gfx3", 0 ) /* sprite gfx (encrypted) */
	ROM_LOAD32_WORD( "raiden_2_seibu_obj-1.u0811", 0x000000, 0x200000, CRC(ff08ef0b) SHA1(a1858430e8171ca8bab785457ef60e151b5e5cf1) ) /* Soldered MASK ROM */
	ROM_LOAD32_WORD( "raiden_2_seibu_obj-2.u082",  0x000002, 0x200000, CRC(638eb771) SHA1(9774cc070e71668d7d1d20795502dccd21ca557b) ) /* Soldered MASK ROM */
	ROM_LOAD32_WORD( "raiden_2_seibu_obj-3.u0837", 0x400000, 0x200000, CRC(897a0322) SHA1(abb2737a2446da5b364fc2d96524b43d808f4126) ) /* Soldered MASK ROM */
	ROM_LOAD32_WORD( "raiden_2_seibu_obj-4.u0836", 0x400002, 0x200000, CRC(b676e188) SHA1(19cc838f1ccf9c4203cd0e5365e5d99ff3a4ff0f) ) /* Soldered MASK ROM */

	ROM_REGION( 0x100000, "oki1", 0 )	/* ADPCM samples */
	ROM_LOAD( "seibu__6.voice1.23c020.u1017", 0x00000, 0x40000, CRC(fab9f8e4) SHA1(b1eff154c4f766b2d272ac6a57f8d54c9e39e3bb) )

	ROM_REGION( 0x100000, "oki2", 0 )	/* ADPCM samples */
	ROM_LOAD( "raiden_2_pcm.u1018", 0x00000, 0x40000, CRC(8cf0d17e) SHA1(0fbe0b1e1ca5360c7c8329331408e3d799b4714c) ) /* Soldered MASK ROM */

	ROM_REGION( 0x10000, "pals", 0 )	/* PALS */
	ROM_LOAD( "jj4b02__ami18cv8-15.u0342.jed", 0x0000, 0x288, NO_DUMP)
	ROM_LOAD( "jj4b01__mmipal16l8bcn.u0341.jed", 0x0000, 0x288, NO_DUMP)
ROM_END


/* Raiden DX sets */

ROM_START( raidndx )
	ROM_REGION( 0x200000, "mainprg", 0 ) /* v30 main cpu */
	ROM_LOAD32_BYTE("1d.4n",   0x000000, 0x80000, CRC(14d725fc) SHA1(f12806f64f069fdc4ee29b309a32f7ca00b36f93) )
	ROM_LOAD32_BYTE("2d.4p",   0x000001, 0x80000, CRC(5e7e45cb) SHA1(94eff893b5335c522f1c063c3175b9bac87b0a25) )
	ROM_LOAD32_BYTE("3d.6n",   0x000002, 0x80000, CRC(f0a47e67) SHA1(8cbd21993077b2e01295db6e343cae9e0e4bfefe) )
	ROM_LOAD32_BYTE("4d.6p",   0x000003, 0x80000, CRC(2a2003e8) SHA1(f239b351759babe4683d16e745a5ac2f3c2ab06b) )

	ROM_REGION( 0x40000, "user2", 0 )	/* COPX */
	ROM_LOAD( "copx-d2.6s",   0x00000, 0x40000, CRC(a6732ff9) SHA1(c4856ec77869d9098da24b1bb3d7d58bb74b4cda) ) /* Shared with original Raiden 2 */

	ROM_REGION( 0x20000, "audiocpu", ROMREGION_ERASEFF ) /* 64k code for sound Z80 */
	ROM_LOAD( "dx_5.5b",  0x000000, 0x08000,  CRC(8c46857a) SHA1(8b269cb20adf960ba4eb594d8add7739dbc9a837) )
	ROM_CONTINUE(0x10000,0x8000)
	ROM_COPY( "audiocpu", 0, 0x018000, 0x08000 )

	ROM_REGION( 0x020000, "gfx1", 0 ) /* chars */
	ROM_LOAD( "dx_7.4s",	0x000000,	0x020000,	CRC(c73986d4) SHA1(d29345077753bda53560dedc95dd23f329e521d9) )

	ROM_REGION( 0x400000, "gfx2", 0 ) /* background gfx */
	ROM_LOAD( "dx_back1.1s",   0x000000, 0x200000, CRC(90970355) SHA1(d71d57cd550a800f583550365102adb7b1b779fc) )
	ROM_LOAD( "dx_back2.2s",   0x200000, 0x200000, CRC(5799af3e) SHA1(85d6532abd769da77bcba70bd2e77915af40f987) )

	ROM_REGION( 0x800000, "gfx3", 0 ) /* sprite gfx (encrypted) */
	ROM_LOAD32_WORD( "obj1",        0x000000, 0x200000, CRC(ff08ef0b) SHA1(a1858430e8171ca8bab785457ef60e151b5e5cf1) ) /* Shared with original Raiden 2 */
	ROM_LOAD32_WORD( "obj2",        0x000002, 0x200000, CRC(638eb771) SHA1(9774cc070e71668d7d1d20795502dccd21ca557b) ) /* Shared with original Raiden 2 */
	ROM_LOAD32_WORD( "dx_obj3.4k",  0x400000, 0x200000, CRC(ba381227) SHA1(dfc4d659aca1722a981fa56a31afabe66f444d5d) )
	ROM_LOAD32_WORD( "dx_obj4.6k",  0x400002, 0x200000, CRC(65e50d19) SHA1(c46147b4132abce7314b46bf419ce4773e024b05) )

	ROM_REGION( 0x100000, "oki1", 0 )	/* ADPCM samples */
	ROM_LOAD( "dx_6.3b",   0x00000, 0x40000, CRC(9a9196da) SHA1(3d1ee67fb0d40a231ce04d10718f07ffb76db455) )

	ROM_REGION( 0x100000, "oki2", 0 )	/* ADPCM samples */
	ROM_LOAD( "dx_pcm.3a", 0x00000, 0x40000, CRC(8cf0d17e) SHA1(0fbe0b1e1ca5360c7c8329331408e3d799b4714c) ) /* Shared with original Raiden 2 */
ROM_END

ROM_START( raidndxa1 )
	ROM_REGION( 0x200000, "mainprg", 0 ) /* v30 main cpu */
	ROM_LOAD32_BYTE("dx_1h.4n",   0x000000, 0x80000, BAD_DUMP CRC(7624c36b) SHA1(84c17f2988031210d06536710e1eac558f4290a1) ) // bad
	ROM_LOAD32_BYTE("dx_2h.4p",   0x000001, 0x80000, CRC(4940fdf3) SHA1(c87e307ed7191802583bee443c7c8e4f4e33db25) )
	ROM_LOAD32_BYTE("dx_3h.6n",   0x000002, 0x80000, CRC(6c495bcf) SHA1(fb6153ecc443dabc829dda6f8d11234ad48de88a) )
	ROM_LOAD32_BYTE("dx_4h.6k",   0x000003, 0x80000, CRC(9ed6335f) SHA1(66975204b120915f23258a431e19dbc017afd912) )

	ROM_REGION( 0x40000, "user2", 0 )	/* COPX */
	ROM_LOAD( "copx-d2.6s",   0x00000, 0x40000, CRC(a6732ff9) SHA1(c4856ec77869d9098da24b1bb3d7d58bb74b4cda) )

	ROM_REGION( 0x20000, "audiocpu", ROMREGION_ERASEFF ) /* 64k code for sound Z80 */
	ROM_LOAD( "dx_5.5b",  0x000000, 0x08000,  CRC(8c46857a) SHA1(8b269cb20adf960ba4eb594d8add7739dbc9a837) )
	ROM_CONTINUE(0x10000,0x8000)
	ROM_COPY( "audiocpu", 0, 0x018000, 0x08000 )

	ROM_REGION( 0x020000, "gfx1", 0 ) /* chars */
	ROM_LOAD( "dx_7.4s",	0x000000,	0x020000,	CRC(c73986d4) SHA1(d29345077753bda53560dedc95dd23f329e521d9) )

	ROM_REGION( 0x400000, "gfx2", 0 ) /* background gfx */
	ROM_LOAD( "dx_back1.1s",   0x000000, 0x200000, CRC(90970355) SHA1(d71d57cd550a800f583550365102adb7b1b779fc) )
	ROM_LOAD( "dx_back2.2s",   0x200000, 0x200000, CRC(5799af3e) SHA1(85d6532abd769da77bcba70bd2e77915af40f987) )

	ROM_REGION( 0x800000, "gfx3", 0 ) /* sprite gfx (encrypted) */
	ROM_LOAD32_WORD( "obj1",        0x000000, 0x200000, CRC(ff08ef0b) SHA1(a1858430e8171ca8bab785457ef60e151b5e5cf1) ) /* Shared with original Raiden 2 */
	ROM_LOAD32_WORD( "obj2",        0x000002, 0x200000, CRC(638eb771) SHA1(9774cc070e71668d7d1d20795502dccd21ca557b) ) /* Shared with original Raiden 2 */
	ROM_LOAD32_WORD( "dx_obj3.4k",  0x400000, 0x200000, CRC(ba381227) SHA1(dfc4d659aca1722a981fa56a31afabe66f444d5d) )
	ROM_LOAD32_WORD( "dx_obj4.6k",  0x400002, 0x200000, CRC(65e50d19) SHA1(c46147b4132abce7314b46bf419ce4773e024b05) )

	ROM_REGION( 0x100000, "oki1", 0 )	/* ADPCM samples */
	ROM_LOAD( "dx_6.3b",   0x00000, 0x40000, CRC(9a9196da) SHA1(3d1ee67fb0d40a231ce04d10718f07ffb76db455) )

	ROM_REGION( 0x100000, "oki2", 0 )	/* ADPCM samples */
	ROM_LOAD( "dx_pcm.3a", 0x00000, 0x40000, CRC(8cf0d17e) SHA1(0fbe0b1e1ca5360c7c8329331408e3d799b4714c) )
ROM_END

ROM_START( raidndxa2 )
	ROM_REGION( 0x200000, "mainprg", 0 ) /* v30 main cpu */
	ROM_LOAD32_BYTE("1d.bin",   0x000000, 0x80000, CRC(22b155ae) SHA1(388151e2c8fb301bd5bc66a974e9fe16816ae0bc) )
	ROM_LOAD32_BYTE("2d.bin",   0x000001, 0x80000, CRC(2be98ca8) SHA1(491e990405b0ad3de45bdbcc2453af9215ae19c8) )
	ROM_LOAD32_BYTE("3d.bin",   0x000002, 0x80000, CRC(b4785576) SHA1(aa5eee7b0c635c6d18a7fc1e037bf570a677dd90) )
	ROM_LOAD32_BYTE("4d.bin",   0x000003, 0x80000, CRC(5a77f7b4) SHA1(aa757e6308893ca63963170c5b1743de7c7ab034) )

	ROM_REGION( 0x40000, "user2", 0 )	/* COPX */
	ROM_LOAD( "copx-d2.6s",   0x00000, 0x40000, CRC(a6732ff9) SHA1(c4856ec77869d9098da24b1bb3d7d58bb74b4cda) ) /* Shared with original Raiden 2 */

	ROM_REGION( 0x20000, "audiocpu", ROMREGION_ERASEFF ) /* 64k code for sound Z80 */
	ROM_LOAD( "dx_5.5b",  0x000000, 0x08000,  CRC(8c46857a) SHA1(8b269cb20adf960ba4eb594d8add7739dbc9a837) )
	ROM_CONTINUE(0x10000,0x8000)
	ROM_COPY( "audiocpu", 0, 0x018000, 0x08000 )

	ROM_REGION( 0x020000, "gfx1", 0 ) /* chars */
	ROM_LOAD( "dx_7.4s",	0x000000,	0x020000,	CRC(c73986d4) SHA1(d29345077753bda53560dedc95dd23f329e521d9) )

	ROM_REGION( 0x400000, "gfx2", 0 ) /* background gfx */
	ROM_LOAD( "dx_back1.1s",   0x000000, 0x200000, CRC(90970355) SHA1(d71d57cd550a800f583550365102adb7b1b779fc) )
	ROM_LOAD( "dx_back2.2s",   0x200000, 0x200000, CRC(5799af3e) SHA1(85d6532abd769da77bcba70bd2e77915af40f987) )

	ROM_REGION( 0x800000, "gfx3", 0 ) /* sprite gfx (encrypted) */
	ROM_LOAD32_WORD( "obj1",        0x000000, 0x200000, CRC(ff08ef0b) SHA1(a1858430e8171ca8bab785457ef60e151b5e5cf1) ) /* Shared with original Raiden 2 */
	ROM_LOAD32_WORD( "obj2",        0x000002, 0x200000, CRC(638eb771) SHA1(9774cc070e71668d7d1d20795502dccd21ca557b) ) /* Shared with original Raiden 2 */
	ROM_LOAD32_WORD( "dx_obj3.4k",  0x400000, 0x200000, CRC(ba381227) SHA1(dfc4d659aca1722a981fa56a31afabe66f444d5d) )
	ROM_LOAD32_WORD( "dx_obj4.6k",  0x400002, 0x200000, CRC(65e50d19) SHA1(c46147b4132abce7314b46bf419ce4773e024b05) )

	ROM_REGION( 0x100000, "oki1", 0 )	/* ADPCM samples */
	ROM_LOAD( "dx_6.3b",   0x00000, 0x40000, CRC(9a9196da) SHA1(3d1ee67fb0d40a231ce04d10718f07ffb76db455) )

	ROM_REGION( 0x100000, "oki2", 0 )	/* ADPCM samples */
	ROM_LOAD( "dx_pcm.3a", 0x00000, 0x40000, CRC(8cf0d17e) SHA1(0fbe0b1e1ca5360c7c8329331408e3d799b4714c) ) /* Shared with original Raiden 2 */
ROM_END

ROM_START( raidndxj )
	ROM_REGION( 0x200000, "mainprg", 0 ) /* v30 main cpu */
	ROM_LOAD32_BYTE("rdxj_1.bin",   0x000000, 0x80000, CRC(b5b32885) SHA1(fb3c592b2436d347103c17bd765176062be95fa2) )
	ROM_LOAD32_BYTE("rdxj_2.bin",   0x000001, 0x80000, CRC(7efd581d) SHA1(4609a0d8afb3d62a38b461089295efed47beea91) )
	ROM_LOAD32_BYTE("rdxj_3.bin",   0x000002, 0x80000, CRC(55ec0e1d) SHA1(6be7f268df51311a817c1c329a578b38abb659ae) )
	ROM_LOAD32_BYTE("rdxj_4.bin",   0x000003, 0x80000, CRC(f8fb31b4) SHA1(b72fd7cbbebcf3d1b2253c309fcfa60674776467) )

	ROM_REGION( 0x40000, "user2", 0 )	/* COPX */
	ROM_LOAD( "copx-d2.6s",   0x00000, 0x40000, CRC(a6732ff9) SHA1(c4856ec77869d9098da24b1bb3d7d58bb74b4cda) ) /* Shared with original Raiden 2 */

	ROM_REGION( 0x20000, "audiocpu", ROMREGION_ERASEFF ) /* 64k code for sound Z80 */
	ROM_LOAD( "dx_5.5b",  0x000000, 0x08000,  CRC(8c46857a) SHA1(8b269cb20adf960ba4eb594d8add7739dbc9a837) )
	ROM_CONTINUE(0x10000,0x8000)
	ROM_COPY( "audiocpu", 0, 0x018000, 0x08000 )

	ROM_REGION( 0x020000, "gfx1", 0 ) /* chars */
	ROM_LOAD( "dx_7.4s",	0x000000,	0x020000,	CRC(c73986d4) SHA1(d29345077753bda53560dedc95dd23f329e521d9) )

	ROM_REGION( 0x400000, "gfx2", 0 ) /* background gfx */
	ROM_LOAD( "dx_back1.1s",   0x000000, 0x200000, CRC(90970355) SHA1(d71d57cd550a800f583550365102adb7b1b779fc) )
	ROM_LOAD( "dx_back2.2s",   0x200000, 0x200000, CRC(5799af3e) SHA1(85d6532abd769da77bcba70bd2e77915af40f987) )

	ROM_REGION( 0x800000, "gfx3", 0 ) /* sprite gfx (encrypted) */
	ROM_LOAD32_WORD( "obj1",        0x000000, 0x200000, CRC(ff08ef0b) SHA1(a1858430e8171ca8bab785457ef60e151b5e5cf1) ) /* Shared with original Raiden 2 */
	ROM_LOAD32_WORD( "obj2",        0x000002, 0x200000, CRC(638eb771) SHA1(9774cc070e71668d7d1d20795502dccd21ca557b) ) /* Shared with original Raiden 2 */
	ROM_LOAD32_WORD( "dx_obj3.4k",  0x400000, 0x200000, CRC(ba381227) SHA1(dfc4d659aca1722a981fa56a31afabe66f444d5d) )
	ROM_LOAD32_WORD( "dx_obj4.6k",  0x400002, 0x200000, CRC(65e50d19) SHA1(c46147b4132abce7314b46bf419ce4773e024b05) )

	ROM_REGION( 0x100000, "oki1", 0 )	/* ADPCM samples */
	ROM_LOAD( "dx_6.3b",   0x00000, 0x40000, CRC(9a9196da) SHA1(3d1ee67fb0d40a231ce04d10718f07ffb76db455) )

	ROM_REGION( 0x100000, "oki2", 0 )	/* ADPCM samples */
	ROM_LOAD( "dx_pcm.3a", 0x00000, 0x40000, CRC(8cf0d17e) SHA1(0fbe0b1e1ca5360c7c8329331408e3d799b4714c) ) /* Shared with original Raiden 2 */
ROM_END

ROM_START( raidndxu )
	ROM_REGION( 0x200000, "mainprg", 0 ) /* v30 main cpu */
	ROM_LOAD32_BYTE("1a.u1210", 0x000000, 0x80000, CRC(53e63194) SHA1(a957330e14649cf46ad27fb99c460576c59e60b1) )
	ROM_LOAD32_BYTE("2a.u1211", 0x000001, 0x80000, CRC(ec8d1647) SHA1(5ceae132c6c09d6bb8565e9141ee1170bbdfd5fc) )
	ROM_LOAD32_BYTE("3a.u129",  0x000002, 0x80000, CRC(7dbfd73d) SHA1(43cb1dbc3ccbded64fc300c262d1fd528e0391a2) )
	ROM_LOAD32_BYTE("4a.u1212", 0x000003, 0x80000, CRC(cb41a459) SHA1(532f0ed00a5b50a7537e5f48884d632aa5b92fb0) )

	ROM_REGION( 0x40000, "user2", 0 )	/* COPX */
	ROM_LOAD( "copx-d2.6s",   0x00000, 0x40000, CRC(a6732ff9) SHA1(c4856ec77869d9098da24b1bb3d7d58bb74b4cda) ) /* Shared with original Raiden 2 */

	ROM_REGION( 0x20000, "audiocpu", ROMREGION_ERASEFF ) /* 64k code for sound Z80 */
	ROM_LOAD( "dx_5.5b",  0x000000, 0x08000,  CRC(8c46857a) SHA1(8b269cb20adf960ba4eb594d8add7739dbc9a837) )
	ROM_CONTINUE(0x10000,0x8000)
	ROM_COPY( "audiocpu", 0, 0x018000, 0x08000 )

	ROM_REGION( 0x020000, "gfx1", 0 ) /* chars */
	ROM_LOAD( "dx_7.4s",	0x000000,	0x020000,	CRC(c73986d4) SHA1(d29345077753bda53560dedc95dd23f329e521d9) )

	ROM_REGION( 0x400000, "gfx2", 0 ) /* background gfx */
	ROM_LOAD( "dx_back1.1s",   0x000000, 0x200000, CRC(90970355) SHA1(d71d57cd550a800f583550365102adb7b1b779fc) )
	ROM_LOAD( "dx_back2.2s",   0x200000, 0x200000, CRC(5799af3e) SHA1(85d6532abd769da77bcba70bd2e77915af40f987) )

	ROM_REGION( 0x800000, "gfx3", 0 ) /* sprite gfx (encrypted) */
	ROM_LOAD32_WORD( "obj1",        0x000000, 0x200000, CRC(ff08ef0b) SHA1(a1858430e8171ca8bab785457ef60e151b5e5cf1) ) /* Shared with original Raiden 2 */
	ROM_LOAD32_WORD( "obj2",        0x000002, 0x200000, CRC(638eb771) SHA1(9774cc070e71668d7d1d20795502dccd21ca557b) ) /* Shared with original Raiden 2 */
	ROM_LOAD32_WORD( "dx_obj3.4k",  0x400000, 0x200000, CRC(ba381227) SHA1(dfc4d659aca1722a981fa56a31afabe66f444d5d) )
	ROM_LOAD32_WORD( "dx_obj4.6k",  0x400002, 0x200000, CRC(65e50d19) SHA1(c46147b4132abce7314b46bf419ce4773e024b05) )

	ROM_REGION( 0x100000, "oki1", 0 )	/* ADPCM samples */
	ROM_LOAD( "dx_6.3b",   0x00000, 0x40000, CRC(9a9196da) SHA1(3d1ee67fb0d40a231ce04d10718f07ffb76db455) )

	ROM_REGION( 0x100000, "oki2", 0 )	/* ADPCM samples */
	ROM_LOAD( "dx_pcm.3a", 0x00000, 0x40000, CRC(8cf0d17e) SHA1(0fbe0b1e1ca5360c7c8329331408e3d799b4714c) ) /* Shared with original Raiden 2 */
ROM_END

ROM_START( raidndxg )
	ROM_REGION( 0x200000, "mainprg", 0 ) /* v30 main cpu */
	ROM_LOAD32_BYTE("1d.u1210", 0x000000, 0x80000, CRC(14d725fc) SHA1(f12806f64f069fdc4ee29b309a32f7ca00b36f93) )
	ROM_LOAD32_BYTE("2d.u1211", 0x000001, 0x80000, CRC(5e7e45cb) SHA1(94eff893b5335c522f1c063c3175b9bac87b0a25) )
	ROM_LOAD32_BYTE("3d.u129",  0x000002, 0x80000, CRC(f0a47e67) SHA1(8cbd21993077b2e01295db6e343cae9e0e4bfefe) )
	ROM_LOAD32_BYTE("4d.u1212", 0x000003, 0x80000, CRC(6bde6edc) SHA1(c3565a55b858c10659fd9b93b1cd92bc39e6446d) )

	ROM_REGION( 0x40000, "user2", 0 )	/* COPX */
	ROM_LOAD( "copx-d2.u0313", 0x00000, 0x40000, CRC(a6732ff9) SHA1(c4856ec77869d9098da24b1bb3d7d58bb74b4cda) ) /* Shared with original Raiden 2 */

	ROM_REGION( 0x20000, "audiocpu", ROMREGION_ERASEFF ) /* 64k code for sound Z80 */
	ROM_LOAD( "5.u1110", 0x000000, 0x08000, CRC(8c46857a) SHA1(8b269cb20adf960ba4eb594d8add7739dbc9a837) )
	ROM_CONTINUE(0x10000,0x8000)
	ROM_COPY( "audiocpu", 0, 0x018000, 0x08000 )

	ROM_REGION( 0x020000, "gfx1", 0 ) /* chars */
	ROM_LOAD( "7.u0724", 0x000000, 0x020000, CRC(c73986d4) SHA1(d29345077753bda53560dedc95dd23f329e521d9) )

	ROM_REGION( 0x400000, "gfx2", 0 ) /* background gfx */
	ROM_LOAD( "dx_back-1.u075",  0x000000, 0x200000, CRC(90970355) SHA1(d71d57cd550a800f583550365102adb7b1b779fc) )
	ROM_LOAD( "dx_back-2.u0714", 0x200000, 0x200000, CRC(5799af3e) SHA1(85d6532abd769da77bcba70bd2e77915af40f987) )

	ROM_REGION( 0x800000, "gfx3", 0 ) /* sprite gfx (encrypted) */
	ROM_LOAD32_WORD( "raiden_2_seibu_obj-1.u0811", 0x000000, 0x200000, CRC(ff08ef0b) SHA1(a1858430e8171ca8bab785457ef60e151b5e5cf1) ) /* Shared with original Raiden 2 */
	ROM_LOAD32_WORD( "raiden_2_seibu_obj-2.u082",  0x000002, 0x200000, CRC(638eb771) SHA1(9774cc070e71668d7d1d20795502dccd21ca557b) ) /* Shared with original Raiden 2 */
	ROM_LOAD32_WORD( "dx_obj-3.u0837", 0x400000, 0x200000, CRC(ba381227) SHA1(dfc4d659aca1722a981fa56a31afabe66f444d5d) )
	ROM_LOAD32_WORD( "dx_obj-4.u0836", 0x400002, 0x200000, CRC(65e50d19) SHA1(c46147b4132abce7314b46bf419ce4773e024b05) )

	ROM_REGION( 0x100000, "oki1", 0 )	/* ADPCM samples */
	ROM_LOAD( "6.u1017", 0x00000, 0x40000, CRC(9a9196da) SHA1(3d1ee67fb0d40a231ce04d10718f07ffb76db455) )

	ROM_REGION( 0x100000, "oki2", 0 )	/* ADPCM samples */
	ROM_LOAD( "pcm.u1018", 0x00000, 0x40000, CRC(8cf0d17e) SHA1(0fbe0b1e1ca5360c7c8329331408e3d799b4714c) ) /* Shared with original Raiden 2 */
ROM_END


/* Zero Team sets */
/* Zero team is slightly older hardware (early 93 instead of late 93) but
almost identical to raiden 2 with a few key differences:
Zero Team:                     Raiden 2:
BG/FG roms marked MUSHA        BG/FG roms marked RAIDEN 2
SEI251 fg/sprite gate array    SEI252 fg/sprite gate array
about 15 74xx logic chips      SEI360 gate array
3x dipswitch arrays            2x dipswitch arrays
4x 8bit program roms           2x 16bit program roms (some older pcbs have 4x 8bit like zt)
YM3812 plus Y3014              YM2151 plus Y3012 plus NJM4550 (some older pcbs have YM2151, Y3014)
1x OKI M6295 & voice rom       2x OKI M6295s & 2x voice roms
2x 8bit licensee bgroms        1x 16bit licensee bgrom
2x fg/sprite mask roms         4x fg/sprite mask roms
4x pals (two are stacked)      2x pals
*/
/* ZERO TEAM Seibu Kaihatsu 1993

(C) 1993 ZERO TEAM SEIBU KAIHATSU INC.,o
|----------------------------------------------------------|
|      1    2   3   4   5   6    7      8      9     10    |
|LA4460    M6295   6   Z8400A          BATTERY3.6v        A|
|   YM3812       LH5116 5                                 B|
|     VOL   YM3014                                         |
|HB-45A         |------|                               C|
|HB-2           |SIE150|    LH5116    |---------|          |
|RC220          |      |    LH5116    | SEI251  | 28.6360 D|
|RC220          |------|              |SB03-012 | MHz      |
|RC220                                |(QFP208) |         E|
|J                     OBJ-2    OBJ-1 |         |         F|
|A                                    |---------|          |
|M                                                        G|
|M                                              LH522258   |
|A                                              LH522258  H|
|                                               LH522258  J|
|               DSW1(8) PAL2  1       2         LH522258  K|
|           DSW2(8)     PAL14 4       3     |---------|   L|
|           DSW3(8) PAL3                    |SEI1000  |   M|
|                           COPX-D2         |SB01-001 |   N|
|                                           |(QFP184) |    |
|         CXK5863                           |         |   P|
|         CXK5863      |------|             |---------|    |
|                      |SEI200|                           Q|
|                      |      |  8             |----|     R|
|                      |------|                |V30 |      |
|       BACK-2     BACK-1        7             |----|     S|
|----------------------------------------------------------|
Notes:
      V30 clock    - 16.000MHz [32/2]. Chip is stamped "NEC D70116HG-16 V30 NEC '84" (QFP52)
      Z80 clock    - 3.579545MHz [28.63636/8]
      YM3812 clock - 3.579545MHz [28.63636/8]
      Yamaha DAC   - ym3014 mono dac
      M6295 clocks - 1.022MHz [28.63636/28] and pin 7 HIGH
      LH52258      - Sharp LH52258 32k x8 SRAM (= 62256)
      CXK5863      - Sony CXK5863 8k x8 SRAM (= 6264)
      6116         - 2k x8 SRAM
      LH5116       - 2k x8 SRAM
      HB-45A       - Seibu custom ceramic module sound DAC (SIP20)
      HB-2         - Seibu custom ceramic module connected to coin counters (SIP10)
      RC220        - Custom resistor network module used for inputs (SIP14)
      VSync        - 55.4859Hz  \
      HSync        - 15.5586kHz / not measured but assumed same as Raiden 2 DX
      PAL14        - Two pals in a stack, along with a resistor and wires to sei0200 and the sie150
                     'V3C004X'  (DIP20), has a resistor between one pin and gnd
                     <unknown, maybe V3C001, under above pal> (DIP20) u0310
      PAL2         - TIBPAL16L8-25CN stamped 'V3C002' (DIP20) u0322
      PAL3         - AMI 18CV8P-15 stamped 'V3C003' (DIP20) u0619
      ROMs         - 6         - 27C020 EPROM labelled 'SEIBU 6' at location U105 (DIP32), pcb labeled VOICE
                     5         - 27C512 EPROM labelled 'SEIBU 5' at location U1110 (DIP28)
                     *OBJ-1    - 16Mbit TC5316200BP MaskROM stamped 'MUSHA OBJ-1' at location U0811 (DIP42)
                     *OBJ-2    - 16Mbit TC5316200BP MaskROM stamped 'MUSHA OBJ-2' at location U082 (DIP42)
                     1         - 27C020 EPROM labelled 'SEIBU 1' at location U024 (DIP32)
                     2         - 27C020 EPROM labelled 'SEIBU 2' at location U025 (DIP32)
                     3         - 27C020 EPROM labelled 'SEIBU 3' at location U023 (DIP32)
                     4         - 27C020 EPROM labelled 'SEIBU 4' at location U026 (DIP32)
                     *BACK-1   - 8Mbit TC538200AP MaskROM stamped 'MUSHA BACK-1' at location U075 (DIP42)
                     *BACK-2   - 4Mbit TC534200AP MaskROM stamped 'MUSHA BACK-2' at location U0714 (DIP40)
                     7         - 27C512 EPROM labelled 'SEIBU 7' at location U072 (DIP28)
                     8         - 27C512 EPROM labelled 'SEIBU 8' at location U077 (DIP28)
                     *COPX-D2  - 2M MaskROM stamped 'COPX-D2' at location U0313 (DIP40)

                     * = these ROMs are soldered-in

      SEIBU Custom ICs -
                        SIE150 (QFP100) - z80 interface
                        SEI251 SB03-012 (QFP208) - fg/sprite gfx and its decryption
                        SEI0200 TC110G21AF 0076 (QFP100) - bg gfx
                        SEI1000 SB01-001 (QFP184) - main protection

*/

ROM_START( zeroteam ) // Fabtek, US licensee, displays 'USA' under zero team logo, board had serial 'Seibu Kaihatsu No. 0001468' on it, as well as AAMA 0458657
	ROM_REGION( 0x200000, "mainprg", 0 ) /* v30 main cpu */
	ROM_LOAD32_BYTE("seibu__1.u024.5k",   0x000000, 0x40000, CRC(25aa5ba4) SHA1(40e6047620fbd195c87ac3763569af099096eff9) ) // alternate label "1"
	ROM_LOAD32_BYTE("seibu__3.u023.6k",   0x000002, 0x40000, CRC(ec79a12b) SHA1(515026a2fca92555284ac49818499af7395783d3) ) // alternate label "3"
	ROM_LOAD32_BYTE("seibu__2.u025.6l",   0x000001, 0x40000, CRC(54f3d359) SHA1(869744185746d55c60d2f48eabe384a8499e00fd) ) // alternate label "2"
	ROM_LOAD32_BYTE("seibu__4.u026.5l",   0x000003, 0x40000, CRC(a017b8d0) SHA1(4a93ff1ab18f4b61c7ef580995f64840c19ce6b9) ) // alternate label "4"

	ROM_REGION( 0x40000, "user2", 0 )	/* COPX */
	ROM_LOAD( "copx-d2.u0313.6n",   0x00000, 0x40000, CRC(a6732ff9) SHA1(c4856ec77869d9098da24b1bb3d7d58bb74b4cda) )

	ROM_REGION( 0x20000, "audiocpu", ROMREGION_ERASEFF ) /* 64k code for sound Z80 */
	ROM_LOAD( "seibu__5.u1110.5b",  0x000000, 0x08000, CRC(7ec1fbc3) SHA1(48299d6530f641b18764cc49e283c347d0918a47) ) // // alternate label "5"
	ROM_CONTINUE(0x10000,0x8000)
	ROM_COPY( "audiocpu", 0, 0x018000, 0x08000 )

	ROM_REGION( 0x020000, "gfx1", 0 ) /* chars */
	ROM_LOAD16_BYTE( "seibu__7.u072.5s",	0x000000,	0x010000,	CRC(9f6aa0f0) SHA1(1caad7092c07723d12a07aa363ae2aa69cb6be0d) ) // alternate label "7"
	ROM_LOAD16_BYTE( "seibu__8.u077.5r",	0x000001,	0x010000,	CRC(68f7dddc) SHA1(6938fa974c6ef028751982fdabd6a3820b0d30a8) ) // alternate label "8"

	ROM_REGION( 0x400000, "gfx2", 0 ) /* background gfx */
	ROM_LOAD( "musha_back-1.u075.4s",   0x000000, 0x100000, CRC(8b7f9219) SHA1(3412b6f8a4fe245e521ddcf185a53f2f4520eb57) )
	ROM_LOAD( "musha_back-2.u0714.2s",   0x100000, 0x080000, CRC(ce61c952) SHA1(52a843c8ba428b121fab933dd3b313b2894d80ac) )

	ROM_REGION( 0x800000, "gfx3", 0 ) /* sprite gfx (encrypted) (diff encrypt to raiden2? ) */
	ROM_LOAD32_WORD( "musha_obj-1.u0811.6f",  0x000000, 0x200000, CRC(45be8029) SHA1(adc164f9dede9a86b96a4d709e9cba7d2ad0e564) )
	ROM_LOAD32_WORD( "musha_obj-2.u082.5f",  0x000002, 0x200000, CRC(cb61c19d) SHA1(151a2ce9c32f3321a974819e9b165dddc31c8153) )

	ROM_REGION( 0x100000, "oki", 0 )	/* ADPCM samples */
	ROM_LOAD( "seibu__6.u105.4a", 0x00000, 0x40000,  CRC(48be32b1) SHA1(969d2191a3c46871ee8bf93088b3cecce3eccf0c) ) // alternate label "6"

	ROM_REGION( 0x10000, "pals", 0 )	/* PALS */
	ROM_LOAD( "v3c001.pal.u0310.jed", 0x0000, 0x288, NO_DUMP) // located UNDER v3c004x, unknown pal type
	ROM_LOAD( "v3c002.tibpal16l8-25.u0322.jed", 0x0000, 0x288, NO_DUMP)
	ROM_LOAD( "v3c003.ami18cv8p-15.u0619.jed", 0x0000, 0x288, NO_DUMP)
	ROM_LOAD( "v3c004x.ami18cv8pc-25.u0310.jed", 0x0000, 0x288, NO_DUMP) // located piggybacking on v3c001 and attached to some rework wires
ROM_END

ROM_START( zeroteama ) // No licensee, original japan?
	ROM_REGION( 0x200000, "mainprg", 0 ) /* v30 main cpu */
	ROM_LOAD32_BYTE("1.u024.5k",   0x000000, 0x40000, CRC(bd7b3f3a) SHA1(896413901a429d0efa3290f61920063c81730e9b) )
	ROM_LOAD32_BYTE("3.u023.6k",   0x000002, 0x40000, CRC(19e02822) SHA1(36c9b887eaa9b9b67d65c55e8f7eefd08fe0be15) )
	ROM_LOAD32_BYTE("2.u025.6l",   0x000001, 0x40000, CRC(0580b7e8) SHA1(d4416264aa5acdaa781ebcf51f128b3e665cc903) )
	ROM_LOAD32_BYTE("4.u026.5l",   0x000003, 0x40000, CRC(cc666385) SHA1(23a8878315b6009dcc1f27e49572e5be29f6a1a6) )

	ROM_REGION( 0x40000, "user2", 0 )	/* COPX */
	ROM_LOAD( "copx-d2.u0313.6n",   0x00000, 0x40000, CRC(a6732ff9) SHA1(c4856ec77869d9098da24b1bb3d7d58bb74b4cda) )

	ROM_REGION( 0x20000, "audiocpu", ROMREGION_ERASEFF ) /* 64k code for sound Z80 */
	ROM_LOAD( "5.a.u1110.5b",  0x000000, 0x08000, CRC(efc484ca) SHA1(c34b8e3e7f4c2967bc6414348993478ed637d338) )
	ROM_CONTINUE(0x10000,0x8000)
	ROM_COPY( "audiocpu", 0, 0x018000, 0x08000 )

	ROM_REGION( 0x020000, "gfx1", 0 ) /* chars */
	ROM_LOAD16_BYTE( "7.a.u072.5s",	0x000000,	0x010000, CRC(eb10467f) SHA1(fc7d576dc41bc878ff20f0370e669e19d54fcefb) )
	ROM_LOAD16_BYTE( "8.a.u077.5r",	0x000001,	0x010000, CRC(a0b2a09a) SHA1(9b1f6c732000b84b1ad635f332ebead5d65cc491) )

	ROM_REGION( 0x400000, "gfx2", 0 ) /* background gfx */
	ROM_LOAD( "musha_back-1.u075.4s",   0x000000, 0x100000, CRC(8b7f9219) SHA1(3412b6f8a4fe245e521ddcf185a53f2f4520eb57) )
	ROM_LOAD( "musha_back-2.u0714.2s",   0x100000, 0x080000, CRC(ce61c952) SHA1(52a843c8ba428b121fab933dd3b313b2894d80ac) )

	ROM_REGION( 0x800000, "gfx3", 0 ) /* sprite gfx (encrypted) (diff encrypt to raiden2? ) */
	ROM_LOAD32_WORD( "musha_obj-1.u0811.6f",  0x000000, 0x200000, CRC(45be8029) SHA1(adc164f9dede9a86b96a4d709e9cba7d2ad0e564) )
	ROM_LOAD32_WORD( "musha_obj-2.u082.5f",  0x000002, 0x200000, CRC(cb61c19d) SHA1(151a2ce9c32f3321a974819e9b165dddc31c8153) )

	ROM_REGION( 0x100000, "oki", 0 )	/* ADPCM samples */
	ROM_LOAD( "6.u105.4a", 0x00000, 0x40000,  CRC(48be32b1) SHA1(969d2191a3c46871ee8bf93088b3cecce3eccf0c) ) // 6.bin

	ROM_REGION( 0x10000, "pals", 0 )	/* PALS */
	ROM_LOAD( "v3c001.pal.u0310.jed", 0x0000, 0x288, NO_DUMP) // located UNDER v3c004x, unknown pal type
	ROM_LOAD( "v3c002.tibpal16l8-25.u0322.jed", 0x0000, 0x288, NO_DUMP)
	ROM_LOAD( "v3c003.ami18cv8p-15.u0619.jed", 0x0000, 0x288, NO_DUMP)
	ROM_LOAD( "v3c004x.ami18cv8pc-25.u0310.jed", 0x0000, 0x288, NO_DUMP) // located piggybacking on v3c001 and attached to some rework wires
ROM_END

/* this set, consisting of updated program roms, is a later version or hack of zero team to incorporate the writing
of the fg sei251 'key data' to the pcb on bootup (like raiden 2 does) rather than relying on the sram to hold the
keys as programmed from factory (or via the suicide revival kit below); hence this romset is immune to the common
problem of the 3.6v lithium battery dying and the missing keys to cause the sprites to show up as gibberish */
// note: it is possible *but not proven* that this specific set in mame is a frankenstein-hybrid of the japan and us
// sets, using the sound and char roms from us set and code from later japan set. This would make sense if it was dumped
// from a 'fixed, suicide free' modified us board where someone swapped in the later suicideless japan code roms.
ROM_START( zeroteamb ) // No licensee, later japan?
	ROM_REGION( 0x200000, "mainprg", 0 ) /* v30 main cpu */
	ROM_LOAD32_BYTE("1b.u024.5k",   0x000000, 0x40000, CRC(157743d0) SHA1(f9c84c9025319f76807ef0e79f1ee1599f915b45) )
	ROM_LOAD32_BYTE("3b.u023.6k",   0x000002, 0x40000, CRC(fea7e4e8) SHA1(08c4bdff82362ae4bcf86fa56fcfc384bbf82b71) )
	ROM_LOAD32_BYTE("2b.u025.6l",   0x000001, 0x40000, CRC(21d68f62) SHA1(8aa85b38e8f36057ef6c7dce5a2878958ce93ce8) )
	ROM_LOAD32_BYTE("4b.u026.5l",   0x000003, 0x40000, CRC(ce8fe6c2) SHA1(69627867c7866e43e771ab85014553117044d18d) )

	ROM_REGION( 0x40000, "user2", 0 )	/* COPX */
	ROM_LOAD( "copx-d2.u0313.6n",   0x00000, 0x40000, CRC(a6732ff9) SHA1(c4856ec77869d9098da24b1bb3d7d58bb74b4cda) )

	ROM_REGION( 0x20000, "audiocpu", ROMREGION_ERASEFF ) /* 64k code for sound Z80 */
	ROM_LOAD( "5.u1110.5b",  0x000000, 0x08000, CRC(7ec1fbc3) SHA1(48299d6530f641b18764cc49e283c347d0918a47) ) // 5.5c
	ROM_CONTINUE(0x10000,0x8000)
	ROM_COPY( "audiocpu", 0, 0x018000, 0x08000 )

	ROM_REGION( 0x020000, "gfx1", 0 ) /* chars */
	ROM_LOAD16_BYTE( "7.u072.5s",	0x000000,	0x010000,	CRC(9f6aa0f0) SHA1(1caad7092c07723d12a07aa363ae2aa69cb6be0d) )
	ROM_LOAD16_BYTE( "8.u077.5r",	0x000001,	0x010000,	CRC(68f7dddc) SHA1(6938fa974c6ef028751982fdabd6a3820b0d30a8) )

	ROM_REGION( 0x400000, "gfx2", 0 ) /* background gfx */
	ROM_LOAD( "musha_back-1.u075.4s",   0x000000, 0x100000, CRC(8b7f9219) SHA1(3412b6f8a4fe245e521ddcf185a53f2f4520eb57) )
	ROM_LOAD( "musha_back-2.u0714.2s",   0x100000, 0x080000, CRC(ce61c952) SHA1(52a843c8ba428b121fab933dd3b313b2894d80ac) )

	ROM_REGION( 0x800000, "gfx3", 0 ) /* sprite gfx (encrypted) (diff encrypt to raiden2? ) */
	ROM_LOAD32_WORD( "musha_obj-1.u0811.6f",  0x000000, 0x200000, CRC(45be8029) SHA1(adc164f9dede9a86b96a4d709e9cba7d2ad0e564) )
	ROM_LOAD32_WORD( "musha_obj-2.u082.5f",  0x000002, 0x200000, CRC(cb61c19d) SHA1(151a2ce9c32f3321a974819e9b165dddc31c8153) )

	ROM_REGION( 0x100000, "oki", 0 )	/* ADPCM samples */
	ROM_LOAD( "6.u105.4a", 0x00000, 0x40000,  CRC(48be32b1) SHA1(969d2191a3c46871ee8bf93088b3cecce3eccf0c) ) // 6.4a

	ROM_REGION( 0x10000, "pals", 0 )	/* PALS */
	ROM_LOAD( "v3c001.pal.u0310.jed", 0x0000, 0x288, NO_DUMP) // located UNDER v3c004x, unknown pal type
	ROM_LOAD( "v3c002.tibpal16l8-25.u0322.jed", 0x0000, 0x288, NO_DUMP)
	ROM_LOAD( "v3c003.ami18cv8p-15.u0619.jed", 0x0000, 0x288, NO_DUMP)
	ROM_LOAD( "v3c004x.ami18cv8pc-25.u0310.jed", 0x0000, 0x288, NO_DUMP) // located piggybacking on v3c001 and attached to some rework wires
ROM_END

ROM_START( zeroteamc ) // Liang Hwa, Taiwan licensee, no special word under logo on title
	ROM_REGION( 0x200000, "mainprg", 0 ) /* v30 main cpu */
	ROM_LOAD32_BYTE("b1.u024.5k",   0x000000, 0x40000, CRC(528de3b9) SHA1(9ca8cdc0212f2540e852d20ab4c04f68b967d024) )
	ROM_LOAD32_BYTE("b3.u023.6k",   0x000002, 0x40000, CRC(3688739a) SHA1(f98f461fb8e7804b3b4020a5e3762d36d6458a62) )
	ROM_LOAD32_BYTE("b2.u025.6l",   0x000001, 0x40000, CRC(5176015e) SHA1(6b372564b2f1b1f56cae0c98f4ca588b784bfa3d) )
	ROM_LOAD32_BYTE("b4.u026.5l",   0x000003, 0x40000, CRC(c79925cb) SHA1(aaff9f626ec61bc0ff038ebd722fe361dccc49fb) )

	ROM_REGION( 0x40000, "user2", 0 )	/* COPX */
	ROM_LOAD( "copx-d2.u0313.6n",   0x00000, 0x40000, CRC(a6732ff9) SHA1(c4856ec77869d9098da24b1bb3d7d58bb74b4cda) )

	ROM_REGION( 0x20000, "audiocpu", ROMREGION_ERASEFF ) /* 64k code for sound Z80 */
	ROM_LOAD( "5.c.u1110.5b",  0x000000, 0x08000, CRC(efc484ca) SHA1(c34b8e3e7f4c2967bc6414348993478ed637d338) )
	ROM_CONTINUE(0x10000,0x8000)
	ROM_COPY( "audiocpu", 0, 0x018000, 0x08000 )

	ROM_REGION( 0x020000, "gfx1", 0 ) /* chars */
	ROM_LOAD16_BYTE( "b7.u072.5s",	0x000000,	0x010000, CRC(30ec0241) SHA1(a0d0be9458bf97cb9764fb85c988bb816710475e) )
	ROM_LOAD16_BYTE( "b8.u077.5r",	0x000001,	0x010000, CRC(e18b3a75) SHA1(3d52bba8d47d0d9108ee79014fd64d6e856a3fde) )

	ROM_REGION( 0x400000, "gfx2", 0 ) /* background gfx */
	ROM_LOAD( "musha_back-1.u075.4s",   0x000000, 0x100000, CRC(8b7f9219) SHA1(3412b6f8a4fe245e521ddcf185a53f2f4520eb57) )
	ROM_LOAD( "musha_back-2.u0714.2s",   0x100000, 0x080000, CRC(ce61c952) SHA1(52a843c8ba428b121fab933dd3b313b2894d80ac) )

	ROM_REGION( 0x800000, "gfx3", 0 ) /* sprite gfx (encrypted) (diff encrypt to raiden2? ) */
	ROM_LOAD32_WORD( "musha_obj-1.u0811.6f",  0x000000, 0x200000, CRC(45be8029) SHA1(adc164f9dede9a86b96a4d709e9cba7d2ad0e564) )
	ROM_LOAD32_WORD( "musha_obj-2.u082.5f",  0x000002, 0x200000, CRC(cb61c19d) SHA1(151a2ce9c32f3321a974819e9b165dddc31c8153) )

	ROM_REGION( 0x100000, "oki", 0 )	/* ADPCM samples */
	ROM_LOAD( "6.c.u105.4a", 0x00000, 0x40000,  CRC(b4a6e899) SHA1(175ab656db3c3258ff10eede89890f62435d2298) )

	ROM_REGION( 0x10000, "pals", 0 )	/* PALS */
	ROM_LOAD( "v3c001.pal.u0310.jed", 0x0000, 0x288, NO_DUMP) // located UNDER v3c004x, unknown pal type
	ROM_LOAD( "v3c002.tibpal16l8-25.u0322.jed", 0x0000, 0x288, NO_DUMP)
	ROM_LOAD( "v3c003.ami18cv8p-15.u0619.jed", 0x0000, 0x288, NO_DUMP)
	ROM_LOAD( "v3c004x.ami18cv8pc-25.u0310.jed", 0x0000, 0x288, NO_DUMP) // located piggybacking on v3c001 and attached to some rework wires
ROM_END

ROM_START( zeroteamd ) // Dream Soft, Korea licensee, no special word under logo on title; board had serial 'no 1041' on it.
	// this is weird, on other zt sets the rom order is 1 3 2 4, but this one is 1 3 4 2. blame seibu or whoever marked the roms, which were labeled in pen
	ROM_REGION( 0x200000, "mainprg", 0 ) /* v30 main cpu */
	ROM_LOAD32_BYTE("1.d.u024.5k",   0x000000, 0x40000, CRC(6CC279BE) SHA1(63143BA3105D24D133E60FFDB3EDC2CEB2D5DC5B) )
	ROM_LOAD32_BYTE("3.d.u023.6k",   0x000002, 0x40000, CRC(0212400D) SHA1(28F77B5FDDB9D724B735C3FF2255BD518B166E67) )
	ROM_LOAD32_BYTE("4.d.u025.6l",   0x000001, 0x40000, CRC(08813EBB) SHA1(454779CEC2FD0E71B72F7161E7D9334893EE42DE) )
	ROM_LOAD32_BYTE("2.d.u026.5l",   0x000003, 0x40000, CRC(9236129D) SHA1(8561AB62E3593CD9353D9FFDDEDBDB77E9AE2C45) )

	ROM_REGION( 0x40000, "user2", 0 )	/* COPX */
	ROM_LOAD( "copx-d2.u0313.6n",   0x00000, 0x40000, CRC(a6732ff9) SHA1(c4856ec77869d9098da24b1bb3d7d58bb74b4cda) )

	ROM_REGION( 0x20000, "audiocpu", ROMREGION_ERASEFF ) /* 64k code for sound Z80 */
	ROM_LOAD( "512kb.u1110.5b",  0x000000, 0x08000, CRC(efc484ca) SHA1(c34b8e3e7f4c2967bc6414348993478ed637d338) ) // this is a soldered mask rom on this pcb version! the contents match the taiwan version eprom; the mask rom has no label
	ROM_CONTINUE(0x10000,0x8000)
	ROM_COPY( "audiocpu", 0, 0x018000, 0x08000 )

	ROM_REGION( 0x020000, "gfx1", 0 ) /* chars */
	ROM_LOAD16_BYTE( "512kb.u072.5s",	0x000000,	0x010000, CRC(30ec0241) SHA1(a0d0be9458bf97cb9764fb85c988bb816710475e) ) // this is a soldered mask rom on this pcb version! the contents match the taiwan version eprom; the mask rom has no label
	ROM_LOAD16_BYTE( "512kb.u077.5r",	0x000001,	0x010000, CRC(e18b3a75) SHA1(3d52bba8d47d0d9108ee79014fd64d6e856a3fde) ) // this is a soldered mask rom on this pcb version! the contents match the taiwan version eprom; the mask rom has no label

	ROM_REGION( 0x400000, "gfx2", 0 ) /* background gfx */
	ROM_LOAD( "musha_back-1.u075.4s",   0x000000, 0x100000, CRC(8b7f9219) SHA1(3412b6f8a4fe245e521ddcf185a53f2f4520eb57) )
	ROM_LOAD( "musha_back-2.u0714.2s",   0x100000, 0x080000, CRC(ce61c952) SHA1(52a843c8ba428b121fab933dd3b313b2894d80ac) )

	ROM_REGION( 0x800000, "gfx3", 0 ) /* sprite gfx (encrypted) (diff encrypt to raiden2? ) */
	ROM_LOAD32_WORD( "musha_obj-1.u0811.6f",  0x000000, 0x200000, CRC(45be8029) SHA1(adc164f9dede9a86b96a4d709e9cba7d2ad0e564) )
	ROM_LOAD32_WORD( "musha_obj-2.u082.5f",  0x000002, 0x200000, CRC(cb61c19d) SHA1(151a2ce9c32f3321a974819e9b165dddc31c8153) )

	ROM_REGION( 0x100000, "oki", 0 )	/* ADPCM samples */
	ROM_LOAD( "8.u105.4a", 0x00000, 0x40000,  CRC(b4a6e899) SHA1(175ab656db3c3258ff10eede89890f62435d2298) ) // same rom as '6' labeled one in zeroteamc above but has '8' written on label in pen

	ROM_REGION( 0x10000, "pals", 0 )	/* PALS */
	ROM_LOAD( "v3c001.pal.u0310.jed", 0x0000, 0x288, NO_DUMP) // located UNDER v3c004x, unknown pal type
	ROM_LOAD( "v3c002.tibpal16l8-25.u0322.jed", 0x0000, 0x288, NO_DUMP)
	ROM_LOAD( "v3c003.ami18cv8p-15.u0619.jed", 0x0000, 0x288, NO_DUMP)
	ROM_LOAD( "v3c004x.ami18cv8pc-25.u0310.jed", 0x0000, 0x288, NO_DUMP) // located piggybacking on v3c001 and attached to some rework wires
ROM_END
// A version of the above exists (which dr.kitty used to own) which DOES have 'Korea' under the logo on title, needs dumping

ROM_START( zeroteams ) // No license, displays 'Selection' under logo
	ROM_REGION( 0x200000, "mainprg", 0 ) /* v30 main cpu */
	ROM_LOAD32_BYTE("1_sel.bin",   0x000000, 0x40000, CRC(d99d6273) SHA1(21dccd5d71c720b8364406835812b3c9defaff6c) )
	ROM_LOAD32_BYTE("3_sel.bin",   0x000002, 0x40000, CRC(0a9fe0b1) SHA1(3588fe19788f77d07e9b5ab8182b94362ffd0024) )
	ROM_LOAD32_BYTE("2_sel.bin",   0x000001, 0x40000, CRC(4e114e74) SHA1(fcccbb68c6b7ffe8d109ed3a1ec9120d338398f9) )
	ROM_LOAD32_BYTE("4_sel.bin",   0x000003, 0x40000, CRC(0df8ba94) SHA1(e07dce6cf3c3cfe1ea3b7f01e18833c1da5ed1dc) )

	ROM_REGION( 0x40000, "user2", 0 )	/* COPX */
	ROM_LOAD( "copx-d2.u0313.6n",   0x00000, 0x40000, CRC(a6732ff9) SHA1(c4856ec77869d9098da24b1bb3d7d58bb74b4cda) )

	ROM_REGION( 0x20000, "audiocpu", ROMREGION_ERASEFF ) /* 64k code for sound Z80 */
	ROM_LOAD( "5_sel.bin",  0x000000, 0x08000, CRC(ed91046c) SHA1(de815c999aeeb814d3f091d5a9ac34ea9a388ddb) )
	ROM_CONTINUE(0x10000,0x8000)
	ROM_COPY( "audiocpu", 0, 0x018000, 0x08000 )

	ROM_REGION( 0x020000, "gfx1", 0 ) /* chars */
	ROM_LOAD16_BYTE( "7.u072.5s",	0x000000,	0x010000,	CRC(9f6aa0f0) SHA1(1caad7092c07723d12a07aa363ae2aa69cb6be0d) )
	ROM_LOAD16_BYTE( "8.u077.5r",	0x000001,	0x010000,	CRC(68f7dddc) SHA1(6938fa974c6ef028751982fdabd6a3820b0d30a8) )

	ROM_REGION( 0x400000, "gfx2", 0 ) /* background gfx */
	ROM_LOAD( "musha_back-1.u075.4s",   0x000000, 0x100000, CRC(8b7f9219) SHA1(3412b6f8a4fe245e521ddcf185a53f2f4520eb57) )
	ROM_LOAD( "musha_back-2.u0714.2s",   0x100000, 0x080000, CRC(ce61c952) SHA1(52a843c8ba428b121fab933dd3b313b2894d80ac) )

	ROM_REGION( 0x800000, "gfx3", 0 ) /* sprite gfx (encrypted) (diff encrypt to raiden2? ) */
	ROM_LOAD32_WORD( "musha_obj-1.u0811.6f",  0x000000, 0x200000, CRC(45be8029) SHA1(adc164f9dede9a86b96a4d709e9cba7d2ad0e564) )
	ROM_LOAD32_WORD( "musha_obj-2.u082.5f",  0x000002, 0x200000, CRC(cb61c19d) SHA1(151a2ce9c32f3321a974819e9b165dddc31c8153) )

	ROM_REGION( 0x100000, "oki", 0 )	/* ADPCM samples */
	ROM_LOAD( "6.u105.4a", 0x00000, 0x40000,  CRC(48be32b1) SHA1(969d2191a3c46871ee8bf93088b3cecce3eccf0c) ) // 6.bin

	ROM_REGION( 0x10000, "pals", 0 )	/* PALS */
	ROM_LOAD( "v3c001.pal.u0310.jed", 0x0000, 0x288, NO_DUMP) // located UNDER v3c004x, unknown pal type
	ROM_LOAD( "v3c002.tibpal16l8-25.u0322.jed", 0x0000, 0x288, NO_DUMP)
	ROM_LOAD( "v3c003.ami18cv8p-15.u0619.jed", 0x0000, 0x288, NO_DUMP)
	ROM_LOAD( "v3c004x.ami18cv8pc-25.u0310.jed", 0x0000, 0x288, NO_DUMP) // located piggybacking on v3c001 and attached to some rework wires
ROM_END

/*
"Zero Team Suicide Revival Kit"

As the name implies, this is used to give life again to a "suicided" ZT PCB, where the 3.6v
lithium battery which backs up the FG/sprite encryption keys has died, and the sprites display
as garbage blocks.
To use: replace the 3.6v battery with a working one, and then remove the normal four code roms
and install these instead.
Boot the pcb, it should rewrite the sei251 decryption keys and display a message on screen.
Next, turn off power and reinsert the old code roms, and the pcb should now have working sprites.
*/

ROM_START( zeroteamsr )
	ROM_REGION( 0x200000, "mainprg", 0 ) /* v30 main cpu */
	ROM_LOAD32_BYTE("zteam1.u24",   0x000000, 0x40000, CRC(c531e009) SHA1(731881fca3dc0a8269ecdd295ba7119d93c892e7) )
	ROM_LOAD32_BYTE("zteam3.u23",   0x000002, 0x40000, CRC(1f988808) SHA1(b1fcb8c96e57c4942bc032d42408d7289c6a3681) )
	ROM_LOAD32_BYTE("zteam2.u25",   0x000001, 0x40000, CRC(b7234b93) SHA1(35bc093e8ad4bce1d2130a392ed1b9487a5642a1) )
	ROM_LOAD32_BYTE("zteam4.u26",   0x000003, 0x40000, CRC(c2d26708) SHA1(d65191b40f5dd7cdbbc004e2de10134db6092fd1) )

	ROM_REGION( 0x40000, "user2", 0 )	/* COPX */
	ROM_LOAD( "copx-d2.u0313.6n",   0x00000, 0x40000, CRC(a6732ff9) SHA1(c4856ec77869d9098da24b1bb3d7d58bb74b4cda) )

	ROM_REGION( 0x20000, "audiocpu", ROMREGION_ERASEFF ) /* 64k code for sound Z80 */
	ROM_LOAD( "5.5c",  0x000000, 0x08000, CRC(7ec1fbc3) SHA1(48299d6530f641b18764cc49e283c347d0918a47) ) // 5.5c
	ROM_CONTINUE(0x10000,0x8000)
	ROM_COPY( "audiocpu", 0, 0x018000, 0x08000 )

	ROM_REGION( 0x020000, "gfx1", 0 ) /* chars */
	ROM_LOAD16_BYTE( "7.u072.5s",	0x000000,	0x010000,	CRC(9f6aa0f0) SHA1(1caad7092c07723d12a07aa363ae2aa69cb6be0d) )
	ROM_LOAD16_BYTE( "8.u077.5r",	0x000001,	0x010000,	CRC(68f7dddc) SHA1(6938fa974c6ef028751982fdabd6a3820b0d30a8) )

	ROM_REGION( 0x400000, "gfx2", 0 ) /* background gfx */
	ROM_LOAD( "musha_back-1.u075.4s",   0x000000, 0x100000, CRC(8b7f9219) SHA1(3412b6f8a4fe245e521ddcf185a53f2f4520eb57) )
	ROM_LOAD( "musha_back-2.u0714.2s",   0x100000, 0x080000, CRC(ce61c952) SHA1(52a843c8ba428b121fab933dd3b313b2894d80ac) )

	ROM_REGION( 0x800000, "gfx3", 0 ) /* sprite gfx (encrypted) (diff encrypt to raiden2? ) */
	ROM_LOAD32_WORD( "musha_obj-1.u0811.6f",  0x000000, 0x200000, CRC(45be8029) SHA1(adc164f9dede9a86b96a4d709e9cba7d2ad0e564) )
	ROM_LOAD32_WORD( "musha_obj-2.u082.5f",  0x000002, 0x200000, CRC(cb61c19d) SHA1(151a2ce9c32f3321a974819e9b165dddc31c8153) )

	ROM_REGION( 0x100000, "oki", 0 )	/* ADPCM samples */
	ROM_LOAD( "6.u105.4a", 0x00000, 0x40000,  CRC(48be32b1) SHA1(969d2191a3c46871ee8bf93088b3cecce3eccf0c) ) // 6.4a

	ROM_REGION( 0x10000, "pals", 0 )	/* PALS */
	ROM_LOAD( "v3c001.pal.u0310.jed", 0x0000, 0x288, NO_DUMP) // located UNDER v3c004x, unknown pal type
	ROM_LOAD( "v3c002.tibpal16l8-25.u0322.jed", 0x0000, 0x288, NO_DUMP)
	ROM_LOAD( "v3c003.ami18cv8p-15.u0619.jed", 0x0000, 0x288, NO_DUMP)
	ROM_LOAD( "v3c004x.ami18cv8pc-25.u0310.jed", 0x0000, 0x288, NO_DUMP) // located piggybacking on v3c001 and attached to some rework wires
ROM_END


/*

X Se Dae Quiz
Seibu/Dream Island, 1995

This game runs on a Zero Team PCB

PCB Layout
ZERO TEAM-V2 SEIBU KAIHATSU INC.
|----------------------------------------|
|LA4460 YM2151 M6295 9  Z80    Y         |
|HB-46A1 YM3014 SEI150 8                 |
|VOL      6116     6116  28.6362MHz 6116 |
|                  6116   SEI251    6116 |
|J HB-2                            62256 |
|A                  OBJ-2  OBJ-1   62256 |
|M                                 62256 |
|M      SW1(8) PAL    1      3     62256 |
|A             PAL    4      2  SEI1000  |
|   SW2(8)     6264                      |
|          PAL 6264          X           |
|   SW3(8)       SEI0200  5    D71011    |
|         7     BG-1      6         V30  |
|----------------------------------------|
Notes:
      PCB is identical to standard Zero Team PCB
      with the following differences....
      1. X - location for COPX ROM, not populated
      2. Y - location for battery, not populated
      3. NEC V30 and NEC D71011 are located on a sub board and
         the surface-mounted V30 (UPD70116) is not populated
      4. ROM7 is located in a 8M-DIP42 to 4M-DIP40 adapter and is a 27C4002 EPROM
      5. ROM8 has the top 4 pins hanging out of the DIP28 socket and is a 27C1001
         EPROM. Pins 30,31 & 32 are tied together and pin 2 is tied to the SEI150
         with a wire.
*/

ROM_START( xsedae )
	ROM_REGION( 0x200000, "mainprg", 0 ) /* v30 main cpu */
	ROM_LOAD32_BYTE("1.u024",   0x000000, 0x40000, CRC(185437f9) SHA1(e46950b6a549d11dc57105dd7d9cb512a8ecbe70) )
	ROM_LOAD32_BYTE("2.u025",   0x000001, 0x40000, CRC(a2b052df) SHA1(e8bf9ab3d5d4e601ea9386e1f2d4e017b025407e) )
	ROM_LOAD32_BYTE("3.u023",   0x000002, 0x40000, CRC(293fd6c1) SHA1(8b1a231f4bedbf9c0f347330e13fdf092b9888b4) )
	ROM_LOAD32_BYTE("4.u026",   0x000003, 0x40000, CRC(5adf20bf) SHA1(42a0bb5a460c656675b2c432c043fc61a9049276) )

	ROM_REGION( 0x40000, "user2", ROMREGION_ERASEFF )	/* COPX */
	/* Not populated */

	ROM_REGION( 0x30000, "audiocpu", ROMREGION_ERASEFF ) /* 64k code for sound Z80 */
	ROM_LOAD( "8.u1110",  0x000000, 0x08000, CRC(2dc2f81a) SHA1(0f6605042e0e295b4256b43dbdf5d53daebe1a9a) )
	ROM_CONTINUE(0x10000,0x8000)
	ROM_CONTINUE(0x20000,0x10000) // TODO
	ROM_COPY( "audiocpu", 0, 0x018000, 0x08000 )

	ROM_REGION( 0x020000, "gfx1", 0 ) /* chars */
	ROM_LOAD16_BYTE( "6.u072.5s",	0x000000,	0x010000, CRC(a788402d) SHA1(8a1ac4760cf75cd2e32c1d15f36ad15cce3d411b) )
	ROM_LOAD16_BYTE( "5.u077.5r",	0x000001,	0x010000, CRC(478deced) SHA1(88cd72cb76bbc1c4255c3dfae4b9a10af9b050b2) )

	ROM_REGION( 0x400000, "gfx2", 0 ) /* background gfx */
	ROM_LOAD( "bg-1.u075",   0x000000, 0x100000, CRC(ac087560) SHA1(b6473b20c55ec090961cfc46a024b3c5b707ec25) )
	ROM_LOAD( "7.u0714",     0x100000, 0x080000, CRC(296105dc) SHA1(c2b80d681646f504b03c2dde13e37b1d820f82d2) )

	ROM_REGION( 0x800000, "gfx3", 0 ) /* sprite gfx (not encrypted) */
	ROM_LOAD32_WORD( "obj-1.u0811",  0x000000, 0x200000, CRC(6ae993eb) SHA1(d9713c79eacb4b3ce5e82dd3ce39003e3a433d8f) )
	ROM_LOAD32_WORD( "obj-2.u082",   0x000002, 0x200000, CRC(26c806ee) SHA1(899a76a1b3f933c6f5cb6b5dcdf5b58e1b7e49c6) )

	ROM_REGION( 0x100000, "oki1", 0 )	/* ADPCM samples */
	ROM_LOAD( "9.u105.4a", 0x00000, 0x40000, CRC(a7a0c5f9) SHA1(7882681ac152642aa4f859071f195842068b214b) )

	ROM_REGION( 0x100000, "oki2", ROMREGION_ERASEFF )	/* ADPCM samples */
ROM_END

static DRIVER_INIT (raiden2)
{
	memory_configure_bank(machine, "mainbank", 0, 2, machine.region("mainprg")->base(), 0x20000);
	raiden2_decrypt_sprites(machine);
}

static DRIVER_INIT (raidendx)
{
	memory_configure_bank(machine, "mainbank", 0, 0x10, machine.region("mainprg")->base(), 0x20000);
	raiden2_decrypt_sprites(machine);
}

static DRIVER_INIT (xsedae)
{
	/* doesn't have banking */
	//memory_configure_bank(machine, "mainbank", 0, 2, machine.region("mainprg")->base(), 0x20000);
}

static DRIVER_INIT (zeroteam)
{
	memory_configure_bank(machine, "mainbank", 0, 2, machine.region("mainprg")->base(), 0x20000);
	zeroteam_decrypt_sprites(machine);
}

/* GAME DRIVERS */

// rev numbers at end of the line just indicate which sets are the same code revisions (just a region byte change), they don't reflect the actual order of release
GAME( 1993, raiden2,  0,       raiden2,  raiden2,  raiden2,  ROT270, "Seibu Kaihatsu (Fabtek license)",        "Raiden II (set 1, US Fabtek)",     GAME_NOT_WORKING) // rev 1
GAME( 1993, raiden2a, raiden2, raiden2,  raiden2,  raiden2,  ROT270, "Seibu Kaihatsu (Metrotainment license)", "Raiden II (set 2, Metrotainment)", GAME_NOT_WORKING) //  ^
GAME( 1993, raiden2b, raiden2, raiden2,  raiden2,  raiden2,  ROT270, "Seibu Kaihatsu",                         "Raiden II (set 3, Japan)",         GAME_NOT_WORKING) //  ^
GAME( 1993, raiden2c, raiden2, raiden2,  raiden2,  raiden2,  ROT270, "Seibu Kaihatsu",                         "Raiden II (set 4, Italy)",         GAME_NOT_WORKING) // rev 2
GAME( 1993, raiden2d, raiden2, raiden2,  raiden2,  raiden2,  ROT270, "Seibu Kaihatsu",                         "Raiden II (set 5, Easy Version)",  GAME_NOT_WORKING) // rev 3
GAME( 1993, raiden2e, raiden2, raiden2,  raiden2,  raiden2,  ROT270, "Seibu Kaihatsu",                         "Raiden II (set 6, Easy Version)",  GAME_NOT_WORKING) // rev 4
GAME( 1993, raiden2f, raiden2, raiden2,  raiden2,  raiden2,  ROT270, "Seibu Kaihatsu (Fabtek license)",        "Raiden II (set 7, US Fabtek, Easy Version)",     GAME_NOT_WORKING) //  ^
GAME( 1993, raiden2g, raiden2, raiden2,  raiden2,  raiden2,  ROT270, "Seibu Kaihatsu (Fabtek license)",        "Raiden II (set 8, US Fabtek, Easy Version)",         GAME_NOT_WORKING) // rev 3 and 4 mix?


GAME( 1994, raidndx,  0,       raidendx,  raidendx, raidendx,  ROT270, "Seibu Kaihatsu",                         "Raiden DX (UK)",                   GAME_NOT_WORKING)
GAME( 1994, raidndxa1,raidndx, raidendx,  raidendx, raidendx,  ROT270, "Seibu Kaihatsu (Metrotainment license)", "Raiden DX (Asia set 1)",           GAME_NOT_WORKING)
GAME( 1994, raidndxa2,raidndx, raidendx,  raidendx, raidendx,  ROT270, "Seibu Kaihatsu (Metrotainment license)", "Raiden DX (Asia set 2)",           GAME_NOT_WORKING)
GAME( 1994, raidndxj, raidndx, raidendx,  raidendx, raidendx,  ROT270, "Seibu Kaihatsu",                         "Raiden DX (Japan)",                GAME_NOT_WORKING)
GAME( 1994, raidndxu, raidndx, raidendx,  raidendx, raidendx,  ROT270, "Seibu Kaihatsu (Fabtek license)",        "Raiden DX (US)",                   GAME_NOT_WORKING)
GAME( 1994, raidndxg, raidndx, raidendx,  raidendx, raidendx,  ROT270, "Seibu Kaihatsu (Tuning license)",        "Raiden DX (Germany)",              GAME_NOT_WORKING)

GAME( 1993, zeroteam, 0,       zeroteam, zeroteam,  zeroteam,  ROT0,   "Seibu Kaihatsu", "Zero Team USA (set 1, US, Fabtek license)", GAME_NOT_WORKING)
GAME( 1993, zeroteama,zeroteam,zeroteam, zeroteam,  zeroteam,  ROT0,   "Seibu Kaihatsu", "Zero Team (set 2, Japan? (earlier?))", GAME_NOT_WORKING)
GAME( 1993, zeroteamb,zeroteam,zeroteam, zeroteam,  zeroteam,  ROT0,   "Seibu Kaihatsu", "Zero Team (set 3, Japan? (later batteryless))", GAME_NOT_WORKING) // reprograms the sprite decrypt data of the SEI251 on every boot, like raiden2 does. hack?
GAME( 1993, zeroteamc,zeroteam,zeroteam, zeroteam,  zeroteam,  ROT0,   "Seibu Kaihatsu", "Zero Team (set 4, Taiwan, Liang Hwa license)", GAME_NOT_WORKING)
GAME( 1993, zeroteamd,zeroteam,zeroteam, zeroteam,  zeroteam,  ROT0,   "Seibu Kaihatsu", "Zero Team (set 5, Korea, Dream Soft license)", GAME_NOT_WORKING)
//GAME( 1993, zeroteame,zeroteam,zeroteam, zeroteam,  zeroteam,  ROT0,   "Seibu Kaihatsu", "Zero Team Korea (set 6, Korea, Dream Soft license)", GAME_NOT_WORKING) // NOT DUMPED but seen on a board drkitty had

GAME( 1993, zeroteams,zeroteam,zeroteam, zeroteam,  zeroteam,  ROT0,   "Seibu Kaihatsu", "Zero Team Selection", GAME_NOT_WORKING)
GAME( 1993, zeroteamsr,zeroteam,zeroteam, zeroteam,  zeroteam,  ROT0,  "Seibu Kaihatsu", "Zero Team Suicide Revival Kit", GAME_NOT_WORKING) // reprograms the sprite decrypt data of the SEI251 only, no game code

GAME( 1995, xsedae,   0,       xsedae,   xsedae,  xsedae,   ROT0,   "Dream Island",   "X Se Dae Quiz", GAME_NOT_WORKING)
