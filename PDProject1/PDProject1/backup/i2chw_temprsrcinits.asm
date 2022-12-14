;;*****************************************************************************
;;*****************************************************************************
;;  FILENAME: I2CHW_Temp.asm
;;  Version: 2.00, Updated on 2015/3/4 at 22:26:25
;;  Generated by PSoC Designer 5.4.3191
;;
;;  DESCRIPTION: I2Cs User Module software implementation file
;;
;;  NOTE: User Module APIs conform to the fastcall16 convention for marshalling
;;        arguments and observe the associated "Registers are volatile" policy.
;;        This means it is the caller's responsibility to preserve any values
;;        in the X and A registers that are still needed after the API functions
;;        returns. For Large Memory Model devices it is also the caller's 
;;        responsibility to perserve any value in the CUR_PP, IDX_PP, MVR_PP and 
;;        MVW_PP registers. Even though some of these registers may not be modified
;;        now, there is no guarantee that will remain the case in future releases.
;;-----------------------------------------------------------------------------
;;  Copyright (c) Cypress Semiconductor 2015. All Rights Reserved.
;;*****************************************************************************
;;*****************************************************************************

include "m8c.inc"
include "memory.inc"
include "I2CHW_TempCommon.inc"
include "PSoCGPIOINT.inc"
include "PSoCAPI.inc"

;-----------------------------------------------
; include instance specific register definitions
;-----------------------------------------------

;-----------------------------------------------
;  Global Symbols
;-----------------------------------------------
;-------------------------------------------------------------------
;  Declare the functions global for both assembler and C compiler.
;
;  Note that there are two names for each API. First name is
;  assembler reference. Name with underscore is name refence for
;  C compiler.  Calling function in C source code does not require
;  the underscore.
;-------------------------------------------------------------------

export    I2CHW_Temp_ResumeInt
export   _I2CHW_Temp_ResumeInt
export    I2CHW_Temp_EnableInt
export   _I2CHW_Temp_EnableInt
export    I2CHW_Temp_ClearInt
export   _I2CHW_Temp_ClearInt
IF (I2CHW_Temp_MUM_SEL & (I2CHW_Temp_SLAVE | I2CHW_Temp_MMS))
export    I2CHW_Temp_EnableSlave
export   _I2CHW_Temp_EnableSlave
ENDIF
IF (I2CHW_Temp_MUM_SEL & (I2CHW_Temp_MSTR | I2CHW_Temp_MMS))
export    I2CHW_Temp_EnableMstr
export   _I2CHW_Temp_EnableMstr
ENDIF
export    I2CHW_Temp_Start
export   _I2CHW_Temp_Start
export    I2CHW_Temp_DisableInt
export   _I2CHW_Temp_DisableInt
IF (I2CHW_Temp_MUM_SEL & (I2CHW_Temp_SLAVE | I2CHW_Temp_MMS))
export    I2CHW_Temp_DisableSlave
export   _I2CHW_Temp_DisableSlave
ENDIF
IF (I2CHW_Temp_MUM_SEL & (I2CHW_Temp_MSTR | I2CHW_Temp_MMS))
export    I2CHW_Temp_DisableMstr
export   _I2CHW_Temp_DisableMstr
ENDIF
export    I2CHW_Temp_Stop
export   _I2CHW_Temp_Stop

IF (I2CHW_Temp_MUM_SEL & (I2CHW_Temp_SLAVE | I2CHW_Temp_MMS))
 IF (I2CHW_Temp_CY8C22x45)
export    I2CHW_Temp_EnableHWAddrCheck
export   _I2CHW_Temp_EnableHWAddrCheck
export    I2CHW_Temp_DisableHWAddrCheck
export   _I2CHW_Temp_DisableHWAddrCheck
 ENDIF
ENDIF

AREA UserModules (ROM, REL)

.SECTION

;-----------------------------------------------------------------------------
;  FUNCTION NAME: I2CHW_Temp_Start
;
;  DESCRIPTION:
;   Initialize the I2CHW_Temp I2C bus interface.
;
;-----------------------------------------------------------------------------
;
;  ARGUMENTS:
;
;  RETURNS: none
;
;  SIDE EFFECTS:
;    The A and X registers may be modified by this or future implementations
;    of this function.  The same is true for all RAM page pointer registers in
;    the Large Memory Model.  When necessary, it is the calling function's
;    responsibility to perserve their values across calls to fastcall16 
;    functions.
;          
 I2CHW_Temp_Start:
_I2CHW_Temp_Start:
    RAM_PROLOGUE RAM_USE_CLASS_1
IF (I2CHW_Temp_MUM_SEL & (I2CHW_Temp_SLAVE | I2CHW_Temp_MMS))
 IF (I2CHW_Temp_CY8C22x45)
   M8C_SetBank1
   mov   reg[I2CHW_Temp_ADDR], I2CHW_Temp_SLAVE_ADDR;I2CHW_Temp_HW_ADDR_EN
   M8C_SetBank0
 ENDIF
ENDIF
    RAM_EPILOGUE RAM_USE_CLASS_1
    ret
.ENDSECTION

.SECTION
;-----------------------------------------------------------------------------
;  FUNCTION NAME: I2CHW_Temp_ResumeInt
;
;  DESCRIPTION:
;     reEnables SDA interrupt allowing start condition detection. 
;     Skips clearing INT_CLR3 by entering the EnableInt at ResumeIntEntry:.
;     Remember to call the global interrupt enable function by using
;     the macro: M8C_EnableGInt.
;-----------------------------------------------------------------------------
;  ARGUMENTS: none
;
;  RETURNS: none
;
;  SIDE EFFECTS:
;    The A and X registers may be modified by this or future implementations
;    of this function.  The same is true for all RAM page pointer registers in
;    the Large Memory Model.  When necessary, it is the calling function's
;    responsibility to perserve their values across calls to fastcall16 
;    functions.
;-----------------------------------------------------------------------------
 I2CHW_Temp_ResumeInt:
_I2CHW_Temp_ResumeInt:
    RAM_PROLOGUE RAM_USE_CLASS_1
    jmp ResumeIntEntry
    
;-----------------------------------------------------------------------------
;  FUNCTION NAME: I2CHW_Temp_EnableInt
;
;  DESCRIPTION:
;     Enables SDA interrupt allowing start condition detection. Remember to call the
;     global interrupt enable function by using the macro: M8C_EnableGInt.
;
;-----------------------------------------------------------------------------
;
;  ARGUMENTS: none
;
;  RETURNS: none
;
;  SIDE EFFECTS:
;    The A and X registers may be modified by this or future implementations
;    of this function.  The same is true for all RAM page pointer registers in
;    the Large Memory Model.  When necessary, it is the calling function's
;    responsibility to perserve their values across calls to fastcall16 
;    functions.
;-----------------------------------------------------------------------------
 I2CHW_Temp_EnableInt:
_I2CHW_Temp_EnableInt:
    RAM_PROLOGUE RAM_USE_CLASS_1
    ;first clear any pending interrupts
    M8C_ClearIntFlag INT_CLR3, I2CHW_Temp_INT_MASK
ResumeIntEntry:
    M8C_EnableIntMask I2CHW_Temp_INT_REG, I2CHW_Temp_INT_MASK
    RAM_EPILOGUE RAM_USE_CLASS_1
    ret

.ENDSECTION

.SECTION
;-----------------------------------------------------------------------------
;  FUNCTION NAME: I2CHW_Temp_ClearInt
;
;  DESCRIPTION:
;     Clears only the I2C interrupt in the INT_CLR3 register.
;
;-----------------------------------------------------------------------------
;
;  ARGUMENTS: none
;
;  RETURNS: none
;
;  SIDE EFFECTS:
;    The A and X registers may be modified by this or future implementations
;    of this function.  The same is true for all RAM page pointer registers in
;    the Large Memory Model.  When necessary, it is the calling function's
;    responsibility to perserve their values across calls to fastcall16 
;    functions.
;          
 I2CHW_Temp_ClearInt:
_I2CHW_Temp_ClearInt:
    RAM_PROLOGUE RAM_USE_CLASS_1
    M8C_ClearIntFlag INT_CLR3, I2CHW_Temp_INT_MASK
    RAM_EPILOGUE RAM_USE_CLASS_1
    ret
    
.ENDSECTION

IF (I2CHW_Temp_MUM_SEL & (I2CHW_Temp_MSTR | I2CHW_Temp_MMS))	
.SECTION
;-----------------------------------------------------------------------------
;  FUNCTION NAME: I2CHW_Temp_EnableMstr
;
;  DESCRIPTION:
;     Enables SDA interrupt allowing start condition detection. Remember to call the
;     global interrupt enable function by using the macro: M8C_EnableGInt.
;
;-----------------------------------------------------------------------------
;
;  ARGUMENTS: none
;
;  RETURNS: none
;
;  SIDE EFFECTS:
;    The A and X registers may be modified by this or future implementations
;    of this function.  The same is true for all RAM page pointer registers in
;    the Large Memory Model.  When necessary, it is the calling function's
;    responsibility to perserve their values across calls to fastcall16 
;    functions.
;          
 I2CHW_Temp_EnableMstr:
_I2CHW_Temp_EnableMstr:
    RAM_PROLOGUE RAM_USE_CLASS_1
	;;CDT 28399
	RAM_SETPAGE_CUR >I2CHW_Temp_bStatus
	and [I2CHW_Temp_bStatus], ~0x80 ;; ~I2CHW_Temp_ISR_ACTIVE
	RAM_SETPAGE_CUR >I2CHW_Temp_RsrcStatus
    and    [I2CHW_Temp_RsrcStatus], ~0x80;;~I2CHW_ISR_ACTIVE        ; Make sure internal control variables weren't corrupted previous to start.
    BitSetI2CHW_Temp_CFG I2C_M_EN                                    ;Enable SDA interupt
    RAM_EPILOGUE RAM_USE_CLASS_1
    ret

.ENDSECTION
ENDIF

IF (I2CHW_Temp_MUM_SEL & (I2CHW_Temp_SLAVE | I2CHW_Temp_MMS))
.SECTION
;-----------------------------------------------------------------------------
;  FUNCTION NAME: I2CHW_Temp_EnableSlave
;
;  DESCRIPTION:
;     Enables SDA interrupt allowing start condition detection. Remember to call the
;     global interrupt enable function by using the macro: M8C_EnableGInt.
;
;-----------------------------------------------------------------------------
;
;  ARGUMENTS: none
;
;  RETURNS: none
;
;  SIDE EFFECTS:
;    The A and X registers may be modified by this or future implementations
;    of this function.  The same is true for all RAM page pointer registers in
;    the Large Memory Model.  When necessary, it is the calling function's
;    responsibility to perserve their values across calls to fastcall16 
;    functions.
;          
 I2CHW_Temp_EnableSlave:
_I2CHW_Temp_EnableSlave:
    RAM_PROLOGUE RAM_USE_CLASS_1
    
    M8C_SetBank1 ;The SDA and SCL pins are setting to Hi-z drive mode
    and reg[I2CHW_TempSDA_DriveMode_0_ADDR],~(I2CHW_TempSDA_MASK|I2CHW_TempSCL_MASK)
    or  reg[I2CHW_TempSDA_DriveMode_1_ADDR], (I2CHW_TempSDA_MASK|I2CHW_TempSCL_MASK)
    M8C_SetBank0
    or  reg[I2CHW_TempSDA_DriveMode_2_ADDR], (I2CHW_TempSDA_MASK|I2CHW_TempSCL_MASK)
   
    BitSetI2CHW_Temp_CFG I2C_S_EN                                    ;Enable SDA interrupt
    nop
    nop
    nop
    nop
    nop
   
    M8C_SetBank1 ;The SDA and SCL pins are restored to Open Drain Low drive mode
    or reg[I2CHW_TempSDA_DriveMode_0_ADDR], (I2CHW_TempSDA_MASK|I2CHW_TempSCL_MASK)
    or reg[I2CHW_TempSDA_DriveMode_1_ADDR], (I2CHW_TempSDA_MASK|I2CHW_TempSCL_MASK)
    M8C_SetBank0
    or reg[I2CHW_TempSDA_DriveMode_2_ADDR], (I2CHW_TempSDA_MASK|I2CHW_TempSCL_MASK)
    
    RAM_EPILOGUE RAM_USE_CLASS_1
    ret

.ENDSECTION
ENDIF

.SECTION
;-----------------------------------------------------------------------------
;  FUNCTION NAME: I2CHW_Temp_DisableInt
;  FUNCTION NAME: I2CHW_Temp_Stop
;
;  DESCRIPTION:
;     Disables I2CHW_Temp slave by disabling SDA interrupt
;
;-----------------------------------------------------------------------------
;
;  ARGUMENTS: none
;
;  RETURNS: none
;
;  SIDE EFFECTS:
;    The A and X registers may be modified by this or future implementations
;    of this function.  The same is true for all RAM page pointer registers in
;    the Large Memory Model.  When necessary, it is the calling function's
;    responsibility to perserve their values across calls to fastcall16 
;    functions.
;          
 I2CHW_Temp_DisableInt:
_I2CHW_Temp_DisableInt:
 I2CHW_Temp_Stop:
_I2CHW_Temp_Stop:
    RAM_PROLOGUE RAM_USE_CLASS_1
    M8C_DisableIntMask I2CHW_Temp_INT_REG, I2CHW_Temp_INT_MASK
    RAM_EPILOGUE RAM_USE_CLASS_1
    ret

.ENDSECTION

IF (I2CHW_Temp_MUM_SEL & (I2CHW_Temp_SLAVE | I2CHW_Temp_MMS))
.SECTION
;-----------------------------------------------------------------------------
;  FUNCTION NAME: I2CHW_Temp_DisableSlave
;
;  DESCRIPTION:
;     Disables I2CHW_Temp slave by disabling SDA interrupt
;
;-----------------------------------------------------------------------------
;
;  ARGUMENTS: none
;
;  RETURNS: none
;
;  SIDE EFFECTS:
;    The A and X registers may be modified by this or future implementations
;    of this function.  The same is true for all RAM page pointer registers in
;    the Large Memory Model.  When necessary, it is the calling function's
;    responsibility to perserve their values across calls to fastcall16 
;    functions.
;          
 I2CHW_Temp_DisableSlave:
_I2CHW_Temp_DisableSlave:
    RAM_PROLOGUE RAM_USE_CLASS_1
    BitClrI2CHW_Temp_CFG I2C_S_EN                                    ;Disable the Slave
    RAM_EPILOGUE RAM_USE_CLASS_1
    ret

.ENDSECTION
ENDIF

IF (I2CHW_Temp_MUM_SEL & (I2CHW_Temp_MSTR | I2CHW_Temp_MMS))
.SECTION
;-----------------------------------------------------------------------------
;  FUNCTION NAME: I2CHW_Temp_DisableMstr
;
;  DESCRIPTION:
;     Disables I2CHW_Temp slave by disabling SDA interrupt
;
;-----------------------------------------------------------------------------
;
;  ARGUMENTS: none
;
;  RETURNS: none
;
;  SIDE EFFECTS:
;    The A and X registers may be modified by this or future implementations
;    of this function.  The same is true for all RAM page pointer registers in
;    the Large Memory Model.  When necessary, it is the calling function's
;    responsibility to perserve their values across calls to fastcall16 
;    functions.
;          
 I2CHW_Temp_DisableMstr:
_I2CHW_Temp_DisableMstr:
    RAM_PROLOGUE RAM_USE_CLASS_1
    BitClrI2CHW_Temp_CFG I2C_M_EN                                    ;Disable the Master
    RAM_EPILOGUE RAM_USE_CLASS_1
   ret

.ENDSECTION
ENDIF

IF (I2CHW_Temp_MUM_SEL & (I2CHW_Temp_SLAVE | I2CHW_Temp_MMS))
 IF (I2CHW_Temp_CY8C22x45)
 .SECTION
;-----------------------------------------------------------------------------
;  FUNCTION NAME: void  I2CHW_Temp_EnableHWAddrCheck(void)
;
;  DESCRIPTION:
;   Set respective bit to engage the HardWare Address Recognition 
;   feature in I2C slave block.
;
;-----------------------------------------------------------------------------
;
;  ARGUMENTS: none
;
;  RETURNS: none
;
;  SIDE EFFECTS:
;    If the HardWare Address Recognition feature is enabled, the ROM registers reading does not work.
;    The HardWare Address Recognition feature should be disabled for using ROM registers.
;
;    The A and X registers may be modified by this or future implementations
;    of this function.  The same is true for all RAM page pointer registers in
;    the Large Memory Model.  When necessary, it is the calling function's
;    responsibility to perserve their values across calls to fastcall16 
;    functions.
;
 I2CHW_Temp_EnableHWAddrCheck:
_I2CHW_Temp_EnableHWAddrCheck:
   RAM_PROLOGUE RAM_USE_CLASS_1
   M8C_SetBank1
   or    reg[I2CHW_Temp_ADDR], I2CHW_Temp_HW_ADDR_EN
   M8C_SetBank0
   RAM_EPILOGUE RAM_USE_CLASS_1
   ret
.ENDSECTION

.SECTION
;-----------------------------------------------------------------------------
;  FUNCTION NAME: void  I2CHW_Temp_DisableHWAddrCheck(void)
;
;  DESCRIPTION:
;   Clear respective bit to disengage the HardWare Address Recognition 
;   feature in I2C slave block.
;
;-----------------------------------------------------------------------------
;
;  ARGUMENTS: none
;
;  RETURNS: none
;
;  SIDE EFFECTS:
;    The A and X registers may be modified by this or future implementations
;    of this function.  The same is true for all RAM page pointer registers in
;    the Large Memory Model.  When necessary, it is the calling function's
;    responsibility to perserve their values across calls to fastcall16 
;    functions.
;
 I2CHW_Temp_DisableHWAddrCheck:
_I2CHW_Temp_DisableHWAddrCheck:
   RAM_PROLOGUE RAM_USE_CLASS_1
   M8C_SetBank1
   and   reg[I2CHW_Temp_ADDR], ~I2CHW_Temp_HW_ADDR_EN
   M8C_SetBank0
   RAM_EPILOGUE RAM_USE_CLASS_1
   ret
.ENDSECTION
 ENDIF
ENDIF

; End of File I2CHW_Temp.asm
