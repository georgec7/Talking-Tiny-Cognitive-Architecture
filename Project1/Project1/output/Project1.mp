  ROM 2% full. 798 bytes used (does not include absolute areas).
  RAM 0% full. 13 bytes used (does not include stack usage).

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                             lit   01A0   01DD =    477. bytes (rel,con,rom)

       Addr  Global Symbol
      -----  --------------------------------
       01A0  LoadConfigTBL_project1_Ordered
       0289  LoadConfigTBL_project1_Bank0
       0340  LoadConfigTBL_project1_Bank1
       037D  __lit_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                        func_lit   037D   0002 =      2. bytes (rel,con,rom)

       Addr  Global Symbol
      -----  --------------------------------
       037D  __func_lit_start
       037F  __func_lit_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                     psoc_config   037F   006B =    107. bytes (rel,con,rom)

       Addr  Global Symbol
      -----  --------------------------------
       037F  LoadConfigInit
       037F  _LoadConfigInit
       038F  _LoadConfig_project1
       038F  LoadConfig_project1
       03EA  __psoc_config_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                     UserModules   03EA   00D2 =    210. bytes (rel,con,rom)

       Addr  Global Symbol
      -----  --------------------------------
       03F8  _DUALADC_1_CNT1_ISR
       03FB  _DUALADC_1_CNT2_ISR
       03FE  _DUALADC_1_PWM16_ISR
       04BC  __UserModules_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                            text   04BC   0002 =      2. bytes (rel,con,rom)

       Addr  Global Symbol
      -----  --------------------------------
       04BC  _main
       04BC  __text_start
       04BE  __text_end
       04C0  __xidata_end

Area                               Start  End    Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                             TOP   0068   0140 =    320. bytes (abs,ovr,rom)

       Addr  Global Symbol
      -----  --------------------------------
       0068  __Start
       013D  bGetPowerSetting
       013D  _bGetPowerSetting

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                    InterruptRAM   0000   000B =     11. bytes (rel,con,ram)

       Addr  Global Symbol
      -----  --------------------------------
       0000  NO_SHADOW
       0000  _Port_2_Data_SHADE
       0000  _NO_SHADOW
       0000  Port_2_Data_SHADE
       0001  _Port_2_DriveMode_0_SHADE
       0001  Port_2_DriveMode_0_SHADE
       0002  Port_2_DriveMode_1_SHADE
       0002  _Port_2_DriveMode_1_SHADE
       0003  DUALADC_1_cCounter1U
       0004  DUALADC_1_cCounter2U
       0005  _DUALADC_1_iResult1
       0005  DUALADC_1_iResult1
       0007  _DUALADC_1_iResult2
       0007  DUALADC_1_iResult2
       0009  DUALADC_1_bfStatus
       0009  _DUALADC_1_bfStatus
       000A  DUALADC_1_bSampC

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
               virtual_registers   000B   0002 =      2. bytes (rel,con,ram)

       Addr  Global Symbol
      -----  --------------------------------
       000B  __r1
       000C  __r0

Files Linked      [ module(s) ]

./obj/boot.o	[ boot.asm ]
./obj/main.o	[ main.asm ]
<library>	[ psocconfigtbl.asm, psocconfig.asm, dualadc_1int.asm, r1.asm, r0.asm ]

User Base Address Definitions

lit = 0x1a0
data = 0x0
SSCParmBlk:0x00F7.0x00FF
InterruptRAM = 0x0
virtual_registers = 0xb
project1_RAM

