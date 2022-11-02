	cpu LMM
	.module main.c
	.area text(rom, con, rel)
	.dbfile ./main.c
	.area data(ram, con, rel)
	.dbfile ./main.c
_my_table::
	.byte 0
	.byte 0,0
	.word 0,0,0,0,0
	.word 0,0,0,0,0
	.word 0,0,0,0,0
	.word 0,0,0,0,0
	.word 0,0,0,0,0
	.byte 0,0,0,0,0,0,0
	.dbfile C:\Users\georg\OneDrive\DOCUME~1\GitHub\TALKIN~1\PDPROJ~1\PDPROJ~1\main.c
	.dbstruct 0 3 vocabTable
	.dbfield 0 word c
	.dbfield 1 frequency c
	.dbfield 2 age c
	.dbend
	.dbsym e my_table _my_table A[60:20]S[vocabTable]
	.area data(ram, con, rel)
	.dbfile C:\Users\georg\OneDrive\DOCUME~1\GitHub\TALKIN~1\PDPROJ~1\PDPROJ~1\main.c
	.area data(ram, con, rel)
	.dbfile C:\Users\georg\OneDrive\DOCUME~1\GitHub\TALKIN~1\PDPROJ~1\PDPROJ~1\main.c
_neighours_table::
	.byte 0
	.byte 0,0
	.word 0,0,0,0,0
	.word 0,0,0,0,0
	.word 0,0,0,0,0
	.word 0,0,0,0,0
	.word 0,0,0,0,0
	.byte 0,0,0,0,0,0,0
	.dbsym e neighours_table _neighours_table A[60:20]S[vocabTable]
	.area data(ram, con, rel)
	.dbfile C:\Users\georg\OneDrive\DOCUME~1\GitHub\TALKIN~1\PDPROJ~1\PDPROJ~1\main.c
	.area data(ram, con, rel)
	.dbfile C:\Users\georg\OneDrive\DOCUME~1\GitHub\TALKIN~1\PDPROJ~1\PDPROJ~1\main.c
_sortVocabTables::
	.byte 0
	.dbsym e sortVocabTables _sortVocabTables c
	.area data(ram, con, rel)
	.dbfile C:\Users\georg\OneDrive\DOCUME~1\GitHub\TALKIN~1\PDPROJ~1\PDPROJ~1\main.c
	.area text(rom, con, rel)
	.dbfile C:\Users\georg\OneDrive\DOCUME~1\GitHub\TALKIN~1\PDPROJ~1\PDPROJ~1\main.c
	.dbfunc e TimerISR _TimerISR fV
_TimerISR::
	.dbline -1
	or F,-64
	push A
	mov A,REG[0xd0]
	push A
	.dbline 47
; //----------------------------------------------------------------------------
; // C main line
; //----------------------------------------------------------------------------
; 
; #include <m8c.h>        // part specific constants and macros
; #include "PSoCAPI.h"    // PSoC API definitions for all User Modules
; #include <stdio.h>
; #define TEMP_SENSOR_SLAVE_ADDRESS 0x18
; #define TEMP_REG    			  0x05
; #define MIC_ADC_CHANNEL			  0x01
; #define LIGHT_SENSOR_ADC_CHANNEL  0x02
; #define SIZE 					  20
; #define X 						  3400 // 3.4s
; #define Z						  200  // 200ms
; #define Y                         200  // 200ms
; #define Q                         200  // Have find an optimum value
; 
; enum light  {DARK, NORMAL , BRIGHT};
; enum temp   {COLD, MEDIUM, HOT};
; 
; typedef struct vocabTable
; {
; 	BYTE word;
; 	BYTE frequency;
; 	BYTE age;
; }vocabTable_type;
; 
; vocabTable_type my_table[SIZE]= {0};		// To do: finalize on the size
; vocabTable_type neighours_table[SIZE] = {0};// To do: finalize on the size
; 
; BOOL sortVocabTables = FALSE;
; void print_LCD_debug_msg(char* msg,INT row,INT col);
; INT readDualAdc(INT channel);
; INT read_temperature(void);
; float getLux(void) ;
; BYTE create_word(float lux, INT temp);
; void blocking_delay(BYTE bTimes);
; void update_table(BYTE word_encoding, vocabTable_type* table );
; void quickSort(vocabTable_type A[], int l, int h);
; int part(vocabTable_type arr[], int l, int h);
; void swap(vocabTable_type* a, vocabTable_type* b);
; 
; #pragma interrupt_handler TimerISR
; #pragma interrupt_handler ResetSwitchISR
; // This ISR is triffered every 60 seconds
; void TimerISR(void )
; {
	.dbline 48
; 	sortVocabTables = TRUE;
	mov REG[0xd0],>_sortVocabTables
	mov [_sortVocabTables],1
	.dbline -2
L1:
	pop A
	mov REG[208],A
	pop A
	.dbline 0 ; func end
	reti
	.dbend
	.dbfunc e ResetSwitchISR _ResetSwitchISR fV
_ResetSwitchISR::
	.dbline -1
	push A
	.dbline 52
; }
; 
; void ResetSwitchISR(void )
; {
	.dbline 53
; 	INT_MSK3 = INT_MSK3 | 0x80;
	or REG[0xde],-128
	.dbline -2
L2:
	pop A
	.dbline 0 ; func end
	reti
	.dbend
	.dbfunc e main _main fV
;           word -> X+6
;      tempValue -> X+4
;       luxValue -> X+0
_main::
	.dbline -1
	push X
	mov X,SP
	add SP,7
	.dbline 58
; 	// Trigger the hardware reset ISR from the software	
; }
; 
; void main(void)
; {
	.dbline 64
; 	float luxValue; // Ambient Light illumiance value
; 	INT   tempValue;     // Ambient temperature
; 	BYTE  word;
; 	
; 	// PGA1 init
; 	PGA_1_Start(PGA_1_HIGHPOWER);
	push X
	mov A,3
	xcall _PGA_1_Start
	.dbline 66
; 	// PGA2 init
; 	PGA_2_Start(PGA_1_HIGHPOWER);
	mov A,3
	xcall _PGA_2_Start
	.dbline 69
; 
; 	// Initial the Dual ADC
; 	DUALADC_1_Start(DUALADC_1_HIGHPOWER);     	// Turn on Analog section
	mov A,3
	xcall _DUALADC_1_Start
	.dbline 70
; 	DUALADC_1_SetResolution(10);            	// Set resolution to 10 Bits
	mov A,10
	xcall _DUALADC_1_SetResolution
	.dbline 71
; 	DUALADC_1_GetSamples(0); 
	mov A,0
	xcall _DUALADC_1_GetSamples
	.dbline 74
; 
; 	// Initializes LCD to use the multi-line 4-bit interface
; 	LCD_2_Start();
	xcall _LCD_2_Start
	.dbline 77
; 	
; 	// Enables the I2C HW block as a Master 
; 	I2CHW_Temp_EnableMstr();
	xcall _I2CHW_Temp_EnableMstr
	.dbline 83
; 	
; 	//Enable reset button interrupt
; 	//To do How to trigger a software reset via button?
; 	
; 	// Initialize time
; 	Timer16_1_WritePeriod(0xffff);// Do this in the config
	mov A,-1
	mov X,-1
	xcall _Timer16_1_WritePeriod
	.dbline 84
;   	Timer16_1_WriteCompareValue(0x0001); 
	mov X,0
	mov A,1
	xcall _Timer16_1_WriteCompareValue
	.dbline 85
; 	Timer16_1_EnableInt();//Enable Timer interrupt
	xcall _Timer16_1_EnableInt
	pop X
	.dbline 89
; 	
; 	
; 	// Enable global interrupts
; 	M8C_EnableGInt;
		or  F, 01h

	.dbline 92
; 	
; 	// Start the timer
; 	Timer16_1_Start(); 
	push X
	xcall _Timer16_1_Start
	pop X
	xjmp L5
L4:
	.dbline 94
; 	while(1)
; 	{
	.dbline 99
; 		
; 		// Sense /////////////////////////////
; 		
; 		//Read light sensor value
; 		luxValue   = getLux(); // luxValue ranges from 0 to 1000
	xcall _getLux
	mov REG[0xd0],>__r0
	mov A,[__r0]
	mov [X+0],A
	mov A,[__r1]
	mov [X+1],A
	mov A,[__r2]
	mov [X+2],A
	mov A,[__r3]
	mov [X+3],A
	.dbline 101
; 		//Read temperature value
; 		tempValue  = read_temperature();\
	xcall _read_temperature
	mov REG[0xd0],>__r0
	mov A,[__r1]
	mov [X+5],A
	mov A,[__r0]
	mov [X+4],A
	.dbline 104
; 	
; 		// Word formation //////////////////
; 		word = create_word(luxValue, tempValue);
	mov A,[X+4]
	push A
	mov A,[X+5]
	push A
	mov A,[X+0]
	push A
	mov A,[X+1]
	push A
	mov A,[X+2]
	push A
	mov A,[X+3]
	push A
	xcall _create_word
	mov REG[0xd0],>__r0
	mov [X+6],A
	.dbline 108
; 		
; 		
; 		// Update my vocab
; 		update_table(word,my_table);
	mov A,>_my_table
	push A
	mov A,<_my_table
	push A
	mov A,[X+6]
	push A
	xcall _update_table
	add SP,-9
	.dbline 113
; 		
; 
; 		// Sort vocab table
; 		
; 		if (sortVocabTables == TRUE)
	mov REG[0xd0],>_sortVocabTables
	cmp [_sortVocabTables],1
	jnz L7
	.dbline 115
; 		{
; 			quickSort(my_table, 0 , SIZE-1);
	.dbline 115
	mov A,0
	push A
	mov A,19
	push A
	mov A,0
	push A
	push A
	mov A,>_my_table
	push A
	mov A,<_my_table
	push A
	xcall _quickSort
	.dbline 117
; 			// Remove the table entries which does not meet the required frequency threshold
; 			remove(my_table,Q);
	mov A,0
	push A
	mov A,-56
	push A
	mov A,>_my_table
	push A
	mov A,<_my_table
	push A
	xcall _remove
	add SP,-10
	.dbline 118
; 		}
L7:
	.dbline 122
; 		// Speak ///////////////////////////
; 		
; 		// Listen? /////////////////////////
; 	}
L5:
	.dbline 93
	xjmp L4
X0:
	.dbline -2
L3:
	add SP,-7
	pop X
	.dbline 0 ; func end
	jmp .
	.dbsym l word 6 c
	.dbsym l tempValue 4 I
	.dbsym l luxValue 0 D
	.dbend
	.dbfunc e create_word _create_word fc
;           temp -> X-9
;            lux -> X-7
_create_word::
	pop X
	.dbline 0 ; func end
	ret
	.dbsym l temp -9 I
	.dbsym l lux -7 D
	.dbend
	.dbfunc e update_table _update_table fV
;          table -> X-6
;  word_encoding -> X-4
_update_table::
	pop X
	.dbline 0 ; func end
	ret
	.dbsym l table -6 pS[vocabTable]
	.dbsym l word_encoding -4 c
	.dbend
	.dbfunc e swap _swap fV
;              b -> X-7
;              a -> X-5
_swap::
	pop X
	.dbline 0 ; func end
	ret
	.dbsym l b -7 pS[vocabTable]
	.dbsym l a -5 pS[vocabTable]
	.dbend
	.dbfunc e part _part fI
;              h -> X-9
;              l -> X-7
;            arr -> X-5
_part::
	pop X
	.dbline 0 ; func end
	ret
	.dbsym l h -9 I
	.dbsym l l -7 I
	.dbsym l arr -5 pS[vocabTable]
	.dbend
	.dbfunc e quickSort _quickSort fV
;              h -> X-9
;              l -> X-7
;              A -> X-5
_quickSort::
	pop X
	.dbline 0 ; func end
	ret
	.dbsym l h -9 I
	.dbsym l l -7 I
	.dbsym l A -5 pS[vocabTable]
	.dbend
