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

int pwlength, keywrong, k;
char Llave;
char keyarray[6];

void Contrasena()
{
LCD_Init();          
	for(;;)
	{	
		LCD_WriteCtrl(0x02);//------------------- Set Top Line 
		LCD_printf( "Input password :");//----------- Display a test message
		pwlength = 0;//-------------------------- password length sentinel	
		for( ;; )
		{		
			Llave = ScanKeypad();//-------------- Call KeyPad Scanning Function
			if( Llave )
			{
				if ( Llave == '#') break;//------ breaks loop if # is pressed
				else if(Llave != '#')			
				{
					keyarray[pwlength] = Llave;//-- user input password is put in array
					LCD_printf( "*", Llave );//-- * is displayed when a key is pressed
					pwlength++;					
				}
			}
		}
		
	if(pwlength==6)//---------------------------- passsword length is compared to see if enought characters are pressed
	{	
		keywrong=0;
		for(k=0;k<6;k++)//----------------------- this for loop compares every input in array to the predefined password
		{
			if(keyarray[k] != PIN[k])//---------- if password is different then a flag is made
			{
				keywrong=1;					
			}
		}
		if(keywrong==1)//------------------------ if flag is raised incorrect pin is displayed
		{
			LCD_WriteCtrl(0xC0);
			LCD_printf("Incorrect password");
			delay(5000);
			LCD_Init();//------------------------ Initialize LCD
		}
		else//----------------------------------- if flag is not raised Correct pin is displayed
		{
			LCD_WriteCtrl(0xC0);
			LCD_printf("WELCOME & ENJOY !");
			delay(5000);
			LCD_Init();
			temp();//---------------------------- then the function temp() is called up
		}
	}
	
	else if(pwlength!=6)//----------------------- if the password is not equal to 6 character incorrect pin is displayed
	{
			LCD_WriteCtrl(0xC0);
			LCD_printf("Incorrect password");
			delay(5000);
			LCD_Init();
	}

	
	}

}

//2/4