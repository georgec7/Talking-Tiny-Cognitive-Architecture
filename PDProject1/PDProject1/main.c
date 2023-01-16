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
#define X 						  1600 // 1.6s
#define Z						  200  // 200ms
#define Z_ns					  (Z*1000000) //ns
#define Z_div					  (Z_ns/50)
#define Y                         200  // 200ms
#define Y_ns					  (Z*1000000) //ns
#define Y_div					  (Z_ns/50)
#define Q                         200  // Have find an optimum value
#define LS_NIBBLE_MASK 			  0x0F
#define MS_NIBBLE_MASK 			  0xF0

#define LOUD_STAMP_THRESHOLD 	  0x200

enum light  {DARK, NORMAL , BRIGHT};
enum temp   {COLD, MEDIUM, HOT};
enum comb   {DARK_COLD, 
			DARK_MEDIUM,
			DARK_HOT,
			NORMAL_COLD,
			NORMAL_MEDIUM,
			NORMAL_HOT,
			BRIGHT_COLD,
			BRIGHT_MEDIUM,
			BRIGHT_HOT, 
			DARK_ONLY,
			NORMAL_ONLY,
			BRIGHT_ONLY,
			COLD_ONLY,
			MEDIUM_ONLY,
			HOT_ONLY};
char lookuptable[15] = {"DARK_COLD",
						"DARK_MEDIUM",
						"DARK_HOT",
						"NORMAL_COLD",
						"NORMAL_MEDIUM",
						"NORMAL_HOT",
						"BRIGHT_COLD",
						"BRIGHT_MEDIUM",
						"BRIGHT_HOT",
						"DARK_ONLY"
						"NORMAL_ONLY",
						"BRIGHT_ONLY",
						"COLD_ONLY",
						"MEDIUM_ONLY",
						"HOT_ONLY"
}
typedef struct vocabTable
{
	BYTE word;
	enum comb label;
	BYTE frequency;
	BYTE age;
}vocabTable_type;
typedef  struct learn_table
{
	BYTE word;
	enum comb label;
	BYTE frequency;
	BYTE frequency_of_mismatch;
}learn_table_type;

vocabTable_type my_table[SIZE]= {0};			  
vocabTable_type neighours_table[SIZE] = {0};	  
learn_table_type bayesian_learn_table[18] = {0};

// This is the initial default ranges that both the systems will start with
INT lux_range_1 = 333;
INT lux_range_2 = 666;
INT temp_range_1 = 10;
INT temp_range_2 =23;
/*
typedef struct learning_table
{
	BYTE 	
}
*/

vocabTable_type my_table[SIZE]= {0};			  // To do: finalize on the size
vocabTable_type neighours_table[SIZE] = {0};	  // To do: finalize on the size
learn_table_type bayesian_learn_table[18] = {0};// To do: finalize on the size


INT wElapsedTime = 0; 						// timer with us resolution

BOOL sortVocabTables = FALSE;
void print_LCD_debug_msg(char* msg,INT row,INT col);
INT readDualAdc(INT channel);
INT read_temperature(void);
float getLux(void) ;
BYTE create_word(float lux, INT temp, char *label,INT l1,INT l2,INT t1,INT t2);
void blocking_delay(BYTE bTimes);
BOOL update_table(BYTE word_encoding, vocabTable_type* table,char * label);
void quickSort(vocabTable_type A[], int l, int h);
int part(vocabTable_type arr[], int l, int h);
void swap(vocabTable_type* a, vocabTable_type* b);
void remove(BYTE freq_threshold,vocabTable_type* table );
void speak(BYTE word_encoding);
BYTE listen(void);
BOOL check_if_labelPresent(char * index);
BOOL check_if_bit_stream_match(char index,BYTE word_listened);
void re_learn_ranges(BYTE word_listened);
BYTE highest_conditional_prob(void);

#pragma interrupt_handler Timer16_1_ISR
void Timer16_1_ISR(void )
{
		wElapsedTime++;
}
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
	BYTE  speak_word;
	BYTE  word_listened;
	BOOL neword;
	char * label;
	char * index;
	
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
		tempValue  = read_temperature();
	
		// Word formation //////////////////
		speak_word= create_word(luxValue, tempValue,label, lux_range_1,lux_range_2,temp_range_1,temp_range_2);
		
		
		// Update my vocab
		update_table(speak_word,my_table,label);
		

		// Sort vocab table
		
		if (sortVocabTables == TRUE)
		{
			quickSort(my_table, 0 , SIZE-1);
			// Remove the table entries which does not meet the required frequency threshold
			remove(Q,my_table);
		}
		// Speak ///////////////////////////
		if(neword)
		{
			speak(speak_word);
		}
		// Listen the bit stream from the other agent
		word_listened = listen();
		
		// Check if the listened word needs to be entered in the neighbour's vocab table
			// Check if the received bit stream represents a light condition or temp condition or both.
			// For example, if listened bit steam is 0000 1010, since the Most Significant 4 bit representing Light is zero, and Least Significant 4 bits is non zero, this means that 
			// this word bit stream is represents only a temperature condition.
			// If the a same word label is already present but with a different bit stream it means the sender agent have extended the word meaning
			// to include a new value for temperature or light conditions. In this case, re-adjust or re learn the ranges for tempearture and light.
		// 
		// Create a word label for when the a word is received and check if there is a bit stream data available in the neighbours vocab table with the same label.
		// if the same label is present but the bit stream mismatches with the received bit stream , it means the other agent have extended the meaning of the word.
		
		if(check_if_labelPresent(index))
		{
			//Check bitstream associated to the receieved word matches with the bitstream of the word which is already present in the neighbours table
			if(check_if_bit_stream_match(*index,word_listened))
			{
				//If bit stream matches re_learning of ranges not required. Other agent hasn't extended its meaning.
			}
			else 
			{
				//Bit stream doesn't match. Adjust the ranges of temp/light and generate the bitstream/word_encoding. Continue this till the bistream generated 
				//matches with the received bit stream
				re_learn_ranges(word_listened);
				
				// This re learning of ranges will inturn improve the bayesian leaning process and will improve the accuracy of the word 
				// prediction when in different environment.
			}
		}
		else
		{
			//It is a new word. update the table with new word received.
			update_table(word_listened,neighours_table,NULL);
		}
		
		if (mode == SAME_ENVIRONMENT)
		{
			//Learn only when in same evironment
			//Used for bayesian word prediction when mode == Different environment.
			update_frequency_table(word_listened);
		}
		
		//-----------------Decesion Making---------------------------
		if( mode == SAME_ENVIRONMENT)
		//Use sensor values of the agent to predict the meaning of the word received
		{
			// This wil print the predicted word
			create_word(luxValue, tempValue,label, lux_range_1,lux_range_2,temp_range_1,temp_range_2);
		}
			
		if( mode == DIFFERENT_ENVIRONMENT)
		{
			//Use bayesian table to predict the meaning of the word associated to the received bit stream	*/
			predicted_label = highest_conditional_prob();
			for(i<0;i<SIZE;i++)
			{
				if(predicted_label == i)
				{
					LCD_2_Position(0,0);
					LCD_2_PrCString("Word Predicted:");
					LCD_2_Position(1,0);
					//LCD_2_PrCString("%s",lookuptable[predicted_label]);
				}
			}
		}
	}
}

BYTE create_word(float lux, INT temp, char *label,INT l1,INT l2,INT t1,INT t2)
{
	enum light light_label;
	enum temp temp_label;
	BYTE bit_stream;
	
	if (lux >= 0 && lux < l1)
		light_label = DARK;
	else if(lux >= l1 && lux < l2)
		light_label = NORMAL;
	else if(lux >=l2 && lux <=1000)
		light_label = BRIGHT;
	
	// Check this
	if (temp < t1)
		temp_label = COLD;
	else if(temp >=t1 && temp < t2)
		temp_label = MEDIUM;
	else if(temp >= t2)
		temp_label = HOT;
	
	if (light_label == DARK && temp_label == COLD)
	{	
		bit_stream = 0xa9;//Bit stream : 1010 1001 | Dark light and Cold condition
		*label = DARK_COLD;
		LCD_2_Position(0,0);
		LCD_2_PrCString("Word Generated is: DARK COLD");
	}
	else if (light_label == DARK && temp_label == MEDIUM)
	{
		bit_stream = 0xaa; //Bit stream : 1010 1010 |  Dark light and Medium temp condition
		*label = DARK_MEDIUM;
		LCD_2_Position(0,0);
		LCD_2_PrCString("Word Generated is: DARK MEDIUM");
	}
	else if (light_label == DARK && temp_label == HOT)
	{
		bit_stream = 0xa8;//Bit stream : 1010 1000 | Dark light and Hot temp condition
		*label = DARK_HOT;
		LCD_2_Position(0,0);
		LCD_2_PrCString("Word Generated is: DARK HOT");
	}
	else if (light_label == NORMAL && temp_label == COLD)
	{
		bit_stream = 0x89; //Bit stream : 1000 1001 | Normal light and cold temp condition
		*label = NORMAL_COLD;
		LCD_2_Position(0,0);
		LCD_2_PrCString("Word Generated is: NORMAL COLD");
	}
	else if (light_label == NORMAL && temp_label == MEDIUM)
	{
		bit_stream = 0x8a; //Bit stream : 1000 1010 | Normal light and medium temp condition
		*label = NORMAL_MEDIUM;
		LCD_2_Position(0,0);
		LCD_2_PrCString("Word Generated is: NORMAL MEDIUM");
	}
	else if (light_label == NORMAL && temp_label == HOT)
	{
		bit_stream = 0x88; //Bit stream : 1000 1000 | Normal light and hot temp condition
		*label = NORMAL_HOT;
		LCD_2_Position(0,0);
		LCD_2_PrCString("Word Generated is: NORMAL HOT");
	}
	else if (light_label == BRIGHT && temp_label == COLD)
	{
		bit_stream = 0x49; //Bit stream : 01001001 | Bright light and cold temp condition
		*label = BRIGHT_COLD;
		LCD_2_Position(0,0);
		LCD_2_PrCString("Word Generated is: BRIGHT COLD");
	}
	else if (light_label == BRIGHT && temp_label == MEDIUM)
	{
		bit_stream = 0x4a; //Bit stream : 01001010 | Bright light and medium temp condition
		*label = BRIGHT_MEDIUM;
		LCD_2_Position(0,0);
		LCD_2_PrCString("Word Generated is: BRIGHT MEDIUM");
	}
	else if (light_label == BRIGHT && temp_label == HOT)
	{
		bit_stream = 0x48; //Bit stream : 01001000 | Bright light and hot temp condition
		*label = BRIGHT_HOT;
		LCD_2_Position(0,0);
		LCD_2_PrCString("Word Generated is: BRIGHT HOT");
	}
	return bit_stream;// return word encoding
}


BOOL update_table(BYTE word_encoding, vocabTable_type* table, char * label)
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
		table[i].label = *label;
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

void remove(BYTE freq_threshold,vocabTable_type* table )
{
	int i;
	for(i= 0; i< SIZE; i++)
	{	
		if(table[i].frequency < freq_threshold)
		{
			// Remove the old word and details and initial with zero
			table[i].word= 0;
			table[i].frequency = 0;
			table[i].age= 0;
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
		// Type(2 bits) | spacer(1 bit) | payload (17 bits) (so in total 20bits of bit stream per work speak operation.
		// One word consists of 8 stamps
		case 0xa9: //1010 1001 4 loud stamp, 4 silent stamp | Dark light and Cold condition 
			PWM8_1_Start();
			blocking_delay(Z_div);//1
			PWM8_1_Stop();
			blocking_delay(Y_div);//0
			PWM8_1_Start();
			blocking_delay(Z_div);//1
			PWM8_1_Stop();
			blocking_delay(Y_div);//0
			PWM8_1_Start();
			blocking_delay(Z_div);//1
			PWM8_1_Stop();
			blocking_delay(2*Y_div);//00
			PWM8_1_Start();
			blocking_delay(Z_div);//1
			PWM8_1_Stop();
			
			/*y = Y_div* 16;             
			blocking_delay(y);*/
			break;;
		case 0xaa:// 1010 1010 4 loud stamp, 4 rest silent stamps |  Dark light and Medium temp condition
			PWM8_1_Start();
			blocking_delay(Z_div);//1
			PWM8_1_Stop(); 
			blocking_delay(Y_div);//0
			PWM8_1_Start();
			blocking_delay(Z_div);//1
			PWM8_1_Stop();
			blocking_delay(Y_div);//0
			PWM8_1_Start();
			blocking_delay(Z_div);//1
			PWM8_1_Stop();
			blocking_delay(Y_div);//0
			PWM8_1_Start();
			blocking_delay(Z_div);//1
			PWM8_1_Stop();
			blocking_delay(Y_div);//0
			/*y = Y_div *14;
			blocking_delay(y);*/
			break;
		case 0xa8:// 1010 1000 3 loud stamp,5 rest silent stamps | Dark light and Hot temp condition
			//To do
			break;
		case 0x89:// 1000 1001  3 loud stamp,5 rest silent stamps |  Normal light and cold temp condition
			//To do
			break;
		case 0x8a:// 1000 1010  3 loud stamp and 5 rest silent stamps | Normal light and medium temp condition
			//To do
			break;
		case 0x88:// 1000 1000 2 loud stamp, 6 rest silent stamps | Normal light and hot temp condition
			//To do
			break;
		case 0x49:// 01001001 3 loud stamp,5 rest silent stamps | Bright light and cold temp condition
			//To do
			break;
		case 0x4a:// 01001010 3 loud stamp,5 rest silent stamps | Bright light and medium temp condition
			break;
		case 0x48:// 0100 1000 2 loud stamp, 6 rest silent stamps | | Bright light and hot temp condition
			//To do
			break;
	}
}

BYTE listen(void)
{
	int iResult1,iResult2,avg_mic_output,count,total;
	INT start_time =wElapsedTime;
	INT bit_start_time;
	INT X_time_elaspsed = 0;
	INT ZY_time_elaspsed = 0;
	BYTE bit_stream = 0;
	// Listen for a word duration to heat the bit stream sent by the other agent
	while (X_time_elaspsed < X)
	{
		// Listen for lound or silen stamp for the Y = Z = 200ms duration
		avg_mic_output = 0;
		count = 0;
		total = 0;
		while ((ZY_time_elaspsed < Z || ZY_time_elaspsed < Y))
		{
			ZY_time_elaspsed = wElapsedTime - bit_start_time;
			
			while(DUALADC_1_fIsDataAvailable() == 0);  // Wait for data to be ready
        
	 		iResult1 = DUALADC_1_iGetData1();          // Get Data from ADC Input1
	  		iResult2 = DUALADC_1_iGetData2();
	
	  		DUALADC_1_iGetData1ClearFlag();
      		DUALADC_1_iGetData2ClearFlag();
			count++;
			total = total + iResult1;
			avg_mic_output = total/count;
		}
		// Detected loud stamp
		if (avg_mic_output> LOUD_STAMP_THRESHOLD)
		{
			bit_stream = bit_stream | 0x1;			
		}
		// Detected silent stamp
		else
		{
			bit_stream = bit_stream | 0x0;
		}
		//shift the bit stream for the next bit value
		bit_stream = bit_stream <<1; 
		
		X_time_elaspsed = wElapsedTime - start_time;
		
		bit_start_time = wElapsedTime; 
		ZY_time_elaspsed = 0;
	}
	
	return bit_stream;
}

// Check if word label is present in neighbours for the current temp&/light condition
BOOL check_if_labelPresent(char* index)
{
	float luxValue; // Ambient Light illumiance value
	INT   tempValue;     // Ambient temperature
	char * label;
	BOOL present =FALSE;
	int i;
	// Sense /////////////////////////////
		
	//Read light sensor value
	luxValue   = getLux(); // luxValue ranges from 0 to 1000
	//Read temperature value
	tempValue  = read_temperature();
	
	//Create a word label based on the sensor values	
	create_word(luxValue, tempValue,label,lux_range_1,lux_range_2,temp_range_1,temp_range_2);
	
	// Check if this label is present in neighbour's table
	for (i= 0; i< SIZE; i++)
	{
		if (neighours_table[i].label == *label)
		{
			present =TRUE;
			*index = i;
		}
	}
	return present;
}
BOOL check_if_bit_stream_match(char index,BYTE word_listened)
{
	if(neighours_table[index].word == word_listened)
		return TRUE;
	else 
		return FALSE;
}

void re_learn_ranges(BYTE word_listened)
{
	float luxValue; // Ambient Light illumiance value
	INT   tempValue;
	char *label;
	//Read light sensor value
	luxValue   = getLux(); // luxValue ranges from 0 to 1000
	//Read temperature value
	tempValue  = read_temperature();
	
	//check if the received word indicates temperature only condition or light only condition or both temperature&light condition
	if ((word_listened & MS_NIBBLE_MASK != 0) && (word_listened & LS_NIBBLE_MASK != 0))
	{	
		// generate the bit stream for each different combinations of range values, the find the new range that will
		// generate the bit stream that matches with the received bit stream
		if (create_word(luxValue, tempValue,label,luxValue,lux_range_2,temp_range_1,temp_range_2) == word_listened)
		{
			lux_range_1 = luxValue;
		}
		else if (create_word(luxValue, tempValue,label,lux_range_1,luxValue,temp_range_1,temp_range_2) == word_listened)
		{
			lux_range_2 = luxValue;
		}
		else if (create_word(luxValue, tempValue,label,lux_range_1,lux_range_2,tempValue,temp_range_2) == word_listened)
		{
			temp_range_1 = tempValue;
		}
		else if (create_word(luxValue, tempValue,label,lux_range_1,lux_range_2,temp_range_1,tempValue) == word_listened)
		{
			temp_range_2 = tempValue;
		}
	}
	else if((word_listened & MS_NIBBLE_MASK ==0) && (word_listened & LS_NIBBLE_MASK != 0))
	{
		//relearn only temperature ranges
		if (create_word(luxValue, tempValue,label,lux_range_1,lux_range_2,tempValue,temp_range_2) == word_listened)
		{
			temp_range_1 = tempValue;
		}
		else if (create_word(luxValue, tempValue,label,lux_range_1,lux_range_2,temp_range_1,tempValue) == word_listened)
		{
			temp_range_2 = tempValue;
		}
	}
	else if ((word_listened & MS_NIBBLE_MASK !=0 ) && (word_listened & LS_NIBBLE_MASK == 0))
	{
		//relearn only light ranges
		if (create_word(luxValue, tempValue,label,luxValue,lux_range_2,temp_range_1,temp_range_2) == word_listened)
		{
			lux_range_1 = luxValue;
		}
		else if (create_word(luxValue, tempValue,label,lux_range_1,luxValue,temp_range_1,temp_range_2) == word_listened)
		{
			lux_range_2 = luxValue;
		}
	}
}
void update_frequency_table(BYTE word_encoding)
{
	int i;
	BOOL found = FALSE;
	char * label;
	
	float luxValue; // Ambient Light illumiance value
	INT   tempValue;
	char *label;
	//Read light sensor value
	luxValue   = getLux(); // luxValue ranges from 0 to 1000
	//Read temperature value
	tempValue  = read_temperature();
	
	//Create a word label based on the sensor values	
	create_word(luxValue, tempValue,label,lux_range_1,lux_range_2,temp_range_1,temp_range_2);
	
	for (i= 0; i< SIZE; i++)
	{
		table[i].age++;
		if (bayesian_learn_table[i].word == word_encoding)
		{
			found =TRUE;
			table[i].frequency++;
		}
	}
	if (found == FALSE)
	{	
		table[i].word = word_encoding;
		table[i].frequency = 1;
		table[i].label = *label;
		table[i].frequency_of_mismatch = 0;
	}
	return found;			
}

BYTE highest_conditional_prob(BYTE word_listened)
{
	INT highest_c_prob=0;
	INT p1=0;
	int index=0;
	for (i= 0; i< SIZE; i++)
	total = total + bayesian_learn_table[i].frequency;
	
	for (i= 0; i< SIZE; i++)
	{	
		if (bayesian_learn_table[i].label == word_listened && bayesian_learn_table[i].label == label )
		{
			p1 = bayesian_learn_table[i].frequency/total;
		}
		if (bayesian_learn_table[i].label == word_listened)
		{
			p2 = bayesian_learn_table[i].frequency/total;
		}
		temp  = p1*p2;
		if (temp > highest_c_prob)
		{
			highest_c_prob = temp;
			index = i;
		}
	}
	return bayesian_learn_table[index].label;
}