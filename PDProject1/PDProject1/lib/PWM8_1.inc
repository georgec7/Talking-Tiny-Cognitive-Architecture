;;*****************************************************************************
;;*****************************************************************************
;;  FILENAME: PWM8_1.inc
;;   Version: 2.60, Updated on 2015/3/4 at 22:26:52
;;  Generated by PSoC Designer 5.4.3191
;;
;;  DESCRIPTION: Assembler declarations for the PWM8 user module interface
;;-----------------------------------------------------------------------------
;;  Copyright (c) Cypress Semiconductor 2015. All Rights Reserved.
;;*****************************************************************************
;;*****************************************************************************

include "m8c.inc"


;--------------------------------------------------
; Constants for PWM8_1 API's.
;--------------------------------------------------

PWM8_1_CONTROL_REG_START_BIT:              equ 0x01    ; Control register start bit
PWM8_1_INT_REG:                            equ 0x0e1
PWM8_1_INT_MASK:                           equ 0x20


;--------------------------------------------------
; Constants for PWM8_1 user defined values
;--------------------------------------------------

PWM8_1_PERIOD:                             equ 0xff
PWM8_1_PULSE_WIDTH:                        equ 0x32


;--------------------------------------------------
; Register Address Constants for PWM8_1
;--------------------------------------------------

PWM8_1_COUNTER_REG:                    equ 0x34   ; DR0 Count register
PWM8_1_PERIOD_REG:                     equ 0x35   ; DR1 Period register
PWM8_1_COMPARE_REG:                    equ 0x36   ; DR2 Compare register
PWM8_1_CONTROL_REG:                    equ 0x37   ; Control register
PWM8_1_FUNC_REG:                       equ 0x34   ; Function register
PWM8_1_INPUT_REG:                      equ 0x35   ; Input register
PWM8_1_OUTPUT_REG:                     equ 0x36   ; Output register


;--------------------------------------------------
; PWM8_1 Macro 'Functions'
;--------------------------------------------------

   macro PWM8_1_Start_M
   or    reg[PWM8_1_CONTROL_REG],  PWM8_1_CONTROL_REG_START_BIT
   endm

   macro PWM8_1_Stop_M
   and   reg[PWM8_1_CONTROL_REG], ~PWM8_1_CONTROL_REG_START_BIT
   endm

   macro PWM8_1_EnableInt_M
   M8C_EnableIntMask PWM8_1_INT_REG, PWM8_1_INT_MASK
   endm

   macro PWM8_1_DisableInt_M
   M8C_DisableIntMask PWM8_1_INT_REG, PWM8_1_INT_MASK
   endm


; end of file PWM8_1.inc
