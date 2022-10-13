//----------------------------------------------------------------------------
// C main line
//----------------------------------------------------------------------------

#include <m8c.h>        // part specific constants and macros
#include "PSoCAPI.h"    // PSoC API definitions for all User Modules

#define ADDRESS_TEMP_SENSOR 0x18
#define READ_BYTE 			0x01
#define WRITE_BYTE          0x00
#define TEMP_REG    		0x05

// Function prototype
void read_temp_and_print(void);



void main(void)
{
	// M8C_EnableGInt ; // Uncomment this line to enable Global Interrupts
	// Insert your main routine code here.
	
	while (1)
	{
		read_temp_and_print();	
	}
}

void read_temp_and_print(void)
{
	BYTE UpperByte,LowerByte;
	INT Temperature;
	
	I2CHW_Temp_fSendStart(ADDRESS_TEMP_SENSOR, I2CHW_Temp_WRITE);
	I2CHW_Temp_fWrite(TEMP_REG);
	I2CHW_Temp_SendStop();
	
	I2CHW_Temp_fSendStart(ADDRESS_TEMP_SENSOR, I2CHW_Temp_READ);
	UpperByte = I2CHW_Temp_bRead(I2CHW_Temp_ACKslave);
	LowerByte = I2CHW_Temp_bRead(I2CHW_Temp_NAKslave);
	I2CHW_Temp_SendStop();
	
	//First Check flag bits
	if ((UpperByte & 0x80) == 0x80){ //TA ≥ TCRIT
	}
	if ((UpperByte & 0x40) == 0x40){ //TA > TUPPER
	}
	if ((UpperByte & 0x20) == 0x20){ //TA < TLOWER
	}
	UpperByte = UpperByte & 0x1F; //Clear flag bits
	if ((UpperByte & 0x10) == 0x10){ //TA < 0°C
	UpperByte = UpperByte & 0x0F;//Clear SIGN
	Temperature = 256 - (UpperByte * 16 + LowerByte / 16);
	}else //TA ≥ 0°C
	
	Temperature = (UpperByte * 16 + LowerByte / 16);//Temperature = Ambient Temperature (°C)
	
	
	
	
}
