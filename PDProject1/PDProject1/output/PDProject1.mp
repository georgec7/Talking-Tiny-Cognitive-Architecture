Project:      output/PDProject1
Build Number: 0
Date:         Thu Oct 13 17:12:36 2022
Compiler:     ICCM8C
Version:      7.05.00


Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                             lit   0190   00D0 =    208. bytes (rel,con,rom,lit)

       Addr  Global Symbol
      -----  --------------------------------
       0190  LoadConfigTBL_pdproject1_Bank0
       01DF  LoadConfigTBL_pdproject1_Bank1
       0260  __lit_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                        func_lit   0260   0002 =      2. bytes (rel,con,rom,proclab)

       Addr  Global Symbol
      -----  --------------------------------
       0260  __func_lit_start
       0262  __func_lit_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                     psoc_config   0262   0127 =    295. bytes (rel,con,rom,code)

       Addr  Global Symbol
      -----  --------------------------------
       0262  LoadConfigTBL_pdproject1_Ordered
       0327  _LoadConfigInit
       0327  LoadConfigInit
       032E  _LoadConfig_pdproject1
       032E  LoadConfig_pdproject1
       0389  __psoc_config_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                     UserModules   0389   03B6 =    950. bytes (rel,con,rom,code)

       Addr  Global Symbol
      -----  --------------------------------
       0389  _I2CHW_Temp_fReadBytes
       03CD  _I2CHW_Temp_bWriteBytes
       0411  _I2CHW_Temp_bWriteCBytes
       047D  I2CHW_Temp_fSendRepeatStart
       047D  _I2CHW_Temp_fSendRepeatStart
       04B9  _I2CHW_Temp_fSendStart
       04B9  I2CHW_Temp_fSendStart
       04FD  I2CHW_Temp_fWrite
       04FD  _I2CHW_Temp_fWrite
       053D  _I2CHW_Temp_bRead
       053D  I2CHW_Temp_bRead
       0573  _I2CHW_Temp_SendStop
       0573  I2CHW_Temp_SendStop
       057C  I2CHW_Temp_bReadBusStatus
       057C  _I2CHW_Temp_bReadBusStatus
       0582  _I2CHW_Temp_ISR
       06BF  _I2CHW_Temp_InitWrite
       06BF  I2CHW_Temp_InitWrite
       06FD  _I2CHW_Temp_InitRamRead
       06FD  I2CHW_Temp_InitRamRead
       072A  I2CHW_Temp_InitFlashRead
       072A  _I2CHW_Temp_InitFlashRead
       072B  _I2CHW_Temp_bReadI2CStatus
       072B  I2CHW_Temp_bReadI2CStatus
       0731  I2CHW_Temp_ClrRdStatus
       0731  _I2CHW_Temp_ClrRdStatus
       0738  _I2CHW_Temp_ClrWrStatus
       0738  I2CHW_Temp_ClrWrStatus
       073F  __UserModules_end

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                            text   073F   0198 =    408. bytes (rel,con,rom,code)

       Addr  Global Symbol
      -----  --------------------------------
       073F  __text_start
       073F  _main
       0747  _read_temp_and_print
       08D7  __xidata_start
       08D7  __text_end
       08D9  __xidata_end

Area                               Start  End    Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                             TOP   0000   0151 =    337. bytes (abs,ovr,rom,code)

       Addr  Global Symbol
      -----  --------------------------------
       0080  __Start
       014E  _bGetPowerSetting
       014E  bGetPowerSetting

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
                    InterruptRAM   0000   0009 =      9. bytes (rel,con,ram)

       Addr  Global Symbol
      -----  --------------------------------
       0000  _I2CHW_Temp_bStatus
       0000  _NO_SHADOW
       0000  NO_SHADOW
       0000  I2CHW_Temp_bStatus
       0001  _I2CHW_Temp_SlaveAddr
       0001  I2CHW_Temp_SlaveAddr
       0002  I2CHW_Temp_RsrcStatus
       0002  _I2CHW_Temp_RsrcStatus
       0003  _I2CHW_Temp_Write_Count
       0003  I2CHW_Temp_Write_Count
       0004  pI2CHW_Temp_Write_BufHI
       0004  _pI2CHW_Temp_Write_BufHI
       0005  pI2CHW_Temp_Write_BufLO
       0005  _pI2CHW_Temp_Write_Buf
       0005  _pI2CHW_Temp_Write_BufLO
       0005  pI2CHW_Temp_Write_Buf
       0006  _pI2CHW_Temp_Read_BufHI
       0006  pI2CHW_Temp_Read_BufHI
       0007  _pI2CHW_Temp_Read_BufLO
       0007  pI2CHW_Temp_Read_Buf
       0007  pI2CHW_Temp_Read_BufLO
       0007  _pI2CHW_Temp_Read_Buf
       0008  _I2CHW_Temp_Read_Count
       0008  I2CHW_Temp_Read_Count

Area                               Addr   Size   Decimal Bytes (Attributes)
--------------------------------   ----   ----   ------- ----- ------------
               virtual_registers   0009   0007 =      7. bytes (rel,con,ram)

       Addr  Global Symbol
      -----  --------------------------------
       0009  __rZ
       000A  __rY
       000B  __rX
       000C  __r3
       000D  __r2
       000E  __r1
       000F  __r0

Files Linked      [ module(s) ]

./obj/boot.o	[ boot.asm ]
obj/main.o	[ main.c ]
<library>	[ psocconfigtbl.asm, psocconfig.asm, i2chw_tempmstr.asm, i2chw_tempint.asm, i2chw_tempcommon.asm ]
[ rZ.asm, rY.asm, rX.asm, r3.asm, r2.asm, r1.asm, r0.asm, divmod_16x16_16_util.asm, i_div16_block_util.asm ]


Area Definitions

-blit:0x190.0x3fff
-bdata:0.0x2ff
-BInterruptRAM:0	allocated at: 0
-Bvirtual_registers	allocated at: 0x9
-Bpdproject1_RAM
-rwh:SSCParmBlk:0xf7.0xff

