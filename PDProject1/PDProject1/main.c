//----------------------------------------------------------------------------
// C main line
//----------------------------------------------------------------------------

#include <m8c.h>        // part specific constants and macros
#include "PSoCAPI.h"    // PSoC API definitions for all User Modules
#include <stdio.h>
#define TEMP_SENSOR_SLAVE_ADDRESS 0x18
#define TEMP_REG    			  0x05
#define MIC_ADC_CHANNEL			  0x01
#define LIGHT_SENSOR_ADC_CHANNEL  0x02
#define SIZE 					  20
#define X 						  3400 // 3.4s
#define Z						  200  // 200m
#define Z_ns					  (Z*1000000) //ns
#define Z_div					  (Z_ns/50)
#define Y                         200  // 200ms
#define Y_ns					  (Z*1000000) //ns
#define Y_div					  (Z_ns/50)
#define Q                         200  // Have find an optimum value

enum light  {DARK, NORMAL , BRIGHT};
enum temp   {COLD, MEDIUM, HOT};

typedef struct vocabTable
{
	BYTE word;
	BYTE frequency;
	BYTE age;
}vocabTable_type;

vocabTable_type my_table[SIZE]= {0};		// To do: finalize on the size
vocabTable_type neighours_table[SIZE] = {0};// To do: finalize on the size

BOOL sortVocabTables = FALSE;
void print_LCD_debug_msg(char* msg,INT row,INT col);
INT readDualAdc(INT channel);
INT read_temperature(void);
float getLux(void) ;
BYTE create_word(float lux, INT temp);
void blocking_delay(BYTE bTimes);
void update_table(BYTE word_encoding, vocabTable_type* table );
void quickSort(vocabTable_type A[], int l, int h);
int part(vocabTable_type arr[], int l, int h);
void swap(vocabTable_type* a, vocabTable_type* b);

#pragma interrupt_handler TimerISR
#pragma interrupt_handler ResetSwitchISR
// This ISR is triffered every 60 seconds
void TimerISR(void )
{
	sortVocabTables = TRUE;
}

void ResetSwitchISR(void )
{
	// Trigger a reset 
	// Trigger the hardware reset ISR from the software	
}

void main(void)
{
	float luxValue; // Ambient Light illumiance value
	INT   tempValue;     // Ambient temperature
	BYTE  word;
	BOOL neword;
	// PGA1 init
	PGA_1_Start(PGA_1_HIGHPOWER);
	// PGA2 init
	PGA_2_Start(PGA_1_HIGHPOWER);

	// Initial the Dual ADC
	DUALADC_1_Start(DUALADC_1_HIGHPOWER);     	// Turn on Analog section
	DUALADC_1_SetResolution(10);            	// Set resolution to 10 Bits
	DUALADC_1_GetSamples(0); 

	// Initializes LCD to use the multi-line 4-bit interface
	LCD_2_Start();
	
	// Enables the I2C HW block as a Master 
	I2CHW_Temp_EnableMstr();
	
	//Enable reset button interrupt
	//To do How to trigger a software reset via button?
	
	// Initialize time
	Timer16_1_WritePeriod(0xffff);// Do this in the config
  	Timer16_1_WriteCompareValue(0x0001); 
	Timer16_1_EnableInt();//Enable Timer interrupt
	
	
	// Enable global interrupts
	M8C_EnableGInt;
	
	// Start the timer
	Timer16_1_Start(); 
	while(1)
	{
		
		// Sense /////////////////////////////
		
		//Read light sensor value
		luxValue   = getLux(); // luxValue ranges from 0 to 1000
		//Read temperature value
		tempValue  = read_temperature();\
	
		// Word formation //////////////////
		word = create_word(luxValue, tempValue);
		
		
		// Update my vocab
		update_table(word,my_table);
		

		// Sort vocab table
		
		if (sortVocabTables == TRUE)
		{
			quickSort(my_table, 0 , SIZE-1);
			// Remove the table entries which does not meet the required frequency threshold
			remove(my_table,Q);
		}
		// Speak ///////////////////////////
		if(neword)
		{
			speak();
		}
		// Listen? ///////////////////////// To Do
	}
}

BYTE create_word(float lux, INT temp)
{
	enum light light_label;
	enum temp temp_label;
	BYTE word;
	if (lux >= 0 && lux < 333)
		light_label = DARK;
	else if(lux >= 333 && lux < 666)
		light_label = NORMAL;
	else if(lux >=666 && lux <=1000)
		light_label = BRIGHT;
	
	// Check this
	if (temp < 10)
		temp_label = COLD;
	else if(temp >=10 && temp < 23)
		temp_label = MEDIUM;
	else if(temp >= 23)
		temp_label = HOT;
	
	if (light_label == DARK && temp_label == COLD)
		word = 1; //Dark light and Cold condition
		LCD_2_Position(0,0);
		LCD_2_PrCString("Word Generated is: DARK COLD");
	else if (light_label == DARK && temp_label == MEDIUM)
		word = 2; //Dark light and Medium temp condition
		LCD_2_Position(0,0);
		LCD_2_PrCString("Word Generated is: DARK MEDIUM");
	else if (light_label == DARK && temp_label == HOT)
		word = 3; //Dark light and Hot temp condition
		LCD_2_Position(0,0);
		LCD_2_PrCString("Word Generated is: DARK HOT");
	else if (light_label == NORMAL && temp_label == COLD)
		word = 4; //Normal light and cold temp condition
		LCD_2_Position(0,0);
		LCD_2_PrCString("Word Generated is: NORMAL COLD");
	else if (light_label == NORMAL && temp_label == MEDIUM)
		word = 5; //Normal light and medium temp condition
		LCD_2_Position(0,0);
		LCD_2_PrCString("Word Generated is: NORMAL MEDIUM");
	else if (light_label == NORMAL && temp_label == HOT)
		word = 6; //Normal light and hot temp condition
		LCD_2_Position(0,0);
		LCD_2_PrCString("Word Generated is: NORMAL HOT");
	else if (light_label == BRIGHT && temp_label == COLD)
		word = 7; //Bright light and cold temp condition
	else if (light_label == BRIGHT && temp_label == MEDIUM)
		word = 8; //Bright light and medium temp condition
		LCD_2_Position(0,0);
		LCD_2_PrCString("Word Generated is: BRIGHT MEDIUM");
	else if (light_label == BRIGHT && temp_label == HOT)
		word = 9; //Bright light and hot temp condition
		LCD_2_Position(0,0);
		LCD_2_PrCString("Word Generated is: BRIGHT HOT");
	
	return word;// return word encoding
}


BOOL update_table(BYTE word_encoding, vocabTable_type* table)
{
	int i;
	BOOL found = FALSE;
	for (i= 0; i< SIZE; i++)
	{
		table[i].age++;
		if (table[i].word == word_encoding)
		{
			found =TRUE;
			table[i].frequency++;
		}
	}
	if (found == FALSE)
	{	
		table[i].word = word_encoding;
		table[i].frequency = 1;
		table[i].age = 1;
	}
return found;
}

// Sort Algorithm for tables based on quick sort

void swap(vocabTable_type* a, vocabTable_type* b)
{
    vocabTable_type temp = *a;
    *a = *b;
    *b = temp;
}

int part(vocabTable_type arr[], int l, int h)
{
    int x = arr[h].frequency;
    int i = (l - 1);
 	int j; 
    for ( j = l; j <= h - 1; j++) 
	{
        if (arr[j].frequency >= x) 
		{
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[h]);
    return (i + 1);
}

void quickSort(vocabTable_type A[], int l, int h)
{
    if (l < h) {
        int p = part(A, l, h);
        quickSort(A, l, p - 1);
        quickSort(A, p + 1, h);
    }
}

void remove(BYTE freq_threshold,vocabTable_type table )
{
	int i;
	for(i= 0; i< SIZE; i++)
	{	
		if(table[i].frequency < freq_threshold)
		{
			table[i]= {0};// Remove the old word and details and initial with zero	
		}
	}
	
}
INT read_temperature(void)
{
	BYTE UpperByte = 0;
	BYTE LowerByte = 0;
	INT Temperature =0;
	
	I2CHW_Temp_fSendStart(TEMP_SENSOR_SLAVE_ADDRESS, I2CHW_Temp_WRITE);
	I2CHW_Temp_fWrite(TEMP_REG);
	I2CHW_Temp_SendStop();
	I2CHW_Temp_fSendStart(TEMP_SENSOR_SLAVE_ADDRESS, I2CHW_Temp_READ);
	UpperByte = I2CHW_Temp_bRead(I2CHW_Temp_ACKslave);
	LowerByte = I2CHW_Temp_bRead(I2CHW_Temp_NAKslave);
	I2CHW_Temp_SendStop();
	
	UpperByte = UpperByte & 0x1F; //Clear flag bits
	
	if ((UpperByte & 0x10) == 0x10){ //TA < 0°C
	UpperByte = UpperByte & 0x0F;//Clear SIGN
	Temperature = 256 - (UpperByte * 16 + LowerByte / 16);
	}else //TA ≥ 0°C*/
	Temperature = (UpperByte * 16 + LowerByte / 16);//Temperature = Ambient Temperature (°C)
	
	return Temperature;
}
float getLux(void )
{
	float volts = readDualAdc(LIGHT_SENSOR_ADC_CHANNEL)*5.0 /1024.0;
	float amps  = volts/10000.0; // Across 10,000 Ohms
	float microamps = amps * 1000000;
	float lux = microamps * 2.0;
	
	return lux;
}	

INT readDualAdc(INT channel)
{
	int iResult1, iResult2, iResult;
	while(DUALADC_1_fIsDataAvailable() == 0);  // Wait for data to be ready
	iResult1 = DUALADC_1_iGetData1();          // Get Data from ADC Input1
	iResult2 = DUALADC_1_iGetData2ClearFlag(); // Get Data from ADC Input2
    
                                      		// and clear data ready flag
	if (channel == 1)
	{
		iResult = iResult1;					// Return Mic ADC readings result
	}
	else if ( channel == 2)
	{
		iResult = iResult2;					// Return light sensor ADC readings result
	}
	return iResult;
}


void blocking_delay(BYTE bTimes)
{
	// Can be used only if the delaly required is a mutiple of 50us.
	// This sufficient for this project.
	LCD_2_Delay50uTimes(bTimes);// Reuse the delay function provided by LCD user module		
}

void speak(BYTE word_encoding)
{
	BYTE y;
	switch (word_encoding)
	{
		//        10101010101010101 One word consists of 17 stamps
		case 1: //10000000000000000 1 loud stamp, 16 silent stamp ( 
			PWM8_1_Start();
			blocking_delay(Z_div);
			PWM8_1_Stop();
			y = Y_div* 16;             
			blocking_delay(y);
			break;;
		case 2:// 10100000000000000 2 loud stamp,rest silent stamps 
			PWM8_1_Start();
			blocking_delay(Z_div);
			PWM8_1_Stop();
			blocking_delay(Y_div);
			PWM8_1_Start();
			blocking_delay(Z_div);
			PWM8_1_Stop();
			y = Y_div *14;
			blocking_delay(y);
			break;
		case 3:// 10101000000000000 3 loud stamp,rest silent stamps
			//To do
			break;
		case 4:// 10101010000000000 4 loud stamp,rest silent stamps
			//To do
			break;
		case 5:// 10101010100000000 5 loud stamp,rest silent stamps
			//To do
			break;
		case 6:// 10101010101000000 6 loud stamp,rest silent stamps
			//To do
			break;
		case 7:// 10101010101010000 7 loud stamp,rest silent stamps
			//To do
			break;
		case 8:// 10101010101010100 8 loud stamp,rest silent stamps
			//To do
			break;
		case 9:// 10101010101010101 9 loud stamp,rest silent stamps
			//To do
			break;
	}
}


