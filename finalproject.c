/*----------------------University of Houston--------------------|
				     ELET 3325 Embedded Systems  
				   "Temperature Monitoring System"
				             Fall 2008
				             
				                 By
				                 
				          Edwin O. Gonzales
				           Nivedita Rakhit  

The user must enter a password using the keypad in order to access the program. 
Once the password is entered, the system will determine if the password is correct or incorrect. 
If the password is incorrect, the user is prompted to try again. If the password is correct, 
then the program starts. Once the program is activated, options to select Fahrenheit or Celsius 
scales are offered. The user will be prompted for the desired temperature scale. After selecting 
the desired scale, the user would be able to enter temperature thresholds. All real time 
temperatures must be displayed on the LCD. If the measured temperature is below the lowest 
threshold number, a yellow LED turns on. If the current temperature is above the highest 
threshold number, a red LED turns on. And finally, if the current temperature is between the 
two threshold numbers, the green LED light turns on. Also, the user should be able to enter a 
code in order to activate two motors. These motors will turn in different directions according 
to the temperature readings shown on the LCD. If the temperature is under the threshold, the 
motors will move right or left. If the temperature is in between, then the motors will turn right. 
If the temperature is above the threshold, then the motors will turn left. The program should 
generate a threshold (M) which is in the middle between the upper and the lower threshold. 
The motor turns right if temperature is between the upper threshold and M, it turns left if 
temperature is between M and the lower threshold.*/

//------------------------------------------------------------------------------------------//

//--- Start of Program ------//

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


void main()
{
	
	LCD_Init();//------------------------------------ Initialize LCD
	LCD_WriteCtrl(0x02);//--------------------------- Setting the top line 
	LCD_printf( "  Temperature Monitoring ");//------ Text to identify equipment
	LCD_WriteCtrl(0xC0);//--------------------------- Setting the bottom line
	LCD_printf( "         System   ");//------------- Text to identify equipment
	delay(5000);
	LCD_WriteCtrl(0x01);//--------------------------- Clear screen
	Contrasena();//---------------------------------- Contrasena means password in spanish. To password function

}

//1/4
