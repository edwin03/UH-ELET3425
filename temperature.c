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
                                                                           
#define MAX_ROWS	4 //------------------------ settings for keypad
#define MAX_COLS	4 //------------------------ settings for keypad
#define PIN "111111" //------------------------- setting password

#define ADCCS 	P3.7
#define ADCCLK	P1.6
#define ADCDIN	P1.4
#define ADCDOUT	P1

static char KeyTable[] = { 	'1', '2', '3', 'A',
							'4', '5', '6', 'B',	 
							'7', '8', '9', 'C',
							'*', '0', '#', 'D'
						 };
						
static unsigned char RowTable[] = { 0xFE, 0xFD, 0xFB, 0xF7 };
void Reloj();//--------------------------------------- Declaring function for clock
void limites();//------------------------------------- Declaring function for threshold
void LED_Fah();//------------------------------------- Declaring function for LED control while in Farenheit mode
void LED_Fah2();//------------------------------------ Declaring function for LED control with motor
void LED_Cel();//------------------------------------- Declaring function for LED control while in Celsius mode
void LED_Cel2();//------------------------------------ Declaring function for LED control with motor
void Temperaturas();//-------------------------------- Declaring function for threshold input display
void Motor_adelante();//------------------------------ Declaring function for motor functionalilty (forward)
void Motor_atras();//--------------------------------- Declaring function for motor functionality (backwardds)
void Motor_ambos();//--------------------------------- Declaring function for motor functionality (both motors)
void Prender1();//------------------------------------ Declaring function for motor on with celsius
void Prender2();//------------------------------------ Declaring function for motor on with fahr
void motor_off();//----------------------------------- Declaring function for option to turn-off motor
void motor_off2();//---------------------------------- Declaring function for dissabling motor (motor off)

unsigned int Sensor_celsius( unsigned int channel);
unsigned int Sensor_fahre( unsigned int channel);
char array[3];
char narray[3];
char ScanKeypad();
char key5, key4, key3, key2, key1, choice;
int fah, templ, temph;
int i, l, n, c, value;

int ltemp[2];
int z;
int utemp[2];
int uTenTemp, lTenTemp, lowtemp, uptemp, temperature;

void temp()
{
	LCD_Init();//--------------------------------- LCD initialize
	LCD_WriteCtrl(0x02);//------------------------ Set Top Line 
	LCD_printf( "Press 'C' for celsius"); //------ Display a test message
	LCD_WriteCtrl(0xC0);//------------------------ set bottom line
	LCD_printf("Press 'D' for Fahrenheit :");//--- Display a test message
	 
	for( ;; )//----------------------------------- Check for input (C or D)
	{	
		key1 = ScanKeypad();//-------------------- Call KeyPad Scanning Function
		if( key1 )
		{
			if ( key1 == '#') 
			break;
			else if(key1 != '#')
			{
				choice=key1;
				LCD_printf( "%c", key1 );
			}
		}
	}
	 
	if (choice == 'C' )//------------------------- if C is pressed, do this
	{
		Sensor_celsius();//----------------------- reading from censor in celsius
		lowtemp=16;//----------------------------- predefined threshold
		uptemp=25;//------------------------------ predefined threshold
		for(;;)
		{
	
		LCD_WriteCtrl(0x02);
		LCD_printf("Degree Celsius");//----------- Display the temperature scale
		LCD_WriteCtrl(0xC0);			
		LCD_printf("Value %d", Sensor_celsius());//------ Displays the real time temperature
		delay(1000);
		LCD_WriteCtrl(0x01);
		LED_Cel();//------------------------------ Call LED in celsius function
		key2 = ScanKeypad();//-------------------- Call KeyPad Scanning Function
			if (key2 == '*') break;
			else if ( key2 == '#')
			{
				limites();//---------------------- Calling the threshold function
				Temperaturas();//----------------- Calling the temperature function			 
				Prender1();
		   } 
		}
	}	
	 
	else if(choice == 'D' )//--------------------- If D is pressed, do this
	{
		Sensor_fahre();//----------------------------- Calling function to convert to fahrenheit
		lowtemp=60;//----------------------------- expected low limit
		uptemp=80;//------------------------------ expected upper limit
		for(;;)
		{	
		LCD_WriteCtrl(0x02);
		LCD_printf("Degree Fahrenheit");//-------- Display the temperature scale
		LCD_WriteCtrl(0xC0);
		LCD_printf("Value %d", Sensor_fahre());//----- Display the real time temperature
		delay(1000);
		LCD_WriteCtrl(0x01);
		LED_Fah();//------------------------------ Calling LED function
		key2 = ScanKeypad();//-------------------- Call KeyPad Scanning Function
			if (key2 == '*') break;
			else if ( key2 == '#')
			{	
				limites();//---------------------- Calling the threshold function
				Temperaturas();//----------------- Calling the temperature function
				Prender2();
			}
		}
	}
	else if ( choice != 'C' || choice != 'D' )//-- if neither C or D are pressed, start over
	{	
		temp();//--------------------------------- back to top
	}
	return 0;
}


void limites()//---------------------------------- Upper threshold function
{
	IE=0x81;
	LCD_Init();//--------------------------------- initialize LCD
	LCD_WriteCtrl(0x02);
	LCD_printf( "Enter upper threshold:");//------ question user to enter upper threshold
	z=0;
	for( ;; )
	{			
		key2 = ScanKeypad();//-------------------- Call keypad scanning function
		if( key2 )//------------------------------ Start scanning
		{
			if ( key2 == '#')//-------------------  
			break;
			else if(key2 != '#')
			{
				utemp[z] = key2-48;
				LCD_printf("%d", utemp[z]);
				z++;
			}
		}
	}
	
	LCD_WriteCtrl(0xC0);
	LCD_printf( "Enter lower threshold:");//------ question user to enter lower threshold
	n=0;
	key3=0;
	for( ;; )
	{		
		key3 = ScanKeypad();//-------------------- call keypad scanning function
		if( key3 )
		{
			if (key3 == '#') 
			break;
			else if(key3 != '#')
			{
				ltemp[n] = key3-48;
				LCD_printf("%d", ltemp[n]);
				n++;
			}
		}
	}
}

void LED_Fah()
{
	temperature=Sensor_fahre();
	if (temperature > uptemp)
	{
		P1 = 0xFF;
		clrbit(P1.3);	
	}	
	else if (temperature < lowtemp)
	{
	P1 = 0xFF;
	clrbit(P1.1);
	}
	else if (temperature <= uptemp && temperature >= lowtemp)
	{
	P1 = 0xFF;
	clrbit(P1.0);
	}
	
}
void LED_Fah2()
{
	temperature=Sensor_fahre();
	if (temperature > uptemp)
	{
		P1 = 0xFF;	
		clrbit(P1.3);
		Motor_atras();//---------------------------- Move motor back function	
	}	
	else if (temperature < lowtemp)
	{
	P1 = 0xFF;

	clrbit(P1.1);
	Motor_ambos();//-------------------------------- move both motor function
	}
	else if (temperature <= uptemp && temperature >= lowtemp)
	{
	P1 = 0xFF;
	clrbit(P1.0);
	Motor_adelante();//----------------------------- move motor foward function
	}
	
}
void LED_Cel()
{
	temperature=Sensor_celsius();
	if (temperature > uptemp)
	{
		P1 = 0xFF;
		clrbit(P1.3);	
	}	
	else if (temperature < lowtemp)
	{
		P1 = 0xFF;
		clrbit(P1.1);
	}
	else if (temperature <= uptemp && temperature >= lowtemp)
	{
		P1 = 0xFF;
		clrbit(P1.0);
	}
	
}
void LED_Cel2()
{
	temperature=Sensor_celsius();
	if (temperature > uptemp)
	{
		P1 = 0xFF;
		clrbit(P1.3);
		
		Motor_atras();//------------------------------ Move motor back function	
	}	
	else if (temperature < lowtemp)
	{
		P1 = 0xFF;
		clrbit(P1.1);
		Motor_ambos();//------------------------------ move both motor function
	}
	else if (temperature <= uptemp && temperature >= lowtemp)
	{
		P1 = 0xFF;
		clrbit(P1.0);
		Motor_adelante();//--------------------------- move motor foward function
	}
	
}
void Temperaturas()
{
	uTenTemp = utemp[0] * 10;
	uptemp = (uTenTemp + utemp[1]);
	lTenTemp = ltemp[0] * 10;
	lowtemp = (lTenTemp + ltemp[1]);

	LCD_WriteCtrl(0x01);
	LCD_WriteCtrl(0x02);
	LCD_printf("Upper Threshold:  %d", uptemp);
	LCD_WriteCtrl(0xC0);
	LCD_printf("Lower Threshold:  %d", lowtemp);
	delay(5000);	
}
unsigned int Sensor_celsius( unsigned int channel)//-----function to read from the sensor in fahre
{
	/* Reset the A/D converter */
	clrbit(ADCCLK);
	setbit(ADCCS);
	setbit(ADCDIN);
	
	/* Start Conversion */
	clrbit(ADCCS);
	delay(1);
	
	/* Start Bit */
	
	setbit(ADCDIN);
	Reloj();
	
	/* SGL */
	setbit(ADCDIN);
	Reloj();
	
	/* ODD */
	clrbit(ADCDIN);
	Reloj();
	
	/* Select */
	clrbit(ADCDIN);
	Reloj();
	
	delay(1);
	Reloj();
	value=0;
	setbit(ADCDIN);
	
	for(i=0;i<8;i++)
	{
		if(ADCDOUT & 0x10)
			value |=1;			
		   Reloj();
		   	
		if(i<7)
			value= value <<1;
	}
	return value;
  }

unsigned int Sensor_fahre( unsigned int channel)//-------------------function to read from the sensor in fahre
{
	/* Reset the A/D converter */
	clrbit(ADCCLK);
	setbit(ADCCS);
	setbit(ADCDIN);
	
	/* Start Conversion */
	clrbit(ADCCS);
	delay(1);
	
	/* Start Bit */
	
	setbit(ADCDIN);
	Reloj();
	
	/* SGL */
	setbit(ADCDIN);
	Reloj();
	
	/* ODD */
	clrbit(ADCDIN);
	Reloj();
	
	/* Select */
	clrbit(ADCDIN);
	Reloj();
	
	delay(1);
	Reloj();
	value=0;
	setbit(ADCDIN);
	
	for(i=0;i<8;i++)
	{
		if(ADCDOUT & 0x10)
			value |=1;
			
		Reloj();//---------------------------------- calling clock function
		
		if(i<7)
			value= value <<1;
	}
	fah = ((9*value)/5)+32;//------------------------ math to convert celsius to fahren
	return fah;
}
void Reloj()//--------------------------------------- clock function
{
	setbit(ADCCLK);
	setbit(ADCCLK);
	clrbit(ADCCLK);
	
}
void Motor_adelante()//------------------------------- Driver that moves motor foward
{
	setbit(P3.0);//----------------------------------- Enable motor 1
	setbit(P3.3);//----------------------------------- Enable motor 2
	clrbit(P3.1);//----------------------------------- Motor 1 foward
	setbit(P3.2);	
	setbit(P3.4);//----------------------------------- Motor 2 foward
	clrbit(P3.5);
}

void Motor_atras()//---------------------------------- Driver moves motor backwards
{
	setbit(P3.0);	//-------------------------------- Enable motor 1
	setbit(P3.3);	//-------------------------------- Enable motor 2
	setbit(P3.1);   //-------------------------------- Motor 1 backward
	clrbit(P3.2);	
	clrbit(P3.4);	//-------------------------------- Motor 2 backward
	setbit(P3.5);
}

void Motor_ambos()		//Driver moves both motors
{
	setbit(P3.0);	//-------------------------------- Enable motor 1
	setbit(P3.3);	//-------------------------------- Enable motor 2
	clrbit(P3.1);   //-------------------------------- Motor 1 foward
	setbit(P3.2);	
	clrbit(P3.4);	//-------------------------------- Motor 2 backward
	setbit(P3.5);

}
void Prender2()
{
	for(;;)
		{	
		LCD_WriteCtrl(0x02);
		LCD_printf("Degree Fahrenheit");//-------- Display the temperature scale
		LCD_WriteCtrl(0xC0);
		LCD_printf("Value %d", Sensor_fahre());//-- Display the real time temperature
		delay(1000);
		LCD_WriteCtrl(0x01);
		LED_Fah2();//----------------------------- Calling LED function
		key2 = ScanKeypad();//-------------------- Call KeyPad Scanning Function
			if (key2 == '1')
				{	 
				motor_off();//-------------------- Calling the option to turn off monitor
				}
			else if ( key2 == '*')	
			 break;
			else if( key2 == '#')
			{	
				limites();//---------------------- Calling the threshold function
				Temperaturas();//----------------- Calling the temperature function
			}
		}
}
void Prender1()
{
	for(;;)
		{
	
		LCD_WriteCtrl(0x02);
		LCD_printf("Degree Celsius");//----------- Display the temperature scale
		LCD_WriteCtrl(0xC0);			
		LCD_printf("Value %d", Sensor_celsius());//Displays the real time temperature
		delay(1000);
		LCD_WriteCtrl(0x01);
		LED_Cel2();//----------------------------- Call LED in celsius function
		key2 = ScanKeypad();//-------------------- Call KeyPad Scanning Function
			if (key2 == '1')
				{	 
				motor_off();//-------------------- Calling the option to turn off monitor
				}
			else if ( key2 == '*')	
			 break;
			else if( key2 == '#')
			{	
				limites();//---------------------- Calling the threshold function
				Temperaturas();//----------------- Calling the temperature function
			}
		}
}
void motor_off()//-------------------------------- "Turn off motor option" function
{
	LCD_Init();//--------------------------------- LCD initialize
	LCD_WriteCtrl(0x02);//------------------------ Set Top Line 
	LCD_printf( "Please select 'B' to"); //------ Display a test message
	LCD_WriteCtrl(0xC0);//------------------------ set bottom line
	LCD_printf("turn off motors");//-------------- Display a test message
	 
	for( ;; ) 
	{	
		key5 = ScanKeypad();//-------------------- Call KeyPad Scanning Function
		if( key5 )
		{
			if ( key5 == 'B') 
			{
		 	motor_off2();//----------------------- calling function to turn off monitors
		 	}
			else if(key5 != 'B')
			{
				temp();
			}
		}
	}
}	

void motor_off2()//------------------------------ Function to turn off monitors
{
    clrbit(P3.0);	 
	clrbit(P3.3);	 
}

//4/4