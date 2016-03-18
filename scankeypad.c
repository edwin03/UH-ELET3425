#include <8051io.h>
#include <8051bit.h>            /* Bit set/clear macros */
#include <8051reg.h>

#include <BipomLib\8051\MEDIUM\INCLUDE\typesm.h>
#include <BipomLib\8051\MEDIUM\INCLUDE\DAQ2543m.h>
#include <BipomLib\8051\MEDIUM\INCLUDE\DAQ128m.h>
#include <BipomLib\8051\MEDIUM\INCLUDE\rtcm.h>
#include <BipomLib\8051\MEDIUM\INCLUDE\relaym.h>
#include <BipomLib\8051\MEDIUM\INCLUDE\lcdm.h>
#include <8051int.h>

#define I2C_PCF8574_0         0		//Address (0..7) of expander on the bus ( JP1,JP2 and JP3 connected )
                                                                           
#define MAX_ROWS	4 //------------------------- settings for keypad
#define MAX_COLS	4 //------------------------- settings for keypad
#define PIN "111111"  //------------------------- This will set the password


static char KeyTable[] = { 	'1', '2', '3', 'A',
							'4', '5', '6', 'B',	 
							'7', '8', '9', 'C',
							'*', '0', '#', 'D'
						 };
						
static unsigned char RowTable[] = { 0xFE, 0xFD, 0xFB, 0xF7 };
char ScanKeypad();

char ScanKeypad()
{
	char row;
	char col;
	
	col = 0;  //  current column 
	
	for( row=0; row<MAX_ROWS; row++ )
	{
		P2 = RowTable[row];
		
		if( !(P2 & 0x80) )col = 4;
		if( !(P2 & 0x40) )col = 3;
		if( !(P2 & 0x20) )col = 2;
		if( !(P2 & 0x10) )col = 1;
		if( col != 0 )		
		{
			delay(750);
			return KeyTable[col-1 + row*MAX_COLS];
		}					
	}
	
	return 0;
}
//3/4