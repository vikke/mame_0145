/*

  Sega Naomi / Naomi 2 / Atomiswave

  Driver by Samuele Zannoli, R. Belmont, ElSemi,
            David Haywood, Angelo Salese and Olivier Galibert
  Special thanks to CaH4e3, Deunan Knute, drkIIRaziel, Guru, Psyman and ZeZu for the help given.

 Notes:
  Several early Naomi games are running on an earlier revision mainboard (HOTD2 etc.) which appears to have an earlier
   revision of the graphic chip.  Attempting to run these games on the later board results in graphical glitches and/or
   other problems.

  Naomi 2 is backwards compatible with Naomi 1

  The later revision games (released after GD-ROM had been discontinued) require the 'h' revision bios, which checks the SH-4 ID register.

  Error 51 means that you need to change the cabinet to a different player arrangement in main test mode (usually 1p)

Sega Naomi is Dreamcast based Arcade hardware.

TODO (general):
    - all games that uses YUV just updates one frame then dies, why?
    - SH to ARM sound streaming doesn't work (used by ADX compression system)
    - ngdup23a, ngdup23c: missing DIMM emulation, hence they can't possibly work, emulate the DIMM means to add an extra SH-4 ...

    - Following games doesn't boot, any attempt makes it to return to the system test mode (almost likely to be JVS related):
    * Death Crimson OX
    * La Keyboard
    * Lupin the Third
    * Moeru Casinyo
    * Puyo Puyo DA!
    * Puyo Puyo Fever
    * Samba de Amigo
    * Samba de Amigo Ver. 2000
    * Shootout Pool
    * Shootout Pool Medal
    * Shootout Pool Prize
    * Usagi Yamashiro Hen
    * Virtual On
    * WWF Royal Rumble

    - missing inputs (needs rotary channels):
    * Crakin' DJ
    * Dynamic Golf
    * Inu no Osampo

    - wrong JVS I/O specs, doesn't boot due of it:
    * Derby Owners Club II
    * Kick '4' Cash (hopper)
    * Sega Marine Fishing
    * Wave Runner GP

    - doesn't boot consistently (JVS checksum error?):
    * Cannon Spike;
    * Heavy Metal GeoMatrix;
    * Moeru Justice Gakuen;
    * Power Stone 2;
    * Spawn;

TODO (game-specific):
    - 18th Wheeler Deluxe: "MOTOR NETWORK ERR IN 01 OUT FF" msg pops up during gameplay;
    - Airline Pilots Deluxe: returns error 03
    - Derby Owner Club: if you try to start a game, it moans about something and enters into some kind of JP test mode, pretty bogus behaviour;
    - Ferrari 355 Challenge: dies at the network check;
    - Giant Gram 2: no VMU emulation;
    - Gun Survivor 2: crashes during game loading;
    - Idol Janshi Suchie-Pai 3: returns "i/o board error" msg in game mode;
    - Monkey Ball: dies when attempts to load the gameplay;
    - Oinori-Daimyoujin Matsuri: reports "B. RAM error" in test mode, inputs doesn't seem to work after that point;
    - OutTrigger: crashes on naomibd_r();
    - Ringout 4x4: needs cabinet set to 4p, moans about not having two jamma i/o boards;
    - Super Major League '99: attract mode/gameplay bogusly have stop-motions from time to time;
    - The House of the Dead 2: game uses an earlier PVR so it has extra gfx issues;
    - The Typing of the Dead: missing keyboard inputs, doesn't enter into attract/test mode anymore (JVS issue);
    - Virtua Tennis: dies when accessing the gameplay or the attract mode (PVR or SH-4 bug, most likely);
    - World Kicks: MAME crashes even before showing Naomi logo;
    (more will come up soon ...)

---------------------------------------------------------------------------------------------------------------------------------------------------

Guru's Readme
-------------

Sega NAOMI Mainboard
Sega, 1998-2005

PCB Layout
----------
837-13544-01
171-7772F
837-13707 (sticker)
(C) SEGA 1999
|---------------------------------------------------|
|    CN1                           CN3              |
|PC910  62256   EPF8452AQC160-3                     |
|    BATTERY EPC1064   JP4  3771  93C46             |
|A179B    315-6188.IC31     3771                3773|
|ADM485  BIOS.IC27   5264165            5264165     |
|                    5264165  |-----|   5264165     |
|    CN2                      | SH4 |               |
|                             |     | 33.3333MHZ    |
|CN26                         |-----|          27MHZ|
|                                         CY2308SC-3|
|        KM416S4030      |------|     HY57V161610   |
|                        | POWER|     HY57V161610   |
| C844G        315-6232  | VR2  |             32MHZ |
|            33.8688MHZ  |------|     HY57V161610   |
|                         xMHz        HY57V161610   |
|      PCM1725    JP1                    62256      |
|                     HY57V161610                   |
|                          HY57V161610              |
|              315-6145                             |
|CN25                CY2308SC-1          315-6146   |
|          LED1                              93C46  |
|          LED2                     14.7456MHZ      |
|---------------------------------------------------|
Notes:
      CN1/2/3         - Connectors for ROM cart
      CN25/26         - Connectors for filter board
      EPF8452AQC160-3 - Altera FLEX EPF8452AQC160-3 FPGA (QFP160)
      315-6188.IC31   - Altera EPC1064 (DIP8)
                        According to the datasheet, it's an FPGA Configuration
                        Device which loads the Altera Flex EPF8452 with some info
                        on power-up.
      JP1             - set to 2-3. Alt setting is 1-2
      JP4             - set to 2-3. Alt setting is 1-2
      93C46           - 128 bytes serial EEPROM
      A179B 96K       - TI SN75179B Differential driver and receiver pair (like RS485)
      ADM485          - Analog Devices ADM485
      BIOS.IC27       - 27C160 EPROM
      5264165         - Hitachi 5264165FTTA60 (video RAM)
      HY57V161610     - Hyundai 57V161610DTC-8 (main program RAM)
      CY2308SC-3      - Clock generator IC
      KM416S4030      - Samsung KM416S4030 16MBit SDRAM (sound related RAM?)
      315-6232        - Sega Custom IC (QFP100)
      315-6145        - Sega Custom IC (QFP56)
      315-6146        - Sega Custom IC (QFP176)
      C844G           - ? (SOIC14)
      62256           - 32kx8 SRAM
      PCM1725         - Burr-Brown PCM1725
      xMHz            - Small round XTAL (possibly 32.768kHz for a clock?)
      SH4             - Hitachi SH4 CPU (BGAxxx, with heatsink)
      POWERVR2        - POWERVR2 video generator (BGAxxx, with heatsink and fan)


Filter Board
------------
839-1069
|----------------------------------------------------|
|SW2 SW1   DIPSW   CN5                      CN12 CN10|
|                                                    |
|                                                    |
|           DIN1                     DIN2            |
|                                                    |
|               CNTX  CNRX                           |
| CN9    CN8                                         |
|    CN7     CN6           CN4       CN3    CN2   CN1|
|----------------------------------------------------|
Notes:
      CN1/CN2   - Power input
      CN3       - HD15 (i.e. VGA connector) RGB Video Output @ 15kHz or 31.5kHz
      CN4       - RCA Audio Output connectors
      CN5       - USB connector (connection to I/O board)
      CN6       - 10 pin connector labelled 'MAPLE 0-1'
      CN7       - 11 pin connector labelled 'MAPLE 2-3'
      CN8       - RS422 connector
      CN9       - Midi connector
      CNTX/CNRX - Network connectors
      DIN1/DIN2 - Connectors joining to mainboard CN25/26
      SW1       - Test Switch
      SW2       - Service Switch
      DIPSW     - 4-position DIP switch block
      CN10      - 12 volt output for internal case exhaust fan
      CN11      - RGB connector (not populated)
      CN12      - 5 volt output connector



---------------------------------------------------------
Bios Version Information                                |
---------------------------------------------------------
    Bios                     |   Support | Support      |
    Label                    |   GD-ROM  | Cabinet Link |
---------------------------------------------------------
Naomi / GD-ROM               |           |              |
    EPR-21576D (and earlier) |   No      |    No        |
    EPR-21576E               |   Yes     |    No        |
    EPR-21576F               |   Yes     |    Yes       |
    EPR-21576G (and newer)   |   Yes     |    Yes       |
---------------------------------------------------------
Naomi 2 / GD-ROM             |           |              |
    EPR-23605                |   Yes     |    No        |
    EPR-23605A               |   Yes     |    Yes       |
    EPR-23605B (and newer)   |   Yes     |    Yes       |
---------------------------------------------------------



NAOMI ROM cart usage
-------------------------
There are 5 known types of carts manufactured by Sega: 171-7885A, 171-7919A, 171-7978B, 171-8132B, 171-8346C
There are also 2 types of carts manufactured by Namco: MASK-B, MASK-C

837-14124  171-7885A (C) Sega 1998
|------------------------------------------------------------|
|                                          ----CN2----       -|
|                                                      JP2    |
|                                               IC44          |
|                                                             |
|                OSC1                                  IC22   |
|                     IC41     IC42                           | male side
|                                               IC45          |
|                                                             |
|                JP3                                          |
|                   IC37                                      |
|        ----CN3----                       ----CN1----        |
|-------------------------------------------------------------|

 |------------------------------------------------------------|
|-       ----CN2----                                          |
|                                                             |
|IC11S IC10S IC9S IC8S IC7S IC6S IC5S IC4S IC3S IC2S IC1S     |
|ROM11 ROM10 ROM9 ROM8 ROM7 ROM6 ROM5 ROM4 ROM3 ROM2 ROM1     |
|                                                             |
| IC21S IC20S IC19S IC18S IC17S IC16S IC15S IC14S IC13S IC12S | female side
| ROM21 ROM20 ROM19 ROM18 ROM17 ROM16 ROM15 ROM14 ROM13 ROM12 |
|                                                             |
|                                                             |
|                                                             |
|        ----CN1----                       ----CN3----        |
|-------------------------------------------------------------|
Notes:
      OSC1  - oscillator 28.000MHz
      JP2   - JUMPER unknown function
      JP3   - JUMPER unknown function
 IC1S-IC21S - FlashROM (SOP56), either 32Mb or 64Mb. Not all positions are populated
      IC37  - FlashROM (SOIC8) Xicor X76F100 Secure SerialFlash
      IC41  - Sega 315-6206 Altera MAX EPM7064S (QFP100)
      IC42  - SEGA 315-5581 (QFP100). Probably some kind of FPGA or CPLD. Usually different per game
              On the end of the number, -JPN means it requires Japanese BIOS, -COM will run with any BIOS
 IC44-IC45  - SRAM (SOJ28) 32kx8, either IDT71256 or CY7C199
   CN1/2/3  - connectors joining to main board

Games known to use this PCB include....
                                     Sticker    EPROM   MASKROMs    X76F100  EPM7064S  FPGA
Game                                 on cart    IC22#   # of SOP56  IC37#    IC41#     IC42#         Notes
-------------------------------------------------------------------------------------------------------------------------
Inu No Osanpo / Dog Walking (Rev A)  840-0073C  22294A  16 (64Mb)   present  present   317-0316-JPN
Soul Surfer (Rev A)                  840-0095C  23838C  21 (64Mb)   present  present   not present   todo: verify if it's Rev A or Rev C
The King of Route 66 (Rev A)         840-0087C  23819A  20 (64Mb)   present  present   not present



837-13668  171-7919A (C) Sega 1998
|---------------------------------------------------------|
|                                       ----CN2----       -|
|IC1   IC2   IC3   IC4   IC5   IC6                      JP1|
|ROM1  ROM2  ROM3  ROM4  ROM5  ROM6            IC42        |
|                                        IC45              |
|                                        IC44         IC22 |
|            IC7  IC8  IC9  IC10  IC11                     | male side
|            ROM7 ROM8 ROM9 ROM10 ROM11        IC41        |
|                                                          |
|                                              28MHz       |
|                                                          |
|        ----CN3----        IC37        ----CN1----        |
|----------------------------------------------------------|
Notes:
      The other side of the cart PCB just has more locations for
      SOP44 MASKROMs... IC12S to IC21S (ROM12 to ROM21)

  IC1-IC21S - MaskROM (SOP44), either 32Mb or 64Mb. Not all positions are populated
      IC22  - EPROM (DIP42), either 27C160 or 27C322
      JP1   - JUMPER Sets the size of the EPROM. 1-2 = 32M, 2-3 = 16M
      IC37  - FlashROM (SOIC8) Xicor X76F100 Secure SerialFlash
      IC41  - Sega 315-6213 Xilinx XC9536 (PLCC44)
      IC42  - SEGA 315-5581 (QFP100). Probably some kind of FPGA or CPLD. Usually different per game
              On the end of the number, -JPN means it requires Japanese BIOS, -COM will run with any BIOS
 IC44-IC45  - SRAM (SOJ28) 32kx8, either IDT71256 or CY7C199
   CN1/2/3  - connectors joining to main board

Games known to use this PCB include....
                                                Sticker    EPROM   MASKROMs    X76F100  XC9536    315-5581
Game                                            on cart    IC22#   # of SOP44  IC37#    IC41#     IC42#          Notes
-------------------------------------------------------------------------------------------------------------------------
18 Wheeler (Rev A)                              840-0037C  23298?  20 (64Mb)   ?        315-6213  317-0273-COM
18 Wheeler Deluxe (Rev A)                       840-0023C  22185A  20 (64Mb)   ?        315-6213  317-0273-COM
Airline Pilots (Rev A)                          840-0005C  21739A  11 (64Mb)   ?        315-6213  317-0251-COM
Airline Pilots Deluxe (Rev B)                   ?          21787B  11 (64Mb)   present  315-6213  317-0251-COM   2 know BIOS 21801 (USA), 21802 (EXP)
Cosmic Smash                                    840-0044C  23428    8 (64Mb)   ?        315-6213  317-0289-COM   joystick + 2 buttons
Cosmic Smash (Rev A)                            840-0044C  23428A   8 (64Mb)   ?        315-6213  317-0289-COM   joystick + 2 buttons
Crazy Taxi                                      840-0002C  21684   13 (64Mb)   ?        315-6213  317-0248-COM
Dead Or Alive 2                                 841-0003C  22121   21 (64Mb)   present  315-6213  317-5048-COM   joystick + 3 buttons
Dead Or Alive 2 Millennium                      841-0003C  DOA2 M  21 (64Mb)   present  315-6213  317-5048-COM   joystick + 3 buttons
Death Crimson OX                                841-0016C  23524   10 (64Mb)   present  315-6213  317-5066-COM
Dengen Tenshi Taisen Janshi Shangri-La          841-0004C  22060   12 (64Mb)   ?        315-6213  317-5050-JPN
Derby Owners Club (Rev B)                       840-0016C  22099B  14 (64Mb)   ?        315-6213  317-0262-JPN   touch panel + 2 buttons + card reader
Derby Owners Club 2000 Ver.2 (Rev A)            840-0052C  22284A  16 (64Mb)   present  315-6213  not present
Dynamite Baseball '99 / World Series'99 (Rev B) 840-0019C  22141B  19 (64Mb)   ?        315-6213  317-0269-JPN   requires special panel (joystick + 2 buttons + bat controller for each player)
Dynamite Baseball Naomi                         840-0001C  21575   21 (64Mb)   ?        315-6213  317-0246-JPN   requires special panel (joystick + 2 buttons + bat controller for each player)
Ferrari F355 Challenge                          834-13842  21902   21 (64Mb)   ?        315-6213  317-0254-COM
Ferrari F355 Challenge 2 Twin                   840-0042C  23399   21 (64Mb)   ?        315-6213  317-0287-COM   2 know BIOS 22850 (USA), 22851 (EXP)
Ferrari F355 Challenge Twin                     834-13950  22848   21 (64Mb)   ?        315-6213  317-0267-COM   2 know BIOS 22850 (USA), 22851 (EXP)
Giant Gram: All Japan Pro Wrestling 2           840-0007C  21820    9 (64Mb)   ?        315-6213  317-0253-JPN   joystick + 3 buttons
Guilty Gear X                                   841-0013C  23356   14 (64Mb)   ?        315-6213  317-5063-COM
Gun Spike / Cannon Spike                        841-0012C  23210   12 (64Mb)   present  315-6213  317-5060-COM
Heavy Metal Geo Matrix (Rev A)                  HMG016007  23716A  11 (64Mb)   present  315-6213  317-5071-COM   joystick + 2 buttons
Idol Janshi Suchie-Pai 3                        841-0002C  21979   14 (64Mb)   ?        315-6213  317-5047-JPN   requires special I/O board and mahjong panel
Jambo! Safari (Rev A)                           840-0013C  22826A   8 (64Mb)   ?        315-6213  317-0264-COM
OutTrigger                                      840-0017C  22163   19 (64Mb)   ?        315-6213  317-0266-COM   requires analog controllers/special panel
Power Stone                                     841-0001C  21597    8 (64Mb)   present  315-6213  317-5046-COM   joystick + 3 buttons
Power Stone 2                                   841-0008C  23127    9 (64Mb)   present  315-6213  317-5054-COM   joystick + 3 buttons
Puyo Puyo Da!                                   841-0006C  22206   20 (64Mb)   ?        ?         ?
Samba de Amigo (Rev B)                          840-0020C  22966B  16 (64Mb)   ?        315-6213  317-0270-COM   will boot but requires special controller to play it
Sega Marine Fishing                             840-0027C  22221   10 (64Mb)   ?        315-6213  not present    ROM 3&4 not present. Requires special I/O board and fishing controller
Sega Strike Fighter (Rev A)                     840-0035C  23323A  20 (64Mb)   ?        315-6213  317-0281-COM
Sega Tetris                                     840-0018C  22909    6 (64Mb)   present  315-6213  317-0268-COM
Slashout                                        840-0041C  23341   17 (64Mb)   ?        315-6213  317-0286-COM   joystick + 4 buttons
Spawn In the Demon's Hand (Rev B)               841-0005C  22977B  10 (64Mb)   ?        315-6213  317-5051-COM   joystick + 4 buttons
The Typing of the Dead (Rev A)                  840-0026C  23021A  20 (64Mb)   present  315-6213  not present
Toy Fighter / Waffupu                           840-0011C  22035   10 (64Mb)   ?        315-6212  317-0257-COM   joystick + 3 buttons
Virtua NBA                                      840-0021C  23073   21 (64Mb)   present  315-6213  not present
Virtua NBA (original)                           840-0021C  23073   21 (64Mb)   ?        315-6213  not present
Virtua Striker 2 Ver. 2000 (Rev C)              840-0010C  21929C  14 (64Mb)   present  315-6213  317-0258-COM   joystick + 3 buttons (+1x 32Mb)
Virtua Tennis / Power Smash                     840-0015C  22927   11 (64Mb)   present  315-6213  317-0263-COM
Zombie Revenge                                  840-0003C  21707   19 (64Mb)   ?        315-6213  317-0249-COM   joystick + 3 buttons



837-13801  171-7978B (C) Sega 1999
|---------------------------------------------------------|
|                                       ----CN2----       -|
|                                                     JP1  |
|IC18  IC20  IC22  IC24  IC26  IC28            OSC1        |
|ROM2  ROM4  ROM6  ROM8  ROM10 ROM12                       |
|                                            ----     IC11 |
|                                           |IC1 |         | male side
|   IC29  IC31  IC33  IC35  IC37            |    |         |
|   ROM13 ROM15 ROM17 ROM19 ROM21            ----          |
|                                    IC14        IC15      |
|                                                          |
|        ----CN3----                    ----CN1----        |
|----------------------------------------------------------|

 |---------------------------------------------------------|
|-       ----CN2----                                       |
|                                                          |
|                IC27S  IC25S  IC23S  IC21S  IC19S  IC17S  |
|                ROM11  ROM9   ROM7   ROM5   ROM3   ROM1   |
|                                                          |
|                                                          | female side
|                   IC38S  IC36S  IC34S  IC32S  IC30S      |
|       IC13S       ROM22  ROM20  ROM18  ROM16  ROM14      |
|                                                          |
|                                                          |
|        ----CN1----                    ----CN3----        |
|----------------------------------------------------------|
Notes:

      OSC1  - oscillator 50.0000MHz
      IC1   - FPGA ACTEL A54SX32A (QFP208) SEGA part number 317-xxxx-yyy
              On the end of the number, -JPN means it requires Japanese BIOS, -COM will run with any BIOS
      IC11  - EPROM (DIP42), either 27C160 or 27C322
      JP1   - JUMPER Sets the size of the EPROM. 1-2 = 32M, 2-3 = 16M
      IC13S - EEPROM (SOIC8) 25LC040 serial EEPROM
      IC14  - 256 x 9 AsyncFIFO, 5.0V (SOP28)
      IC15  - SRAM (SOJ28) 32kx8, IDT71256
IC17S-IC38S - MaskROM (SOP44), either 32Mb or 64Mb. Not all positions are populated
   CN1/2/3  - connectors joining to main board

Games known to use this PCB include....
                                               Sticker    EPROM   MASKROMs   25LC040  A54SX32
Game                                           on cart    IC11#   # of SOP44 IC13S#   IC1#          Notes
----------------------------------------------------------------------------------------------------------------------------
Giant Gram 2000                                840-0039C  23377   20 (64Mb)  present  317-0296-COM
Kick '4' Cash                                  840-0140C  24212   16 (64Mb)  present  317-0397-COM
Marvel Vs. Capcom 2 New Age of Heroes (Rev A)  841-0007C  23085A  14 (64Mb)  present  317-5058-COM  +2x 32Mb (full cart #:841-0007C-03)
MushiKing The King of Beetles 2K3 2ND          840-0150C  24217    6 (64Mb)  present  317-0394-COM
Quiz Ah Megamisama                             840-0030C  23227   16 (64Mb)  present  317-0280-JPN
Shootout Pool                                  840-0098C  23844    4 (64mb)  present  317-0336-COM
Shootout Pool - Shootout Pool Prize            840-0128C  24065    4 (64mb)  present  317-0367-COM
Shootout Pool Medal                            ?          24148    4 (64Mb)  ?        ?
Touch de UNO! 2                                840-0022C  23071    6 (64Mb)  present  317-0276-JPN
Virtua Fighter 4 Evolution                     840-0106B  23934   20 (64Mb)  present  317-0339-COM
Virtua Tennis 2 / Power Smash 2 (Rev A)        840-0084C  22327A  18 (64Mb)  present  317-0320-COM



837-14114-01  171-8132B (C) Sega 2000
|---------------------------------------------------------|
|      IC11  IC10  IC9                  ----CN2----       -|
|      21/22 19/20 17/18            IC44 IC45              |
|                                            IC27          |
|                                                          |
|IC8   IC7   IC6   IC5                       JP2      IC22 |
|15/16 13/14 11/12 9/10                                    | male side
|                                            IC41 OSC1     |
|IC4   IC3   IC2   IC1               IC42          JP1     |
|7/8   5/6   3/4   1/2                                     |
|                                                          |
|        ----CN3----                    ----CN1----        |
|----------------------------------------------------------|
Notes:
      The female side of the cart PCB only has traces

      OSC1  - oscillator 28.000MHz
  IC1-IC11  - MaskROM (TSOP48), 128Mb. Not all positions are populated
      IC22  - EPROM (DIP42), either 27C160 or 27C322
      JP1   - JUMPER Sets the size of the EPROM. 1-2 = 32M, 2-3 = 16M
      IC27  - PLD Sega 315-6319A ALTERA EPM7032 (PLCC44)
      IC41  - Sega 315-6213 Xilinx XC9536 (PLCC44)
      IC42  - SEGA 315-5581 (QFP100). Probably some kind of FPGA or CPLD. Usually different per game
              On the end of the number, -JPN means it requires Japanese BIOS, -COM will run with any BIOS
 IC44-IC45  - SRAM (SOJ28) 32kx8, either IDT71256 or CY7C199
      JP2   - JUMPER unknown function
   CN1/2/3  - connectors joining to main board

   Games known to use this PCB include....
                                                Sticker    EPROM   MASKROMs    EPM7032    315-6213  315-5581
Game                                            on cart    IC22#   # of SOP44  IC27#      IC41#     IC42#         Notes
----------------------------------------------------------------------------------------------------------------------------
Alien Front (Rev A)                             840-0048C  23586A   5 (128Mb)  315-6319A  present   317-0293-COM
Alien Front (Rev T)                             840-0048C  23586T   5 (128Mb)  315-6319A  present   317-0293-COM
Capcom Vs. SNK Millennium Fight 2000 (Rev A)    841-0011C  23511A   7 (128Mb)  315-6219   present   317-5059-COM  (000804)
Capcom Vs. SNK Millennium Fight 2000 (Rev C)    841-0011C  23511C   7 (128Mb)  315-6319   present   317-5059-COM  (000904)
Club Kart: European Session (Rev D)             840-0062C  21473D  11 (128Mb)  315-6319A  present   317-0313-COM
Derby Owners Club II (Rev B)                    840-0083C  22306B  11 (128Mb)  315-6319A  present   not present
Derby Owners Club World Edition (Rev C)         840-0088C  22336C   7 (128Mb)  315-6319A  present   not present
Derby Owners Club World Edition (Rev D)         840-0088C  22336D   7 (128Mb)  315-6319A  present   not present   2 MaskROM are different from Rev C
Giga Wing 2                                     841-0014C  22270    5 (128Mb)  315-6319A  present   317-5064-COM
Mobile Suit Gundam: Federation Vs. Zeon         841-0017C  23638   10 (128Mb)  ?          ?         ?
Moero Justice Gakuen / Project Justice (Rev A)  841-0015C  23548A  11 (128Mb)  315-6319A  present   317-5065-COM
Oinori-daimyoujin Matsuri                       840-0126B  24053    5 (128Mb)  315-6319A  present   not present
Samba de Amigo Ver.2000                         840-0047C  23600   21 (128Mb)  315-6319A  present   317-0295-COM
Virtua Striker 3 (Rev B)                        840-0061C  23663B  11 (128Mb)  315-6319A  present   317-0310-COM
Virtua Striker 3 (Rev C)                        840-0061C  23663C  11 (128Mb)  315-6319A  present   317-0310-COM
Wave Runner GP                                  840-0064C  24059    6 (128Mb)  315-6319A  present   not present
Wild Riders                                     840-0046C  23622   10 (128Mb)  315-6319A  present   317-0301-COM
WWF Royal Rumble                                840-0040C  22261    8 (128Mb)  315-6319   present   317-0285-COM
Zero Gunner 2                                   841-0020C  23689    5 (128Mb)  315-6319A  present   317-5073-COM



171-8346C (C) Sega 2005
|---------------------------------------------------------|
|  IC12          IC8                    ----CN2----       -|
|             (IC22,1-7)                    IC7            |
|                          IC6                             |
|  IC13          IC9                              IC4      |
|             (IC8-15)                                     |
|                            IC5            IC2      IC3   | male side
|  IC14          IC10             JP1             IC16     |
|             (IC16-21)           JP2                      |
|                                           IC1            |
|  IC15          IC11                                      |
|        ----CN3----                    ----CN1----        |
|----------------------------------------------------------|
Notes:
      The female side of the cart PCB only has traces

      IC1   - 74LVCH16245A (16bit transceiver with direction pin)
      IC2   - XC3S50 Xilinx Spartan FPGA (TQFP144)
      IC3   - PIC16C621A EPROM-Based 8-Bit CMOS Microcontroller (PDIP18) with internal memory
              On the end of the number, -JPN means it requires Japanese BIOS, -COM will run with any BIOS
      IC4   - XCF01SVG Xilinx Platform Flash In-System Programmable Configuration PROMS (TSSOP20)
      IC5   - 74LVC08A (Quad 2-input AND gate)
      IC6   - 74LVCH16245A (16bit transceiver with direction pin)
      IC7   - empty socket for EPROM (DIP42), either 27C160 or 27C322
  IC8-IC15  - S29GL512N FlashROM (TSOP56), 512Mb. Not all positions are populated
      IC16  - R3112N431A Low voltage detector with output delay (SOT-23-5)
      JP1   - JUMPER Sets the size of the EPROM. 1-2 = 32M, 2-3 = 16M
      JP2   - JUMPER unknown function
   CN1/2/3  - connectors joining to main board
      CN4   - 6 legs connector for ISP programming

   Games known to use this PCB include....
                                       Sticker    EPROM        FLASHROMs   XC3S50   PIC16C621A    XCF01S
Game                                   on cart    IC7#         # of SOP56  IC2#     IC3#          IC4#     Notes
------------------------------------------------------------------------------------------------------------------------------
/Akatsuki Denkou Senki Blitz Kampf
\Ausf. Achse                           841-0058C  not present  4 (512Mb)   present  317-5130-JPN  present  IC2# is labeled "VER.2" - IC4# is marked "5A" - IC#10 & IC#11 are empty
Dynamite Deka EX / Asian Dynamite      840-0175C  not present  4 (512Mb)   present  317-0495-COM  present  IC2# is labeled "VER.2"
Illmatic Envelope                      841-0059C  not present  4 (512Mb)   present  317-5131-JPN  present  IC2# is labeled "VER.2" - IC#11 is empty
Mamoru-kun wa Norowarete Shimatta      841-0060C  not present  4 (512Mb)   present  317-5132-JPN  present  IC2# is labeled "VER.2"
Melty Blood Actress Again              841-0061C  not present  6 (512Mb)   present  317-5133-JPN  present  IC2# is labeled "REV.A" - IC4# is marked "5A"
Melty Blood Actress Again (Rev A)      841-0061C  24455        6 (512Mb)   present  317-5133-JPN  present  IC2# is labeled "REV.A" - IC4# is marked "5A"
Mushiking - The King Of Beetles II ENG 840-0164C  24357        2 (512Mb)   present  317-0437-COM  present  IC4# is marked "18"
Poka Suka Ghost / Manic Panic Ghost    840-0170C  not present  5 (512Mb)   present  317-0461-COM  present
Radirgy Noa                            841-0062C  not present  4 (512Mb)   present  317-5138-JPN  present  IC2# is labeled "VER.2" - IC4# is marked "8A"
Shooting Love 2007                     841-0057C  not present  4 (512Mb)   present  317-5129-JPN  present  IC2# is labeled "VER.2"
Touch De Zunou (Rev A)                 840-0166C  not present  2 (512Mb)   present  317-0435-JPN  present  IC4# is marked "18"



MASK B-F2X (C) Namco 2000
|-------------------------------------------------------------------------------------|
|                                                               ----CN2----            -|
|                                                                                       |
|7        LED1                                                                          |
|                                                                                       |
|6        MA23  MA22  MA21  MA20  MA19  MA18  MA17  MA16  MA15  MA14  MA13  MA12        |
|                                                                                       |
|5                                                                          ISSI        |
|                                                                                       |
|4        MA11  MA10  MA9   MA8   MA7   MA6   MA5   MA4   MA3   MA2   MA1   ISSI        | male side
|                                                                                       |
|3                                            OSC1                                      |
|                                                                                       |
|2       FLASH FLASH FLASH       FLASH              NAOD                                |
|         FL3   FL2   FL1        FL0                EC1B                                |
|1 J J J                          X76F  NAOD                    SEGA                    |
|  P P P                          100   EC2A                  315-5881                  |
|  3 2 1                                                                                |
|    A     B     C     D     E     F     H     J     K     L     M     N     P     R    |
|             ----CN3----                                       ----CN1----             |
|---------------------------------------------------------------------------------------|
Notes:
      The female side of the cart PCB only has traces

        JP1 - JUMPER silkscreened   USE - NOT USE
        JP2 - JUMPER silkscreened   64M - 128M
        JP3 - JUMPER silkscreened BANK0 - BANK1
         1F - FLASHROM (SOIC8) Xicor X76F100 Secure SerialFlash. Silkscreened X76F100
         1H - NAODEC2A (QFP100) Altera MAX EPM7064S. Silkscreened NAODEC2A
         1M - SEGA 315-5581 (QFP100). Probably some kind of FPGA or CPLD. Usually different per game
              On the end of the number, -JPN means it requires Japanese BIOS, -COM will run with any BIOS
2B,2C,2D,2F - DA28F640J5 FlashROM (SSOP56), either 32Mb or 64Mb. Not all positions are populated.
              Silkscreened VOYAGER64. Looks like the equivalent of IC11/22 on Sega carts
         2K - NAODEC1B (QFP100) Altera MAX EPM7064S. Silkscreened NAODEC1A
         3J - oscillator 28.000MHz
4B-4N,6B-6P - MASKROM (TSOP48), 128Mb. Not all positions are populated. Silkscreened MASK128MT
      4P,5P - SRAM (SOJ28) 32kx8, ISSI IS61C256AH-15J
   CN1/2/3  - connectors joining to main board

   Games known to use this PCB include....
                                   Sticker   FL0-FL3   FLASHROMs   X76F100      EPM7064   EPM7064   315-5581      Game
 Game                              on cart   FLASHROM  # of SOP48  IC @ 1F      IC @ 1H   IC @ 2K   IC @ 1M       code       Notes
-----------------------------------------------------------------------------------------------------------------------------------------------
/Gun Survivor 2 Biohazard
\Code: Veronica                    25709801  1 (64Mb)  14 (128Mb)  not present  NAODEC2A  NAODEC1B  317-5075-COM  BHF1
/Shin Nihon Prowrestling                                                                                                     /FL0 & FL1 have pin55 raised from PCB.
\Toukon Retsuden 4 Arcade Edition  25349801  2 (64Mb)  15 (128Mb)  not present  NAODEC2A  NAODEC1B  317-5040-COM  TRF1Ver.A  \They are connected togheter and go to pin89 on 2K.
World Kicks                        25209801  2 (32Mb)  10 (128Mb)  not present  NAODEC2A  NAODEC1A  317-5040-COM  WK3 Ver.A  317-5040-COM is confirmed.



MASK C (C) Namco 2000
|-------------------------------------------------------------------------------------|
|                                                               ----CN2----            -|
|                                                                                       |
|7        LED1  LED2                                                                    |
|                                                                                       |
|6        MA21  MA20  MA19  MA18  MA17  MA16  MA15  MA14  MA13  MA12  MA11              |
|                                                                                       |
|5                                                                     I     I          |
|                                                                      S     S          |
|4        MA10  MA9   MA8   MA7   MA6   MA5   MA4   MA3   MA2   MA1    S     S          | male side
|                                                                      I     I          |
|3                                     JP1    NAOD                                      |
|                                             EC3                                       |
|2       FLASH FLASH FLASH      FLASH  OSC1                                             |
|         FL3   FL2   FL1       FL0                                                     |
|1                                X76F                          SEGA                    |
|                                 100                         315-5881                  |
|                                                                                       |
|    A     B     C     D     E     F     H     J     K     L     M     N     P   R   S  |
|             ----CN3----                                       ----CN1----             |
|---------------------------------------------------------------------------------------|
Notes:
      The female side of the cart PCB only has traces

        JP1 - JUMPER silkscreened OPEN
         1F - FLASHROM (SOIC8) Xicor X76F100 Secure SerialFlash. Silkscreened X76F100
         2H - oscillator 28.000MHz
         3J - NAODEC3 (QFP100) Cypres CY37128. Silkscreened NAODEC3
         1M - SEGA 315-5581 (QFP100). Probably some kind of FPGA or CPLD. Usually different per game
              On the end of the number, -JPN means it requires Japanese BIOS, -COM will run with any BIOS
2B,2C,2D,2F - DA28F640J5 FlashROM (SSOP56), either 32Mb or 64Mb. Not all positions are populated.
              Silkscreened VOYAGER64. Looks like the equivalent of IC11/22 on Sega carts
4B-4M,6B-6N - MASKROM (TSOP48), 128Mb. Not all positions are populated. Silkscreened MASK128MT
      4N,4P - SRAM (SOJ28) 32kx8, ISSI IS61C256AH-15J
   CN1/2/3  - connectors joining to main board

   Games known to use this PCB include....
                           Sticker   FL0-FL3   FLASHROMs   X76F100  CY37128  315-5581      Game
 Game                      on cart   FLASHROM  # of SOP48  IC @ 1F  IC @ 3J  IC @ 1M       code        Notes
--------------------------------------------------------------------------------------------------------------------
Mazan: Flash of the Blade  25869812  1 (64Mb)   8 (128Mb)  present  NAODEC3  317-0056-COM  MAZ1
Ninja Assault              25469801  3 (64Mb)   9 (128Mb)  present  NAODEC3  317-5068-COM  NJA3 Ver.A




      Note! Generally, games that require a special I/O board or controller will not boot at all with a
            standard NAOMI I/O board. Usually they display a message saying the I/O board is not acceptable
            or not connected properly.



Sega I/O boards
---------------

These are used on NAOMI and all other Sega games from 1999 onwards.
Not all I/O boards are listed here. If you know of others, please let us know.


838-13683
838-13683-91 (sticker)
838-13683-92 (sticker)
838-13683-93 (sticker)
|-----------------------------|
| CN6     CN7   CN2  CN1 RELAY|
| IC4 IC7          IC5    CN5 |
| IC3                  IC6    |
|CN3         IC1    IC2    IC9|
|    OSC1     JP1             |
|CN4                          |
|  IC8               IC10     |
|--|        JAMMA        |----|
   |---------------------|
Notes:
      JVS to JAMMA I/O board. Has both digital and analog inputs

              JVS test mode strings
              ---------------------
              NAME         SEGA ENTERPRISES,LTD.
                           I/O 838-13683B
                           Ver1.07
                           99/06
              CMD VER      1.1
              JVS VER      2.0
              COM VER      1.0
              SWITCH       2 PLAYER(S) 11 BITS
              COIN         2 SLOT
              ANALOG       8 CH
              ROTARY       0 CH
              KEYCODE      0
              SCREEN       X:0 Y:0 CH:0
              CARD         0 SLOT
              HOPPER OUT   0 CH
              DRIVER OUT   8 SLOT
              ANALOG OUT   0 CH
              CHARACTER    CHARA:0 LINE:0
              BACKUP       0

      CN1   - USB connector type A
      CN2   - USB connector type B
      CN3   - 14 pin connector used for switch input or lamp output via jumper setting
              1-2    +5V
              3      NC
              4-6    1P SW6-SW8
              7      NC
              8-10   2P SW6-SW8
              11-12  NC
              13-14  GND
      CN4   - 2 pin connector
      CN5   - 15 pin VGA connector
      CN6   - 6 pin connector used for 5 volt and 12 volt power input/output
              1-2 +5V
              3-4 +12V
              5-6 GND
      CN7   - 26 pin analog controls connector
              1-2  +5V              15     Reserved
              3    1P Analog Y      16     Reserved
              4    2P Analog Y      21     Reserved
              9    1P Analog X      22     Reserved
              10   2P Analog X      23-24  GND
      IC1   - Toshiba TMP90PH44 microcontroller with sticker 'SP5001-B' (SDIP64)
      IC2   - location for TMP90PH44 QFP64 (not used)
      IC3   - 74HC541 (SOIC20)
      IC4   - BA6212 (DIP20)
      IC5   - Analog Devices ADM485 (SOIC8)
      IC6   - 5W393 (SOIC8)
      IC7   - 74HC4052 (SOIC16)
      IC8   - Toshiba TD62384 (SOIC16)
      IC9   - A7666FS (SOIC16)
      IC10  - 74F86 (SOIC14)
      JP1   - Set in position A to use CN3 as input for switches. Set in position B to use as output (for lamps etc)
              When in position A, in the JVS test mode, 'SWITCH' becomes '2 PLAYER(S) 15 BITS' and 'DRIVER OUT' becomes '0 SLOT'
      RELAY - Omron G6S-2 relay
      OSC1  - 14.74MHz


837-13551
837-13551-92 (sticker)
837-13551-93 (sticker)
|-----------------------------|
|CN4 CN5         CN6   CN7 CN8|
|LED   ADM485                 |
|  RELAY         IC1A    *    |
|                             |
|                    14.745MHz|
|            PS2801-4(x8)     |
|                          LED|
|CN1 CN2      CN3             |
|-----------------------------|
Notes:
             JVS I/O board. Has both digital and analogue inputs.
             This is the most common type. Used on Sega driving games, NAOMI, Hikaru, Triforce, Chihiro etc

             JVS test mode strings
             ---------------------
             NAME         SEGA ENTERPRISES,LTD.
                          I/O BD JVS
                          837-13551
                          Ver1.00
             CMD VER      1.1
             JVS VER      2.0
             COM VER      1.0
             SWITCH       2 PLAYER(S) 13 BITS
             COIN         2 SLOT(S)
             ANALOG       8 CH
             ROTARY       0 CH
             KEYCODE      0
             SCREEN       X:0 Y:0 CH:0
             CARD         0 SLOT(S)
             HOPPER OUT   0 CH
             DRIVER OUT   6 CH
             ANALOG OUT   0 CH
             CHARACTER    CHARA:0 LINE:0
             BACKUP       0

      IC1A - Toshiba TMP90PH44 microcontroller marked '315-6215' (SDIP64)
         * - location under the PCB for TMP90PH44 QFP64 (not used)
     RELAY - NEC EB2-4.5NU relay
    ADM485 - Analog Devices ADM485
       CN1 - 5 pin connector 12 volt power input
             1-2 +12V
             3 NC
             4-5 GND
       CN2 - 5 pin connector 12 volt power output
             1-2 +12V
             3 NC
             4-5 GND
       CN3 - 60 pin digital input connector
             1-8   +5V          33    1P SW4
             9-15  GND          34    2P SW4
             16    NC           35    1P SW5
             17    1P Start     36    2P SW5
             18    2P Start     37    1P SW6
             19    1P Right     38    2P SW6
             20    2P Right     39    1P SW7
             21    1P Left      40    2P SW7
             22    2P Left      41    1P Service Credit
             23    1P Up        42    2P Service Credit
             24    2P Up        43    Test SW
             25    1P Down      44    Tilt SW
             26    2P Down      45    Coin SW1
             27    1P SW1       46    Coin SW2
             28    2P SW1       47-48 NC
             29    1P SW2       49-50 Coin Meter 1 & 2
             30    2P SW2       51-56 Output 1-6
             31    1P SW3       57-60 +12V
             32    2P SW3
             Note: For coin input to work, the coin meters need to be connected to
             pins 49 and 50, or tie +5 volts to pin 49 and 50 via a resistor.
       CN4 - USB connector type B
       CN5 - USB connector type A
       CN6 - Analog I/O connector
             1-2   VCC     15    AD 2
             3     AD 0    16    AD 6
             4     AD 4    17-18 NC
             5     GND     19-20 VCC
             6     GND     21    AD 3
             7-8   NC      22    AD 7
             9     AD 1    23    GND
             10    AD 5    24    GND
             11-14 NC      25-26 NC
       CN7 - 4 pin connector 5V power input
             1-2 +5v       3-4 GND
       CN8 - 4 pin connector 5V power out
             1-2 +5v       3-4 GND


837-13741
837-13844-02 (sticker)
|-----------------------------|
|       CN8        CN9        |
|DB9                       CN1|
|RESET_SW |----|              |
|         |IC6 |  OSC1     CN5|
|         |----|              |
|CN7      LED  DSW1(5)     CN2|
|   OSC2   RELAY              |
|CNx  USB USB  IC7 LED     CN6|
|-----------------------------|
Notes: (most info taken from poor quality pics/scans, better info is needed)

             JVS I/O board 2. Has both digital and analogue inputs.
             This board is used with F355, Ghost Squad, and many
             others including network/satellite games.

             JVS test mode strings
             ---------------------

             NAME         SEGA ENTERPRISES,LTD.;837-13741
                          I/O CONTROL BD2;Ver0.15;99/06
             CMD VER      1.1
             JVS VER      2.0
             COM VER      1.0
             SWITCH       2 PLAYERS 12BITS
             COIN         2 SLOTS
             ANALOG       8CH
             DRIVER OUT   22CH

      IC6  - Sega 315-6146 custom IC (QFP176)
      IC7  - 27C512 EPROM with label 'EPR-22082' (DIP28)
             On plain 837-13844 (no -02) this is 'EPR-21868' (DIP28)
      IC8  - Sharp LH52256 32k x8 SRAM (SOP28)
      IC10 - Something by NEC? (QFP44)
      OSC1 - 14.7456MHz
      OSC2 - 32MHz
      CNx  - 6 pin connector
      CN1  - 5 pin connector 12 volt power input
             1-2 +12V
             3 NC
             4-5 GND
      CN2  - 4 pin connector 5V power input
             1-2 +5v
             3-4 GND
      CN5  - 5 pin connector 12 volt power output
             1-2 +12V
             3 NC
             4-5 GND
      CN6  - 4 pin connector 5V power output
             1-2 +5v
             3-4 GND
      CN7  - 26 pin connector (many pins unknown)
             1  +5V              14
             2                   15  Analog Output
             3  Analog Output    16
             4                   17  GND
             5  GND              18
             6                   19  +5V
             7  +5V              20
             8                   21  Analog Output
             9  Analog Output    22
             10                  23  GND
             11 GND              24
             12                  25
             13 +5V              26
      CN8  - 40 pin connector (many pins unknown)
             3-5   +5V           21    Switch
             7-9   GND           22    Switch
             13-14 Coin SW1/2    23    Switch
             15    Test SW       24    Switch
             17    1P Start SW   25    2P Start SW
             18    Service SW    31    Switch
             19    Switch        32    Switch
             20    Switch        33-40 RX1-RX8 (for communications)
      CN9  - 34 pin connector (some pins unknown)
             1-4   +5V
             5-8   GND
             9-10  Coin Meter 1 & 2
             11    Lamp
             12    Lamp
             13    Lamp
             14    Lamp
             15    Lamp
             16    Lamp
             17    Lamp
             18    Lamp
             19    Lamp
             20    Lamp
             21    Coin LED
             22-24 ?
             25-32 TX1-TX8 (for communications)
             33-34 12V


837-13938
171-7807A
|--------------------|
|CN2      CN1    IC9S|
|         OSC1       |
|      |-----|       |
|      | IC2 |OSC2   |
| CN3  |     |       |
|  IC7S|-----|    IC3|
|LED    CN4     IC4  |
|--------------------|
Notes:
      This is the I/O board used for Dynamic Golf which is
      located under the panel. It's also used for spinner
      controls such as those used on Outtrigger.
      It must be daisy-chained to the normal I/O board with a USB cable.

      CN1 - 24 pin connector (not used on Dynamic Golf, other use unknown)
      CN2 - 4 pin connector used for 5 volt power input
      CN3 - USB connector type B
      CN4 - 16 pin connector used for buttons and trackball
      IC1 - HC240 logic IC (SOIC20)
      IC2 - Sega 315-6146 custom IC (QFP176)
      IC3 - 27C512 EPROM with label 'EPR-22084' (DIP28)
      IC4 - HC4020 logic IC (SOIC16)
      These parts on the other side of the PCB....
      IC7S - Analog Devices ADM485 (SOIC8)
      IC9S - Sharp LH52256 32k x8 SRAM (SOP28)
      OSC1 - 14.7456MHz
      OSC2 - 32MHz
      Not shown above.... Sharp PC410 (x8, at PC9S to PC16S)
                          HC74 at IC8S
                          34164 (?) at IC10S (SOIC8)

Sega's I/O board has:
- spare output of 5V, 12V, and GND (from JAMMA power input via noise filter)
- analog input
- USB input (connect to NAOMI motherboard)
- USB output (not used)
- shrinked D-sub 15pin connector (connect JVS video output, the signal is routed to JAMMA connector, signal is amplified to 3Vp-p and H/V sync signal is mixed (composite))
- external I/O connector (JST 12pin)
- switch to select function of external I/O connector (extra button input or 7-seg LED(x2) output of total wins for 'Versus City' cabinet)
- spare audio input (the signal goes to JAMMA speaker output)
- JAMMA connector

external I/O connector

old version
 1 +5V
 2 +5V
 3 +5V
 4 1P PUSH 4
 5 1P PUSH 5
 6 1P PUSH 6
 7 1P PUSH 7
 8 2P PUSH 4
 9 2P PUSH 5
10 2P PUSH 6
11 2P PUSH 7
12 GND
13 GND
14 GND
(PUSH4 and 5 are common to JAMMA)

new version
 1 +5V
 2 +5V
 3 +5V
 4 1P PUSH 6
 5 1P PUSH 7
 6 1P PUSH 8
 7 1P PUSH 9
 8 2P PUSH 6
 9 2P PUSH 7
10 2P PUSH 8
11 2P PUSH 9
12 GND
13 GND
14 GND

mahjong panel uses ext. I/O 4-8 (regardless of I/O board version)
key matrix is shown in below

  +------------------------------------ ext. I/O 8
  |     +------------------------------ ext. I/O 7
  |     |     +------------------------ ext. I/O 6
  |     |     |     +------------------ ext. I/O 5
  |     |     |     |     +------------ ext. I/O 4
(LST)-( D )-( C )-( B )-( A )---------- JAMMA 17 (1p start)
  |     |     |     |     |
  |   ( H )-( G )-( F )-( E )---------- JAMMA 18 (1p up)
  |     |     |     |     |
  |   ( L )-( K )-( J )-( I )---------- JAMMA 19 (1p down)
  |     |     |     |     |
(F/F)-(PON)-(CHI)-( N )-( M )---------- JAMMA 20 (1p left)
        |     |     |     |
        +---(RON)-(RCH)-(KAN)---------- JAMMA 21 (1p right)
              |     |     |
              +---(BET)-(STR)---------- JAMMA 22 (1p push1)

* LST = Last chance, F/F = Flip flop, STR = Start


---------------------------------------------------------------------------------------------------------------------------------------------------

Guru's Readme
-------------

Atomis Wave (system overview)
Sammy/SEGA, 2002

The Atomiswave System is basically just a Sega Dreamcast using ROM carts.

PCB Layout
----------

Sammy AM3AGA-04 Main PCB 2002 (top side)
1111-00006701 (bottom side)
   |--------------------------------------------|
  |-      TA8210AH               D4721   3V_BATT|
  |VGA                              BS62LV1023TC|
  |VOL                                          |
  |SER      |-----|               PCM1725U      |
  |-        |ROMEO|       D4516161              |
   |CN3     |     |                    BIOS.IC23|
|--|SW1     |-----|                             |
|                     |-----|           |-----| |
|           33.8688MHz|315- |           |ROMEO| |
|           |-----|   |6232 |           |     | |
|           |315- |   |-----|           |-----| |
|           |6258 |  32.768kHz                  |
|J          |-----|                             |
|A                 |-------------|    D4564323  |
|M                 |             |              |
|M                 |             |  |--------|  |
|A        D4516161 |  315-6267   |  |        |  |
|                  |             |  |  SH4   |  |
|                  |             |  |        |  |
|  TD62064         |             |  |        |  |
|         D4516161 |             |  |--------|  |
|                  |-------------|              |
|                                     D4564323  |
|--|             D4516161  D4516161             |
   |                             W129AG  13.5MHz|
   |--------------------------------------------|
Notes:
------
BS62LV1023TC-70 - Brilliance Semiconductor Low Power 128k x8 CMOS SRAM (TSOP32)
TA8210AH        - Toshiba 19W x 2 Channel Power Amplifier
D4516161AG5     - NEC uPD4516161AG5-A80 1M x16 (16MBit) SDRAM (SSOP50)
D4564323        - NEC uPD4564323G5-A10 512K x 32 x 4 (64MBit) SDRAM (SSOP86)
D4721           - NEC uPD4721 RS232 Line Driver Receiver IC (SSOP20)
PCM1725U        - Burr-Brown PCM1725U 16Bit Digital to Analog Converter (SOIC14)
2100            - New Japan Radio Co. Ltd. NJM2100 Dual Op Amp (SOIC8)
ROMEO           - Sammy AX0201A01 'ROMEO' 4111-00000501 0250 K13 custom ASIC (TQFP100)
315-6232        - SEGA 315-6232 custom ASIC (QFP100)
315-6258        - SEGA 315-6258 custom ASIC (QFP56)
315-6267        - SEGA 315-6267 custom ASIC (BGAxxx)
TD62064         - Toshiba TD62064 NPN 50V 1.5A Quad Darlington Driver (SOIC16)
SH4             - Hitachi SH-4 HD6417091RA CPU (BGA256)
BIOS.IC23       - Macronix 29L001MC-10 3.3volt FlashROM (SOP44)
                  This is a little strange, the ROM appears to be a standard SOP44 with reverse pinout but the
                  address lines are shifted one pin out of position compared to industry-standard pinouts.
                  The actual part number doesn't exist on the Macronix web site, even though they have datasheets for
                  everything else! So it's probably a custom design for Sammy and top-secret!
                  The size is assumed to be 1MBit and is 8-bit (D0-D7 only). The pinout appears to be this.....

                               +--\/--+
tied to WE of BS62LV1023   VCC | 1  44| VCC - tied to a transistor, probably RESET
                           NC  | 2  43| NC
                           A9  | 3  42| NC
                           A10 | 4  41| A8
                           A11 | 5  40| A7
                           A12 | 6  39| A6
                           A13 | 7  38| A5
                           A14 | 8  37| A4
                           A15 | 9  36| A3
                           A16 |10  35| A2
                           NC  |11  34| A1
                           NC  |12  33| CE - tied to 315-6267
                           GND |13  32| GND
                           A0  |14  31| OE
                           D7  |15  30| D0
                           NC  |16  29| NC
                           D6  |17  28| D1
                           NC  |18  27| NC
                           D5  |19  26| D2
                           NC  |20  25| NC
                           D4  |21  24| D3
                           VCC |22  23| NC
                               +------+

W129AG          - IC Works W129AG Programmable Clock Frequency Generator, clock input of 13.5MHz (SOIC16)
SW1             - 2-position Dip Switch
VGA             - 15 pin VGA out connector @ 31.5kHz
SER             - 9 pin Serial connector  \
VOL             - Volume pot              / These are on a small daughterboard that plugs into the main PCB via a multi-wire cable.
CN3             - 10 pin Speaker output & Extension serial connector
3V_BATT         - Panasonic ML2020 3 Volt Coin Battery

CN3 Pinout
Pin     Function    I/O    Pin   Function  I/O
----------------------------------------------
 1    Stereo L (+)  Out  |  2      TXD     Out
 3    Stereo L (-)  Out  |  4      RXD      In
 5    Stereo R (+)  Out  |  6      GND      -
 7    Stereo R (-)  Out  |  8      +5V     Out
 9      No Connection    | 10  No Connection

The bottom of the PCB contains nothing significant except some connectors. One for the game cart, one for special controls
or I/O, one for a communication module, one for a cooling fan and one for the serial connection daughterboard.

---------------------------------------------------------------------------------------------------------------------------------



Atomiswave cart PCB layout and game usage (revision 1.0 26/2/2011 5:59pm)
-----------------------------------------

Type 1 ROM Board:


AM3AGB-04
MROM PCB
2002
|----------------------------|
| XC9536                     |
|         IC18 IC17*   IC10  |
|                            |
|                            |
|              IC16*   IC11  |
|                            |
|                            |
||-|           IC15*   IC12  |
|| |                         |
|| |                         |
|| |CN1        IC14*   IC13  |
|| |                         |
||-|                         |
|----------------------------|
Notes:
           * - Denotes those devices are on the other side of the PCB
         CN1 - This connector plugs into the main board.
      XC9536 - Xilinx XC9536 in-system programmable CPLD (PLCC44), stamped with a
               game code. This code is different for each different game.
               The last 3 digits seems to be for the usage.
               F01 = CPLD/protection device and M01 = MASKROM

               Game (sorted by code)                 Code
               -----------------------------------------------
               Sports Shooting USA                   AX0101F01
               Dolphin Blue                          AX0401F01
               Maximum Speed                         AX0501F01
               Demolish Fist                         AX0601F01
               Guilty Gear X Ver. 1.5                AX0801F01
               Guilt Gear Isuka                      AX1201F01
               Knights Of Valour Seven Spirits       AX1301F01
               Salaryman Kintaro                     AX1401F01
               Ranger Mission                        AX1601F01
               Faster Than Speed                     AX1701F01
               Rumble Fish                           AX1801F01
               Fist Of The North Star                AX1901F01
               Victory Furlong : Horse Racing        AX2001F01
               King Of Fighters NEOWAVE              AX2201F01
               Extreme Hunting                       AX2401F01

        IC18 - Fujitsu 29DL640E 64M TSOP48 FlashROM. This ROM has no additional custom markings
               The name in the archive has been devised purely for convenience.
               This ROM holds the main program.

IC10 to IC17 - Custom-badged 128M TSOP48 mask ROMs.
               IC10 - Not Populated for 7 ROMs or less (ROM 01 if 8 ROMs are populated)
               IC11 - ROM 01 (or ROM 02 if 8 ROMs are populated)
               IC12 - ROM 02 (or ROM 03 if 8 ROMs are populated)
               IC13 - ROM 03 (or ROM 04 if 8 ROMs are populated)
               IC14 - ROM 04 (or ROM 05 if 8 ROMs are populated)
               IC15 - ROM 05 (or ROM 06 if 8 ROMs are populated)
               IC16 - ROM 06 (or ROM 07 if 8 ROMs are populated)
               IC17 - ROM 07 (or ROM 08 if 8 ROMs are populated)

               ROM Codes
               ---------
                                                                               Number
               Game (sorted by code)                 Code                      of ROMs
               -----------------------------------------------------------------------
               Sports Shooting USA                   AX0101M01 to AX0104M01    4
               Dolphin Blue                          AX0401M01 to AX0405M01    5
               Maximum Speed                         AX0501M01 to AX0505M01    5
               Demolish Fist                         AX0601M01 to AX0607M01    7
               Guilty Gear X Ver. 1.5                AX0801M01 to AX0807M01    7
               Guilty Gear Isuka                     AX1201M01 to AX1208M01    8
               Knights Of Valour Seven Spirits       AX1301M01 to AX1307M01    7
               Salaryman Kintaro                     AX1401M01 to AX1407M01    7
               Ranger Mission                        AX1601M01 to AX1605M01    5
               Faster Than Speed                     AX1701M01 to AX1706M01    6
               Rumble Fish                           AX1801M01 to AX1807M01    7
               Fist Of The North Star                AX1901M01 to AX1907M01    7
               Victory Furlong : Horse Racing        AX2001M01 to AX2007M01    7
               King Of Fighters NEOWAVE              AX2201M01 to AX2206M01    6
               Extreme Hunting                       AX2401M01 to AX2406M01    6


Type 2 ROM Board:


AM3ALW-02
MROM2 PCB
2005
|----------------------------|
|     FMEM1                  |
|     FMEM2*   MROM12        |
|              MROM11*       |
|                      MROM9 |
|              MROM10  MROM8*|
| XCR3128XL*   MROM7*        |
|                            |
||-|           MROM6         |
|| |           MROM3*  MROM4 |
|| |                   MROM5*|
|| |CN1        MROM2         |
|| |           MROM1*        |
||-|                         |
|----------------------------|
Notes:
           * - Denotes those devices are on the other side of the PCB
         CN1 - This connector plugs into the main board.
   XCR3128XL - Xilinx XCR3128XL in-system programmable 128 Macro-cell CPLD (TQFP100)
               stamped with a game code. This code is different for each different game.
               The last 3 digits seems to be for the usage.
               F01 = CPLD/protection device and M01 = MASKROM

               Game (sorted by code)                 Code
               -----------------------------------------------
               Samurai Spirits Tenkaichi Kenkakuden  AX2901F01
               Metal Slug 6                          AX3001F01
               The King Of Fighters XI               AX3201F01
               Neogeo Battle Coliseum                AX3301F01
               Rumble Fish 2                         AX3401F01

 FMEM1/FMEM2 - Fujitsu 29DL640E 64M TSOP48 FlashROM. This ROM has no additional custom markings
               The name in the archive has been devised purely for convenience.
               This ROM holds the main program.
               This location is wired to accept TSOP56 ROMs, however the actual chip populated
               is a TSOP48, using the middle pins. The other 2 pins on each side of the ROM
               are not connected to anything.

       MROM* - Custom-badged 256M SSOP70 mask ROMs

               ROM Codes
               ---------
                                                                               Number
               Game (sorted by code)                 Code                      of ROMs
               -----------------------------------------------------------------------
               Samurai Spirits Tenkaichi Kenkakuden  AX2901M01 to AX2907M01    7
               Metal Slug 6                          AX3001M01 to AX3004M01    4
               The King Of Fighters XI               AX3201M01 to AX3207M01    7
               Neogeo Battle Coliseum                AX3301M01 to AX3307M01    7
               Rumble Fish 2                         AX3401M01 to AX3405M01    5


Type 3 ROM Board:

This type is manufactured by Sega when Sammy merged with Sega.

171-8355A
PC BD A/W 128M FLASH
837-14608 (sticker for Extreme Hunting 2 Tournament Edition)
837-14695 (sticker for Dirty Pigskin Football)
|----------------------------|
| XC9536*         U16   U2*  |
|                            |
|J2                          |
|                            |
|                 U4*   U14  |
|                            |
|                            |
||-|                         |
|| |              U17   U1*  |
|| |                         |
|| |J1*                      |
|| |                         |
||-|              U3*   U15  |
|----------------------------|
Notes:
           * - Denotes those parts are on the other side of the PCB
          J1 - This connector plugs into the main board.
          J2 - 6 pin connector for programming the XC9536 CPLD and/or the flash ROMs
      XC9536 - Xilinx XC9536 in-system programmable CPLD (PLCC44), stamped with a
               Sega 315-xxxx part number with a sticker over the top of it.

               Game                                  Part#      Sticker
               ---------------------------------------------------------
               Extreme Hunting 2 Tournament Edition  315-6428P  315-6248
               Dirty Pigskin Football                ?          ?

          U* - Fujitsu MBM29PL12LM-10PCN 128M MirrorFlash TSOP56 flash ROM.
               (configured as 16Mbytes x8bit or 8Mwords x16bit)
               This ROM has no additional custom markings. The name in the archive has been devised
               purely for convenience using the Sega 837- sticker number. The number of ROMs may vary
               between games. So far all 8 positions have been seen populated. It's also possible all
               positions are present when manufactured, each board is programmed to requirements and
               depending on the total data length, some ROMs may be empty.



Network Board
-------------

This board is required for Extreme Hunting 2 Tournament Edition (and possibly some other Sega-made Atomiswave carts)
although it doesn't need to be connected to a network or another Atomiswave unit to boot up. However it must be
plugged into the PCB in the communication slot or the game will not go in-game. It will boot but then displays
NETWORK BOARD ERROR if not present. Externally there's a hole for an RJ45 network cable and a slot for a
PIC16C621/PIC16C622 PIC enclosed in a black plastic housing. This is the same type as used in NAOMI etc. This
board probably acts like the NAOMI network DIMM board minus the on-board DIMM RAM storage.

837-14508R
171-8324C
(C) SEGA 2005
|-----------------------------------|
| RJ45    24LC0241* IC2       CN3   |
|LLLL        K4S643232*       IC14  |
|  RTL8201 LLLL                     |
|25MHz                              |
|       6417710          IC4S*      |
|                        XC3S200    |
|MAX3221        JP1     XCF01S*     |
|CN5  33.333MHz JP2             CN2*|
|-----------------------------------|
Notes:
      *        - Denotes those parts are on the other side of the PCB
      L        - LED
      RJ45     - RJ45 network connector
      24LC0241 - EEPROM (SOIC8)
      K4S643232- Samsung K4S643232 512k x 32bit x 4 banks synchronous DRAM (TSOPII-86)
      RTL8201  - Realtek RTL8201 Single Port 10/100M Fast Ethernet IC (QFP48)
      6417710  - Renesas HD6417710BPV SH3-DSP 32-Bit RISC Microcomputer SuperHTM RISC engine Family / SH7700 Series (BGA256)
      XC3S200  - Xilinx Spartan XC3S200 FPGA (QFP100)
      XCF01S   - Xilinx XCF01S In-System Programmable 1Mbit PROM for Configuration of Xilinx FPGAs (TSSOP20)
      MAX3221  - Maxim MAX3221 3.0V to 5.5V, 250kbps, RS-232 Transceivers with Auto Shutdown (TSSOP16)
      JP1/2    - Jumpers, both set to 1-2
      CN2      - This connector plugs into the main board
      CN3      - 6 pin connector
      CN5      - 3 position connector
      IC2      - ST M29DW324DB 32M flash ROM (TSOP48)
      IC4S     - Spartan S29GL128N10TFIO1 128M flash ROM (TSOP56)
      IC14     - socket for PIC16C621 mounted in a plastic plug-in case
                 PIC Usage:
                           Game                                   Sega Part#
                           ---------------------------------------------------
                           Extreme Hunting 2 Tournament Edition   317-0445-COM


AW-NET Network Board
--------------------

Sammy
AM3AJG-01
LAN PCB
2003
SAMLAN Rev: D
|-----------------------------------|
| RJ45                              |
|PULSE                              |
|        25Mz                       |
|                                   |
|      RTL8139CL+*           CN*    |
|                  315-6310*        |
|                                   |
|       L46R                        |
|-----------------------------------|
Notes:
      *          - Denotes those parts are on the other side of the PCB
      RJ45       - RJ45 network connector
      PULSE      - Pulse H0011 10/100 LAN Magnetics Module (SOIC16)
      L46R       - 93C46 compatible 128x8 EEPROM (SOIC8)
      RTL8139CL+ - Realtek RTL8139CL+ 3.3 volt Single Chip Fast Ethernet Controller with Power Management (QFP120)
      315-6310   - Sega 315-6310 Custom IC (QFP100)
      CN         - This connector plugs into the main board



Gun Sub Board
-------------

AM3AGT-02 GUN SUB PCB
|------------------|
|CN5  CN4  CN3  CN2|
|       74HC74     |
|                  |
| 74HC74    7CHC74 |
|      74HC74      |
|                  |
|       CN1        |
|------------------|
Notes:
      CN1 - 8 pin connector joining to I/O Expansion Module (which is plugged into main board)
      CN2 - Gun connection for player 2 trigger and optical
      CN3 - Gun connection for player 2 pump switch
      CN4 - Gun connection for player 1 trigger and optical
      CN5 - Gun connection for player 1 pump switch


Other games not dumped (some may have been cancelled)
----------------------
Chase 1929
Force Five
Kenju
Premier Eleven
Sushi Bar
Sega Clay Challenge
Sega Bass Fishing Challenge

*/

#include "emu.h"
#include "cpu/arm7/arm7.h"
#include "cpu/z80/z80.h"
#include "machine/eeprom.h"
#include "machine/intelfsh.h"
#include "machine/x76f100.h"
#include "machine/maple-dc.h"
#include "machine/dc-ctrl.h"
#include "machine/mie.h"
#include "machine/naomirom.h"
#include "machine/naomigd.h"
#include "machine/naomim1.h"
#include "machine/naomim2.h"
#include "machine/naomim4.h"
#include "machine/awboard.h"
#include "includes/naomi.h"
#include "cpu/sh4/sh4.h"
#include "cpu/arm7/arm7core.h"
#include "sound/aica.h"
#include "includes/dc.h"
#include "machine/jvsdev.h"
#include "machine/jvs13551.h"

#define CPU_CLOCK (200000000)
static macronix_29l001mc_device *awflash;

                                             /* MD2 MD1 MD0 MD6 MD4 MD3 MD5 MD7 MD8 */
static const struct sh4_config sh4cpu_config = {  1,  0,  1,  0,  0,  0,  1,  1,  0, CPU_CLOCK };

static READ64_HANDLER( naomi_arm_r )
{
	dc_state *state = space->machine().driver_data<dc_state>();

	return *((UINT64 *)state->dc_sound_ram+offset);
}

static WRITE64_HANDLER( naomi_arm_w )
{
	dc_state *state = space->machine().driver_data<dc_state>();

	COMBINE_DATA((UINT64 *)state->dc_sound_ram + offset);
}

static READ64_HANDLER( naomi_unknown1_r )
{
	if ((offset * 8) == 0xc0) // trick so that it does not "wait for multiboard sync"
		return -1;
	return 0;
}

static WRITE64_HANDLER( naomi_unknown1_w )
{
}

/*
* Non-volatile memories
*/


static READ64_DEVICE_HANDLER( eeprom_93c46a_r )
{
	int res;

	/* bit 3 is EEPROM data */
	res = downcast<eeprom_device *>(device)->read_bit() << 4;
	return res;
}

static WRITE64_DEVICE_HANDLER( eeprom_93c46a_w )
{
	/* bit 4 is data */
	/* bit 2 is clock */
	/* bit 5 is cs */
	eeprom_device *eeprom = downcast<eeprom_device *>(device);
	eeprom->write_bit(data & 0x8);
	eeprom->set_cs_line((data & 0x20) ? CLEAR_LINE : ASSERT_LINE);
	eeprom->set_clock_line((data & 0x4) ? ASSERT_LINE : CLEAR_LINE);
}

/* Dreamcast MAP

0 0x00000000 - 0x001FFFFF MPX System/Boot ROM
0 0x00200000 - 0x0021FFFF Flash Memory
0 0x00400000 - 0x005F67FF Unassigned
0 0x005F6800 - 0x005F69FF System Control Reg.
0 0x005F6C00 - 0x005F6CFF Maple i/f Control Reg.
0 0x005F7000 - 0x005F70FF GD-ROM
0 0x005F7400 - 0x005F74FF G1 i/f Control Reg.
0 0x005F7800 - 0x005F78FF G2 i/f Control Reg.
0 0x005F7C00 - 0x005F7CFF PVR i/f Control Reg.
0 0x005F8000 - 0x005F9FFF TA / PVR Core Reg.
0 0x00600000 - 0x006007FF MODEM
0 0x00600800 - 0x006FFFFF G2 (Reserved)
0 0x00700000 - 0x00707FFF AICA- Sound Cntr. Reg.
0 0x00710000 - 0x0071000B AICA- RTC Cntr. Reg.
0 0x00800000 - 0x00FFFFFF AICA- Wave Memory
0 0x01000000 - 0x01FFFFFF Ext. Device
0 0x02000000 - 0x03FFFFFF Image Area (Mirror Area)

1 0x04000000 - 0x04FFFFFF MPX Tex.Mem. 64bit Acc.
1 0x05000000 - 0x05FFFFFF Tex.Mem. 32bit Acc.
1 0x06000000 - 0x07FFFFFF Image Area*

2 0x08000000 - 0x0BFFFFFF Unassigned

3 0x0C000000 - 0x0CFFFFFF System Memory
3 0x0D000000 - 0x0DFFFFFF (Mirror on DC, Extra RAM on Naomi)

3 0x0E000000 - 0x0FFFFFFF Image Area (Mirror Area)

4 0x10000000 - 0x107FFFFF MPX TA FIFO Polygon Cnv.
4 0x10800000 - 0x10FFFFFF TA FIFO YUV Conv.
4 0x11000000 - 0x11FFFFFF Tex.Mem. 32/64bit Acc.
4 0x12000000 - 0x13FFFFFF Image Area (Mirror Area)

5 0x14000000 - 0x17FFFFFF MPX Ext.

6 0x18000000 - 0x1BFFFFFF Unassigned

7 0x1C000000 - 0x1FFFFFFF(SH4 Internal area)



*/

/*
 * Common address map for Naomi 1, Naomi GD-Rom, Naomi 2, Atomiswave ...
 */

 // SB_LMMODE0
 static WRITE64_HANDLER( ta_texture_directpath0_w )
 {
	dc_state *state = space->machine().driver_data<dc_state>();

	int mode = state->pvrctrl_regs[SB_LMMODE0]&1;
	if (mode&1)
	{
		printf("ta_texture_directpath0_w 32-bit access!\n");
		COMBINE_DATA(&state->dc_framebuffer_ram[offset]);
	}
	else
	{
		COMBINE_DATA(&state->dc_texture_ram[offset]);
	}
 }

 // SB_LMMODE1
 static WRITE64_HANDLER( ta_texture_directpath1_w )
 {
	dc_state *state = space->machine().driver_data<dc_state>();

	int mode = state->pvrctrl_regs[SB_LMMODE1]&1;
	if (mode&1)
	{
		printf("ta_texture_directpath1_w 32-bit access!\n");
		COMBINE_DATA(&state->dc_framebuffer_ram[offset]);
	}
	else
	{
		COMBINE_DATA(&state->dc_texture_ram[offset]);
	}
 }


/*
 * Naomi 1 address map
 */

static ADDRESS_MAP_START( naomi_map, AS_PROGRAM, 64 )
	/* Area 0 */
	AM_RANGE(0x00000000, 0x001fffff) AM_MIRROR(0xa2000000) AM_ROM AM_REGION("maincpu", 0) // BIOS

	AM_RANGE(0x00200000, 0x00207fff) AM_MIRROR(0x02000000) AM_RAM                                             // bios uses it (battery backed ram ?)
	AM_RANGE(0x005f6800, 0x005f69ff) AM_MIRROR(0x02000000) AM_READWRITE( dc_sysctrl_r, dc_sysctrl_w )
	AM_RANGE(0x005f6c00, 0x005f6cff) AM_MIRROR(0x02000000) AM_DEVICE32( "maple_dc", maple_dc_device, amap, U64(0xffffffffffffffff) )
	AM_RANGE(0x005f7000, 0x005f70ff) AM_MIRROR(0x02000000) AM_DEVICE16( "rom_board", naomi_board, submap, U64(0x0000ffff0000ffff) )
	AM_RANGE(0x005f7400, 0x005f74ff) AM_MIRROR(0x02000000) AM_DEVICE32( "rom_board", naomi_g1_device, amap, U64(0xffffffffffffffff) )
	AM_RANGE(0x005f7800, 0x005f78ff) AM_MIRROR(0x02000000) AM_READWRITE( dc_g2_ctrl_r, dc_g2_ctrl_w )
	AM_RANGE(0x005f7c00, 0x005f7cff) AM_MIRROR(0x02000000) AM_READWRITE( pvr_ctrl_r, pvr_ctrl_w )
	AM_RANGE(0x005f8000, 0x005f9fff) AM_MIRROR(0x02000000) AM_READWRITE( pvr_ta_r, pvr_ta_w )
	AM_RANGE(0x00600000, 0x006007ff) AM_MIRROR(0x02000000) AM_READWRITE( dc_modem_r, dc_modem_w )
	AM_RANGE(0x00700000, 0x00707fff) AM_MIRROR(0x02000000) AM_DEVREADWRITE( "aica", dc_aica_reg_r, dc_aica_reg_w )
	AM_RANGE(0x00710000, 0x0071000f) AM_MIRROR(0x02000000) AM_READWRITE( dc_rtc_r, dc_rtc_w )
	AM_RANGE(0x00800000, 0x00ffffff) AM_MIRROR(0x02000000) AM_READWRITE( naomi_arm_r, naomi_arm_w )           // sound RAM (8 MB)

	/* External Device */
	AM_RANGE(0x01010098, 0x0101009f) AM_MIRROR(0x02000000) AM_RAM	// Naomi 2 BIOS tests this, needs to read back as written
	AM_RANGE(0x0103ff00, 0x0103ffff) AM_MIRROR(0x02000000) AM_READWRITE( naomi_unknown1_r, naomi_unknown1_w ) // bios uses it, actual start and end addresses not known

	/* Area 1 */
	AM_RANGE(0x04000000, 0x04ffffff) AM_MIRROR(0x02000000) AM_RAM AM_BASE_MEMBER( dc_state,dc_texture_ram )      // texture memory 64 bit access
	AM_RANGE(0x05000000, 0x05ffffff) AM_MIRROR(0x02000000) AM_RAM AM_BASE_MEMBER( dc_state,dc_framebuffer_ram ) // apparently this actually accesses the same memory as the 64-bit texture memory access, but in a different format, keep it apart for now

	/* Area 2*/
	AM_RANGE(0x08000000, 0x09ffffff) AM_MIRROR(0x02000000) AM_NOP // 'Unassigned'

	/* Area 3 */
	AM_RANGE(0x0c000000, 0x0dffffff) AM_MIRROR(0xa2000000) AM_RAM AM_BASE(&naomi_ram64)

	/* Area 4 */
	AM_RANGE(0x10000000, 0x107fffff) AM_MIRROR(0x02000000) AM_WRITE( ta_fifo_poly_w )
	AM_RANGE(0x10800000, 0x10ffffff) AM_MIRROR(0x02000000) AM_WRITE( ta_fifo_yuv_w )
	AM_RANGE(0x11000000, 0x11ffffff) AM_WRITE( ta_texture_directpath0_w ) // access to texture / framebuffer memory (either 32-bit or 64-bit area depending on SB_LMMODE0 register - cannot be written directly, only through dma / store queue)
	/*       0x12000000 -0x13ffffff Mirror area of  0x10000000 -0x11ffffff */
	AM_RANGE(0x13000000, 0x13ffffff) AM_WRITE( ta_texture_directpath1_w ) // access to texture / framebuffer memory (either 32-bit or 64-bit area depending on SB_LMMODE1 register - cannot be written directly, only through dma / store queue)

	/* Area 5 */
	//AM_RANGE(0x14000000, 0x17ffffff) AM_NOP // MPX Ext.

	/* Area 6 */
	//AM_RANGE(0x18000000, 0x1bffffff) AM_NOP // Unassigned

	/* Area 7 */
	//AM_RANGE(0x1c000000, 0x1fffffff) AM_NOP // SH4 Internal
ADDRESS_MAP_END

/*
 * Naomi 2 address map
 */

static ADDRESS_MAP_START( naomi2_map, AS_PROGRAM, 64 )
	/* Area 0 */
	AM_RANGE(0x00000000, 0x001fffff) AM_MIRROR(0xa2000000) AM_ROM AM_REGION("maincpu", 0) // BIOS

	AM_RANGE(0x00200000, 0x00207fff) AM_MIRROR(0x02000000) AM_RAM                                             // bios uses it (battery backed ram ?)
	AM_RANGE(0x005f6800, 0x005f69ff) AM_MIRROR(0x02000000) AM_READWRITE( dc_sysctrl_r, dc_sysctrl_w )
	AM_RANGE(0x005f6c00, 0x005f6cff) AM_MIRROR(0x02000000) AM_DEVICE32( "maple_dc", maple_dc_device, amap, U64(0xffffffffffffffff) )
	AM_RANGE(0x005f7000, 0x005f70ff) AM_MIRROR(0x02000000) AM_DEVICE16( "rom_board", naomi_board, submap, U64(0x0000ffff0000ffff) )
	AM_RANGE(0x005f7400, 0x005f74ff) AM_MIRROR(0x02000000) AM_DEVICE32( "rom_board", naomi_g1_device, amap, U64(0xffffffffffffffff) )
	AM_RANGE(0x005f7800, 0x005f78ff) AM_MIRROR(0x02000000) AM_READWRITE( dc_g2_ctrl_r, dc_g2_ctrl_w )
	AM_RANGE(0x005f7c00, 0x005f7cff) AM_READWRITE( pvr_ctrl_r, pvr_ctrl_w )
	AM_RANGE(0x005f8000, 0x005f9fff) AM_READWRITE( pvr_ta_r, pvr_ta_w )
	AM_RANGE(0x00600000, 0x006007ff) AM_MIRROR(0x02000000) AM_READWRITE( dc_modem_r, dc_modem_w )
	AM_RANGE(0x00700000, 0x00707fff) AM_MIRROR(0x02000000) AM_DEVREADWRITE( "aica", dc_aica_reg_r, dc_aica_reg_w )
	AM_RANGE(0x00710000, 0x0071000f) AM_MIRROR(0x02000000) AM_READWRITE( dc_rtc_r, dc_rtc_w )
	AM_RANGE(0x00800000, 0x00ffffff) AM_MIRROR(0x02000000) AM_READWRITE( naomi_arm_r, naomi_arm_w )           // sound RAM (8 MB)

	/* External Device */
	AM_RANGE(0x01010098, 0x0101009f) AM_MIRROR(0x02000000) AM_RAM	// Naomi 2 BIOS tests this, needs to read back as written
	AM_RANGE(0x0103ff00, 0x0103ffff) AM_MIRROR(0x02000000) AM_READWRITE( naomi_unknown1_r, naomi_unknown1_w ) // bios uses it, actual start and end addresses not known

//  AM_RANGE(0x025f6800, 0x025f69ff) AM_READWRITE( dc_sysctrl_r, dc_sysctrl_w ) // second PVR DMA!
//  AM_RANGE(0x025f7c00, 0x025f7cff) AM_READWRITE( pvr_ctrl_r, pvr_ctrl_w )
	AM_RANGE(0x025f8000, 0x025f9fff) AM_READWRITE( pvr2_ta_r, pvr2_ta_w )

	/* Area 1 */
	AM_RANGE(0x04000000, 0x04ffffff) AM_RAM AM_BASE_MEMBER( dc_state, dc_texture_ram )      // texture memory 64 bit access
	AM_RANGE(0x05000000, 0x05ffffff) AM_RAM AM_BASE_MEMBER( dc_state, dc_framebuffer_ram ) // apparently this actually accesses the same memory as the 64-bit texture memory access, but in a different format, keep it apart for now
	AM_RANGE(0x06000000, 0x06ffffff) AM_RAM AM_BASE_MEMBER( dc_state, pvr2_texture_ram )   // 64 bit access 2nd PVR RAM
	AM_RANGE(0x07000000, 0x07ffffff) AM_RAM AM_BASE_MEMBER( dc_state, pvr2_framebuffer_ram )// 32 bit access 2nd PVR RAM

	/* Area 2*/
	AM_RANGE(0x085f6800, 0x085f69ff) AM_WRITE( dc_sysctrl_w ) // writes to BOTH PVRs
	AM_RANGE(0x085f8000, 0x085f9fff) AM_WRITE( pvrs_ta_w ) // writes to BOTH PVRs
	AM_RANGE(0x08800000, 0x088000ff) AM_READWRITE32( elan_regs_r, elan_regs_w, U64(0xffffffffffffffff) ) // T&L chip registers
//  AM_RANGE(0x09000000, 0x09??????) T&L command processing
    AM_RANGE(0x0a000000, 0x0bffffff) AM_RAM AM_BASE_MEMBER( dc_state, elan_ram ) // T&L chip RAM

	/* Area 3 */
	AM_RANGE(0x0c000000, 0x0dffffff) AM_MIRROR(0xa2000000) AM_RAM AM_BASE(&naomi_ram64)

	/* Area 4 */
	AM_RANGE(0x10000000, 0x107fffff) AM_WRITE( ta_fifo_poly_w )
	AM_RANGE(0x10800000, 0x10ffffff) AM_WRITE( ta_fifo_yuv_w )
	AM_RANGE(0x11000000, 0x11ffffff) AM_WRITE( ta_texture_directpath0_w ) // access to texture / framebuffer memory (either 32-bit or 64-bit area depending on SB_LMMODE0 register - cannot be written directly, only through dma / store queue)
	/*       0x12000000 -0x13ffffff Mirror area of  0x10000000 -0x11ffffff */
	AM_RANGE(0x13000000, 0x13ffffff) AM_WRITE( ta_texture_directpath1_w ) // access to texture / framebuffer memory (either 32-bit or 64-bit area depending on SB_LMMODE1 register - cannot be written directly, only through dma / store queue)

	/* Area 5 */
	//AM_RANGE(0x14000000, 0x17ffffff) AM_NOP // MPX Ext.

	/* Area 6 */
	//AM_RANGE(0x18000000, 0x1bffffff) AM_NOP // Unassigned

	/* Area 7 */
	//AM_RANGE(0x1c000000, 0x1fffffff) AM_NOP // SH4 Internal
ADDRESS_MAP_END


static ADDRESS_MAP_START( naomi_port, AS_IO, 64 )
	AM_RANGE(0x00, 0x0f) AM_DEVREADWRITE("main_eeprom", eeprom_93c46a_r, eeprom_93c46a_w)
ADDRESS_MAP_END

/*
 * Atomiswave address map, almost identical to Dreamcast
 */

static READ64_HANDLER( aw_flash_r )
{
	return (UINT64)awflash->read(offset*8) | (UINT64)awflash->read((offset*8)+1)<<8 | (UINT64)awflash->read((offset*8)+2)<<16 | (UINT64)awflash->read((offset*8)+3)<<24 |
	       (UINT64)awflash->read((offset*8)+4)<<32 | (UINT64)awflash->read((offset*8)+5)<<40 | (UINT64)awflash->read((offset*8)+6)<<48 | (UINT64)awflash->read((offset*8)+7)<<56;
}

static WRITE64_HANDLER( aw_flash_w )
{
	int i;
	UINT32 addr = offset * 8;

	for (i = 0; i < 8; i++)
	{
		if (mem_mask & ((UINT64)0xff)<< (i*8))
		{
			addr += i;
			break;
		}
	}

	data >>= (i*8);

	awflash->write(addr, data);
}

INLINE int decode_reg32_64(running_machine &machine, UINT32 offset, UINT64 mem_mask, UINT64 *shift)
{
	int reg = offset * 2;

	*shift = 0;

	// non 32-bit accesses have not yet been seen here, we need to know when they are
	if ((mem_mask != U64(0xffffffff00000000)) && (mem_mask != U64(0x00000000ffffffff)))
	{
		mame_printf_verbose("%s:Wrong mask!\n", machine.describe_context());
//      debugger_break(machine);
	}

	if (mem_mask == U64(0xffffffff00000000))
	{
		reg++;
		*shift = 32;
	}

	return reg;
}

static READ64_HANDLER( aw_modem_r )
{
	int reg;
	UINT64 shift;

	reg = decode_reg32_64(space->machine(), offset, mem_mask, &shift);

	if (reg == 0x280/4)
	{
		UINT32 coins = input_port_read(space->machine(), "COINS");

		if (coins & 0x01)
		{
			return U64(0xffffffff00000002);	// coin A
		}
		else if (coins & 0x02)
		{
			return U64(0xffffffff00000001);	// coin B
		}

		return U64(0xffffffffffffffff);
	}

	mame_printf_verbose("MODEM:  Unmapped read %08x\n", 0x600000+reg*4);
	return 0;
}

static WRITE64_HANDLER( aw_modem_w )
{
	int reg;
	UINT64 shift;
	UINT32 dat;

	reg = decode_reg32_64(space->machine(), offset, mem_mask, &shift);
	dat = (UINT32)(data >> shift);
	mame_printf_verbose("MODEM: [%08x=%x] write %" I64FMT "x to %x, mask %" I64FMT "x\n", 0x600000+reg*4, dat, data, offset, mem_mask);
}

static ADDRESS_MAP_START( aw_map, AS_PROGRAM, 64 )
	/* Area 0 */
	AM_RANGE(0x00000000, 0x0001ffff) AM_READWRITE( aw_flash_r, aw_flash_w ) AM_REGION("awflash", 0)
	AM_RANGE(0xa0000000, 0xa001ffff) AM_READWRITE( aw_flash_r, aw_flash_w ) AM_REGION("awflash", 0)

	AM_RANGE(0x00200000, 0x0021ffff) AM_RAM 	// battery backed up RAM
	AM_RANGE(0x005f6800, 0x005f69ff) AM_READWRITE( dc_sysctrl_r, dc_sysctrl_w )
	AM_RANGE(0x005f6c00, 0x005f6cff) AM_MIRROR(0x02000000) AM_DEVICE32( "maple_dc", maple_dc_device, amap, U64(0xffffffffffffffff) )
	AM_RANGE(0x005f7000, 0x005f70ff) AM_MIRROR(0x02000000) AM_DEVICE16( "rom_board", aw_rom_board, submap, U64(0x0000ffff0000ffff) )
	AM_RANGE(0x005f7400, 0x005f74ff) AM_MIRROR(0x02000000) AM_DEVICE32( "rom_board", naomi_g1_device, amap, U64(0xffffffffffffffff) )
	AM_RANGE(0x005f7800, 0x005f78ff) AM_READWRITE( dc_g2_ctrl_r, dc_g2_ctrl_w )
	AM_RANGE(0x005f7c00, 0x005f7cff) AM_READWRITE( pvr_ctrl_r, pvr_ctrl_w )
	AM_RANGE(0x005f8000, 0x005f9fff) AM_READWRITE( pvr_ta_r, pvr_ta_w )
	AM_RANGE(0x00600000, 0x006007ff) AM_READWRITE( aw_modem_r, aw_modem_w )
	AM_RANGE(0x00700000, 0x00707fff) AM_DEVREADWRITE( "aica", dc_aica_reg_r, dc_aica_reg_w )
	AM_RANGE(0x00710000, 0x0071000f) AM_READWRITE( dc_rtc_r, dc_rtc_w )
	AM_RANGE(0x00800000, 0x00ffffff) AM_READWRITE( naomi_arm_r, naomi_arm_w )           // sound RAM (8 MB)


	AM_RANGE(0x0103ff00, 0x0103ffff) AM_READWRITE( naomi_unknown1_r, naomi_unknown1_w ) // bios uses it, actual start and end addresses not known

	/* Area 1 - half the texture memory, like dreamcast, not naomi */
	AM_RANGE(0x04000000, 0x047fffff) AM_RAM	AM_MIRROR(0x00800000) AM_BASE_MEMBER( dc_state, dc_texture_ram )      // texture memory 64 bit access
	AM_RANGE(0x05000000, 0x057fffff) AM_RAM AM_MIRROR(0x00800000) AM_BASE_MEMBER( dc_state, dc_framebuffer_ram ) // apparently this actually accesses the same memory as the 64-bit texture memory access, but in a different format, keep it apart for now

	/* Area 2*/
	AM_RANGE(0x08000000, 0x0bffffff) AM_NOP // 'Unassigned'

	/* Area 3 */
	AM_RANGE(0x0c000000, 0x0cffffff) AM_RAM AM_BASE(&naomi_ram64) AM_SHARE("share4")
	AM_RANGE(0x0d000000, 0x0dffffff) AM_RAM AM_SHARE("share4")// extra ram on Naomi (mirror on DC)
	AM_RANGE(0x0e000000, 0x0effffff) AM_RAM AM_SHARE("share4")// mirror
	AM_RANGE(0x0f000000, 0x0fffffff) AM_RAM AM_SHARE("share4")// mirror

	AM_RANGE(0x8c000000, 0x8cffffff) AM_RAM AM_SHARE("share4") // RAM access through cache
	AM_RANGE(0x8d000000, 0x8dffffff) AM_RAM AM_SHARE("share4") // RAM access through cache

	/* Area 4 - half the texture memory, like dreamcast, not naomi */
	AM_RANGE(0x10000000, 0x107fffff) AM_MIRROR(0x02000000) AM_WRITE( ta_fifo_poly_w )
	AM_RANGE(0x10800000, 0x10ffffff) AM_MIRROR(0x02000000) AM_WRITE( ta_fifo_yuv_w )
	AM_RANGE(0x11000000, 0x117fffff) AM_WRITE( ta_texture_directpath0_w ) AM_MIRROR(0x00800000)  // access to texture / framebuffer memory (either 32-bit or 64-bit area depending on SB_LMMODE0 register - cannot be written directly, only through dma / store queue
	/*       0x12000000 -0x13ffffff Mirror area of  0x10000000 -0x11ffffff */
	AM_RANGE(0x13000000, 0x137fffff) AM_WRITE( ta_texture_directpath1_w ) AM_MIRROR(0x00800000) // access to texture / framebuffer memory (either 32-bit or 64-bit area depending on SB_LMMODE1 register - cannot be written directly, only through dma / store queue


	/* Area 5 */
	//AM_RANGE(0x14000000, 0x17ffffff) AM_NOP // MPX Ext.

	/* Area 6 */
	//AM_RANGE(0x18000000, 0x1bffffff) AM_NOP // Unassigned

	/* Area 7 */
	//AM_RANGE(0x1c000000, 0x1fffffff) AM_NOP // SH4 Internal
ADDRESS_MAP_END

/*
 * Aica
 */
static void aica_irq(device_t *device, int irq)
{
	cputag_set_input_line(device->machine(), "soundcpu", ARM7_FIRQ_LINE, irq ? ASSERT_LINE : CLEAR_LINE);
}


static const aica_interface aica_config =
{
	TRUE,
	0,
	aica_irq
};


static ADDRESS_MAP_START( dc_audio_map, AS_PROGRAM, 32 )
	ADDRESS_MAP_UNMAP_HIGH
	AM_RANGE(0x00000000, 0x007fffff) AM_RAM	AM_BASE_MEMBER( dc_state, dc_sound_ram )                /* shared with SH-4 */
	AM_RANGE(0x00800000, 0x00807fff) AM_DEVREADWRITE("aica", dc_arm_aica_r, dc_arm_aica_w)
ADDRESS_MAP_END

/*
* Input ports
*/

static INPUT_PORTS_START( naomi_debug )
	PORT_START("MAMEDEBUG")
	PORT_DIPNAME( 0x01, 0x00, "Bilinear Filtering" )
	PORT_DIPSETTING(    0x00, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x01, DEF_STR( On ) )
	PORT_DIPNAME( 0x02, 0x00, "Disable Render Calls" )
	PORT_DIPSETTING(    0x00, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x02, DEF_STR( On ) )
INPUT_PORTS_END

static INPUT_PORTS_START( naomi_mie )
	PORT_START("MIE:3")
	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_OUTPUT ) PORT_WRITE_LINE_DEVICE_MEMBER("mie_eeprom", eeprom_device, write_bit)
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_OUTPUT ) PORT_WRITE_LINE_DEVICE_MEMBER("mie_eeprom", eeprom_device, set_cs_line)
	PORT_BIT( 0x40, IP_ACTIVE_HIGH, IPT_OUTPUT ) PORT_WRITE_LINE_DEVICE_MEMBER("mie_eeprom", eeprom_device, set_clock_line)

	PORT_START("MIE:5")
	PORT_DIPNAME( 0x01, 0x00, "Monitor" ) PORT_DIPLOCATION("SW1:1")
	PORT_DIPSETTING(    0x01, "31 kHz" )
	PORT_DIPSETTING(    0x00, "15 kHz" )
	PORT_DIPNAME( 0x02, 0x02, DEF_STR( Unknown ) ) PORT_DIPLOCATION("SW1:2")
	PORT_DIPSETTING(    0x02, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x04, 0x04, DEF_STR( Unknown ) ) PORT_DIPLOCATION("SW1:3")
	PORT_DIPSETTING(    0x04, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_DIPNAME( 0x08, 0x08, DEF_STR( Unknown ) ) PORT_DIPLOCATION("SW1:4")
	PORT_DIPSETTING(    0x08, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x00, DEF_STR( On ) )
	PORT_SERVICE_NO_TOGGLE( 0x10, IP_ACTIVE_LOW )
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_SERVICE1 )
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_SERVICE1 )
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_SPECIAL ) PORT_READ_LINE_DEVICE_MEMBER("mie_eeprom", eeprom_device, read_bit)
INPUT_PORTS_END

/* 2 players with 1 joystick and 6 buttons each */
static INPUT_PORTS_START( naomi )
	PORT_INCLUDE( naomi_mie )
	PORT_INCLUDE( naomi_debug )

	PORT_START("TILT")
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_TILT )
	PORT_BIT( 0x7f, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("P1")
	PORT_BIT( 0x8000, IP_ACTIVE_HIGH, IPT_START1 )
	PORT_BIT( 0x2000, IP_ACTIVE_HIGH, IPT_JOYSTICK_UP ) PORT_8WAY
	PORT_BIT( 0x1000, IP_ACTIVE_HIGH, IPT_JOYSTICK_DOWN ) PORT_8WAY
	PORT_BIT( 0x0800, IP_ACTIVE_HIGH, IPT_JOYSTICK_LEFT ) PORT_8WAY
	PORT_BIT( 0x0400, IP_ACTIVE_HIGH, IPT_JOYSTICK_RIGHT ) PORT_8WAY
	PORT_BIT( 0x0200, IP_ACTIVE_HIGH, IPT_BUTTON1 )
	PORT_BIT( 0x0100, IP_ACTIVE_HIGH, IPT_BUTTON2 )
	PORT_BIT( 0x0080, IP_ACTIVE_HIGH, IPT_BUTTON3 )
	PORT_BIT( 0x0040, IP_ACTIVE_HIGH, IPT_BUTTON4 )
	PORT_BIT( 0x0020, IP_ACTIVE_HIGH, IPT_BUTTON5 )
	PORT_BIT( 0x0010, IP_ACTIVE_HIGH, IPT_BUTTON6 )
	PORT_BIT( 0x400f, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("P2")
	PORT_BIT( 0x8000, IP_ACTIVE_HIGH, IPT_START2 )
	PORT_BIT( 0x2000, IP_ACTIVE_HIGH, IPT_JOYSTICK_UP ) PORT_8WAY PORT_PLAYER(2)
	PORT_BIT( 0x1000, IP_ACTIVE_HIGH, IPT_JOYSTICK_DOWN ) PORT_8WAY PORT_PLAYER(2)
	PORT_BIT( 0x0800, IP_ACTIVE_HIGH, IPT_JOYSTICK_LEFT ) PORT_8WAY PORT_PLAYER(2)
	PORT_BIT( 0x0400, IP_ACTIVE_HIGH, IPT_JOYSTICK_RIGHT ) PORT_8WAY PORT_PLAYER(2)
	PORT_BIT( 0x0200, IP_ACTIVE_HIGH, IPT_BUTTON1 ) PORT_PLAYER(2)
	PORT_BIT( 0x0100, IP_ACTIVE_HIGH, IPT_BUTTON2 ) PORT_PLAYER(2)
	PORT_BIT( 0x0080, IP_ACTIVE_HIGH, IPT_BUTTON3 ) PORT_PLAYER(2)
	PORT_BIT( 0x0040, IP_ACTIVE_HIGH, IPT_BUTTON4 ) PORT_PLAYER(2)
	PORT_BIT( 0x0020, IP_ACTIVE_HIGH, IPT_BUTTON5 ) PORT_PLAYER(2)
	PORT_BIT( 0x0010, IP_ACTIVE_HIGH, IPT_BUTTON6 ) PORT_PLAYER(2)
	PORT_BIT( 0x400f, IP_ACTIVE_HIGH, IPT_UNUSED )

	/* Dummy so we can easily get the analog ch # */
	PORT_START("A0")
	PORT_BIT( 0x00ff, IP_ACTIVE_LOW, IPT_UNUSED )

	PORT_START("A1")
	PORT_BIT( 0x01ff, IP_ACTIVE_LOW, IPT_UNUSED )

	PORT_START("A2")
	PORT_BIT( 0x02ff, IP_ACTIVE_LOW, IPT_UNUSED )

	PORT_START("A3")
	PORT_BIT( 0x03ff, IP_ACTIVE_LOW, IPT_UNUSED )

	PORT_START("A4")
	PORT_BIT( 0x04ff, IP_ACTIVE_LOW, IPT_UNUSED )

	PORT_START("A5")
	PORT_BIT( 0x05ff, IP_ACTIVE_LOW, IPT_UNUSED )

	PORT_START("A6")
	PORT_BIT( 0x06ff, IP_ACTIVE_LOW, IPT_UNUSED )

	PORT_START("A7")
	PORT_BIT( 0x07ff, IP_ACTIVE_LOW, IPT_UNUSED )
INPUT_PORTS_END

static INPUT_PORTS_START( hotd2 )
	PORT_INCLUDE( naomi_mie )
	PORT_INCLUDE( naomi_debug )

	PORT_START("TILT")
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_TILT )
	PORT_BIT( 0x7f, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("P1")
	PORT_BIT( 0x8000, IP_ACTIVE_HIGH, IPT_START1 )
	PORT_BIT( 0x0200, IP_ACTIVE_HIGH, IPT_BUTTON1 ) PORT_NAME("P1 Trigger") PORT_PLAYER(1)
	PORT_BIT( 0x0100, IP_ACTIVE_HIGH, IPT_BUTTON2 ) PORT_NAME("P1 Screen-In") PORT_PLAYER(1) //reload
	PORT_BIT( 0x7cff, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("P2")
	PORT_BIT( 0x8000, IP_ACTIVE_HIGH, IPT_START2 )
	PORT_BIT( 0x0200, IP_ACTIVE_HIGH, IPT_BUTTON1 ) PORT_NAME("P2 Trigger") PORT_PLAYER(2)
	PORT_BIT( 0x0100, IP_ACTIVE_HIGH, IPT_BUTTON2 ) PORT_NAME("P2 Screen-In") PORT_PLAYER(2) //reload
	PORT_BIT( 0x7cff, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("A0")
	PORT_BIT( 0xff00, 0x8000, IPT_LIGHTGUN_X ) PORT_CROSSHAIR(X, 1.0, 0.0, 0) PORT_SENSITIVITY(25) PORT_KEYDELTA(10) PORT_PLAYER(1)
	PORT_BIT( 0x00ff, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("A1")
	PORT_BIT( 0xff00, 0x8000, IPT_LIGHTGUN_Y ) PORT_CROSSHAIR(Y, 1.0, 0.0, 0) PORT_SENSITIVITY(25) PORT_KEYDELTA(10) PORT_PLAYER(1)
	PORT_BIT( 0x00ff, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("A2")
	PORT_BIT( 0xff00, 0x8000, IPT_LIGHTGUN_X ) PORT_CROSSHAIR(X, 1.0, 0.0, 0) PORT_SENSITIVITY(25) PORT_KEYDELTA(10) PORT_PLAYER(2)
	PORT_BIT( 0x00ff, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("A3")
	PORT_BIT( 0xff00, 0x8000, IPT_LIGHTGUN_Y ) PORT_CROSSHAIR(Y, 1.0, 0.0, 0) PORT_SENSITIVITY(25) PORT_KEYDELTA(10) PORT_PLAYER(2)
	PORT_BIT( 0x00ff, IP_ACTIVE_HIGH, IPT_UNUSED )
INPUT_PORTS_END

static INPUT_PORTS_START( crzytaxi )
	PORT_INCLUDE( naomi_mie )
	PORT_INCLUDE( naomi_debug )

	PORT_START("TILT")
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_TILT )
	PORT_BIT( 0x7f, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("P1")
	PORT_BIT( 0x8000, IP_ACTIVE_HIGH, IPT_START1 )
	PORT_BIT( 0x2000, IP_ACTIVE_HIGH, IPT_BUTTON1 ) PORT_NAME("Drive Gear")
	PORT_BIT( 0x1000, IP_ACTIVE_HIGH, IPT_BUTTON2 ) PORT_NAME("Reverse Gear")
	PORT_BIT( 0x4fff, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("P2")
	PORT_BIT( 0xffff, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("A0")
	PORT_BIT( 0xff00, 0x8000, IPT_PADDLE ) PORT_SENSITIVITY(30) PORT_KEYDELTA(30)
	PORT_BIT( 0x00ff, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("A1")
	PORT_BIT( 0xff00, 0x0000, IPT_PEDAL ) PORT_MINMAX(0x00,0xff00) PORT_SENSITIVITY(100) PORT_KEYDELTA(40)
	PORT_BIT( 0x00ff, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("A2")
	PORT_BIT( 0xff00, 0x0000, IPT_PEDAL2 ) PORT_MINMAX(0x00,0xff00) PORT_SENSITIVITY(100) PORT_KEYDELTA(40)
	PORT_BIT( 0x00ff, IP_ACTIVE_HIGH, IPT_UNUSED )
INPUT_PORTS_END

static INPUT_PORTS_START( dybbnao )
	PORT_INCLUDE( naomi_mie )
	PORT_INCLUDE( naomi_debug )

	PORT_START("TILT")
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_TILT )
	PORT_BIT( 0x7f, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("P1")
	PORT_BIT( 0x8000, IP_ACTIVE_HIGH, IPT_START1 )
	PORT_BIT( 0x0200, IP_ACTIVE_HIGH, IPT_BUTTON1 )
	PORT_BIT( 0x0100, IP_ACTIVE_HIGH, IPT_BUTTON2 )
	PORT_BIT( 0x7cff, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("P2")
	PORT_BIT( 0x8000, IP_ACTIVE_HIGH, IPT_START2 )
	PORT_BIT( 0x0200, IP_ACTIVE_HIGH, IPT_BUTTON1 ) PORT_PLAYER(2)
	PORT_BIT( 0x0100, IP_ACTIVE_HIGH, IPT_BUTTON2 ) PORT_PLAYER(2)
	PORT_BIT( 0x7cff, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("A0")
	PORT_BIT( 0xff00, 0x8000, IPT_AD_STICK_Y ) PORT_MINMAX(0x00, 0xff00) PORT_SENSITIVITY(25) PORT_KEYDELTA(200) PORT_REVERSE PORT_PLAYER(1)
	PORT_BIT( 0x00ff, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("A1")
	PORT_BIT( 0xff00, 0x8000, IPT_AD_STICK_X ) PORT_MINMAX(0x00, 0xff00) PORT_SENSITIVITY(25) PORT_KEYDELTA(200) PORT_REVERSE PORT_PLAYER(1)
	PORT_BIT( 0x00ff, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("A2")
	PORT_BIT( 0xff00, 0x0000, IPT_PEDAL ) PORT_SENSITIVITY(25) PORT_KEYDELTA(200) PORT_REVERSE PORT_PLAYER(1)
	PORT_BIT( 0x00ff, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("A4")
	PORT_BIT( 0xff00, 0x8000, IPT_AD_STICK_Y ) PORT_MINMAX(0x00, 0xff00) PORT_SENSITIVITY(25) PORT_KEYDELTA(200) PORT_REVERSE PORT_PLAYER(2)
	PORT_BIT( 0x00ff, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("A5")
	PORT_BIT( 0xff00, 0x8000, IPT_AD_STICK_X ) PORT_MINMAX(0x00, 0xff00) PORT_SENSITIVITY(25) PORT_KEYDELTA(200) PORT_REVERSE PORT_PLAYER(2)
	PORT_BIT( 0x00ff, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("A6")
	PORT_BIT( 0xff00, 0x0000, IPT_PEDAL ) PORT_SENSITIVITY(25) PORT_KEYDELTA(200) PORT_REVERSE PORT_PLAYER(2)
	PORT_BIT( 0x00ff, IP_ACTIVE_HIGH, IPT_UNUSED )
INPUT_PORTS_END

static INPUT_PORTS_START( zombrvn )
	PORT_INCLUDE( naomi_mie )
	PORT_INCLUDE( naomi_debug )

	PORT_START("TILT")
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_TILT )
	PORT_BIT( 0x7f, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("P1")
	PORT_BIT( 0x8000, IP_ACTIVE_HIGH, IPT_START1 )
	PORT_BIT( 0x0200, IP_ACTIVE_HIGH, IPT_BUTTON1 )
	PORT_BIT( 0x0100, IP_ACTIVE_HIGH, IPT_BUTTON2 )
	PORT_BIT( 0x0080, IP_ACTIVE_HIGH, IPT_BUTTON3 )
	PORT_BIT( 0x7c7f, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("P2")
	PORT_BIT( 0x8000, IP_ACTIVE_HIGH, IPT_START2 )
	PORT_BIT( 0x0200, IP_ACTIVE_HIGH, IPT_BUTTON1 ) PORT_PLAYER(2)
	PORT_BIT( 0x0100, IP_ACTIVE_HIGH, IPT_BUTTON2 ) PORT_PLAYER(2)
	PORT_BIT( 0x0080, IP_ACTIVE_HIGH, IPT_BUTTON3 ) PORT_PLAYER(2)
	PORT_BIT( 0x7c7f, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("A0")
	PORT_BIT( 0xff00, 0x8000, IPT_AD_STICK_Y ) PORT_MINMAX(0x00, 0xff00) PORT_SENSITIVITY(25) PORT_KEYDELTA(200) PORT_REVERSE PORT_PLAYER(1)
	PORT_BIT( 0x00ff, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("A1")
	PORT_BIT( 0xff00, 0x8000, IPT_AD_STICK_X ) PORT_MINMAX(0x00, 0xff00) PORT_SENSITIVITY(25) PORT_KEYDELTA(200) PORT_REVERSE PORT_PLAYER(1)
	PORT_BIT( 0x00ff, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("A4")
	PORT_BIT( 0xff00, 0x8000, IPT_AD_STICK_Y ) PORT_MINMAX(0x00, 0xff00) PORT_SENSITIVITY(25) PORT_KEYDELTA(200) PORT_REVERSE PORT_PLAYER(2)
	PORT_BIT( 0x00ff, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("A5")
	PORT_BIT( 0xff00, 0x8000, IPT_AD_STICK_X ) PORT_MINMAX(0x00, 0xff00) PORT_SENSITIVITY(25) PORT_KEYDELTA(200) PORT_REVERSE PORT_PLAYER(2)
	PORT_BIT( 0x00ff, IP_ACTIVE_HIGH, IPT_UNUSED )
INPUT_PORTS_END

static INPUT_PORTS_START( jambo )
	PORT_INCLUDE( naomi_mie )
	PORT_INCLUDE( naomi_debug )

	PORT_START("TILT")
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_TILT )
	PORT_BIT( 0x7f, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("P1")
	PORT_BIT( 0x8000, IP_ACTIVE_HIGH, IPT_START1 )
	PORT_BIT( 0x7fff, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("P2")
	PORT_BIT( 0x2000, IP_ACTIVE_HIGH, IPT_BUTTON1 ) PORT_NAME("Shift Down")
	PORT_BIT( 0x1000, IP_ACTIVE_HIGH, IPT_BUTTON2 ) PORT_NAME("Shift Up")
	PORT_BIT( 0xcfff, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("A0")
	PORT_BIT( 0xff00, 0x8000, IPT_PADDLE ) PORT_SENSITIVITY(30) PORT_KEYDELTA(30)
	PORT_BIT( 0x00ff, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("A1")
	PORT_BIT( 0xff00, 0x0000, IPT_PEDAL ) PORT_MINMAX(0x00,0xff00) PORT_SENSITIVITY(100) PORT_KEYDELTA(40)
	PORT_BIT( 0x00ff, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("A2")
	PORT_BIT( 0xff00, 0x0000, IPT_PEDAL2 ) PORT_MINMAX(0x00,0xff00) PORT_SENSITIVITY(100) PORT_KEYDELTA(40)
	PORT_BIT( 0x00ff, IP_ACTIVE_HIGH, IPT_UNUSED )
INPUT_PORTS_END

static INPUT_PORTS_START( 18wheelr )
	PORT_INCLUDE( naomi_mie )
	PORT_INCLUDE( naomi_debug )

	PORT_START("TILT")
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_TILT )
	PORT_BIT( 0x7f, IP_ACTIVE_HIGH, IPT_UNUSED )


	PORT_START("P1")
	PORT_BIT( 0x8000, IP_ACTIVE_HIGH, IPT_START1 )
	PORT_BIT( 0x1000, IP_ACTIVE_HIGH, IPT_BUTTON4 ) PORT_NAME("View")
	PORT_BIT( 0x0200, IP_ACTIVE_HIGH, IPT_BUTTON5 ) PORT_NAME("Horn")
	PORT_BIT( 0x6dff, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("P2")
	// TODO: this is a tri-state shift lever, arrangement can be better.
	PORT_BIT( 0x2000, IP_ACTIVE_HIGH, IPT_BUTTON2 ) PORT_NAME("Shift H")
	PORT_BIT( 0x1000, IP_ACTIVE_HIGH, IPT_BUTTON1 ) PORT_NAME("Shift L")
	PORT_BIT( 0x0800, IP_ACTIVE_HIGH, IPT_BUTTON3 ) PORT_NAME("Shift R")
	PORT_BIT( 0xc7ff, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("A0")
	PORT_BIT( 0xff00, 0x8000, IPT_PADDLE ) PORT_SENSITIVITY(30) PORT_KEYDELTA(30)
	PORT_BIT( 0x00ff, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("A1")
	PORT_BIT( 0xff00, 0x0000, IPT_PEDAL ) PORT_MINMAX(0x00,0xff00) PORT_SENSITIVITY(100) PORT_KEYDELTA(40)
	PORT_BIT( 0x00ff, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("A2")
	PORT_BIT( 0xff00, 0x0000, IPT_PEDAL2 ) PORT_MINMAX(0x00,0xff00) PORT_SENSITIVITY(100) PORT_KEYDELTA(40)
	PORT_BIT( 0x00ff, IP_ACTIVE_HIGH, IPT_UNUSED )
INPUT_PORTS_END

static INPUT_PORTS_START( alpilota )
	PORT_INCLUDE( naomi_mie )
	PORT_INCLUDE( naomi_debug )

	PORT_START("TILT")
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_TILT )
	PORT_BIT( 0x7f, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("P1")
	PORT_BIT( 0x8000, IP_ACTIVE_HIGH, IPT_START1 )
	PORT_BIT( 0x0200, IP_ACTIVE_HIGH, IPT_BUTTON1 ) PORT_NAME("Landing Gear Switch") PORT_TOGGLE
	PORT_BIT( 0x0100, IP_ACTIVE_HIGH, IPT_BUTTON2 ) PORT_NAME("View Change")
	PORT_BIT( 0x0080, IP_ACTIVE_HIGH, IPT_BUTTON3 ) PORT_NAME("Flap Switch") PORT_TOGGLE
	PORT_BIT( 0x7c7f, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("P2")
	PORT_BIT( 0xffff, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("A0")
	PORT_BIT( 0xff00, 0x8000, IPT_PADDLE ) PORT_SENSITIVITY(30) PORT_KEYDELTA(30) PORT_NAME("Elevator Wheel")
	PORT_BIT( 0x00ff, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("A1")
	PORT_BIT( 0xff00, 0x8000, IPT_PADDLE ) PORT_SENSITIVITY(30) PORT_KEYDELTA(30) PORT_NAME("Aileron Wheel") PORT_PLAYER(2)
	PORT_BIT( 0x00ff, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("A3")
	PORT_BIT( 0xff00, 0x0000, IPT_PEDAL ) PORT_MINMAX(0x00,0xff00) PORT_SENSITIVITY(100) PORT_KEYDELTA(40) PORT_NAME("Rudder Pedal")
	PORT_BIT( 0x00ff, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("A4")
	PORT_BIT( 0xff00, 0x8000, IPT_AD_STICK_Y ) PORT_MINMAX(0x00, 0xff00) PORT_SENSITIVITY(25) PORT_KEYDELTA(200) PORT_NAME("Thrust Lever L")
	PORT_BIT( 0x00ff, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("A5")
	PORT_BIT( 0xff00, 0x8000, IPT_AD_STICK_Y ) PORT_MINMAX(0x00, 0xff00) PORT_SENSITIVITY(25) PORT_KEYDELTA(200) PORT_NAME("Thrust Lever R") PORT_PLAYER(2)
	PORT_BIT( 0x00ff, IP_ACTIVE_HIGH, IPT_UNUSED )
INPUT_PORTS_END

static INPUT_PORTS_START( sstrkfgt )
	PORT_INCLUDE( naomi_mie )
	PORT_INCLUDE( naomi_debug )

	PORT_START("TILT")
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_TILT )
	PORT_BIT( 0x7f, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("P1")
	PORT_BIT( 0x8000, IP_ACTIVE_HIGH, IPT_START1 )
	PORT_BIT( 0x0200, IP_ACTIVE_HIGH, IPT_BUTTON1 ) PORT_NAME("Gun Trigger")
	PORT_BIT( 0x0100, IP_ACTIVE_HIGH, IPT_BUTTON2 ) PORT_NAME("Missile Button")
	PORT_BIT( 0x0080, IP_ACTIVE_HIGH, IPT_BUTTON3 ) PORT_NAME("Air Break")
	PORT_BIT( 0x0040, IP_ACTIVE_HIGH, IPT_BUTTON4 ) PORT_NAME("View Change")
	PORT_BIT( 0x7c3f, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("P2")
	PORT_BIT( 0xffff, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("A0")
	PORT_BIT( 0xff00, 0x8000, IPT_PADDLE ) PORT_SENSITIVITY(30) PORT_KEYDELTA(30) PORT_NAME("Elevator Wheel")
	PORT_BIT( 0x00ff, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("A1")
	PORT_BIT( 0xff00, 0x8000, IPT_PADDLE ) PORT_SENSITIVITY(30) PORT_KEYDELTA(30) PORT_NAME("Aileron Wheel") PORT_PLAYER(2)
	PORT_BIT( 0x00ff, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("A2")
	PORT_BIT( 0xff00, 0x8000, IPT_AD_STICK_Y ) PORT_MINMAX(0x00, 0xff00) PORT_SENSITIVITY(25) PORT_KEYDELTA(200) PORT_NAME("Thrust Lever")
	PORT_BIT( 0x00ff, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("A3")
	PORT_BIT( 0xff00, 0x0000, IPT_PEDAL ) PORT_MINMAX(0x00,0xff00) PORT_SENSITIVITY(100) PORT_KEYDELTA(40) PORT_NAME("Rudder Pedal")
	PORT_BIT( 0x00ff, IP_ACTIVE_HIGH, IPT_UNUSED )
INPUT_PORTS_END


static INPUT_PORTS_START( crackndj )
	PORT_INCLUDE( naomi_mie )
	PORT_INCLUDE( naomi_debug )

	PORT_START("TILT")
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_TILT )
	PORT_BIT( 0x7f, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("P1")
	PORT_BIT( 0x8000, IP_ACTIVE_HIGH, IPT_START1 )
	PORT_BIT( 0x7fff, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("P2")
	PORT_BIT( 0xffff, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("A0")
	PORT_BIT( 0xff00, 0x8000, IPT_AD_STICK_X ) PORT_MINMAX(0x00, 0xff00) PORT_SENSITIVITY(25) PORT_KEYDELTA(200) PORT_REVERSE PORT_NAME("Fader")
	PORT_BIT( 0x00ff, IP_ACTIVE_HIGH, IPT_UNUSED )
INPUT_PORTS_END

static INPUT_PORTS_START( monkeyba )
	PORT_INCLUDE( naomi_mie )
	PORT_INCLUDE( naomi_debug )

	PORT_START("TILT")
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_TILT )
	PORT_BIT( 0x7f, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("P1")
	PORT_BIT( 0x8000, IP_ACTIVE_HIGH, IPT_START1 )
	PORT_BIT( 0x7fff, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("P2")
	PORT_BIT( 0xffff, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("A0")
	PORT_BIT( 0xff00, 0x8000, IPT_AD_STICK_Y ) PORT_MINMAX(0x00, 0xff00) PORT_SENSITIVITY(25) PORT_KEYDELTA(200) PORT_REVERSE
	PORT_BIT( 0x00ff, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("A1")
	PORT_BIT( 0xff00, 0x8000, IPT_AD_STICK_X ) PORT_MINMAX(0x00, 0xff00) PORT_SENSITIVITY(25) PORT_KEYDELTA(200) PORT_REVERSE
	PORT_BIT( 0x00ff, IP_ACTIVE_HIGH, IPT_UNUSED )
INPUT_PORTS_END

static INPUT_PORTS_START( shaktamb )
	PORT_INCLUDE( naomi_mie )
	PORT_INCLUDE( naomi_debug )

	PORT_START("TILT")
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_TILT )
	PORT_BIT( 0x7f, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("P1")
	PORT_BIT( 0x8000, IP_ACTIVE_HIGH, IPT_START1 )
	PORT_BIT( 0x1000, IP_ACTIVE_HIGH, IPT_BUTTON1 ) PORT_NAME("P1 Knock Switch")
	PORT_BIT( 0x0800, IP_ACTIVE_HIGH, IPT_BUTTON4 ) PORT_NAME("P1 Down")
	PORT_BIT( 0x0400, IP_ACTIVE_HIGH, IPT_BUTTON5 ) PORT_NAME("P1 Up")
	PORT_BIT( 0x0200, IP_ACTIVE_HIGH, IPT_BUTTON2 ) PORT_NAME("P1 Shake L Switch")
	PORT_BIT( 0x0100, IP_ACTIVE_HIGH, IPT_BUTTON3 ) PORT_NAME("P1 Shake R Switch")
	PORT_BIT( 0x0080, IP_ACTIVE_HIGH, IPT_BUTTON6 ) PORT_NAME("P1 Screen-In")
	PORT_BIT( 0x607f, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("P2")
	PORT_BIT( 0x8000, IP_ACTIVE_HIGH, IPT_START2 )
	PORT_BIT( 0x1000, IP_ACTIVE_HIGH, IPT_BUTTON1 ) PORT_NAME("P2 Knock Switch") PORT_PLAYER(2)
	PORT_BIT( 0x0800, IP_ACTIVE_HIGH, IPT_BUTTON4 ) PORT_NAME("P2 Down") PORT_PLAYER(2)
	PORT_BIT( 0x0400, IP_ACTIVE_HIGH, IPT_BUTTON5 ) PORT_NAME("P2 Up") PORT_PLAYER(2)
	PORT_BIT( 0x0200, IP_ACTIVE_HIGH, IPT_BUTTON2 ) PORT_NAME("P2 Shake L Switch") PORT_PLAYER(2)
	PORT_BIT( 0x0100, IP_ACTIVE_HIGH, IPT_BUTTON3 ) PORT_NAME("P2 Shake R Switch") PORT_PLAYER(2)
	PORT_BIT( 0x0080, IP_ACTIVE_HIGH, IPT_BUTTON6 ) PORT_NAME("P2 Screen-In") PORT_PLAYER(2)
	PORT_BIT( 0x607f, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("A0")
	PORT_BIT( 0xff00, 0x8000, IPT_AD_STICK_X ) PORT_MINMAX(0x00, 0xff00) PORT_SENSITIVITY(25) PORT_KEYDELTA(200) PORT_REVERSE
	PORT_BIT( 0x00ff, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("A1")
	PORT_BIT( 0xff00, 0x8000, IPT_AD_STICK_Y ) PORT_MINMAX(0x00, 0xff00) PORT_SENSITIVITY(25) PORT_KEYDELTA(200) PORT_REVERSE
	PORT_BIT( 0x00ff, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("A3")
	PORT_BIT( 0xff00, 0x8000, IPT_AD_STICK_X ) PORT_MINMAX(0x00, 0xff00) PORT_SENSITIVITY(25) PORT_KEYDELTA(200) PORT_REVERSE PORT_PLAYER(2)
	PORT_BIT( 0x00ff, IP_ACTIVE_HIGH, IPT_UNUSED )

	PORT_START("A4")
	PORT_BIT( 0xff00, 0x8000, IPT_AD_STICK_Y ) PORT_MINMAX(0x00, 0xff00) PORT_SENSITIVITY(25) PORT_KEYDELTA(200) PORT_REVERSE PORT_PLAYER(2)
	PORT_BIT( 0x00ff, IP_ACTIVE_HIGH, IPT_UNUSED )
INPUT_PORTS_END


/* JVS mahjong panel */
static INPUT_PORTS_START( naomi_mp )
	PORT_INCLUDE( naomi_mie )
	PORT_INCLUDE( naomi_debug )

	PORT_START("P1") /* inputs are all there, it needs a mux write mechanism of some sort ... */
	PORT_DIPNAME( 0x0100, 0x0000, "SYSA" )
	PORT_DIPSETTING(    0x0000, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x0100, DEF_STR( On ) )
	PORT_DIPNAME( 0x0200, 0x0000, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x0000, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x0200, DEF_STR( On ) )
	PORT_DIPNAME( 0x0400, 0x0000, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x0000, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x0400, DEF_STR( On ) )
	PORT_DIPNAME( 0x0800, 0x0000, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x0000, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x0800, DEF_STR( On ) )
	PORT_DIPNAME( 0x1000, 0x0000, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x0000, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x1000, DEF_STR( On ) )
	PORT_DIPNAME( 0x2000, 0x0000, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x0000, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x2000, DEF_STR( On ) )
	PORT_DIPNAME( 0x4000, 0x0000, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x0000, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x4000, DEF_STR( On ) )
	PORT_DIPNAME( 0x8000, 0x0000, DEF_STR( Unknown ) )
	PORT_DIPSETTING(    0x0000, DEF_STR( Off ) )
	PORT_DIPSETTING(    0x8000, DEF_STR( On ) )

	PORT_START("KEY1")
	PORT_BIT( 0xff, IP_ACTIVE_HIGH, IPT_UNUSED ) //TODO: mahjong panel test & service buttons are presumably here

	PORT_START("KEY2")
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_UNUSED )
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_START1 )
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_MAHJONG_KAN )
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_MAHJONG_M )
	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_MAHJONG_I )
	PORT_BIT( 0x20, IP_ACTIVE_HIGH, IPT_MAHJONG_E )
	PORT_BIT( 0x40, IP_ACTIVE_HIGH, IPT_UNUSED )
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_MAHJONG_A )
	PORT_START("KEY3")
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_UNUSED )
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_UNUSED )
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_MAHJONG_REACH )
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_MAHJONG_N )
	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_MAHJONG_J )
	PORT_BIT( 0x20, IP_ACTIVE_HIGH, IPT_MAHJONG_F )
	PORT_BIT( 0x40, IP_ACTIVE_HIGH, IPT_UNUSED )
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_MAHJONG_B )
	PORT_START("KEY4")
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_UNUSED )
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_UNUSED )
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_MAHJONG_RON )
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_MAHJONG_CHI )
	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_MAHJONG_K )
	PORT_BIT( 0x20, IP_ACTIVE_HIGH, IPT_MAHJONG_G )
	PORT_BIT( 0x40, IP_ACTIVE_HIGH, IPT_UNUSED )
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_MAHJONG_C )
	PORT_START("KEY5")
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_UNUSED )
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_UNUSED )
	PORT_BIT( 0x04, IP_ACTIVE_HIGH, IPT_UNUSED )
	PORT_BIT( 0x08, IP_ACTIVE_HIGH, IPT_MAHJONG_PON )
	PORT_BIT( 0x10, IP_ACTIVE_HIGH, IPT_MAHJONG_L )
	PORT_BIT( 0x20, IP_ACTIVE_HIGH, IPT_MAHJONG_H )
	PORT_BIT( 0x40, IP_ACTIVE_HIGH, IPT_UNUSED )
	PORT_BIT( 0x80, IP_ACTIVE_HIGH, IPT_MAHJONG_D )
INPUT_PORTS_END

// Atomiswave - inputs are read as standard Dreamcast controllers.
// Controller bit patterns:
//
// SHOT3   (1<<0)
// SHOT2   (1<<1)
// SHOT1   (1<<2)
// START   (1<<3)
// UP      (1<<4)
// DOWN    (1<<5)
// LEFT    (1<<6)
// RIGHT   (1<<7)
// SHOT5   (1<<9)
// SHOT4   (1<<10)
// SERVICE (1<<13)
// TEST    (1<<14)

// 2 joysticks variant
static INPUT_PORTS_START( aw2c )
	PORT_START("P1:0")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT ) PORT_8WAY PORT_PLAYER(1)
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT ) PORT_8WAY PORT_PLAYER(1)
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN ) PORT_8WAY PORT_PLAYER(1)
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_JOYSTICK_UP ) PORT_8WAY PORT_PLAYER(1)
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_START1 )
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_PLAYER(1)
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_PLAYER(1)
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_PLAYER(1)
	PORT_START("P1:1")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_SERVICE_NO_TOGGLE( 0x40, IP_ACTIVE_LOW )
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_SERVICE1 )
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_PLAYER(1)
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_PLAYER(1)
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_PLAYER(1)

	PORT_START("P2:0")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT ) PORT_8WAY PORT_PLAYER(2)
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT ) PORT_8WAY PORT_PLAYER(2)
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN ) PORT_8WAY PORT_PLAYER(2)
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_JOYSTICK_UP ) PORT_8WAY PORT_PLAYER(2)
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_START2 )
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_PLAYER(2)
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_PLAYER(2)
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_BUTTON3 ) PORT_PLAYER(2)
	PORT_START("P2:1")
	PORT_BIT( 0x80, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x40, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_SERVICE2 )
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_UNKNOWN )
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON4 ) PORT_PLAYER(2)
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON5 ) PORT_PLAYER(2)
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_BUTTON6 ) PORT_PLAYER(2)

	PORT_START("COINS")
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_COIN1 )
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_COIN2 )

	PORT_INCLUDE( naomi_debug )
INPUT_PORTS_END

// Single-player wheel variant
static INPUT_PORTS_START( aw1w )
	PORT_START("P1:0")
	PORT_BIT( 0xf1, IP_ACTIVE_LOW, IPT_UNUSED )
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_START1 )
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_PLAYER(1)
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_BUTTON2 ) PORT_PLAYER(1)

	PORT_START("P1:1")
	PORT_SERVICE_NO_TOGGLE( 0x40, IP_ACTIVE_LOW )
	PORT_BIT( 0x20, IP_ACTIVE_LOW, IPT_SERVICE1 )
	PORT_BIT( 0x9f, IP_ACTIVE_LOW, IPT_UNUSED )

	PORT_START("P2:A0") /* steering */
	PORT_BIT( 0xff, 0x80, IPT_PADDLE ) PORT_SENSITIVITY(100) PORT_KEYDELTA(4)

	PORT_START("P2:A1")	/* gas pedal */
	PORT_BIT( 0xff, 0x00, IPT_PEDAL ) PORT_SENSITIVITY(100) PORT_KEYDELTA(20)

	PORT_START("P2:A2")	/* brake */
	PORT_BIT( 0xff, 0x00, IPT_PEDAL2 ) PORT_SENSITIVITY(100) PORT_KEYDELTA(40)

	PORT_START("P2:A3") /* steering */
	PORT_BIT( 0xff, 0x80, IPT_PADDLE ) PORT_SENSITIVITY(100) PORT_KEYDELTA(4)

	PORT_START("P2:A4")	/* gas pedal */
	PORT_BIT( 0xff, 0x00, IPT_PEDAL ) PORT_SENSITIVITY(100) PORT_KEYDELTA(20)

	PORT_START("P2:A5")	/* brake */
	PORT_BIT( 0xff, 0x00, IPT_PEDAL2 ) PORT_SENSITIVITY(100) PORT_KEYDELTA(40)

	PORT_START("COINS")
	PORT_BIT( 0x01, IP_ACTIVE_HIGH, IPT_COIN1 )
	PORT_BIT( 0x02, IP_ACTIVE_HIGH, IPT_COIN2 )

	PORT_INCLUDE( naomi_debug )
INPUT_PORTS_END

static MACHINE_RESET( naomi )
{
	dc_state *state = machine.driver_data<dc_state>();

	MACHINE_RESET_CALL(dc);
	aica_set_ram_base(machine.device("aica"), state->dc_sound_ram, 8*1024*1024);
}

/*
 * Common for Naomi 1, Naomi GD-Rom, Naomi 2, Atomiswave ...
 */

static MACHINE_CONFIG_START( naomi_aw_base, dc_state )
	/* basic machine hardware */
	MCFG_CPU_ADD("maincpu", SH4LE, CPU_CLOCK) // SH4!!!
	MCFG_CPU_CONFIG(sh4cpu_config)
	MCFG_CPU_PROGRAM_MAP(naomi_map)
	MCFG_CPU_IO_MAP(naomi_port)

	MCFG_CPU_ADD("soundcpu", ARM7, ((XTAL_33_8688MHz*2)/3)/8)	// AICA bus clock is 2/3rds * 33.8688.  ARM7 gets 1 bus cycle out of each 8.
	MCFG_CPU_PROGRAM_MAP(dc_audio_map)

	MCFG_MAPLE_DC_ADD( "maple_dc", "maincpu" )

	MCFG_MACHINE_START( dc )
	MCFG_MACHINE_RESET( naomi )

	MCFG_EEPROM_93C46_ADD("main_eeprom")
	MCFG_EEPROM_DEFAULT_VALUE(0)

	/* video hardware */
	MCFG_SCREEN_ADD("screen", RASTER)
	MCFG_SCREEN_REFRESH_RATE(60)
	MCFG_SCREEN_VBLANK_TIME(ATTOSECONDS_IN_USEC(2500) /* not accurate */)
	MCFG_SCREEN_SIZE(640, 480)
	MCFG_SCREEN_VISIBLE_AREA(0, 640-1, 0, 480-1)
	MCFG_SCREEN_UPDATE_STATIC(dc)

	MCFG_PALETTE_LENGTH(0x1000)

	MCFG_VIDEO_START(dc)

	MCFG_SPEAKER_STANDARD_STEREO("lspeaker", "rspeaker")
	MCFG_SOUND_ADD("aica", AICA, 0)
	MCFG_SOUND_CONFIG(aica_config)
	MCFG_SOUND_ROUTE(0, "lspeaker", 2.0)
	MCFG_SOUND_ROUTE(1, "rspeaker", 2.0)
MACHINE_CONFIG_END

static MACHINE_CONFIG_DERIVED( naomi_base, naomi_aw_base )
	MCFG_MIE_ADD("mie", 4000000, "maple_dc", 0, 0, 0, 0, "MIE:3", 0, "MIE:5", 0, 0) // Actual frequency unknown
	MCFG_SEGA_837_13551_DEVICE_ADD("837_13551", "mie", "TILT", "P1", "P2", "A0", "A1", "A2", "A3", "A4", "A5", "A6", "A7", "OUTPUT")
	MCFG_EEPROM_93C46_8BIT_ADD("mie_eeprom")

	MCFG_X76F100_ADD("naomibd_eeprom")
MACHINE_CONFIG_END

/*
 * Naomi 1, unprotected ROM sub-board
 */

static MACHINE_CONFIG_DERIVED( naomi, naomi_base )
	MCFG_NAOMI_ROM_BOARD_ADD("rom_board", "naomibd_eeprom", "maincpu", naomi_g1_irq)
MACHINE_CONFIG_END

/*
 * Naomi 1 GD-Rom
 */

static MACHINE_CONFIG_DERIVED( naomigd, naomi_base )
	MCFG_NAOMI_GDROM_BOARD_ADD("rom_board", ":gdrom", "pic", "naomibd_eeprom", "maincpu", naomi_g1_irq)
MACHINE_CONFIG_END

/*
 * Naomi 1, M1 sub-board
 */

static MACHINE_CONFIG_DERIVED( naomim1, naomi_base )
	MCFG_NAOMI_M1_BOARD_ADD("rom_board", "rom_key", "naomibd_eeprom", "maincpu", naomi_g1_irq)
MACHINE_CONFIG_END

/*
 * Naomi 1, M2/3 sub-board
 */

static MACHINE_CONFIG_DERIVED( naomim2, naomi_base )
	MCFG_NAOMI_M2_BOARD_ADD("rom_board", "rom_key", "naomibd_eeprom", "maincpu", naomi_g1_irq)
MACHINE_CONFIG_END

/*
 * Naomi 1, M4 sub-board
 */

static MACHINE_CONFIG_DERIVED( naomim4, naomi_base )
	MCFG_NAOMI_M4_BOARD_ADD("rom_board", "rom_key", "naomibd_eeprom", "maincpu", naomi_g1_irq)
MACHINE_CONFIG_END

/*
 * Naomi 2
 */

static MACHINE_CONFIG_DERIVED( naomi2, naomi )
	MCFG_CPU_MODIFY("maincpu")
	MCFG_CPU_PROGRAM_MAP(naomi2_map)
MACHINE_CONFIG_END

/*
 * Naomi 2 GD-Rom
 */

static MACHINE_CONFIG_DERIVED( naomi2gd, naomigd )
	MCFG_CPU_MODIFY("maincpu")
	MCFG_CPU_PROGRAM_MAP(naomi2_map)
MACHINE_CONFIG_END

/*
 * Naomi 2, M1 sub-board
 */

static MACHINE_CONFIG_DERIVED( naomi2m1, naomim1 )
	MCFG_CPU_MODIFY("maincpu")
	MCFG_CPU_PROGRAM_MAP(naomi2_map)
MACHINE_CONFIG_END

/*
 * Naomi 2, M2/3 sub-board
 */

static MACHINE_CONFIG_DERIVED( naomi2m2, naomim2 )
	MCFG_CPU_MODIFY("maincpu")
	MCFG_CPU_PROGRAM_MAP(naomi2_map)
MACHINE_CONFIG_END

/*
 * Atomiswave
 */

static MACHINE_CONFIG_DERIVED( aw_base, naomi_aw_base )
	MCFG_CPU_MODIFY("maincpu")
	MCFG_CPU_PROGRAM_MAP(aw_map)
	MCFG_MACRONIX_29L001MC_ADD("awflash")
	MCFG_AW_ROM_BOARD_ADD("rom_board", "rom_key", "maincpu", naomi_g1_irq)
MACHINE_CONFIG_END

static MACHINE_CONFIG_DERIVED( aw1c, aw_base )
	MCFG_DC_CONTROLLER_ADD("dcctrl0", "maple_dc", 0, "P1:0", "P1:1", "P1:A0", "P1:A1", "P1:A2", "P1:A3", "P1:A4", "P1:A5")
	MCFG_DC_CONTROLLER_ADD("dcctrl1", "maple_dc", 1, "P2:0", "P2:1", "P2:A0", "P2:A1", "P2:A2", "P2:A3", "P2:A4", "P2:A5")
MACHINE_CONFIG_END

static MACHINE_CONFIG_DERIVED( aw2c, aw_base )
	MCFG_DC_CONTROLLER_ADD("dcctrl0", "maple_dc", 0, "P1:0", "P1:1", "P1:A0", "P1:A1", "P1:A2", "P1:A3", "P1:A4", "P1:A5")
	MCFG_DC_CONTROLLER_ADD("dcctrl1", "maple_dc", 1, "P2:0", "P2:1", "P2:A0", "P2:A1", "P2:A2", "P2:A3", "P2:A4", "P2:A5")
MACHINE_CONFIG_END

#define ROM_LOAD16_WORD_SWAP_BIOS(bios,name,offset,length,hash) \
		ROMX_LOAD(name, offset, length, hash, ROM_GROUPWORD | ROM_BIOS(bios+1)) /* Note '+1' */

/* BIOS info:

Revisions A through C can handle game carts only
Revisions D and later can also handle GD-Rom board
Revisions F and later can also handle GD-Rom board and or the network GD-Rom board

F355 has it's own BIOS (3 screen version) and different JVS I/O Board

Info from roms starting at 0x1ffd60

EPR-21576h - NAOMI BOOT ROM 2002 07/08  1.8- (Japan)
EPR-21576g - NAOMI BOOT ROM 2001 09/10  1.70 (Japan)
EPR-21576e - NAOMI BOOT ROM 2000 08/25  1.50 (Japan)
EPR-21576d - NAOMI BOOT ROM 1999 06/04  1.40 (Japan)
EPR-21576b - NAOMI BOOT ROM 1999 02/15  1.20 (Japan)
EPR-21576a - NAOMI BOOT ROM 1999 01/14  1.10 (Japan)
EPR-21576  - NAOMI BOOT ROM 1998 12/18  1.00 (Japan)
EPR-21577g - NAOMI BOOT ROM 2001 09/10  1.70 (USA)
EPR-21577e - NAOMI BOOT ROM 2000 08/25  1.50 (USA)
EPR-21577d - NAOMI BOOT ROM 1999 06/04  1.40 (USA)
EPR-21577a - NAOMI BOOT ROM 1999 02/15  1.20 (USA)    <-- ROM was bad, but "A" was v1.20 and not v1.10 (not dumped)
EPR-21578h - NAOMI BOOT ROM 2002 07/08  1.8- (Export)
EPR-21578g - NAOMI BOOT ROM 2001 09/10  1.70 (Export)
EPR-21578e - NAOMI BOOT ROM 2000 08/25  1.50 (Export)
EPR-21578d - NAOMI BOOT ROM 1999 06/04  1.40 (Export)
EPR-21578b - NAOMI BOOT ROM 1999 02/15  1.20 (Export) <-- Also found as EPR-21578A
EPR-21579d - NAOMI BOOT ROM 1999 06/04  1.40 (Korea)
EPR-21579  - NAOMI BOOT ROM 1999 01/14  1.10 (Korea)
EPR-21580  - No known dump (Australia)

EPR-21577e & EPR-2178e differ by 7 bytes:

0x53e20 is the region byte (only one region byte)
0x1ffffa-0x1fffff is the BIOS checksum


House of the Dead 2 specific Naomi BIOS roms:

Info from roms starting at 0x1ff060

EPR-21330  - HOUSE OF THE DEAD 2 IPL ROM 1998 11/14 (USA)
EPR-21331  - HOUSE OF THE DEAD 2 IPL ROM 1998 11/14 (Export)

EPR-21330 & EPR-21331 differ by 7 bytes:

0x40000 is the region byte (only one region byte)
0x1ffffa-0x1fffff is the BIOS checksum


Ferrari F355 specific Naomi BIOS roms:

EPR-22850 - NAOMI BOOT ROM 1999 08/30  1.35 (USA)
EPR-22851 - NAOMI BOOT ROM 1999 08/30  1.35 (Export)

EPR-22850 & EPR-22851 differ by 7 bytes:

0x52F08 is the region byte (only one region byte)
0x1ffffa-0x1fffff is the BIOS checksum


Airline Pilot specific Naomi BIOS roms:

EPR-21801 - NAOMI BOOT ROM 1999 03/11  1.30 (USA)
EPR-21802 - NAOMI BOOT ROM 1999 03/11  1.30 (Export)

0x4D148 is the region byte (only one region byte)
0x1ffffa-0x1fffff is the BIOS checksum


Region byte encoding is as follows:

0x00 = Japan
0x01 = USA
0x02 = Export
0x03 = Korea
0x?? = Australia

Scan ROM for the text string "LOADING TEST MODE NOW" back up four (4) bytes for the region byte.
  NOTE: this doesn't work for the HOTD2 or multi screen boot roms

*/
// game specific bios roms quite clearly don't belong in here.
// Japan bios is default, because most games require it.
#define NAOMI_BIOS \
	ROM_REGION( 0x200000, "maincpu", 0) \
	ROM_SYSTEM_BIOS( 0, "bios0", "epr-21576h (Japan)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 0, "epr-21576h.ic27",  0x000000, 0x200000, CRC(d4895685) SHA1(91424d481ff99a8d3f4c45cea6d3f0eada049a6d) ) \
	ROM_SYSTEM_BIOS( 1, "bios1", "epr-21576g (Japan)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 1, "epr-21576g.ic27",  0x000000, 0x200000, CRC(d2a1c6bf) SHA1(6d27d71aec4dfba98f66316ae74a1426d567698a) ) \
	ROM_SYSTEM_BIOS( 2, "bios2", "epr-21576e (Japan)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 2, "epr-21576e.ic27",  0x000000, 0x200000, CRC(08c0add7) SHA1(e7c1a7673cb2ccb21748ef44105e46d1bad7266d) ) \
	ROM_SYSTEM_BIOS( 3, "bios3", "epr-21576d (Japan)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 3, "epr-21576d.ic27",  0x000000, 0x200000, CRC(3b2afa7b) SHA1(d007e1d321c198a38c5baff86eb2ab84385d150a) ) \
	ROM_SYSTEM_BIOS( 4, "bios4", "epr-21576b (Japan)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 4, "epr-21576b.ic27",  0x000000, 0x200000, CRC(755a6e07) SHA1(7e8b8ccfc063144d89668e7224dcd8a36c54f3b3) ) \
	ROM_SYSTEM_BIOS( 5, "bios5", "epr-21576a (Japan)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 5, "epr-21576a.ic27",  0x000000, 0x200000, CRC(cedfe439) SHA1(f27798bf3d890863ef0c1d9dcb4e7782249dca27) ) \
	ROM_SYSTEM_BIOS( 6, "bios6", "epr-21576 (Japan)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 6, "epr-21576.ic27", 0x000000, 0x200000, CRC(9dad3495) SHA1(5fb66f9a2b68d120f059c72758e65d34f461044a) ) \
	ROM_SYSTEM_BIOS( 7, "bios7", "epr-21578h (Export)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 7, "epr-21578h.ic27", 0x000000, 0x200000, CRC(7b452946) SHA1(8e9f153bbada24b37066dc45b64a7bf0d4f26a9b) ) \
	ROM_SYSTEM_BIOS( 8, "bios8", "epr-21578g (Export)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 8, "epr-21578g.ic27", 0x000000, 0x200000, CRC(55413214) SHA1(bd2748365a9fc1821c9369aa7155d7c41c4df43e) ) \
	ROM_SYSTEM_BIOS( 9, "bios9", "epr-21578e (Export)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 9, "epr-21578e.ic27",  0x000000, 0x200000, CRC(087f09a3) SHA1(0418eb2cf9766f0b1b874a4e92528779e22c0a4a) ) \
	ROM_SYSTEM_BIOS( 10, "bios10", "epr-21578d (Export)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 10, "epr-21578d.ic27",  0x000000, 0x200000, CRC(dfd5f42a) SHA1(614a0db4743a5e5a206190d6786ade24325afbfd) ) \
	ROM_SYSTEM_BIOS( 11, "bios11", "epr-21578b (Export)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 11, "epr-21578b.ic27",  0x000000, 0x200000, CRC(6c9aad83) SHA1(555918de76d8dbee2a97d8a95297ef694b3e803f) ) \
	ROM_SYSTEM_BIOS( 12, "bios12", "epr-21577g (USA)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 12, "epr-21577g.ic27",  0x000000, 0x200000, CRC(25f64af7) SHA1(99f9e6cc0642319bd2da492611220540add573e8) ) \
	ROM_SYSTEM_BIOS( 13, "bios13", "epr-21577e (USA)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 13, "epr-21577e.ic27",  0x000000, 0x200000, CRC(cf36e97b) SHA1(b085305982e7572e58b03a9d35f17ae319c3bbc6) ) \
	ROM_SYSTEM_BIOS( 14, "bios14", "epr-21577d (USA)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 14, "epr-21577d.ic27",  0x000000, 0x200000, CRC(60ddcbbe) SHA1(58b15096d269d6df617ca1810b66b47deb184958) ) \
	ROM_SYSTEM_BIOS( 15, "bios15", "epr-21579d (Korea)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 15, "epr-21579d.ic27",  0x000000, 0x200000, CRC(33513691) SHA1(b1d8c7c516e1471a788fcf7a02a794ad2f05aeeb) ) \
	ROM_SYSTEM_BIOS( 16, "bios16", "epr-21579 (Korea)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 16, "epr-21579.ic27",  0x000000, 0x200000, CRC(71f9c918) SHA1(d15af8b947f41eea7c203b565cd403e3f37a2017) ) \
	ROM_SYSTEM_BIOS( 17, "bios17", "Naomi Dev BIOS" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 17,  "dcnaodev.bios", 0x000000, 0x080000, CRC(7a50fab9) SHA1(ef79f448e0bf735d1264ad4f051d24178822110f) ) /* This one comes from a dev / beta board. The eprom was a 27C4096 */


// bios for House of the Dead 2
#define HOTD2_BIOS \
	ROM_REGION( 0x200000, "maincpu", 0) \
	ROM_SYSTEM_BIOS( 0, "bios0", "HOTD2 (Export)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 0,  "epr-21331.ic27", 0x000000, 0x200000, CRC(065f8500) SHA1(49a3881e8d76f952ef5e887200d77b4a415d47fe) ) \
	ROM_SYSTEM_BIOS( 1, "bios1", "HOTD2 (USA)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 1,  "epr-21330.ic27", 0x000000, 0x200000, CRC(9e3bfa1b) SHA1(b539d38c767b0551b8e7956c1ff795de8bbe2fbc) )

#define F355_BIOS \
	ROM_REGION( 0x200000, "maincpu", 0) \
	ROM_SYSTEM_BIOS( 0, "bios0", "Ferrari F355 (Export)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 0,  "epr-22851.ic27", 0x000000, 0x200000, CRC(62483677) SHA1(3e3bcacf5f972c376b569f45307ee7fd0b5031b7) ) \
	ROM_SYSTEM_BIOS( 1, "bios1", "Ferrari F355 (USA)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 1,  "epr-22850.ic27", 0x000000, 0x200000, CRC(28aa539d) SHA1(14485368656af80504b212da620179c49f84c1a2) )

#define AIRLINE_BIOS \
	ROM_REGION( 0x200000, "maincpu", 0) \
	ROM_SYSTEM_BIOS( 0, "bios0", "Airline Pilots Deluxe (Export)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 0,  "epr-21802.ic27", 0x000000, 0x200000, CRC(a77c6b1c) SHA1(bd50a6bb8fa9bac121b076e21ea048a83a240a48) ) \
	ROM_SYSTEM_BIOS( 1, "bios1", "Airline Pilots Deluxe (USA)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 1,  "epr-21801.ic27", 0x000000, 0x200000, CRC(a21bef24) SHA1(c6c6ed09772b63a9a84ef0678fc1b7527484038a) )


/* only revisions d and higher support the GDROM, and there is an additional bios (and SH4!) on the DIMM board for the CD Controller */
/* bios e works, the newer japan one gives 'board malfunction' with some games?' */
#define NAOMIGD_BIOS \
	ROM_REGION( 0x200000, "maincpu", 0) \
	ROM_SYSTEM_BIOS( 0, "bios0", "epr-21576e (Japan)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 0, "epr-21576e.bin",  0x000000, 0x200000, CRC(08c0add7) SHA1(e7c1a7673cb2ccb21748ef44105e46d1bad7266d) ) \
	ROM_SYSTEM_BIOS( 1, "bios1", "epr-21576g (Japan)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 1, "epr-21576g.bin",  0x000000, 0x200000, CRC(d2a1c6bf) SHA1(6d27d71aec4dfba98f66316ae74a1426d567698a) ) \
	ROM_SYSTEM_BIOS( 2, "bios2", "epr-21576h (Japan)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 2, "epr-21576h.bin",  0x000000, 0x200000, CRC(d4895685) SHA1(91424d481ff99a8d3f4c45cea6d3f0eada049a6d) ) \
	ROM_SYSTEM_BIOS( 3, "bios3", "epr-21576d (Japan)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 3, "epr-21576d.bin",  0x000000, 0x200000, CRC(3b2afa7b) SHA1(d007e1d321c198a38c5baff86eb2ab84385d150a) ) \
	ROM_SYSTEM_BIOS( 4, "bios4", "epr-21578h (Export)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 4, "epr-21578h.ic27", 0x000000, 0x200000, CRC(7b452946) SHA1(8e9f153bbada24b37066dc45b64a7bf0d4f26a9b) ) \
	ROM_SYSTEM_BIOS( 5, "bios5", "epr-21578g (Export)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 5, "epr-21578g.ic27", 0x000000, 0x200000, CRC(55413214) SHA1(bd2748365a9fc1821c9369aa7155d7c41c4df43e) ) \
	ROM_SYSTEM_BIOS( 6, "bios6", "epr-21578e (Export)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 6, "epr-21578e.bin",  0x000000, 0x200000, CRC(087f09a3) SHA1(0418eb2cf9766f0b1b874a4e92528779e22c0a4a) ) \
	ROM_SYSTEM_BIOS( 7, "bios7", "epr-21578d (Export)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 7, "epr-21578d.bin",  0x000000, 0x200000, CRC(dfd5f42a) SHA1(614a0db4743a5e5a206190d6786ade24325afbfd) ) \
	ROM_SYSTEM_BIOS( 8, "bios8", "epr-21577g (USA)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 8, "epr-21577g.bin",  0x000000, 0x200000, CRC(25f64af7) SHA1(99f9e6cc0642319bd2da492611220540add573e8) ) \
	ROM_SYSTEM_BIOS( 9, "bios9", "epr-21577e (USA)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 9, "epr-21577e.bin",  0x000000, 0x200000, CRC(cf36e97b) SHA1(b085305982e7572e58b03a9d35f17ae319c3bbc6) ) \
	ROM_SYSTEM_BIOS( 10, "bios10", "epr-21577d (USA)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 10, "epr-21577d.bin",  0x000000, 0x200000, CRC(60ddcbbe) SHA1(58b15096d269d6df617ca1810b66b47deb184958) ) \
	ROM_SYSTEM_BIOS( 11, "bios11", "epr-21579d (Korea)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 11, "epr-21579d.ic27",  0x000000, 0x200000, CRC(33513691) SHA1(b1d8c7c516e1471a788fcf7a02a794ad2f05aeeb) ) \
	ROM_REGION( 0x200000, "user2", 0) \
	ROM_LOAD16_WORD_SWAP( "fpr-23489c.ic14", 0x000000, 0x200000, CRC(bc38bea1) SHA1(b36fcc6902f397d9749e9d02de1bbb7a5e29d468) )

/* NAOMI2 BIOS:

EPR-23605C - NAOMI BOOT ROM 2002 07/08  1.8- (Japan)
EPR-23605B - NAOMI BOOT ROM 2001 09/10  1.70 (Japan)
EPR-23605A - NAOMI BOOT ROM 2001 06/20  1.60 (Japan)
EPR-23605  - NAOMI BOOT ROM 2001 01/19  1.50 (Japan)
EPR-23607B - NAOMI BOOT ROM 2001 09/10  1.70 (USA)
EPR-23607  - NAOMI BOOT ROM 2001 01/19  1.50 (USA)
EPR-23608C - NAOMI BOOT ROM 2002 07/08  1.8- (Export)
EPR-23608B - NAOMI BOOT ROM 2001 09/10  1.70 (Export)
EPR-23608A - NAOMI BOOT ROM 2001 06/20  1.60 (Export)
EPR-23608  - NAOMI BOOT ROM 2001 01/19  1.50 (Export)

EPR-23605B, EPR-23607B & EPR-23608B all differ by 8 bytes:

0x0553a0 is the first region byte
0x1ecf40 is a second region byte (value is the same as the first region byte )
0x1fffa-1ffff is the BIOS rom checksum

Region byte encoding is as follows:

0x00 = Japan
0x01 = USA
0x02 = Export
0x?? = Korea
0x?? = Australia

*/

#define NAOMI2_BIOS \
	ROM_REGION( 0x200000, "maincpu", 0) \
	ROM_SYSTEM_BIOS( 0, "bios0", "epr-23605c (Japan)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 0, "epr-23605c.bin",   0x000000, 0x200000, CRC(297ea6ed) SHA1(cfbfe57c80e6ee86a101fa83aec0a01e00c0f42a) ) \
	ROM_SYSTEM_BIOS( 1, "bios1", "epr-23605b (Japan)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 1, "epr-23605b.bin",   0x000000, 0x200000, CRC(3a3242d4) SHA1(aaca4df51ef91d926f8191d372f3dfe1d20d9484) ) \
	ROM_SYSTEM_BIOS( 2, "bios2", "epr-23605a (Japan)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 2, "epr-23605a.bin",   0x000000, 0x200000, CRC(7bc3fc2d) SHA1(a4a9531a7c66ff30046908cf71f6c7b6fb59c392) ) \
	ROM_SYSTEM_BIOS( 3, "bios3", "epr-23605 (Japan)"  ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 3, "epr-23605.bin",    0x000000, 0x200000, CRC(5731e446) SHA1(787b0844fc408cf124c12405c095c59948709ea6) ) \
	ROM_SYSTEM_BIOS( 4, "bios4", "epr-23608c (Export)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 4, "epr-23608c.bin",   0x000000, 0x200000, CRC(6ef1dd8e) SHA1(25ef957ec1c58fdaff5e89102002bca6c38832c5) ) \
	ROM_SYSTEM_BIOS( 5, "bios5", "epr-23608b (Export)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 5, "epr-23608b.bin",   0x000000, 0x200000, CRC(a554b1e3) SHA1(343b727a3619d1c75a9b6d4cc156a9050447f155) ) \
	ROM_SYSTEM_BIOS( 6, "bios6", "epr-23608a (Export)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 6, "epr-23608a.bin",   0x000000, 0x200000, CRC(e8f884d1) SHA1(28f4de747bb3cf860b9ebf897322fbc5d7c1e156) ) \
	ROM_SYSTEM_BIOS( 7, "bios7", "epr-23608 (Export)"  ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 8, "epr-23608.bin",    0x000000, 0x200000, CRC(929cc3a6) SHA1(47d00c818de23f733a4a33b1bbc72eb8aa729246) ) \
	ROM_SYSTEM_BIOS( 8, "bios8", "epr-23607b (USA)" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 8, "epr-23607b.bin",   0x000000, 0x200000, CRC(f308c5e9) SHA1(5470ab1cee6afecbd8ca8cf40f8fbe4ec2cb1471) ) \
	ROM_SYSTEM_BIOS( 9, "bios9", "epr-23607 (USA)"  ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 9, "epr-23607.bin",    0x000000, 0x200000, CRC(2b55add2) SHA1(547de5f97d3183c8cd069c4fa3c09f13d8b637d9) ) \

/* First half is BIOS, second half is game settings and is blanked/reprogrammed by the BIOS as necessary */
#define AW_BIOS \
	ROM_REGION( 0x200000, "awflash", 0) \
	ROM_SYSTEM_BIOS( 0, "bios0", "Atomiswave BIOS" ) \
	ROM_LOAD16_WORD_SWAP_BIOS( 0, "bios.ic23_l", 0x000000, 0x010000, BAD_DUMP CRC(e5693ce3) SHA1(1bde3ed87af64b0f675ebd47f12a53e1fc5709c1) ) /* Might be bad.. especially. bytes 0x0000, 0x6000, 0x8000 which gave different reads */ \

/* default EEPROM values, same works for all games */
#define NAOMI_DEFAULT_EEPROM \
	ROM_REGION16_BE( 0x80, "main_eeprom", 0 ) \
	ROM_LOAD16_WORD("main_eeprom.bin", 0x0000, 0x0080, CRC(fea29cbb) SHA1(4099f1747aafa07db34f6e072cd9bfaa83bae10e) ) \
	ROM_REGION( 0x84, "naomibd_eeprom", 0 ) \
	ROM_LOAD("x76f100_eeprom.bin", 0x0000, 0x0084, CRC(3ea24b6a) SHA1(3a730ebcf56e0060fef6b1b02eb2eb7cfb7e61dc) )

/* Version without the default x76f100 eeprom */
#define NAOMI_DEFAULT_EEPROM_NO_BD	\
	ROM_REGION16_BE( 0x80, "main_eeprom", 0 ) \
	ROM_LOAD16_WORD("main_eeprom.bin", 0x0000, 0x0080, CRC(fea29cbb) SHA1(4099f1747aafa07db34f6e072cd9bfaa83bae10e) )

ROM_START( naomi )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x8400000, "rom_board", ROMREGION_ERASE)
ROM_END

ROM_START( naomigd )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x8400000, "rom_board", ROMREGION_ERASE)
ROM_END

ROM_START( hod2bios )
	HOTD2_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x8400000, "rom_board", ROMREGION_ERASE)
ROM_END

ROM_START( f355bios )
	F355_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x8400000, "rom_board", ROMREGION_ERASE)
ROM_END

ROM_START( airlbios )
	AIRLINE_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x8400000, "rom_board", ROMREGION_ERASE)
ROM_END

ROM_START( naomi2 )
	NAOMI2_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x8400000, "rom_board", ROMREGION_ERASE)
ROM_END

ROM_START( awbios )
	AW_BIOS

	ROM_REGION( 0x8400000, "rom_board", ROMREGION_ERASE)
ROM_END


/**********************************************
 *
 * Naomi Cart ROM defines
 *
 *********************************************/


/* Info above each set is automatically generated from the IC22 rom and may not be accurate */

/*
SYSTEMID: NAOMI
JAP: GUN SPIKE
USA: CANNON SPIKE
EXP: CANNON SPIKE

NO.     Type    Byte    Word
IC22    32M     0000*   0000* invalid value
IC1     64M     7AC6    C534
IC2     64M     3959    6667
IC3     64M     F60D    69E5
IC4     64M     FBD4    AE40
IC5     64M     1717    F3EC
IC6     64M     A622    1D3D
C7     64M     33A3    4480
IC8     64M     FC26    A49D
IC9     64M     528D    5206
IC10    64M     7C94    8779
IC11    64M     271E    BEF7
IC12    64M     BA24    102F

*/

ROM_START( cspike )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x6800000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD("epr-23210.ic22", 0x0000000, 0x0400000, CRC(a15c54b5) SHA1(5c7872244d3d648e4c04751f120d0e9d47239921) )
	ROM_RELOAD( 0x400000, 0x400000)
	ROM_LOAD("mpr-23198.ic1",  0x0800000, 0x0800000, CRC(ce8d3edf) SHA1(1df5bb4eb440c221b8f1e5f019b02accc235fc28) )
	ROM_LOAD("mpr-23199.ic2",  0x1000000, 0x0800000, CRC(0979392a) SHA1(7dc433da6f3e47a721a2e86720a65d9752248e92) )
	ROM_LOAD("mpr-23200.ic3",  0x1800000, 0x0800000, CRC(e4b2db33) SHA1(063bc3789f68be5fcefeeec9e1c8268feb84b7eb) )
	ROM_LOAD("mpr-23201.ic4",  0x2000000, 0x0800000, CRC(c55ca0fa) SHA1(e6fde606b9ed4fd195da304a7b57e8b7797e368f) )
	ROM_LOAD("mpr-23202.ic5",  0x2800000, 0x0800000, CRC(983bb21c) SHA1(a30f9b09370cceadf11defc85b5acd3e578477e0) )
	ROM_LOAD("mpr-23203.ic6",  0x3000000, 0x0800000, CRC(f61b8d96) SHA1(a3522963b1e13b809818ffe5a209dd4ce087ec38) )
	ROM_LOAD("mpr-23204.ic7",  0x3800000, 0x0800000, CRC(03593ecd) SHA1(5ef3ccbfb7b1cc85ad352b13d70eefcad2b209f6) )
	ROM_LOAD("mpr-23205.ic8",  0x4000000, 0x0800000, CRC(e8c9349b) SHA1(310f02c5dad84e84362f0f674afa405f7d72f8ce) )
	ROM_LOAD("mpr-23206.ic9",  0x4800000, 0x0800000, CRC(8089d80f) SHA1(821f5f24616920bf0ed4c86597c27f6a3c39b8e6) )
	ROM_LOAD("mpr-23207.ic10", 0x5000000, 0x0800000, CRC(39f692a1) SHA1(14bc86b48a995378b4dd3609d38b90cddf2d7483) )
	ROM_LOAD("mpr-23208.ic11", 0x5800000, 0x0800000, CRC(b9494f4b) SHA1(2f35b25edf5210a82d4b67e639eeae11440d065a) )
	ROM_LOAD("mpr-23209.ic12s",0x6000000, 0x0800000, CRC(560188c0) SHA1(77f14c9a031c6e5414ffa854d20c40115361d715) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "cspike-key.bin", 0, 4, CRC(a3e9c6d0) SHA1(c098eae7aee716fbdca39f0eb1f55bd54d398394) )
ROM_END

/*

SYSTEMID: NAOMI
JAP: CAPCOM VS SNK  JAPAN
USA: CAPCOM VS SNK  USA
EXP: CAPCOM VS SNK  EXPORT

NO.     Type    Byte    Word
IC22    32M     0000    0000
IC1     64M     B836    4AA4
IC2     64M     19C1    9965
IC3     64M     B98C    EFB2
IC4     64M     2458    31CD
IC5     64M     59D2    E957
IC6     64M     1004    7E0B
IC7     64M     C63F    B2A7
IC8     64M     9D78    342F
IC9     64M     681F    D97A
IC10    64M     7544    E4D3
IC11    64M     8351    8A4C
IC12    64M     B713    2408
IC13    64M     A12E    8DE4

*/

// ver 000904
ROM_START( capsnk )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x7800000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD( "epr-23511c.ic22", 0x000000, 0x400000, CRC(3dbf8eb2) SHA1(1f7b89ba99e018cc85022fa852d56d4e345e1bd2) )
	ROM_LOAD( "mpr-23504.ic1", 0x0800000, 0x1000000, CRC(e01a31d2) SHA1(e00e138f6a20175c7aadb6500f6d7541b91def14) )
	ROM_LOAD( "mpr-23505.ic2", 0x1800000, 0x1000000, CRC(3a34d5fe) SHA1(f3c5f6fcbaa7004d371923eb412ea1fcf3fa461a) )
	ROM_LOAD( "mpr-23506.ic3", 0x2800000, 0x1000000, CRC(9cbab27d) SHA1(f166352355a03c9ccafbc15f926330b3622ec040) )
	ROM_LOAD( "mpr-23507.ic4", 0x3800000, 0x1000000, CRC(363c1734) SHA1(16b0485f1aacc8925b3c6d6152680139748e6df8) )
	ROM_LOAD( "mpr-23508.ic5", 0x4800000, 0x1000000, CRC(0a3590aa) SHA1(84c0e1853f069b003d09b268caee97e58c4dacb6) )
	ROM_LOAD( "mpr-23509.ic6", 0x5800000, 0x1000000, CRC(281d633d) SHA1(d773be8e95f7bf9212ee1061f3076220d4fce9e0) )
	ROM_LOAD( "mpr-23510.ic7", 0x6800000, 0x1000000, CRC(b856fef5) SHA1(0634f86740c438b40286256a0269570d24cb845a) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "capsnk-key.bin", 0, 4, CRC(2144df1c) SHA1(9069ca78e7450a285173431b3e52c5c25299e473) )
ROM_END

// ver 000804
ROM_START( capsnka )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x7800000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD( "epr23511a.ic22", 0x000000, 0x400000, CRC(fe00650f) SHA1(ca8e9e9178ed2b6598bdea83be1bf0dd7aa509f9) )
	ROM_LOAD( "mpr-23504.ic1", 0x0800000, 0x1000000, CRC(e01a31d2) SHA1(e00e138f6a20175c7aadb6500f6d7541b91def14) )
	ROM_LOAD( "mpr-23505.ic2", 0x1800000, 0x1000000, CRC(3a34d5fe) SHA1(f3c5f6fcbaa7004d371923eb412ea1fcf3fa461a) )
	ROM_LOAD( "mpr-23506.ic3", 0x2800000, 0x1000000, CRC(9cbab27d) SHA1(f166352355a03c9ccafbc15f926330b3622ec040) )
	ROM_LOAD( "mpr-23507.ic4", 0x3800000, 0x1000000, CRC(363c1734) SHA1(16b0485f1aacc8925b3c6d6152680139748e6df8) )
	ROM_LOAD( "mpr-23508.ic5", 0x4800000, 0x1000000, CRC(0a3590aa) SHA1(84c0e1853f069b003d09b268caee97e58c4dacb6) )
	ROM_LOAD( "mpr-23509.ic6", 0x5800000, 0x1000000, CRC(281d633d) SHA1(d773be8e95f7bf9212ee1061f3076220d4fce9e0) )
	ROM_LOAD( "mpr-23510.ic7", 0x6800000, 0x1000000, CRC(b856fef5) SHA1(0634f86740c438b40286256a0269570d24cb845a) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "capsnka-key.bin", 0, 4, CRC(2144df1c) SHA1(9069ca78e7450a285173431b3e52c5c25299e473) )
ROM_END

/*

SYSTEMID: NAOMI
JAP: COSMIC SMASH IN JAPAN
USA: COSMIC SMASH IN USA
EXP: COSMIC SMASH IN EXPORT

NO.     Type    Byte    Word
IC22    32M     0000    0000     EPR23428A.22
IC1     64M     C82B    E769     MPR23420.1
IC2     64M     E0C3    43B6     MPR23421.2
IC3     64M     C896    F766     MPR23422.3
IC4     64M     2E60    4CBF     MPR23423.4
IC5     64M     BB81    7E26     MPR23424.5
IC6     64M     B3A8    F2EA     MPR23425.6
IC7     64M     05C5    A084     MPR23426.7
?IC8     64M     9E13    7535     MPR23427.8

Serial: BCHE-01A0803

*/

ROM_START( csmash )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x4800000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD("epr-23428a.ic22", 0x0000000, 0x400000, CRC(d628dbce) SHA1(91ec1296ead572a64c37f8ac2c1a96742f19d50b) )
	ROM_RELOAD( 0x400000, 0x400000)
	ROM_LOAD("mpr-23420.ic1",   0x0800000, 0x0800000, CRC(9d5991f2) SHA1(c75871db314b01935d1daaacf1a762e73e5fd411) )
	ROM_LOAD("mpr-23421.ic2",   0x1000000, 0x0800000, CRC(6c351db3) SHA1(cdd601321a38fc34152517abdc473b73a4c6f630) )
	ROM_LOAD("mpr-23422.ic3",   0x1800000, 0x0800000, CRC(a1d4bd29) SHA1(6c446fd1819f55412351f15cf57b769c0c56c1db) )
	ROM_LOAD("mpr-23423.ic4",   0x2000000, 0x0800000, CRC(08cbf373) SHA1(0d9a593f5cc5d632d85d7253c135eef2e8e01598) )
	ROM_LOAD("mpr-23424.ic5",   0x2800000, 0x0800000, CRC(f4404000) SHA1(e49d941e47e63bb7f3fddc3c3d2c1653611914ee) )
	ROM_LOAD("mpr-23425.ic6",   0x3000000, 0x0800000, CRC(47f51da2) SHA1(af5ecd460114caed3a00157ffd3a2df0fbf348c0) )
	ROM_LOAD("mpr-23426.ic7",   0x3800000, 0x0800000, CRC(7f91b13f) SHA1(2d534f77291ebfedc011bf0e803a1b9243fb477f) )
	ROM_LOAD("mpr-23427.ic8",   0x4000000, 0x0800000, CRC(5851d525) SHA1(1cb1073542d75a3bcc0d363ed31d49bcaf1fd494) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "csmash-key.bin", 0, 4, CRC(21b1cd6f) SHA1(a74eef5e3475cc6952f4ffe31579769a854b62a9) )
ROM_END

ROM_START( csmasho )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x4800000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD("epr-23428.ic22", 0x0000000, 0x400000, CRC(f8597496) SHA1(2bb9f25b63b7410934ae4b1e052e1308a5c5a57f) )
	ROM_RELOAD( 0x400000, 0x400000)
	ROM_LOAD("mpr-23420.ic1", 0x0800000, 0x0800000, CRC(9d5991f2) SHA1(c75871db314b01935d1daaacf1a762e73e5fd411) )
	ROM_LOAD("mpr-23421.ic2", 0x1000000, 0x0800000, CRC(6c351db3) SHA1(cdd601321a38fc34152517abdc473b73a4c6f630) )
	ROM_LOAD("mpr-23422.ic3", 0x1800000, 0x0800000, CRC(a1d4bd29) SHA1(6c446fd1819f55412351f15cf57b769c0c56c1db) )
	ROM_LOAD("mpr-23423.ic4", 0x2000000, 0x0800000, CRC(08cbf373) SHA1(0d9a593f5cc5d632d85d7253c135eef2e8e01598) )
	ROM_LOAD("mpr-23424.ic5", 0x2800000, 0x0800000, CRC(f4404000) SHA1(e49d941e47e63bb7f3fddc3c3d2c1653611914ee) )
	ROM_LOAD("mpr-23425.ic6", 0x3000000, 0x0800000, CRC(47f51da2) SHA1(af5ecd460114caed3a00157ffd3a2df0fbf348c0) )
	ROM_LOAD("mpr-23426.ic7", 0x3800000, 0x0800000, CRC(7f91b13f) SHA1(2d534f77291ebfedc011bf0e803a1b9243fb477f) )
	ROM_LOAD("mpr-23427.ic8", 0x4000000, 0x0800000, CRC(5851d525) SHA1(1cb1073542d75a3bcc0d363ed31d49bcaf1fd494) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "csmasho-key.bin", 0, 4, CRC(21b1cd6f) SHA1(a74eef5e3475cc6952f4ffe31579769a854b62a9) )
ROM_END

/*

SYSTEMID: NAOMI
JAP: DEATH CRIMSON OX
USA: DEATH CRIMSON OX
EXP: DEATH CRIMSON OX

*/

ROM_START( deathcox )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x5800000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD("epr-23524.ic22",0x0000000, 0x0400000, CRC(edc20e44) SHA1(6167ee86624f5b78b3ced0dd82259e83053f4f9d) )
	ROM_RELOAD( 0x400000, 0x400000)
	ROM_LOAD("mpr-23514.ic1", 0x0800000, 0x0800000, CRC(1f2b090e) SHA1(f2863d306512112cd3025c9ce3300ac0a396ee2d) )
	ROM_LOAD("mpr-23515.ic2", 0x1000000, 0x0800000, CRC(dc8557eb) SHA1(855bf4a8a7a7184a64a60d30efd505eb1181d8c6) )
	ROM_LOAD("mpr-23516.ic3", 0x1800000, 0x0800000, CRC(94494cbb) SHA1(fc977c77fa424541573c5cac28dac013d3354754) )
	ROM_LOAD("mpr-23517.ic4", 0x2000000, 0x0800000, CRC(69ba6a41) SHA1(1d5528f7d3f8721492db966ec041966192bebdf8) )
	ROM_LOAD("mpr-23518.ic5", 0x2800000, 0x0800000, CRC(49882766) SHA1(f6a7a7039dc251e02d69d4c95130102dfbb25fc9) )
	ROM_LOAD("mpr-23519.ic6", 0x3000000, 0x0800000, CRC(cdc82805) SHA1(947cdcdc16fc61ba4ca1258d170483b3decdacf2) )
	ROM_LOAD("mpr-23520.ic7", 0x3800000, 0x0800000, CRC(1a268360) SHA1(b35dab00e4e656f13fcad92bebd2c256c1965f54) )
	ROM_LOAD("mpr-23521.ic8", 0x4000000, 0x0800000, CRC(cf8674b8) SHA1(bdd2a0ef98138021707b3dd06b1d9855308ed3ec) )
	ROM_LOAD("mpr-23522.ic9", 0x4800000, 0x0800000, CRC(7ae6716e) SHA1(658b794ae6e3898885524582a207faa1076a65ca) )
	ROM_LOAD("mpr-23523.ic10",0x5000000, 0x0800000, CRC(c91efb67) SHA1(3d79870551310da7a641858ffec3840714e9cc22) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "deathcox-key.bin", 0, 4, CRC(aa534c8a) SHA1(fcc45e477c646b51afe7c4168d7246964a1f23b5) )
ROM_END

/*

SYSTEMID: NAOMI
JAP: DEAD OR ALIVE 2
USA: DEAD OR ALIVE 2 USA ------
EXP: DEAD OR ALIVE 2 EXPORT----

NO.     Type    Byte    Word
IC22    32M     2B49    A054
IC1     64M     B74A    1815
IC2     64M     6B34    AB5A
IC3     64M     7EEF    EA1F
IC4     64M     0700    8C2F
IC5     64M     E365    B9CC
IC6     64M     7FE0    DC66
IC7     64M     BF8D    439B
IC8     64M     84DC    2F86
IC9     64M     15CF    8961
IC10    64M     7776    B985
IC11    64M     BCE9    21E9
IC12    64M     87FA    E9C0
IC13    64M     B82E    47A7
IC14    64M     3821    846E
IC15    64M     B491    C66E
IC16    64M     5774    918D
IC17    64M     219B    A171
IC18    64M     4848    643A
IC19    64M     6E1F    2570
IC20    64M     0CED    F2A8
IC21    64M     002C    8ECA

*/

ROM_START( doa2 )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM_NO_BD

	ROM_REGION( 0xb000000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD("epr-22121.ic22",0x0000000, 0x0400000,  CRC(30f93b5e) SHA1(0e33383e7ab9a721dab4708b063598f2e9c9f2e7) ) // partially encrypted

	ROM_LOAD("mpr-22100.ic1", 0x0800000, 0x0800000, CRC(92a53e5e) SHA1(87fcdeee9c4e65a3eb6eb345eed85d4f2df26c3c) )
	ROM_LOAD("mpr-22101.ic2", 0x1000000, 0x0800000, CRC(14cd7dce) SHA1(5df14a5dad14bc922b4f88881dc2e9c8e74d6170) )
	ROM_LOAD("mpr-22102.ic3", 0x1800000, 0x0800000, CRC(34e778b1) SHA1(750ddf5cda9622dd2b0f7069d247ffd55911c38f) )
	ROM_LOAD("mpr-22103.ic4", 0x2000000, 0x0800000, CRC(6f3db8df) SHA1(e9bbcf7897594ae47a9e3c8641ccb2c09b0809fe) )
	ROM_LOAD("mpr-22104.ic5", 0x2800000, 0x0800000, CRC(fcc2787f) SHA1(c28eaf91fa64e49e2276702678a4f8f17e09c3b9) )
	ROM_LOAD("mpr-22105.ic6", 0x3000000, 0x0800000, CRC(3e2da942) SHA1(d8f28c40ab59fa96a1fb19ad3adbee687088a5ab) )
	ROM_LOAD("mpr-22106.ic7", 0x3800000, 0x0800000, CRC(03aceaaf) SHA1(977e5b660254e7c5fdbd9d52c1f00c8a174a5d7b) )
	ROM_LOAD("mpr-22107.ic8", 0x4000000, 0x0800000, CRC(6f1705e4) SHA1(b8215dd4ef7214e75c2ec79ad974a32422c17647) )
	ROM_LOAD("mpr-22108.ic9", 0x4800000, 0x0800000, CRC(d34d3d8a) SHA1(910f1e4d8a54a621d9212e1425152c3029c96234) )
	ROM_LOAD("mpr-22109.ic10",0x5000000, 0x0800000, CRC(00ef44dd) SHA1(3fd100007daf59693de2329df1b4981dcdf435cd) )
	ROM_LOAD("mpr-22110.ic11",0x5800000, 0x0800000, CRC(a193b577) SHA1(3513853f88c491905481dadc5ce00cc5819b2663) )
	ROM_LOAD("mpr-22111.ic12",0x6000000, 0x0800000, CRC(55dddebf) SHA1(a7b8702cf578f5be4dcf8e2eaf11bf8b71d1b4ad) )
	ROM_LOAD("mpr-22112.ic13",0x6800000, 0x0800000, CRC(c5ffe564) SHA1(efe4d0cb5a536b26489c6dd31b1e446a9be643c9) )
	ROM_LOAD("mpr-22113.ic14",0x7000000, 0x0800000, CRC(12e7adf0) SHA1(2755c3efc6ca6d5680ead1489f42798c0187c5a4) )
	ROM_LOAD("mpr-22114.ic15",0x7800000, 0x0800000, CRC(d181d0a0) SHA1(2a0e46dbb31f5c11b6ae2fc8c786192bf3701ec5) )
	ROM_LOAD("mpr-22115.ic16",0x8000000, 0x0800000, CRC(ee2c842d) SHA1(8e33f241300481bb8875bda37e3917be71ed2594) )
	ROM_LOAD("mpr-22116.ic17",0x8800000, 0x0800000, CRC(224ab770) SHA1(85d849ee077e36da1df759caa4a32525395f741c) )
	ROM_LOAD("mpr-22117.ic18",0x9000000, 0x0800000, CRC(884a45a9) SHA1(d947cb3a045c5463523355fa631d55148e12c31e) )
	ROM_LOAD("mpr-22118.ic19",0x9800000, 0x0800000, CRC(8d631cbf) SHA1(fe8a65d35b1cdaed650ddde931e59f0768ffff53) )
	ROM_LOAD("mpr-22119.ic20",0xa000000, 0x0800000, CRC(d608fa86) SHA1(54c8107cccec8cbb536f13cda5b220b7972190b7) )
	ROM_LOAD("mpr-22120.ic21",0xa800000, 0x0800000, CRC(a30facb4) SHA1(70415ca34095c795297486bce1f956f6a8d4817f) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "doa2-key.bin", 0, 4, CRC(43f97c96) SHA1(7d1e6fcca3ca936f33d59d57834c3e4ff74f587d) )

	// on-cart X76F100 eeprom contents
	ROM_REGION( 0x84, "naomibd_eeprom", 0 )
	ROM_LOAD( "841-0003.sf",  0x000000, 0x000084, CRC(3a119a17) SHA1(d37a092cca7c9cfc5f2637b355af90a65d04013e) )
ROM_END

/*

SYSTEMID: NAOMI
JAP: DEAD OR ALIVE 2
USA: DEAD OR ALIVE 2 USA ------
EXP: DEAD OR ALIVE 2 EXPORT----

NO.     Type    Byte    Word
IC22    32M     2B49    A054
IC1     64M     B74A    1815
IC2     64M     6B34    AB5A
IC3     64M     7EEF    EA1F
IC4     64M     0700    8C2F
IC5     64M     E365    B9CC
IC6     64M     7FE0    DC66
IC7     64M     BF8D    439B
IC8     64M     84DC    2F86
IC9     64M     15CF    8961
IC10    64M     7776    B985
IC11    64M     BCE9    21E9
IC12    64M     87FA    E9C0
IC13    64M     B82E    47A7
IC14    64M     3821    846E
IC15    64M     B491    C66E
IC16    64M     5774    918D
IC17    64M     219B    A171
IC18    64M     4848    643A
IC19    64M     6E1F    2570
IC20    64M     0CED    F2A8
IC21    64M     002C    8ECA

Serial: BALH-13A0175

*/

ROM_START( doa2m )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM_NO_BD

	ROM_REGION( 0xb000000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD("doa2verm.ic22", 0x0000000, 0x0400000,  CRC(94b16f08) SHA1(225cd3e5dd5f21facf0a1d5e66fa17db8497573d) )

	ROM_LOAD("mpr-22100.ic1", 0x0800000, 0x0800000, CRC(92a53e5e) SHA1(87fcdeee9c4e65a3eb6eb345eed85d4f2df26c3c) )
	ROM_LOAD("mpr-22101.ic2", 0x1000000, 0x0800000, CRC(14cd7dce) SHA1(5df14a5dad14bc922b4f88881dc2e9c8e74d6170) )
	ROM_LOAD("mpr-22102.ic3", 0x1800000, 0x0800000, CRC(34e778b1) SHA1(750ddf5cda9622dd2b0f7069d247ffd55911c38f) )
	ROM_LOAD("mpr-22103.ic4", 0x2000000, 0x0800000, CRC(6f3db8df) SHA1(e9bbcf7897594ae47a9e3c8641ccb2c09b0809fe) )
	ROM_LOAD("mpr-22104.ic5", 0x2800000, 0x0800000, CRC(fcc2787f) SHA1(c28eaf91fa64e49e2276702678a4f8f17e09c3b9) )
	ROM_LOAD("mpr-22105.ic6", 0x3000000, 0x0800000, CRC(3e2da942) SHA1(d8f28c40ab59fa96a1fb19ad3adbee687088a5ab) )
	ROM_LOAD("mpr-22106.ic7", 0x3800000, 0x0800000, CRC(03aceaaf) SHA1(977e5b660254e7c5fdbd9d52c1f00c8a174a5d7b) )
	ROM_LOAD("mpr-22107.ic8", 0x4000000, 0x0800000, CRC(6f1705e4) SHA1(b8215dd4ef7214e75c2ec79ad974a32422c17647) )
	ROM_LOAD("mpr-22108.ic9", 0x4800000, 0x0800000, CRC(d34d3d8a) SHA1(910f1e4d8a54a621d9212e1425152c3029c96234) )
	ROM_LOAD("mpr-22109.ic10",0x5000000, 0x0800000, CRC(00ef44dd) SHA1(3fd100007daf59693de2329df1b4981dcdf435cd) )
	ROM_LOAD("mpr-22110.ic11",0x5800000, 0x0800000, CRC(a193b577) SHA1(3513853f88c491905481dadc5ce00cc5819b2663) )
	ROM_LOAD("mpr-22111.ic12",0x6000000, 0x0800000, CRC(55dddebf) SHA1(a7b8702cf578f5be4dcf8e2eaf11bf8b71d1b4ad) )
	ROM_LOAD("mpr-22112.ic13",0x6800000, 0x0800000, CRC(c5ffe564) SHA1(efe4d0cb5a536b26489c6dd31b1e446a9be643c9) )
	ROM_LOAD("mpr-22113.ic14",0x7000000, 0x0800000, CRC(12e7adf0) SHA1(2755c3efc6ca6d5680ead1489f42798c0187c5a4) )
	ROM_LOAD("mpr-22114.ic15",0x7800000, 0x0800000, CRC(d181d0a0) SHA1(2a0e46dbb31f5c11b6ae2fc8c786192bf3701ec5) )
	ROM_LOAD("mpr-22115.ic16",0x8000000, 0x0800000, CRC(ee2c842d) SHA1(8e33f241300481bb8875bda37e3917be71ed2594) )
	ROM_LOAD("mpr-22116.ic17",0x8800000, 0x0800000, CRC(224ab770) SHA1(85d849ee077e36da1df759caa4a32525395f741c) )
	ROM_LOAD("mpr-22117.ic18",0x9000000, 0x0800000, CRC(884a45a9) SHA1(d947cb3a045c5463523355fa631d55148e12c31e) )
	ROM_LOAD("mpr-22118.ic19",0x9800000, 0x0800000, CRC(8d631cbf) SHA1(fe8a65d35b1cdaed650ddde931e59f0768ffff53) )
	ROM_LOAD("mpr-22119.ic20",0xa000000, 0x0800000, CRC(d608fa86) SHA1(54c8107cccec8cbb536f13cda5b220b7972190b7) )
	ROM_LOAD("mpr-22120.ic21",0xa800000, 0x0800000, CRC(a30facb4) SHA1(70415ca34095c795297486bce1f956f6a8d4817f) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "doa2m-key.bin", 0, 4, CRC(43f97c96) SHA1(7d1e6fcca3ca936f33d59d57834c3e4ff74f587d) )

	// on-cart X76F100 eeprom contents
	ROM_REGION( 0x84, "naomibd_eeprom", 0 )
	ROM_LOAD( "841-0003.sf",  0x000000, 0x000084, CRC(3a119a17) SHA1(d37a092cca7c9cfc5f2637b355af90a65d04013e) )
ROM_END

/*

SYSTEMID: NAOMI
JAP:  DERBY OWNERS CLUB ------------
USA:  DERBY OWNERS CLUB ------------
EXP:  DERBY OWNERS CLUB IN EXPORT --

NO.     Type    Byte    Word
IC22    32M     0000    0000
IC1     64M     8AF3    D0BC
IC2     64M     1E79    0410
IC3     64M     146D    C51E
IC4     64M     E9AD    86BE
IC5     64M     BBB2    8685
IC6     64M     A0E1    C2E0
IC7     64M     B8CF    67B5
IC8     64M     005E    C1D6
IC9     64M     1F53    9304
IC10    64M     FAC9    8AA4
IC11    64M     B6B1    5665
IC12    64M     21DB    74F5
IC13    64M     A991    A8AB
IC14    64M     05BD    428D

Serial: BAXE-02A1386

*/

ROM_START( derbyoc )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x7800000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD("epr-22099b.ic22", 0x0000000, 0x0400000, CRC(5e708879) SHA1(fada4f4bf29fc8f77f354167f8db4f904610fe1a) )
	ROM_LOAD("mpr-22085.ic1",   0x0800000, 0x0800000, CRC(fffe9cc5) SHA1(ce6082fc648718b3831f709ba8b6212946c72d70) )
	ROM_LOAD("mpr-22086.ic2",   0x1000000, 0x0800000, CRC(610fe214) SHA1(c982d9e4722c2b6cb87f2bc3e2ac0f764f0bae79) )
	ROM_LOAD("mpr-22087.ic3",   0x1800000, 0x0800000, CRC(f0cd2a26) SHA1(21ff7d6540cfeb5e563d3528bd4bb31c5f285f1a) )
	ROM_LOAD("mpr-22088.ic4",   0x2000000, 0x0800000, CRC(62a7e6db) SHA1(103e2413c9706a5a98f05646fd3a7d7808593ad8) )
	ROM_LOAD("mpr-22089.ic5",   0x2800000, 0x0800000, CRC(cb135eb6) SHA1(a49df8fbae1ea0fb1251d0d8f302cc8687c3be0b) )
	ROM_LOAD("mpr-22090.ic6",   0x3000000, 0x0800000, CRC(13e44d57) SHA1(450fe281d34c088e61a4c2ee6ae434f330deb482) )
	ROM_LOAD("mpr-22091.ic7",   0x3800000, 0x0800000, CRC(efa1e2fc) SHA1(058635bee7a87b8191127060c6a28c053001b466) )
	ROM_LOAD("mpr-22092.ic8",   0x4000000, 0x0800000, CRC(de1ea163) SHA1(f2b0169fac3e1074628dec75642e7c41c8160964) )
	ROM_LOAD("mpr-22093.ic9",   0x4800000, 0x0800000, CRC(ecbc523b) SHA1(952618a0966838f5b814ff1265c899481aae1ba9) )
	ROM_LOAD("mpr-22094.ic10",  0x5000000, 0x0800000, CRC(72af7a70) SHA1(b1437dbf47f95bbdb9fe7a215c5a3b0f3839d917) )
	ROM_LOAD("mpr-22095.ic11",  0x5800000, 0x0800000, CRC(ae74b61a) SHA1(1c9de865447c9993d7faff2e61837e4b74353c3a) )
	ROM_LOAD("mpr-22096.ic12",  0x6000000, 0x0800000, CRC(d8c41648) SHA1(d465f4b841164da0738336e203c5bc6e1e799a76) )
	ROM_LOAD("mpr-22097.ic13",  0x6800000, 0x0800000, CRC(f1dedac5) SHA1(9d4499cbafe80dd0b36be617de7994a96e1e9a01) )
	ROM_LOAD("mpr-22098.ic14",  0x7000000, 0x0800000, CRC(f9824d2e) SHA1(f20f8cc2b1bef9077ede1cb874da8f2a335d39de) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "derbyoc-key.bin", 0, 4, CRC(bc568260) SHA1(6dc4923a730bc0380490519c82f1443226365c3a) )
ROM_END

ROM_START( derbyocw )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x7800000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD( "epr-22336d.ic22",0x000000, 0x0400000, CRC(e6c0cb0c) SHA1(b8c7fe62cb370793fd5ed0af27c18d36e9c0ce54) )
	ROM_LOAD( "mpr-22328.ic1", 0x0800000, 0x1000000, CRC(179cec02) SHA1(cbaba86082370a082d2e9f18427691d5cfa0e4f0) )
	ROM_LOAD( "mpr-22329.ic2", 0x1800000, 0x1000000, CRC(e0d5b98c) SHA1(5bf1ac0d895fd7725d170a54f01cd717a5e54110) )
	ROM_LOAD( "mpr-22330.ic3", 0x2800000, 0x1000000, CRC(6737cd62) SHA1(08429ca39fef3b36cb491813ddcd0feef3b24372) )
	ROM_LOAD( "mpr-22331.ic4", 0x3800000, 0x1000000, CRC(8fb5cbcf) SHA1(c4e6cbbe7d3549e1841654b41b1946b9bc356e74) )
	ROM_LOAD( "mpr-22332.ic5", 0x4800000, 0x1000000, CRC(c5e365a8) SHA1(5839ea24b8fd02552931175d35e2ceca75fca089) )
	ROM_LOAD( "mpr-22337.ic6", 0x5800000, 0x1000000, CRC(87ca3a2f) SHA1(2ffc01597107eb60dfa7aa49d51f203b51a44334) )
	ROM_LOAD( "mpr-22338.ic7", 0x6800000, 0x1000000, CRC(4bda7303) SHA1(db27d91ef811d741cfdb5c0196e61be722c2f5bd) )
ROM_END

ROM_START( drbyocwc )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x7800000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD( "epr-22336c.ic22", 0x000000, 0x400000, CRC(50053f82) SHA1(5b31abb480043ece3645b2479ba566ac6592acd8) )
	ROM_LOAD( "mpr-22328.ic1", 0x0800000, 0x1000000, CRC(179cec02) SHA1(cbaba86082370a082d2e9f18427691d5cfa0e4f0) )
	ROM_LOAD( "mpr-22329.ic2", 0x1800000, 0x1000000, CRC(e0d5b98c) SHA1(5bf1ac0d895fd7725d170a54f01cd717a5e54110) )
	ROM_LOAD( "mpr-22330.ic3", 0x2800000, 0x1000000, CRC(6737cd62) SHA1(08429ca39fef3b36cb491813ddcd0feef3b24372) )
	ROM_LOAD( "mpr-22331.ic4", 0x3800000, 0x1000000, CRC(8fb5cbcf) SHA1(c4e6cbbe7d3549e1841654b41b1946b9bc356e74) )
	ROM_LOAD( "mpr-22332.ic5", 0x4800000, 0x1000000, CRC(c5e365a8) SHA1(5839ea24b8fd02552931175d35e2ceca75fca089) )
	ROM_LOAD( "mpr-22333.ic6", 0x5800000, 0x1000000, CRC(96f324aa) SHA1(bc41e2097c1841771d786ba9ad1a31df1494a856) )
	ROM_LOAD( "mpr-22334.ic7", 0x6800000, 0x1000000, CRC(5389b05a) SHA1(e206e4d82d7b1a59c33043ec0812eb69be08d9b3) )
ROM_END

/*
SYSTEMID: NAOMI
JAP: DERBY OWNERS CLUB II-----------
USA: DERBY OWNERS CLUB II-----------
EXP: DERBY OWNERS CLUB II-IN EXPORT
*/

ROM_START( derbyoc2 )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0xb800000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD("epr-22306b.ic22",  0x0000000, 0x0400000, CRC(fcac20eb) SHA1(26cec9f615cd18ce7fccfc5e273e42c58dea1995) )
	ROM_LOAD( "mpr-22295.ic1",  0x0800000, 0x1000000, CRC(1085001b) SHA1(096ac0ecc7d5324f04514d24eb338de591926a3d) )
	ROM_LOAD( "mpr-22296.ic2",  0x1800000, 0x1000000, CRC(f18cb28b) SHA1(e7f253a35e15f62e178a1ff565f8a94102eea057) )
	ROM_LOAD( "mpr-22297.ic3",  0x2800000, 0x1000000, CRC(a78a527b) SHA1(7d4e7bd93519377a8676f7ca6f72c4d0de2a0475) )
	ROM_LOAD( "mpr-22298.ic4",  0x3800000, 0x1000000, CRC(cd76c7ea) SHA1(2db200c8853d1bc9da31da6c197c6b55e570dc69) )
	ROM_LOAD( "mpr-22299.ic5",  0x4800000, 0x1000000, CRC(ffb96da5) SHA1(bfe8f4a455d3eeec0fa9728fc1bb52cebd13b2a8) )
	ROM_LOAD( "mpr-22300.ic6",  0x5800000, 0x1000000, CRC(6fb05214) SHA1(9cececc46ac463bc5df3c3f5fb9268d5ced31837) )
	ROM_LOAD( "mpr-22301.ic7",  0x6800000, 0x1000000, CRC(52eb076d) SHA1(a79bb286594cc5d196a37a17147ad1770db5cd67) )
	ROM_LOAD( "mpr-22302.ic8",  0x7800000, 0x1000000, CRC(86767b0b) SHA1(54ed418cef78506fb824e3d16cadefdb684f2b34) )
	ROM_LOAD( "mpr-22303.ic9",  0x8800000, 0x1000000, CRC(73a80bd5) SHA1(d607f1d993af4b78d3609991e47e9540664380fd) )
	ROM_LOAD( "mpr-22304.ic10", 0x9800000, 0x1000000, CRC(46c1fb1f) SHA1(6daca76a75df3501f77e473eb065d48804fcc64a) )
	ROM_LOAD( "mpr-22305.ic11", 0xa800000, 0x1000000, CRC(027d0e7b) SHA1(e3c874e60cabb6f9ce686696d9055a0c0d5289ae) )
ROM_END

/*

SYSTEMID: NAOMI
JAP: DYNAMITE BASEBALL NAOMI
USA: SAMPLE GAME IN USA--------
EXP: SAMPLE GAME

NO.     Type    Byte    Word
IC22    16M     EF41    1DBC
IC1     64M     2743    8DE9
IC2     64M     1D2B    B4D5
IC3     64M     9127    8536
IC4     64M     946A    851B
IC5     64M     BDF4    AF2C
IC6     64M     78A2    DADB
IC7     64M     9816    06D3
IC8     64M     F8D9    9C38
IC9     64M     3C7D    532A
IC10    64M     37A2    D3F1
IC11    64M     5BF2    05FC
IC12    64M     694F    A25A
IC13    64M     685C    CDA8
IC14    64M     3DFA    32A9
IC15    64M     071F    820F
IC16    64M     1E89    D6B5
IC17    64M     889C    504B
IC18    64M     8B78    1BB5
IC19    64M     9816    7EE9
IC20    64M     E5C2    CECB
IC21    64M     5C65    8F82

Serial: ??? (sticker removed)

Protection notes (same code snippet seen in Zombie Revenge):
0C0A8148: 013C   MOV.B   @(R0,R3),R1
0C0A814A: 611C   EXTU.B  R1,R1
0C0A814C: 31C7   CMP/GT  R12,R1
0C0A814E: 1F11   MOV.L   R1,@($04,R15)
0C0A8150: 8F04   BFS     $0C0A815C
0C0A8152: E500   MOV     #$00,R5
0C0A8154: D023   MOV.L   @($008C,PC),R0 [0C0A81E4]
0C0A8156: 2052   MOV.L   R5,@R0
0C0A8158: AFFE   BRA     $0C0A8158
0C0A815A: 0009   NOP

*/

ROM_START( dybbnao )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0xb000000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD("epr-21575.ic22",0x0000000, 0x0200000, CRC(ba61e248) SHA1(3cce5d8b307038515d7da7ec567bfa2e3aafc274) )
	ROM_LOAD("mpr-21554.ic1", 0x0800000, 0x0800000, CRC(6eb29c37) SHA1(3548a93f9efa3bd548f9e30223a9b3570031f126) )
	ROM_LOAD("mpr-21555.ic2", 0x1000000, 0x0800000, CRC(3ff79959) SHA1(abd5407fcfa5556fc3f0c56892daad0c741a681f) )
	ROM_LOAD("mpr-21556.ic3", 0x1800000, 0x0800000, CRC(79bc8caf) SHA1(8cb77c66a86a99b85f2e3c8a5fed457f75598af4) )
	ROM_LOAD("mpr-21557.ic4", 0x2000000, 0x0800000, CRC(6f88e6fb) SHA1(7a7fdf910769d451a7cfc571811180433c353e8d) )
	ROM_LOAD("mpr-21558.ic5", 0x2800000, 0x0800000, CRC(6d4416cf) SHA1(e7ea9c0fe86e84c0358797664807056d8cfdcefe) )
	ROM_LOAD("mpr-21559.ic6", 0x3000000, 0x0800000, CRC(f4afbadf) SHA1(0d30b02835968e6044334204e5e8f8e88be6e783) )
	ROM_LOAD("mpr-21560.ic7", 0x3800000, 0x0800000, CRC(3b2e6e64) SHA1(0ad1daae658d53ca9ae9b197676eafacf820a0fe) )
	ROM_LOAD("mpr-21561.ic8", 0x4000000, 0x0800000, CRC(3c5136ea) SHA1(a0c8f4a947a6c729597a0a3d2348954d35eb5b11) )
	ROM_LOAD("mpr-21562.ic9", 0x4800000, 0x0800000, CRC(e158f4be) SHA1(37b8bcaaaede70c626cee891c53c0004b1cf23df) )
	ROM_LOAD("mpr-21563.ic10",0x5000000, 0x0800000, CRC(6b15befa) SHA1(9e0fd34a878d20b249b07bf01ce167f82f67de53) )
	ROM_LOAD("mpr-21564.ic11",0x5800000, 0x0800000, CRC(cecfaa8a) SHA1(fad935bf97a05e5991f4e0894e81c2c51f920db5) )
	ROM_LOAD("mpr-21565.ic12",0x6000000, 0x0800000, CRC(7e87d973) SHA1(66e2da9f721020e4a6aa423b3922b50b774b15f7) )
	ROM_LOAD("mpr-21566.ic13",0x6800000, 0x0800000, CRC(5354d553) SHA1(7dd84c30b0554b60598cc430366227be594b8221) )
	ROM_LOAD("mpr-21567.ic14",0x7000000, 0x0800000, CRC(9e17fdb2) SHA1(f709a2723bc028553f8c538a4b891333b70c4a62) )
	ROM_LOAD("mpr-21568.ic15",0x7800000, 0x0800000, CRC(b278efcd) SHA1(aa033eb7c5bfc76c847e0e79c3ac04f56edc5688) )
	ROM_LOAD("mpr-21569.ic16",0x8000000, 0x0800000, CRC(724e4d34) SHA1(4398fbc02e70c1ccd9869d18b345e2d790f6c314) )
	ROM_LOAD("mpr-21570.ic17",0x8800000, 0x0800000, CRC(b3375b2b) SHA1(e442d5359bab5581419408ecef796a48eee373ab) )
	ROM_LOAD("mpr-21571.ic18",0x9000000, 0x0800000, CRC(4bcefff9) SHA1(47437073756351b447cc939a2c99ebabe7a6436b) )
	ROM_LOAD("mpr-21572.ic19",0x9800000, 0x0800000, CRC(a47fd15e) SHA1(b595cadedc2e378219146ce19c0338f7e0dcc769) )
	ROM_LOAD("mpr-21573.ic20",0xa000000, 0x0800000, CRC(5d822e63) SHA1(8412980b288531c294d5cf9a6394aa0b9503d7df) )
	ROM_LOAD("mpr-21574.ic21",0xa800000, 0x0800000, CRC(d794a42c) SHA1(a79c7818c6ec993e718494b1d5407eb270a29abe) )
ROM_END

/*

SYSTEMID: NAOMI
JAP: DYNAMITE BASEBALL '99
USA: WORLD SERIES 99
EXP: WORLD SERIES 99

NO.     Type    Byte    Word
IC22    16M     0000    0000
IC1     64M     77B9    3C1B
IC2     64M     F7FB    025A
IC3     64M     B3D4    22C1
IC4     64M     060F    6279
IC5     64M     FE49    CAEB
IC6     64M     E34C    5FAD
IC7     64M     CC04    498C
IC8     64M     388C    DF17
IC9     64M     5B91    C458
IC10    64M     AF73    4A18
IC11    64M     2E5B    A198
IC12    64M     FFDB    41CA
IC13    64M     04E1    EA4C
IC14    64M     5B22    DA9A
IC15    64M     64E7    0873
IC16    64M     1EE7    BE11
IC17    64M     79C3    3608
IC18    64M     D4CE    5AEB
IC19    64M     E846    60B8

Serial: BBDE-01A0097

*/

ROM_START( dybb99 )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0xa000000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD("epr-22141b.ic22",0x0000000, 0x0200000, CRC(6d0e0785) SHA1(aa19e7bac4c266771d1e65cffa534a49d7566f51) )
	ROM_LOAD("mpr-22122.ic1",  0x0800000, 0x0800000, CRC(403da794) SHA1(321bc5b8966d05e60110bc0b17d0f37fe1facc56) )
	ROM_LOAD("mpr-22123.ic2",  0x1000000, 0x0800000, CRC(14cfeab8) SHA1(593d006bc2e6f4d1602d7045dc51d974fc5bbd4c) )
	ROM_LOAD("mpr-22124.ic3",  0x1800000, 0x0800000, CRC(19f89fce) SHA1(a442af4e3c913fd34257bc9af29e2361f98f2fa5) )
	ROM_LOAD("mpr-22125.ic4",  0x2000000, 0x0800000, CRC(a9e7298e) SHA1(287284a3d5ea230f3b17e9acb606f28498da230e) )
	ROM_LOAD("mpr-22126.ic5",  0x2800000, 0x0800000, CRC(9f6a5d94) SHA1(71849a4e0bf1bc033e7d073ecbf85793502384c4) )
	ROM_LOAD("mpr-22127.ic6",  0x3000000, 0x0800000, CRC(653d27e3) SHA1(f31c2f237f79cfcc0db657e1fb83503da65029d8) )
	ROM_LOAD("mpr-22128.ic7",  0x3800000, 0x0800000, CRC(e1fd22a1) SHA1(3d12f025ebf5323ce28508062dd2039d186b6223) )
	ROM_LOAD("mpr-22129.ic8",  0x4000000, 0x0800000, CRC(ecf90b4a) SHA1(0403ada8958c2aee56b236032359ae13267ed966) )
	ROM_LOAD("mpr-22130.ic9",  0x4800000, 0x0800000, CRC(26638b66) SHA1(915a8a9b6835b74f49594a02212a7da170c6a74b) )
	ROM_LOAD("mpr-22131.ic10", 0x5000000, 0x0800000, CRC(60e911f8) SHA1(035694e1382e3ca99d4b0cda1082a3a2bd84bcac) )
	ROM_LOAD("mpr-22132.ic11", 0x5800000, 0x0800000, CRC(093ee986) SHA1(e43743f8a93def9e56463bb99ef45a0de3b66d0f) )
	ROM_LOAD("mpr-22133.ic12", 0x6000000, 0x0800000, CRC(d4fc133d) SHA1(a04a21107c1d2dc6c52385e52627f6d97adc6934) )
	ROM_LOAD("mpr-22134.ic13", 0x6800000, 0x0800000, CRC(31497387) SHA1(a1c9626f2fe2d2c75e02513616865da87a140aa8) )
	ROM_LOAD("mpr-22135.ic14", 0x7000000, 0x0800000, CRC(42ab4b4f) SHA1(5f5ba43926ee24649d893e5087f68ef92f8ae88c) )
	ROM_LOAD("mpr-22136.ic15", 0x7800000, 0x0800000, CRC(1f313f03) SHA1(5e7b9d3935049473c128f24cb7718cb3385b03b7) )
	ROM_LOAD("mpr-22137.ic16", 0x8000000, 0x0800000, CRC(819e4cb2) SHA1(1f6a4382c6787d9453b49bca2ae2acab89710368) )
	ROM_LOAD("mpr-22138.ic17", 0x8800000, 0x0800000, CRC(59557b9f) SHA1(beda44c65c69110bdf8afb7542ae39913dab54f2) )
	ROM_LOAD("mpr-22139.ic18", 0x9000000, 0x0800000, CRC(92faa2ca) SHA1(4953f0219c3ae62de0a89473cb7b9dd30b33fcfb) )
	ROM_LOAD("mpr-22140.ic19", 0x9800000, 0x0800000, CRC(4cb54893) SHA1(a99b39cc3c82c3cf90f794bb8c8ba60638a6f921) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "dybb99-key.bin", 0, 4, CRC(90263797) SHA1(83649c8b3e562bee1f08663e59c5ba8f404ed36c) )
ROM_END

ROM_START( smlg99 )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0xb000000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD( "epr-22059.ic22", 0x0000000, 0x200000, CRC(5784f970) SHA1(e9ec692206a95cc260521154305693f6022190bc) )
	ROM_LOAD( "mpr-22038.ic1",  0x0800000, 0x800000, CRC(0a59fc5b) SHA1(efcbe2f847927fba173d333c889dd7df329d6df6) )
	ROM_LOAD( "mpr-22039.ic2",  0x1000000, 0x800000, CRC(4de79b52) SHA1(c63a3ce88db316e882948baf121fa96242010c8d) )
	ROM_LOAD( "mpr-22040.ic3",  0x1800000, 0x800000, CRC(b993fd90) SHA1(609de1509f393f884813ca4bcac533e10088ca84) )
	ROM_LOAD( "mpr-22041.ic4",  0x2000000, 0x800000, CRC(ddadfabd) SHA1(64531e68b10635415d49c0304a8ba550a3a9cef1) )
	ROM_LOAD( "mpr-22042.ic5",  0x2800000, 0x800000, CRC(136c101e) SHA1(a35d8d574b263d672a08963a992a7b507c838b70) )
	ROM_LOAD( "mpr-22043.ic6",  0x3000000, 0x800000, CRC(7f15a0a5) SHA1(5c36fa580f7eef448fb32d050078843470fafb31) )
	ROM_LOAD( "mpr-22044.ic7",  0x3800000, 0x800000, CRC(94376002) SHA1(8c4b954da69a079d9a73b8f34a9f1a94cceee9cc) )
	ROM_LOAD( "mpr-22045.ic8",  0x4000000, 0x800000, CRC(e520e2d9) SHA1(c7a9306b2dafb20baaa8bd6708fb5ece775c37f0) )
	ROM_LOAD( "mpr-22046.ic9",  0x4800000, 0x800000, CRC(976edfc8) SHA1(340b701b9e1d256963a8fe056ec975d8cbfec3d8) )
	ROM_LOAD( "mpr-22047.ic10", 0x5000000, 0x800000, CRC(32b136de) SHA1(4019e9836174b47135d7e1bbc02c23dd3ab52904) )
	ROM_LOAD( "mpr-22048.ic11", 0x5800000, 0x800000, CRC(32a9488a) SHA1(70c11fbcb1e24ed120f74aa455806e7dfbfa75b5) )
	ROM_LOAD( "mpr-22049.ic12", 0x6000000, 0x800000, CRC(8295696d) SHA1(5820cac054070aa35a64abc5bf8c6f45fe9be03f) )
	ROM_LOAD( "mpr-22050.ic13", 0x6800000, 0x800000, CRC(1cfebe44) SHA1(24415f723cd6c1efaa2513f6b52e076364eb0875) )
	ROM_LOAD( "mpr-22051.ic14", 0x7000000, 0x800000, CRC(58b9dbef) SHA1(1021d4dfbd5bcf6e1703ce608560c76d60dac71c) )
	ROM_LOAD( "mpr-22052.ic15", 0x7800000, 0x800000, CRC(95b87c1d) SHA1(bf0a89703fea7bd37e4d0fd10c7729d2cfe848d6) )
	ROM_LOAD( "mpr-22053.ic16", 0x8000000, 0x800000, CRC(666589a1) SHA1(130477f247661e87bdc4f2370788ca676336f563) )
	ROM_LOAD( "mpr-22054.ic17", 0x8800000, 0x800000, CRC(9d8c82e9) SHA1(13ebf9ffac9e1e960fa9662800c5e682284a5cdd) )
	ROM_LOAD( "mpr-22055.ic18", 0x9000000, 0x800000, CRC(fa865125) SHA1(04e0d77287f4e29df514875683992ede1e385dbc) )
	ROM_LOAD( "mpr-22056.ic19", 0x9800000, 0x800000, CRC(45a23d29) SHA1(2499637a4b389cda7cc9a7aa21014696bd1dafe2) )
	ROM_LOAD( "mpr-22057.ic20", 0xa000000, 0x800000, CRC(a056c109) SHA1(637e80c2d605851265430b0fa771a4ad5233be8a) )
	ROM_LOAD( "mpr-22058.ic21", 0xa800000, 0x800000, CRC(f16edaa0) SHA1(e093f5594df43c592a9acd45002ecc65035c2435) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "smlg99-key.bin", 0, 4, CRC(90263797) SHA1(83649c8b3e562bee1f08663e59c5ba8f404ed36c) )
ROM_END

/*

SYSTEMID: NAOMI
JAP: F355 CHALLENGE JAPAN
USA: F355 CHALLENGE USA
EXP: F355 CHALLENGE EXPORT

*/

ROM_START( f355 )
	F355_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0xb000000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD("epr-21902.ic22",0x0000000, 0x0400000, CRC(04e8acec) SHA1(82e20f99876b13b77c0393ef545316f9eeb2c29c) )

	ROM_LOAD("mpr-21881.ic1", 0x0800000, 0x0800000, CRC(00bf0d58) SHA1(cf2c58168501c77318e946a4a4d4663993a7913c) )
	ROM_LOAD("mpr-21882.ic2", 0x1000000, 0x0800000, CRC(f87923cd) SHA1(71de4f550e507c9e967331c4a17349df064608ea) )
	ROM_LOAD("mpr-21883.ic3", 0x1800000, 0x0800000, CRC(8c8280b8) SHA1(1a7003f4111ed9715b9ef0b13b0e9ace6a6f5434) )
	ROM_LOAD("mpr-21884.ic4", 0x2000000, 0x0800000, CRC(7bfa2f9a) SHA1(5796291b14ab25f8fed8d4af43558c7294d49e27) )
	ROM_LOAD("mpr-21885.ic5", 0x2800000, 0x0800000, CRC(5a999e6c) SHA1(7a60fe7d2f234c5d9c02ba403422e3a3de5a86ba) )
	ROM_LOAD("mpr-21886.ic6", 0x3000000, 0x0800000, CRC(dee42cfb) SHA1(437257e035e1b9cfbc3a0c15b24ef1aac4f2fbcb) )
	ROM_LOAD("mpr-21887.ic7", 0x3800000, 0x0800000, CRC(fdcc0334) SHA1(3e3d2094a082f3f2dac5ffe5a7e26cf9e61a279b) )
	ROM_LOAD("mpr-21888.ic8", 0x4000000, 0x0800000, CRC(0c717590) SHA1(d304351b07145252816afc9dd82587a1731f665d) )
	ROM_LOAD("mpr-21889.ic9", 0x4800000, 0x0800000, CRC(e8935135) SHA1(609788d5adf976d5313b3fca02ebc2f3c5e2758b) )
	ROM_LOAD("mpr-21890.ic10",0x5000000, 0x0800000, CRC(aeb9d086) SHA1(22f7d2c09718bf3acb910b5950b0601adad859a2) )
	ROM_LOAD("mpr-21891.ic11",0x5800000, 0x0800000, CRC(16d07b04) SHA1(6f222e226e63e30a73649735349c1928c37e011b) )
	ROM_LOAD("mpr-21892.ic12",0x6000000, 0x0800000, CRC(2d91eed2) SHA1(f3cda9776c800ac11e13b6914d59edb11f3e116b) )
	ROM_LOAD("mpr-21893.ic13",0x6800000, 0x0800000, CRC(e55ef69b) SHA1(fa62f8034728751477effcfecff2bc4cdc982b28) )
	ROM_LOAD("mpr-21894.ic14",0x7000000, 0x0800000, CRC(f1acfaea) SHA1(2684f79c6b7595075df41d1f398f228b4aedab16) )
	ROM_LOAD("mpr-21895.ic15",0x7800000, 0x0800000, CRC(98368844) SHA1(331f87def4f82ceb1bf74b16709ef61dfcda1758) )
	ROM_LOAD("mpr-21896.ic16",0x8000000, 0x0800000, CRC(4bc2ab68) SHA1(3a6d6b7599ca0f2c63cdbc3f5916e548bf3697c7) )
	ROM_LOAD("mpr-21897.ic17",0x8800000, 0x0800000, CRC(4ef4448d) SHA1(475021aec754d4526aff77776c8d2abce2b23199) )
	ROM_LOAD("mpr-21898.ic18",0x9000000, 0x0800000, CRC(cacea996) SHA1(df2b7ce00d8d6171806f676966f5f45d7fb76431) )
	ROM_LOAD("mpr-21899.ic19",0x9800000, 0x0800000, CRC(14a4b87d) SHA1(33177dea88c6aec31e2c16c8d0d3f29c7ea772c5) )
	ROM_LOAD("mpr-21900.ic20",0xa000000, 0x0800000, CRC(81901130) SHA1(1573b5c4360e29ba1a4b4901af49d5399fa1e635) )
	ROM_LOAD("mpr-21901.ic21",0xa800000, 0x0800000, BAD_DUMP CRC(55dcbd6d) SHA1(9fec353f9e58016090e177f899a799e2e8fc7c9f) ) // returns bad in Naomi test mode.  ROM reads different each time and fails test mode on real h/w, need a new cart

	ROM_REGION( 0x10000, "drivebd", 0 )	/* drive board ROM */
	ROM_LOAD( "epr-21867.bin", 0x000000, 0x010000, CRC(4f93a2a0) SHA1(875907e7fcfc44850e2c60c12268ac61c742f217) )
ROM_END

ROM_START( f355twin )
	F355_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0xb000000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD( "epr-22848.ic22", 0x0000000, 0x800000, CRC(54de0bd5) SHA1(5f6be36d2d39eea681ecac14358b92eaf4bf6c1e) )
	ROM_LOAD( "mpr-22827.ic1",  0x0800000, 0x800000, CRC(eeb1b975) SHA1(929f453eaf5565ae3e660dbbb8f406ff8aa7897d) )
	ROM_LOAD( "mpr-22828.ic2",  0x1000000, 0x800000, CRC(691d246a) SHA1(a2d538bc2e0d592a4f18d65f52fea035e1d4c625) )
	ROM_LOAD( "mpr-22829.ic3",  0x1800000, 0x800000, CRC(00719c9c) SHA1(f0f19af4ebe2720bd822a9ea7e0004db163c706a) )
	ROM_LOAD( "mpr-22830.ic4",  0x2000000, 0x800000, CRC(bfeb0e95) SHA1(a2dac7887dec722bd4b90a526bbcb9910b636618) )
	ROM_LOAD( "mpr-22831.ic5",  0x2800000, 0x800000, CRC(697e60a8) SHA1(805dd3fb7b86d1ad8afadba58c7c026444e62e32) )
	ROM_LOAD( "mpr-22832.ic6",  0x3000000, 0x800000, CRC(78e146a0) SHA1(cb6f1313ae51addbc84f78b3fb1e5d3adbe9af7c) )
	ROM_LOAD( "mpr-22834.ic7",  0x3800000, 0x800000, CRC(cbd847ea) SHA1(7c54f909d9bc10fda12bf28d5d4b83052a0583d4) )
	ROM_LOAD( "mpr-22834.ic8",  0x4000000, 0x800000, CRC(3bfc6571) SHA1(c3d7e1a75a8a2490c3b9b6f475ec948c40c84085) )
	ROM_LOAD( "mpr-22835.ic9",  0x4800000, 0x800000, CRC(c0a14f8e) SHA1(811d95d3741a14a215f34b3dc465e4944d746568) )
	ROM_LOAD( "mpr-22836.ic10", 0x5000000, 0x800000, CRC(ee68d756) SHA1(319f5633c3a377461fcedcf4b01edac41a26ad4b) )
	ROM_LOAD( "mpr-22837.ic11", 0x5800000, 0x800000, CRC(3b53f0c9) SHA1(b9be9c3de9af3eefb16b77eb0ee8d2f144d66919) )
	ROM_LOAD( "mpr-22838.ic12", 0x6000000, 0x800000, CRC(c17a2228) SHA1(0fcea748f5bacfdc784275e6f810001897f07bf5) )
	ROM_LOAD( "mpr-22839.ic13", 0x6800000, 0x800000, CRC(31ab7352) SHA1(3a5b5a04172d4d32c2fcff540dd71ddb99bf662c) )
	ROM_LOAD( "mpr-22840.ic14", 0x7000000, 0x800000, CRC(af4c757b) SHA1(b17722fa1f762c38e777ba36ffaf967062f86eb9) )
	ROM_LOAD( "mpr-22841.ic15", 0x7800000, 0x800000, CRC(7adceb6b) SHA1(17e1833d3d22a244cd16ba93c74bd25bbaa1018d) )
	ROM_LOAD( "mpr-22842.ic16", 0x8000000, 0x800000, CRC(1ce2ec11) SHA1(279464955f3b10c71aef1e41c68337f85d871739) )
	ROM_LOAD( "mpr-22843.ic17", 0x8800000, 0x800000, CRC(1c659384) SHA1(4c5ca20c9924c56e5f7a51ecaaafac3c5c6f91c8) )
	ROM_LOAD( "mpr-22844.ic18", 0x9000000, 0x800000, CRC(361ea725) SHA1(b2d17b2f09b9ae1e19bdc395189fa966ba462c06) )
	ROM_LOAD( "mpr-22845.ic19", 0x9800000, 0x800000, CRC(3327aed1) SHA1(8bd81aa79ffe764da5810fe79a317530a4f3c191) )
	ROM_LOAD( "mpr-22846.ic20", 0xa000000, 0x800000, CRC(d4148f39) SHA1(b6598ce52bcaa42805c581de326c953d27c1b2b4) )
	ROM_LOAD( "mpr-22847.ic21", 0xa800000, 0x800000, CRC(955ad42e) SHA1(e396ca02b5786557434632c4fac56af3a4a9f8ce) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "f355twin-key.bin", 0, 4, CRC(7d915a40) SHA1(ea441c10673b79237a4e7368948cfad31ed45a26) )
ROM_END

ROM_START( f355twn2 )
	F355_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0xb000000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD( "epr-23399.ic22", 0x0000000, 0x400000, CRC(36de514c) SHA1(1c32064169c233156921fdf170c1958dc0f8a750) )
	ROM_LOAD( "mpr-23378.ic1",  0x0800000, 0x800000, CRC(1ad80f12) SHA1(415a021987e07bb298e43eacb54ff898619837b1) )
	ROM_LOAD( "mpr-23379.ic2",  0x1000000, 0x800000, CRC(a198f0a8) SHA1(7025adfd26f80087fa405acb49797d5c77a55e98) )
	ROM_LOAD( "mpr-23380.ic3",  0x1800000, 0x800000, CRC(b1993286) SHA1(01ddc81ba3542f37dd2dadac972114ec254059a1) )
	ROM_LOAD( "mpr-23381.ic4",  0x2000000, 0x800000, CRC(1204d518) SHA1(5b272be2ff7d48ee8005194d03ae79a01cea1b92) )
	ROM_LOAD( "mpr-23382.ic5",  0x2800000, 0x800000, CRC(f2f3f7ab) SHA1(3e662197d7cc0b606706a2edb9433093d2bcd2d9) )
	ROM_LOAD( "mpr-23383.ic6",  0x3000000, 0x800000, CRC(f19069c1) SHA1(ccec4008d95f8305e9d77a5b34a3de8aec3606dc) )
	ROM_LOAD( "mpr-23384.ic7",  0x3800000, 0x800000, CRC(5150bb17) SHA1(9fab19f02e6f79e68e8e10aad78e47135081957d) )
	ROM_LOAD( "mpr-23385.ic8",  0x4000000, 0x800000, CRC(747e4025) SHA1(ad3ee65ac473fda8c82c5e3dd349abaa9312bd35) )
	ROM_LOAD( "mpr-23386.ic9",  0x4800000, 0x800000, CRC(28e22914) SHA1(e57d627de314dd13c229de91d061588df53b2164) )
	ROM_LOAD( "mpr-23387.ic10", 0x5000000, 0x800000, CRC(3b5c0fe4) SHA1(cbe93bf95d6386e5a0d44e27e6953e259f8667bd) )
	ROM_LOAD( "mpr-23388.ic11", 0x5800000, 0x800000, CRC(35d55060) SHA1(7c566f2ee82478aa63689f96088138902d9ee710) )
	ROM_LOAD( "mpr-23389.ic12", 0x6000000, 0x800000, CRC(360b9078) SHA1(26160273848d4f9c3992ea125b0d36bed58add49) )
	ROM_LOAD( "mpr-23390.ic13", 0x6800000, 0x800000, CRC(d5878f2d) SHA1(d918a37198033c74dadc1c531889f61c88ef94a8) )
	ROM_LOAD( "mpr-23391.ic14", 0x7000000, 0x800000, CRC(eef8b0c6) SHA1(0339a094cbecf97c785bb1071b2b598c1ab60e40) )
	ROM_LOAD( "mpr-23392.ic15", 0x7800000, 0x800000, CRC(df16126b) SHA1(b932afe38dd8e5f96412807743f44043fe450f14) )
	ROM_LOAD( "mpr-23393.ic16", 0x8000000, 0x800000, CRC(e6d383a3) SHA1(6a77318f718171fe998a8e18d542dd43b8a9b87d) )
	ROM_LOAD( "mpr-23394.ic17", 0x8800000, 0x800000, CRC(045235c4) SHA1(f1c4e09847840769d26719a26bdcf3c9241280a5) )
	ROM_LOAD( "mpr-23395.ic18", 0x9000000, 0x800000, CRC(ed645203) SHA1(b621c96c9ca49a7582a50bf5c513b910dead4e13) )
	ROM_LOAD( "mpr-23396.ic19", 0x9800000, 0x800000, CRC(42826956) SHA1(386eca3cda2bddb1825dbae850f0c17d1374eb41) )
	ROM_LOAD( "mpr-23397.ic20", 0xa000000, 0x800000, CRC(28d2caf6) SHA1(67a3bc19abccf7f211c3aae67e751815857bd564) )
	ROM_LOAD( "mpr-23398.ic21", 0xa800000, 0x800000, CRC(ea4d4d2a) SHA1(3dc9c7164516ae7f3b988c088ab819d8fd40d75e) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "f355twn2-key.bin", 0, 4, CRC(784fab7a) SHA1(5d9687876b390d35309c0b1404f5717daa533286) )
ROM_END

ROM_START( alpiltdx )
	AIRLINE_BIOS
	NAOMI_DEFAULT_EEPROM_NO_BD

	ROM_REGION( 0xb000000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD( "epr-21787b.ic22", 0x0000000, 0x400000, CRC(56893156) SHA1(8e56e0633f92b1f50105421b7eb8428f51a78b27) )
	ROM_LOAD( "mpr-21728.ic1",  0x0800000, 0x800000, CRC(872338d4) SHA1(04857b300196c0ec51361d7cf7bb57274a15a326) )
	ROM_LOAD( "mpr-21729.ic2",  0x1000000, 0x800000, CRC(9a9b72ad) SHA1(ce96da7904dd82abaa448df45e954521dd834ed8) )
	ROM_LOAD( "mpr-21730.ic3",  0x1800000, 0x800000, CRC(93c25058) SHA1(658374bca3cf615982ebcf493eeaaa9e40e70f03) )
	ROM_LOAD( "mpr-21731.ic4",  0x2000000, 0x800000, CRC(f14e578b) SHA1(d572903f7021757aebbb903b25a11a5aaf9f7a71) )
	ROM_LOAD( "mpr-21732.ic5",  0x2800000, 0x800000, CRC(28ea4e8c) SHA1(7f87fe08819e756bb7aadca2aaacb0f6e59c13f0) )
	ROM_LOAD( "mpr-21733.ic6",  0x3000000, 0x800000, CRC(5aee9e99) SHA1(8db726a73723c931fd8a4be2dd99d7c32352ad21) )
	ROM_LOAD( "mpr-21734.ic7",  0x3800000, 0x800000, CRC(0574390d) SHA1(5988bdd089d23035ee2dd3596ea9c822455311d3) )
	ROM_LOAD( "mpr-21735.ic8",  0x4000000, 0x800000, CRC(811400b4) SHA1(5f8d8b70f499b293b2d952c754c853c53b39c438) )
	ROM_LOAD( "mpr-21736.ic9",  0x4800000, 0x800000, CRC(d74eda63) SHA1(d6794fa433cea9f06dc0a20dc9e10388162e7fd8) )
	ROM_LOAD( "mpr-21737.ic10", 0x5000000, 0x800000, CRC(260aaa98) SHA1(d1082587afe9d79f286df8b107a553ee51c27643) )
	ROM_LOAD( "mpr-21738.ic11", 0x5800000, 0x800000, CRC(95a592e8) SHA1(862dce467e8805381bab001df68262f1baf3c498) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "alpiltdx-key.bin", 0, 4, CRC(bb539511) SHA1(dea206e7db23fdabf7b957104ddd1499eff509ff) )

	// on-cart X76F100 eeprom contents
	ROM_REGION( 0x84, "naomibd_eeprom", 0 )
	ROM_LOAD( "airlinepdx.sf",  0x000000, 0x000084, CRC(404b2add) SHA1(540c8474806775646ace111a2993397b1419fee3) )
ROM_END

ROM_START( alpilota )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0xb000000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD( "epr-21739a.ic22", 0x000000, 0x400000, CRC(08f22bab) SHA1(fedc80eef7c824381fd834cc04202383c9340c4f) )
	ROM_LOAD( "mpr-21728.ic1",  0x0800000, 0x800000, CRC(872338d4) SHA1(04857b300196c0ec51361d7cf7bb57274a15a326) )
	ROM_LOAD( "mpr-21729.ic2",  0x1000000, 0x800000, CRC(9a9b72ad) SHA1(ce96da7904dd82abaa448df45e954521dd834ed8) )
	ROM_LOAD( "mpr-21730.ic3",  0x1800000, 0x800000, CRC(93c25058) SHA1(658374bca3cf615982ebcf493eeaaa9e40e70f03) )
	ROM_LOAD( "mpr-21731.ic4",  0x2000000, 0x800000, CRC(f14e578b) SHA1(d572903f7021757aebbb903b25a11a5aaf9f7a71) )
	ROM_LOAD( "mpr-21732.ic5",  0x2800000, 0x800000, CRC(28ea4e8c) SHA1(7f87fe08819e756bb7aadca2aaacb0f6e59c13f0) )
	ROM_LOAD( "mpr-21733.ic6",  0x3000000, 0x800000, CRC(5aee9e99) SHA1(8db726a73723c931fd8a4be2dd99d7c32352ad21) )
	ROM_LOAD( "mpr-21734.ic7",  0x3800000, 0x800000, CRC(0574390d) SHA1(5988bdd089d23035ee2dd3596ea9c822455311d3) )
	ROM_LOAD( "mpr-21735.ic8",  0x4000000, 0x800000, CRC(811400b4) SHA1(5f8d8b70f499b293b2d952c754c853c53b39c438) )
	ROM_LOAD( "mpr-21736.ic9",  0x4800000, 0x800000, CRC(d74eda63) SHA1(d6794fa433cea9f06dc0a20dc9e10388162e7fd8) )
	ROM_LOAD( "mpr-21737.ic10", 0x5000000, 0x800000, CRC(260aaa98) SHA1(d1082587afe9d79f286df8b107a553ee51c27643) )
	ROM_LOAD( "mpr-21738.ic11", 0x5800000, 0x800000, CRC(95a592e8) SHA1(862dce467e8805381bab001df68262f1baf3c498) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "alpilota-key.bin", 0, 4, CRC(bb539511) SHA1(dea206e7db23fdabf7b957104ddd1499eff509ff) )
ROM_END

ROM_START( hotd2 )
	HOTD2_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x10000, "gunboard", ROMREGION_ERASEFF ) // ROM on the lightgun i/o board, near a D78213 ROM-less MCU
	ROM_LOAD( "tg12.ic2", 0x00000, 0x10000, CRC(2c9600b1) SHA1(91813a43851c48d400fde41b1198dabf55bade2d) )

	ROM_REGION( 0xa800000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD( "epr-21585.ic22", 0x0000000, 0x200000, CRC(b23d1a0c) SHA1(9e77980d1aa980c879886e53cc76a16d7a9d43a1) )
	ROM_LOAD( "mpr-21386.ic1",  0x0800000, 0x800000, CRC(88fb0562) SHA1(185a0eab68d86617cb6325d64c48a2dd4854622b) )
	ROM_LOAD( "mpr-21387.ic2",  0x1000000, 0x800000, CRC(5f4dd576) SHA1(5483c3949e587bbcca7e8fc7db9aff4cd2a33f02) )
	ROM_LOAD( "mpr-21388.ic3",  0x1800000, 0x800000, CRC(3e62fca4) SHA1(8cdebdebabc88160f458e1e779d9ebb4e6a14523) )
	ROM_LOAD( "mpr-21389.ic4",  0x2000000, 0x800000, CRC(6f73a852) SHA1(d5fd4c0800b3a1ea04231018fcaba79184fa1d87) )
	ROM_LOAD( "mpr-21390.ic5",  0x2800000, 0x800000, CRC(c7950445) SHA1(4f56768f07703452ef92d183e4ee654ab9711283) )
	ROM_LOAD( "mpr-21391.ic6",  0x3000000, 0x800000, CRC(5a812247) SHA1(7636661da0cc9bd5a1a2062f9f3ef65889c86fd5) )
	ROM_LOAD( "mpr-21392.ic7",  0x3800000, 0x800000, CRC(17e9414a) SHA1(9f291c4dd9a049eeed88d80867f7fca1d15c6095) )
	ROM_LOAD( "mpr-21393.ic8",  0x4000000, 0x800000, CRC(5d2d8134) SHA1(a2941b6afd0302822133d932064d1aad873b1c04) )
	ROM_LOAD( "mpr-21394.ic9",  0x4800000, 0x800000, CRC(eacaf26d) SHA1(21e35def0ed998a70cc982f373feb50b7974612a) )
	ROM_LOAD( "mpr-21395.ic10", 0x5000000, 0x800000, CRC(1e3686be) SHA1(7ec1b3c9c94882c5fe7b6ba6ffe9220e90824870) )
	ROM_LOAD( "mpr-21396.ic11", 0x5800000, 0x800000, CRC(5ada00a2) SHA1(981c65310c89e7a26e2b2c3e57623e78f6ad33d0) )
	ROM_LOAD( "mpr-21397.ic12", 0x6000000, 0x800000, CRC(9eff6247) SHA1(9257492fc3e48516897002dd3ff247093af27d87) )
	ROM_LOAD( "mpr-21398.ic13", 0x6800000, 0x800000, CRC(8a80b16a) SHA1(ffeb061b31027ac322c14b9050c686b2b844d2e1) )
	ROM_LOAD( "mpr-21399.ic14", 0x7000000, 0x800000, CRC(7ae20daf) SHA1(b36d8e490ac477db178b8df08f7997448308d3fd) )
	ROM_LOAD( "mpr-21400.ic15", 0x7800000, 0x800000, CRC(fbb8641b) SHA1(6cb44f0a3f80eb68a218bba97b2395961c596b9c) )
	ROM_LOAD( "mpr-21401.ic16", 0x8000000, 0x800000, CRC(3881ec23) SHA1(e4b87a6c6fd6a2eeda8e0e5ae7bed01b18386e54) )
	ROM_LOAD( "mpr-21402.ic17", 0x8800000, 0x800000, CRC(66bff6e4) SHA1(f87d618231b71b65952fc7ea7ccabdd208622a00) )
	ROM_LOAD( "mpr-21403.ic18", 0x9000000, 0x800000, CRC(8cd2f654) SHA1(77eb7061caaf0288aad04ed88c4247d27617f338) )
	ROM_LOAD( "mpr-21404.ic19", 0x9800000, 0x800000, CRC(6cf6e705) SHA1(68d7e9becefe27b556e0c5d7ba00efd2d1fb71ca) )
	ROM_LOAD( "mpr-21405.ic20", 0xa000000, 0x800000, CRC(495e6265) SHA1(57936367fec0000691641525682fb8aefc4e4f56) )
ROM_END

ROM_START( hotd2o )
	HOTD2_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x10000, "gunboard", ROMREGION_ERASEFF ) // ROM on the lightgun i/o board, near a D78213 ROM-less MCU
	ROM_LOAD( "tg12.ic2", 0x00000, 0x10000, CRC(2c9600b1) SHA1(91813a43851c48d400fde41b1198dabf55bade2d) )

	ROM_REGION( 0xa800000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD( "epr-21385.ic22", 0x0000000, 0x200000, CRC(dedffe5f) SHA1(98b2a4c67ecb30cb096b9cea9061d904cf495937) )
	ROM_LOAD( "mpr-21386.ic1",  0x0800000, 0x800000, CRC(88fb0562) SHA1(185a0eab68d86617cb6325d64c48a2dd4854622b) )
	ROM_LOAD( "mpr-21387.ic2",  0x1000000, 0x800000, CRC(5f4dd576) SHA1(5483c3949e587bbcca7e8fc7db9aff4cd2a33f02) )
	ROM_LOAD( "mpr-21388.ic3",  0x1800000, 0x800000, CRC(3e62fca4) SHA1(8cdebdebabc88160f458e1e779d9ebb4e6a14523) )
	ROM_LOAD( "mpr-21389.ic4",  0x2000000, 0x800000, CRC(6f73a852) SHA1(d5fd4c0800b3a1ea04231018fcaba79184fa1d87) )
	ROM_LOAD( "mpr-21390.ic5",  0x2800000, 0x800000, CRC(c7950445) SHA1(4f56768f07703452ef92d183e4ee654ab9711283) )
	ROM_LOAD( "mpr-21391.ic6",  0x3000000, 0x800000, CRC(5a812247) SHA1(7636661da0cc9bd5a1a2062f9f3ef65889c86fd5) )
	ROM_LOAD( "mpr-21392.ic7",  0x3800000, 0x800000, CRC(17e9414a) SHA1(9f291c4dd9a049eeed88d80867f7fca1d15c6095) )
	ROM_LOAD( "mpr-21393.ic8",  0x4000000, 0x800000, CRC(5d2d8134) SHA1(a2941b6afd0302822133d932064d1aad873b1c04) )
	ROM_LOAD( "mpr-21394.ic9",  0x4800000, 0x800000, CRC(eacaf26d) SHA1(21e35def0ed998a70cc982f373feb50b7974612a) )
	ROM_LOAD( "mpr-21395.ic10", 0x5000000, 0x800000, CRC(1e3686be) SHA1(7ec1b3c9c94882c5fe7b6ba6ffe9220e90824870) )
	ROM_LOAD( "mpr-21396.ic11", 0x5800000, 0x800000, CRC(5ada00a2) SHA1(981c65310c89e7a26e2b2c3e57623e78f6ad33d0) )
	ROM_LOAD( "mpr-21397.ic12", 0x6000000, 0x800000, CRC(9eff6247) SHA1(9257492fc3e48516897002dd3ff247093af27d87) )
	ROM_LOAD( "mpr-21398.ic13", 0x6800000, 0x800000, CRC(8a80b16a) SHA1(ffeb061b31027ac322c14b9050c686b2b844d2e1) )
	ROM_LOAD( "mpr-21399.ic14", 0x7000000, 0x800000, CRC(7ae20daf) SHA1(b36d8e490ac477db178b8df08f7997448308d3fd) )
	ROM_LOAD( "mpr-21400.ic15", 0x7800000, 0x800000, CRC(fbb8641b) SHA1(6cb44f0a3f80eb68a218bba97b2395961c596b9c) )
	ROM_LOAD( "mpr-21401.ic16", 0x8000000, 0x800000, CRC(3881ec23) SHA1(e4b87a6c6fd6a2eeda8e0e5ae7bed01b18386e54) )
	ROM_LOAD( "mpr-21402.ic17", 0x8800000, 0x800000, CRC(66bff6e4) SHA1(f87d618231b71b65952fc7ea7ccabdd208622a00) )
	ROM_LOAD( "mpr-21403.ic18", 0x9000000, 0x800000, CRC(8cd2f654) SHA1(77eb7061caaf0288aad04ed88c4247d27617f338) )
	ROM_LOAD( "mpr-21404.ic19", 0x9800000, 0x800000, CRC(6cf6e705) SHA1(68d7e9becefe27b556e0c5d7ba00efd2d1fb71ca) )
	ROM_LOAD( "mpr-21405.ic20", 0xa000000, 0x800000, CRC(495e6265) SHA1(57936367fec0000691641525682fb8aefc4e4f56) )
ROM_END

ROM_START( hotd2p )
	HOTD2_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x10000, "gunboard", ROMREGION_ERASEFF ) // ROM on the lightgun i/o board, near a D78213 ROM-less MCU
	ROM_LOAD( "tg12.ic2", 0x00000, 0x10000, CRC(2c9600b1) SHA1(91813a43851c48d400fde41b1198dabf55bade2d) )

	ROM_REGION( 0xa800000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD( "hotd2proto.ic22", 0x000000, 0x200000, CRC(676318a6) SHA1(19c0330468dcc20653bb9570df004af1daa37a33) )
	ROM_LOAD( "mpr-21386.ic1",  0x0800000, 0x800000, CRC(88fb0562) SHA1(185a0eab68d86617cb6325d64c48a2dd4854622b) )
	ROM_LOAD( "mpr-21387.ic2",  0x1000000, 0x800000, CRC(5f4dd576) SHA1(5483c3949e587bbcca7e8fc7db9aff4cd2a33f02) )
	ROM_LOAD( "mpr-21388.ic3",  0x1800000, 0x800000, CRC(3e62fca4) SHA1(8cdebdebabc88160f458e1e779d9ebb4e6a14523) )
	ROM_LOAD( "mpr-21389.ic4",  0x2000000, 0x800000, CRC(6f73a852) SHA1(d5fd4c0800b3a1ea04231018fcaba79184fa1d87) )
	ROM_LOAD( "mpr-21390.ic5",  0x2800000, 0x800000, CRC(c7950445) SHA1(4f56768f07703452ef92d183e4ee654ab9711283) )
	ROM_LOAD( "mpr-21391.ic6",  0x3000000, 0x800000, CRC(5a812247) SHA1(7636661da0cc9bd5a1a2062f9f3ef65889c86fd5) )
	ROM_LOAD( "mpr-21392.ic7",  0x3800000, 0x800000, CRC(17e9414a) SHA1(9f291c4dd9a049eeed88d80867f7fca1d15c6095) )
	ROM_LOAD( "mpr-21393.ic8",  0x4000000, 0x800000, CRC(5d2d8134) SHA1(a2941b6afd0302822133d932064d1aad873b1c04) )
	ROM_LOAD( "mpr-21394.ic9",  0x4800000, 0x800000, CRC(eacaf26d) SHA1(21e35def0ed998a70cc982f373feb50b7974612a) )
	ROM_LOAD( "mpr-21395.ic10", 0x5000000, 0x800000, CRC(1e3686be) SHA1(7ec1b3c9c94882c5fe7b6ba6ffe9220e90824870) )
	ROM_LOAD( "mpr-21396.ic11", 0x5800000, 0x800000, CRC(5ada00a2) SHA1(981c65310c89e7a26e2b2c3e57623e78f6ad33d0) )
	ROM_LOAD( "mpr-21397.ic12", 0x6000000, 0x800000, CRC(9eff6247) SHA1(9257492fc3e48516897002dd3ff247093af27d87) )
	ROM_LOAD( "mpr-21398.ic13", 0x6800000, 0x800000, CRC(8a80b16a) SHA1(ffeb061b31027ac322c14b9050c686b2b844d2e1) )
	ROM_LOAD( "mpr-21399.ic14", 0x7000000, 0x800000, CRC(7ae20daf) SHA1(b36d8e490ac477db178b8df08f7997448308d3fd) )
	ROM_LOAD( "mpr-21400.ic15", 0x7800000, 0x800000, CRC(fbb8641b) SHA1(6cb44f0a3f80eb68a218bba97b2395961c596b9c) )
	ROM_LOAD( "mpr-21401.ic16", 0x8000000, 0x800000, CRC(3881ec23) SHA1(e4b87a6c6fd6a2eeda8e0e5ae7bed01b18386e54) )
	ROM_LOAD( "mpr-21402.ic17", 0x8800000, 0x800000, CRC(66bff6e4) SHA1(f87d618231b71b65952fc7ea7ccabdd208622a00) )
	ROM_LOAD( "mpr-21403.ic18", 0x9000000, 0x800000, CRC(8cd2f654) SHA1(77eb7061caaf0288aad04ed88c4247d27617f338) )
	ROM_LOAD( "mpr-21404.ic19", 0x9800000, 0x800000, CRC(6cf6e705) SHA1(68d7e9becefe27b556e0c5d7ba00efd2d1fb71ca) )
	ROM_LOAD( "mpr-21405.ic20", 0xa000000, 0x800000, CRC(495e6265) SHA1(57936367fec0000691641525682fb8aefc4e4f56) )
ROM_END

/*
SYSTEMID: NAOMI
JAP: GIANT GRAM
USA: GIANT GRAM
EXP: GIANT GRAM

NO.     Type    Byte    Word
IC22    16M     0000    1111
IC1     64M     E504    548E

Serial: BAJE-01A0021
*/

ROM_START( ggram2 )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x6000000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD("epr-21820.ic22", 0x0000000, 0x0200000, CRC(0a198278) SHA1(0df5fc8b56ddafc66d92cb3923b851a5717b551d) )
	ROM_RELOAD( 0x200000, 0x200000 )
	ROM_RELOAD( 0x400000, 0x200000 )
	ROM_RELOAD( 0x600000, 0x200000 )
	ROM_LOAD("mpr-21821.ic1",  0x0800000, 0x0800000, CRC(ed127b65) SHA1(8b6d03fc733f601a48006d3268faa8983ca69d70) )
	/* IC2 empty */
	ROM_LOAD("mpr-21823.ic3",  0x1800000, 0x0800000, CRC(a304b528) SHA1(32197c74c659de2cc5f72f13c84bacac7b136d36) )
	ROM_LOAD("mpr-21824.ic4",  0x2000000, 0x0800000, CRC(6cf92c79) SHA1(378c71f506f129b6a9aebc9fc1faf96722a6d46d) )
	ROM_LOAD("mpr-21825.ic5",  0x2800000, 0x0800000, CRC(ebac834e) SHA1(bf01fa9021b79418af63d494d8ab89ef58570fb9) )
	ROM_LOAD("mpr-21826.ic6",  0x3000000, 0x0800000, CRC(da00dcb6) SHA1(744a67d7a63aa57fd5d85c5bb3dd2b2fff30dd1d) )
	ROM_LOAD("mpr-21827.ic7",  0x3800000, 0x0800000, CRC(40874dc1) SHA1(86a2958af503264ebe12928b6f3f17e2fb6675ae) )
	/* IC8 empty */
	ROM_LOAD("mpr-21829.ic9",  0x4800000, 0x0800000, CRC(c5553df2) SHA1(b97a82ac9133dab4bfd87392f803754b6d00389f) )
	ROM_LOAD("mpr-21830.ic10", 0x5000000, 0x0800000, CRC(e01ceb86) SHA1(dd5703d7712cfc0053bddfff63e78dda372b6ff2) )
	ROM_LOAD("mpr-21831.ic11", 0x5800000, 0x0800000, CRC(751848d0) SHA1(9c2267fd3c6f9ea5f2679bb2ca20d511a49b2845) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "ggram2-key.bin", 0, 4, CRC(14283fd8) SHA1(c928382e512e9fb685436020f88b8245b7c60d41) )
ROM_END

/*

SYSTEMID: NAOMI
JAP: GIANT GRAM 2000
USA: GIANT GRAM 2000
EXP: GIANT GRAM 2000

NO.     Type    Byte    Word
IC22    32M     0000    0000
IC1     64M     904A    81AE
IC2     64M     E9F7    B152
IC3     64M     A4D0    8FB7
IC4     64M     A869    64FB
IC5     64M     30CB    3483
IC6     64M     94DD    7F14
IC7     64M     BA8B    EA07
IC8     64M     6ADA    5CDA
IC9     64M     7CDA    86C1
IC10    64M     86F2    73A3
IC11    64M     44D8    1D11
IC12    64M     F25E    EDA8
IC13    64M     4804    6251
IC14    64M     E4FE    3808
IC15    64M     FD3D    D37A
IC16    64M     6D48    F5B3
IC17    64M     F0C6    CA29
IC18    64M     07C3    E4AE
IC19    64M     50F8    8500
IC20    64M     4EA2    D0CE
IC21    64M     090B    5667

Serial: BCCG-21A0451

*/

ROM_START( gram2000 )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0xb000000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD("epr-23377.ic11",        0x0000000, 0x0400000, CRC(4ca3149c) SHA1(9d25fc659658b416202b033754669be2f3abcdbe) )
	// the 0x0800000 - 0x0ffffff space mirrors part of roms 17/18, see ROM_COPY below, gets tested as 'IC1' if actel_id is 0xffff
	ROM_LOAD32_WORD("mpr-23357.ic17", 0x1000000, 0x0800000, CRC(eaf77487) SHA1(bdfc4666a6724441c11b31d89fa30c4bd11cbdd1) ) //ic 2
	ROM_LOAD32_WORD("mpr-23358.ic18", 0x1000002, 0x0800000, CRC(96819a5b) SHA1(e28c9d7b0579ab5d6116401b49f30dc8e4961618) ) //ic 3
	ROM_LOAD32_WORD("mpr-23359.ic19", 0x2000000, 0x0800000, CRC(757b9e89) SHA1(b131af1cbcb4fcebb7081b208acc86841192ff14) ) //ic 4
	ROM_LOAD32_WORD("mpr-23360.ic20", 0x2000002, 0x0800000, CRC(b38d28ff) SHA1(df4ff5be67c9812cdf8f018a9e60ec82b9faf7e4) ) //ic 5
	ROM_LOAD32_WORD("mpr-23361.ic21", 0x3000000, 0x0800000, CRC(680d7d77) SHA1(83cb29157f84739e424df7565e7bcb935564866f) ) //ic 6
	ROM_LOAD32_WORD("mpr-23362.ic22", 0x3000002, 0x0800000, CRC(84b7988d) SHA1(4e5d657be03f2c0b5e771e19c907aac60d8d8dac) ) //ic 7
	ROM_LOAD32_WORD("mpr-23363.ic23", 0x4000000, 0x0800000, CRC(17ae2b21) SHA1(8672166fbf99393ea2485ffb0fc4e64f43865bde) ) //ic 8
	ROM_LOAD32_WORD("mpr-23364.ic24", 0x4000002, 0x0800000, CRC(3d422a1c) SHA1(c82b0a9ebb56f17b4ce60293beee612c08564a25) ) //ic 9
	ROM_LOAD32_WORD("mpr-23365.ic25", 0x5000000, 0x0800000, CRC(e975496c) SHA1(6b309e28697c2884b806d17900702c62074d90a4) ) //ic 10
	ROM_LOAD32_WORD("mpr-23366.ic26", 0x5000002, 0x0800000, CRC(55e96378) SHA1(190ec23fabc60b820fd9b1486fd6cb1bfe56ba6c) ) //ic 11
	ROM_LOAD32_WORD("mpr-23367.ic27", 0x6000000, 0x0800000, CRC(5d40d017) SHA1(67a405c58687c119e774511b97399b5854ceb09b) ) //ic 12
	ROM_LOAD32_WORD("mpr-23368.ic28", 0x6000002, 0x0800000, CRC(8fb3be5f) SHA1(52c1f4537bf2dc2b47996dea87317ee9b7860cd9) ) //ic 13
	ROM_LOAD32_WORD("mpr-23369.ic29", 0x7000000, 0x0800000, CRC(a6a1671d) SHA1(4c458ce901a5cbb1cfd09bcf6926160a89c81e30) ) //ic 14
	ROM_LOAD32_WORD("mpr-23370.ic30", 0x7000002, 0x0800000, CRC(29876427) SHA1(18faeadd0c285edc94ff269b0c2faa0a3cc4c296) ) //ic 15
	ROM_LOAD32_WORD("mpr-23371.ic31", 0x8000000, 0x0800000, CRC(5cad6596) SHA1(1f19ca43c13afbe1a7cc48cf51a82aa06aec99f8) ) //ic 16
	ROM_LOAD32_WORD("mpr-23372.ic32", 0x8000002, 0x0800000, CRC(3d848b16) SHA1(328289998981dae6b593636a5bd2c6d0954c2625) ) //ic 17
	ROM_LOAD32_WORD("mpr-23373.ic33", 0x9000000, 0x0800000, CRC(369227f9) SHA1(85ce0f4f139788cda35471658196a84a36019fe6) ) //ic 18
	ROM_LOAD32_WORD("mpr-23374.ic34", 0x9000002, 0x0800000, CRC(1f8a2e08) SHA1(ff9b9bfada831baeb4830a3d1a4bfb38570b9972) ) //ic 19
	ROM_LOAD32_WORD("mpr-23375.ic35", 0xa000000, 0x0800000, CRC(7d4043db) SHA1(cadf22419e5b63c33a179bb6b0742035fc9d8028) ) //ic 20
	ROM_LOAD32_WORD("mpr-23376.ic36", 0xa000002, 0x0800000, CRC(e09cb473) SHA1(c3ec980f1a56142a0e06bae9594d6038acf0690d) ) //ic 21

	ROM_COPY( "rom_board", 0x1400000, 0x0800000, 0x0800000 ) // mirror data so IC1 check can pass
	//ROM_COPY( "rom_board", 0x1000000, 0x0400000, 0x0c00000 ) // or does it mirror the start of 17/18 from 0x0400000 upwards like this, check on real hw

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "gram2000-key.bin", 0, 4, CRC(179314d9) SHA1(3dbbc04e9ff62800d08c4a239af3a83252a28dc0) )
ROM_END

ROM_START( tduno2 )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

    ROM_REGION( 0x4000000, "rom_board", ROMREGION_ERASEFF)
    ROM_LOAD( "epr-23071.ic11",         0x0000000, 0x0200000, CRC(1b58f24a) SHA1(90f91af31beff9399c8d72ae0087bf4d3122cac2) )
    ROM_LOAD32_WORD( "mpr-23063.ic17s", 0x1000000, 0x0800000, CRC(9678a759) SHA1(6f2602c6eef8db0d4a145d832aec3ea0e0491c0c) )
    ROM_LOAD32_WORD( "mpr-23064.ic18",  0x1000002, 0x0800000, CRC(e159c44d) SHA1(fd0dcd74f7e214c7c9214bb04167f7c3acea30cf) )
    ROM_LOAD32_WORD( "mpr-23065.ic19s", 0x2000000, 0x0800000, CRC(10eff527) SHA1(c208529b0825ba45dc474c6aaa4b4c5557335c10) )
    ROM_LOAD32_WORD( "mpr-23066.ic20",  0x2000002, 0x0800000, CRC(4f9c1710) SHA1(7834bab5933a156656756f837c217e7820f2ae63) )
    ROM_LOAD32_WORD( "mpr-23067.ic21s", 0x3000000, 0x0800000, CRC(32724cd9) SHA1(7bfdd58dd5e69529125a720f51c53ebe526ef2e9) )
    ROM_LOAD32_WORD( "mpr-23068.ic22",  0x3000002, 0x0800000, CRC(2ce3e0ea) SHA1(805ec80273856ec6e5ee04fdca5fd1407c404f8e) )

    ROM_REGION( 4, "rom_key", 0 )
    ROM_LOAD( "tduno2.key",   0x000000, 0x000004, CRC(8e0f0f3b) SHA1(914d3db8746c806d559539cc0851169161d32c04) )
ROM_END

ROM_START( mtkob2 )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

    ROM_REGION( 0x4000000, "rom_board", ROMREGION_ERASEFF)
    ROM_LOAD( "epr-24217.ic11",         0x0000000, 0x0400000, CRC(5ecf807b) SHA1(f91d03a68a44d02970e360789b746bec9289275f) )
    ROM_LOAD32_WORD( "mpr-24218.ic17s", 0x1000000, 0x0800000, CRC(e8c51e1d) SHA1(e81f0580e144aa7a7e8c9399ffa09227b6e93675) )
    ROM_LOAD32_WORD( "mpr-24219.ic18",  0x1000002, 0x0800000, CRC(b0a5709f) SHA1(993f7a99d59a924641c37a549208723342007e5a) )
    ROM_LOAD32_WORD( "mpr-24220.ic19s", 0x2000000, 0x0800000, CRC(9f6cefe2) SHA1(499e17d2c284b340db6b124b63a23c7fa5045d0f) )
    ROM_LOAD32_WORD( "mpr-24221.ic20",  0x2000002, 0x0800000, CRC(73bf9cc6) SHA1(ab187cc0babd1435f5e8636331818d81de23636f) )
    ROM_LOAD32_WORD( "mpr-24222.ic21s", 0x3000000, 0x0800000, CRC(7098e728) SHA1(82f4f57efcee3063467c24758cefd406dccb1ea4) )
    ROM_LOAD32_WORD( "mpr-24223.ic22",  0x3000002, 0x0800000, CRC(eca13c90) SHA1(26a66906bf4ebda8697140d89eb5e493e941e8b2) )

    ROM_REGION( 4, "rom_key", 0 )
    ROM_LOAD( "mtkob2-key.bin", 0x000000, 0x000004, CRC(db088208) SHA1(14d65ad2555183a445abcd93907c85df4032b41d) )
ROM_END

ROM_START( crackndj )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0xa800000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD( "epr-23450.ic22", 0x0000000, 0x400000, CRC(ba0ee505) SHA1(7777f532ace9032a25fc949316c07bd70dd03851) )
	ROM_LOAD( "mpr-23525.ic1",  0x0800000, 0x1000000, CRC(01996526) SHA1(1080305424989593e606f8195d295e0fb822ae43) )
	ROM_LOAD( "mpr-23526.ic2",  0x1800000, 0x1000000, CRC(d7a0d52e) SHA1(64a03c6da70c64fc62dbb0f9a4b0fb35de59c72f) )
	ROM_LOAD( "mpr-23527.ic3",  0x2800000, 0x1000000, CRC(9361afd9) SHA1(fee55341b623cc1928d8f95acd53e20759db725f) )
	ROM_LOAD( "mpr-23528.ic4",  0x3800000, 0x1000000, CRC(e40b7970) SHA1(6467463994f2b4535f822357ff2c8ca2dd4450c8) )
	ROM_LOAD( "mpr-23529.ic5",  0x4800000, 0x1000000, CRC(5920e6e5) SHA1(eec207dba99ca541f2abc98674e8dcaef506af3b) )
	ROM_LOAD( "mpr-23530.ic6",  0x5800000, 0x1000000, CRC(2bb0aefc) SHA1(8e5e90a4b8780411a41f14f1ca16dd049aefcd4b) )
	ROM_LOAD( "mpr-23531.ic7",  0x6800000, 0x1000000, CRC(43592459) SHA1(1b69ce3c54ad2c054ea72547afba6ef55a2daf63) )
	ROM_LOAD( "mpr-23532.ic8",  0x7800000, 0x1000000, CRC(bbcddeee) SHA1(8649da1411e404953d5ebd4e459d407eb79b61b1) )
	ROM_LOAD( "mpr-23533.ic9",  0x8800000, 0x1000000, CRC(fc909c00) SHA1(9cf22a97ea272c4586f3942aefdb803bd0e6ede7) )
	ROM_LOAD( "mpr-23534.ic10", 0x9800000, 0x1000000, CRC(62ed85b6) SHA1(b88336bc6115c92a839981cb0c0d0a67b1f7eda5) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "crackndj-key.bin", 0, 4, CRC(6269265a) SHA1(e2ab31b0058eb130d8f54d30e86d569687f9a068) )
ROM_END

ROM_START( samba2k )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0xb000000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD( "epr-23600.ic22",  0x0000000, 0x400000, CRC(8b6fed00) SHA1(72842f266ad272e4c02be42a6529c2462fd8b63f) )
	ROM_LOAD( "mpr-23579.ic1",   0x0800000, 0x800000, CRC(ded76d71) SHA1(9187434587173f2fcc50af817222121703c253b7) )
	ROM_LOAD( "mpr-23580.ic2",   0x1000000, 0x800000, CRC(a9b29be1) SHA1(4f71b37b21623d87d805cb8d7b9da73bca1d22f3) )
	ROM_LOAD( "mpr-23581.ic3",   0x1800000, 0x800000, CRC(7c44d4c9) SHA1(a2c6e7392a2b52c1b5c0b133bb95fbe53736ca3e) )
	ROM_LOAD( "mpr-23582.ic4",   0x2000000, 0x800000, CRC(99b1af20) SHA1(598abe358d6c97a0ad6d68f5273cea1b96f73237) )
	ROM_LOAD( "mpr-23583.ic5",   0x2800000, 0x800000, CRC(7a81bbd1) SHA1(e36895d8964d1cdd3f316f0ec27356faa3d3f6e0) )
	ROM_LOAD( "mpr-23584.ic6",   0x3000000, 0x800000, CRC(e478b1df) SHA1(5b316feb618a0a61974c3fb5b4c5527873055a5b) )
	ROM_LOAD( "mpr-23585.ic7",   0x3800000, 0x800000, CRC(2f378c38) SHA1(881c6001c173c1ef8ddbb3ae15667e45fac11f96) )
	ROM_LOAD( "mpr-23586.ic8",   0x4000000, 0x800000, CRC(23bec2a7) SHA1(960a6613966bedf20c19454bbc5ec52d14c3f308) )
	ROM_LOAD( "mpr-23587.ic9",   0x4800000, 0x800000, CRC(f52a356b) SHA1(4e3783f5e8d02d09f474cc5ece457af5c4296508) )
	ROM_LOAD( "mpr-23588.ic10",  0x5000000, 0x800000, CRC(4bae020b) SHA1(e3750f87309c7ac9ab6fc504a9377971451ccb5a) )
	ROM_LOAD( "mpr-23589.ic11",  0x5800000, 0x800000, CRC(f6a09854) SHA1(d4ae9d7ddc349e353196cb145f732798a692e560) )
	ROM_LOAD( "mpr-23590.ic12s", 0x6000000, 0x800000, CRC(88e29d52) SHA1(42b66f13302772729920b9af546123c771ecec6b) )
	ROM_LOAD( "mpr-23591.ic13s", 0x6800000, 0x800000, CRC(8736d9b1) SHA1(d829d499dc147f0d14413554ff33187c7df9134f) )
	ROM_LOAD( "mpr-23592.ic14s", 0x7000000, 0x800000, CRC(c7dbeefd) SHA1(7b84d2f5532668936f60314fb3e112b1350bcc51) )
	ROM_LOAD( "mpr-23593.ic15s", 0x7800000, 0x800000, CRC(c8d63355) SHA1(64b06027ebbcfb72ead1d2041423ffd2fe4dcd97) )
	ROM_LOAD( "mpr-23594.ic16s", 0x8000000, 0x800000, CRC(30a04934) SHA1(5b3e5226b1eef9f2d9a64b89307380e2b347547e) )
	ROM_LOAD( "mpr-23595.ic17s", 0x8800000, 0x800000, CRC(1fea68ad) SHA1(d4615f6ea0b9ce6483cb688810ef7ae287aef082) )
	ROM_LOAD( "mpr-23596.ic18s", 0x9000000, 0x800000, CRC(2e57f934) SHA1(9231d3b881e210fa83dda8f03a1702166eb48750) )
	ROM_LOAD( "mpr-23597.ic19s", 0x9800000, 0x800000, CRC(fb50cfdb) SHA1(2e95407b1c8660ae8e85b2d06c13bdc474b93d6e) )
	ROM_LOAD( "mpr-23598.ic20s", 0xa000000, 0x800000, CRC(897c2304) SHA1(e21b68fd935092b5bd019e3087647e9612be1169) )
	ROM_LOAD( "mpr-23599.ic21s", 0xa800000, 0x800000, CRC(f2d1f73f) SHA1(40ac3ef9b1c3797e986fc4a0c593dd4e49df8892) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "samba2k-key.bin", 0, 4, CRC(01c0d74a) SHA1(4461cfd761919ba4bc6f1d1839a880f11eaf8d0f) )
ROM_END

ROM_START( alienfnt )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x5800000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD( "epr-23586t.bin",  0x0000000, 0x0400000, CRC(2303d764) SHA1(2787e0cc81484065fb8a70610cf0e445535c95eb) )
	ROM_LOAD( "mpr-23581.ic1",   0x0800000, 0x1000000, CRC(ef0b93ce) SHA1(52fc7f52dc0b079df8c44a6766c8f54678e6a722) )
	ROM_LOAD( "mpr-23582.ic2",   0x1800000, 0x1000000, CRC(e396009c) SHA1(70bf0d78f20e0bd9632d3f4d6501bc1dedfe0672) )
	ROM_LOAD( "mpr-23583.ic3",   0x2800000, 0x1000000, CRC(878e8efe) SHA1(a48a4ab2816605b817dc62d4080b7dc88100a270) )
	ROM_LOAD( "mpr-23584.ic4",   0x3800000, 0x1000000, CRC(8d444756) SHA1(89c480f9ed1239c8ae565c85fa0fd50324264b20) )
	ROM_LOAD( "mpr-23585.ic5",   0x4800000, 0x1000000, CRC(883a6482) SHA1(e3145710df793b7fd67f02707904416210a71978) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "alienfnt-key.bin", 0x000000, 0x000004, CRC(0cac6605) SHA1(7a050f13e3b9a10d6cdb851a8c67006f6548c60a) )
ROM_END

ROM_START( alienfnta )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x5800000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD( "epr-23586a.ic22", 0x0000000, 0x0400000, CRC(0fea04fc) SHA1(e9356e941d10db80d6d5dfa5f3237ea7c9044a73) )
	ROM_LOAD( "mpr-23581.ic1",   0x0800000, 0x1000000, CRC(ef0b93ce) SHA1(52fc7f52dc0b079df8c44a6766c8f54678e6a722) )
	ROM_LOAD( "mpr-23582.ic2",   0x1800000, 0x1000000, CRC(e396009c) SHA1(70bf0d78f20e0bd9632d3f4d6501bc1dedfe0672) )
	ROM_LOAD( "mpr-23583.ic3",   0x2800000, 0x1000000, CRC(878e8efe) SHA1(a48a4ab2816605b817dc62d4080b7dc88100a270) )
	ROM_LOAD( "mpr-23584.ic4",   0x3800000, 0x1000000, CRC(8d444756) SHA1(89c480f9ed1239c8ae565c85fa0fd50324264b20) )
	ROM_LOAD( "mpr-23585.ic5",   0x4800000, 0x1000000, CRC(883a6482) SHA1(e3145710df793b7fd67f02707904416210a71978) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "alienfnt-key.bin", 0x000000, 0x000004, CRC(0cac6605) SHA1(7a050f13e3b9a10d6cdb851a8c67006f6548c60a) )
ROM_END

/*
SYSTEMID: NAOMI
JAP: GUILTY GEAR X
USA: DISABLE
EXP: DISABLE
*/

ROM_START( ggx )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x7800000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD("epr-23356.ic22", 0x0000000, 0x0400000, CRC(ed2d289f) SHA1(d4f73c6cd25f320616e21f1ff0cdc0a566185dcb) )
	ROM_LOAD("mpr-23342.ic1",  0x0800000, 0x0800000, CRC(4fd89557) SHA1(3a687393d38e890acb0d1b0edc3ea585773c0222) )
	ROM_LOAD("mpr-23343.ic2",  0x1000000, 0x0800000, CRC(2e4417b6) SHA1(0f87fa92f01116b0acfae5f1b5a148c1a12a487f) )
	ROM_LOAD("mpr-23344.ic3",  0x1800000, 0x0800000, CRC(968eea3b) SHA1(a3bb7233b9a950f00b4dcd7bb055dbdba2b29860) )
	ROM_LOAD("mpr-23345.ic4",  0x2000000, 0x0800000, CRC(30efe1ec) SHA1(be28243ab84acb41229d42056ba051a839e7af65) )
	ROM_LOAD("mpr-23346.ic5",  0x2800000, 0x0800000, CRC(b34d9461) SHA1(44bd132189c5487fef559883300993393f9f29c6) )
	ROM_LOAD("mpr-23347.ic6",  0x3000000, 0x0800000, CRC(5a254cd1) SHA1(5ca00400c9e7f6c2565b1ff2d2552a90faadf6dd) )
	ROM_LOAD("mpr-23348.ic7",  0x3800000, 0x0800000, CRC(aff43142) SHA1(c23bbfceb47885164250ca4800a52b9e9e9e80bc) )
	ROM_LOAD("mpr-23349.ic8",  0x4000000, 0x0800000, CRC(e83871c7) SHA1(49a8140f38d896e8645fbc838f22af561bd2aa7d) )
	ROM_LOAD("mpr-23350.ic9",  0x4800000, 0x0800000, CRC(4237010b) SHA1(e757dff4c353416f99eaf3cb1945b94d2768fc4f) )
	ROM_LOAD("mpr-23351.ic10", 0x5000000, 0x0800000, CRC(b096f712) SHA1(f8e2322ba83224029cd4b91cf4d51a9376923b45) )
	ROM_LOAD("mpr-23352.ic11", 0x5800000, 0x0800000, CRC(1a01ab38) SHA1(c161d5f0d60849f4e2b51ac00ca877e1c5624bff) )
	ROM_LOAD("mpr-23353.ic12", 0x6000000, 0x0800000, CRC(daa0ca24) SHA1(afce14e213e79add7fded838e71bb4447425906a) )
	ROM_LOAD("mpr-23354.ic13", 0x6800000, 0x0800000, CRC(cea127f7) SHA1(11f12472ebfc93eb72b764c780e30afd4812dbe9) )
	ROM_LOAD("mpr-23355.ic14", 0x7000000, 0x0800000, CRC(e809685f) SHA1(dc052b4eb4fdcfdc22c4807316ce34ee7a0d58a6) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "ggx-key.bin", 0, 4, CRC(455a831b) SHA1(1834fc714224ac990a0677e8925493e92a4b5e5e) )
ROM_END

/*

SYSTEMID: NAOMI
JAP: HEAVY METAL JAPAN
USA: HEAVY METAL USA
EXP: HEAVY METAL EURO

NO.     Type    Byte    Word
IC22    32M     0000    0000
IC1     64M     CBA3    16D2
IC2     64M     087A    079B
IC3     64M     CDB0    804C
IC4     64M     326A    E815
IC5     64M     C164    5DB4
IC6     64M     38A0    AAFC
IC7     64M     1134    DFCC
IC8     64M     6597    6975
IC9     64M     D6FB    8917
IC10    64M     6442    18AC
IC11    64M     4F77    EEFE

*/


ROM_START( hmgeo )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x6000000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD("epr-23716a.ic22", 0x0000000, 0x0400000,  CRC(c5cb0d3b) SHA1(20de8f5ee183e996ccde77b10564a302939662db) )
	ROM_RELOAD( 0x400000, 0x400000)
	ROM_LOAD("mpr-23705.ic1", 0x0800000, 0x0800000, CRC(2549b57d) SHA1(02c04c8ccb0de680171d06700ca9a40208286894) )
	ROM_LOAD("mpr-23706.ic2", 0x1000000, 0x0800000, CRC(9f21865c) SHA1(a1f5aec34097cf2b86110110f586ba8b3cf28bd1) )
	ROM_LOAD("mpr-23707.ic3", 0x1800000, 0x0800000, CRC(ba2f42cd) SHA1(e924f8ef58cc81b7303d8fb3baf0e384c6387e7f) )
	ROM_LOAD("mpr-23708.ic4", 0x2000000, 0x0800000, CRC(19c4e61b) SHA1(a4619df98818d33bdaa3e6429c14d1aeec316e6a))
	ROM_LOAD("mpr-23709.ic5", 0x2800000, 0x0800000, CRC(676430b3) SHA1(5fa40c45afe97b0f09e575e3c01d44aa9259961d) )
	ROM_LOAD("mpr-23710.ic6", 0x3000000, 0x0800000, CRC(5d32dba3) SHA1(5bb5796a682cc6ee68458403c69343bf753ece7a) )
	ROM_LOAD("mpr-23711.ic7", 0x3800000, 0x0800000, CRC(650df507) SHA1(dff192b3bd4f39627779e2ba86d9dd13536221dd) )
	ROM_LOAD("mpr-23712.ic8", 0x4000000, 0x0800000, CRC(154f10ce) SHA1(67f6ff297f77632efe1965a81ed9f5c7dfa7a6b3) )
	ROM_LOAD("mpr-23713.ic9", 0x4800000, 0x0800000, CRC(2969bac7) SHA1(5f1cf6ac726c2fe183d66e4022962e44592f9ccd) )
	ROM_LOAD("mpr-23714.ic10",0x5000000, 0x0800000, CRC(da462c44) SHA1(ca450b6c07f939f96eba7b44c45b4e38abd598aa) )
	ROM_LOAD("mpr-23715.ic11",0x5800000, 0x0800000, CRC(c750abbd) SHA1(2a5bedc2b21cd3f991c7145ccfd8c7a9e7f647ae) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "hmgeo-key.bin", 0, 4, CRC(78411d2f) SHA1(9809ef35406ea48639cf78d5d366069a06a411f0) )
ROM_END

/*

SYSTEMID: NAOMI
JAP: GIGAWING2 JAPAN
USA: GIGAWING2 USA
EXP: GIGAWING2 EXPORT

NO.     Type    Byte    Word
IC22    16M     C1C3    618F
IC1     64M     8C09    3A15
IC2     64M     91DC    C17F
IC3     64M     25CB    2AA0
IC4     64M     EB35    C1FF
IC5     64M     8B25    914E
IC6     64M     72CB    68FA
IC7     64M     191E    2AF3
IC8     64M     EACA    12CD
IC9     64M     717F    40ED
IC10    64M     1E43    0F1A

*/

ROM_START( gwing2 )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x5800000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD("epr-22270.ic22",0x0000000, 0x0200000,  CRC(876b3c97) SHA1(eb171d4a0521c3bea42b4aae3607faec63e10581) )
	ROM_LOAD("mpr-22271.ic1", 0x0800000, 0x1000000, CRC(9a072af5) SHA1(d5edff43d180346ba4d4f214c08f2db290a72def) )
	ROM_LOAD("mpr-22272.ic2", 0x1800000, 0x1000000, CRC(1e816ab1) SHA1(6e1fd47a21f5da7d2145caaf68094445f122a239) )
	ROM_LOAD("mpr-22273.ic3", 0x2800000, 0x1000000, CRC(cd633dcf) SHA1(f044d93802a4ba29d0e70c597d3fbe65da591335) )
	ROM_LOAD("mpr-22274.ic4", 0x3800000, 0x1000000, CRC(f8daaaf3) SHA1(8854d3f8e3d55715ede33ee918b641e251f752b4) )
	ROM_LOAD("mpr-22275.ic5", 0x4800000, 0x1000000, CRC(61aa1521) SHA1(7d9f5790e72a9151d128ac7887e236526fdf72a0) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "gwing2-key.bin", 0, 4, CRC(433132ce) SHA1(27f6e6ba2dd28c777317ff085432c2d43b93a603) )
ROM_END

/*
SYSTEMID: NAOMI
JAP: IDOL JANSHI SUCHIE-PAI 3
USA: DISABLE
EXP: DISABLE

NO.     Type    Byte    Word
IC22    16M     0000    0000
IC1     64M     E467    524B
IC2     64M     9D05    4992
IC3     64M     E3F7    6481
IC4     64M     6C22    25E3
IC5     64M     180F    E89F
IC6     64M     60C9    2B86
IC7     64M     4EDE    4539
IC8     64M     3AD3    0046
IC9     64M     8D37    BA16
IC10    64M     8AE3    4D71
IC11    64M     B519    1393
IC12    64M     4695    B159
IC13    64M     536F    D0C6
IC14    32M     81F9    DA1B
*/

ROM_START( suchie3 )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x7800000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD("epr-21979.ic22",0x0000000, 0x0200000, CRC(335c9e25) SHA1(476790fdd99a8c13336e795b4a39b071ed86a97c) )
	ROM_LOAD("mpr-21980.ic1", 0x0800000, 0x0800000, CRC(2b5f958a) SHA1(609585dda27c5e111378a92f04fa03ae11d42540) )
	ROM_LOAD("mpr-21981.ic2", 0x1000000, 0x0800000, CRC(b4fff4ee) SHA1(333fb5a662775662881154b654233f207782a8aa) )
	ROM_LOAD("mpr-21982.ic3", 0x1800000, 0x0800000, CRC(923ee0ff) SHA1(4f92cc1abfd948a1ed15fdca11251aba96bdc022) )
	ROM_LOAD("mpr-21983.ic4", 0x2000000, 0x0800000, CRC(dd659ab1) SHA1(96d9825fc5cf72a9ef83f10e480fd8925b1d6762) )
	ROM_LOAD("mpr-21984.ic5", 0x2800000, 0x0800000, CRC(b34de0c7) SHA1(dbb7a6a19af2571441b5ecbddddae6891809ffcf) )
	ROM_LOAD("mpr-21985.ic6", 0x3000000, 0x0800000, CRC(f1516e0a) SHA1(246d287df592cd69df689dc10e8647a9dbf804b7) )
	ROM_LOAD("mpr-21986.ic7", 0x3800000, 0x0800000, CRC(2779c418) SHA1(8d1a89ddf0c68f1eaf6eb0dafadf9b614492fff1) )
	ROM_LOAD("mpr-21987.ic8", 0x4000000, 0x0800000, CRC(6aaaacdd) SHA1(f5e67c88db8bce8f2f4cab73a5d0a24ba57c812b) )
	ROM_LOAD("mpr-21988.ic9", 0x4800000, 0x0800000, CRC(ed61b155) SHA1(679124f0f7c7bc4791025cff274d903cf5bcae70) )
	ROM_LOAD("mpr-21989.ic10",0x5000000, 0x0800000, CRC(ae8562cf) SHA1(e31986e53159729434a7952e8c4ed2adf8dd8e9d) )
	ROM_LOAD("mpr-21990.ic11",0x5800000, 0x0800000, CRC(57fd9fdd) SHA1(62b3bc4a2828751459557b63d900ca6d46792e24) )
	ROM_LOAD("mpr-21991.ic12",0x6000000, 0x0800000, CRC(d82f834a) SHA1(06902713bdf6f68182749916cacc9ae6528dc355) )
	ROM_LOAD("mpr-21992.ic13",0x6800000, 0x0800000, CRC(599a2fb8) SHA1(2a0007064ad2ee1e1a0fda1d5676df4ff19a9f2f) )
	ROM_LOAD("mpr-21993.ic14",0x7000000, 0x0400000, CRC(fb28cf0a) SHA1(d51b1d4514a93074d1f77bd1bc5995739604cf56) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "suchie3-key.bin", 0, 4, CRC(592b5204) SHA1(ba18a5302ea271f58617a9793dfb85a0123080c8) )
ROM_END

/*
SYSTEMID: NAOMI
JAP: SHANGRI-LA
USA: SHANGRI-LA
EXP: SHANGRI-LA
*/

ROM_START( shangril )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x6800000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD("epr-22060.ic22", 0x0000000, 0x0400000, CRC(5ae18595) SHA1(baaf8fd948b07ab9970571fecebc3c4fab5d4897) )
	ROM_LOAD("mpr-22061.ic1",  0x0800000, 0x0800000, CRC(4d760b34) SHA1(ba7dce0ab7961a77622a41c3f50c112a7e9904aa) )
	ROM_LOAD("mpr-22062.ic2",  0x1000000, 0x0800000, CRC(f713c59f) SHA1(75d8559f1b847fd6a51009fe9333b9627adcbd75) )
	ROM_LOAD("mpr-22063.ic3",  0x1800000, 0x0800000, CRC(a93ad631) SHA1(c829c58ed899fe3d4f71950c883098a215bcda1b) )
	ROM_LOAD("mpr-22064.ic4",  0x2000000, 0x0800000, CRC(56e34efd) SHA1(f810a4a0105adb7f1eaa078440e28a9bac20c3ea) )
	ROM_LOAD("mpr-22065.ic5",  0x2800000, 0x0800000, CRC(44b230bd) SHA1(d560690ddd1b9bbe919b20599d25c544df2dc808) )
	ROM_LOAD("mpr-22066.ic6",  0x3000000, 0x0800000, CRC(69f0be28) SHA1(05fc6f3b18645b165cfa0ac7b3d56013aabb360b) )
	ROM_LOAD("mpr-22067.ic7",  0x3800000, 0x0800000, CRC(344f9d01) SHA1(260e748dc265fb2b5d50f9a856ccdd157ac103fd) )
	ROM_LOAD("mpr-22068.ic8",  0x4000000, 0x0800000, CRC(48d0d510) SHA1(d3aa51f29699363c8949b20493eba1a5c585ca0e) )
	ROM_LOAD("mpr-22069.ic9",  0x4800000, 0x0800000, CRC(94e6dfa9) SHA1(83ca9ea5d2892511626be362ff2cab22f2b945cf) )
	ROM_LOAD("mpr-22070.ic10", 0x5000000, 0x0800000, CRC(8dcd2b3d) SHA1(0d8b735120fc63306516f6acc333345cc7774ff1) )
	ROM_LOAD("mpr-22071.ic11", 0x5800000, 0x0800000, CRC(1ab1f1ab) SHA1(bb8fa8d5a681115a82e9598ebe599b106f7aae9d) )
	ROM_LOAD("mpr-22072.ic12", 0x6000000, 0x0800000, CRC(cb8d2634) SHA1(03ac8fb3a1acb1f8e32d9325c4da42417752f934) )
ROM_END

/*
SYSTEMID: NAOMI
JAP: MARVEL VS. CAPCOM 2
USA: MARVEL VS. CAPCOM 2
EXP: MARVEL VS. CAPCOM 2

Note: the following game is one of the few known regular Naomi game to have a rom test item in its specific test mode menu.
So the Naomi regular board test item is unreliable in this circumstance.

*/

ROM_START( mvsc2 )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x8900000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD("epr-23085a.ic11", 0x0000000, 0x0400000, CRC(5d5b7ad1) SHA1(f58c31b245fc33fa541f9f074548402a63f7c3d3) )
	ROM_RELOAD( 0x400000, 0x400000)
	ROM_LOAD("mpr-23048.ic17",  0x0800000, 0x0800000, CRC(93d7a63a) SHA1(c50d10b4a3f9db51eae5749f5b665d7c8ab6c898) )
	ROM_LOAD("mpr-23049.ic18",  0x1000000, 0x0800000, CRC(003dcce0) SHA1(fb71c8ca9271d2155878c72d8fe2df3031e6c014) )
	ROM_LOAD("mpr-23050.ic19",  0x1800000, 0x0800000, CRC(1d6b88a7) SHA1(ba42e9d1d912d88a7ad839b878975ba590634320) )
	ROM_LOAD("mpr-23051.ic20",  0x2000000, 0x0800000, CRC(01226aaa) SHA1(a4c6a0eda05e53d0e51b92a4317a86a708a7efdb) )
	ROM_LOAD("mpr-23052.ic21",  0x2800000, 0x0800000, CRC(74bee120) SHA1(5a0fb48fa758a2be2e08e3b1298103c5aa748835) )
	ROM_LOAD("mpr-23053.ic22",  0x3000000, 0x0800000, CRC(d92d4401) SHA1(a868780f8d2e176ff10781e1c08bf932f34ac504) )
	ROM_LOAD("mpr-23054.ic23",  0x3800000, 0x0800000, CRC(78ba02e8) SHA1(0f696a33e1e6671001efc309ed62f084a246ad24) )
	ROM_LOAD("mpr-23055.ic24",  0x4000000, 0x0800000, CRC(84319604) SHA1(c3dde162e043a54e1325202b46191b32e8784a1c) )
	ROM_LOAD("mpr-23056.ic25",  0x4800000, 0x0800000, CRC(d7386034) SHA1(be1f3ca5f283e428dc59dc072de3e7d36e122d53) )
	ROM_LOAD("mpr-23057.ic26",  0x5000000, 0x0800000, CRC(a3f087db) SHA1(b52d7c072cb5c2fdd10d0ac0b62cebe48b229ae3) )
	ROM_LOAD("mpr-23058.ic27",  0x5800000, 0x0800000, CRC(61a6cc5d) SHA1(34e52cb076888313a80f2b87876b8d37b91d85a0) )
	ROM_LOAD("mpr-23059.ic28",  0x6000000, 0x0800000, CRC(64808024) SHA1(1a6c60c330642b273978d3dd02d95d17d36ee3f2) )
	ROM_LOAD("mpr-23060.ic29",  0x6800000, 0x0800000, CRC(67519942) SHA1(fc758d9075625f8140d5d828c8f6b7a91bcc9119) )
	ROM_LOAD("mpr-23061.ic30",  0x7000000, 0x0800000, CRC(fb1844c4) SHA1(1d1571516a6dbed0c4ded3b80efde9cc9281f66f) )
	ROM_LOAD("mpr-23083.ic31",  0x7800000, 0x0400000, CRC(c61d2dfe) SHA1(a05fb979ed7c8040de91716fc8814e6bd995efa2) )
	ROM_LOAD("mpr-23084.ic32",  0x8000000, 0x0400000, CRC(e228cdfd) SHA1(d02a2e3557bd24cf34c5ddb42d41ca15e78ae885) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "mvsc2-key.bin", 0, 4, CRC(76f095b4) SHA1(773fd67e1eb471a989b3ee6e969a3d33bf61e779) )
ROM_END

/* toy fighter - 1999 sega */

ROM_START( toyfight )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x8000000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD("epr-22035.ic22",0x0000000, 0x0400000, CRC(dbc76493) SHA1(a9772bdb62610a39adf2b9f397781bcddda3e635) )

	ROM_LOAD("mpr-22025.ic1", 0x0800000, 0x0800000, CRC(30237202) SHA1(e229a7671b3a34b26a461716bd7b437da100e1c8) )
	ROM_LOAD("mpr-22026.ic2", 0x1000000, 0x0800000, CRC(f28e71ff) SHA1(019425fcf234beca2b586de5235cf9f171563533) )
	ROM_LOAD("mpr-22027.ic3", 0x1800000, 0x0800000, CRC(1a84632d) SHA1(f3880f21399c6713c48c710c06d0344a0a28f026) )
	ROM_LOAD("mpr-22028.ic4", 0x2000000, 0x0800000, CRC(2b34ccba) SHA1(76c39ea19c3be1d9a9ce9e67035be7543b71ff26) )
	ROM_LOAD("mpr-22029.ic5", 0x2800000, 0x0800000, CRC(8162953a) SHA1(15c9e10080a5f2e70c31b9b89a256050a1aed4e9) )
	ROM_LOAD("mpr-22030.ic6", 0x3000000, 0x0800000, CRC(5bf5fed6) SHA1(6c8eedb177aa49aee9a8b090f2e5f96644416c6c) )
	ROM_LOAD("mpr-22031.ic7", 0x3800000, 0x0800000, CRC(ee7c40cc) SHA1(b9d92ef5bae0e932ec8769a30ebd841a263d3e2a) )
	ROM_LOAD("mpr-22032.ic8", 0x4000000, 0x0800000, CRC(3c48c9ba) SHA1(00be199b23040f8e81db2ec489ba98cbf615652c) )
	ROM_LOAD("mpr-22033.ic9", 0x4800000, 0x0800000, CRC(5fe5586e) SHA1(3ff41ae1f81469597684faadd88e62b5e0634352) )
	ROM_LOAD("mpr-22034.ic10",0x5000000, 0x0800000, CRC(3aa5ce5e) SHA1(f00a906235e4522d6fc2ac771324114346875314) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "toyfight-key.bin", 0, 4, CRC(8e074319) SHA1(b1b88bc57b81c08853d4aa77a5da48e293da4f42) )
ROM_END

/* Crazy Taxi */
ROM_START( crzytaxi )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x8800000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD( "epr-21684.ic22",   0x0000000, 0x400000, CRC(f1de77b7) SHA1(4490b828534db6676b2d0129498fd7694eb9e5ff) )
	ROM_LOAD( "mpr-21671.ic1",   0x0800000, 0x800000, CRC(2d362137) SHA1(ed6eb45eadb784910eee44d0273534ab68ad6937) )
	ROM_LOAD( "mpr-21672.ic2",   0x1000000, 0x800000, CRC(72c7da8e) SHA1(0ed3d71c052a2cccbbf0f7b20e2ec688316c7247) )
	ROM_LOAD( "mpr-21673.ic3",   0x1800000, 0x800000, CRC(27481c0d) SHA1(08779e33eda1a45cb06319327cb4254dc3e4460f) )
	ROM_LOAD( "mpr-21674.ic4",   0x2000000, 0x800000, CRC(c2e2a98c) SHA1(9a40456ca025c2a6314cef705f7d147bbf95c0f0) )
	ROM_LOAD( "mpr-21675.ic5",   0x2800000, 0x800000, CRC(6b755510) SHA1(3fa967587e05c5bd45db3fe8a2cbc56f44166ef6) )
	ROM_LOAD( "mpr-21676.ic6",   0x3000000, 0x800000, CRC(f33d1f39) SHA1(b1d589a9ab7ec4988e63bfb458ef006308e1de70) )
	ROM_LOAD( "mpr-21677.ic7",   0x3800000, 0x800000, CRC(ab4dc61b) SHA1(d92ff434e7a2b9d3598f9d7004aa717b9bd21980) )
	ROM_LOAD( "mpr-21678.ic10",  0x5000000, 0x800000, CRC(297c778a) SHA1(67e5685cd03a3aaaac1c47f15c7b3f3e341d34b1) )
	ROM_LOAD( "mpr-21679.ic11",  0x5800000, 0x800000, CRC(6b540c4a) SHA1(9877c31b41110230182c0ee8d40753907981c7f9) )
	ROM_LOAD( "mpr-21680.ic12s", 0x6000000, 0x800000, CRC(e76f03f9) SHA1(ea20aa86d02a77315cca8cb6be75ca4ca9cc7484) )
	ROM_LOAD( "mpr-21681.ic13s", 0x6800000, 0x800000, CRC(e5dcde7d) SHA1(8a90d9fb4ce0d2ceb609fcf4c54cf5b55c266c50) )
	ROM_LOAD( "mpr-21682.ic14s", 0x7000000, 0x800000, CRC(54c0290e) SHA1(6e07ab6e95c29a2aabed0ba1a7af0d7d605e0309) )
	ROM_LOAD( "mpr-21683.ic15s", 0x7800000, 0x800000, CRC(ac8a27e0) SHA1(8e71d853a102dd6c164d5326e6d157ccfb8c7b36) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "crzytaxi-key.bin", 0, 4, CRC(3d36c13d) SHA1(746e82e7f1d6f4629dde00fb6b32943741b6ec70) )
ROM_END

/* Jambo! Safari */
ROM_START( jambo )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x8800000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD( "epr-22826a.ic22", 0x0000000, 0x400000, CRC(18f8f3bc) SHA1(417f2282c9970775e51b56d2eeb671a50ca293a7) )
	ROM_LOAD( "mpr-22818.ic1",  0x0800000, 0x800000, CRC(3a709e11) SHA1(e7dd71dd244e872c35595456bd428dd79a81f081) )
	ROM_LOAD( "mpr-22819.ic2",  0x1000000, 0x800000, CRC(57b2d565) SHA1(be5e6404c8187dc75cd6f033a36af413bf28bdee) )
	ROM_LOAD( "mpr-22820.ic3",  0x1800000, 0x800000, CRC(3284e16b) SHA1(bdf9249f19c0a444a9f00e831563e91c576a7cca) )
	ROM_LOAD( "mpr-22821.ic4",  0x2000000, 0x800000, CRC(5ca54154) SHA1(7bb1ba3fae71368145fd68d31bdce0588f641f78) )
	ROM_LOAD( "mpr-22822.ic5",  0x2800000, 0x800000, CRC(8bc0c4d5) SHA1(b250ddeaab904b15737f1348b62d7b3f11103609) )
	ROM_LOAD( "mpr-22823.ic6",  0x3000000, 0x800000, CRC(00c33e51) SHA1(c55646a146ed259e6c61fd912c93fa784b5e6910) )
	ROM_LOAD( "mpr-22824.ic7",  0x3800000, 0x800000, CRC(cc55304a) SHA1(e548d8de83469e5816c55dbbb00afbb894282fd6) )
	ROM_LOAD( "mpr-22825.ic8",  0x4000000, 0x800000, CRC(85bada10) SHA1(b6e15d8f1d6bca12ffa4816ed0393c04ca500fba) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "jambo-key.bin", 0, 4, CRC(e78e9ac8) SHA1(677d7f21c238e12362ad65d6496f285d688fc3a9) )
ROM_END

/* 18 Wheeler DELUXE (Rev A) */
ROM_START( 18wheelr )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0xa800000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD( "epr-22185.ic22",  0x0000000, 0x400000, CRC(219b29b0) SHA1(2f32caf3906fc1408fd8126a500e74c682ff20fa) )
	ROM_LOAD( "mpr-22164.ic1",   0x0800000, 0x800000, CRC(ca045315) SHA1(0c5becb5220659fa86f1e7901032f8c9a1329a51) )
	ROM_LOAD( "mpr-22165.ic2",   0x1000000, 0x800000, CRC(e43f4ba8) SHA1(768159bccae6a72c809e9f374538df304c92fbfe) )
	ROM_LOAD( "mpr-22166.ic3",   0x1800000, 0x800000, CRC(ea67edb1) SHA1(1214fdbfd8ce9f7a2b33e97f7c4e22ebb3eee988) )
	ROM_LOAD( "mpr-22167.ic4",   0x2000000, 0x800000, CRC(df6125e2) SHA1(7244278cf89b88dbc7b8b1e3a537bf0b3f521c3a) )
	ROM_LOAD( "mpr-22168.ic5",   0x2800000, 0x800000, CRC(8a919f9c) SHA1(28f1f9d8943e0a0b7bc186808cdab5d21d914e05) )
	ROM_LOAD( "mpr-22169.ic6",   0x3000000, 0x800000, CRC(a0fa7d68) SHA1(010fc87f0df3cf9e3d01a5ca4d4aa7e84728652d) )
	ROM_LOAD( "mpr-22170.ic7",   0x3800000, 0x800000, CRC(1f407049) SHA1(3c11b25168715d200f8d78a3db7bfc8cb3c29897) )
	ROM_LOAD( "mpr-22171.ic8",   0x4000000, 0x800000, CRC(03ce8dcd) SHA1(9bf1eb0a2628317bea5d8899e34f6f4363729c52) )
	ROM_LOAD( "mpr-22172.ic9",   0x4800000, 0x800000, CRC(c3e8c978) SHA1(96cbaa0f13e22365b04818cb5cad2ddc2027e38a) )
	ROM_LOAD( "mpr-22173.ic10",  0x5000000, 0x800000, CRC(3caec8fc) SHA1(88ee6b0a1735788570d0a6507eec14a31ebabb9a) )
	ROM_LOAD( "mpr-22174.ic11",  0x5800000, 0x800000, CRC(17245a27) SHA1(b3701155b1bbdbcbfb5ea686470c3c432d2573b7) )
	ROM_LOAD( "mpr-22175.ic12s", 0x6000000, 0x800000, CRC(4d984682) SHA1(60270d6caa3bbc0025a0c01cf4d7b10783216e0b) )
	ROM_LOAD( "mpr-22176.ic13s", 0x6800000, 0x800000, CRC(3ea2403f) SHA1(efde74c621a8fe17d8aa3a24da35e2ca6bc0bd9a) )
	ROM_LOAD( "mpr-22177.ic14s", 0x7000000, 0x800000, CRC(15514cbc) SHA1(0171d67560b8d72ca3f718dcce301acc60dee1fa) )
	ROM_LOAD( "mpr-22178.ic15s", 0x7800000, 0x800000, CRC(9ea0552f) SHA1(4b282110ef9f60f942518f3849acfff4a5faf4bd) )
	ROM_LOAD( "mpr-22179.ic16s", 0x8000000, 0x800000, CRC(6915c4e6) SHA1(b44d49edcfdc0f2958bf1a3856b09b5442e8f1a3) )
	ROM_LOAD( "mpr-22180.ic17s", 0x8800000, 0x800000, CRC(744c3a40) SHA1(56fba6ebc45d542ba6e4f4dd205194344f127ac2) )
	ROM_LOAD( "mpr-22181.ic18s", 0x9000000, 0x800000, CRC(5a39b68e) SHA1(0f81ed1116b1829262f320fc82f93df107b6f848) )
	ROM_LOAD( "mpr-22182.ic19s", 0x9800000, 0x800000, CRC(c5606c42) SHA1(5871104ff1c7acde0493e13b9a4d0abdf8a40728) )
	ROM_LOAD( "mpr-22183.ic20s", 0xa000000, 0x800000, CRC(776af308) SHA1(7d29cb4dce75d34c622549fea7e102868d0da60a) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "18wheelr-key.bin", 0, 4, CRC(046f97f5) SHA1(11eb5cffdd67d460718b63ddee8ff4dfa7d72314) )

	// JVS I/O board 837-13844, code is for a Z80 of unknown type (it's inside the big Sega ASIC)
	ROM_REGION( 0x20000, "jvsio", ROMREGION_ERASEFF)
	ROM_LOAD( "epr-21868.ic7", 0x000000, 0x010000, CRC(c306a51f) SHA1(7833b73dc34c4c62401a30637968f46b949ceac0) )
	// later version of the same I/O board (temporary, we'll handle this properly later)
	ROM_LOAD( "epr-22082.ic7", 0x010000, 0x010000, CRC(de26fc6c) SHA1(cf8ef7969770fff8697299c3e3152413b898a967) )

	// 18 Wheeler motor controller 838-13992, code is for a TMPZ84C015 which is Z80 compatible
	ROM_REGION( 0x10000, "motorio", ROMREGION_ERASEFF)
	ROM_LOAD( "epr-23000.ic8", 0x000000, 0x010000, CRC(e3b162f7) SHA1(52c7ad759c3c4a3148764e14d77ba5006bc8af48) )
ROM_END

/* 18 Wheeler (standard) */
ROM_START( 18wheels )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0xa800000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD( "epr-23298.ic22",  0x0000000, 0x400000, CRC(bfaf8302) SHA1(e4d4d1aadd84fc03a45e154001cd9262eb6be585) )
	ROM_LOAD( "mpr-22164.ic1",   0x0800000, 0x800000, CRC(ca045315) SHA1(0c5becb5220659fa86f1e7901032f8c9a1329a51) )
	ROM_LOAD( "mpr-22165.ic2",   0x1000000, 0x800000, CRC(e43f4ba8) SHA1(768159bccae6a72c809e9f374538df304c92fbfe) )
	ROM_LOAD( "mpr-22166.ic3",   0x1800000, 0x800000, CRC(ea67edb1) SHA1(1214fdbfd8ce9f7a2b33e97f7c4e22ebb3eee988) )
	ROM_LOAD( "mpr-22167.ic4",   0x2000000, 0x800000, CRC(df6125e2) SHA1(7244278cf89b88dbc7b8b1e3a537bf0b3f521c3a) )
	ROM_LOAD( "mpr-22168.ic5",   0x2800000, 0x800000, CRC(8a919f9c) SHA1(28f1f9d8943e0a0b7bc186808cdab5d21d914e05) )
	ROM_LOAD( "mpr-22169.ic6",   0x3000000, 0x800000, CRC(a0fa7d68) SHA1(010fc87f0df3cf9e3d01a5ca4d4aa7e84728652d) )
	ROM_LOAD( "mpr-22170.ic7",   0x3800000, 0x800000, CRC(1f407049) SHA1(3c11b25168715d200f8d78a3db7bfc8cb3c29897) )
	ROM_LOAD( "mpr-22171.ic8",   0x4000000, 0x800000, CRC(03ce8dcd) SHA1(9bf1eb0a2628317bea5d8899e34f6f4363729c52) )
	ROM_LOAD( "mpr-22172.ic9",   0x4800000, 0x800000, CRC(c3e8c978) SHA1(96cbaa0f13e22365b04818cb5cad2ddc2027e38a) )
	ROM_LOAD( "mpr-22173.ic10",  0x5000000, 0x800000, CRC(3caec8fc) SHA1(88ee6b0a1735788570d0a6507eec14a31ebabb9a) )
	ROM_LOAD( "mpr-22174.ic11",  0x5800000, 0x800000, CRC(17245a27) SHA1(b3701155b1bbdbcbfb5ea686470c3c432d2573b7) )
	ROM_LOAD( "mpr-22175.ic12s", 0x6000000, 0x800000, CRC(4d984682) SHA1(60270d6caa3bbc0025a0c01cf4d7b10783216e0b) )
	ROM_LOAD( "mpr-22176.ic13s", 0x6800000, 0x800000, CRC(3ea2403f) SHA1(efde74c621a8fe17d8aa3a24da35e2ca6bc0bd9a) )
	ROM_LOAD( "mpr-22177.ic14s", 0x7000000, 0x800000, CRC(15514cbc) SHA1(0171d67560b8d72ca3f718dcce301acc60dee1fa) )
	ROM_LOAD( "mpr-22178.ic15s", 0x7800000, 0x800000, CRC(9ea0552f) SHA1(4b282110ef9f60f942518f3849acfff4a5faf4bd) )
	ROM_LOAD( "mpr-22179.ic16s", 0x8000000, 0x800000, CRC(6915c4e6) SHA1(b44d49edcfdc0f2958bf1a3856b09b5442e8f1a3) )
	ROM_LOAD( "mpr-22180.ic17s", 0x8800000, 0x800000, CRC(744c3a40) SHA1(56fba6ebc45d542ba6e4f4dd205194344f127ac2) )
	ROM_LOAD( "mpr-22181.ic18s", 0x9000000, 0x800000, CRC(5a39b68e) SHA1(0f81ed1116b1829262f320fc82f93df107b6f848) )
	ROM_LOAD( "mpr-22182.ic19s", 0x9800000, 0x800000, CRC(c5606c42) SHA1(5871104ff1c7acde0493e13b9a4d0abdf8a40728) )
	ROM_LOAD( "mpr-22183.ic20s", 0xa000000, 0x800000, CRC(776af308) SHA1(7d29cb4dce75d34c622549fea7e102868d0da60a) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "18wheelr-key.bin", 0, 4, CRC(046f97f5) SHA1(11eb5cffdd67d460718b63ddee8ff4dfa7d72314) )

	// JVS I/O board 837-13844, code is for a Z80 of unknown type (it's inside the big Sega ASIC)
	ROM_REGION( 0x20000, "jvsio", ROMREGION_ERASEFF)
	ROM_LOAD( "epr-21868.ic7", 0x000000, 0x010000, CRC(c306a51f) SHA1(7833b73dc34c4c62401a30637968f46b949ceac0) )
	// later version of the same I/O board (temporary, we'll handle this properly later)
	ROM_LOAD( "epr-22082.ic7", 0x010000, 0x010000, CRC(de26fc6c) SHA1(cf8ef7969770fff8697299c3e3152413b898a967) )

	// 18 Wheeler motor controller 838-13992, code is for a TMPZ84C015 which is Z80 compatible
	ROM_REGION( 0x10000, "motorio", ROMREGION_ERASEFF)
	ROM_LOAD( "epr-23000.ic8", 0x000000, 0x010000, CRC(e3b162f7) SHA1(52c7ad759c3c4a3148764e14d77ba5006bc8af48) )
ROM_END

/* Sega Strike Fighter */
ROM_START( sstrkfgt )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0xb000000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD( "epr-23323a.ic22", 0x0000000, 0x400000, CRC(f3fd304b) SHA1(012eefebd857200195d9e2d80c24a793b258d7e2) )
	ROM_LOAD( "mpr-23302.ic1",   0x0800000, 0x800000, CRC(3429321b) SHA1(6fc1be2ca71a690a2ceca9dc968183a1222177f7) )
	ROM_LOAD( "mpr-23303.ic2",   0x1000000, 0x800000, CRC(f7b3ada2) SHA1(d2caea852241cb0d91243d84d1c5523dfddac721) )
	ROM_LOAD( "mpr-23304.ic3",   0x1800000, 0x800000, CRC(3bf145e9) SHA1(a000e135ad640472418de418b92dbdb83dcf872b) )
	ROM_LOAD( "mpr-23305.ic4",   0x2000000, 0x800000, CRC(924ee9fd) SHA1(dd56f8cd7e9dda87968abb810694bddeeb31db5c) )
	ROM_LOAD( "mpr-23306.ic5",   0x2800000, 0x800000, CRC(4021e805) SHA1(75988ff8d710da6d90608cef87fc8b4408a617fb) )
	ROM_LOAD( "mpr-23307.ic6",   0x3000000, 0x800000, CRC(090c1812) SHA1(e3e32d5c1f42191e188f91dbd4a753030894aa6f) )
	ROM_LOAD( "mpr-23308.ic7",   0x3800000, 0x800000, CRC(f23d2198) SHA1(9775796a388ab903102126fb190867a0d192903e) )
	ROM_LOAD( "mpr-23309.ic8",   0x4000000, 0x800000, CRC(0d6a7c9d) SHA1(0df846289d598efdf5605ca8e09758eb8b5878f9) )
	ROM_LOAD( "mpr-23310.ic9",   0x4800000, 0x800000, CRC(f4ec4baa) SHA1(77e2ea1c5747ced4951286142bd429780f9d4115) )
	ROM_LOAD( "mpr-23311.ic10",  0x5000000, 0x800000, CRC(a1467573) SHA1(cf38527b0e812ba90e7402aa53e4557ce756cf43) )
	ROM_LOAD( "mpr-23312.ic11",  0x5800000, 0x800000, CRC(9b0ae703) SHA1(35f0e3cdbc206b91dad4a97feb3c533bc12a77f1) )
	ROM_LOAD( "mpr-23313.ic12s", 0x6000000, 0x800000, CRC(d309fea9) SHA1(2ba2da81976126f0a79b066d855706d800279150) )
	ROM_LOAD( "mpr-23314.ic13s", 0x6800000, 0x800000, CRC(0aeedeac) SHA1(5e5086a7a51a9576e786911a2c7f4b509d5bc2f4) )
	ROM_LOAD( "mpr-23315.ic14s", 0x7000000, 0x800000, CRC(88f22650) SHA1(3425433d233b458ae73e30cc0c7d25fca2a9d589) )
	ROM_LOAD( "mpr-23316.ic15s", 0x7800000, 0x800000, CRC(38ff3a9d) SHA1(56978183fe61fd2ad59ab2979cb61fbf2cde07e6) )
	ROM_LOAD( "mpr-23317.ic16s", 0x8000000, 0x800000, CRC(d6d45776) SHA1(102963243f6e127d4c35d150eeb09aa99a3738d4) )
	ROM_LOAD( "mpr-23318.ic17s", 0x8800000, 0x800000, CRC(5f33207e) SHA1(6eceb6bb9171da8634fcba9dd7409794447fe069) )
	ROM_LOAD( "mpr-23319.ic18s", 0x9000000, 0x800000, CRC(ff42857a) SHA1(adbc025c4e02ad3b15ead9340aee494c16005ad5) )
	ROM_LOAD( "mpr-23320.ic19s", 0x9800000, 0x800000, CRC(5ec75a45) SHA1(696e5d14678c794dec67246507bd580f7e5b5043) )
	ROM_LOAD( "mpr-23321.ic20s", 0xa000000, 0x800000, CRC(018627d4) SHA1(2519f39ad046d14f602648fed39bc3719185b55e) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "sstrkfgt-key.bin", 0, 4, CRC(1884e4ee) SHA1(14ec026ed32825252987fed350f19f6e2565aa4d) )
ROM_END


/* Sega Tetris */
ROM_START( sgtetris )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x3800000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD( "epr-22909.ic22", 0x000000, 0x200000, CRC(486b2fdf) SHA1(da54fec42b7ac16f73b2b9f166b9b2ab45426fd7) )
	ROM_LOAD( "mpr-22910.ic1", 0x0800000, 0x800000, CRC(7968b67e) SHA1(4a83c22a30b3a3ce7d7167f703a11b78d3f6cea6) )
	ROM_LOAD( "mpr-22911.ic2", 0x1000000, 0x800000, CRC(4014aa6a) SHA1(86a9bd852c9fff70c0b902b7014c136a1d82e9a4) )
	ROM_LOAD( "mpr-22912.ic3", 0x1800000, 0x800000, CRC(67667a56) SHA1(89f3cab6c5db2f6ecac4e6a0dee085fa39cb5cbb) )
	ROM_LOAD( "mpr-22913.ic4", 0x2000000, 0x800000, CRC(1fbdc41a) SHA1(eb8b9577b7677b9e9aec05ae950dee516ae15bf5) )
	ROM_LOAD( "mpr-22914.ic5", 0x2800000, 0x800000, CRC(77844b60) SHA1(65d71febb8a160d00778ac7b53e082253cad9834) )
	ROM_LOAD( "mpr-22915.ic6", 0x3000000, 0x800000, CRC(e48148ac) SHA1(c1273353eeaf9bb6b185f133281d7d04271bc895) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "sgtetris-key.bin", 0, 4, CRC(03bf7ea1) SHA1(0229c1f9a6eca65b09e3571768a1fbf3c1dad6b1) )
ROM_END

/*

SYSTEMID: NAOMI
JAP: SLASHOUT JAPAN VERSION
USA: SLASHOUT USA VERSION
EXP: SLASHOUT EXPORT VERSION

NO.  Type   Byte    Word
IC22 32M    0000    0000
IC1  64M    D1BF    FB18
IC2  64M    1F98    4295
IC3  64M    5F61    67E3
IC4  64M    C6A4    449B
IC5  64M    BB2A    58AB
IC6  64M    60B2    5262
IC7  64M    178B    3705
IC8  64M    E4B9    FF46
IC9  64M    D4FC    2273
IC10 64M    6BA5    8087
IC11 64M    7DBA    A143
IC12 64M    B708    0C61
IC13 64M    0C4A    8DF0
IC14 64M    B2FF    A057
IC15 64M    60DB    3D06
IC16 64M    B5EA    4965
IC17 64M    6586    1F3F

*/

ROM_START( slasho )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x9000000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD("epr-23341.ic22", 0x0000000, 0x0400000, CRC(477fa123) SHA1(d2474766dcd0b0e5fe317a858534829eb1c26789) )
	ROM_RELOAD( 0x400000, 0x400000)
	ROM_LOAD("mpr-23324.ic1",  0x0800000, 0x0800000, CRC(8624493a) SHA1(4fe940a889619f2a75c45e15efb2b8ed9020bc55) )
	ROM_LOAD("mpr-23325.ic2",  0x1000000, 0x0800000, CRC(f952d0d4) SHA1(4b5403b98bf977c1e3a045619e1eddb4e4ab69c7) )
	ROM_LOAD("mpr-23326.ic3",  0x1800000, 0x0800000, CRC(6c5ce16e) SHA1(110b5d536557ab6610a7c32db2e6e46901da9579) )
	ROM_LOAD("mpr-23327.ic4",  0x2000000, 0x0800000, CRC(1b3d02a0) SHA1(3e02a0cb3d945e5d6cea03236d6571d45a7afd51) )
	ROM_LOAD("mpr-23328.ic5",  0x2800000, 0x0800000, CRC(50053662) SHA1(3ced87ee533fd7a32d64c41f1fcbde9c648ab188) )
	ROM_LOAD("mpr-23329.ic6",  0x3000000, 0x0800000, CRC(96148e80) SHA1(c0f30556395edb9a7558006e89d6adc2f6bdc048) )
	ROM_LOAD("mpr-23330.ic7",  0x3800000, 0x0800000, CRC(15f2f9a1) SHA1(9cea71b6f6466ccd840218f5dcb09ea7525208d8) )
	ROM_LOAD("mpr-23331.ic8",  0x4000000, 0x0800000, CRC(a084ab51) SHA1(1f5c863012004bbeefc82b172a92011a175428a6) )
	ROM_LOAD("mpr-23332.ic9",  0x4800000, 0x0800000, CRC(50539e17) SHA1(38ec16a868c892e177fbb45be563e1b649956550) )
	ROM_LOAD("mpr-23333.ic10", 0x5000000, 0x0800000, CRC(29891831) SHA1(f318bca11ac5eb24b32d5b910a596280221a44ab) )
	ROM_LOAD("mpr-23334.ic11", 0x5800000, 0x0800000, CRC(c1ad0614) SHA1(e38ff316da889eb029d0a9348f6b2284f3a36f29) )
	ROM_LOAD("mpr-23335.ic12s",0x6000000, 0x0800000, CRC(faeb25ed) SHA1(623f3f78c94ba44e77491c18a6521a19b1101a67) )
	ROM_LOAD("mpr-23336.ic13s",0x6800000, 0x0800000, CRC(63589d0f) SHA1(53770cc1268892e8cdb76b6edf2fb39e8b605554) )
	ROM_LOAD("mpr-23337.ic14s",0x7000000, 0x0800000, CRC(2bc46263) SHA1(38ec579768ac37ed3ad21911b1970241906af8ea) )
	ROM_LOAD("mpr-23338.ic15s",0x7800000, 0x0800000, CRC(323e4db2) SHA1(c5484589c1613110faef6cf8b8f4def8867a8226) )
	ROM_LOAD("mpr-23339.ic16s",0x8000000, 0x0800000, CRC(fd8c2736) SHA1(34ae1a4e35b4aac6666719fb4fc0959bd64ff3d6) )
	ROM_LOAD("mpr-23340.ic17s",0x8800000, 0x0800000, CRC(001604f8) SHA1(615ec027d383d44d4aadb1175be6320e4139d7d1) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "slasho-key.bin", 0, 4, CRC(78e31e35) SHA1(a0b06cb7fbb0b2f1734831fc3e6a8315c0e78edd) )
ROM_END


/*

SYSTEMID: NAOMI
JAP: MOERO JUSTICE GAKUEN  JAPAN
USA: PROJECT JUSTICE  USA
EXP: PROJECT JUSTICE  EXPORT

NO.     Type    Byte    Word
IC22    32M     0000    0000
IC1     64M     3E87    5491
IC2     64M     2789    9802
IC3     64M     60E7    E775
IC4     64M     36F4    9353
IC5     64M     31B6    CEF6
IC6     64M     3F79    7B58
IC7     64M     620C    A31F
IC8     64M     A093    160C
IC9     64M     4DD9    4184
IC10    64M     AF3F    C64A
IC11    64M     0EE1    A0C2
IC12    64M     2EF9    E0A3
IC13    64M     72A5    3156
IC14    64M     D414    B896
IC15    64M     7BCE    3A7A
IC16    64M     E371    962D
IC17    64M     E813    E342
IC18    64M     D2B8    3989
IC19    64M     3A4B    4614
IC20    64M     11B0    9921
IC21    64M     698C    7A39

Serial: BCLE-01A2130

*/

ROM_START( pjustic )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0xb800000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD("epr-23548a.ic22", 0x0000000, 0x0400000,  CRC(f4ccf1ec) SHA1(97485b2a4b9452ffeea2501f42d20d718410e716) )
	ROM_LOAD( "mpr-23537.ic1",  0x0800000, 0x1000000, CRC(a2462770) SHA1(2d06f2efb686b2c45e5cc0b0776ba5fb1d392951) )
	ROM_LOAD( "mpr-23538.ic2",  0x1800000, 0x1000000, CRC(e4480832) SHA1(281700b10bd6b29e4d33b5230d085f9cc102fa01) )
	ROM_LOAD( "mpr-23539.ic3",  0x2800000, 0x1000000, CRC(97e3f7f5) SHA1(89ad30782ba148777ce3aad2d41e9dfda2dd0c5c) )
	ROM_LOAD( "mpr-23540.ic4",  0x3800000, 0x1000000, CRC(b9e92d21) SHA1(a9b465e83ecfbf47168f83ad4ae8ed4b802345ac) )
	ROM_LOAD( "mpr-23541.ic5",  0x4800000, 0x1000000, CRC(95b8a9c6) SHA1(2c9df93d9f599cf01d895a37d03ba0d86b9b3033) )
	ROM_LOAD( "mpr-23542.ic6",  0x5800000, 0x1000000, CRC(dfd490f5) SHA1(13fe0a11a75f1a7ebfe40433833241e656bb1511) )
	ROM_LOAD( "mpr-23543.ic7",  0x6800000, 0x1000000, CRC(66847ebd) SHA1(853d4fc7e53ac7b19b9ba616f756eb8a8fcd242d) )
	ROM_LOAD( "mpr-23544.ic8",  0x7800000, 0x1000000, CRC(d1f5b460) SHA1(f0789630871d728113abacceff21c6328a9fa9fc) )
	ROM_LOAD( "mpr-23545.ic9",  0x8800000, 0x1000000, CRC(60bd692f) SHA1(37b508f4a821d832eafff81574e7df3fe1c729f8) )
	ROM_LOAD( "mpr-23546.ic10", 0x9800000, 0x1000000, CRC(85db2248) SHA1(37845c269a2e65ee6181a8e7500c2e7dd9b2e343) )
	ROM_LOAD( "mpr-23547.ic11", 0xa800000, 0x1000000, CRC(18b369c7) SHA1(b61cb3fda8cc685865684f7afc7dad0b29d93ca5) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "pjustic-key.bin", 0, 4, CRC(4a2bcbaa) SHA1(a9434482498f97605f76d08f749685452651a753) )
ROM_END

/*

SYSTEMID: NAOMI
JAP: POWER STONE JAPAN
USA: POWER STONE USA
EXP: POWER STONE EURO

NO. Type    Byte    Word
IC22    16M 0000    0000
IC1 64M 0258    45D8
IC2 64M 0DF2    0810
IC3 64M 5F93    9FAF
IC4 64M 05E0    C80F
IC5 64M F023    3F68
IC6 64M 941E    F563
IC7 64M 374E    46F6
IC8 64M C529    0501

prot

*/

ROM_START( pstone )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x4800000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD("epr-21597.ic22",0x0000000, 0x0200000, CRC(62c7acc0) SHA1(bb61641a7f3650757132cde379447bdc9bd91c78) )
	ROM_LOAD("mpr-21589.ic1", 0x0800000, 0x0800000, CRC(2fa66608) SHA1(144bda75f892a1e4dbd8332439e9e44fad1d0695) )
	ROM_LOAD("mpr-21590.ic2", 0x1000000, 0x0800000, CRC(6341b399) SHA1(d123b6a3eb7c4800950cc5849d748b0edafabc7d) )
	ROM_LOAD("mpr-21591.ic3", 0x1800000, 0x0800000, CRC(7f2d99aa) SHA1(00f9ae67be0d7229c37479b6dc0ed5816035fd98) )
	ROM_LOAD("mpr-21592.ic4", 0x2000000, 0x0800000, CRC(6ebe3b25) SHA1(c7dec77d55b0fcf1d230311b24553581a90a7d22) )
	ROM_LOAD("mpr-21593.ic5", 0x2800000, 0x0800000, CRC(84366f3e) SHA1(c61985f627813db2e16182e437ab4a69d5253c9f) )
	ROM_LOAD("mpr-21594.ic6", 0x3000000, 0x0800000, CRC(ddfa0467) SHA1(e758eae50035d5f18d99dbed728513e306d9566f) )
	ROM_LOAD("mpr-21595.ic7", 0x3800000, 0x0800000, CRC(7ab218f7) SHA1(c5c022e63f926cce09d49331647cde20e8e42ab3) )
	ROM_LOAD("mpr-21596.ic8", 0x4000000, 0x0800000, CRC(f27dbdc5) SHA1(d54717d62897546968de2f049239f68bee49bdd8) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "pstone-key.bin", 0, 4, CRC(7386d0de) SHA1(72f6b552440e470344f707e349d100d9ba21b69d) )
ROM_END

/*

SYSTEMID: NAOMI
JAP: POWER STONE 2 JAPAN
USA: POWER STONE 2 USA
EXP: POWER STONE 2 EURO

NO.     Type    Byte    Word
IC22    32M     0000    0000
IC1     64M     04FF    B3D4
IC2     64M     52D4    0BF0
IC3     64M     5273    0EB8
IC4     64M     B39A    21F5
IC5     64M     53CB    6540
IC6     64M     0AC8    74ED
IC7     64M     D05A    EB30
IC8     64M     8217    4E66
IC9     64M     193C    6851

Serial: BBJE-01A1613

*/

ROM_START( pstone2 )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x5000000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD("epr-23127.ic22", 0x0000000, 0x0400000,  CRC(185761d6) SHA1(8c91b594dd59313d249c9da7b39dee21d3c9082e) )
	ROM_RELOAD( 0x400000, 0x400000)
	ROM_LOAD("mpr-23118.ic1", 0x0800000, 0x0800000, CRC(c69f3c3c) SHA1(e96ad24473197f8581f5e4398244b9b76957bfdd) )
	ROM_LOAD("mpr-23119.ic2", 0x1000000, 0x0800000, CRC(a80d444d) SHA1(a7d2a5831412134a26ba37bf83e5ce38eb9f3928) )
	ROM_LOAD("mpr-23120.ic3", 0x1800000, 0x0800000, CRC(c285dd64) SHA1(e64507caedb9f312ab291b41b8d7fe8922eb434e) )
	ROM_LOAD("mpr-23121.ic4", 0x2000000, 0x0800000, CRC(1f3f6505) SHA1(da5eb3b9b5c85f5f0b4afe0c0ee8d034108300a2) )
	ROM_LOAD("mpr-23122.ic5", 0x2800000, 0x0800000, CRC(5e403a12) SHA1(e71c15e63c30e60b6db1fcd2841f66490f31579a) )
	ROM_LOAD("mpr-23123.ic6", 0x3000000, 0x0800000, CRC(4b71078b) SHA1(f3ed39402f585ae5cf6f8987bf6be6c6d46eafa1) )
	ROM_LOAD("mpr-23124.ic7", 0x3800000, 0x0800000, CRC(508c0207) SHA1(e50d97a17cdd6771fbc63a254a4d638e7daa8f57) )
	ROM_LOAD("mpr-23125.ic8", 0x4000000, 0x0800000, CRC(b9938bbc) SHA1(d55d7adecb5a5a4a276a5a17c12808085d980fd9) )
	ROM_LOAD("mpr-23126.ic9", 0x4800000, 0x0800000, CRC(fbb0325b) SHA1(21b965519d7508d84344641d43e8af2c3ca29ba4) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "pstone2-key.bin", 0, 4, CRC(3858144b) SHA1(f5fb9157b38969c986f8abbe8ed3f95e788e45c0) )
ROM_END


/*

SYSTEMID: NAOMI
JAP: OUTTRIGGER     JAPAN
USA: OUTTRIGGER     USA
EXP: OUTTRIGGER     EXPORT

NO.     Type    Byte    Word
IC22    32M     0000    0000
IC1     64M     362E    D34B
IC2     64M     4EF4    FF8D
IC3     64M     5E77    9052
IC4     64M     E123    41B3
IC5     64M     43A0    58D4
IC6     64M     C946    D3EE
IC7     64M     5313    3F17
IC8     64M     2591    FEB7
IC9     64M     CBA3    E150
IC10    64M     2639    D291
IC11    64M     3A96    86EA
IC12    64M     8586    3ED5
IC13    64M     9028    E59C
IC14    64M     8A42    26E2
IC15    64M     98C4    1618
IC16    64M     122B    8C85
IC17    64M     3D5E    F9B0
IC18    64M     1EFA    490E
IC19    64M     9F22    6F77

Serial (from 2 carts): BAZE-01A0288
                       BAZE-02A0217

*/

ROM_START( otrigger )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0xa000000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD("epr-22163.ic22", 0x0000000, 0x0400000, CRC(3bdafb6a) SHA1(c4c5a4ba94d85c4353df22d70bb08be67e9c22c3) )
	ROM_RELOAD( 0x400000, 0x400000)
	ROM_LOAD("mpr-22142.ic1",  0x0800000, 0x0800000, CRC(5b45fa35) SHA1(7d3fbecc6f0dce2b13bfb21ed68f44632b91b94b) )
	ROM_LOAD("mpr-22143.ic2",  0x1000000, 0x0800000, CRC(b43c4d6d) SHA1(77e0b37ca3ee94b7f77d88ccb14bd0469a76aac0) )
	ROM_LOAD("mpr-22144.ic3",  0x1800000, 0x0800000, CRC(e78581af) SHA1(d1fe4da3f16dd5ebc7d9eaa092de1e16ec9c3321) )
	ROM_LOAD("mpr-22145.ic4",  0x2000000, 0x0800000, CRC(2b6274ea) SHA1(89165cf84ebb02e99163624c6d31da38aeec000e) )
	ROM_LOAD("mpr-22146.ic5",  0x2800000, 0x0800000, CRC(c24eb03f) SHA1(2f4b720b4ab106f891f4469b6e93a9979b1c1061) )
	ROM_LOAD("mpr-22147.ic6",  0x3000000, 0x0800000, CRC(578e36fd) SHA1(f39f74b046efbff7e7baf70effdd368605da496f) )
	ROM_LOAD("mpr-22148.ic7",  0x3800000, 0x0800000, CRC(e6053373) SHA1(e7bafaffeac9b6851a3fce060be21e8be8eaa71e) )
	ROM_LOAD("mpr-22149.ic8",  0x4000000, 0x0800000, CRC(cc86691b) SHA1(624958bc07eef5fac98642e9acd460cd5fe0c815) )
	ROM_LOAD("mpr-22150.ic9",  0x4800000, 0x0800000, CRC(f585d41d) SHA1(335df3d3f2631e5c03c39465cd702b77ce3f9717) )
	ROM_LOAD("mpr-22151.ic10", 0x5000000, 0x0800000, CRC(aae31a4b) SHA1(1472e477c2c6b89ca03824838757bdf20efbdf45) )
	ROM_LOAD("mpr-22152.ic11", 0x5800000, 0x0800000, CRC(5ed2c5ea) SHA1(2b9237eda566ccb87b4914db61a03e2c9035a280) )
	ROM_LOAD("mpr-22153.ic12s",0x6000000, 0x0800000, CRC(16630b85) SHA1(10e926c0d13270b5bf99d7456fe63baafc2df56a) )
	ROM_LOAD("mpr-22154.ic13s",0x6800000, 0x0800000, CRC(30a2d60b) SHA1(6431b2d4e5106e25e5517707c9667bcd714f43ac) )
	ROM_LOAD("mpr-22155.ic14s",0x7000000, 0x0800000, CRC(163993a5) SHA1(351a626a0dc9a3030b10fc0b822075f3010fdc05) )
	ROM_LOAD("mpr-22156.ic15s",0x7800000, 0x0800000, CRC(37720b4f) SHA1(bd60beadb0081ed20610c3988577bbf37bfdab07) )
	ROM_LOAD("mpr-22157.ic16s",0x8000000, 0x0800000, CRC(dfd6fa83) SHA1(e0dc9606f5521af16c29a30378e81843c8dbc188) )
	ROM_LOAD("mpr-22158.ic17s",0x8800000, 0x0800000, CRC(f5d96fe9) SHA1(d5d0ac3d6b7c9b851a18b22d5fb599710c684a76) )
	ROM_LOAD("mpr-22159.ic18s",0x9000000, 0x0800000, CRC(f8b5e99d) SHA1(bb174a6a80967d0ff05c3a7512e4f0f9c921d130) )
	ROM_LOAD("mpr-22160.ic19s",0x9800000, 0x0800000, CRC(579eef4e) SHA1(bfcabd57f623647053afcedcabfbc74e5736819f) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "otrigger-key.bin", 0, 4, CRC(79ebd41a) SHA1(3545dacdeac66b46e8706565f0eca784291040d1) )
ROM_END

/*

SYSTEMID: NAOMI
JAP: AH! MY GODDESS QUIZ GAME--
USA: AH! MY GODDESS QUIZ GAME--
EXP: AH! MY GODDESS QUIZ GAME--

*/

ROM_START( qmegamis )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x9000200, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD("epr-23227.ic11", 0x0000000, 0x0400000, CRC(3f76087e) SHA1(664d28ef95394590b186e7badaf96ddaf781c104) ) //ic 22
	ROM_RELOAD( 0x400000, 0x400000)
	/*ic 1 empty */
	ROM_LOAD32_WORD("mpr-23211.ic17", 0x1000000, 0x0800000, CRC(a46670e7) SHA1(dbcb72fdf444f07ce986af329e7ff2cb42721729) ) //ic 2
	ROM_LOAD32_WORD("mpr-23212.ic18", 0x1000002, 0x0800000, CRC(5e6839d5) SHA1(678c706f5c1eee65b32d9455ca4d0803c38349bd) ) //ic 3
	ROM_LOAD32_WORD("mpr-23213.ic19", 0x2000000, 0x0800000, CRC(98e5e2c1) SHA1(1d5338c625fcd979dd841c3e5de09a3bd3d239b6) ) //ic 4
	ROM_LOAD32_WORD("mpr-23214.ic20", 0x2000002, 0x0800000, CRC(37cfdd37) SHA1(e9097af91a164b6ffeed98008e85f4d4f00894df) ) //ic 5
	ROM_LOAD32_WORD("mpr-23215.ic21", 0x3000000, 0x0800000, CRC(f0d97107) SHA1(35422cd3238f516243fa6d1f282d802ff4f4ab17) ) //ic 6
	ROM_LOAD32_WORD("mpr-23216.ic22", 0x3000002, 0x0800000, CRC(68a9363f) SHA1(ca9a015f18041bff1a0c57a61a50143c42115f9d) ) //ic 7
	ROM_LOAD32_WORD("mpr-23217.ic23", 0x4000000, 0x0800000, CRC(1b49de82) SHA1(07b115d8e66f02fe3f184c23353fb11452dcb2b4) ) //ic 8
	ROM_LOAD32_WORD("mpr-23218.ic24", 0x4000002, 0x0800000, CRC(2c2c1e42) SHA1(ac8b1d580a8aaef184415ac4572eb9b2d5f37cf8) ) //ic 9
	ROM_LOAD32_WORD("mpr-23219.ic25", 0x5000000, 0x0800000, CRC(62957622) SHA1(ca7e2cd009fb38db3c25896ef8206350a7221fc8) ) //ic 10
	ROM_LOAD32_WORD("mpr-23220.ic26", 0x5000002, 0x0800000, CRC(70c2bea3) SHA1(3fe1d806358a35eced1f1c3a83e3593d92c3cf52) ) //ic 11
	ROM_LOAD32_WORD("mpr-23221.ic27", 0x6000000, 0x0800000, CRC(eb6a522e) SHA1(a7928d2296d67f7913d6582bdb6cd58b09d01673) ) //ic 12
	ROM_LOAD32_WORD("mpr-23222.ic28", 0x6000002, 0x0800000, CRC(f7d932bd) SHA1(e2b53fd30af5f45160aa988e3a80cee9330f0deb) ) //ic 13
	ROM_LOAD32_WORD("mpr-23223.ic29", 0x7000000, 0x0800000, CRC(2f8c15e0) SHA1(55c8554404263b629172d30dbb240104ab352c0f) ) //ic 14
	ROM_LOAD32_WORD("mpr-23224.ic30", 0x7000002, 0x0800000, CRC(bed270e1) SHA1(342199ac5903681f2bfdb9dfd57ce06202f14685) ) //ic 15
	ROM_LOAD32_WORD("mpr-23225.ic31", 0x8000000, 0x0800000, CRC(ea558614) SHA1(b7dfe5598639a8e59e3cbbee38b1d9a1d8e022ea) ) //ic 16
	ROM_LOAD32_WORD("mpr-23226.ic32", 0x8000002, 0x0800000, CRC(cd5da506) SHA1(2e76c8892c1d389b0f12a0046213f43d2ab07d78) ) //ic 17

	ROM_COPY( "rom_board", 0x1400000, 0x0800000, 0x0800000 ) // mirror data for IC1 (no test on this game, but layout is the same as gram2000)

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "qmegamis-key.bin", 0, 4, CRC(b08650c0) SHA1(9e6b0fac6fb05209da9e01bb1a5437949d218078) )
ROM_END

/*

SYSTEMID: NAOMI
JAP: SAMBA DE AMIGO
USA: SAMBADEAMIGO
EXP: SAMBADEAMIGO

NO.     Type    Byte    Word
IC22    32M     0000    0000
IC1     64M     B1FA    1BE9
IC2     64M     51FD    0C32
IC3     64M     8AA0    6E7A
IC4     64M     3B30    E31D
IC5     64M     D604    FBE3
IC6     64M     1D51    FF2D
IC7     64M     EE89    720D
IC8     64M     0551    7046
IC9     64M     6883    6427
IC10    64M     70E5    CEC3
IC11    64M     E70E    0C63
IC12    64M     0FD0    B1F8
IC13    64M     2D48    6B19
IC14    64M     CBFF    F163
IC15    64M     10D1    E09D
IC16    64M     A10B    DDB4

*/

ROM_START( samba )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x8800000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD("epr-22966b.ic22",0x0000000, 0x0400000, CRC(893116b8) SHA1(35cb4f40690ff21af5ab7cc5adbc53228d6fb0b3) )
	ROM_LOAD("mpr-22950.ic1",  0x0800000, 0x0800000, CRC(16dee15c) SHA1(b46849e492756ff406bf8956303472255fcf55a5) )
	ROM_LOAD("mpr-22951.ic2",  0x1000000, 0x0800000, CRC(f509496f) SHA1(41281576f7d58c8ede9c0a89bfd46a98d5b97033) )
	ROM_LOAD("mpr-22952.ic3",  0x1800000, 0x0800000, CRC(fb9b3ef0) SHA1(e9d44b673c273e97445a12186496a0594e291542) )
	ROM_LOAD("mpr-22953.ic4",  0x2000000, 0x0800000, CRC(07207ce0) SHA1(b802bb4e78f3737a4e333f819b9a4e0249037288) )
	ROM_LOAD("mpr-22954.ic5",  0x2800000, 0x0800000, CRC(c8e797d1) SHA1(fadbd1e24882787634229003245293ce79ba2617) )
	ROM_LOAD("mpr-22955.ic6",  0x3000000, 0x0800000, CRC(064ef007) SHA1(8325f9aa537ce329e71dce2b588a3d4fc176c37b) )
	ROM_LOAD("mpr-22956.ic7",  0x3800000, 0x0800000, CRC(fe8f2964) SHA1(3a33162f797cd93b7dbb313b531215e340719110) )
	ROM_LOAD("mpr-22957.ic8",  0x4000000, 0x0800000, CRC(74842c01) SHA1(b02884925270edb66831ab502a0aa2f9430adc9f) )
	ROM_LOAD("mpr-22958.ic9",  0x4800000, 0x0800000, CRC(b1ead447) SHA1(06b848eb7f592763768050a1ae82b4cac9499684) )
	ROM_LOAD("mpr-22959.ic10", 0x5000000, 0x0800000, CRC(d32d7983) SHA1(86a9e5eae4598b6998f0ea578d6152e66c1a0df1) )
	ROM_LOAD("mpr-22960.ic11", 0x5800000, 0x0800000, CRC(6c3b228e) SHA1(782c0fda106222be75b1973586c8bf78fd2186e7) )
	ROM_LOAD("mpr-22961.ic12s",0x6000000, 0x0800000, CRC(d6d26a8d) SHA1(7d416f8ac9fbbeb9bfe217ccc8eccf1644511110) )
	ROM_LOAD("mpr-22962.ic13s",0x6800000, 0x0800000, CRC(c2f41101) SHA1(0bf87cbffb7d6a5ab32543cef56c9759f475419a) )
	ROM_LOAD("mpr-22963.ic14s",0x7000000, 0x0800000, CRC(a53e9919) SHA1(d81eb79bc706f85ebfbc56a9b2889ae62d629e8e) )
	ROM_LOAD("mpr-22964.ic15s",0x7800000, 0x0800000, CRC(f581d5a3) SHA1(8cf769f5b0a48951246bb60e9cf58232bcee7bc8) )
	ROM_LOAD("mpr-22965.ic16s",0x8000000, 0x0800000, CRC(8f7bfa8a) SHA1(19f137b1552978d232785c4408805b71835585c6) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "samba-key.bin", 0, 4, CRC(e17e3603) SHA1(5611615064a93cc9d957cc4edd12c716a72cd7f2) )
ROM_END

/*

SYSTEMID: NAOMI
JAP: SEGA MARINE FISHING IN JAPAN
USA: SEGA MARINE FISHING IN USA
EXP: SEGA MARINE FISHING IN EXPORT

*/

ROM_START( smarinef )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x6800000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD("epr-22221.ic22",0x0000000, 0x0400000, CRC(9d984375) SHA1(fe1185d70b4bc1529e3579fd6b2b678c7d548400) )
	ROM_LOAD("mpr-22208.ic1", 0x0800000, 0x0800000, CRC(6a1e418c) SHA1(7092c6a34ac0c2c6fb2b4b78415d08ef473785d9) )
	ROM_LOAD("mpr-22209.ic2", 0x1000000, 0x0800000, CRC(ecf5be54) SHA1(d7c264da4e232ce6f9b05c9920394f8027fa4a1d) )
	/* IC3 empty */
	/* IC4 empty */
	ROM_LOAD("mpr-22212.ic5", 0x2800000, 0x0800000, CRC(8305f462) SHA1(7993231fa71f509b3b7fec691b5a6139947a01e7) )
	ROM_LOAD("mpr-22213.ic6", 0x3000000, 0x0800000, CRC(0912eaea) SHA1(e4cb1262f3b53d3c619900767cfa192115a53d4b) )
	ROM_LOAD("mpr-22214.ic7", 0x3800000, 0x0800000, CRC(661526b6) SHA1(490321a893f706eaea49c6c35c01af6ae45adf01) )
	ROM_LOAD("mpr-22215.ic8", 0x4000000, 0x0800000, CRC(a80714fa) SHA1(b32dde5cc79a9ae9f7f34064c2382115e9303070) )
	ROM_LOAD("mpr-22216.ic9", 0x4800000, 0x0800000, CRC(cf3d1049) SHA1(a390304256dfac623b6fe1b205d918ce3eb67723) )
	ROM_LOAD("mpr-22217.ic10",0x5000000, 0x0800000, CRC(48c92fd6) SHA1(26b17a8d0130512807cf533a60c10c6d1e769de0) )
	ROM_LOAD("mpr-22218.ic11",0x5800000, 0x0800000, CRC(f9ca31b8) SHA1(ea3d0f38ca1a46c896c06f038a6362ad3c9f90b2) )
	ROM_LOAD("mpr-22219.ic12",0x6000000, 0x0800000, CRC(b3b45811) SHA1(045e7236b814f848d4c9767618ddcd4344d880ec) )
ROM_END

/*

SYSTEMID: NAOMI
JAP: SHOOTOUT POOL
USA: SHOOTOUT POOL
EXP: SHOOTOUT POOL PRIZE

*/

ROM_START( shootpl )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x2800000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD("epr-24065a.ic11",0x0000000, 0x0400000, CRC(622a9ba0) SHA1(2f4963b8447ecda78fea0107497c2811f075c07a) )
	ROM_RELOAD( 0x400000, 0x400000)
	ROM_LOAD32_WORD("opr-24060.ic17s", 0x0800000, 0x0800000, CRC(7f3d868c) SHA1(dc352981371c5479a69756bb1cbbbca43252216d) )
	ROM_LOAD32_WORD("opr-24061.ic18",  0x0800002, 0x0800000, CRC(e934267c) SHA1(fdbe2b80e309aa8d9fefd2634aef20153735019d) )
	ROM_LOAD32_WORD("opr-24062.ic19s", 0x1800000, 0x0800000, CRC(26e32af4) SHA1(49412a04198175240ef9adb4b7afb8a628eb127d) )
	ROM_LOAD32_WORD("opr-24063.ic20",  0x1800002, 0x0800000, CRC(683fdcff) SHA1(890816ef1b3e604e16289998cf66e221ef75a0fe) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "shootpl-key.bin", 0, 4, CRC(03c30b17) SHA1(e8e8659aa27b3d1cac2268850d3973d9afeaeba9) )
ROM_END

// SHOOTOUT POOL (the original, the above set is a sequel)
ROM_START( shootopl )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x2800000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD( "epr-23844.ic22", 0x000000, 0x400000, CRC(5c229638) SHA1(9185f9f2369bb2423faff4222419001ac9037d3f) )
	ROM_RELOAD( 0x400000, 0x400000)
	ROM_LOAD32_WORD( "mpr-23840.17s", 0x0800000, 0x800000, CRC(985e5ff4) SHA1(a6f529b1855cc2aef3bed8503746c2e38061f944) )
	ROM_LOAD32_WORD( "mpr-23841.18",  0x0800002, 0x800000, CRC(255fc335) SHA1(34ffec963880383bb9c02642f73ba3c852699831) )
	ROM_LOAD32_WORD( "mpr-23842.19s", 0x1800000, 0x800000, CRC(80724895) SHA1(ed4fa1160b35b3987702c0178bd31c3c5db69e6e) )
	ROM_LOAD32_WORD( "mpr-23843.20",  0x1800002, 0x800000, CRC(3574f616) SHA1(40130e8f98fb31c98428d444b79491f6a06ac208) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "shootopl-key.bin", 0, 4, CRC(45547e02) SHA1(4f79f478ff1eea14bc939a67ff570143cb56a4bf) )
ROM_END

/* Shootout Pool Medal */
ROM_START( shootplm )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x2800000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD( "epr-24148.ic11", 0x000000, 0x400000, CRC(d575f311) SHA1(7f45d897412fd75eda740a82320fce08331fa310) )
	ROM_RELOAD( 0x400000, 0x400000)
	ROM_LOAD32_WORD( "opr-24174.ic17s", 0x0800000, 0x800000, CRC(ccd6aec5) SHA1(a8105ce6986601d8673ffea41353fe399cf8557d) )
	ROM_LOAD32_WORD( "opr-24175.ic18",  0x0800002, 0x800000, CRC(e66e6345) SHA1(28a372168419c9352cb7fc5285bbd37bd37f3b71) )
	ROM_LOAD32_WORD( "opr-24176.ic19s", 0x1800000, 0x800000, CRC(1277bca8) SHA1(e1bd9d1a6f4170a9c29658f95e9e96caf4b0cb84) )
	ROM_LOAD32_WORD( "opr-24177.ic20",  0x1800002, 0x800000, CRC(122eac82) SHA1(2acf00686d682e0f354708fa597933a0d6de4a6f) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "shootpl-key.bin", 0, 4, CRC(03c30b17) SHA1(e8e8659aa27b3d1cac2268850d3973d9afeaeba9) )
ROM_END

/* Oinori-daimyoujin Matsuri (medal) */
ROM_START( oinori )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x5800000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD( "epr-24053.ic22",0x0000000, 0x0400000, CRC(f127bdab) SHA1(9095e618069fa977f6225ad323f38852131f59cd) )
	ROM_RELOAD( 0x400000, 0x400000)
	ROM_LOAD( "mpr-24054.ic1", 0x0800000, 0x1000000, CRC(db595e72) SHA1(030f33ba2c6cc0a3e1b36b5f3be17b3b83f83a42) )
	ROM_LOAD( "mpr-24055.ic2", 0x1800000, 0x1000000, CRC(12a7f86f) SHA1(bfc890df4fb5f96848ed225a676e6f934bdea33a) )
	ROM_LOAD( "mpr-24056.ic3", 0x2800000, 0x1000000, CRC(0da67885) SHA1(c7205060a9518c2d4015718edea191eb0e30a093) )
	ROM_LOAD( "mpr-24057.ic4", 0x3800000, 0x1000000, CRC(6dec3518) SHA1(3e65065df22680e2bbf2d3db22da413f347a1abe) )
	ROM_LOAD( "mpr-24058.ic5", 0x4800000, 0x1000000, CRC(0eba9049) SHA1(a71ca72aeaf17180cde59d7c7b42c97a1b4259ab) )
ROM_END

/*

SYSTEMID: NAOMI
JAP: SPAWN JAPAN
USA: SPAWN USA
EXP: SPAWN EURO

NO.     Type    Byte    Word
IC22    32M     FFFF    FFFF
IC1     64M     C56E    3D11
IC2     64M     A206    CC87
IC3     64M     FD3F    C5DF
IC4     64M     5833    09A4
IC5     64M     B42C    AA08
IC6     64M     C7A4    E2DE
IC7     64M     58CB    5DFD
IC8     64M     144B    783D
IC9     64M     A4A8    D0BE
IC10    64M     94A8    401F

Serial: BAVE-02A1305

*/

ROM_START( spawn )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x5800000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD("epr-22977b.ic22",0x0000000, 0x0400000, CRC(814ff5d1) SHA1(5a0a9e55878927f98750000eb7d9391cbecfe21d) )
	ROM_LOAD("mpr-22967.ic1",  0x0800000, 0x0800000, CRC(78c7d914) SHA1(0a000e396f9a83c2c777cfb61212a82ec17417ba) )
	ROM_LOAD("mpr-22968.ic2",  0x1000000, 0x0800000, CRC(8c4ae1bb) SHA1(a91934a01d306c8fd8f987b3013f33aec028de70) )
	ROM_LOAD("mpr-22969.ic3",  0x1800000, 0x0800000, CRC(2928627c) SHA1(146844edd22b0caf00b40f7635c8753d5758e958) )
	ROM_LOAD("mpr-22970.ic4",  0x2000000, 0x0800000, CRC(12e27ffd) SHA1(d09096cd1ff9218cd849bfe05b34ec4d642e1663) )
	ROM_LOAD("mpr-22971.ic5",  0x2800000, 0x0800000, CRC(993d2bce) SHA1(f04e484704dbddbbff0f36ac5a019fbdde56d402) )
	ROM_LOAD("mpr-22972.ic6",  0x3000000, 0x0800000, CRC(e0f75067) SHA1(741ebef9e7ae6e5207f1819c3eea80491b934c63) )
	ROM_LOAD("mpr-22973.ic7",  0x3800000, 0x0800000, CRC(698498ca) SHA1(b3691409cbf644b8acea01abaebf7b2dea4dd4f7) )
	ROM_LOAD("mpr-22974.ic8",  0x4000000, 0x0800000, CRC(20983c51) SHA1(f7321abf8bf5f2a7329c98174e5cf9b1ebf596b2) )
	ROM_LOAD("mpr-22975.ic9",  0x4800000, 0x0800000, CRC(0d3c70d1) SHA1(22920bc5fd1dda760b5cb17482e9181be899bc08) )
	ROM_LOAD("mpr-22976.ic10", 0x5000000, 0x0800000, CRC(092d8063) SHA1(14fafd3f4c4f2b37172453d1c815fb9b8f4814f4) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "spawn-key.bin", 0, 4, CRC(dd211f09) SHA1(d436cd5009744c4c69fb94c84837103b4ca1c9a7) )
ROM_END

/*

SYSTEMID: NAOMI
JAP: THE TYPING OF THE DEAD
USA: THE TYPING OF THE DEAD
EXP: THE TYPING OF THE DEAD

*/

ROM_START( totd )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0xb000000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD("epr-23021a.ic22", 0x0000000, 0x0400000,  CRC(07d21033) SHA1(d1e619d13c1c01648eb1a6964aad1554dd16c6d5) )

	ROM_LOAD("mpr-23001.ic1", 0x0800000, 0x0800000, CRC(2eaab8ed) SHA1(e078bd8781e2a04e23fd18b11d118b2548fa59a8) )
	ROM_LOAD("mpr-23002.ic2", 0x1000000, 0x0800000, CRC(617edcc7) SHA1(10f92cd9be94739c7c2f94cf9a5fa54accbe6227) )
	ROM_LOAD("mpr-23003.ic3", 0x1800000, 0x0800000, CRC(37d6d9f8) SHA1(3ad3fa65f33d250eb8a620e7dc7c6b1209794a80) )
	ROM_LOAD("mpr-23004.ic4", 0x2000000, 0x0800000, CRC(e41186f2) SHA1(2f4b26d8dba1629db539736cf88ec85c21820aeb) )
	ROM_LOAD("mpr-23005.ic5", 0x2800000, 0x0800000, CRC(2b8e1fc6) SHA1(a5cd8c5840dd316dd1ad9500804b459476ca8ba0) )
	ROM_LOAD("mpr-23006.ic6", 0x3000000, 0x0800000, CRC(3de23e27) SHA1(d3aae2a7e5c78fc3bf8e296392d8f893961d946f) ) //on board but actually 0xff filled
	ROM_LOAD("mpr-23007.ic7", 0x3800000, 0x0800000, CRC(ca16cfdf) SHA1(6279bc9bd661bde2d3e36ca52625f9b91867c4b4) )
	ROM_LOAD("mpr-23008.ic8", 0x4000000, 0x0800000, CRC(8c33191c) SHA1(6227fbb3d51c4301dd1fc60ec43df7c18eef06fa) )
	ROM_LOAD("mpr-23009.ic9", 0x4800000, 0x0800000, CRC(c982d24d) SHA1(d5a15d04f19f5569709b0b1cde64814230f4f0bb) )
	ROM_LOAD("mpr-23010.ic10",0x5000000, 0x0800000, CRC(c6e129b4) SHA1(642a9e1052efcb43d2b809f13d10617b43bd38f3) )
	ROM_LOAD("mpr-23011.ic11",0x5800000, 0x0800000, CRC(9e6942ff) SHA1(8c657d7d74c4c9106756a9934bc3c850f5069e29) )
	ROM_LOAD("mpr-23012.ic12s",0x6000000, 0x0800000, CRC(20e1ebe8) SHA1(e24cb5f48101e665c90af9be333e54ec274004fb) )
	ROM_LOAD("mpr-23013.ic13s",0x6800000, 0x0800000, CRC(3de23e27) SHA1(d3aae2a7e5c78fc3bf8e296392d8f893961d946f) ) //on board but actually 0xff filled
	ROM_LOAD("mpr-23014.ic14s",0x7000000, 0x0800000, CRC(c4f95fdb) SHA1(8c0e806e27d7bed274dcb20b932897ea8b8bbf86) )
	ROM_LOAD("mpr-23015.ic15s",0x7800000, 0x0800000, CRC(5360c49d) SHA1(dbdf955d9bb9a387ded8ada18d26d222d73514d7) )
	ROM_LOAD("mpr-23016.ic16s",0x8000000, 0x0800000, CRC(fae2958b) SHA1(2bfe164723b7b2f57ae0c6e2fe348459f00dc460) )
	ROM_LOAD("mpr-23017.ic17s",0x8800000, 0x0800000, CRC(22337e15) SHA1(6a9f5569177c2936d8ff04da74e1fd036a093422) )
	ROM_LOAD("mpr-23018.ic18s",0x9000000, 0x0800000, CRC(5a608e74) SHA1(4f2ec47dad71d77ad1b8c640db236332c06d7ab7) )
	ROM_LOAD("mpr-23019.ic19s",0x9800000, 0x0800000, CRC(5cc91cc4) SHA1(66a68991f716ec23555784163aa5140b4e44c7ab) )
	ROM_LOAD("mpr-23020.ic20s",0xa000000, 0x0800000, CRC(b5943007) SHA1(d0e95084aec5e05027c21a6b4a3331408853781b) )
	//ic21 not populated
ROM_END

/*

SYSTEMID: NAOMI
JAP: VIRTUA NBA
USA: VIRTUA NBA
EXP: VIRTUA NBA

NO.     Type    Byte    Word
IC22    32M 0000    0000
IC1     64M 5C4A    BB88
IC2     64M 1799    B55E
IC3     64M FB19    6FE8
IC4     64M 6207    33FE
IC5     64M 38F0    F24C
IC6     64M A3B1    FF6F
IC7     64M 737F    B4DD
IC8     64M FD19    49CE
IC9     64M 424E    76D5
IC10    64M 84CC    B74C
IC11    64M 8FC6    D9C8
IC12    64M A838    143A
IC13    64M 88C3    456F
IC14    64M 1C72    971E
IC15    64M B950    F203
IC16    64M 39F6    54CE
IC17    64M 91C7    47B0
IC18    64M 5B94    7E77
IC19    64M DE42    F390
IC20    64M B876    73CE
IC21    64M AD60    2F74

*/

ROM_START( virnba )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0xb000000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD("epr-23073.ic22",0x0000000, 0x0400000, CRC(ce5c3d28) SHA1(ca3eeae1cf78435787338bb7b3e71301c0f71dd9) )
	ROM_LOAD("mpr-22928.ic1", 0x0800000, 0x0800000, CRC(63245c98) SHA1(a5a542244f07c6c8b66961a231fb56c89d2cf20c) )
	ROM_LOAD("mpr-22929.ic2", 0x1000000, 0x0800000, CRC(eea89d21) SHA1(5fe184267e637f155d767f8d931462d9593eff5a) )
	ROM_LOAD("mpr-22930.ic3", 0x1800000, 0x0800000, CRC(2fbefa9a) SHA1(a6df46cb8742022e436cdc6a9a50490c7a551421) )
	ROM_LOAD("mpr-22931.ic4", 0x2000000, 0x0800000, CRC(7332e559) SHA1(9147b69f84713f8e6c2c84b71ccd48bae879c655) )
	ROM_LOAD("mpr-22932.ic5", 0x2800000, 0x0800000, CRC(ef80e18c) SHA1(51406b82c66dc1822657948c62e1c4b8e628a739) )
	ROM_LOAD("mpr-22933.ic6", 0x3000000, 0x0800000, CRC(6a374076) SHA1(3b7c1ce5e3ae027e578c60a885724deeadc07448) )
	ROM_LOAD("mpr-22934.ic7", 0x3800000, 0x0800000, CRC(72f3ee15) SHA1(cf81e47c311769c9dc38fdbbef1a5e3f6b8a0be1) )
	ROM_LOAD("mpr-22935.ic8", 0x4000000, 0x0800000, CRC(35fda6e9) SHA1(857b3c0f576d69d3637503fa53608bc6484eb331) )
	ROM_LOAD("mpr-22936.ic9", 0x4800000, 0x0800000, CRC(b26df107) SHA1(900f1d06fdc9b6951de1b7e61a27ac846b2061db) )
	ROM_LOAD("mpr-22937.ic10",0x5000000, 0x0800000, CRC(477a374b) SHA1(309b723f7d2840d6a2f24ad2f877928cc8138a12) )
	ROM_LOAD("mpr-22938.ic11",0x5800000, 0x0800000, CRC(d59431a4) SHA1(6e3cd8cbde18a6a8672aa302cb119e486c0417e0) )
	ROM_LOAD("mpr-22939.ic12",0x6000000, 0x0800000, CRC(b31d3e6d) SHA1(d55e56a66dc678b973c3d60d3cffb59032bc3c46) )
	ROM_LOAD("mpr-22940.ic13",0x6800000, 0x0800000, CRC(90a81fbf) SHA1(5066b5eda80e881f6f399722f010161c0a452922) )
	ROM_LOAD("mpr-22941.ic14",0x7000000, 0x0800000, CRC(8a72a77d) SHA1(5ce73a76c7915d5a19b05f57b1dfdcd1fe3c53a1) )
	ROM_LOAD("mpr-22942.ic15",0x7800000, 0x0800000, CRC(710f709f) SHA1(2483f0b1106bc82710457a148772e50e83a439d8) )
	ROM_LOAD("mpr-22943.ic16",0x8000000, 0x0800000, CRC(c544f593) SHA1(553af7b6c63d6d6221c4286b8a13840a86e55d5f) )
	ROM_LOAD("mpr-22944.ic17",0x8800000, 0x0800000, CRC(cb096baa) SHA1(cbc267953a749dd24a03d87b65bc19b19bebf205) )
	ROM_LOAD("mpr-22945.ic18",0x9000000, 0x0800000, CRC(f2f914e8) SHA1(ec600abde40bfb5004ec8200ee0eef9410ebca6a) )
	ROM_LOAD("mpr-22946.ic19",0x9800000, 0x0800000, CRC(c79696c5) SHA1(4a9ac8b4ae1ce5d196e6c74fecc241b74aebc4ab) )
	ROM_LOAD("mpr-22947.ic20",0xa000000, 0x0800000, CRC(5e5eb595) SHA1(401d4a11d436988d716bb014b36233f171dc576d) )
	ROM_LOAD("mpr-22948.ic21",0xa800000, 0x0800000, CRC(1b0de917) SHA1(fd1742ea9bb2f1ce871ee3266171f26634e1c8e7) )
ROM_END

ROM_START( virnbao )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0xb000000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD("epr-22949.ic22",0x0000000, 0x0400000, CRC(fd91447e) SHA1(0759d6517aeb684d0cb809c1ae1350615cc0aecc) )
	ROM_LOAD("mpr-22928.ic1", 0x0800000, 0x0800000, CRC(63245c98) SHA1(a5a542244f07c6c8b66961a231fb56c89d2cf20c) )
	ROM_LOAD("mpr-22929.ic2", 0x1000000, 0x0800000, CRC(eea89d21) SHA1(5fe184267e637f155d767f8d931462d9593eff5a) )
	ROM_LOAD("mpr-22930.ic3", 0x1800000, 0x0800000, CRC(2fbefa9a) SHA1(a6df46cb8742022e436cdc6a9a50490c7a551421) )
	ROM_LOAD("mpr-22931.ic4", 0x2000000, 0x0800000, CRC(7332e559) SHA1(9147b69f84713f8e6c2c84b71ccd48bae879c655) )
	ROM_LOAD("mpr-22932.ic5", 0x2800000, 0x0800000, CRC(ef80e18c) SHA1(51406b82c66dc1822657948c62e1c4b8e628a739) )
	ROM_LOAD("mpr-22933.ic6", 0x3000000, 0x0800000, CRC(6a374076) SHA1(3b7c1ce5e3ae027e578c60a885724deeadc07448) )
	ROM_LOAD("mpr-22934.ic7", 0x3800000, 0x0800000, CRC(72f3ee15) SHA1(cf81e47c311769c9dc38fdbbef1a5e3f6b8a0be1) )
	ROM_LOAD("mpr-22935.ic8", 0x4000000, 0x0800000, CRC(35fda6e9) SHA1(857b3c0f576d69d3637503fa53608bc6484eb331) )
	ROM_LOAD("mpr-22936.ic9", 0x4800000, 0x0800000, CRC(b26df107) SHA1(900f1d06fdc9b6951de1b7e61a27ac846b2061db) )
	ROM_LOAD("mpr-22937.ic10",0x5000000, 0x0800000, CRC(477a374b) SHA1(309b723f7d2840d6a2f24ad2f877928cc8138a12) )
	ROM_LOAD("mpr-22938.ic11",0x5800000, 0x0800000, CRC(d59431a4) SHA1(6e3cd8cbde18a6a8672aa302cb119e486c0417e0) )
	ROM_LOAD("mpr-22939.ic12",0x6000000, 0x0800000, CRC(b31d3e6d) SHA1(d55e56a66dc678b973c3d60d3cffb59032bc3c46) )
	ROM_LOAD("mpr-22940.ic13",0x6800000, 0x0800000, CRC(90a81fbf) SHA1(5066b5eda80e881f6f399722f010161c0a452922) )
	ROM_LOAD("mpr-22941.ic14",0x7000000, 0x0800000, CRC(8a72a77d) SHA1(5ce73a76c7915d5a19b05f57b1dfdcd1fe3c53a1) )
	ROM_LOAD("mpr-22942.ic15",0x7800000, 0x0800000, CRC(710f709f) SHA1(2483f0b1106bc82710457a148772e50e83a439d8) )
	ROM_LOAD("mpr-22943.ic16",0x8000000, 0x0800000, CRC(c544f593) SHA1(553af7b6c63d6d6221c4286b8a13840a86e55d5f) )
	ROM_LOAD("mpr-22944.ic17",0x8800000, 0x0800000, CRC(cb096baa) SHA1(cbc267953a749dd24a03d87b65bc19b19bebf205) )
	ROM_LOAD("mpr-22945.ic18",0x9000000, 0x0800000, CRC(f2f914e8) SHA1(ec600abde40bfb5004ec8200ee0eef9410ebca6a) )
	ROM_LOAD("mpr-22946.ic19",0x9800000, 0x0800000, CRC(c79696c5) SHA1(4a9ac8b4ae1ce5d196e6c74fecc241b74aebc4ab) )
	ROM_LOAD("mpr-22947.ic20",0xa000000, 0x0800000, CRC(5e5eb595) SHA1(401d4a11d436988d716bb014b36233f171dc576d) )
	ROM_LOAD("mpr-22948.ic21",0xa800000, 0x0800000, CRC(1b0de917) SHA1(fd1742ea9bb2f1ce871ee3266171f26634e1c8e7) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "virnbao-key.bin", 0, 4, CRC(980e3be8) SHA1(234a2f8ee408144d2c51e8083d0a033eea6026e0) )
ROM_END

ROM_START( virnbap )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0xb000000, "rom_board", ROMREGION_ERASEFF)
    ROM_LOAD("vnbaearly.ic22",0x0000000, 0x0400000, CRC(5bbf7a45) SHA1(ad71ae8e9e08d7e0a9a60d1ba51bc5dcfeb0f50c) )
	ROM_LOAD("mpr-22928.ic1", 0x0800000, 0x0800000, CRC(63245c98) SHA1(a5a542244f07c6c8b66961a231fb56c89d2cf20c) )
	ROM_LOAD("mpr-22929.ic2", 0x1000000, 0x0800000, CRC(eea89d21) SHA1(5fe184267e637f155d767f8d931462d9593eff5a) )
	ROM_LOAD("mpr-22930.ic3", 0x1800000, 0x0800000, CRC(2fbefa9a) SHA1(a6df46cb8742022e436cdc6a9a50490c7a551421) )
	ROM_LOAD("mpr-22931.ic4", 0x2000000, 0x0800000, CRC(7332e559) SHA1(9147b69f84713f8e6c2c84b71ccd48bae879c655) )
	ROM_LOAD("mpr-22932.ic5", 0x2800000, 0x0800000, CRC(ef80e18c) SHA1(51406b82c66dc1822657948c62e1c4b8e628a739) )
	ROM_LOAD("mpr-22933.ic6", 0x3000000, 0x0800000, CRC(6a374076) SHA1(3b7c1ce5e3ae027e578c60a885724deeadc07448) )
	ROM_LOAD("mpr-22934.ic7", 0x3800000, 0x0800000, CRC(72f3ee15) SHA1(cf81e47c311769c9dc38fdbbef1a5e3f6b8a0be1) )
	ROM_LOAD("mpr-22935.ic8", 0x4000000, 0x0800000, CRC(35fda6e9) SHA1(857b3c0f576d69d3637503fa53608bc6484eb331) )
	ROM_LOAD("mpr-22936.ic9", 0x4800000, 0x0800000, CRC(b26df107) SHA1(900f1d06fdc9b6951de1b7e61a27ac846b2061db) )
	ROM_LOAD("mpr-22937.ic10",0x5000000, 0x0800000, CRC(477a374b) SHA1(309b723f7d2840d6a2f24ad2f877928cc8138a12) )
	ROM_LOAD("mpr-22938.ic11",0x5800000, 0x0800000, CRC(d59431a4) SHA1(6e3cd8cbde18a6a8672aa302cb119e486c0417e0) )
	ROM_LOAD("mpr-22939.ic12",0x6000000, 0x0800000, CRC(b31d3e6d) SHA1(d55e56a66dc678b973c3d60d3cffb59032bc3c46) )
	ROM_LOAD("mpr-22940.ic13",0x6800000, 0x0800000, CRC(90a81fbf) SHA1(5066b5eda80e881f6f399722f010161c0a452922) )
	ROM_LOAD("mpr-22941.ic14",0x7000000, 0x0800000, CRC(8a72a77d) SHA1(5ce73a76c7915d5a19b05f57b1dfdcd1fe3c53a1) )
	ROM_LOAD("mpr-22942.ic15",0x7800000, 0x0800000, CRC(710f709f) SHA1(2483f0b1106bc82710457a148772e50e83a439d8) )
	ROM_LOAD("mpr-22943.ic16",0x8000000, 0x0800000, CRC(c544f593) SHA1(553af7b6c63d6d6221c4286b8a13840a86e55d5f) )
	ROM_LOAD("mpr-22944.ic17",0x8800000, 0x0800000, CRC(cb096baa) SHA1(cbc267953a749dd24a03d87b65bc19b19bebf205) )
	ROM_LOAD("mpr-22945.ic18",0x9000000, 0x0800000, CRC(f2f914e8) SHA1(ec600abde40bfb5004ec8200ee0eef9410ebca6a) )
	ROM_LOAD("mpr-22946.ic19",0x9800000, 0x0800000, CRC(c79696c5) SHA1(4a9ac8b4ae1ce5d196e6c74fecc241b74aebc4ab) )
	ROM_LOAD("mpr-22947.ic20",0xa000000, 0x0800000, CRC(5e5eb595) SHA1(401d4a11d436988d716bb014b36233f171dc576d) )
	ROM_LOAD("mpr-22948.ic21",0xa800000, 0x0800000, CRC(1b0de917) SHA1(fd1742ea9bb2f1ce871ee3266171f26634e1c8e7) )
ROM_END

/*

SYSTEMID: NAOMI
JAP: VIRTUA STRIKER 2 VER.2000
USA: VIRTUA STRIKER 2 VER.2000
EXP: VIRTUA STRIKER 2 VER.2000

NO.     Type    Byte    Word
IC22    32M     2B49    A054    EPR21929C.22
IC1     64M     F5DD    E983    MPR21914
IC2     64M     4CB7    198B    MPR21915
IC3     64M     5661    47C0    MPR21916
IC4     64M     CD15    DC9A    MPR21917
IC5     64M     7855    BCC7    MPR21918
IC6     64M     59D2    CB75    MPR21919
IC7     64M     B795    BE9C    MPR21920
IC8     64M     D2DE    5AF2    MPR21921
IC9     64M     7AAD    0DD5    MPR21922
IC10    64M     B31B    2C4E    MPR21923
IC11    64M     5C32    D746    MPR21924
IC12    64M     1886    D5EA    MPR21925
IC13    64M     D7B3    24D7    MPR21926
IC14    64M     9EF2    E513    MPR21927
IC15    32M     0DF9    FC01    MPR21928

*/

ROM_START( vs2_2k )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x8000000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD("epr-21929.ic22", 0x0000000, 0x0400000, CRC(831af08a) SHA1(af4c74623be823fd061765cede354c6a9722fd10) )
	ROM_LOAD("mpr-21924.ic1",  0x0800000, 0x0800000, CRC(f91ef69b) SHA1(4ed23091efad7ddf1878a0bfcdcbba3cf151af84) )
	ROM_LOAD("mpr-21925.ic2",  0x1000000, 0x0800000, CRC(40128a67) SHA1(9d191c4ec33465f29bbc09491dde62f354a9ab15) )
	ROM_LOAD("mpr-21911.ic3",  0x1800000, 0x0800000, CRC(19708b3c) SHA1(7d1ef995ce870ffcb68f420a571efb084f5bfcf2) )
	ROM_LOAD("mpr-21926.ic4",  0x2000000, 0x0800000, CRC(b082379b) SHA1(42f585279da1de7e613e42b76e1b81986c48e6ea) )
	ROM_LOAD("mpr-21913.ic5",  0x2800000, 0x0800000, CRC(a3bc1a47) SHA1(0e5043ab6e118feb59f68c84c095cf5b1dba7d09) )
	ROM_LOAD("mpr-21914.ic6",  0x3000000, 0x0800000, CRC(b1dfada7) SHA1(b4c906bc96b615975f6319a1fdbd5b990e7e4124) )
	ROM_LOAD("mpr-21915.ic7",  0x3800000, 0x0800000, CRC(1c189e28) SHA1(93400de2cb803357fa17ae7e1a5297177f9bcfa1) )
	ROM_LOAD("mpr-21916.ic8",  0x4000000, 0x0800000, CRC(55bcb652) SHA1(4de2e7e584dd4999dc8e405837a18a904dfee0bf) )
	ROM_LOAD("mpr-21917.ic9",  0x4800000, 0x0800000, CRC(81daa7bc) SHA1(2fc0ddd0cca3ddd120f634ddf08ffbf889ee7181) )
	ROM_LOAD("mpr-21918.ic10", 0x5000000, 0x0800000, CRC(a5cd42ad) SHA1(59f62e995d45311b1592434d1ffa42c261fa8ba1) )
	ROM_LOAD("mpr-21919.ic11", 0x5800000, 0x0800000, CRC(cc1a4ed9) SHA1(0e3aaeaa55f1d145fb4877b6d187a3ee78cf214e) )
	ROM_LOAD("mpr-21920.ic12s",0x6000000, 0x0800000, CRC(9452c5fb) SHA1(5a04f96d83cca6248f513de0c6240fc671bcadf9) )
	ROM_LOAD("mpr-21921.ic13s",0x6800000, 0x0800000, CRC(d6346491) SHA1(830971cbc14cab022a09ad4c6e11ee49c550e308) )
	ROM_LOAD("mpr-21922.ic14s",0x7000000, 0x0800000, CRC(a1901e1e) SHA1(2281f91ac696cc14886bcdf4b0685ce2f5bb8117) )
	ROM_LOAD("mpr-21923.ic15s",0x7800000, 0x0400000, CRC(d127d9a5) SHA1(78c95357344ea15469b84fa8b1332e76521892cd) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "vs2_2k-key.bin", 0, 4, CRC(f9fb4716) SHA1(83277f61e97fc6c4d8d27b903bde0acdfb0c7dbc) )
ROM_END

/*

SYSTEMID: NAOMI
JAP:  POWER SMASH --------------
USA:  VIRTUA TENNIS IN USA -----
EXP:  VIRTUA TENNIS IN EXPORT --

NO. Type    Byte    Word
IC22    32M 0000    1111
IC1 64M 7422    83DD
IC2 64M 7F26    A93D
IC3 64M 8E02    D3FC
IC4 64M 2545    F734
IC5 64M E197    B75D
IC6 64M 9453    CF75
IC7 64M 29AC    2FEB
IC8 64M 0434    2E9E
IC9 64M C86E    79E6
IC10    64M C67A    BF14
IC11    64M F590    D280

*/

ROM_START( vtennis )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x6000000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD("epr-22927.ic22", 0x0000000, 0x0400000,  CRC(89781723) SHA1(cf644aa66abcec6964d77485a0292f11ba80dd0d) )
	ROM_RELOAD( 0x400000, 0x400000)
	ROM_LOAD("mpr-22916.ic1", 0x0800000, 0x0800000, CRC(903873e5) SHA1(09af791bc02cca0e2dc72187679830ed9f4fc772) )
	ROM_LOAD("mpr-22917.ic2", 0x1000000, 0x0800000, CRC(5f020fa6) SHA1(bd2519be8c88ff34cf2fd2b17271d2b41b64ce9f) )
	ROM_LOAD("mpr-22918.ic3", 0x1800000, 0x0800000, CRC(3c3bf533) SHA1(db43ca9332e76b968b9b388b4824b768f82b9859) )
	ROM_LOAD("mpr-22919.ic4", 0x2000000, 0x0800000, CRC(3d8dd003) SHA1(91f494b06b9977215ab726a2499b5855d4d49e81) )
	ROM_LOAD("mpr-22920.ic5", 0x2800000, 0x0800000, CRC(efd781d4) SHA1(ced5a8dc8ff7677b3cac2a4fae04670c46cc96af) )
	ROM_LOAD("mpr-22921.ic6", 0x3000000, 0x0800000, CRC(79e75be1) SHA1(82318613c947907e01bbe50569b05ef24789d7c9) )
	ROM_LOAD("mpr-22922.ic7", 0x3800000, 0x0800000, CRC(44bd3883) SHA1(5c595d903d8865bf8bf3aafb1f527bff232718ed) )
	ROM_LOAD("mpr-22923.ic8", 0x4000000, 0x0800000, CRC(9ebdf0f8) SHA1(f1b688bda387fc00c70cb6a0c374c6c13926c138) )
	ROM_LOAD("mpr-22924.ic9", 0x4800000, 0x0800000, CRC(ecde9d57) SHA1(1fbe7fdf66a56f4f1765baf113dff95142bfd114) )
	ROM_LOAD("mpr-22925.ic10",0x5000000, 0x0800000, CRC(81057e42) SHA1(d41137ae28c64dbdb50150db8cf25851bc0709c4) )
	ROM_LOAD("mpr-22926.ic11",0x5800000, 0x0800000, CRC(57eec89d) SHA1(dd8f9a9155e51ee5260f559449fb0ea245077952) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "vtennis-key.bin", 0, 4, CRC(f355af89) SHA1(581a9b0f8c2f3606006e85e136f9cf0ea908c773) )
ROM_END

/*
SYSTEMID: NAOMI
JAP: ROYAL RUMBLE
USA: ROYAL RUMBLE
EXP: ROYAL RUMBLE
*/

ROM_START( wwfroyal )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x8800000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD("epr-22261.ic22", 0x0000000, 0x0400000, CRC(60e5a6cd) SHA1(d74ee8318e40190231b94030176223da8305c053) )
	ROM_LOAD( "mpr-22262.ic1", 0x0800000, 0x1000000, CRC(f18c7920) SHA1(76fb592f62b8a359df19074265b44772ab2ecea0) )
	ROM_LOAD( "mpr-22263.ic2", 0x1800000, 0x1000000, CRC(5a397a54) SHA1(4b41b19ab7c49e09eeb5df2e688fdaecf8bb33a9) )
	ROM_LOAD( "mpr-22264.ic3", 0x2800000, 0x1000000, CRC(edca701e) SHA1(459b533d862e011f8daa0e4997d69fa7339b0755) )
	ROM_LOAD( "mpr-22265.ic4", 0x3800000, 0x1000000, CRC(7dfe71a1) SHA1(f2053544de8a177ab931c4d6a9010dfb6cc92e31) )
	ROM_LOAD( "mpr-22266.ic5", 0x4800000, 0x1000000, CRC(3e9ac148) SHA1(05a5725c72bbfc65db47aaa677b95f07aa9a3909) )
	ROM_LOAD( "mpr-22267.ic6", 0x5800000, 0x1000000, CRC(67ec1027) SHA1(2432b33983bbc9b07477459adb5ee1a62b6c0ea3) )
	ROM_LOAD( "mpr-22268.ic7", 0x6800000, 0x1000000, CRC(536f5eea) SHA1(f1de8624f82595adf75693b604fb026bf3f778ee) )
	ROM_LOAD( "mpr-22269.ic8", 0x7800000, 0x1000000, CRC(6c0cf740) SHA1(da10b33a6e54afbe1d7e52801216e7119b0b33b1) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "wwfroyal-key.bin", 0, 4, CRC(e14721b1) SHA1(1f2f447bbc4b1e0b016c5ac545d1d805abc61ebd) )
ROM_END

ROM_START( sl2007 )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x10000000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD( "ic8.bin",    0x0000000, 0x4000000, CRC(30f0dcda) SHA1(eb1ebb0b089bb27571721de07e8635c89734d23e) )
	ROM_LOAD( "ic9.bin",    0x4000000, 0x4000000, CRC(5475556e) SHA1(b895125b7c9f01723df42b44073c206cb43871ac) )
	ROM_LOAD( "ic10.bin",   0x8000000, 0x4000000, CRC(133c742c) SHA1(89f857a31731dc918afc72b6cb716f5c77cb9d6e) )
	ROM_LOAD( "ic11.bin",   0xc000000, 0x4000000, CRC(562fb88e) SHA1(172678e3e27cfad7f7e6217c4653a4ba119bfbdf) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "sl2007-key.bin", 0, 4, CRC(d5d1e807) SHA1(8a0cc371729c622bb05c5d26b3e39ec31d29ace1) )
ROM_END

ROM_START( asndynmt )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x10000000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD( "fpr-24382.ic8",  0x0000000, 0x4000000, CRC(4daefde5) SHA1(ceba09315a22541f500dbfe1f8ebfb0a1f9a8a62) )
	ROM_LOAD( "fpr-24383.ic9",  0x4000000, 0x4000000, CRC(8ac2fe5d) SHA1(1c606140ffb2720433bdb0d225ef3c70e2260d27) )
	ROM_LOAD( "fpr-24384.ic10", 0x8000000, 0x4000000, CRC(2e9116c4) SHA1(58903a33c4ce72a1f75aefcab94393fc2e8bd2d9) )
	ROM_LOAD( "fpr-24385.ic11", 0xc000000, 0x4000000, CRC(2b79f45d) SHA1(db97d980bf1590df4b983a4b7786977687238ef5) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "asndynmt-key.bin", 0, 4, CRC(bf5396a9) SHA1(0b27fdc800143fb977cb2f1e937078d7a7006939) )
ROM_END

ROM_START( illvelo )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x10000000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD( "illvelo.ic8",  0x0000000, 0x4000000, CRC(34ddd732) SHA1(fc714e9593225e9fa3a3caa7a988ab1aa994e50f) )
	ROM_LOAD( "illvelo.ic9",  0x4000000, 0x4000000, CRC(2a880b08) SHA1(87c8e742b8f0658fee91ea97c61b1b9d5dbb25f5) )
	ROM_LOAD( "illvelo.ic10", 0x8000000, 0x4000000, CRC(c02040f9) SHA1(27ad2cb45e8a516433917f060ca9798412bb95f7) )
	ROM_LOAD( "illvelo.ic11", 0xc000000, 0x4000000, CRC(b2eb30ed) SHA1(44fac4bedde4df04b9572ac665d3ac2c5cd00c7d) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "illvelo-key.bin", 0, 4, CRC(e164952f) SHA1(6c0dfe567640e1e843a5d7bf858a24c101dfcf95) )
ROM_END

ROM_START( mbaa )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x18000000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD( "ic8.bin",      0x00000000, 0x4000000, CRC(0fbc0512) SHA1(a84969ae0abc571afc297afd0c628c6551b52819) )
	ROM_LOAD( "ic9.bin",      0x04000000, 0x4000000, CRC(06d8f022) SHA1(e20a5f66a5aa36c9fa61fd39cbdc2946bb905568) )
	ROM_LOAD( "ic10.bin",     0x08000000, 0x4000000, CRC(b6bb7ce4) SHA1(51185c8fa95a67d3a4dfa422ed0eee4bf62c759d) )
	ROM_LOAD( "ic11.bin",     0x0c000000, 0x4000000, CRC(211ac347) SHA1(9d8348db90971204e5d60f2d561fcca33ee7c264) )
	ROM_LOAD( "ic12.bin",     0x10000000, 0x4000000, CRC(b8a6bff2) SHA1(befbc2e917b3107f1c4bfb9169623282ff97bfb2) )
	ROM_LOAD( "ic13.bin",     0x14000000, 0x4000000, CRC(4886329f) SHA1(6ccf6fb83cfdbef3f85f6c06e641c38ff434d605) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "mbaa-key.bin", 0x000000, 0x000004, CRC(f4ad909f) SHA1(27ba44592c2642b5862a24f68c755ad4115e6047) )
ROM_END

ROM_START( ausfache )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x8000000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD( "ic8.bin",    0x0000000, 0x4000000, CRC(f6a16173) SHA1(7167885ce27a99bce286ba71128b4a2c8363015a) )
	ROM_LOAD( "ic9.bin",    0x4000000, 0x4000000, CRC(18c994d7) SHA1(159e1425b2fc645133814b0d26d93a90e9849b1a) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "ausfache-key.bin", 0, 4, CRC(93cdc793) SHA1(f0a0c321a3bdf8ca87cbd840a168a9057c08f16a) )
ROM_END

ROM_START( pokasuka )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x14000000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD( "fpr-24365.ic8",  0x00000000, 0x4000000, CRC(11489cda) SHA1(d9902a61491061f522650f825f92e81541fcc772) )
	ROM_LOAD( "fpr-24366.ic9",  0x04000000, 0x4000000, CRC(7429714a) SHA1(e45b442f447d24de0c746943a59c0dceb6e359cc) )
	ROM_LOAD( "fpr-24367.ic10", 0x08000000, 0x4000000, CRC(dee87bab) SHA1(c5386cda2e84992e18b7959e7d9965c28c1185a4) )
	ROM_LOAD( "fpr-24368.ic11", 0x0c000000, 0x4000000, CRC(124f55e2) SHA1(bc2cb9514acd98f116917ea771b06c4e03ffae73) )
	ROM_LOAD( "fpr-24369.ic12", 0x10000000, 0x4000000, CRC(35b544ab) SHA1(270a75883a867318fd417ec819c40c36f2d296b8) )

	ROM_REGION( 0x200000, "ioboard", 0)	// touch screen I/O board, program disassembles as little-endian SH-4
	ROM_LOAD( "fpr24351.ic14", 0x000000, 0x200000, CRC(4d1b7b89) SHA1(965b8c6b5a2e7b3f1b1e2eac19c86000c3b66754) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "pokasuka-key.bin", 0, 4, CRC(f00bcd61) SHA1(b8315b851656c2e0b7853979988d1c44eab0886b) )
ROM_END

/*

SYSTEMID: NAOMI
JAP: ZOMBIE REVENGE IN JAPAN
USA: ZOMBIE REVENGE IN USA
EXP: ZOMBIE REVENGE IN EXPORT

NO. Type    Byte    Word
IC22    16M 0000    0000
IC1 64M 899B    97E1
IC2 64M 6F0B    2D2D
IC3 64M 4328    C898
IC4 64M 0205    57C5
IC5 64M 93A7    A717
IC6 64M 936B    A35B
IC7 64M 2F51    2BFC
IC8 64M D627    54C5
IC9 64M D2F9    B84C
IC10    64M 9B5A    B70B
IC11    64M 3F0F    9AEB
IC12    64M 5778    EBCA
IC13    64M 75DB    8563
IC14    64M 427A    577C
IC15    64M A7B7    D0D6
IC16    64M 9F01    FCFE
IC17    64M DFB4    58F7
IC18    64M C453    B313
IC19    64M 04B8    49FB

Protection notes:
0C0E6758: 013C   MOV.B   @(R0,R3),R1 ;checks $c7a45b8+94, natively it's 0xbb, it should be 0 or 1
0C0E675A: 611C   EXTU.B  R1,R1
0C0E675C: 31C7   CMP/GT  R12,R1
0C0E675E: 1F11   MOV.L   R1,@($04,R15)
0C0E6760: 8F04   BFS     $0C0E676C ;if R12 > R1 go ahead, otherwise kill yourself
0C0E6762: E500   MOV     #$00,R5
0C0E6764: D023   MOV.L   @($008C,PC),R0 [0C0E67F4]
0C0E6766: 2052   MOV.L   R5,@R0
0C0E6768: AFFE   BRA     $0C0E6768
*/

ROM_START( zombrvn )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0xa000000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD("epr-21707.ic22", 0x0000000, 0x0200000,  CRC(4daa11e9) SHA1(2dc219a5e0d0b41cce6d07631baff0495c479e13) )
	ROM_RELOAD( 0x200000, 0x200000)
	ROM_RELOAD( 0x400000, 0x200000)
	ROM_RELOAD( 0x600000, 0x200000)
	ROM_LOAD("mpr-21708.ic1", 0x0800000, 0x0800000, CRC(b1ca1ca0) SHA1(7f6823c8f8b58d3102e73c153a3f4ce5ad70694d) )
	ROM_LOAD("mpr-21709.ic2", 0x1000000, 0x0800000, CRC(1ccc22bb) SHA1(0d0b4b13a997e33d89c0b67e579ff5cb63f49355) )
	ROM_LOAD("mpr-21710.ic3", 0x1800000, 0x0800000, CRC(954f49ba) SHA1(67d532048eeb0e7ddd77784138708b256a9386cd) )
	ROM_LOAD("mpr-21711.ic4", 0x2000000, 0x0800000, CRC(bda785e2) SHA1(85fe90fce718f278fc90d3b64411be2b420fef17) )
	ROM_LOAD("mpr-21712.ic5", 0x2800000, 0x0800000, CRC(38309255) SHA1(f693e76b520f25bc510ab1025303cd7e544d9386) )
	ROM_LOAD("mpr-21713.ic6", 0x3000000, 0x0800000, CRC(7c66c88e) SHA1(3bac6db0a5ea65b100911a9674312d4b94f6f57a) )
	ROM_LOAD("mpr-21714.ic7", 0x3800000, 0x0800000, CRC(dd8db07e) SHA1(087299d342e86f629e4878d592540faaba78d5c1) )
	ROM_LOAD("mpr-21715.ic8", 0x4000000, 0x0800000, CRC(7243da2e) SHA1(a797ff85527224d128268cf62e028ee8b308b406) )
	ROM_LOAD("mpr-21716.ic9", 0x4800000, 0x0800000, CRC(01dd88c2) SHA1(77b8bf78d760ad769964209e881e5eddc74d45d4) )
	ROM_LOAD("mpr-21717.ic10",0x5000000, 0x0800000, CRC(95139ec0) SHA1(90db6f18e18e842f731ef34892ac520fd9f4a8d6) )
	ROM_LOAD("mpr-21718.ic11",0x5800000, 0x0800000, CRC(4d36a24a) SHA1(0bc2d80e6149b2d97582a58fdf43d0bdbcfcedfc) )
	ROM_LOAD("mpr-21719.ic12",0x6000000, 0x0800000, CRC(2b86df0a) SHA1(1d6bf4d2568df3ce3a2e60dc51167b5344b00ebd) )
	ROM_LOAD("mpr-21720.ic13",0x6800000, 0x0800000, CRC(ff681ece) SHA1(896e2c484e640d8c426f0159a1be419e476ad14f) )
	ROM_LOAD("mpr-21721.ic14",0x7000000, 0x0800000, CRC(216abba6) SHA1(0819d727a235fe6a3ccfe6474fce9b13718e235c) )
	ROM_LOAD("mpr-21722.ic15",0x7800000, 0x0800000, CRC(b2de7e5f) SHA1(626bf13c40df936a34176821d38418214a5407cb) )
	ROM_LOAD("mpr-21723.ic16",0x8000000, 0x0800000, CRC(515932ae) SHA1(978495c9f9f24d0cdae5a44c3376f7a43f0ce9f5) )
	ROM_LOAD("mpr-21724.ic17",0x8800000, 0x0800000, CRC(f048aeb7) SHA1(39b7bf0ce65f6e13aa0ae5fd6a142959b9ce5acf) )
	ROM_LOAD("mpr-21725.ic18",0x9000000, 0x0800000, CRC(2202077b) SHA1(0893a85379f994277083c0bc5b178dd34508f816) )
	ROM_LOAD("mpr-21726.ic19",0x9800000, 0x0800000, CRC(429bf290) SHA1(6733e1bcf100e73ab43273f6feedc187fcaa55d4) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "zombrvn-key.bin", 0, 4, CRC(572d3944) SHA1(16ae634d85169851b9deae3c03b5dd4d4bf2794a) )
ROM_END

ROM_START( gunsur2 )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x10000000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD( "bhf1vere.2f",  0x0000000, 0x0800000, CRC(121ea283) SHA1(e4bf2b90fa3d42727b8393ffa2c5a8863914a630) )
	ROM_LOAD( "bhf1ma2.4m",   0x2000000, 0x1000000, CRC(8073dec7) SHA1(2d4173ff0de37b95a8cb02e1b572a9cdeb448c24) )
	ROM_LOAD( "bhf1ma3.4l",   0x3000000, 0x1000000, CRC(43cd16a4) SHA1(86258df34b652a614129efd4c825c62ff1382318) )
	ROM_LOAD( "bhf1ma4.4k",   0x4000000, 0x1000000, CRC(f6aebed8) SHA1(687057aacb45ebffe3b5cf2b8dd52d24039392f1) )
	ROM_LOAD( "bhf1ma5.4j",   0x5000000, 0x1000000, CRC(b5e1b582) SHA1(47763113e6917bbf48840292c08d4f63c3ce085a) )
	ROM_LOAD( "bhf1ma6.4h",   0x6000000, 0x1000000, CRC(345fd824) SHA1(61ebc12c7cd7f2e2c5173cc0f57240855ec99c6f) )
	ROM_LOAD( "bhf1ma7.4f",   0x7000000, 0x1000000, CRC(465ecff1) SHA1(f9eabc77ed8135fa77c8e40335e6b3df1a64042c) )
	ROM_LOAD( "bhf1ma8.4d",   0x8000000, 0x1000000, CRC(76c92354) SHA1(0049b10144d65f574d14d9ad9d1d5380bf154532) )
	ROM_LOAD( "bhf1ma9.4e",   0x9000000, 0x1000000, CRC(d45a46ee) SHA1(c12764f5ba17f10fb309e47450bb89fbef51e252) )
	ROM_LOAD( "bhf1ma10.4c",  0xa000000, 0x1000000, CRC(8c38d1f7) SHA1(3fbc280590c49fa094c1fc1e23d6c9d0031298c5) )
	ROM_LOAD( "bhf1ma11.4b",  0xb000000, 0x1000000, CRC(f49153c4) SHA1(85d5583cac492317ba52dc7a31a443f5f26a67c9) )
	ROM_LOAD( "bhf1ma12.6p",  0xc000000, 0x1000000, CRC(0e2bdd9a) SHA1(e2f82d2c9e33da1a297d79a0324558d0ff614172) )
	ROM_LOAD( "bhf1ma13.6n",  0xd000000, 0x1000000, CRC(055718ad) SHA1(355c4780231a4361aa6237dd34819b60d9df0fc7) )
	ROM_LOAD( "bhf1ma14.6m",  0xe000000, 0x1000000, CRC(d06c9bd7) SHA1(54668a2fd31059976890da92709c18f308634887) )
	ROM_LOAD( "bhf1ma15.6l",  0xf000000, 0x1000000, CRC(db3c396b) SHA1(da0e125d627ce890906ca100081ab0685e11c0ef) )
ROM_END

ROM_START( wldkicks )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0xb000000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD( "wk1-fl1.2d",   0x0400000, 0x0400000, CRC(f833a543) SHA1(328b64f2790bfd0b63168a8a3175227e083b995d) )
	ROM_LOAD( "wk1-fl2.2c",   0x0c00000, 0x0400000, CRC(20ee71b0) SHA1(074e0c531e8770b0efd6464193bce7a284a3c057) )
	ROM_LOAD( "wk1-ma2.4m",   0x1000000, 0x1000000, CRC(650590ec) SHA1(bb9d5d5df2321df24ee0fb9e8bf2757d5277f8ea) )
	ROM_RELOAD( 0x800000, 0x400000)
	ROM_LOAD( "wk1-ma3.4l",   0x2000000, 0x1000000, CRC(3b340dc0) SHA1(2412e41d5bd74d1233fb91f8ce2276a318bfc53d) )
	ROM_LOAD( "wk1-ma4.4k",   0x3000000, 0x1000000, CRC(263fbb16) SHA1(b5d3a3d085f9623d70030ca3c49afb84e25549e3) )
	ROM_LOAD( "wk1-ma5.4j",   0x4000000, 0x1000000, CRC(9697db68) SHA1(7926e2acff0519403afcba9bdb5f68de28b06c79) )
	ROM_LOAD( "wk1-ma6.4h",   0x5000000, 0x1000000, CRC(65017db3) SHA1(a66cd73cdfc9355df63da781a46aa832889f583a) )
	ROM_LOAD( "wk1-ma7.4f",   0x6000000, 0x1000000, CRC(902eea85) SHA1(aa7964eb85b468d4fe112f9f0faaf2fa3f1aa96b) )
	ROM_LOAD( "wk1-ma8.4e",   0x7000000, 0x1000000, CRC(90e917ed) SHA1(53d32ce0ae2b05fa55a95b8697927045d07f4e8a) )
	ROM_LOAD( "wk1-ma9.4d",   0x8000000, 0x1000000, CRC(1d227a05) SHA1(9f816bcdf0279785e0b37ab2f3c5eb5912114dd5) )
	ROM_LOAD( "wk1-ma10.4c",  0x9000000, 0x1000000, CRC(29635a54) SHA1(a3109d0f8f271e2183316846df2a6a819f6a9b20) )
	ROM_LOAD( "wk1-ma11.4b",  0xa000000, 0x1000000, CRC(e96f312c) SHA1(0a92640277111aef5c6e9dab4218a8ae2196ce61) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "wldkicks-key.bin", 0, 4, CRC(c1e3000b) SHA1(36c2546833effe9452e2b3f7d31335fc5e349f49) )
ROM_END

ROM_START( toukon4 )
    NAOMI_BIOS
    NAOMI_DEFAULT_EEPROM

    ROM_REGION( 0x10000000, "rom_board", ROMREGION_ERASEFF)
    ROM_LOAD( "trf1_fl0.2f",  0x0000000, 0x0800000, CRC(862e673d) SHA1(f2c90932ba4abe31d02e86f80ef3e2689342c384) )
    ROM_LOAD( "trf1_fl1.2d",  0x0800000, 0x0800000, CRC(59a935c9) SHA1(7298d83a27eac74bad18d716a791ab2016fe028f) )
    ROM_LOAD( "trf1_ma1.4n",  0x1000000, 0x1000000, CRC(b1b3ab96) SHA1(3dc4bcf796583a0fb51359a86a46d7883df54fad) )
    ROM_LOAD( "trf1_ma2.4m",  0x2000000, 0x1000000, CRC(a27d3dda) SHA1(b9ec4be5845cbe91fd7bc537669e96716513a5f0) )
    ROM_LOAD( "trf1_ma3.4l",  0x3000000, 0x1000000, CRC(345ec434) SHA1(986d9930e7de46b88936c898ba8b05d950262a1f) )
    ROM_LOAD( "trf1_ma4.4k",  0x4000000, 0x1000000, CRC(fc47a104) SHA1(07ced58c1b17719ac36db1589771e67ce16d912e) )
    ROM_LOAD( "trf1_ma5.4j",  0x5000000, 0x1000000, CRC(e1bb077d) SHA1(6b70d8103bf067319e9022742fc1dd843d7e5076) )
    ROM_LOAD( "trf1_ma6.4h",  0x6000000, 0x1000000, CRC(abae4d06) SHA1(56da2ca3551287942afabf37e7fd1b884bd3cac8) )
    ROM_LOAD( "trf1_ma7.4f",  0x7000000, 0x1000000, CRC(19d0092a) SHA1(2ae2dbf2f3958da9f69b3a8636c6837e4243bb67) )
    ROM_LOAD( "trf1_ma8.4e",  0x8000000, 0x1000000, CRC(6fbf34ab) SHA1(f4747d1a1c02e22502e68d72a4f0c152fa69d778) )
    ROM_LOAD( "trf1_ma9.4d",  0x9000000, 0x1000000, CRC(2fa36c5d) SHA1(6478687c91484141521ae79a997cecbcfbb7beae) )
    ROM_LOAD( "trf1_ma10.4c", 0xa000000, 0x1000000, CRC(bc866a37) SHA1(d4d12f285a3bc9136cf3fc2a59dba5ad557cc7d7) )
    ROM_LOAD( "trf1_ma11.4b", 0xb000000, 0x1000000, CRC(68d11482) SHA1(fe991ba5664d6ccf0aac5167f05c5a780f851ae9) )
    ROM_LOAD( "trf1_ma12.6p", 0xc000000, 0x1000000, CRC(a7c3bd3c) SHA1(ed5a24e4c47f686120577dae4550fb9378209cf8) )
    ROM_LOAD( "trf1_ma13.6n", 0xd000000, 0x1000000, CRC(e52d41fe) SHA1(e1769d42d6048f9621ca289af3ddeca7c14cee00) )
    ROM_LOAD( "trf1_ma14.6m", 0xe000000, 0x1000000, CRC(87cb31a0) SHA1(27aef9ac571a0b5e3a76e4ee22f5bc5d0ae962f2) )
    ROM_LOAD( "trf1_ma15.6l", 0xf000000, 0x1000000, CRC(42d318c5) SHA1(f9fe82ffbfc51fcb52333f94c55a7092e7124fb4) )

    ROM_REGION( 4, "rom_key", 0 )
    ROM_LOAD( "tr4ae-key.bin", 0x000000, 0x000004, CRC(986a7cee) SHA1(70c3ff80f86de6a0655251658c66a156fb644995) )
ROM_END

ROM_START( vtenis2c )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x10000000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD( "epr-22327a.bin", 0x000000, 0x400000, CRC(e949004c) SHA1(54db84c3e1db30b233612f68dcd094b597deffd0) )
	ROM_LOAD32_WORD( "mpr-22307.17s",  0x0800000, 0x800000, CRC(db5c0818) SHA1(d39d2241adeb962d68bfb76a9d1c0dcd538c3538) )
	ROM_LOAD32_WORD( "mpr-22308.18",   0x0800002, 0x800000, CRC(ac996a0e) SHA1(5cf110b79ef250cd64919205874278b4a85a42f6) )
	ROM_LOAD32_WORD( "mpr-22309.19s",  0x1800000, 0x800000, CRC(fba79592) SHA1(09f2be133cf503e6f5e13eef1b1e38c268529b68) )
	ROM_LOAD32_WORD( "mpr-22310.20",   0x1800002, 0x800000, CRC(10876476) SHA1(590fd5532c72c67e285a12acd33e828f2e1ff5b1) )
	ROM_LOAD32_WORD( "mpr-22311.21s",  0x2800000, 0x800000, CRC(ad735e61) SHA1(6d340ae2f8fe44b82b1bbace02b5c10e89e40400) )
	ROM_LOAD32_WORD( "mpr-22312.22",   0x2800002, 0x800000, CRC(46830ffa) SHA1(89b843588a48352b958a70e85deb625c19a2fcc7) )
	ROM_LOAD32_WORD( "mpr-22313.23s",  0x3800000, 0x800000, CRC(44900a20) SHA1(3a82938a81fd6634c51595479d59ae4215fb8af3) )
	ROM_LOAD32_WORD( "mpr-22314.24",   0x3800002, 0x800000, CRC(3b0ba6e9) SHA1(0ee1dd2d67dd688de19b1ede00c48234994a4832) )
	ROM_LOAD32_WORD( "mpr-22315.25s",  0x4800000, 0x800000, CRC(eebe4877) SHA1(bc13e50d85f51aa2baf3c00440a089a8aff998ef) )
	ROM_LOAD32_WORD( "mpr-22316.26",   0x4800002, 0x800000, CRC(0a82d403) SHA1(fc002210397eb792c27885d440671225d0bed71f) )
	ROM_LOAD32_WORD( "mpr-22317.27s",  0x5800000, 0x800000, CRC(55ef3e71) SHA1(d7643c5d3bb6ccf4b35511be2b5c776fc46a2f45) )
	ROM_LOAD32_WORD( "mpr-22318.28",   0x5800002, 0x800000, CRC(3dfabb96) SHA1(97863c939e5bb3e976c517839faa49bcfbee83c5) )
	ROM_LOAD32_WORD( "mpr-22319.29s",  0x6800000, 0x800000, CRC(9b92fd02) SHA1(19c9b279aca1aa6575047494ce26a36dbc93812f) )
	ROM_LOAD32_WORD( "mpr-22320.30",   0x6800002, 0x800000, CRC(9b762f6e) SHA1(7f787a89574e45f529970377e8b6394472cd001b) )
	ROM_LOAD32_WORD( "mpr-22321.31s",  0x7800000, 0x800000, CRC(5ce2b3f1) SHA1(9bac49167ced0415ab3b2cac20c7bcf205aecfdf) )
	ROM_LOAD32_WORD( "mpr-22322.32",   0x7800002, 0x800000, CRC(21bf1caf) SHA1(bc1828db2f5f71ef87153a81b49b2ba72ba176e1) )
	ROM_LOAD32_WORD( "mpr-22323.33s",  0x8800000, 0x800000, CRC(0bd632ab) SHA1(21acab5336dd0ba317839176f2557df95917729e) )
	ROM_LOAD32_WORD( "mpr-22324.34",   0x8800002, 0x800000, CRC(ff2571d4) SHA1(c2cbb2345163bbf1973e63cefb9a952e5a52a6cf) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "vtenis2c-key.bin", 0, 4, CRC(b8c5b510) SHA1(f36d037a62a576e71211093e075f0ffa7e312c2d) )
ROM_END

ROM_START( kick4csh )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x9000084, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD( "epr-24212.ic11",   0x0000000, 0x400000, CRC(935453e8) SHA1(b34b3ac976229cba941843ffac9db420068999de) )
	ROM_LOAD32_WORD( "opr-24213a.ic17s", 0x1000000, 0x800000, CRC(e24affe1) SHA1(5b4efbf9cdde8e49c26edba69b515d79b9b7acea) )
	ROM_LOAD32_WORD( "opr-24214a.ic18",  0x1000002, 0x800000, CRC(19bc5bca) SHA1(1c16ac929757a0c46456938bd9ccec7438130c66) )
	ROM_LOAD32_WORD( "opr-24215a.ic19s", 0x2000000, 0x800000, CRC(fed7750e) SHA1(61af9799a73ddc80e6763d44b23cbf8db497f144) )
	ROM_LOAD32_WORD( "opr-24216a.ic20",  0x2000002, 0x800000, CRC(635e5365) SHA1(e22f3fb0ac03963596e36592bfce791b0ee183e9) )
	ROM_LOAD32_WORD( "opr-24224a.ic21s", 0x3000000, 0x800000, CRC(a86b9368) SHA1(d73b8250700d368d3b194170b507a1ec1c818287) )
	ROM_LOAD32_WORD( "opr-24225a.ic22",  0x3000002, 0x800000, CRC(dbdebe45) SHA1(916897ea376175b48652efb58cdf22350d2a64cf) )
	ROM_LOAD32_WORD( "opr-24226a.ic23s", 0x4000000, 0x800000, CRC(43edd5a1) SHA1(0992c355d3d7d86dfe425de4b823fa4b64c0ee46) )
	ROM_LOAD32_WORD( "opr-24227a.ic24",  0x4000002, 0x800000, CRC(0e75009a) SHA1(f897ba64b6c6d98a080ee45a6a5f10cd6a764d61) )
	ROM_LOAD32_WORD( "opr-24228a.ic25s", 0x5000000, 0x800000, CRC(e1343e87) SHA1(8ea4eb85b9cbc1d1ef59c661357fc3dd473143d0) )
	ROM_LOAD32_WORD( "opr-24229a.ic26",  0x5000002, 0x800000, CRC(cca7f957) SHA1(8e2035c5d96fef849cf7517219dd6f1ae1e0f84b) )
	ROM_LOAD32_WORD( "opr-24230a.ic27s", 0x6000000, 0x800000, CRC(71f52068) SHA1(c21d2ce2fa2e83d0048cc52c8c7dbece11780e2f) )
	ROM_LOAD32_WORD( "opr-24231a.ic28",  0x6000002, 0x800000, CRC(16f3a1f1) SHA1(c3893d536ac0c16793accaa5aeb166fef4035630) )
	ROM_LOAD32_WORD( "opr-24232a.ic29",  0x7000000, 0x800000, CRC(4cdbd372) SHA1(745645c959689d4f2234aba37694e851b272528d) )
	ROM_LOAD32_WORD( "opr-24233a.ic30s", 0x7000002, 0x800000, CRC(70d638c6) SHA1(ef8bd9860a587dc76f3915047a5e408ab9f1f9b6) )
	ROM_LOAD32_WORD( "opr-24234a.ic31",  0x8000000, 0x800000, CRC(f78deb2a) SHA1(7103333c7f388545ebba8e8bb9443ec5ea90589b) )
	ROM_LOAD32_WORD( "opr-24235a.ic32s", 0x8000002, 0x800000, CRC(a2bb0d26) SHA1(bafd66250f8ad472eaa179bd73edc0dc22b681f5) )
	ROM_LOAD( "25lc040.ic13s", 0x9000000, 0x000084, CRC(19d77c96) SHA1(1d82af6b11f7fde93a3c4dd3561f1f2ab74c8d65) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "kick4csh-key.bin", 0, 4, CRC(889d2ea1) SHA1(daf7acf41b6bc607d443a93221a3e4554b99547f) )
ROM_END

ROM_START( wrungp )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x6800000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD( "epr-24059.ic22", 0x0000000, 0x400000, CRC(f82c00b5) SHA1(e885a46b2d3d52d9222d9ce038766858a2046ea1) )
	ROM_LOAD( "mpr-23719.ic1",  0x0800000, 0x1000000, CRC(b9fb79df) SHA1(1568320c25118f4ee5c508dcca4e4496ff23c067) )
	ROM_LOAD( "mpr-23720.ic2",  0x1800000, 0x1000000, CRC(d3f19874) SHA1(cde22c56dac233f5407d2e3ac8e6ea4f8681d0bf) )
	ROM_LOAD( "mpr-23721.ic3",  0x2800000, 0x1000000, CRC(f599a52e) SHA1(ca0edc2e9496f218117cef7d71bf1761bed8d4ac) )
	ROM_LOAD( "mpr-23722.ic4",  0x3800000, 0x1000000, CRC(e08a6a36) SHA1(ef37d8c7bc9d5055008d522825ef3e80e27745c2) )
	ROM_LOAD( "mpr-23723.ic5",  0x4800000, 0x1000000, CRC(651610eb) SHA1(4dfe4f876a5440bd1034f41a4d76e1d6bd3e0e32) )
	ROM_LOAD( "mpr-23724.ic6",  0x5800000, 0x1000000, CRC(c633c45a) SHA1(23b45140f965428d33e2424b0574715c0b952d05) )
ROM_END

ROM_START( gundmct )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0xa800000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD( "epr-23638.ic22", 0x0000000, 0x0400000, CRC(03e8600d) SHA1(bf9bb6ed03a5744c91c2c4038be764408dd85234) )
	ROM_RELOAD( 0x400000, 0x400000)
	ROM_LOAD( "mpr-23628.ic1",  0x0800000, 0x1000000, CRC(8668ba2f) SHA1(cedc67e6ce267a8c99ced4728f891bcae01cce24) )
	ROM_LOAD( "mpr-23629.ic2",  0x1800000, 0x1000000, CRC(b60f3048) SHA1(e575547e00b93129b1da49c61fc2a56706e8f362) )
	ROM_LOAD( "mpr-23630.ic3",  0x2800000, 0x1000000, CRC(0b47643f) SHA1(3cc4e51ca85ecdd04fe7c91e3b877dd5e6c0e67e) )
	ROM_LOAD( "mpr-23631.ic4",  0x3800000, 0x1000000, CRC(dbd863c9) SHA1(0c5d3b76a56902861e9a9595c0997f3726583cda) )
	ROM_LOAD( "mpr-23632.ic5",  0x4800000, 0x1000000, CRC(8c008562) SHA1(3fd696fadafd012b312a1407345c1ce1cd41ca45) )
	ROM_LOAD( "mpr-23633.ic6",  0x5800000, 0x1000000, CRC(ca104486) SHA1(05e6d1f9481a13189739671620a96a41af91433e) )
	ROM_LOAD( "mpr-23634.ic7",  0x6800000, 0x1000000, CRC(32bf6938) SHA1(f7bc0bed0a26c6d964c147fa78c666fd830730cf) )
	ROM_LOAD( "mpr-23635.ic8",  0x7800000, 0x1000000, CRC(f9279277) SHA1(823ae02a754ca8a8effdb957ccc6726765fc29b8) )
	ROM_LOAD( "mpr-23636.ic9",  0x8800000, 0x1000000, CRC(57199e9f) SHA1(73a6f20ee7b3133ed4c6286e477e2ff9757106bd) )
	ROM_LOAD( "mpr-23637.ic10", 0x9800000, 0x1000000, CRC(737b5dff) SHA1(0a405b711ffb096a3e6d52ececed73a5f93ebf02) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "gundmct-key.bin", 0, 4, CRC(0dee7a39) SHA1(24eef111ad7d8ad984e38b63101d01463b36542e) )
ROM_END

ROM_START( puyoda )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0xa800000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD( "epr-22206.ic22", 0x0000000, 0x400000, CRC(3882dd01) SHA1(9c287b531d0adfd9ecb10d5bf71a7b0f17454c21) )
	ROM_RELOAD( 0x400000, 0x400000)
	ROM_LOAD( "mpr-22186.ic1",  0x0800000, 0x800000, CRC(30b1a1d6) SHA1(73914c53a030b496e854b4c1fa454153bb025217) )
	ROM_LOAD( "mpr-22187.ic2",  0x1000000, 0x800000, CRC(0eae60e5) SHA1(c695c07000310089aa79b525fbe36015c5526165) )
	ROM_LOAD( "mpr-22188.ic3",  0x1800000, 0x800000, CRC(2e651f16) SHA1(b4ef7a90ade379cb3f7d4c64faedb25032465c25) )
	ROM_LOAD( "mpr-22189.ic4",  0x2000000, 0x800000, CRC(69ec44fc) SHA1(052e5e14ce433cddfae4a8b9b7c179c6266e9c1c) )
	ROM_LOAD( "mpr-22190.ic5",  0x2800000, 0x800000, CRC(d86bef21) SHA1(8c822438cf81023d83985d6800e7e3884f5c6a55) )
	ROM_LOAD( "mpr-22191.ic6",  0x3000000, 0x800000, CRC(b52364cd) SHA1(cc1ca522e6d0085a9bdf286e88aacb2041669daf) )
	ROM_LOAD( "mpr-22192.ic7",  0x3800000, 0x800000, CRC(08f09148) SHA1(c6a248199823f281cb9a9ac8080ebcae331d7e6f) )
	ROM_LOAD( "mpr-22193.ic8",  0x4000000, 0x800000, CRC(be5f58a8) SHA1(0d9f61182878540596909b2559158e03ffbd75c8) )
	ROM_LOAD( "mpr-22194.ic9",  0x4800000, 0x800000, CRC(2d0370fd) SHA1(f52587d6c2c06e2d872375f4ab0f0a9e11e932c3) )
	ROM_LOAD( "mpr-22195.ic10", 0x5000000, 0x800000, CRC(ccc8bb28) SHA1(d4769c9c8e4c1cdda53f8cb08b57f77c58d27c6f) )
	ROM_LOAD( "mpr-22196.ic11", 0x5800000, 0x800000, CRC(d65aa87b) SHA1(97f519a9c5b6bc6fc08e856d8c09fb69fad2bb04) )
	ROM_LOAD( "mpr-22197.ic12", 0x6000000, 0x800000, CRC(2c79608e) SHA1(01a09398c4f18e9368fddaca6b0ba520b07ca962) )
	ROM_LOAD( "mpr-22198.ic13", 0x6800000, 0x800000, CRC(b5373909) SHA1(6ff02c52a41da3d61e3f45b70fbcfddd4315fdfb) )
	ROM_LOAD( "mpr-22199.ic14", 0x7000000, 0x800000, CRC(4ba34fd9) SHA1(b681efb05df4f38349e96f98f38442db9db1f83a) )
	ROM_LOAD( "mpr-22200.ic15", 0x7800000, 0x800000, CRC(eb3d4a5e) SHA1(747bea94d224d1753e3dea27319d16fbca706459) )
	ROM_LOAD( "mpr-22201.ic16", 0x8000000, 0x800000, CRC(dce19598) SHA1(0081fbb74731f0b639a742fd4e2f5685ffe6887a) )
	ROM_LOAD( "mpr-22202.ic17", 0x8800000, 0x800000, CRC(f3ac92a6) SHA1(6583a3f3d1659d00dcffc98c6d3391f1aac03338) )
	ROM_LOAD( "mpr-22203.ic18", 0x9000000, 0x800000, CRC(881d6316) SHA1(c7a26404759afac346c63e39b35bf408f1a897a6) )
	ROM_LOAD( "mpr-22204.ic19", 0x9800000, 0x800000, CRC(2c5e5140) SHA1(7887fc19459dc85ca78256e0c50c762eea001e51) )
	ROM_LOAD( "mpr-22205.ic20", 0xa000000, 0x800000, CRC(7d523ae5) SHA1(7495082b7e83b6ee8f47660baba4c604d8ba2ff1) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "puyoda-key.bin", 0, 4, CRC(245bb259) SHA1(7ab675779ebf01ad3a78edd39cb287b71acd8bb5) )
ROM_END

ROM_START( zerogu2 )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x5800000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD( "epr23689.bin", 0x0000000, 0x0400000, CRC(ba42267c) SHA1(e8166e33cc2a4d9b2c57410235f531651b2e7f8a) )
	ROM_LOAD( "mpr23684.ic1", 0x0800000, 0x1000000, CRC(035aec98) SHA1(47ea834ca88aca3a72d2e7ef715a64603c40eacd) )
	ROM_LOAD( "mpr23685.ic2", 0x1800000, 0x1000000, CRC(d878ff99) SHA1(577be93d43f6113b91cf0967b710c6cf45131713) )
	ROM_LOAD( "mpr23686.ic3", 0x2800000, 0x1000000, CRC(a61b4d49) SHA1(842c54ad4e8192e491152ae5bb08daf5dd6b8c6c) )
	ROM_LOAD( "mpr23687.ic4", 0x3800000, 0x1000000, CRC(e125439a) SHA1(07e7339f3f53aeb0ebddf7a8ac3eb6d8f3fe9de6) )
	ROM_LOAD( "mpr23688.ic5", 0x4800000, 0x1000000, CRC(38412edf) SHA1(9cae06cf46e134531f47e64deedace449664f69a) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "zerogu2-key.bin", 0, 4, CRC(f2460eb3) SHA1(69ec4e3933e60caa6a0ac44a855a40deed337d92) )
ROM_END

ROM_START( inunoos )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x8800000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD( "epr-22294a.ic22", 0x000000, 0x400000, CRC(bc3a1397) SHA1(73747f10ea034bd0fc952939c6c7576dab9640a6) )

	ROM_LOAD( "mpr-22278.ic1",  0x0800000, 0x800000, CRC(0afec6d7) SHA1(fad13f7c205d048006b48c1f262f272a9d653630) )
	ROM_LOAD( "mpr-22279.ic2",  0x1000000, 0x800000, CRC(80b4a397) SHA1(c7d41a48c327b8ae9fefb15149ad5341b86ff933) )
	ROM_LOAD( "mpr-22280.ic3",  0x1800000, 0x800000, CRC(a58fde28) SHA1(db271690131f093a329a457aa234659cbdba4ad1) )
	ROM_LOAD( "mpr-22281.ic4",  0x2000000, 0x800000, CRC(6fcc34f6) SHA1(28f4d69fdd4e151cf1d98303d5f7ac8ff6d4c141) )
	ROM_LOAD( "mpr-22282.ic5",  0x2800000, 0x800000, CRC(d5582d3e) SHA1(b21eda63314983fff7b88d2ad6ddcd8a2ef20e32) )
	ROM_LOAD( "mpr-22283.ic6",  0x3000000, 0x800000, CRC(24b9fa3b) SHA1(b85be6085c476c8683c6d9e9b72e6021385a376a) )
	ROM_LOAD( "mpr-22284.ic7",  0x3800000, 0x800000, CRC(bb80e02a) SHA1(73e7fda34295eb3b67ac04e46206a3a399b7b88a) )
	ROM_LOAD( "mpr-22285.ic8",  0x4000000, 0x800000, CRC(59e2e25b) SHA1(e32d59dca85f246781a411bd8d0bec8fabc3b26d) )
	ROM_LOAD( "mpr-22286.ic9",  0x4800000, 0x800000, CRC(41eac8af) SHA1(594efd94ea053229f0c807591b4f38643f0995c2) )
	ROM_LOAD( "mpr-22287.ic10", 0x5000000, 0x800000, CRC(811f6e65) SHA1(71448b91d2c68df119a767fc29692bbf115edb37) )
	ROM_LOAD( "mpr-22288.ic11", 0x5800000, 0x800000, CRC(ad1375e8) SHA1(17eeaad1becbf0ad0d68e10457c0d3dac9f168da) )
	ROM_LOAD( "mpr-22289.ic12", 0x6000000, 0x800000, CRC(7e7826c0) SHA1(6a13fb7471555732292ee8e4709ee191aee2528c) )
	ROM_LOAD( "mpr-22290.ic13", 0x6800000, 0x800000, CRC(38a192da) SHA1(501c25190af153d1364a90762c7994ac797a498d) )
	ROM_LOAD( "mpr-22291.ic14", 0x7000000, 0x800000, CRC(79ca1d44) SHA1(449509110a7e181e0e495fe3e1d21762b6e4ce08) )
	ROM_LOAD( "mpr-22292.ic15", 0x7800000, 0x800000, CRC(b85e13ef) SHA1(974f6b8f24efe79d72ea9d7a2cfccf479704243d) )
	ROM_LOAD( "mpr-22293.ic16", 0x8000000, 0x800000, CRC(b8493dbe) SHA1(b641417e1bda49341e7ff86340072d74e3330665) )
ROM_END

ROM_START( ringout )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x5800000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD( "epr-21779.ic22", 0x0000000, 0x400000, CRC(1d419767) SHA1(33065006ef437845abcf32a59e78f4bc202c4460) )
	ROM_LOAD( "mpr-21769.ic1",  0x0800000, 0x800000, CRC(493199fc) SHA1(189cd9a20c9207f0238e6d2e867f1479a476dfaa) )
	ROM_LOAD( "mpr-21770.ic2",  0x1000000, 0x800000, CRC(68173ace) SHA1(0869072915543dd5dfa6d3cdb95205521b4a3bf4) )
	ROM_LOAD( "mpr-21771.ic3",  0x1800000, 0x800000, CRC(41badfc3) SHA1(d4062bdbb994b8c18dac20948db9a9550f030865) )
	ROM_LOAD( "mpr-21772.ic4",  0x2000000, 0x800000, CRC(a8dfb537) SHA1(9f6c98ee23842b22fa3b701dad0a384a24a9bc6a) )
	ROM_LOAD( "mpr-21773.ic5",  0x2800000, 0x800000, CRC(3f1f5ed4) SHA1(a7d6e65fba9732efbd99e10a609f41dd03fd3bb8) )
	ROM_LOAD( "mpr-21774.ic6",  0x3000000, 0x800000, CRC(5a1114f0) SHA1(a45fc82bccb40e582f107b86403409651476180e) )
	ROM_LOAD( "mpr-21775.ic7",  0x3800000, 0x800000, CRC(5645a95c) SHA1(f0e97ff5b744972a1fa4e03e6d16a19ba20fb930) )
	ROM_LOAD( "mpr-21776.ic8",  0x4000000, 0x800000, CRC(22fc33dc) SHA1(922c41f7fb22b26037446bdfb4f9788a8e0c3e46) )
	ROM_LOAD( "mpr-21777.ic9",  0x4800000, 0x800000, CRC(6d22d29d) SHA1(382dcd62065437b34fe101144b1c047eb261f047) )
	ROM_LOAD( "mpr-21778.ic10", 0x5000000, 0x800000, CRC(c5308e61) SHA1(e51f8026351d5ffbda2a5bed39aeef543366febf) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "ringout-key.bin", 0, 4, CRC(8f8c4eb2) SHA1(9bc59c69d04d9ce876415af048f39095ed680a5e) )
ROM_END

ROM_START( vonot )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x7000000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD( "epr-23198.ic22", 0x0000000, 0x400000, CRC(9852eda2) SHA1(71ded8cbdf567afeff30bd593071ce3b7d84a260) )
	ROM_LOAD( "mpr-23185.ic1",  0x0800000, 0x800000, CRC(c4fc1d45) SHA1(cfeff71ac1cccf274f00731fe1ffc182fb85f7a6) )
	ROM_LOAD( "mpr-23186.ic2",  0x1000000, 0x800000, CRC(b17f9924) SHA1(a9aa9f10b76964042b337b25430acff072aaa7c7) )
	ROM_LOAD( "mpr-23187.ic3",  0x1800000, 0x800000, CRC(575f73e8) SHA1(fe6ffeab120ad788bd692b33eca1cf22db881ac3) )
	ROM_LOAD( "mpr-23188.ic4",  0x2000000, 0x800000, CRC(0004ef34) SHA1(926de996549421a5835f3c8c0895a07978519215) )
	ROM_LOAD( "mpr-23189.ic5",  0x2800000, 0x800000, CRC(554eea29) SHA1(c82c24270dee0dd7309b117e1632827ca314615d) )
	ROM_LOAD( "mpr-23190.ic6",  0x3000000, 0x800000, CRC(b74ae1c5) SHA1(003fadbaa03cf43757b686f316d1104d26ae9ce8) )
	ROM_LOAD( "mpr-23191.ic7",  0x3800000, 0x800000, CRC(9f2a88af) SHA1(b1caf9cbf026ee6d0f12ab66cddf120e7ba9884c) )
	ROM_LOAD( "mpr-23192.ic8",  0x4000000, 0x800000, CRC(83014196) SHA1(cc4801534e0f0e649fc3f368af4bd3ac01288732) )
	ROM_LOAD( "mpr-23193.ic9",  0x4800000, 0x800000, CRC(2fde8ecf) SHA1(6da95fb04f141f2c45564460c49834bb945fb1af) )
	ROM_LOAD( "mpr-23194.ic10", 0x5000000, 0x800000, CRC(92f045e5) SHA1(70f741e55a47682725c7d6dcdd0e60982187fc87) )
	ROM_LOAD( "mpr-23195.ic11", 0x5800000, 0x800000, CRC(9e708834) SHA1(2f454688ea5b8b041bbfffaa12047afad01d020f) )
	ROM_LOAD( "mpr-23196.ic12", 0x6000000, 0x800000, CRC(c86a5b9b) SHA1(4b8dda85003289e1464e12c3abf449bb8df20e3a) )
	ROM_LOAD( "mpr-23197.ic13", 0x6800000, 0x800000, CRC(5adea0bd) SHA1(f8614ba83d5f61556c3db1a1796a02ed2c51ce2a) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "vonot-key.bin", 0, 4, CRC(021ac707) SHA1(af57844331226d3b1722aedca1a7aef024468b39) )
ROM_END

ROM_START( derbyo2k )
	NAOMI_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x8800000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD( "epr-22284a.ic22", 0x0000000, 0x400000, CRC(1e8e067c) SHA1(c33d5f7b175828511f69a6489b4558aef1ac2517) )
	ROM_LOAD( "mpr-22223.ic1",   0x0800000, 0x800000, CRC(5159df22) SHA1(612c2d43f02952c7987cb08051933e406e601474) )
	ROM_LOAD( "mpr-22224.ic2",   0x1000000, 0x800000, CRC(ef8bc5a0) SHA1(ecb858788d9bb84cf25aaeef8d0bee3b7ae0233e) )
	ROM_LOAD( "mpr-22225.ic3",   0x1800000, 0x800000, CRC(80e0fdbd) SHA1(b111d469b8f3a7e68960cc7731ecfca4d24f4f32) )
	ROM_LOAD( "mpr-22226.ic4",   0x2000000, 0x800000, CRC(ea940bcf) SHA1(7dfa7f9101b8cd836920c7ebef095d1ec40c0bd3) )
	ROM_LOAD( "mpr-22227.ic5",   0x2800000, 0x800000, CRC(300ed93b) SHA1(a81e0ccef6d32a959beacf66f60a9cbda128c493) )
	ROM_LOAD( "mpr-22228.ic6",   0x3000000, 0x800000, CRC(3dd062c7) SHA1(5515c636d47b128479d7de7659fe2a2adae51fe4) )
	ROM_LOAD( "mpr-22229.ic7",   0x3800000, 0x800000, CRC(da624f37) SHA1(8b9bb8e572f969019aaa8100fd3549fb7f902f42) )
	ROM_LOAD( "mpr-22230.ic8",   0x4000000, 0x800000, CRC(5a806bb6) SHA1(0d9366d215f23e3f54b26eda960a7033f3ac033c) )
	ROM_LOAD( "mpr-22231.ic9",   0x4800000, 0x800000, CRC(c633d57d) SHA1(737b55f5a9d0b34dec1e4b2bef7bd14e9a9e1bed) )
	ROM_LOAD( "mpr-22232.ic10",  0x5000000, 0x800000, CRC(6f25a6dd) SHA1(d4f354f3143b9484b493cdd87c25b94bd6e7c09a) )
	ROM_LOAD( "mpr-22233.ic11",  0x5800000, 0x800000, CRC(cf3ac2df) SHA1(4d6dfa030ef8ec6343153afd4bdca51a51065bb4) )
	ROM_LOAD( "mpr-22234.ic12s", 0x6000000, 0x800000, CRC(d8c41648) SHA1(d465f4b841164da0738336e203c5bc6e1e799a76) )
	ROM_LOAD( "mpr-22235.ic13s", 0x6800000, 0x800000, CRC(f1dedac5) SHA1(9d4499cbafe80dd0b36be617de7994a96e1e9a01) )
	ROM_LOAD( "mpr-22236.ic14s", 0x7000000, 0x800000, CRC(85f54964) SHA1(4592232694de75e245d1c67f506c9b9d7b0af53a) )
	ROM_LOAD( "mpr-22237.ic15s", 0x7800000, 0x800000, CRC(718dd6bf) SHA1(6b71bb6970b582865f53d26e9579587fce86439e) )
	ROM_LOAD( "mpr-22238.ic16s", 0x8000000, 0x800000, CRC(fb3e55da) SHA1(d547ee5b47e6e6fec9e447460300c828fbff8f2e) )
ROM_END


/* GD-ROM titles - a PIC supplies a decryption key

PIC stuff

command             response                   comment

kayjyo!?          ->:\x70\x1f\x71\x1f\0\0\0    (unlock gdrom)
C1strdf0          ->5BDA.BIN                   (lower part of boot filename string, BDA.BIN in this example)
D1strdf1          ->6\0\0\0\0\0\0\0            (upper part of filename string)
bsec_ver          ->8VER0001                   (always the same? )
atestpic          ->7TEST_OK                   (always the same? )
AKEYCODE          ->3.......                   (high 7 bytes of des key)
Bkeycode          ->4.\0\0\0\0\0\0             (low byte of des key, then \0 fill)
!.......          ->0DIMMID0                   (redefine upper 7 bytes of session key)
".......          ->1DIMMID1                   (redefine next 7 bytes)
#..               ->2DIMMID2                   (last 2 bytes)


default session key is
"NAOMIGDROMSYSTEM"

info from Elsemi:

it sends bsec_ver, and if it's ok, then the next commands are the session key changes
if you want to have the encryption described somewhere so it's not lost. it's simple:
unsigned char Enc(unsigned char val,unsigned char n)
{
    val^=Key[8+n];
    val+=Key[n];

    return val;
}

do for each value in the message to send
that will encrypt the char in the nth position in the packet to send
time to go to sleep


*/

#ifdef UNUSED_FUNCTION
// rather crude function to write out a key file
void naomi_write_keyfile(void)
{
	// default key structure
	UINT8 response[10][8] = {
	{ ':', 0x70, 0x1f, 0x71, 0x1f, 0x00, 0x00, 0x00 }, // response to kayjyo!?
	{ '8', 'V',  'E',  'R',  '0',  '0',  '0',  '1'  }, // response to bsec_ver
	{ '7', 'T',  'E',  'S',  'T',  '_',  'O',  'K'  }, // response to atestpic
	{ '6', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // response to D1strdf1 (upper part of filename)
	{ '5', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // response to C1strdf0 (lower part of filename)
	{ '4', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // response to Bkeycode (lower byte of DES key)
	{ '3', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // response to AKEYCODE (high 7 bytes of DES key)
	{ '2', 'D',  'I',  'M',  'M',  'I',  'D',  '2'  }, // response to #..      (rewrite low 2 bytes of session key)
	{ '1', 'D',  'I',  'M',  'M',  'I',  'D',  '1'  }, // response to "....... (rewrite middle 7 bytes of session key)
	{ '0', 'D',  'I',  'M',  'M',  'I',  'D',  '0'  }, // response to !....... (rewrite upper 7 bytes of session key)
	};

	int i;
	char bootname[256];
	char picname[256];

	// ######### edit this ###########
	UINT64 key = 0x4FF16D1A9E0BFBCDULL;

	memset(bootname,0x00,14);
	memset(picname,0x00,256);

	// ######### edit this ###########
	strcpy(picname,"317-5072-com.data");
	strcpy(bootname,"BCY.BIN");

	for (i=0;i<14;i++)
	{
		if (i<7)
		{
			response[4][i+1] = bootname[i];
		}
		else
		{
			response[3][i-6] = bootname[i];
		}
	}

	for (i=0;i<8;i++)
	{
		UINT8 keybyte = (key>>(7-i)*8)&0xff;

		if (i<7)
		{
			response[6][i+1] = keybyte;
		}
		else
		{
			response[5][1] = keybyte;
		}
	}


	{
		FILE *fp;
		fp=fopen(picname, "w+b");
		if (fp)
		{
			fwrite(response, 10*8, 1, fp);
			fclose(fp);
		}
	}


}
#endif


/**********************************************
 *
 * Naomi GD-ROM defines
 *
 *********************************************/

ROM_START( gundmgd )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x80, "mie_eeprom", 0 )
	ROM_LOAD("gundmgd-default-eeprom.bin", 0, 0x80, CRC(dc80fa1e) SHA1(5a412576b9fd4899ab0c11f93257600a5eb8b994))

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gdl-0001", 0, SHA1(615e19c22f32096f3aad557019a14313b60a4070) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C622A (317-5069-COM)
	//(sticker 253-5509-5069)
	ROM_LOAD("317-5069-com.pic", 0x00, 0x4000, CRC(44d0b242) SHA1(cac31c2ed317e2b44ee93d762188aacea2398949) )

ROM_END


ROM_START( sfz3ugd )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x80, "mie_eeprom", 0 )
	ROM_LOAD("sfz3ugd-default-eeprom.bin", 0, 0x80, CRC(699dd01b) SHA1(1a1e6fd1e47ed58a2afbf7f632fccf72a4708531))

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gdl-0002", 0, SHA1(af4669fdd7ce8e6ec4a170748d401e322a3d7ae8) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C622A (317-5072-COM)
	//(sticker 253-5509-5072)
	ROM_LOAD("317-5072-com.pic", 0x00, 0x4000, CRC(3238ba01) SHA1(07c28f17c19eaa652295bbf2d3a96aa27c3748ae) )
ROM_END


ROM_START( cvsgd )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gdl-0004", 0,  SHA1(7a7fba0fbbc769c5120b08e6d692f1ac63a42225) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C622A (317-5076-JPN)
	//(sticker 253-5509-5076J)
	ROM_LOAD("317-5076-jpn.pic", 0x00, 0x4000, CRC(7c125b10) SHA1(557675e33bb45e4969560bbfd61f48d1784a728d) )
ROM_END


ROM_START( starseek )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gdl-0005", 0, SHA1(f5513c88528741798ee49f936b9b69480360f103) )

	/*
    Note:
    There was no sticker or silk screen markings on this PIC.
    I have used the product number as a label instead.

    PIC16C621A-20 (317-0005-JPN)
    Sticker: 253-5509-0005J
    */
	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	ROM_LOAD("317-0005-jpn.pic", 0x00, 0x4000, CRC(19f8d4d0) SHA1(d256f26f757d7019cab7950d81992902cdb65e07) )
ROM_END


ROM_START( gundmxgd )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x80, "mie_eeprom", 0 )
	ROM_LOAD("gundmxgd-default-eeprom.bin", 0, 0x80, CRC(dc0e8d45) SHA1(4088d25fdf7399552882b9656b66dff2345c376e))

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gdl-0006", 0, SHA1(b28d6598711b5a9c744bbf07ad03fc60962d2e28) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C622A (317-5079-COM)
	//(sticker 253-5509-5079)
	ROM_LOAD("317-5079-com.pic", 0x00, 0x4000, CRC(8f9fb55d) SHA1(ca93814ae7a4e99762dd1c2a743e21402b143811) )
ROM_END


ROM_START( cvs2gd )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gdl-0007a", 0, SHA1(56510390667b39b3915d8bc078660cbe093cf566) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C622A (317-5078-COM)
	//(sticker 253-5509-5078)
	ROM_LOAD("317-5078-com.pic", 0x00, 0x4000, CRC(e7bb621d) SHA1(0882d0e12ca4fb81dda2268cd12724a10278c220) )
ROM_END


ROM_START( ikaruga )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gdl-0010", 0, SHA1(65dcc22dd9e9b70975096464ad8e31a4a73dc5fd) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C622A (317-5081-JPN)
	//(sticker 253-5509-5081J)
	ROM_LOAD("317-5081-jpn.pic", 0x00, 0x4000, CRC(72ca4579) SHA1(8a46e92fc4a32016438ea877807928b51b3f3861) )
ROM_END


ROM_START( ggxx )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gdl-0011", 0, SHA1(b7328eb2c588d55284bdcea0fe89bb8e629a8669) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C622A (317-5082-COM)
	//(sticker 253-5509-5082)
	ROM_LOAD("317-5082-com.pic", 0x00, 0x4000, CRC(1b41189b) SHA1(efa0bf233ea4f64a8ed1c7a72b37de40ed069f33) )
ROM_END


ROM_START( cleoftp )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gdl-0012", 0, SHA1(aae4c1321fdee37d5405c6cbe648e0596624ed13) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C622A (317-5083-COM)
	//(sticker 253-5509-5083)
	ROM_LOAD("317-5083-com.pic", 0x00, 0x4000, CRC(096a0fc2) SHA1(8498a0d489a8e075268c07e2c076673904b04be9) )
ROM_END


ROM_START( moeru )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x80, "mie_eeprom", 0 )
	ROM_LOAD("moeru-default-eeprom.bin", 0, 0x80, CRC(50ca079f) SHA1(788a399017b94d9d1a981ea703af0d4a178dadb6))

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gdl-0013", 0, SHA1(c8869069c28bc8eec96d820886bc388d69d46143) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C622A (317-5084-JPN)
	//(sticker 253-5509-5084J)
	ROM_LOAD("317-5084-jpn.pic", 0x00, 0x4000, CRC(db7dac1e) SHA1(b6f7afe9d9d2681005c1abcd4fde24867e65d1e4) )
ROM_END


ROM_START( chocomk )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gdl-0014a", 0, SHA1(f88d8203c8692f51c9492d5549a3ad7d9583dc6f) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C622A (317-5085-JPN)
	//(sticker 253-5509-5085J)
	ROM_LOAD("317-5085-jpn.pic", 0x00, 0x4000,  CRC(677fd544) SHA1(cccd4931bfe3fbcfcde6722088961ddf29a45e89) )
ROM_END


ROM_START( quizqgd )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x80, "mie_eeprom", 0 )
	ROM_LOAD("quizqgd-default-eeprom.bin", 0, 0x80, CRC(46c10aa3) SHA1(0a082243399a45c1c9d757f59ed660b3b7a9730d))

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gdl-0017", 0, SHA1(94a9319633388968611892e36691b45c94b4f83f) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C622A (317-5090-JPN)
	//(sticker 253-5509-5090J)
	ROM_LOAD("317-5090-jpn.pic", 0x00, 0x4000, CRC(141cced2) SHA1(c7fe2fc61be3585e95a2ce4c6d4373ea71e920de) )
ROM_END

ROM_START( azumanga )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gdl-0018", 0, SHA1(3e40ca7d43173fe7048d199fdc127b9411e10360) )

	ROM_REGION( 0x50, "pic", ROMREGION_ERASE)
	//PIC16C622A (317-5091-JPN)
	//(sticker 253-5509-5091J)
	ROM_LOAD("317-5091-jpn.data", 0x00, 0x50, CRC(ca589c79) SHA1(ac44bfdfc8db2f9dbe4d563205719524bf43a674) )
ROM_END

ROM_START( ggxxrl )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gdl-0019a", 0, SHA1(d44906505ff698eda6feee6c2b9402e19f64e5d3) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C622A (317-5092-JPN)
	//(sticker 253-5509-5092J)
	ROM_LOAD("317-5092-jpn.pic", 0x00, 0x4000, CRC(7ad7b541) SHA1(45c1e3da030add3bb07797ee7f22003224ae3f7f) )
ROM_END


ROM_START( tetkiwam )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x80, "mie_eeprom", 0 )
	ROM_LOAD("tetkiwam-default-eeprom.bin", 0, 0x80, CRC(843f2a99) SHA1(6615f5f27e76a71f7415f972bbcdf6570b0e953a))

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gdl-0020", 0, SHA1(7b2ef47ca2038d6a93615b760b03e8f7cb1b83c2) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C622A (317-5093-JPN)
	//(sticker 253-5509-5093J)
	ROM_LOAD("317-5093-jpn.pic", 0x00, 0x4000, CRC(a61e1e2a) SHA1(ccbec76da6454d4d2384a2adb3f8b62aa1fece24) )
ROM_END


ROM_START( shikgam2 )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x80, "mie_eeprom", 0 )
	ROM_LOAD("shikgam2-default-eeprom.bin", 0, 0x80, CRC(5fb60e27) SHA1(a64242083a718f0a4b1d2e4707f5eb7480265719))

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gdl-0021", 0, SHA1(f5036711a28a211e8d71400a8322db3172c5733f) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C622A (317-5095-JPN)
	//(sticker 253-5509-5095J)
	ROM_LOAD("317-5095-jpn.pic", 0x00, 0x4000, CRC(7c25cb5c) SHA1(02797e890030ddf2df470e85ebd6c539f6621e53) )
ROM_END


ROM_START( usagiym )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x80, "mie_eeprom", 0 )
	ROM_LOAD("usagiym-default-eeprom.bin", 0, 0x80, CRC(1fbdf0ca) SHA1(5854c693b7d6451cefeb737109aeaf64751fc4f7))

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gdl-0022", 0, SHA1(45deba05a12abbf6390c0fc0e4cdeaedfa7d2ca5) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C622A (317-5096-JPN)
	//(sticker 253-5509-5096J)
	ROM_LOAD("317-5096-jpn.pic", 0x00, 0x4000, CRC(2d16887b) SHA1(32d11691c3d1242b16bc3fbcc0f1157bb16436e0) )
ROM_END


ROM_START( bdrdown )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x80, "mie_eeprom", 0 )
	ROM_LOAD("bdrdown-default-eeprom.bin", 0, 0x80, CRC(5b19727c) SHA1(1dd9c721d58e4542d04afe17baa77980d0ed8b6a))

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gdl-0023a", 0, SHA1(caac915104d61f2122f5afe27da1ef5fa9cf9f9a) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C622A (317-5097-JPN)
	//(sticker 253-5509-5097J)
	ROM_LOAD("317-5097-jpn.pic", 0x00, 0x4000, CRC(16d2a748) SHA1(5358f89c26427428840fd9af7d584a55db5a76de) )

ROM_END


ROM_START( psyvar2 )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x80, "mie_eeprom", 0 )
	ROM_LOAD("psyvar2-default-eeprom.bin", 0, 0x80, CRC(9d8661f3) SHA1(c696277a7b488bee6ddb33a1d5345a85c1567cbe))

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gdl-0024", 0,  SHA1(d346762036fb1c40a261a434b50e63459f306f14) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C621A (317-5100-JPN)
	//(sticker 253-5509-5100J)
	ROM_LOAD("317-5100-jpn.pic", 0x00, 0x4000, CRC(f37a1dbe) SHA1(a0b43069c9ecd5633418404344b7750db5371ac4) )
ROM_END


ROM_START( cfield )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x80, "mie_eeprom", 0 )
	ROM_LOAD("cfield-default-eeprom.bin", 0, 0x80, CRC(a7acb6bf) SHA1(5aae6366bfb3ee3120da405abb93e2007cd94683))

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gdl-0025", 0, SHA1(be0d88eb4f48403a2ceaa7ef588ed60b96ba93bf) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C621A (317-5102-COM)
	//(sticker 253-5509-5102)
	ROM_LOAD("317-5102-com.pic", 0x00, 0x4000, CRC(8f1d8387) SHA1(1f9427aca21b2de44959cd510b5f9105b845a532) )
ROM_END


ROM_START( trizeal )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x80, "mie_eeprom", 0 )
	ROM_LOAD("trizeal-default-eeprom.bin", 0, 0x80, CRC(ac0847ce) SHA1(ec12a6bbf074bf3bfe2e9bfe2855b7bd7e699f3c))

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gdl-0026", 0, SHA1(e4c1e51292a7923b25bfc61d38fe386bf596002a) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C621A (317-5103-JPN)
	//(sticker 253-5509-5103J)
	ROM_LOAD("317-5103-jpn.pic", 0x00, 0x4000, CRC(93feaff4) SHA1(0b362e9794c83b43e23a3d7299ff0b69f4740481) )
ROM_END


ROM_START( meltybld )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gdl-0028c", 0, SHA1(66de09738551e351784cc9695a58b35fdf6b6c4b) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C622A (317-5104-JPN)
	//(sticker 253-5509-5104J)
	ROM_LOAD("317-5104-jpn.pic", 0x00, 0x4000, CRC(afa5e709) SHA1(c107f6c5b7574f2c7e7ac6ed1fcc37edabdc95e8) )
ROM_END


ROM_START( senko )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x80, "mie_eeprom", 0 )
	ROM_LOAD("senko-default-eeprom.bin", 0, 0x80, CRC(b3d3be09) SHA1(55af4f6e35f82f683682bf731d3070bc275d6e57))

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gdl-0030a", 0,  SHA1(1f7ade47e37a0026451b5baf3ba746400de8d156) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C622A (317-5107-JPN)
	//(sticker 253-5509-5107J)
	ROM_LOAD("317-5107-jpn.pic", 0x00, 0x4000, CRC(6bc3fad0) SHA1(6d5196265232f4c0715a97acc84d6f7376056894) )
ROM_END

ROM_START( senkoo )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x80, "mie_eeprom", 0 )
	ROM_LOAD("senkoo-default-eeprom.bin", 0, 0x80, CRC(a2203a7f) SHA1(2a3a52667b9c8e0c9b4e4003b7c6965cd4de11f3))

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gdl-0030", 0,  SHA1(c7f25c05f47a490c5da9369c588b6136e93c280e) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C622A (317-5107-JPN)
	//(sticker 253-5509-5107J)
	ROM_LOAD("317-5107-jpn.pic", 0x00, 0x4000, CRC(6bc3fad0) SHA1(6d5196265232f4c0715a97acc84d6f7376056894) )
ROM_END


ROM_START( ss2005 )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x80, "mie_eeprom", 0 )
	ROM_LOAD("ss2005-default-eeprom.bin", 0, 0x80, CRC(26bd9003) SHA1(f35551c96c49eef5473ff50a94b82ef5110b0f10))

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gdl-0031", 0, SHA1(83697f45f0ffbb964f0504b19171c554499cd709) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C622A (317-5108-JPN)
	//(sticker 253-5509-5108J)
	ROM_LOAD("317-5108-jpn.pic", 0x00, 0x4000, CRC(4fa7dede) SHA1(f9011e951378364a12512d398f76be174dccce69) )
ROM_END


ROM_START( ss2005a )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gdl-0031a", 0, SHA1(6091525845fc2042ed43cae5a1b60c603e16cf97) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C622A (317-5108-JPN)
	//(sticker 253-5509-5108J)
	ROM_LOAD("317-5108-jpn.pic", 0x00, 0x4000, CRC(4fa7dede) SHA1(f9011e951378364a12512d398f76be174dccce69) )
ROM_END


ROM_START( radirgy )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x80, "mie_eeprom", 0 )
	ROM_LOAD("radirgy-default-eeprom.bin", 0, 0x80, CRC(8d60a282) SHA1(6d81dec88a1ade45e1edf2bdb3683c6cd0651eeb))

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gdl-0032", 0, SHA1(ebd7a40e59082e660ebf9a2d4ae7cb64371dae8d) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C621A (317-5110-JPN)
	//(sticker 253-5509-5110J)
	ROM_LOAD("317-5110-jpn.pic", 0x00, 0x4000, CRC(829d06e2) SHA1(c53d791e82cc75f2bcd49575185c89d448fed672) )
ROM_END


ROM_START( ggxxsla )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gdl-0033a", 0, SHA1(29de69ae97a9099b1bbe936dfa965bb4a3195f68) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C621A (317-5111-JPN)
	//(sticker 253-5509-5111J)
	ROM_LOAD("317-5111-jpn.pic", 0x00, 0x4000, CRC(96bcbd42) SHA1(af4efdf2a02920af9885d104091da0584fca988c) )
ROM_END


ROM_START( kurucham )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gdl-0034", 0, SHA1(10fd7edb0b620133c003d686e5af2ed27004fa09) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C621A (317-5115-JPN)
	//(sticker 253-5509-5115J)
	ROM_LOAD("317-5115-jpn.pic", 0x00, 0x4000, CRC(e5435e85) SHA1(9d5b25de82284e5fcab2cbf7fb73669d1130648a) )
ROM_END


ROM_START( undefeat )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x80, "mie_eeprom", 0 )
	ROM_LOAD("undefeat-default-eeprom.bin", 0, 0x80, CRC(9d2b071c) SHA1(88d90c23b9c2a6aa61bdf318d074a9cfa5c145e5))

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gdl-0035", 0, SHA1(91da482a6a082e48bee5b3bd20d9c92d23936965) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C622A (317-5117-JPN)
	//(sticker 253-5509-5117J)
	ROM_LOAD("317-5117-jpn.pic", 0x00, 0x4000,  CRC(61e65ca8) SHA1(f1a242d3dd1af0df084dd1568320f6b4c51d9e20) )
ROM_END


ROM_START( trgheart )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x80, "mie_eeprom", 0 )
	ROM_LOAD("trgheart-default-eeprom.bin", 0, 0x80, CRC(7faff313) SHA1(1bc25e4595ef050e82eb820842ba6ccd63b6703e))

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gdl-0036a", 0, SHA1(91f1e19136997cb1e2edfb1ad342b9427d1d3bfb) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C622A (317-5121-JPN)
	//(sticker 253-5509-5121J)
	ROM_LOAD("317-5121-jpn.pic", 0x00, 0x4000, CRC(cdb9b179) SHA1(8f7d1e9a99ad90344449c6ebb623e2968f611ec0) )
ROM_END


ROM_START( jingystm )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gdl-0037", 0, SHA1(99ffe2987e3002b3871daf276d2be45f2e9c6e74) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C622A (317-5122-JPN)
	//(sticker 253-5509-5122J)
	ROM_LOAD("317-5122-jpn.pic", 0x00, 0x4000, CRC(88983220) SHA1(410ee292794c44d2249778c8b6adda023286eb04) )
ROM_END


ROM_START( senkosp )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gdl-0038", 0, SHA1(430572f7e76c4fe009e2de91bf891d85613366b7) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C622A (317-5123-COM)
	//(sticker 253-5509-5123)
	ROM_LOAD("317-5123-com.pic", 0x00, 0x4000, CRC(7340df6e) SHA1(003a94e986335e959f9b1e8905fea8cbfdb5b2a1) )
ROM_END


ROM_START( meltyb )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gdl-0039", 0, SHA1(ffc7f6e113ad69422a4f22f318bdf9b1dc5c25db) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C622A (317-5124-JPN)
	//(sticker 253-5509-5124J)
	ROM_LOAD("317-5124-jpn.pic", 0x00, 0x4000, CRC(ad162bfa) SHA1(0e9740ba65a724eb7cd70fb897e5cd9ac17aa55c) )
ROM_END

ROM_START( meltyba )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gdl-0039a", 0, SHA1(e6aa3d65b43a20606e6754bcb8665438770a1f8c) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C622A (317-5124-JPN)
	//(sticker 253-5509-5124J)
	ROM_LOAD("317-5124-jpn.pic", 0x00, 0x4000, CRC(ad162bfa) SHA1(0e9740ba65a724eb7cd70fb897e5cd9ac17aa55c) )
ROM_END


ROM_START( karous )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x80, "mie_eeprom", 0 )
	ROM_LOAD("karous-default-eeprom.bin", 0, 0x80, CRC(b017451c) SHA1(a16d8e2cde8ebe0e2dd6d0b5c027bcdff56a809b))

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gdl-0040", 0, SHA1(a62c8d4b6c5be44a4aeeea1a1a94f3d0fe542593) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C621A (317-5125-COM)
	//(sticker 253-5509-5125)
	ROM_LOAD("317-5125-com.pic", 0x00, 0x4000, CRC(918efc4f) SHA1(e32502b8df0b432eebaf0286176dd3bcd3f65dbb) )
ROM_END

/*

Title             GUILTY GEAR XX ACCENT CORE
Media ID          D0D2
Media Config      GD-ROM1/1
Regions           J
Peripheral String 0000000
Product Number    GDL-0041
Version           V1.002
Release Date      20061107

TOC         DISC
Track       Start Sector  End Sector   Track Size
track01.bin          150         599      1058400
track02.raw          750        2882      5016816
track03.bin        45150      549299   1185760800

PIC16C621A-20 (317-5126-JPN)
Sticker: 253-5509-5126J
VER0001, TEST_OK, BMX.BIN, '70 1F 71 1F' 381F91D9624051F2

*/
ROM_START( ggxxac )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gdl-0041", 0, SHA1(3a589e1c79ead971c821a7566032b0187e339684) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C621A (317-5126-JPN)
	//(sticker 253-5509-5126J)
	ROM_LOAD("317-5126-jpn.pic", 0x00, 0x4000, CRC(87c44284) SHA1(a53bcead58844fb877bc657dd5b09c1a380872ee) )
ROM_END


ROM_START( takoron )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gdl-0042", 0, SHA1(984a4fa012d83dd8c748304958c847c9867f4125) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C621A (317-5127-JPN)
	//(sticker 253-5509-5127J)
	ROM_LOAD("317-5127-jpn.pic", 0x00, 0x4000, CRC(870c55eb) SHA1(cd8861726047250882c73a5f0c2480f45c30f21b) )
ROM_END

/* -------------------------------- 1st party -------------- */


/*
Title   CONFIDENTIAL MISSION
Media ID    FFCA
Media Config    GD-ROM1/1
Regions J
Peripheral String   0000000
Product Number  GDS-0001
Version V1.050
Release Date    20001011
Manufacturer ID SEGA ENTERPRISES
TOC DISC
Track   Start Sector    End Sector  Track Size
track01.bin 150 3788    8558928
track02.raw 3939    6071    5016816
track03.bin 45150   549299  1185760800


PIC
317-0298-COM
*/

ROM_START( confmiss )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gds-0001", 0, SHA1(bd05f197ba8643577883dd25d9d5a74c91b27ca9) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	ROM_LOAD("317-0298-com.pic", 0x00, 0x4000, CRC(15971bf6) SHA1(815152ab05edb1789a26898cfd66b5a7c4a1f765) )
ROM_END

/*
Title   SHAKATTO TAMBOURINE 2K1AUT
Media ID        DED1
Media Config    GD-ROM1/1
Regions J
Peripheral String       0000000
Product Number  GDS-0016
Version V1.000
Release Date    20011017
Manufacturer ID
TOC     DISC
Track   Start Sector    End Sector      Track Size
track01.bin     150     626     1121904
track02.raw     777     2909    5016816
track03.bin     45150   549299  1185760800

*/

ROM_START( shaktamb )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gds-0016", 0, SHA1(59bac62db4a536c63fec3464c665e00633fcf1a3) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	ROM_LOAD( "317-0321-com.pic", 0x000000, 0x004000, CRC(81519e71) SHA1(a30d25f81c77384ed26faa67c942802f2f3d7817) )
ROM_END

ROM_START( sprtjam )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gds-0003", 0, SHA1(caaba214c1faca78b3370bcd4190eb2853d7f825) )

	//PIC16C622A (317-0300-COM)
	//(sticker 253-5508-0300)
	//ROM_LOAD("317-0300-com.data", 0x00, 0x50, CRC(9a08413f) SHA1(d57649dcc3af578d55a93dd7a3f41da62d580f54) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	ROM_LOAD("317-0300-com.pic", 0x00, 0x4000, CRC(19a97214) SHA1(bcee1af2c16daabc7a0f723e1f9281a7c95600c6) )
ROM_END


ROM_START( slashout )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gds-0004", 0, SHA1(3cce788393ed194ba9b603f9896ff893691d6b00) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C622A (317-0302-COM)
	//(sticker 253-5508-0302)
	ROM_LOAD("317-0302-com.pic", 0x00, 0x4000, CRC(fa290329) SHA1(76c7266a124b23eaa5747f870cd2cfe881dd23af) )
ROM_END


ROM_START( spkrbtl )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gds-0005", 0, SHA1(d1c3fb2350e4a89372373e7f629c42b741af29b3) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C622A (317-0303-COM)
	//(sticker 253-5508-0303)
	ROM_LOAD("317-0303-com.pic", 0x00, 0x4000, CRC(b42999dd) SHA1(f285bdf34904517e119bd170b4ed0624eefac7bd) )
ROM_END

/*
Title   MONKEY_BALL
Media ID    43EB
Media Config    GD-ROM1/1
Regions J
Peripheral String   0000000
Product Number  GDS-0008
Version V1.008
Release Date    20010425
Manufacturer ID
Track   Start Sector    End Sector  Track Size
track01.bin 150 449 705600
track02.raw 600 2732    5016816
track03.bin 45150   549299  1185760800


PIC

253-5508-0307
317-0307-COM
*/

ROM_START( monkeyba )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gds-0008", 0, SHA1(2fadcd141bdbde77b2b335b270959a516af44d99) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	ROM_LOAD("317-0307-com.pic", 0x00, 0x4000, CRC(4046de19) SHA1(8adda9f223e926148b36744bbbaa89557544a229) )
ROM_END

/*
This is the I/O board used for Dynamic Golf which is
located under the panel.
It must be connected to the normal I/O board with a USB cable.

PCB Layout
----------

837-13938
|--------------------|
|CN2      CN1        |
|                    |
|      |-----|       |
|      | IC2 |       |
| CN3  |     |       |
|      |-----|    IC3|
|LED    CN4     IC4  |
|--------------------|
Notes:
      CN1 - 24 pin connector. not used
      CN2 - 4 pin connector used for 5 volt power input
      CN3 - USB connector type B
      CN4 - 16 pin connector used for buttons and trackball
      IC1 - HC240 logic IC (SOIC20)
      IC2 - Sega 315-6146 custom IC (QFP176)
      IC3 - 27C512 EPROM with label 'EPR-22084' (DIP28)
      IC4 - HC4020 logic IC (SOIC16)
*/

ROM_START( dygolf )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x10000, "io_board", 0)
	ROM_LOAD("epr-22084.ic3", 0x0000, 0x10000, CRC(18cf58bb) SHA1(1494f8215231929e41bbe2a133658d01882fbb0f) )

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gds-0009a", 0, SHA1(d502155ddaf881c2c9505528004b9904aa32a59c) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C622A (317-0308-COM)
	//(sticker 253-5508-0308)
	ROM_LOAD("317-0308-com.pic", 0x00, 0x4000, CRC(5e1ef2c4) SHA1(57fa3efbb24f8b54e62fe0a2133d863cc7638f53) )
ROM_END


ROM_START( wsbbgd )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gds-0010", 0,  SHA1(c3135ede3a8bdadab91aed49abacbfbde8037069) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C622A (317-0309-COM)
	//(sticker 253-5508-0309)
	ROM_LOAD("317-0309-com.pic", 0x00, 0x4000,  CRC(62d760bf) SHA1(9cf247a63250ce1770ec18e76e1637b2e4e442d9) )
ROM_END


ROM_START( vtennisg )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gds-0011", 0,  SHA1(b778403d73c8cdd13383691c9be2094ddfc1ba84) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C621A (317-0312-COM)
	//(sticker 253-5508-0312)
	ROM_LOAD("317-0312-com.pic", 0x00, 0x4000, CRC(7213684e) SHA1(0b1adb2f6b7576534096832752cf7606a52c166e) )
ROM_END

ROM_START( keyboard )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x80, "mie_eeprom", 0 )
	ROM_LOAD("keyboard-default-eeprom.bin", 0, 0x80, CRC(9262fc90) SHA1(6080c029967932cace361d673bf04c805276a7de))

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gds-0017", 0, SHA1(fb86eff3ef38de7fd78cfde897d5332d2092c172) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	ROM_LOAD("317-0323-com.pic", 0x00, 0x4000, CRC(c8854ef2) SHA1(b43b956df142fe4167dcc2ec805921e25bba180f) )

ROM_END


ROM_START( vathlete )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gds-0019", 0, SHA1(955d3c0cb991be3057138c562cff69c5ef887787) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C622A (317-0330-COM)
	//(sticker 253-5508-0330)
	ROM_LOAD("317-0330-com.pic", 0x00, 0x4000, CRC(33ccf2d1) SHA1(669d459fcbb327d2fcf34777d7a731979477fb02) )
ROM_END

/*

Title   VIRTUA TENNIS 2 (POWER SMASH 2)
Media ID    D72C
Media Config    GD-ROM1/1
Regions J
Peripheral String   0000000
Product Number  GDS-0015A
Version V2.000
Release Date    20010827
Manufacturer ID
TOC DISC
Track   Start Sector    End Sector  Track Size
track01.bin 150 449 705600
track02.raw 600 2732    5016816
track03.bin 45150   549299  1185760800


PIC

253-5508-0318
317-0318-EXP
*/

ROM_START( vtennis2 )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gds-0015a", 0, SHA1(c6e9c9901bd4f075454b7f18baf08df81bc2f1ad) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	ROM_LOAD("317-0318-exp.pic", 0x00, 0x4000, CRC(83de4047) SHA1(1808ac0d8353b92296de37f98b490a42a0e141cf) )

ROM_END


ROM_START( lupinsho )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gds-0018", 0, SHA1(0633a99a666f363ab30450a76b9753685d6b1f57) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	ROM_LOAD("317-0332-j.pic", 0x00, 0x4000, CRC(f71cb2fc) SHA1(281b3b3b03edf9a39e380976de528b7c9674de53) )
ROM_END


ROM_START( luptype )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gds-0021a", 0,  SHA1(15c6f9434494a31693cbb8e33da36e0e8a8f7c62) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C622A (317-0332-JPN)
	//(sticker 253-5508-0332J)
	ROM_LOAD("317-0332-jpn.pic", 0x00, 0x4000, CRC(43e78ecf) SHA1(bbe4b036e965fbba6ab79c88cba4ea8f0ea3f9fc) )

ROM_END

ROM_START( initdv2j )
	NAOMI2_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gds-0026b", 0,  SHA1(8ddc3ccd32ab3416fbe2921f6d6617d6c9f23203) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C622A (317-0345-JPN)
	//(sticker 253-5508-0345J)
	ROM_LOAD( "317-0345-jpn.pic", 0x000000, 0x004000, CRC(56e1274a) SHA1(735a6071226f3297de64bc0a38be741e87f5d023) )
ROM_END

ROM_START( initdv2jo )
	NAOMI2_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gds-0026", 0, SHA1(253acede106b7fbf49e24458e7fda868720e9549) )

	ROM_REGION( 0x50, "pic", ROMREGION_ERASE)
	ROM_LOAD("gds-0026.rom", 0x00, 0x50, NO_DUMP) // file on GD-ROM is BFK.BIN , _NOT_ BEM.BIN which is for Initial D : Arcade Stage (Japan)
ROM_END


ROM_START( initdv2e )
	NAOMI2_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gds-0027", 0,  SHA1(44746f0ceb1a3bbcd1db11a35c78c93a030f02de) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C622A (317-0357-EXP)
	ROM_LOAD( "317-0357-exp.pic", 0x000000, 0x004000, CRC(38f84b4d) SHA1(03c12d8580da1a4b3a554e62fd8b1f3447b7ebbd) )
ROM_END

ROM_START( initdv3j )
	NAOMI2_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gds-0032c", 0,  SHA1(005b93f9785c76a778f90f04d74d4b3cc1785a01) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C622A (317-????-JPN)
	ROM_LOAD( "317-xxxx-jpn.pic", 0x000000, 0x004000, NO_DUMP )	// missing PIC
ROM_END

ROM_START( initdv3jb )
	NAOMI2_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gds-0032b", 0, SHA1(568411aa72ca308a03a6b5b61c79833464b88bc6) )

	ROM_REGION( 0x50, "pic", ROMREGION_ERASE)
	ROM_LOAD("gds-0032_pic", 0x00, 0x50, NO_DUMP ) // PIC was missing
ROM_END

/*
Title   THE_MAZE_OF_THE_KINGS
Media ID    E3D0
Media Config    GD-ROM1/1
Regions J
Peripheral String   0000000
Product Number  GDS-0022
Version V1.001
Release Date    20020306
Manufacturer ID
TOC DISC
Track   Start Sector    End Sector  Track Size
track01.bin 150 449 705600
track02.raw 600 2732    5016816
track03.bin 45150   549299  1185760800


PIC
317-0333-COM
253-5508-0333

*/
ROM_START( mok )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gds-0022", 0, SHA1(70b41745225006e7876176cbd239edecd4c3f8b6) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	ROM_LOAD("317-0333-com.pic", 0x00, 0x4000, CRC(15fb7792) SHA1(03932ba9b1738d5ab75b2a465cc3254e75f59f63) )

ROM_END


ROM_START( ngdup23a )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gds-0023a", 0, SHA1(cd9d808b59eb8f40673ec4353d476f2b9c7f783c) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF) // uses the vf4 pic
	//PIC16C622A (317-0314-COM)
	//(sticker 253-5508-0314)
	ROM_LOAD("317-0314-com.pic", 0x00, 0x4000,  CRC(fa0b6c70) SHA1(c29936cb18e1dd592563b1104281f031e3b12fc2) )

ROM_END

ROM_START( ngdup23c )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gds-0023c", 0, SHA1(1fcb5530748886f4c4f45487d047859182ff7496))

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF) // uses the vf4 evolution pic
	//PIC16C622A (317-0338-JPN)
	//(sticker 253-5508-0338J)
	ROM_LOAD("317-0338-jpn.pic", 0x00, 0x4000, CRC(b177ba7d) SHA1(f751ec43a8e944a01eeda58c01b7bc73e5df749d) )
ROM_END


ROM_START( puyofev )
	NAOMIGD_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0x80, "mie_eeprom", 0 )
	ROM_LOAD("puyofev-default-eeprom.bin", 0, 0x80, CRC(42e5fd40) SHA1(e805bca22ae192e26965ba00534e6b87a3df238f))

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gds-0031", 0, SHA1(da2d421da9472b149619b6931bb2fe624be75fa2) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C621A (317-0375-COM)
	//(sticker 253-5508-0375)
	ROM_LOAD("317-0375-com.pic", 0x00, 0x4000, CRC(52b56b52) SHA1(221590efbb09824621714cb163bda51a921d7d54) )
ROM_END

/**********************************************
 *
 * Naomi 2 Cart defines
 *
 *********************************************/

ROM_START( vstrik3c )
	NAOMI2_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0xb000000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD( "epr-23663c.ic22",0x0000000, 0x0400000, CRC(7007fec7) SHA1(523168f0b218d0bd5c815d65bf0caba2c8468c9d) )
	ROM_LOAD( "mpr-23652.ic1",  0x0800000, 0x1000000, CRC(992758a2) SHA1(5e2a25c520c1795128e5026fc00d355c24852ded) )
	ROM_LOAD( "mpr-23653.ic2",  0x1800000, 0x1000000, CRC(e210e932) SHA1(2f6f0a31c3e98b21f1ff3af1680e50b3535b130f) )
	ROM_LOAD( "mpr-23654.ic3",  0x2800000, 0x1000000, CRC(91335971) SHA1(fc7599b836fb7995dd7da940e64b08b3c09cb180) )
	ROM_LOAD( "mpr-23655.ic4",  0x3800000, 0x1000000, CRC(1afe03b2) SHA1(43446188cc4a939663212159ea24eeed50de27e2) )
	ROM_LOAD( "mpr-23656.ic5",  0x4800000, 0x1000000, CRC(5e5fca1c) SHA1(e29d6b7d24acb5e0210ad9ba6f7f6ebca7ea3bf5) )
	ROM_LOAD( "mpr-23657.ic6",  0x5800000, 0x1000000, CRC(d97602bf) SHA1(1e79daa7acc787f3f6e55b8d92e4489db8861794) )
	ROM_LOAD( "mpr-23658.ic7",  0x6800000, 0x1000000, CRC(c912eacb) SHA1(715401264657a770eaa6165c7db6d588a493f745) )
	ROM_LOAD( "mpr-23659.ic8",  0x7800000, 0x1000000, CRC(db87ff9a) SHA1(9759b0885fa9d443f62129e062f631bcf46846d2) )
	ROM_LOAD( "mpr-23660.ic9",  0x8800000, 0x1000000, CRC(e49e65f5) SHA1(a46cea1c482211048aef375de8324273f6b06f27) )
	ROM_LOAD( "mpr-23661.ic10", 0x9800000, 0x1000000, CRC(7d44dc74) SHA1(cfd6253eab3c1a039629b4873946c9dbc7ed6872) )
	ROM_LOAD( "mpr-23662.ic11", 0xa800000, 0x0800000, CRC(d6ef7d68) SHA1(4ee396af6c5caf4c5af6e9ad0e03a7ac2c5039f4) )
ROM_END

ROM_START( vstrik3cb )
	NAOMI2_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0xb000000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD( "epr-23663b.ic22",0x0000000, 0x0400000, CRC(15733e44) SHA1(5040c518279283b76da6d9f75bb0a48953146ca9) )
	ROM_LOAD( "mpr-23652.ic1",  0x0800000, 0x1000000, CRC(992758a2) SHA1(5e2a25c520c1795128e5026fc00d355c24852ded) )
	ROM_LOAD( "mpr-23653.ic2",  0x1800000, 0x1000000, CRC(e210e932) SHA1(2f6f0a31c3e98b21f1ff3af1680e50b3535b130f) )
	ROM_LOAD( "mpr-23654.ic3",  0x2800000, 0x1000000, CRC(91335971) SHA1(fc7599b836fb7995dd7da940e64b08b3c09cb180) )
	ROM_LOAD( "mpr-23655.ic4",  0x3800000, 0x1000000, CRC(1afe03b2) SHA1(43446188cc4a939663212159ea24eeed50de27e2) )
	ROM_LOAD( "mpr-23656.ic5",  0x4800000, 0x1000000, CRC(5e5fca1c) SHA1(e29d6b7d24acb5e0210ad9ba6f7f6ebca7ea3bf5) )
	ROM_LOAD( "mpr-23657.ic6",  0x5800000, 0x1000000, CRC(d97602bf) SHA1(1e79daa7acc787f3f6e55b8d92e4489db8861794) )
	ROM_LOAD( "mpr-23658.ic7",  0x6800000, 0x1000000, CRC(c912eacb) SHA1(715401264657a770eaa6165c7db6d588a493f745) )
	ROM_LOAD( "mpr-23659.ic8",  0x7800000, 0x1000000, CRC(db87ff9a) SHA1(9759b0885fa9d443f62129e062f631bcf46846d2) )
	ROM_LOAD( "mpr-23660.ic9",  0x8800000, 0x1000000, CRC(e49e65f5) SHA1(a46cea1c482211048aef375de8324273f6b06f27) )
	ROM_LOAD( "mpr-23661.ic10", 0x9800000, 0x1000000, CRC(7d44dc74) SHA1(cfd6253eab3c1a039629b4873946c9dbc7ed6872) )
	ROM_LOAD( "mpr-23662.ic11", 0xa800000, 0x0800000, CRC(d6ef7d68) SHA1(4ee396af6c5caf4c5af6e9ad0e03a7ac2c5039f4) )
ROM_END

ROM_START( wldrider )
	NAOMI2_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0xa800000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD( "epr-23622.ic22", 0x0000000, 0x0400000, CRC(8acafa5b) SHA1(c92bcd40bad6ba8efd1edbfd7e439fb2b3c67fb0) )
	ROM_LOAD( "mpr-23611.ic1",  0x0800000, 0x1000000, CRC(943bc32a) SHA1(11ced99e9dbd7cc93031779e00d4ee1f2dff9086) )
	ROM_LOAD( "mpr-23612.ic2",  0x1800000, 0x1000000, CRC(f71d87e5) SHA1(c36d1d07702642db282278f1cf556ed472e930d3) )
	ROM_LOAD( "mpr-23613.ic3",  0x2800000, 0x1000000, CRC(689e783e) SHA1(1264c3389610bbf2745b7e6d50f327b1df33b63b) )
	ROM_LOAD( "mpr-23614.ic4",  0x3800000, 0x1000000, CRC(e5b8c5e5) SHA1(4b81ecc6375bf731dc3423cf87c2228eb304f2b5) )
	ROM_LOAD( "mpr-23615.ic5",  0x4800000, 0x1000000, CRC(95c35866) SHA1(a3dd4cd2c8818a3c5de7aa6c4afc74f872d52b37) )
	ROM_LOAD( "mpr-23616.ic6",  0x5800000, 0x1000000, CRC(6288848f) SHA1(a69ddde96d15cee9154ed3e87514286db6bbb622) )
	ROM_LOAD( "mpr-23617.ic7",  0x6800000, 0x1000000, CRC(19298892) SHA1(78493587dc49c7c99c5a98f152b17500003316b3) )
	ROM_LOAD( "mpr-23618.ic8",  0x7800000, 0x1000000, CRC(67d7b659) SHA1(44e75e16f2740c6e147e101a8714be8c5d9b71f3) )
	ROM_LOAD( "mpr-23619.ic9",  0x8800000, 0x1000000, CRC(a5f4f6af) SHA1(bb89d3f0f5bbaf7c40fa43680c7e51ef93f7ed26) )
	ROM_LOAD( "mpr-23620.ic10", 0x9800000, 0x1000000, CRC(67aa15a9) SHA1(42c24cbf7069c27430a71509a872cd1c4224aaeb) )
ROM_END

ROM_START( vf4cart )
	NAOMI2_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0xb800000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD( "epr-23785.ic22", 0x0000000, 0x0400000, CRC(9bd98d4b) SHA1(3b0622625317cd6b2736c5b4a23484fb8bf39e4b) )
	ROM_LOAD( "mpr-23774.ic1",  0x0800000, 0x1000000, CRC(0fe7b864) SHA1(357ca3a5d96d7ff27e06367e115ddfd00cb260e3) )
	ROM_LOAD( "mpr-23775.ic2",  0x1800000, 0x1000000, CRC(a11cd9e5) SHA1(7fd8f634d0d14a91dfe9f39b5643b9c761dc7053) )
	ROM_LOAD( "mpr-23776.ic3",  0x2800000, 0x1000000, CRC(44b8429e) SHA1(0ec6b6156bef1621700791651903a4589f4b5f84) )
	ROM_LOAD( "mpr-23777.ic4",  0x3800000, 0x1000000, CRC(78a4264e) SHA1(40d045240173f330ac6f108b132f9a87884922be) )
	ROM_LOAD( "mpr-23778.ic5",  0x4800000, 0x1000000, CRC(02dee78b) SHA1(e08ce0fc0b1db2dcef957c2edb6d51db400a38cb) )
	ROM_LOAD( "mpr-23779.ic6",  0x5800000, 0x1000000, CRC(6e458eea) SHA1(4c85fcacf4ff46d4a137afcf5906092fd88fe4b1) )
	ROM_LOAD( "mpr-23780.ic7",  0x6800000, 0x1000000, CRC(a775a51c) SHA1(b3eae20e5e7d74252368fd902c4e94a6ba6cb154) )
	ROM_LOAD( "mpr-23781.ic8",  0x7800000, 0x1000000, CRC(401bca00) SHA1(10c6011fae7076ea0dc5ab0ebca9cb88659a93e9) )
	ROM_LOAD( "mpr-23782.ic9",  0x8800000, 0x1000000, CRC(4f72e901) SHA1(a1d231c446d2c34e5a7e7145754b2313a2d03fd4) )
	ROM_LOAD( "mpr-23783.ic10", 0x9800000, 0x1000000, CRC(c8d4f6f9) SHA1(9e9df605c050b3780d7df34bd5041d30bc084d2d) )
	ROM_LOAD( "mpr-23784.ic11", 0xa800000, 0x1000000, CRC(f74f2fee) SHA1(84b07baa6d116727e66ef27e24ba6484c3393891) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "vf4cart-key.bin", 0, 4, CRC(22838e16) SHA1(afee674dce3cfdd20360c30da2da5ba69f8d4682) )
ROM_END

ROM_START( kingrt66 )
	NAOMI2_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0xa800000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD( "epr-23819a.ic22", 0x0000000, 0x400000, CRC(92f11b29) SHA1(b33f7eefb849754cfe194be1d48d770ed77ff69a) )
	ROM_RELOAD( 0x400000, 0x400000)
	ROM_LOAD( "mpr-23799.ic1",  0x0800000, 0x800000, CRC(03da8f81) SHA1(01f9b33ffc485d8ccb2630c8cc1fcc0d90da171d) )
	ROM_LOAD( "mpr-23800.ic2",  0x1000000, 0x800000, CRC(1bbbf549) SHA1(0c5269f3e8a88298408bb612dc8da4d545ce329e) )
	ROM_LOAD( "mpr-23801.ic3",  0x1800000, 0x800000, CRC(6c81d0a9) SHA1(2301350098062e16afd6d4c13eeb1e46040d398f) )
	ROM_LOAD( "mpr-23802.ic4",  0x2000000, 0x800000, CRC(fe245803) SHA1(3b5c4d20c447ca02306a74914c027c9193d2bb48) )
	ROM_LOAD( "mpr-23803.ic5",  0x2800000, 0x800000, CRC(d8376c76) SHA1(62a64d0ff4b63a2de3b8668c0ca1d69d4cdac00e) )
	ROM_LOAD( "mpr-23804.ic6",  0x3000000, 0x800000, CRC(d184a4b3) SHA1(db00cf005da52b8add695ec89f8a81cf21fc9bad) )
	ROM_LOAD( "mpr-23805.ic7",  0x3800000, 0x800000, CRC(6e92fb39) SHA1(d992272e474424221c87bb71e269e10cbba1cac9) )
	ROM_LOAD( "mpr-23806.ic8",  0x4000000, 0x800000, CRC(fd4c128f) SHA1(7778920f59b8bc1e9718d685bfd556f73e5c3895) )
	ROM_LOAD( "mpr-23807.ic9",  0x4800000, 0x800000, CRC(5bcb3759) SHA1(57baa595eaee826ffadc17c885cb73174558feb1) )
	ROM_LOAD( "mpr-23808.ic10", 0x5000000, 0x800000, CRC(7d66c409) SHA1(fa8d08587ebcd70ec39237366c5de6948efce881) )
	ROM_LOAD( "mpr-23809.ic11", 0x5800000, 0x800000, CRC(322abecc) SHA1(476a751dc9849a5167e7e7e17e27987b96b1cb00) )
	ROM_LOAD( "mpr-23810.ic12", 0x6000000, 0x800000, CRC(a47fff36) SHA1(5c0088e140654b89749c5dd5d729c4799afc224e) )
	ROM_LOAD( "mpr-23811.ic13", 0x6800000, 0x800000, CRC(f502b666) SHA1(fa483f327cab15d10861efcbf6bdf4a2c5d0a763) )
	ROM_LOAD( "mpr-23812.ic14", 0x7000000, 0x800000, CRC(ed664a57) SHA1(54421ce642208fd1f14fbbe2e326d9c8da7c9944) )
	ROM_LOAD( "mpr-23813.ic15", 0x7800000, 0x800000, CRC(3f905393) SHA1(d63bbe8099b323ad4fd95401e0097b4c7151b44c) )
	ROM_LOAD( "mpr-23814.ic16", 0x8000000, 0x800000, CRC(80756e62) SHA1(9b2f14ed10775ee56f245c75ee23534c99db23d4) )
	ROM_LOAD( "mpr-23815.ic17", 0x8800000, 0x800000, CRC(7404e74e) SHA1(a427bf9f5e8338d34d1091de69460e374ddc6ee6) )
	ROM_LOAD( "mpr-23816.ic18", 0x9000000, 0x800000, CRC(58a7fbac) SHA1(7132e018e73e68a355f92fe26121da7c3c0c927a) )
	ROM_LOAD( "mpr-23817.ic19", 0x9800000, 0x800000, CRC(fd6fb2cf) SHA1(b2bf8a11d42141237db0254506ee73d9cef8b9d1) )
	ROM_LOAD( "mpr-23818.ic20", 0xa000000, 0x800000, CRC(be902c2b) SHA1(1f1aa6933da528bdcef45942a0caa33cb24e7ddd) )
ROM_END

/*
    Tests IC1 like gram2000 if actel_id is 0xffff, see ROM_COPY
*/
ROM_START( vf4evoct )
	NAOMI2_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0xb000000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD( "epr-23934.ic11",  0x0000000, 0x400000, CRC(656a7d84) SHA1(e407ddc923b399de99cb06a8831ef8fb328cfe64) )
	ROM_LOAD32_WORD( "mpr-23912.ic17s", 0x1000000, 0x800000, CRC(b2b13d97) SHA1(978bcdab7e4f220b464d5b9fa27870b5176283ca) )
	ROM_LOAD32_WORD( "mpr-23913.ic18",  0x1000002, 0x800000, CRC(560de9d2) SHA1(e065abb62346f5e50f654ac77beb54ee1bfc1a08) )
	ROM_LOAD32_WORD( "mpr-23914.ic19s", 0x2000000, 0x800000, CRC(a2104728) SHA1(48b752b6266b4d44fcb16fe179fb44ca58e11db0) )
	ROM_LOAD32_WORD( "mpr-23915.ic20",  0x2000002, 0x800000, CRC(295d32f0) SHA1(4cabb1ab54dad2cef7207f8d7a78f609d35800fb) )
	ROM_LOAD32_WORD( "mpr-23916.ic21s", 0x3000000, 0x800000, CRC(d725fdd3) SHA1(ab3a79dfcbaa65b5a085a429bcb76c1d3940d590) )
	ROM_LOAD32_WORD( "mpr-23917.ic22",  0x3000002, 0x800000, CRC(8794c8e8) SHA1(a2542ad8c063f77749fbc4429ec74bba26939556) )
	ROM_LOAD32_WORD( "mpr-23918.ic23s", 0x4000000, 0x800000, CRC(7a3da170) SHA1(72d05430cd8cd12c47ccd36bf171eaadfb987708) )
	ROM_LOAD32_WORD( "mpr-23919.ic24",  0x4000002, 0x800000, CRC(59601746) SHA1(514b00ee44d64593c2d257b48ac20b42ee2a402a) )
	ROM_LOAD32_WORD( "mpr-23920.ic25s", 0x5000000, 0x800000, CRC(6ae07021) SHA1(b018db4f868d9ebcde44759d8aea44d789958087) )
	ROM_LOAD32_WORD( "mpr-23921.ic26",  0x5000002, 0x800000, CRC(42028253) SHA1(aa5a3953e4306f4f69c1be568d515af249e6ab1e) )
	ROM_LOAD32_WORD( "mpr-23922.ic27s", 0x6000000, 0x800000, CRC(baf47df2) SHA1(d5c04e03a69a4ac8c8c066c0c750bef236e8c172) )
	ROM_LOAD32_WORD( "mpr-23923.ic28",  0x6000002, 0x800000, CRC(5c31b7e7) SHA1(cf1307400f7bd4b8dda840544a348eee34710256) )
	ROM_LOAD32_WORD( "mpr-23924.ic29",  0x7000000, 0x800000, CRC(6dfe19d9) SHA1(99b13ef752c7b8f7812ea5c3bce19cbf122008b6) )
	ROM_LOAD32_WORD( "mpr-23925.ic30s", 0x7000002, 0x800000, CRC(e9ec870a) SHA1(cdf385096e7fe15de98cea6ae5aabf3016e007a9) )
	ROM_LOAD32_WORD( "mpr-23926.ic31",  0x8000000, 0x800000, CRC(2c650728) SHA1(328748b2c786eb77c9d05280e88367cfb7b79777) )
	ROM_LOAD32_WORD( "mpr-23927.ic32s", 0x8000002, 0x800000, CRC(f36a765b) SHA1(ecd09db0961b0d11000c8744a2cfcf3fa473eea6) )
	ROM_LOAD32_WORD( "mpr-23928.ic33",  0x9000000, 0x800000, CRC(0ee92b02) SHA1(d0d26f4257a022b42b59cf4f9305a3cb2dc67f4b) )
	ROM_LOAD32_WORD( "mpr-23929.ic34s", 0x9000002, 0x800000, CRC(d768f242) SHA1(3ade29b094308d870ecac53cfe77b843d50af85a) )
	ROM_LOAD32_WORD( "mpr-23930.ic35",  0xa000000, 0x800000, CRC(0e45e4c4) SHA1(974b83d4cc35e8ac9f83d04ebd395f1e2196e829) )
	ROM_LOAD32_WORD( "mpr-23931.ic36s", 0xa000002, 0x800000, CRC(12ecd2f0) SHA1(3222d4d9d3e30c297a072a8888c28503306db40c) )

	ROM_COPY( "rom_board", 0x1400000, 0x0800000, 0x0800000 ) // mirror data so IC1 check can pass

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "vf4evoct-key.bin", 0, 4, CRC(c37dae2b) SHA1(c42dc01b5918db36e7eb2aca523e2f4df567725a) )
ROM_END

ROM_START( clubkrte )
	NAOMI2_BIOS
	NAOMI_DEFAULT_EEPROM

	ROM_REGION( 0xb800000, "rom_board", ROMREGION_ERASEFF)
	ROM_LOAD( "epr-23704.ic22", 0x0000000, 0x0400000, CRC(ff700a0d) SHA1(e2db0d2bd7dc88b3a487077e8ce56eb6cfd9b02d) )
	ROM_LOAD( "mpr-23693.ic1",  0x0800000, 0x1000000, CRC(28995764) SHA1(a1457f9935dde2e5aaa5ef245c736c0f2f8c74b7) )
	ROM_LOAD( "mpr-22694.ic2",  0x1800000, 0x1000000, CRC(37d30111) SHA1(4c07df8cd548cac79d48709e61f692d762471f8f) )
	ROM_LOAD( "mpr-22695.ic3",  0x2800000, 0x1000000, CRC(41ac1510) SHA1(01b889b627fdfc1f12a0c84fcc36debdfb1cf377) )
	ROM_LOAD( "mpr-22696.ic4",  0x3800000, 0x1000000, CRC(6f2da455) SHA1(b655757bc513398820bfeae07dca8a4f3ea9752c) )
	ROM_LOAD( "mpr-22697.ic5",  0x4800000, 0x1000000, CRC(1383c742) SHA1(6efd17632a277a4bb0e47cc912fbc9865a8b14c3) )
	ROM_LOAD( "mpr-22698.ic6",  0x5800000, 0x1000000, CRC(da79cd06) SHA1(fdfe068caca1eb764dec28ab327e56b39144f3ae) )
	ROM_LOAD( "mpr-22699.ic7",  0x6800000, 0x1000000, CRC(ea77f000) SHA1(35aa8ee804d9429e72f516137a3b06c585a57b6d) )
	ROM_LOAD( "mpr-22700.ic8",  0x7800000, 0x1000000, CRC(db9e5c1d) SHA1(db918c0fa1860f4345806e574d44354aba5fcd54) )
	ROM_LOAD( "mpr-22701.ic9",  0x8800000, 0x1000000, CRC(0fa92fd7) SHA1(67a1cf085101884a17a4783d0d509ab198aa6425) )
	ROM_LOAD( "mpr-22702.ic10", 0x9800000, 0x1000000, CRC(e302b582) SHA1(787192ed9f9a08541eecc3128855485cad802a42) )
	ROM_LOAD( "mpr-22703.ic11", 0xa800000, 0x1000000, CRC(702b8b4a) SHA1(3a8dfde458f341e7db20664382b9fce2b6e5d462) )
ROM_END

/**********************************************
 *
 * Naomi 2 GD-ROM defines
 *
 *********************************************/

ROM_START( vstrik3 )
	NAOMI2_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gds-0006", 0, SHA1(aca09a88506f5e462ad3fb33eac5478a2a010609) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C622A (317-0304-COM)
	//(sticker 253-5508-0304)
	ROM_LOAD("317-0304-com.bin", 0x00, 0x4000, CRC(8e82d17a) SHA1(141a4d492b13bbb222dfbe7a1ad296b548d12a3b) )
ROM_END

ROM_START( vf4 )
	NAOMI2_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gds-0012", 0, SHA1(c34588f59c6091cd1c3ef235171dad8d5247e707) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C622A (317-0314-COM)
	//(sticker 253-5508-0314)
	ROM_LOAD("317-0314-com.pic", 0x00, 0x4000, CRC(fa0b6c70) SHA1(c29936cb18e1dd592563b1104281f031e3b12fc2) )

ROM_END

ROM_START( vf4b )
	NAOMI2_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gds-0012b", 0, SHA1(9b8e05c3d28a09323b13c198dfcc2b771bba67cd) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C622A (317-0314-COM)
	//(sticker 253-5508-0314)
	ROM_LOAD("317-0314-com.pic", 0x00, 0x4000, CRC(fa0b6c70) SHA1(c29936cb18e1dd592563b1104281f031e3b12fc2) )

ROM_END

ROM_START( vf4c )
	NAOMI2_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gds-0012c", 0, SHA1(0ec149d7edfb326777cdc45a2ac8ad578a32aba1) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C622A (317-0314-COM)
	//(sticker 253-5508-0314)
	ROM_LOAD("317-0314-com.pic", 0x00, 0x4000, CRC(fa0b6c70) SHA1(c29936cb18e1dd592563b1104281f031e3b12fc2) )

ROM_END


ROM_START( vf4evo )
	NAOMI2_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gds-0024b", 0, SHA1(a829169542f3bed76095ad6bfbbde7d494d04d72) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C622A (317-0338-JPN)
	//(sticker 253-5508-0338J)
	ROM_LOAD("317-0338-jpn.pic", 0x00, 0x4000, CRC(b177ba7d) SHA1(f751ec43a8e944a01eeda58c01b7bc73e5df749d) )

ROM_END

ROM_START( vf4evoa )
	NAOMI2_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gds-0024a", 0, SHA1(6225e778d73db18be26f882d4f9cd3b3a136d1c9) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C622A (317-0338-JPN)
	//(sticker 253-5508-0338J)
	ROM_LOAD("317-0338-jpn.pic", 0x00, 0x4000, CRC(b177ba7d) SHA1(f751ec43a8e944a01eeda58c01b7bc73e5df749d) )
ROM_END

ROM_START( vf4tuned ) // are there multiple files on this GD-ROM? it only compresses to 500 meg when the rom file is closer to half tha
	NAOMI2_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gds-0036f", 0, SHA1(ea35d6ecdf94e5c9a545952758da80f658755df0) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C622A (317-0387-COM)
	//(sticker 253-5508-0387)
	ROM_LOAD("317-0387-com.pic", 0x00, 0x4000, CRC(8728aeaa) SHA1(07983ab41d143f845c3150dfc9b7301968708e18) )
ROM_END

ROM_START( vf4tunedd )
	NAOMI2_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gds-0036d", 0, SHA1(2f7654307a4c978c5af6c8238c44e70275dd34f9) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C622A (317-0387-COM)
	//(sticker 253-5508-0387)
	ROM_LOAD("317-0387-com.pic", 0x00, 0x4000, CRC(8728aeaa) SHA1(07983ab41d143f845c3150dfc9b7301968708e18) )

ROM_END


ROM_START( vf4tuneda )
	NAOMI2_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gds-0036a", 0, SHA1(cd630fc4e8f7ed5641b85c609584d7efe0eac137) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C622A (317-0387-COM)
	//(sticker 253-5508-0387)
	ROM_LOAD("317-0387-com.pic", 0x00, 0x4000, CRC(8728aeaa) SHA1(07983ab41d143f845c3150dfc9b7301968708e18) )
ROM_END


/*

Title   BEACH SPIKERS
Media ID    0897
Media Config    GD-ROM1/1
Regions J
Peripheral String   0000000
Product Number  GDS-0014
Version V1.001
Release Date    20010613
Manufacturer ID
TOC DISC
Track   Start Sector    End Sector  Track Size
track01.bin 150 449 705600
track02.raw 600 2746    5049744
track03.bin 45150   549299  1185760800

PIC

253-5508-0317
317-0317-COM

*/

ROM_START( beachspi )
	NAOMI2_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gds-0014", 0, SHA1(1ebb3695196c11a86276e034df2e1c8d7fa6b96f) )

	//PIC16C622A (317-0317-COM)
	//(sticker 253-5508-0317)

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	ROM_LOAD("317-0317-com.pic", 0x00, 0x4000, CRC(ef65fe73) SHA1(2c02d1570c1fdad56bc684c60bb17255c73c6d45) )

ROM_END

ROM_START( initd )
	NAOMI2_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gds-0020b", 0, SHA1(c0e901623ef4fcd97b7e4d29ae556e6f2e91b8ad) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	//PIC16C622A (317-0331-JPN)
	//(sticker 253-5508-0331J)
	ROM_LOAD("317-0331-jpn.pic", 0x00, 0x4000, CRC(0a3bf606) SHA1(7c0e22df4a43a440571ac55fd0a6575931e8f959) )
ROM_END

ROM_START( initdexp )
	NAOMI2_BIOS
	NAOMI_DEFAULT_EEPROM

	DISK_REGION( "gdrom" )
	DISK_IMAGE_READONLY( "gds-0025", 0, SHA1(8ea92cf6b493f21b9453832edad7cbc5e5b350c1) )

	ROM_REGION( 0x4000, "pic", ROMREGION_ERASEFF)
	ROM_LOAD("317-0343-com.pic", 0x00, 0x4000, CRC(80eea4eb) SHA1(5aedc0d52a2a8a2d186ca591094835d972574092) )
ROM_END



/**********************************************
 *
 * Atomiswave cart defines
 *
 *********************************************/

static DRIVER_INIT( atomiswave )
{
	UINT64 *ROM = (UINT64 *)machine.region("awflash")->base();

	// patch out long startup delay
	ROM[0x98e/8] = (ROM[0x98e/8] & U64(0xffffffffffff)) | (UINT64)0x0009<<48;

	awflash = machine.device<macronix_29l001mc_device>("awflash");
}

ROM_START( fotns )
	AW_BIOS

	ROM_REGION( 0x8000000, "rom_board", ROMREGION_ERASE)
	ROM_LOAD( "ax1901p01.ic18", 0x0000000, 0x0800000,  CRC(a06998b0) SHA1(d617691db5170f6db176e40fc732966d523fd8cf) )
	ROM_LOAD( "ax1901m01.ic11", 0x1000000, 0x1000000,  CRC(ff5a1642) SHA1(49cefcce173f9a811fe9c0c07bee53aeba2bc3a8) )
	ROM_LOAD( "ax1902m01.ic12", 0x2000000, 0x1000000,  CRC(d9aae8a9) SHA1(bf87034088be0847b6e297b7665e0ea4d8cba631) )
	ROM_LOAD( "ax1903m01.ic13", 0x3000000, 0x1000000,  CRC(1711b23d) SHA1(ab628b2ec678839c75245e245297818ef1592d3b) )
	ROM_LOAD( "ax1904m01.ic14", 0x4000000, 0x1000000,  CRC(443bfb26) SHA1(6f7751afa0ca55dd0679758b27bed92b31c1b050) )
	ROM_LOAD( "ax1905m01.ic15", 0x5000000, 0x1000000,  CRC(eb1cada0) SHA1(459d21d622c72606f1d3095e8a25b6c4adccf8ab) )
	ROM_LOAD( "ax1906m01.ic16", 0x6000000, 0x1000000,  CRC(fe6da168) SHA1(d4ab6443383469bb5a4337005de917627a2e21cc) )
	ROM_LOAD( "ax1907m01.ic17", 0x7000000, 0x1000000,  CRC(9d3a0520) SHA1(78583fd171b34439f77a04a97ebe3c9d1bab61cc) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "ax1901f01.bin", 0, 4, CRC(0283c08d) SHA1(5d62b6769ae7f1fc68bd3db028d782621aaa6f9c) )
ROM_END

ROM_START( rangrmsn )
	AW_BIOS

	ROM_REGION( 0x8000000, "rom_board", ROMREGION_ERASE)
	ROM_LOAD( "ax1601p01.ic18", 0x0000000, 0x0800000, CRC(00a74fbb) SHA1(57cc1eedd22d1f553956a825e69a597309ee2bef) )
	ROM_LOAD( "ax1601m01.ic11", 0x1000000, 0x1000000, CRC(f34eed33) SHA1(1c171fb8aa95877f81ed78652d4a9ff80f7713ff) )
	ROM_LOAD( "ax1602m01.ic12", 0x2000000, 0x1000000, CRC(a7d59efb) SHA1(a40938ce1399babefc8cf02f579a86cf08e211ef) )
	ROM_LOAD( "ax1603m01.ic13", 0x3000000, 0x1000000, CRC(7c0aa241) SHA1(3e0e5ff3307dcfa52998fb9b4b14bf54bd056a99) )
	ROM_LOAD( "ax1604m01.ic14", 0x4000000, 0x1000000, CRC(d2369144) SHA1(da1eae9957d27d1682c4191780cf51b32dfe6659) )
	ROM_LOAD( "ax1605m01.ic15", 0x5000000, 0x1000000, CRC(0c11c1f9) SHA1(0585db60618c5b97f9b7c203baf7e5ac90883ca6) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "ax1601f01.bin", 0, 4, CRC(278f1df7) SHA1(bf3e92e0b19dc1604b764382b859e73158d18025) )
ROM_END

ROM_START( sprtshot )
	AW_BIOS

	ROM_REGION( 0x8000000, "rom_board", ROMREGION_ERASE)
	ROM_LOAD( "ax0101p01.ic18", 0x0000000, 0x0800000, CRC(b3642b5d) SHA1(85eabd9551aefb825ae8eb6422092fb5a58d60f6) )
	ROM_LOAD( "ax0101m01.ic11", 0x1000000, 0x1000000, CRC(1e39184d) SHA1(663e0cb9f43a0f89d9841e04b3d009f6c5e88d5e) )
	ROM_LOAD( "ax0102m01.ic12", 0x2000000, 0x1000000, CRC(700764d1) SHA1(310f1606f7bbed1012c119f1ef5d89d231d8489e) )
	ROM_LOAD( "ax0103m01.ic13", 0x3000000, 0x1000000, CRC(6144e7a8) SHA1(4d4341082f008dfd93ef5bf32a44c80869ef02a8) )
	ROM_LOAD( "ax0104m01.ic14", 0x4000000, 0x1000000, CRC(ccb72150) SHA1(a1032d321c27f9ff43da41f20b8687bf1958ddc9) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "ax0101f01.bin", 0, 4, CRC(2144df1c) SHA1(9069ca78e7450a285173431b3e52c5c25299e473) )
ROM_END

ROM_START( xtrmhunt )
	AW_BIOS

	ROM_REGION( 0x8000000, "rom_board", ROMREGION_ERASE)
	ROM_LOAD( "ax2401p01.ic18", 0x0000000, 0x0800000,  CRC(8e2a11f5) SHA1(b5106314fb8d4483254e83ac3982039bb60a78e8) )
	ROM_LOAD( "ax2401m01.ic11", 0x1000000, 0x1000000,  CRC(76dbc286) SHA1(8f36ca94b8e67c76e0f90b21debc5ac7890f0da1) )
	ROM_LOAD( "ax2402m01.ic12", 0x2000000, 0x1000000,  CRC(cd590ea2) SHA1(ee5e38bf68e95da665be478ebba9cc5ffed52bb7) )
	ROM_LOAD( "ax2403m01.ic13", 0x3000000, 0x1000000,  CRC(06f62eb5) SHA1(f7e8d1dda6bb59ca2bc7cfa1105889b9e8e6d55d) )
	ROM_LOAD( "ax2404m01.ic14", 0x4000000, 0x1000000,  CRC(759ef5cb) SHA1(27ac2d12c6fb358b3d631c017c7b693e5ad95fd7) )
	ROM_LOAD( "ax2405m01.ic15", 0x5000000, 0x1000000,  CRC(940d77f1) SHA1(eefdfcb92873032dc7d9ff9310bf5ed715c8bf4f) )
	ROM_LOAD( "ax2406m01.ic16", 0x6000000, 0x1000000,  CRC(cbcf2c5d) SHA1(61362fabcbb3bfc01c996748a7ca65f8a0e02f2f) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "ax2401f01.bin", 0, 4, CRC(2f578ea4) SHA1(6775daa4b4081186905cc20f56df0f8ab147428b) )
ROM_END

ROM_START( xtrmhnt2 )
	AW_BIOS

	ROM_REGION( 0x8000000, "rom_board", ROMREGION_ERASE)
	ROM_LOAD( "610-0752.u3",  0x0000000, 0x1000000, CRC(bab6182e) SHA1(4d25256c81941316887cbb4524a203922f5b7104) )
	ROM_LOAD( "610-0752.u1",  0x1000000, 0x1000000, CRC(3086bc47) SHA1(eb7b04db90d296985528f0cfdd4545f184c40b64) )
	ROM_LOAD( "610-0752.u2",  0x2000000, 0x1000000, CRC(d3a88b31) SHA1(ccf14367e4e7efbc2cc835f3b001fd6d64302a5e) )
	ROM_LOAD( "610-0752.u4",  0x3000000, 0x1000000, CRC(9787f145) SHA1(8445ede0477f70fbdc113810b80356945ce498d2) )
	ROM_LOAD( "610-0752.u14", 0x4000000, 0x1000000, CRC(ce83bcc7) SHA1(e2d324a5a7eacbec7b0df9a4b9e276521bb9ab80) )
	ROM_LOAD( "610-0752.u15", 0x5000000, 0x1000000, CRC(864a6342) SHA1(fb97532d5dd00f8520fdaf68dfcd1ea627bdf90a) )
	ROM_LOAD( "610-0752.u16", 0x6000000, 0x1000000, CRC(8ac71c76) SHA1(080e41e633bf082fc536781541c6031d1ac81939) )
	ROM_LOAD( "610-0752.u17", 0x7000000, 0x1000000, CRC(a79fb1fa) SHA1(f75c5b574fd79677b926c595b369e95605a3c848) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "315-6248.bin", 0, 4, CRC(553dd361) SHA1(a60a26b5ee786cf0bb3d09bb6f00374598fbd7cc) )

	ROM_REGION( 0x1400000, "network", 0)	// network board
	ROM_LOAD( "fpr-24330a.ic2", 0x000000, 0x400000, CRC(8d89877e) SHA1(6caafc49114eb0358e217bc2d1a3ab58a93c8d19) )
	ROM_LOAD( "flash128.ic4s", 0x400000, 0x1000000, CRC(866ed675) SHA1(2c4c06935b7ab1876e640cede51713b841833567) )
ROM_END

ROM_START( anmlbskt )
	AW_BIOS

	ROM_REGION( 0x8000000, "rom_board", ROMREGION_ERASE)
	ROM_LOAD( "vm2001f01.u3",  0x0000000, 0x800000, CRC(4fb33380) SHA1(9070990515544e6e9a1d24b1e0597cdea926a4c9) )
	ROM_LOAD( "vm2001f01.u1",  0x1000000, 0x800000, CRC(3de23e27) SHA1(d3aae2a7e5c78fc3bf8e296392d8f893961d946f) )
	ROM_LOAD( "vm2001f01.u2",  0x2000000, 0x800000, CRC(386070a1) SHA1(bf46980ea822b4cfe67c622f0104bf793031f4ad) )
	ROM_LOAD( "vm2001f01.u4",  0x3000000, 0x800000, CRC(7cb2e7c3) SHA1(8b4e46cf19fbc1d613af75c52faebefb2776280b) )
	ROM_LOAD( "vm2001f01.u14", 0x4000000, 0x800000, CRC(3de23e27) SHA1(d3aae2a7e5c78fc3bf8e296392d8f893961d946f) )
	ROM_LOAD( "vm2001f01.u15", 0x5000000, 0x800000, CRC(2bb1be28) SHA1(fda7967d6c0341a608c52087ae3d461554760435) )
	ROM_LOAD( "vm2001f01.u16", 0x6000000, 0x800000, CRC(3de23e27) SHA1(d3aae2a7e5c78fc3bf8e296392d8f893961d946f) )
	ROM_LOAD( "vm2001f01.u17", 0x7000000, 0x800000, CRC(3de23e27) SHA1(d3aae2a7e5c78fc3bf8e296392d8f893961d946f) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "vm2001f01.bin", 0, 4, CRC(d8d6c32e) SHA1(255a437bdb4bb8372167f33f0ca1668bcd74ea32) )
ROM_END

ROM_START( dolphin )
	AW_BIOS

	ROM_REGION( 0x8000000, "rom_board", ROMREGION_ERASE)
	ROM_LOAD( "ax0401p01.ic18", 0x0000000, 0x0800000, CRC(195d6328) SHA1(cf3b5699f81235919dd3b1974d2ecb0376cb4552) )
	ROM_LOAD( "ax0401m01.ic11", 0x1000000, 0x1000000, CRC(5e5dca57) SHA1(e0623c84f66cada37d4c9399a7a8fc6866933144) )
	ROM_LOAD( "ax0402m01.ic12", 0x2000000, 0x1000000, CRC(77dd4771) SHA1(dcd23b8ddc82eab2f325266ffd7ed3fbc1bcdf71) )
	ROM_LOAD( "ax0403m01.ic13", 0x3000000, 0x1000000, CRC(911d0674) SHA1(eec35badcfbfe412b7104a86c2111f5a1b5fb5cd) )
	ROM_LOAD( "ax0404m01.ic14", 0x4000000, 0x1000000, CRC(f82a4ca3) SHA1(da686d86e176a9f24874d2916b1932f03a99a52d) )
	ROM_LOAD( "ax0405m01.ic15", 0x5000000, 0x1000000, CRC(b88298d7) SHA1(490c3ec471018895b7268ee33498dddaccbbfd5a) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "ax0401f01.bin", 0, 4, CRC(394b52c9) SHA1(aa05d82e7c384f536cf68af48b5c0eb89e6f5dfa) )
ROM_END

ROM_START( demofist )
	AW_BIOS

	ROM_REGION( 0x8000000, "rom_board", ROMREGION_ERASE)
	ROM_LOAD( "ax0601p01.ic18", 0x0000000, 0x0800000, CRC(0efb38ad) SHA1(9400e37efe3e936474d74400ebdf28ad0869b67b) )
	ROM_LOAD( "ax0601m01.ic11", 0x1000000, 0x1000000, CRC(12fda2c7) SHA1(3afbac221ffe249386e4cb50b4edd013d9a40062) )
	ROM_LOAD( "ax0602m01.ic12", 0x2000000, 0x1000000, CRC(aea61fdf) SHA1(0a088848bbf7a47df8b44b69bf72ed0d4a1088f8) )
	ROM_LOAD( "ax0603m01.ic13", 0x3000000, 0x1000000, CRC(d5879d35) SHA1(977cd3b373c6f94eb21ffb24ff564971d3d633e5) )
	ROM_LOAD( "ax0604m01.ic14", 0x4000000, 0x1000000, CRC(a7b09048) SHA1(229fa2332b58fec2a712c3ebd672662f35a9485a) )
	ROM_LOAD( "ax0605m01.ic15", 0x5000000, 0x1000000, CRC(18d8437e) SHA1(fe2e189e40a89141335e754268d29d46e3eb3bb8) )
	ROM_LOAD( "ax0606m01.ic16", 0x6000000, 0x1000000, CRC(42c81617) SHA1(1cc686af5e3fc56143836e3dcc0067893f82fcf9) )
	ROM_LOAD( "ax0607m01.ic17", 0x7000000, 0x1000000, CRC(96e5aa84) SHA1(e9841f550f2ef409d97004542bcadabb6b9e84af) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "ax0601f01.bin", 0, 4, CRC(25c9a3ae) SHA1(060c3fa1f8cd7d41785630db22e107790ade702a) )
ROM_END

ROM_START( rumblef )
	AW_BIOS

	ROM_REGION( 0x8000000, "rom_board", ROMREGION_ERASE)
	ROM_LOAD( "ax1801p01.ic18", 0x0000000, 0x0800000, CRC(2f7fb163) SHA1(bf819d798d9a3a7bc754e111a3f53b9db6d6042a) )
	ROM_LOAD( "ax1801m01.ic11", 0x1000000, 0x1000000, CRC(c38aa61c) SHA1(e2f688a0aa8b0119f5fd3d53c8904e035d43a4b1) )
	ROM_LOAD( "ax1802m01.ic12", 0x2000000, 0x1000000, CRC(72e0ebc8) SHA1(e85300a405ea14c4c9d857eb9685c93faaca1d56) )
	ROM_LOAD( "ax1803m01.ic13", 0x3000000, 0x1000000, CRC(d0f59d98) SHA1(b854796087e9f76a13a21da8249f7224e451e129) )
	ROM_LOAD( "ax1804m01.ic14", 0x4000000, 0x1000000, CRC(15595cba) SHA1(8dd06d1f986cd21a58d20b662b11ed7ba8a6ff7a) )
	ROM_LOAD( "ax1805m01.ic15", 0x5000000, 0x1000000, CRC(3d3f8e0d) SHA1(364a0bda890722b9fb72171f96c742b8f3fef23e) )
	ROM_LOAD( "ax1806m01.ic16", 0x6000000, 0x1000000, CRC(ac2751bb) SHA1(5070fa12bf109ab87e8f7ea46ac4ae78a73105da) )
	ROM_LOAD( "ax1807m01.ic17", 0x7000000, 0x1000000, CRC(3b2fbdb0) SHA1(f9f7e06785d3a07282247aaedd9999aa7c2670b9) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "ax1801f01.bin", 0, 4, CRC(5b2e82d9) SHA1(de0d9c2511c72b95777897403cb63b690f74dfa1))
ROM_END

ROM_START( ngbc )
	AW_BIOS

	ROM_REGION( 0xf000000, "rom_board", ROMREGION_ERASE)
	ROM_LOAD( "ax3301p01.fmem1", 0x0000000, 0x0800000, CRC(6dd78275) SHA1(72d4cab58dbcebd666db21aeef190378ef447580) )
	ROM_LOAD( "ax3301m01.mrom1", 0x1000000, 0x2000000, CRC(e6013de9) SHA1(ccbc7c2e76153348646d75938d5c008dc80df17d) )
	ROM_LOAD( "ax3301m01.mrom2", 0x3000000, 0x2000000, CRC(f7cfef6c) SHA1(c9e6231499a9c9c8650d9e61f34ff1fcce8d442c) )
	ROM_LOAD( "ax3301m01.mrom3", 0x5000000, 0x2000000, CRC(0cdf8647) SHA1(0423f96842bef2c2ff454318dc6960b5052c0551) )
	ROM_LOAD( "ax3301m01.mrom4", 0x7000000, 0x2000000, CRC(2f031db0) SHA1(3214735f04fadf160137f0585bfc1a27eeecfac6) )
	ROM_LOAD( "ax3301m01.mrom5", 0x9000000, 0x2000000, CRC(f6668aaa) SHA1(6a78f8f0c7d7a71854ff87329290d38970cfb476) )
	ROM_LOAD( "ax3301m01.mrom6", 0xb000000, 0x2000000, CRC(5cf32fbd) SHA1(b6ae0abe5791b3d6f8db07b8c8ca22219a153801) )
	ROM_LOAD( "ax3301m01.mrom7", 0xd000000, 0x2000000, CRC(26d9da53) SHA1(0015b4be670005a451274de68279b4302fc42a97) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "ax3301f01.bin", 0, 4, CRC(9afe949b) SHA1(4f7b039f3287da61a53a2d012993bfb57e1459bd) )
ROM_END

ROM_START( kofnw )
	AW_BIOS

	ROM_REGION( 0x8000000, "rom_board", ROMREGION_ERASE)
	ROM_LOAD( "ax2201p01.ic18", 0x0000000, 0x0800000, CRC(27aab918) SHA1(41c5ddd8bd4c91481750606ab44aa115b5fe01d0) )
	ROM_LOAD( "ax2201m01.ic11", 0x1000000, 0x1000000, CRC(22ea665b) SHA1(292c92c9ae43eea2d1c27cedfb89c3956b8dea32) )
	ROM_LOAD( "ax2202m01.ic12", 0x2000000, 0x1000000, CRC(7fad1bea) SHA1(89f3f88af48973a4685955d86ef97a1487b8e7a8) )
	ROM_LOAD( "ax2203m01.ic13", 0x3000000, 0x1000000, CRC(78986ca4) SHA1(5a6c8c12955573f33361d2c6f20f85de35ac7bae) )
	ROM_LOAD( "ax2204m01.ic14", 0x4000000, 0x1000000, CRC(6ffbeb04) SHA1(975062cf364589dbdd5c5cb5ca945f76d87fc120) )
	ROM_LOAD( "ax2205m01.ic15", 0x5000000, 0x1000000, CRC(2851b791) SHA1(566ef95ea066b7bf548986085670242be217befc) )
	ROM_LOAD( "ax2206m01.ic16", 0x6000000, 0x1000000, CRC(e53eb965) SHA1(f50cd53a5859f081d8a278d24a519c9d9b49ab96) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "ax2201f01.bin", 0, 4, CRC(b1fff0c8) SHA1(d83177e3672378a2bbc08653b4b73704333ca30a) )
ROM_END

ROM_START( kofnwj )
	AW_BIOS

	ROM_REGION( 0x8000000, "rom_board", ROMREGION_ERASE)
	    ROM_LOAD( "ax2201p01.ic18", 0x0000000, 0x0800000, CRC(ecc4a5c7) SHA1(97c2ef2be95b39bc978474a8243740df50255a8b) )
	    /* these are taken from the above set, game *seems* to work fine with these ... */
	ROM_LOAD( "ax2201m01.ic11", 0x1000000, 0x1000000, CRC(22ea665b) SHA1(292c92c9ae43eea2d1c27cedfb89c3956b8dea32) )
	ROM_LOAD( "ax2202m01.ic12", 0x2000000, 0x1000000, CRC(7fad1bea) SHA1(89f3f88af48973a4685955d86ef97a1487b8e7a8) )
	ROM_LOAD( "ax2203m01.ic13", 0x3000000, 0x1000000, CRC(78986ca4) SHA1(5a6c8c12955573f33361d2c6f20f85de35ac7bae) )
	ROM_LOAD( "ax2204m01.ic14", 0x4000000, 0x1000000, CRC(6ffbeb04) SHA1(975062cf364589dbdd5c5cb5ca945f76d87fc120) )
	ROM_LOAD( "ax2205m01.ic15", 0x5000000, 0x1000000, CRC(2851b791) SHA1(566ef95ea066b7bf548986085670242be217befc) )
	ROM_LOAD( "ax2206m01.ic16", 0x6000000, 0x1000000, CRC(e53eb965) SHA1(f50cd53a5859f081d8a278d24a519c9d9b49ab96) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "ax2201f01.bin", 0, 4, CRC(b1fff0c8) SHA1(d83177e3672378a2bbc08653b4b73704333ca30a) )
ROM_END

ROM_START( kov7sprt )
	AW_BIOS

	ROM_REGION( 0x8000000, "rom_board", ROMREGION_ERASE)
	ROM_LOAD( "ax1301p01.ic18", 0x0000000, 0x0800000, CRC(6833a334) SHA1(646aaa578e09ad23bc9c7f4fbdfb3c1486916fd3) )
	ROM_LOAD( "ax1301m01.ic11", 0x1000000, 0x1000000, CRC(58ae7ca1) SHA1(e91975697b797ea05488ace649cbb9964b4cd500) )
	ROM_LOAD( "ax1301m02.ic12", 0x2000000, 0x1000000, CRC(871ea03f) SHA1(6806910832ca271a9240aca8e91279556e5b0cb7) )
	ROM_LOAD( "ax1301m03.ic13", 0x3000000, 0x1000000, CRC(abc328bc) SHA1(d9271d4e5abe76d31de0f60a5c106260338d42e9) )
	ROM_LOAD( "ax1301m04.ic14", 0x4000000, 0x1000000, CRC(25a176d1) SHA1(6d815bf6acb645fead060733660e24fb0d44282d) )
	ROM_LOAD( "ax1301m05.ic15", 0x5000000, 0x1000000, CRC(e6573a93) SHA1(0666e52d0088263f28938e4c8aae201e604ec1f2) )
	ROM_LOAD( "ax1301m06.ic16", 0x6000000, 0x1000000, CRC(cb8cacb4) SHA1(5d008e8a934451b9bfa33fedfd492c86d9226ef5) )
	ROM_LOAD( "ax1301m07.ic17", 0x7000000, 0x1000000, CRC(0ca92213) SHA1(115c50fa55e6de3439de23e74621695510c6a7ba) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "ax1301f01.bin", 0, 4, CRC(2a189821) SHA1(d15c9df83782d49ea85e201cba844f5a9e33f15c) )
ROM_END

ROM_START( ggisuka )
	AW_BIOS

	ROM_REGION( 0x9000000, "rom_board", ROMREGION_ERASE)
	ROM_LOAD( "ax1201p01.ic18", 0x0000000, 0x0800000, CRC(0a78d52c) SHA1(e9006dc43cd11d5ba49a092a1dff31dc10700c28) )
	ROM_LOAD( "ax1201m01.ic10", 0x0800000, 0x1000000, CRC(df96ce30) SHA1(25a9f743b1c2b11896d0c7a2dc1c198fc977aaca) )	// second half is blank
	ROM_LOAD( "ax1202m01.ic11", 0x1000000, 0x1000000, CRC(dfc6fd67) SHA1(f9d35b18a03d22f70feda42d314b0f9dd54eea55) )
	ROM_LOAD( "ax1203m01.ic12", 0x2000000, 0x1000000, CRC(bf623df9) SHA1(8b9a8e8100ff6d2ce9a982ab8eb1d542f1c7af03) )
	ROM_LOAD( "ax1204m01.ic13", 0x3000000, 0x1000000, CRC(c80c3930) SHA1(5c39fde36e2ebbfe72967d7d0202eb454a8d3bbe) )
	ROM_LOAD( "ax1205m01.ic14", 0x4000000, 0x1000000, CRC(e99a269d) SHA1(a52148b82b0338b8bad8b52985302eaf81a4cfde) )
	ROM_LOAD( "ax1206m01.ic15", 0x5000000, 0x1000000, CRC(807ab795) SHA1(17c86b1a56333c05b68ff84f83e964d013c1819c) )
	ROM_LOAD( "ax1207m01.ic16", 0x6000000, 0x1000000, CRC(6636d1b8) SHA1(9bd8fc114557f6fbe772f85eeb246f7336d4255e) )
	ROM_LOAD( "ax1208m01.ic17", 0x7000000, 0x1000000, CRC(38bda476) SHA1(0234a6f5fbaf5e958b3ba0db311dff157f80addc) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "ax1201f01.bin", 0, 4, CRC(325cf843) SHA1(c51d19a4fce37433f37e7ce23801a7fc4e09013d) )
ROM_END

ROM_START( maxspeed )
	AW_BIOS

	ROM_REGION( 0x9000000, "rom_board", ROMREGION_ERASE)
	ROM_LOAD( "ax0501p01.ic18", 0x0000000, 0x0800000, CRC(e1651867) SHA1(49caf82f4b111da312b14bb0a9c31e3732b4b24e) )
	ROM_LOAD( "ax0501m01.ic11", 0x0800000, 0x1000000, CRC(4a847a59) SHA1(7808bcd357b85861082b426dbe34a20ae7016f6a) )
	ROM_LOAD( "ax0502m01.ic12", 0x1000000, 0x1000000, CRC(2580237f) SHA1(2e92c940f95edae33d6a7e8a071544a9083a0fd6) )
	ROM_LOAD( "ax0503m01.ic13", 0x2000000, 0x1000000, CRC(e5a3766b) SHA1(1fe6e072adad27ac43c0bff04e3c448678aabc18) )
	ROM_LOAD( "ax0504m01.ic14", 0x3000000, 0x1000000, CRC(7955b55a) SHA1(927f58d6961e702c2a8afce79bac5e5cff3dfed6) )
	ROM_LOAD( "ax0505m01.ic15", 0x4000000, 0x1000000, CRC(e8ccc660) SHA1(a5f414f200a0d41e958430d0fc2d4e1fda1cc67c) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "ax0501f01.bin", 0, 4, CRC(c35d9a95) SHA1(bf260caf33821be51014b06480a11ec982fa4fcd) )
ROM_END

ROM_START( vfurlong )
	AW_BIOS

	ROM_REGION( 0x9000000, "rom_board", ROMREGION_ERASE)
	ROM_LOAD( "ax2001p01.ic18", 0x0000000, 0x0800000, CRC(17ea9aa9) SHA1(c68500e9b3407a9d4b20f2678718ce475f179f7d) )
	ROM_LOAD( "ax2001m01.ic11", 0x1000000, 0x1000000, CRC(64460b24) SHA1(044857d6593897d303622e005a63ca7b3acd7453) )
	ROM_LOAD( "ax2002m01.ic12", 0x2000000, 0x1000000, CRC(d4da357f) SHA1(c462cddec9a369a1a5595676de76499d56683ea9) )
	ROM_LOAD( "ax2003m01.ic13", 0x3000000, 0x1000000, CRC(ad046ad7) SHA1(ac7c46e458595714e327526354e1111fb25b44e0) )
	ROM_LOAD( "ax2004m01.ic14", 0x4000000, 0x1000000, CRC(4d555d7c) SHA1(a5eccc920bdb7ad9cf57c0e1ef6a905c6b9eee45) )
	ROM_LOAD( "ax2005m01.ic15", 0x5000000, 0x1000000, CRC(785208e2) SHA1(6ba5bd3901c5b1d71abcc8d833a011bd4abae6b6) )
	ROM_LOAD( "ax2006m01.ic16", 0x6000000, 0x1000000, CRC(8134ec55) SHA1(843e473d4f99237ded641cce9515b7802cfe3742) )
	ROM_LOAD( "ax2007m01.ic17", 0x7000000, 0x1000000, CRC(d0557e8a) SHA1(df8057597eb690bd18c5d26736f5d4f86e3b1225) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "ax2001f01.bin", 0, 4, CRC(42d45ab8) SHA1(25bc9c046ff085e5219109316fbc0c1fae183d1f) )
ROM_END

ROM_START( salmankt )
	AW_BIOS

	ROM_REGION( 0x9000000, "rom_board", ROMREGION_ERASE)
	ROM_LOAD( "ax1401p01.ic18", 0x0000000, 0x0800000, CRC(28d779e0) SHA1(dab785a595de5f474c18c713e672949176a5b1b5) )
	ROM_LOAD( "ax1401m01.ic11", 0x1000000, 0x1000000, CRC(fd7af845) SHA1(0c8f5a91662e46d5c187a0758af95082183cdf69) )
	ROM_LOAD( "ax1402m01.ic12", 0x2000000, 0x1000000, CRC(88d458a7) SHA1(c661196d5a9640013995c8aa6a5bea057d984f87) )
	ROM_LOAD( "ax1403m01.ic13", 0x3000000, 0x1000000, CRC(074f7c4b) SHA1(4955e4f333d15d5d9cc69bb9658f29a37e912012) )
	ROM_LOAD( "ax1404m01.ic14", 0x4000000, 0x1000000, CRC(af4e3829) SHA1(18d9e8a8d8e930ad697b686a98f31ea175f5fd4a) )
	ROM_LOAD( "ax1405m01.ic15", 0x5000000, 0x1000000, CRC(b548446f) SHA1(8b4661e601e36c067dff2530aff4f7ea76e1c21e) )
	ROM_LOAD( "ax1406m01.ic16", 0x6000000, 0x1000000, CRC(437673e6) SHA1(66f7e5f246ebbb1bdbf074da41ec16bf32720a82) )
	ROM_LOAD( "ax1407m01.ic17", 0x7000000, 0x1000000, CRC(6b6acc0a) SHA1(a8c692c875271a0806460caa79c67fd756231273) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "ax1401f01.bin", 0, 4, CRC(67e742ae) SHA1(7c2e955bcb753ff8756db4bd75409583ffbadf62) )
ROM_END

ROM_START( ftspeed )
	AW_BIOS

	ROM_REGION( 0x9000000, "rom_board", ROMREGION_ERASE)
	ROM_LOAD( "ax1701p01.ic18", 0x0000000, 0x0800000, CRC(480cade7) SHA1(487d4b27d7e5196d8321c5a80175ec7b1b32c1e8) )
	ROM_LOAD( "ax1701m01.ic11", 0x1000000, 0x1000000, CRC(7dcdc784) SHA1(5eeef9a760a0b090ed5aad8b7bdee2baa69a088b) )
	ROM_LOAD( "ax1702m01.ic12", 0x2000000, 0x1000000, CRC(06c9bf85) SHA1(636262dca7140397436646754fb204b97aa08ce9) )
	ROM_LOAD( "ax1703m01.ic13", 0x3000000, 0x1000000, CRC(8f8e0224) SHA1(2a9a17ed726913c00bf1c6a94bdd4fb32e800868) )
	ROM_LOAD( "ax1704m01.ic14", 0x4000000, 0x1000000, CRC(fbb4bb16) SHA1(b582680a880166c5bbdd2ad77b7903fedf9b01ad) )
	ROM_LOAD( "ax1705m01.ic15", 0x5000000, 0x1000000, CRC(d9e69723) SHA1(22ed4dab76320a286b66215607bda5adf21304d1) )
	ROM_LOAD( "ax1706m01.ic16", 0x6000000, 0x1000000, CRC(804b2eb2) SHA1(fcca02a5a8c09eb16548255115fb105c9c49c4e0) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "ax1701f01.bin", 0, 4, CRC(f3f03c35) SHA1(2a8329a29cdcc0219e9360cc573c0f3ad44d0175) )
ROM_END

ROM_START( kofxi )
	AW_BIOS

	ROM_REGION( 0xf000000, "rom_board", ROMREGION_ERASE)
	ROM_LOAD( "ax3201p01.fmem1", 0x0000000, 0x0800000, CRC(6dbdd71b) SHA1(cce3897b104f5d923d8136485fc80eb9717ff4b5) )
	ROM_LOAD( "ax3201m01.mrom1", 0x1000000, 0x2000000, CRC(7f9d6af9) SHA1(001064ad1b8c3408efe799dc766c2728dc6512a9) )
	ROM_LOAD( "ax3202m01.mrom2", 0x3000000, 0x2000000, CRC(1ae40afa) SHA1(9ee7957c86cc3a71e6971ddcd906a82c5b1e16f1) )
	ROM_LOAD( "ax3203m01.mrom3", 0x5000000, 0x2000000, CRC(8c5e3bfd) SHA1(b5443e2a1b88642cc57c5287a3122376c2d48de9) )
	ROM_LOAD( "ax3204m01.mrom4", 0x7000000, 0x2000000, CRC(ba97f80c) SHA1(36f672fe833e13f0bab036b02c39123066327e20) )
	ROM_LOAD( "ax3205m01.mrom5", 0x9000000, 0x2000000, CRC(3c747067) SHA1(54b7ff73d618e2e4e40c125c6cfe99016e69ad1a) )
	ROM_LOAD( "ax3206m01.mrom6", 0xb000000, 0x2000000, CRC(cb81e5f5) SHA1(07faee02a58ac9c600ab3cdd525d22c16b35222d) )
	ROM_LOAD( "ax3207m01.mrom7", 0xd000000, 0x2000000, CRC(164f6329) SHA1(a72c8cbe4ac7b98edda3d4434f6c81a370b8c39b) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "ax3201f01.bin", 0, 4, CRC(065d7fc6) SHA1(e4f18126e9f4e6747ffc5d0664766986fc07c127) )
ROM_END

ROM_START( dirtypig )
	AW_BIOS

	ROM_REGION( 0x8000000, "rom_board", ROMREGION_ERASE)
	ROM_LOAD( "837-14695.u3",  0x0000000, 0x1000000, CRC(9fdd7d07) SHA1(56d580dda116823ea5dc5e1bd5154463a476866a) )
	ROM_LOAD( "837-14695.u1",  0x1000000, 0x1000000, CRC(a91d2fcb) SHA1(8414386c09ba36ea581c8161f6cf2a13cc5ae516) )
	ROM_LOAD( "837-14695.u2",  0x2000000, 0x1000000, CRC(4d82152f) SHA1(a448983d4e81eb6485b62f23a6c99d1112a20c21) )
	ROM_LOAD( "837-14695.u4",  0x3000000, 0x1000000, CRC(3342f237) SHA1(e617b0e1f8d8da9783c58ab98eb91de2363ec36f) )
	ROM_LOAD( "837-14695.u14", 0x4000000, 0x1000000, CRC(55470242) SHA1(789036189ae5488a9da565774bdf91b49cd8264e) )
	ROM_LOAD( "837-14695.u15", 0x5000000, 0x1000000, CRC(d239a549) SHA1(71f3c1c2ae2a9b6f09f30e7be3bb11ba111276ae) )
	ROM_LOAD( "837-14695.u16", 0x6000000, 0x1000000, CRC(730180a4) SHA1(017b82e2d2744695e3e521d35a8511ecc1c8ab43) )
	ROM_LOAD( "837-14695.u17", 0x7000000, 0x1000000, CRC(16bb5992) SHA1(18772587272aba1d50a48d384f472276c3b48d96) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "837-14695.bin", 0, 4, CRC(553dd361) SHA1(a60a26b5ee786cf0bb3d09bb6f00374598fbd7cc) )
ROM_END

ROM_START( mslug6 )
	AW_BIOS

	ROM_REGION( 0xf000000, "rom_board", ROMREGION_ERASE)
	ROM_LOAD( "ax3001p01.fmem1", 0x0000000, 0x0800000, CRC(af67dbce) SHA1(5aba108caf3e4ced6994bc26e752d4e225c231e8) )
	ROM_LOAD( "ax3001m01.mrom1", 0x1000000, 0x2000000, CRC(e56417ee) SHA1(27692ad5c1093aff0973d2aafd01a5e30c7bfbbe) )
	ROM_LOAD( "ax3002m01.mrom2", 0x3000000, 0x2000000, CRC(1be3bbc1) SHA1(d75ce5c855c9c4eeacdbf84d440c73a94de060fe) )
	ROM_LOAD( "ax3003m01.mrom3", 0x5000000, 0x2000000, CRC(4fe37370) SHA1(85d51db94c3e34265e37b636d6545ed2801ba5a6) )
	ROM_LOAD( "ax3004m01.mrom4", 0x7000000, 0x2000000, CRC(2f4c4c6f) SHA1(5815c28fdaf0429003986e725c0015fe4c08721f) )
	ROM_COPY( "rom_board", 0x7000000, 0x9000000, 0x2000000)

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "ax3001f01.bin", 0, 4, CRC(0b9939e9) SHA1(4ca1225c7c9993542a67035a054ac579ed021de5) )
ROM_END

ROM_START( samsptk )
	AW_BIOS

	ROM_REGION( 0x10000000, "rom_board", ROMREGION_ERASE)
	ROM_LOAD( "ax2901p01.fmem1", 0x0000000, 0x0800000, CRC(58e0030b) SHA1(ed8a66833beeb56d83770123eff28df0f25221d1) )
	ROM_LOAD( "ax2901m01.mrom1", 0x1000000, 0x2000000, CRC(dbbbd90d) SHA1(102ee0b249a3e0ca2f659b6c515816c522ad78d0) )
	ROM_LOAD( "ax2902m01.mrom2", 0x3000000, 0x2000000, CRC(a3bd7890) SHA1(9b8d934d6ebc3ef688cd8a6de47657a0663fea10) )
	ROM_LOAD( "ax2903m01.mrom3", 0x5000000, 0x2000000, CRC(56f50fdd) SHA1(8a5a4a99108c0279056998046c7b332e80121dee) )
	ROM_LOAD( "ax2904m01.mrom4", 0x7000000, 0x2000000, CRC(8a3ae175) SHA1(966f527a92e24c8eb770344697f2edf6140cf971) )
	ROM_LOAD( "ax2905m01.mrom5", 0x9000000, 0x2000000, CRC(429877ba) SHA1(88e1f3bc682b18d331e328ef8754065109cf9bda) )
	ROM_LOAD( "ax2906m01.mrom6", 0xb000000, 0x2000000, CRC(cb95298d) SHA1(5fb5d5a0d6801df61101a1b23de0c14ff29ef654) )
	ROM_LOAD( "ax2907m01.mrom7", 0xd000000, 0x2000000, CRC(48015081) SHA1(3c0a0a6dc9ab7bf889579477699e612c3092f9bf) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "ax2901f01.bin", 0, 4, CRC(8a6267aa) SHA1(9705bed35acb87d578f0efcf4f74b2a4b1a7be2e) )
ROM_END

ROM_START( ggx15 )
	AW_BIOS

	ROM_REGION( 0x8000000, "rom_board", ROMREGION_ERASE)
	ROM_LOAD( "ax0801p01.ic18", 0x0000000, 0x0800000, CRC(d920c6bb) SHA1(ab34bbef3c71396447bc5322d8e8786041fc832a) )
	ROM_LOAD( "ax0801m01.ic11", 0x1000000, 0x1000000, CRC(61879b2d) SHA1(9592fbd979cef9d8f465cd92d0f00b9c13ecf7ba) )
	ROM_LOAD( "ax0802m01.ic12", 0x2000000, 0x1000000, CRC(c0ff124d) SHA1(dd403d10de2f097fbaa6b93bc311e2b9e893828d) )
	ROM_LOAD( "ax0803m01.ic13", 0x3000000, 0x1000000, CRC(4400c89a) SHA1(4e13536c01103ecfbfc9e3e33746ceae7a91a520) )
	ROM_LOAD( "ax0804m01.ic14", 0x4000000, 0x1000000, CRC(70f58ab4) SHA1(cd2def19bbad945c87567f8d28f3a2a179a7f7f6) )
	ROM_LOAD( "ax0805m01.ic15", 0x5000000, 0x1000000, CRC(72740e45) SHA1(646eded89f10008c9176cd6772a8ac9d1bf4271a) )
	ROM_LOAD( "ax0806m01.ic16", 0x6000000, 0x1000000, CRC(3bf8ecba) SHA1(43e7fbf21d8ee60bab72ce558640730fd9c3e3b8) )
	ROM_LOAD( "ax0807m01.ic17", 0x7000000, 0x1000000, CRC(e397dd79) SHA1(5fec32dc19dd71ef0d451f8058186f998015723b) )

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "ax0801f01.bin", 0, 4, CRC(a36e5017) SHA1(fd763a4c708fe37c7561ba5b5d0b8d2118cff16b) )
ROM_END

ROM_START( rumblef2 )
	AW_BIOS

	ROM_REGION( 0x10000000, "rom_board", ROMREGION_ERASE)
	ROM_LOAD( "ax3401p01.fmem1", 0x0000000, 0x0800000, CRC(a33601cf) SHA1(2dd60a9c3a2517f2257ab69288fa95645de133fa) )
	ROM_LOAD( "ax3401m01.mrom1", 0x1000000, 0x2000000, CRC(60894d4c) SHA1(5b21af3c7c82d4d64bfd8498c26283111ada1298) )
	ROM_LOAD( "ax3402m01.mrom2", 0x3000000, 0x2000000, CRC(e4224cc9) SHA1(dcab06fcf48cda286f93d2b37f03a83abf3230cb) )
	ROM_LOAD( "ax3403m01.mrom3", 0x5000000, 0x2000000, CRC(081c0edb) SHA1(63a3f1b5f9d7ca4367868c492236406f23996cc3) )
	ROM_LOAD( "ax3404m01.mrom4", 0x7000000, 0x2000000, CRC(a426b443) SHA1(617aab42e432a80b0663281fb7faa6c14ef4f149) )
	ROM_LOAD( "ax3405m01.mrom5", 0x9000000, 0x2000000, CRC(4766ce56) SHA1(349b82013a75905ae5520b14a87702c9038a5def) )
	ROM_COPY( "rom_board", 0x9000000, 0xb000000, 0x2000000)

	ROM_REGION( 4, "rom_key", 0 )
	ROM_LOAD( "ax3401f01.bin", 0, 4, CRC(952919a1) SHA1(d343fdbbd1d8b651401133f21facc1584bb66c04) )
ROM_END

/* All games have the regional titles at the start of the IC22 rom in the following order

  JAPAN
  USA
  EXPORT (EURO in some titles)
  KOREA (ASIA in some titles)
  AUSTRALIA
  UNUSED
  UNUSED
  UNUSED

  with the lists below it has been assumed that if the title is listed for a region
  then it is available / works in that region, this has not been confirmed as correct.

*/

#define GAME_FLAGS (GAME_IMPERFECT_GRAPHICS|GAME_IMPERFECT_SOUND|GAME_NOT_WORKING)

/* Main board and game specific BIOS */
/* Naomi */ GAME( 1998, naomi,    0,        naomi,   naomi,    naomi,   ROT0, "Sega", "Naomi Bios", GAME_FLAGS|GAME_IS_BIOS_ROOT )
/* game  */ GAME( 1998, hod2bios, 0,        naomi,   naomi,    0,       ROT0, "Sega", "Naomi House of the Dead 2 Bios", GAME_FLAGS|GAME_IS_BIOS_ROOT )
/* game  */ GAME( 1999, f355bios, 0,        naomi,   naomi,    0,       ROT0, "Sega", "Naomi Ferrari F355 Challenge Bios", GAME_FLAGS|GAME_IS_BIOS_ROOT )
/* game  */ GAME( 1999, airlbios, 0,        naomi,   naomi,    0,       ROT0, "Sega", "Naomi Airline Pilots Deluxe Bios", GAME_FLAGS|GAME_IS_BIOS_ROOT )
/* Naomi2*/ GAME( 2001, naomi2,   0,        naomi,   naomi,    0,       ROT0, "Sega", "Naomi 2 Bios", GAME_FLAGS|GAME_IS_BIOS_ROOT )
/* GDROM */ GAME( 2001, naomigd,  0,        naomi,   naomi,    naomi,   ROT0, "Sega", "Naomi GD-ROM Bios", GAME_FLAGS|GAME_IS_BIOS_ROOT )

/* 834-xxxxx (Sega Naomi cart with game specific BIOS sets) */
/* 13636 */ GAME( 1998, hotd2,    hod2bios, naomi,   hotd2,    hotd2,   ROT0, "Sega", "House of the Dead 2", GAME_FLAGS ) /* specific BIOS "hod2bios" needed */
/* 13636 */ GAME( 1998, hotd2o,   hotd2,    naomi,   hotd2,    hotd2,   ROT0, "Sega", "House of the Dead 2 (original)", GAME_FLAGS ) /* specific BIOS "hod2bios" needed */
/* 13636 */ GAME( 1998, hotd2p,   hotd2,    naomi,   hotd2,    hotd2,   ROT0, "Sega", "House of the Dead 2 (prototype)", GAME_FLAGS ) /* specific BIOS "hod2bios" needed */
/* 13842 */ GAME( 1999, f355,     f355bios, naomi,   naomi,    0,       ROT0, "Sega", "Ferrari F355 Challenge", GAME_FLAGS ) /* specific BIOS "f355bios" needed */
/* 13950 */ GAME( 1999, f355twin, f355bios, naomim2, naomi,    0,       ROT0, "Sega", "Ferrari F355 Challenge (Twin)", GAME_FLAGS ) /* specific BIOS "f355bios" needed */
/* ????? */ GAME( 2001, f355twn2, f355bios, naomim2, naomi,    0,       ROT0, "Sega", "Ferrari F355 Challenge 2 (Twin)", GAME_FLAGS ) /* specific BIOS "f355bios" needed */
/* ????? */ GAME( 1999, alpiltdx, airlbios, naomim2, naomi,    0,       ROT0, "Sega", "Airline Pilots Deluxe (Rev B)", GAME_FLAGS ) /* specific BIOS "airlbios" needed */

/* 840-xxxxx (Sega Naomi cart games)*/
/* 0001 */ GAME( 1998, dybbnao,  naomi,    naomi,   dybbnao,  naomi,    ROT0, "Sega", "Dynamite Baseball NAOMI (JPN)", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0002 */ GAME( 1999, crzytaxi, naomi,    naomim2, crzytaxi, naomi,    ROT0, "Sega", "Crazy Taxi (JPN, USA, EXP, KOR, AUS)", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0003 */ GAME( 1999, zombrvn,  naomi,    naomim2, zombrvn,  naomi,    ROT0, "Sega", "Zombie Revenge (JPN, USA, EXP, KOR, AUS)", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0005 */ GAME( 1999, alpilota, naomi,    naomim2, alpilota, naomi,    ROT0, "Sega", "Airline Pilots (Rev A)", GAME_FLAGS ) /* specific BIOS "airlbios" needed */
/* 0007 */ GAME( 1999, ggram2,   naomi,    naomim2, naomi,    naomi,    ROT0, "Sega", "Giant Gram: All Japan Pro Wrestling 2 (JPN, USA, EXP, KOR, AUS)", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0010 */ GAME( 1999, vs2_2k,   naomi,    naomim2, naomi,    naomi,    ROT0, "Sega", "Virtua Striker 2 Ver. 2000 (JPN, USA, EXP, KOR, AUS) (Rev C)", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0011 */ GAME( 1999, toyfight, naomi,    naomim2, naomi,    naomi,    ROT0, "Sega", "Toy Fighter", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0012 */ GAME( 1999, smlg99,   naomi,    naomim2, dybbnao,  naomi,    ROT0, "Sega", "Super Major League '99", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0013 */ GAME( 1999, jambo,    naomi,    naomim2, jambo,    naomi,    ROT0, "Sega", "Jambo! Safari (JPN, USA, EXP, KOR, AUS) (Rev A)", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0015 */ GAME( 1999, vtennis,  naomi,    naomim2, naomi,    naomi,    ROT0, "Sega", "Virtua Tennis (USA, EXP, KOR, AUS) / Power Smash (JPN)", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0016 */ GAME( 1999, derbyoc,  naomi,    naomim2, naomi,    naomi,    ROT0, "Sega", "Derby Owners Club (JPN, USA, EXP, KOR, AUS) (Rev B)", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0017 */ GAME( 1999, otrigger, naomi,    naomim2, naomi,    naomi,    ROT0, "Sega", "OutTrigger (JPN, USA, EXP, KOR, AUS)", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0018 */ GAME( 1999, sgtetris, naomi,    naomim2, naomi,    naomi,    ROT0, "Sega", "Sega Tetris", GAME_FLAGS )
/* 0019 */ GAME( 1999, dybb99,   naomi,    naomim2, dybbnao,  naomi,    ROT0, "Sega", "Dynamite Baseball '99 (JPN) / World Series '99 (USA, EXP, KOR, AUS) (Rev B)", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0020 */ GAME( 1999, samba,    naomi,    naomim2, naomi,    naomi,    ROT0, "Sega", "Samba De Amigo (JPN) (Rev B)", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0021 */ GAME( 2000, virnbap,  virnba,   naomi,   naomi,    naomi,    ROT0, "Sega", "Virtua NBA (prototype)", GAME_FLAGS )
/* 0021 */ GAME( 2000, virnbao,  virnba,   naomim2, naomi,    naomi,    ROT0, "Sega", "Virtua NBA (JPN, USA, EXP, KOR, AUS) (original)", GAME_FLAGS )
/* 0021-01 */ GAME( 2000,virnba, naomi,    naomi,   naomi,    naomi,    ROT0, "Sega", "Virtua NBA (JPN, USA, EXP, KOR, AUS)", GAME_FLAGS )
/* 0022 */ GAME( 2000, tduno2,   naomi,    naomim2, naomi,    naomi,    ROT0, "Sega", "Touch de Uno! 2", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0023 */ GAME( 2000, 18wheelr, naomi,    naomim2, 18wheelr, naomi,    ROT0, "Sega", "18 Wheeler Deluxe (Rev A) (JPN)", GAME_FLAGS )
/* 0023 */ GAME( 2000, 18wheels, 18wheelr, naomim2, 18wheelr, naomi,    ROT0, "Sega", "18 Wheeler (Rev A) (JPN)", GAME_FLAGS )
// 0025 Mars TV
/* 0026 */ GAME( 2000, totd,     naomi,    naomi,   naomi,    naomi,    ROT0, "Sega", "The Typing of the Dead (JPN, USA, EXP, KOR, AUS) (Rev A)", GAME_FLAGS )
/* 0027 */ GAME( 2000, smarinef, naomi,    naomi,   naomi,    naomi,    ROT0, "Sega", "Sega Marine Fishing", GAME_FLAGS )
/* 0028 */ GAME( 2000, vonot,    naomi,    naomim2, naomi,    naomi,    ROT0, "Sega", "Virtual On Oratorio Tangram M.S.B.S. ver5.66 2000 Edition", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0030 */ GAME( 2000, qmegamis, naomi,    naomim1, naomi,    qmegamis, ROT0, "Sega", "Quiz Ah Megamisama (JPN, USA, EXP, KOR, AUS)", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0035 */ GAME( 2000, sstrkfgt, naomi,    naomim2, sstrkfgt, naomi,    ROT0, "Sega", "Sega Strike Fighter (Rev A)", GAME_FLAGS )
// 0037 18 Wheeler - American Pro Trucker
/* 0039 */ GAME( 2000, gram2000, naomi,    naomim1, naomi,    gram2000, ROT0, "Sega", "Giant Gram 2000 (JPN, USA, EXP, KOR, AUS)", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0040 */ GAME( 2000, wwfroyal, naomi,    naomim2, naomi,    naomi,    ROT0, "Sega", "WWF Royal Rumble (JPN, USA, EXP, KOR, AUS)", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0041 */ GAME( 2000, slasho,   naomi,    naomim2, naomi,    naomi,    ROT0, "Sega", "Slashout (JPN, USA, EXP, KOR, AUS)", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0043 */ GAME( 2000, crackndj, naomi,    naomim2, crackndj, naomi,    ROT0, "Sega", "Crackin' DJ", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0044 */ GAME( 2000, csmasho,  csmash,   naomim2, naomi,    naomi,    ROT0, "Sega", "Cosmic Smash (JPN, USA, EXP, KOR, AUS)", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0044 */ GAME( 2000, csmash,   naomi,    naomim2, naomi,    naomi,    ROT0, "Sega", "Cosmic Smash (JPN, USA, EXP, KOR, AUS) (Rev A)", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0047 */ GAME( 2000, samba2k,  naomi,    naomim2, naomi,    naomi,    ROT0, "Sega", "Samba de Amigo ver. 2000", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0048 */ GAME( 2001, alienfnt, naomi,    naomim2, naomi,    naomi,    ROT0, "Sega", "Alien Front (Rev T)", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0048 */ GAME( 2001, alienfnta, alienfnt,naomim2, naomi,    naomi,    ROT0, "Sega", "Alien Front (Rev A)", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0052 */ GAME( 2000, derbyo2k, naomi,    naomi,   naomi,    naomi,    ROT0, "Sega", "Derby Owners Club 2000 (Rev A)", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0064 */ GAME( 2001, wrungp,   naomi,    naomi,   naomi,    naomi,    ROT0, "Sega", "Wave Runner GP", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0073 */ GAME( 2001, inunoos,  naomi,    naomi,   naomi,    naomi,    ROT0, "Sega", "Inu No Osanpo / Dog Walking (Rev A)", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0083 */ GAME( 2001, derbyoc2, naomi,    naomi,   naomi,    naomi,    ROT0, "Sega", "Derby Owners Club II (JPN, USA, EXP, KOR, AUS) (Rev B)", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0084 */ GAME( 2001, vtenis2c, naomi,    naomim1, naomi,    naomi,    ROT0, "Sega", "Virtua Tennis 2 / Power Smash 2 (JPN) (USA, EXP, KOR, AUS) (Cart, Rev A)", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0088 */ GAME( 2001, derbyocw, naomi,    naomi,   naomi,    naomi,    ROT0, "Sega", "Derby Owners Club World Edition (JPN, USA, EXP, KOR, AUS) (Rev D)", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0088 */ GAME( 2001, drbyocwc, derbyocw, naomi,   naomi,    naomi,    ROT0, "Sega", "Derby Owners Club World Edition (JPN, USA, EXP, KOR, AUS) (Rev C)", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0098 */ GAME( 2002, shootopl, naomi,    naomim1, naomi,    naomi,    ROT0, "Sega", "Shootout Pool", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0126 */ GAME( 2003, oinori,   naomi,    naomi,   naomi,    naomi,    ROT0, "Sega", "Oinori-daimyoujin Matsuri", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0128 */ GAME( 2002, shootpl,  naomi,    naomim1, naomi,    naomi,    ROT0, "Sega", "Shootout Pool (JPN, USA, KOR, AUS) / Shootout Pool Prize (EXP)", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0140 */ GAME( 2004, kick4csh, naomi,    naomim1, naomi,    kick4csh, ROT0, "Sega", "Kick '4' Cash", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0150 */ GAME( 2003, mtkob2,   naomi,    naomim1, naomi,    naomi,    ROT0, "Sega", "Mushiking The King Of Beetle 2K3 2nd", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
// 0164 MushiKing The King Of Beetle II ENG
// 0166 Touch De Zunou (Japan)
// 0166 Touch De Zunou (Japan) (Rev A)
/* 0170 */ GAME( 2007, pokasuka, naomi,    naomim4, naomi,    naomi,    ROT0, "Sega", "Pokasuka Ghost", GAME_FLAGS )
/* 0175 */ GAME( 2007, asndynmt, naomi,    naomim4, naomi,    naomi,    ROT0, "Sega", "Asian Dynamite", GAME_FLAGS )
/* 00?? */ GAME( 1999, ringout,  naomi,    naomim2, naomi,    naomi,    ROT0, "Sega", "Ring Out 4x4", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0??? */ GAME( 2001, shootplm, naomi,    naomim1, naomi,    naomi,    ROT0, "Sega", "Shootout Pool Medal", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
// 00?? Crackin' DJ Part 2
// 00?? Touch de Uno!

/* 840-xxxxx (Sega Naomi 2 cart games) */
/* 0046 */ GAME( 2001, wldrider, naomi2,   naomi2,   naomi,    naomi2,   ROT0, "Sega", "Wild Riders (JPN, USA, EXP, KOR, AUS)", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0061 */ GAME( 2001, vstrik3c, naomi2,   naomi2,   naomi,    naomi2,   ROT0, "Sega", "Virtua Striker 3 (USA, EXP, KOR, AUS) (Cart, Rev C)", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0061 */ GAME( 2001, vstrik3cb,vstrik3c, naomi2,   naomi,    naomi2,   ROT0, "Sega", "Virtua Striker 3 (USA, EXP, KOR, AUS) (Cart, Rev B)", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0062 */ GAME( 2002, clubkrte, naomi2,   naomi2,   naomi,    naomi2,   ROT0, "Sega", "Club Kart: European Session (Rev D)", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0080 */ GAME( 2002, vf4cart,  naomi2,   naomi2m2, naomi,    naomi2,   ROT0, "Sega", "Virtua Fighter 4 (Cartridge)", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0087 */ GAME( 2002, kingrt66, naomi2,   naomi2,   naomi,    naomi2,   ROT0, "Sega", "King of Route 66 (Rev A)", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
// 0095 Soul Surfer
/* 0106 */ GAME( 2002, vf4evoct, naomi2,   naomi2m1, naomi,    vf4evoct, ROT0, "Sega", "Virtua Fighter 4 Evolution (Cartridge)", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
// 0129 Club Kart Prize

/* 841-xxxxx ("Licensed by Sega" Naomi cart games)*/
/* 0001 */ GAME( 1999, pstone,   naomi,    naomim2, naomi,    naomi,    ROT0, "Capcom",          "Power Stone (JPN, USA, EUR, ASI, AUS)", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0002 */ GAME( 1999, suchie3,  naomi,    naomim2, naomi_mp, naomi_mp, ROT0, "Jaleco",          "Idol Janshi Suchie-Pai 3 (JPN)", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0003 */ GAME( 1999, doa2,     naomi,    naomim2, naomi,    naomi,    ROT0, "Tecmo",           "Dead or Alive 2 (JPN, USA, EXP, KOR, AUS)", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0003 */ GAME( 2000, doa2m,    doa2,     naomim2, naomi,    naomi,    ROT0, "Tecmo",           "Dead or Alive 2 Millennium (JPN, USA, EXP, KOR, AUS)", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0004 */ GAME( 1999, shangril, naomi,    naomi,   naomi_mp, naomi_mp, ROT0, "Marvelous Ent.",  "Dengen Tenshi Taisen Janshi Shangri-la (JPN, USA, EXP, KOR, AUS)", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0005 */ GAME( 1999, spawn,    naomi,    naomim2, naomi,    naomi,    ROT0, "Capcom",          "Spawn In the Demon's Hand (JPN, USA, EUR, ASI, AUS) (Rev B)", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0006 */ GAME( 1999, puyoda,   naomi,    naomim2, naomi,    naomi,    ROT0, "Compile",         "Puyo Puyo Da!", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0007-02 */ GAME( 2000,mvsc2,  naomi,    naomim1, naomi,    mvsc2,    ROT0, "Capcom",          "Marvel Vs. Capcom 2 New Age of Heroes (JPN, USA, EUR, ASI, AUS) (Rev A)", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0008 */ GAME( 2000, pstone2,  naomi,    naomim2, naomi,    naomi,    ROT0, "Capcom",          "Power Stone 2 (JPN, USA, EUR, ASI, AUS)", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0011 */ GAME( 2000, capsnk,   naomi,    naomim2, naomi,    naomi,    ROT0, "Capcom / SNK",    "Capcom Vs. SNK Millennium Fight 2000 (JPN, USA, EXP, KOR, AUS) (Rev C)", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0011 */ GAME( 2000, capsnka,  capsnk,   naomim2, naomi,    naomi,    ROT0, "Capcom / SNK",    "Capcom Vs. SNK Millennium Fight 2000 (JPN, USA, EXP, KOR, AUS) (Rev A)", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0012 */ GAME( 2000, cspike,   naomi,    naomim2, naomi,    naomi,    ROT0, "Psikyo / Capcom", "Gun Spike (JPN) / Cannon Spike (USA, EXP, KOR, AUS)", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0013 */ GAME( 2000, ggx,      naomi,    naomim2, naomi,    naomi,    ROT0, "Arc System Works","Guilty Gear X (JPN)", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0014 */ GAME( 2000, gwing2,   naomi,    naomim2, naomi,    naomi,    ROT0, "Takumi / Capcom", "Giga Wing 2 (JPN, USA, EXP, KOR, AUS)", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0015 */ GAME( 2000, pjustic,  naomi,    naomim2, naomi,    naomi,    ROT0, "Capcom",          "Moero Justice Gakuen (JPN) / Project Justice (USA, EXP, KOR, AUS) (Rev A)", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0016 */ GAME( 2000, deathcox, naomi,    naomim2, naomi,    naomi,    ROT0, "Ecole Software",  "Death Crimson OX (JPN, USA, EXP, KOR, AUS)", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0017 */ GAME( 2001, gundmct,  naomi,    naomi,   naomi,    naomi,    ROT0, "Banpresto",       "Mobile Suit Gundam: Federation Vs. Zeon (2001-02-08)", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0020 */ GAME( 2001, zerogu2,  naomi,    naomim2, naomi,    naomi,    ROT0, "Psikyo",          "Zero Gunner 2", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 0057 */ GAME( 2007, sl2007,   naomi,    naomim4, naomi,    naomi,    ROT270, "Triangle Service", "Shooting Love 2007", GAME_FLAGS )
/* 0058 */ GAME( 2008, ausfache, naomi,    naomim4, naomi,    naomi,    ROT0, "Subtle Style",    "Akatsuki Blitzkampf Ausf Achse", GAME_FLAGS )
/* 0059 */ GAME( 2008, illvelo,  naomi,    naomim4, naomi,    naomi,    ROT270, "Milestone",     "Illvelo (Illmatic Envelope)", GAME_FLAGS )
// 0060 Mamoru-kun wa Norowarete Shimatta!
/* 0061 */ GAME( 2008, mbaa,     naomi,    naomim4, naomi,    naomi,    ROT0, "Type-Moon/Ecole","Melty Blood Actress Again", GAME_FLAGS )
// 0061 Melty Blood Actress Again (Rev A - original is rev 0)
// 0062 Radirgy Noa
/* HMG016007 */ GAME( 2001, hmgeo, naomi,   naomim2, naomi,    naomi,    ROT0, "Capcom",          "Heavy Metal Geomatrix (JPN, USA, EUR, ASI, AUS) (Rev A)", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )

/* Cart games on Namco custom ROM board */
/* 25209801 */ GAME( 2000, wldkicks, naomi, naomim2, naomi,    naomi,    ROT0, "Capcom / Namco", "World Kicks", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
/* 25349801 */ GAME( 2000, toukon4,  naomi, naomim2, naomi,    naomi,    ROT0, "Capcom / Namco", "Shin Nihon Pro Wrestling Toukon Retsuden 4 Arcade Edition", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
// 25469801 Ninja Assault
/* 25709801 */ GAME( 2001, gunsur2,  naomi, naomi,   naomi,    naomi,    ROT0, "Capcom / Namco", "Gun Survivor 2 Biohazard Code: Veronica", GAME_UNEMULATED_PROTECTION|GAME_FLAGS )
// 25869812 Mazan : Flash of the Blade

/* GDS-xxxx (Sega GD-ROM games) */
/* 0001  */ GAME( 2000, confmiss,  naomigd,  naomigd, hotd2,    naomigd,    ROT0, "Sega", "Confidential Mission (GDS-0001)", GAME_FLAGS )
// 0002  Shakatto Tambourine
/* 0003  */ GAME( 2000, sprtjam,   naomigd,  naomigd, naomi,    naomigd,    ROT0, "Sega", "Sports Jam (GDS-0003)", GAME_FLAGS )
/* 0004  */ GAME( 2000, slashout,  naomigd,  naomigd, naomi,    naomigd,    ROT0, "Sega", "Slashout (GDS-0004)", GAME_FLAGS )
/* 0005  */ GAME( 2001, spkrbtl,   naomigd,  naomigd, naomi,    naomigd,    ROT0, "Sega", "Spikers Battle (GDS-0005)", GAME_FLAGS )
/* 0006  */ GAME( 2001, vstrik3,   naomi2,   naomi2gd, naomi,   naomi2,     ROT0, "Sega", "Virtua Striker 3 (GDS-0006)", GAME_FLAGS )
// 0007  Shakatto Tambourine Motto Norinori Shinkyoku Tsuika
/* 0008  */ GAME( 2001, monkeyba,  naomigd,  naomigd, monkeyba, naomigd,    ROT0, "Sega", "Monkey Ball (GDS-0008)", GAME_FLAGS )
// 0009  Dynamic Golf / Virtua Golf
/* 0009A */ GAME( 2001, dygolf,    naomigd,  naomigd, naomi,    naomigd,    ROT0, "Sega", "Dynamic Golf / Virtua Golf (Rev A) (GDS-0009A)", GAME_FLAGS )
/* 0010  */ GAME( 2001, wsbbgd,    naomigd,  naomigd, dybbnao,  naomigd,    ROT0, "Sega", "Super Major League / World Series Baseball (GDS-0010)", GAME_FLAGS )
/* 0011  */ GAME( 1999, vtennisg,  naomigd,  naomigd, naomi,    naomigd,    ROT0, "Sega", "Virtua Tennis / Power Smash (GDS-0011)", GAME_FLAGS )
/* 0012  */ GAME( 2001, vf4,       naomi2,   naomi2gd, naomi,   naomi2,   ROT0, "Sega", "Virtua Fighter 4 (GDS-0012)", GAME_FLAGS )
// 0012A Virtua Fighter 4 (Rev A)
/* 0012B */ GAME( 2001, vf4b,      vf4,      naomi2gd, naomi,   naomi2,   ROT0, "Sega", "Virtua Fighter 4 (Rev B) (GDS-0012B)", GAME_FLAGS )
/* 0012C */ GAME( 2001, vf4c,      vf4,      naomi2gd, naomi,   naomi2,   ROT0, "Sega", "Virtua Fighter 4 (Rev C) (GDS-0012C)", GAME_FLAGS )
// 0013  Shakatto Tambourine 2K1 SPR (PIC: 253-5508-0315)
/* 0014  */ GAME( 2001, beachspi,  naomi2,   naomi2gd, naomi,   naomi2,   ROT0, "Sega", "Beach Spikers (GDS-0014)", GAME_FLAGS )
// 0015  Virtua Tennis 2 / Power Smash 2
/* 0015A */ GAME( 2001, vtennis2,  naomigd,  naomigd, naomi,    naomigd,    ROT0, "Sega", "Virtua Tennis 2 / Power Smash 2 (Rev A) (GDS-0015A)", GAME_FLAGS )
/* 0016  */ GAME( 2001, shaktamb,  naomigd,  naomigd, shaktamb, naomigd,    ROT0, "Sega", "Shakatto Tambourine Cho Powerup Chu (GDS-0016)", GAME_FLAGS )
/* 0017  */ GAME( 2001, keyboard,  naomigd,  naomigd, naomi,    naomigd,    ROT0, "Sega", "La Keyboard (GDS-0017)", GAME_FLAGS )
/* 0018  */ GAME( 2001, lupinsho,  naomigd,  naomigd, hotd2,    naomigd,    ROT0, "Sega", "Lupin The Third - The Shooting (GDS-0018)", GAME_FLAGS )
// 0018A Lupin The Third - The Shooting (Rev A)
/* 0019  */ GAME( 2002, vathlete,  naomigd,  naomigd, naomi,    naomigd,    ROT0, "Sega", "Virtua Athletics / Virtua Athlete (GDS-0019)", GAME_FLAGS )
// 0020  Initial D Arcade Stage
// 0020A Initial D Arcade Stage (Rev A)
/* 0020B */ GAME( 2002, initd,     naomi2,   naomi2gd, naomi,   naomi2,   ROT0, "Sega", "Initial D Arcade Stage (Rev B) (Japan) (GDS-0020B)", GAME_FLAGS )
// 0021  Lupin The Third - The Typing
/* 0021A */ GAME( 2002, luptype,   naomigd,  naomigd, naomi,    naomigd,    ROT0, "Sega", "Lupin The Third - The Typing (Rev A) (GDS-0021A)", GAME_FLAGS )
/* 0022  */ GAME( 2002, mok,       naomigd,  naomigd, hotd2,    naomigd,    ROT0, "Sega", "The Maze of the Kings (GDS-0022)", GAME_FLAGS )
// 0023  Naomi DIMM Firmware Updater
/* 0023A */ GAME( 2001, ngdup23a,  naomigd,  naomigd, naomi,    naomigd,    ROT0, "Sega", "Naomi DIMM Firmware Updater (Rev A) (GDS-0023A)", GAME_FLAGS )
// 0023B Naomi DIMM Firmware Updater (Rev B)
/* 0023C */ GAME( 2001, ngdup23c,  naomigd,  naomigd, naomi,    naomigd,    ROT0, "Sega", "Naomi DIMM Firmware Updater (Rev C) (GDS-0023C)", GAME_FLAGS )
// 0023D Naomi DIMM Firmware Updater (Rev D)
// 0023E Naomi DIMM Firmware Updater (Rev E)
// 0024  Virtua Fighter 4 Evolution
/* 0024A */ GAME( 2002, vf4evoa,   vf4evo,   naomi2gd,naomi,    naomi2,   ROT0, "Sega", "Virtua Fighter 4 Evolution (Rev A) (GDS-0024A)", GAME_FLAGS )
/* 0024B */ GAME( 2002, vf4evo,    naomi2,   naomi2gd,naomi,    naomi2,   ROT0, "Sega", "Virtua Fighter 4 Evolution (Rev B) (GDS-0024B)", GAME_FLAGS )
/* 0025  */ GAME( 2002, initdexp,  naomi2,   naomi2gd,naomi,    naomi2,   ROT0, "Sega", "Initial D Arcade Stage (Export) (GDS-0025)", GAME_FLAGS )
/* 0026  */ GAME( 2002, initdv2jo, initdv2j, naomi2gd,naomi,    naomi2,   ROT0, "Sega", "Initial D Arcade Stage Ver. 2 (Japan) (GDS-0026)", GAME_FLAGS )
// 0026A Initial D Arcade Stage Ver. 2 (Japan) (Rev A)
/* 0026B */ GAME( 2002, initdv2j,  naomi2,   naomi2gd,naomi,    naomi2,   ROT0, "Sega", "Initial D Arcade Stage Ver. 2 (Japan) (Rev. B) (GDS-0026B)", GAME_FLAGS )
/* 0027  */ GAME( 2002, initdv2e,  initdv2j, naomi2gd,naomi,    naomi2,   ROT0, "Sega", "Initial D Arcade Stage Ver. 2 (Export) (GDS-0027)", GAME_FLAGS )
// 0028
// 0029  Club Kart Cycraft Edition
// 0030
/* 0031  */ GAME( 2003, puyofev,   naomigd,  naomigd, naomi,    naomigd,    ROT0, "Sega", "Puyo Puyo Fever (GDS-0031)", GAME_FLAGS )
// 0032  Initial D Arcade Stage Ver. 3 (Japan)
// 0032A Initial D Arcade Stage Ver. 3 (Japan) (Rev A)
/* 0032B */ GAME( 2004, initdv3jb, initdv3j, naomigd, naomi,    naomi2,   ROT0, "Sega", "Initial D Arcade Stage Ver. 3 (Japan) (Rev. B) (GDS-0032B)", GAME_FLAGS )
/* 0032C */ GAME( 2004, initdv3j,  naomi2,   naomi2gd,naomi,    naomi2,   ROT0, "Sega", "Initial D Arcade Stage Ver. 3 (Japan) (Rev. C) (GDS-0032C)", GAME_FLAGS )
// 0033  Initial D Arcade Stage Ver. 3 (Export)
// 0034
// 0035
// 0036  Virtua Fighter 4 Final Tuned
/* 0036A */ GAME( 2004, vf4tuneda, vf4tuned, naomi2gd,naomi,    naomi2,   ROT0, "Sega", "Virtua Fighter 4 Final Tuned (Rev A) (GDS-0036A)", GAME_FLAGS )
/* 0036B */
/* 0036C */
/* 0036D */ GAME( 2004, vf4tunedd, vf4tuned, naomi2gd,naomi,    naomi2,   ROT0, "Sega", "Virtua Fighter 4 Final Tuned (Rev D) (GDS-0036D)", GAME_FLAGS )
/* 0036E */
/* 0036F */ GAME( 2004, vf4tuned,  naomi2,   naomi2gd,naomi,    naomi2,   ROT0, "Sega", "Virtua Fighter 4 Final Tuned (Rev F) (GDS-0036F)", GAME_FLAGS )
// 0037? Puyo Puyo Fever (Export)
// 0038
// 0039  Initial D: Arcade Stage Ver. 3 Cycraft Edition
// 0039A Initial D: Arcade Stage Ver. 3 Cycraft Edition (Rev A)
// 0039B Initial D: Arcade Stage Ver. 3 Cycraft Edition (Rev B)
// 0040
// 0041  Dragon Treasure 3
// 0041A Dragon Treasure 3 (Rev A)
// 0042  NAOMI DIMM Firm Update for CF-BOX
// 0042A NAOMI DIMM Firm Update for CF-BOX (Rev A)
// 00??  Dragon Treasure
// 00??  Dragon Treasure 2
// 00??  Get Bass 2
// 00??  Kodai Ouja Kyouryuu King
// 00??  World Club Champion Football Serie A 2001-2002

/* GDL-xxxx ("licensed by Sega" GD-ROM games) */
/* 0001  */ GAME( 2001, gundmgd,   naomigd,  naomigd, naomi,    naomigd,  ROT0,   "Capcom / Banpresto", "Mobile Suit Gundam: Federation Vs. Zeon (GDL-0001)", GAME_FLAGS )
/* 0002  */ GAME( 2001, sfz3ugd,   naomigd,  naomigd, naomi,    sfz3ugd,  ROT0,   "Capcom",       "Street Fighter Zero 3 Upper (GDL-0002)", GAME_FLAGS )
// 0003
/* 0004  */ GAME( 2001, cvsgd,     naomigd,  naomigd, naomi,    naomigd,  ROT0,   "Capcom / SNK", "Capcom Vs. SNK Millennium Fight 2000 Pro (GDL-0004)", GAME_FLAGS )
/* 0005  */ GAME( 2001, starseek,  naomigd,  naomigd, naomi,    naomigd,  ROT0,   "G-Rev",        "Doki Doki Idol Star Seeker (GDL-0005)", GAME_IMPERFECT_GRAPHICS|GAME_IMPERFECT_SOUND )
/* 0006  */ GAME( 2001, gundmxgd,  naomigd,  naomigd, naomi,    naomigd,  ROT0,   "Capcom",       "Mobile Suit Gundam: Federation Vs. Zeon DX  (GDL-0006)", GAME_FLAGS )
// 0007  Capcom Vs. SNK 2
/* 0007A */ GAME( 2001, cvs2gd,    naomigd,  naomigd, naomi,    naomigd,  ROT0,   "Capcom / SNK", "Capcom Vs. SNK 2 Millionaire Fighting 2001 (Rev A) (GDL-0007A)", GAME_FLAGS )
// 0008  Capcom Vs. SNK 2 Mark Of The Millennium 2001 (Export)
// 0009
/* 0010  */ GAME( 2001, ikaruga,   naomigd,  naomigd, naomi,    naomigd,  ROT270, "Treasure",     "Ikaruga (GDL-0010)", GAME_FLAGS )
/* 0011  */ GAME( 2002, ggxx,      naomigd,  naomigd, naomi,    ggxx,     ROT0,"Arc System Works","Guilty Gear XX (GDL-0011)", GAME_FLAGS )
/* 0012  */ GAME( 2002, cleoftp,   naomigd,  naomigd, naomi,    naomigd,  ROT0,   "Altron",       "Cleopatra Fortune Plus (GDL-0012)", GAME_FLAGS )
/* 0013  */ GAME( 2002, moeru,     naomigd,  naomigd, naomi,    naomigd,  ROT0,   "Altron",       "Moeru Casinyo (GDL-0013)", GAME_FLAGS )
// 0014  Musapey's Choco Marker
/* 0014A */ GAME( 2002, chocomk,   naomigd,  naomigd, naomi,    naomigd,  ROT0, "Ecole Software", "Musapey's Choco Marker (Rev A) (GDL-0014A)", GAME_FLAGS )
// 0015
// 0016  Yonin Uchi Mahjong MJ
/* 0017  */ GAME( 2002, quizqgd,   naomigd,  naomigd, naomi,    naomigd,  ROT270, "Amedio (Taito license)", "Quiz Keitai Q mode (GDL-0017)", GAME_FLAGS )
/* 0018  */ GAME( 2002, azumanga,  naomigd,  naomigd, naomi,    naomigd,  ROT0,   "Moss (Taito license)", "Azumanga Daioh Puzzle Bobble (GDL-0018)", GAME_FLAGS )
// 0019  Guilty Gear XX #Reload
/* 0019A */ GAME( 2003, ggxxrl,    naomigd,  naomigd, naomi,    ggxxrl,   ROT0,"Arc System Works","Guilty Gear XX #Reload (Rev A) (GDL-0019A)", GAME_FLAGS )
/* 0020  */ GAME( 2004, tetkiwam,  naomigd,  naomigd, naomi,    naomigd,  ROT0,   "Success",      "Tetris Kiwamemichi (GDL-0020)", GAME_FLAGS )
/* 0021  */ GAME( 2003, shikgam2,  naomigd,  naomigd, naomi,    naomigd,  ROT270, "Alfa System",  "Shikigami No Shiro II / The Castle of Shikigami II (GDL-0021)", GAME_FLAGS )
/* 0022  */ GAME( 2003, usagiym,   naomigd,  naomigd, naomi_mp, naomigd_mp,ROT0,   "Warashi / Mahjong Kobo / Taito", "Usagi - Yamashiro Mahjong Hen (GDL-0022)", GAME_FLAGS )
// 0023  Border Down
/* 0023A */ GAME( 2003, bdrdown,   naomigd,  naomigd, naomi,    naomigd,  ROT0,   "G-Rev",        "Border Down (Rev A) (GDL-0023A)", GAME_FLAGS )
/* 0024  */ GAME( 2003, psyvar2,   naomigd,  naomigd, naomi,    naomigd,  ROT270, "Success",      "Psyvariar 2 - The Will To Fabricate (GDL-0024)", GAME_FLAGS )
/* 0025  */ GAME( 2004, cfield,    naomigd,  naomigd, naomi,    naomigd,  ROT0,   "Able",         "Chaos Field (GDL-0025)", GAME_FLAGS )
/* 0026  */ GAME( 2004, trizeal,   naomigd,  naomigd, naomi,    naomigd,  ROT270, "Taito",        "Trizeal (GDL-0026)", GAME_FLAGS )
// 0027  Melty Blood Act Cadenza?
// 0028  Melty Blood Act Cadenza
// 0028A Melty Blood Act Cadenza (Rev A)
// 0028B Melty Blood Act Cadenza (Rev B)
/* 0028C */ GAME( 2005, meltybld,  naomigd,  naomigd, naomi,    naomigd,  ROT0, "Ecole Software", "Melty Blood Act Cadenza (Rev C) (GDL-0028C)", GAME_FLAGS )
// 0029
/* 0030  */ GAME( 2005, senkoo,    senko,    naomigd, naomi,    naomigd,  ROT0,   "G-Rev",        "Senko No Ronde (GDL-0030)", GAME_FLAGS )
/* 0030A */ GAME( 2005, senko,     naomigd,  naomigd, naomi,    naomigd,  ROT0,   "G-Rev",        "Senko No Ronde (Rev A) (GDL-0030A)", GAME_FLAGS )
/* 0031  */ GAME( 2005, ss2005,    naomigd,  naomigd, naomi,    naomigd,  ROT0,   "Starfish",     "Super Shanghai 2005 (GDL-0031)", GAME_FLAGS )
/* 0031A */ GAME( 2005, ss2005a,   ss2005,   naomigd, naomi,    naomigd,  ROT0,   "Starfish",     "Super Shanghai 2005 (Rev A) (GDL-0031A)", GAME_FLAGS )
/* 0032  */ GAME( 2005, radirgy,   naomigd,  naomigd, naomi,    naomigd,  ROT270, "Milestone",    "Radirgy (GDL-0032)", GAME_FLAGS )
// 0032A Radirgy (Rev A)
// 0033  Guilty Gear XX Slash
/* 0033A */ GAME( 2005, ggxxsla,   naomigd,  naomigd, naomi,    ggxxsla,  ROT0,"Arc System Works","Guilty Gear XX Slash (Rev A) (GDL-0033A)", GAME_FLAGS )
/* 0034  */ GAME( 2006, kurucham,  naomigd,  naomigd, naomi,    naomigd,  ROT0,   "Able",         "Kurukuru Chameleon (GDL-0034)", GAME_FLAGS )
/* 0035  */ GAME( 2005, undefeat,  naomigd,  naomigd, naomi,    naomigd,  ROT270, "G-Rev",        "Under Defeat (GDL-0035)", GAME_FLAGS )
// 0036  Trigger Heart Exelica
/* 0036A */ GAME( 2006, trgheart,  naomigd,  naomigd, naomi,    naomigd,  ROT270, "Warashi",      "Trigger Heart Exelica (Rev A) (GDL-0036A)", GAME_FLAGS )
/* 0037  */ GAME( 2006, jingystm,  naomigd,  naomigd, naomi,    naomigd,  ROT0, "Atrativa Japan", "Jingi Storm - The Arcade (GDL-0037)", GAME_FLAGS )
/* 0038  */ GAME( 2006, senkosp,   naomigd,  naomigd, naomi,    naomigd,  ROT0,   "G-Rev",        "Senko No Ronde Special (GDL-0038)", GAME_FLAGS )
/* 0039  */ GAME( 2006, meltyb,    naomigd,  naomigd, naomi,    naomigd,  ROT0, "Ecole Software", "Melty Blood Act Cadenza Ver B (GDL-0039)", GAME_FLAGS )
/* 0039A */ GAME( 2006, meltyba,   meltyb,   naomigd, naomi,    naomigd,  ROT0, "Ecole Software", "Melty Blood Act Cadenza Ver B (Rev A) (GDL-0039A)", GAME_FLAGS )
/* 0040  */ GAME( 2006, karous,    naomigd,  naomigd, naomi,    naomigd,  ROT270, "Milestone",    "Karous (GDL-0040)", GAME_FLAGS )
/* 0041  */ GAME( 2006, ggxxac,    naomigd,  naomigd, naomi,    naomigd,  ROT0,"Arc System Works","Guilty Gear XX Accent Core (GDL-0041)", GAME_FLAGS )
/* 0042  */ GAME( 2006, takoron,   naomigd,  naomigd, naomi,    naomigd,  ROT0,   "Compile",      "Noukone Puzzle Takoron (GDL-0042)", GAME_FLAGS )

/* Atomiswave */
GAME( 2001, awbios,   0,        aw2c,    aw2c,  atomiswave, ROT0, "Sammy",                           "Atomiswave Bios", GAME_FLAGS|GAME_IS_BIOS_ROOT )

GAME( 2002, maxspeed, awbios,   aw1c,    aw1w,  atomiswave, ROT0, "Sammy",                           "Maximum Speed", GAME_FLAGS )
GAME( 2002, sprtshot, awbios,   aw2c,    aw2c,  atomiswave, ROT0, "Sammy USA",                       "Sports Shooting USA", GAME_FLAGS )
GAME( 2003, ggx15,    awbios,   aw2c,    aw2c,  atomiswave, ROT0, "Sammy / Arc System Works",        "Guilty Gear X ver. 1.5", GAME_IMPERFECT_GRAPHICS|GAME_IMPERFECT_SOUND|GAME_NOT_WORKING )
GAME( 2003, demofist, awbios,   aw2c,    aw2c,  atomiswave, ROT0, "Polygon Magic / Dimps",           "Demolish Fist", GAME_FLAGS )
GAME( 2003, dolphin,  awbios,   aw2c,    aw2c,  atomiswave, ROT0, "Sammy",                           "Dolphin Blue", GAME_IMPERFECT_GRAPHICS|GAME_IMPERFECT_SOUND|GAME_NOT_WORKING )
GAME( 2003, ggisuka,  awbios,   aw2c,    aw2c,  atomiswave, ROT0, "Sammy / Arc System Works",        "Guilty Gear Isuka", GAME_IMPERFECT_GRAPHICS|GAME_IMPERFECT_SOUND|GAME_NOT_WORKING )
GAME( 2004, dirtypig, awbios,   aw2c,    aw2c,  atomiswave, ROT0, "Sammy",                           "Dirty Pigskin Football", GAME_IMPERFECT_GRAPHICS|GAME_IMPERFECT_SOUND|GAME_NOT_WORKING )
GAME( 2004, rumblef,  awbios,   aw2c,    aw2c,  atomiswave, ROT0, "Sammy / Dimps",                   "The Rumble Fish", GAME_IMPERFECT_GRAPHICS|GAME_IMPERFECT_SOUND|GAME_NOT_WORKING )
GAME( 2004, rangrmsn, awbios,   aw2c,    aw1w,  atomiswave, ROT0, "Sammy",                           "Ranger Mission", GAME_FLAGS )
GAME( 2004, salmankt, awbios,   aw2c,    aw2c,  atomiswave, ROT0, "Sammy",                           "Salary Man Kintarou", GAME_FLAGS )
GAME( 2004, ftspeed,  awbios,   aw1c,    aw1w,  atomiswave, ROT0, "Sammy",                           "Faster Than Speed", GAME_FLAGS )
GAME( 2004, kov7sprt, awbios,   aw2c,    aw2c,  atomiswave, ROT0, "Sammy / IGS",                     "Knights of Valour - The Seven Spirits", GAME_IMPERFECT_GRAPHICS|GAME_IMPERFECT_SOUND|GAME_NOT_WORKING )
GAME( 2005, vfurlong, awbios,   aw2c,    aw2c,  atomiswave, ROT0, "Sammy",                           "Net Select Keiba Victory Furlong", GAME_FLAGS )
GAME( 2004, rumblef2, awbios,   aw2c,    aw2c,  atomiswave, ROT0, "Sammy / Dimps",                   "The Rumble Fish 2", GAME_FLAGS )
GAME( 2005, anmlbskt, awbios,   aw2c,    aw2c,  atomiswave, ROT0, "Sammy / Moss",                    "Animal Basket", GAME_FLAGS )
GAME( 2005, ngbc,     awbios,   aw2c,    aw2c,  atomiswave, ROT0, "Sammy / SNK Playmore",            "Neo-Geo Battle Coliseum", GAME_FLAGS )
GAME( 2005, samsptk,  awbios,   aw2c,    aw2c,  atomiswave, ROT0, "Sammy / SNK Playmore",            "Samurai Spirits Tenkaichi Kenkakuden", GAME_FLAGS )
GAME( 2005, kofxi,    awbios,   aw2c,    aw2c,  atomiswave, ROT0, "Sammy / SNK Playmore",            "The King of Fighters XI", GAME_FLAGS )
GAME( 2005, fotns,    awbios,   aw2c,    aw2c,  atomiswave, ROT0, "Sega / Arc System Works",         "Fist Of The North Star", GAME_FLAGS )
GAME( 2005, kofnw,    awbios,   aw2c,    aw2c,  atomiswave, ROT0, "Sammy / SNK Playmore",            "The King of Fighters Neowave", GAME_FLAGS )
GAME( 2005, kofnwj,   kofnw,    aw2c,    aw2c,  atomiswave, ROT0, "Sammy / SNK Playmore",            "The King of Fighters Neowave (Japan)", GAME_FLAGS )
GAME( 2005, xtrmhunt, awbios,   aw2c,    aw2c,  atomiswave, ROT0, "Sammy",                           "Extreme Hunting", GAME_FLAGS )
GAME( 2006, mslug6,   awbios,   aw2c,    aw2c,  atomiswave, ROT0, "Sega / SNK Playmore",             "Metal Slug 6", GAME_IMPERFECT_GRAPHICS|GAME_IMPERFECT_SOUND|GAME_NOT_WORKING )
GAME( 2006, xtrmhnt2, awbios,   aw2c,    aw2c,  atomiswave, ROT0, "Sega",                            "Extreme Hunting 2", GAME_FLAGS )
