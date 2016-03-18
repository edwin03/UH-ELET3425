// This project contains sample code for interfacing with many of the BiPOM
// peripheral boards. Modify as needed for your project :)

#include <8051io.h>
#include <8051bit.h>            /* Bit set/clear macros */
#include <8051reg.h>

#include <BipomLib\include\types.h>
#include <BipomLib\daq2543\DAQ2543m.h>
#include <BipomLib\daq128\DAQ128m.h>
#include <BipomLib\rtc\include\rtcm.h>
#include <BipomLib\relay\relaym.h>
#include <BipomLib\lcd\lcdm.h>

#define I2C_PCF8574_0         0		//Address (0..7) of expander on the bus ( JP1,JP2 and JP3 connected )
                                                                           
#define MAX_ROWS	4
#define MAX_COLS	4


#define PIN "3425"  //------------------------- This will set the password
#define ADCCS           P3.7
#define ADCCLK          P1.6
#define ADCDIN          P1.4
#define ADCDOUT         P1		/* P1.4 */


static char KeyTable[] = { 	'1', '2', '3', 'A',
				'4', '5', '6', 'B',	 
				'7', '8', '9', 'C',
				'*', '0', '#', 'D'
			};
						
static unsigned char RowTable[] = { 0xFE, 0xFD, 0xFB, 0xF7 };
char ScanKeypad();
void Contrasena();
void lcdstart(); //----------------Main Menu
char lcdselection(); //------------Selection Input
void selection(); //---------------Selection Menu
int selectionmade(char x); //-----Implements Selection Choice
void implement(char i);
void Clock();
unsigned int Convert( unsigned int channel);
unsigned int Convert1( unsigned int channel);
void temp();
char manualmodesel(char x);
void manualsel();
void manualaction();
void postmanualmove(char z);
void aimplement(int i);
void automatic();
void dtemp();
void tempconv();
void LED_Fah();
void motoron ();
void motoroff ();
void servoboot();

int value, i, farenheit, t;
int pwlength, keywrong, k;
char Llave, status, light;
char keyarray[6];
char s, cont, tempc, h;
int utemp[2], uTenTemp, uptemp, temperature, brightness; 

int main ()
{

servoboot();
lcdstart();
lcdselection();
temp();
return 0;
}

void Contrasena()
{
    LCD_WriteCtrl(0x01);     
	for(;;)
	{	
		LCD_WriteCtrl(0x02);//------------------- Set Top Line 
		LCD_printf( "Input PIN :");//----------- Display a test message
		pwlength = 0;//-------------------------- password length sentinel	
		for( ;; )
		{		
			Llave = ScanKeypad();//-------------- Call KeyPad Scanning Function
			if( Llave )
			{
				if ( Llave == '#') 
				{
				break;//------ breaks loop if # is pressed
				}
				else if(Llave != '#')			
				{
					keyarray[pwlength] = Llave;//-- user input password is put in array
					LCD_printf( "*", Llave );//-- * is displayed when a key is pressed
					pwlength++;					
				}
			}
		}
		
	if(pwlength==4)//---------------------------- passsword length is compared to see if enought characters are pressed
	{	
		keywrong=0;
		for(k=0;k<4;k++)//----------------------- this for loop compares every input in array to the predefined password
		{
			if(keyarray[k] != PIN[k])//---------- if password is different then a flag is made
			{
				keywrong=1;					
			}
		}
		if(keywrong==1)//------------------------ if flag is raised incorrect pin is displayed
		{
			LCD_WriteCtrl(0xC0);
			LCD_printf("Incorrect PIN");
			delay(5000);
			LCD_Init();//------------------------ Initialize LCD
		}
		else//----------------------------------- if flag is not raised Correct pin is displayed
		{
			LCD_WriteCtrl(0xC0);
			LCD_printf("PIN Accepted");
			delay(5000);
			LCD_Init();
			break;
			
			
		}
	}
	
	else if(pwlength!=4)//----------------------- if the password is not equal to 6 character incorrect pin is displayed
	{
			LCD_WriteCtrl(0xC0);
			LCD_printf("Incorrect PIN");
			delay(5000);
			LCD_Init();
	}

	
	}

}

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
			delay(500);
			return KeyTable[col-1 + row*MAX_COLS];
		}					
	}
	
	return 0;
}

void lcdstart()
{
	LCD_WriteCtrl(0x01);//--------------------------- Clear screen
	LCD_WriteCtrl(0x02);//--------------------------- Setting the top line 
	LCD_printf( "  THE SMART WINDOW   ");//------ Text to identify equipment
	LCD_WriteCtrl(0xC0);//--------------------------- Setting the bottom line
	LCD_printf( "       SYSTEM   ");//------------- Text to identify equipment
	delay(10000);
	LCD_WriteCtrl(0x01);//--------------------------- Clear screen
	Contrasena();//----------------
}

char lcdselection()
{
	
	selection();
	s = 0;
	for(;;)
	{
	s = ScanKeypad();
	if (s =='1' || s == '2')
	{
	selectionmade(s);
	break;
	}
	if (s == '3' || s == '4' || s == '5' || s == '6' || s == '7' || s == '8' || s == '9')
	{ 
	LCD_WriteCtrl(0x01);//--------------------------- Clear screen
	LCD_WriteCtrl(0x02);//--------------------------- Setting the top line 
	LCD_printf( "Try Again");//------ Text to identify equipment
	delay(5000);
	selection();
	}
	}
}

void selection()
{
	LCD_WriteCtrl(0x01);//--------------------------- Clear screen
	LCD_WriteCtrl(0x02);//--------------------------- Setting the top line 
	LCD_printf( "Press 1 for Manual  ");//------ Text to identify equipment
	LCD_WriteCtrl(0xC0);//--------------------------- Setting the bottom line
	LCD_printf( "Press 2 for Automatic");//------------- Text to identify equipment
}


int selectionmade(char x)
{
	int a;
	switch (x)
	{
		case '1':
		delay(100);
		a = 1;
		implement(a);
		break;
		
		case '2':
		delay(100);
		a = 2;
		implement(a);
		break;
	}
	
}

void implement(int i)
{
	
	switch (i)
	{
		case 1:
		manualmodesel();
		break;
		
		case 2:
		automatic();
		break;
		
	}

}



void temp()
{
    unsigned i;
    serinit(9600);

loop:
	
	
	
    while(!chkch()) 
    {        
    	for( i=0; i<4; i++ )
    	{
    		
    		LCD_WriteCtrl(0x02);
			LCD_printf( "     Farenheit: %d",Convert());
			LCD_WriteCtrl(0xC0);
			LCD_printf( "  Press # to Continue");
			cont = ScanKeypad();
			if (cont == '#')
			{
			lcdselection();
			}
			
		}
	}							
	
	goto loop;
}

unsigned int Convert( unsigned int channel)//-------------------function to read from the sensor in fahre
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
	Clock();
	
	/* SGL */
	setbit(ADCDIN);
	Clock();
	
	/* ODD */
	clrbit(ADCDIN);
	Clock();
	
	/* Select */
	clrbit(ADCDIN);
	Clock();
	
	delay(1);
	Clock();
	value=0;
	setbit(ADCDIN);
	
	for(i=0;i<8;i++)
	{
		if(ADCDOUT & 0x10)
			value |=1;
			
		Clock();//---------------------------------- calling clock function
		
		if(i<7)
			value= value <<1;
	}
	farenheit = ((9*value)/5)+32;//------------------------ math to convert celsius to fahren
	return farenheit;

}

unsigned int Convert1( unsigned int channel)
{
	unsigned int value;
	unsigned char i;
	
/* Reset the A/D converter */
	clrbit(ADCCLK);
	setbit(ADCCS);
	setbit(ADCDIN);
	
	/* Start Conversion */
	clrbit(ADCCS);
	delay(1);
	
	/* Start Bit */
	
	setbit(ADCDIN);
	Clock();
	
	/* SGL */
	setbit(ADCDIN);
	Clock();
	
	/* ODD */
	clrbit(ADCDIN);
	Clock();
	
	/* Select */
	setbit(ADCDIN);
	Clock();
	
	delay(1);
	Clock();
	value=0;
	setbit(ADCDIN);
	
	for(i=0;i<8;i++)
	{
		if(ADCDOUT & 0x10)
			value |=1;
			
		Clock();//---------------------------------- calling clock function
		
		if(i<7)
			value= value <<1;
	}
	
	brightness = value;
	
	if (brightness < 240)
	{
	
		status = 'H';
		return 	status;
	}
	if (brightness > 240)
	{
		status = 'L';
		return status;
	}
}


void Clock()
{
	setbit(ADCCLK);
	setbit(ADCCLK);		/* A little delay */
	clrbit(ADCCLK);
}

void manualsel()
{
	LCD_WriteCtrl(0x01);//--------------------------- Clear screen
	LCD_WriteCtrl(0x02);//--------------------------- Setting the top line 
	LCD_printf( "Manual Mode  ");//------ Text to identify equipment
	LCD_WriteCtrl(0xC0);//--------------------------- Setting the bottom line
	LCD_printf( "1)100 2)75 3)50 4)25 5)0");//------------- Text to identify equipment
}

char manualmodesel()
{
	char a;
	manualsel();
	for(;;)
	{
	a = ScanKeypad();
	if (a =='1' || a == '2' || a == '3' || a == '4' || a == '5' )
	{
	postmanualmove(a);
	break;
	}
	if (a == '3' || a == '4' || a == '5' || a == '6' || a == '7' || a == '8' || a == '9')
	{ 
	LCD_WriteCtrl(0x01);//--------------------------- Clear screen
	LCD_WriteCtrl(0x02);//--------------------------- Setting the top line 
	LCD_printf( "Try Again");//------ Text to identify equipment
	delay(5000);
	manualsel();
	}
	}
}



void postmanualmove(char z)
{
	int c;
	switch (z)
	{
		case '1':
		LCD_WriteCtrl(0x01);//--------------------------- Clear screen
		LCD_WriteCtrl(0x02);//--------------------------- Setting the top line 
		LCD_printf("100 Percent");
		delay(100);
		c = 1;
		aimplement(c);
		temp();
		break;
		
		case '2':
		LCD_WriteCtrl(0x01);//--------------------------- Clear screen
		LCD_WriteCtrl(0x02);//--------------------------- Setting the top line 
		LCD_printf("75 Percent");
		delay(100);
		c = 2;
		aimplement(c);
		temp();
		break;
		
		case '3':
		LCD_WriteCtrl(0x01);//--------------------------- Clear screen
		LCD_WriteCtrl(0x02);//--------------------------- Setting the top line 
		LCD_printf("50 Percent");
		delay(100);
		c = 3;
		aimplement(c);
		temp();
		break;
		
		case '4':
		LCD_WriteCtrl(0x01);//--------------------------- Clear screen
		LCD_WriteCtrl(0x02);//--------------------------- Setting the top line 
		LCD_printf("25 Percent");
		delay(100);
		c = 4;
		aimplement(c);
		temp();
		break;
		
		case '5':
		LCD_WriteCtrl(0x01);//--------------------------- Clear screen
		LCD_WriteCtrl(0x02);//--------------------------- Setting the top line 
		LCD_printf("0 Percent");
		delay(100);
		c = 5;
		aimplement(c);
		temp();
		break;
		
		default:
		LCD_WriteCtrl(0x01);//--------------------------- Clear screen
		LCD_WriteCtrl(0x02);//--------------------------- Setting the top line 
		LCD_printf("Try Again");
		delay(1000);
		temp();
		break;
	}

}

void aimplement(int i)
{
	
	switch (i)
	{
		case 1:
		asm // 100%
{
		
		MOV R2,#100
		MOV TMOD,#01
HERE:
		MOV TL0,#000H
		MOV TH0,#0f4H
		SETB P1.0
		ACALL DELAY1
		CLR P1.0
		ACALL DELAY2
		DJNZ R2,HERE
		NOP	
		
DELAY1:
		SETB TCON.4	
AGAIN:	JNB TCON.5,AGAIN
		CLR TCON.4
		CLR TCON.5
		RET
		
DELAY2:
		MOV TMOD,#01
		MOV TL0,#053H
		MOV TH0,#06fH
		SETB TCON.4	
AGAIN2:	JNB TCON.5,AGAIN2
		CLR TCON.4
		CLR TCON.5
		RET
		
}
LCD_WriteCtrl(0x01);
delay(100);
break;

	case 2:
	
	asm // 75%
{
		
		MOV R2,#100
		MOV TMOD,#01
HERE1:
		MOV TL0,#000H
		MOV TH0,#0f6H
		SETB P1.0
		ACALL DELAY3
		CLR P1.0
		ACALL DELAY4
		DJNZ R2,HERE1
		NOP		
		
DELAY3:
		SETB TCON.4	
AGAIN1:	JNB TCON.5,AGAIN1
		CLR TCON.4
		CLR TCON.5
		RET
		
DELAY4:
		MOV TMOD,#01
		MOV TL0,#053H
		MOV TH0,#06fH
		SETB TCON.4	
AGAIN3:	JNB TCON.5,AGAIN3
		CLR TCON.4
		CLR TCON.5
		RET
		
}
LCD_WriteCtrl(0x01);
delay(100);
break;


	case 3:
	
	asm // 50%
{
		
		MOV R2,#100
		MOV TMOD,#01
HERE2:
		MOV TL0,#000H
		MOV TH0,#0f7H
		SETB P1.0
		ACALL DELAY5
		CLR P1.0
		ACALL DELAY6
		DJNZ R2,HERE2
		NOP		
		
DELAY5:
		SETB TCON.4	
AGAIN4:	JNB TCON.5,AGAIN4
		CLR TCON.4
		CLR TCON.5
		RET
		
DELAY6:
		MOV TMOD,#01
		MOV TL0,#053H
		MOV TH0,#06fH
		SETB TCON.4	
AGAIN5:	JNB TCON.5,AGAIN5
		CLR TCON.4
		CLR TCON.5
		RET	
	}
LCD_WriteCtrl(0x01);
delay(100);
break;

	
	case 4:
	
	asm // 25%
{		
		
		MOV R2,#100
		MOV TMOD,#01
HERE3:
		MOV TL0,#000H
		MOV TH0,#0f9H
		SETB P1.0
		ACALL DELAY7
		CLR P1.0
		ACALL DELAY8
		DJNZ R2,HERE3
		NOP	
DELAY7:
		SETB TCON.4	
AGAIN6:	JNB TCON.5,AGAIN6
		CLR TCON.4
		CLR TCON.5
		RET
		
DELAY8:
		MOV TMOD,#01
		MOV TL0,#053H
		MOV TH0,#06fH
		SETB TCON.4	
AGAIN7:	JNB TCON.5,AGAIN7
		CLR TCON.4
		CLR TCON.5
		RET	
	}
LCD_WriteCtrl(0x01);
delay(100);
break;

	
	case 5:
	
	asm // 00%
{
		
		MOV R2,#100
		MOV TMOD,#01
HERE4:
		MOV TL0,#000H
		MOV TH0,#0fbH
		SETB P1.0
		ACALL DELAY9
		CLR P1.0
		ACALL DELAY10
		DJNZ R2,HERE4
		NOP		
		
DELAY9:
		SETB TCON.4	
AGAIN8:	JNB TCON.5,AGAIN8
		CLR TCON.4
		CLR TCON.5
		RET
		
DELAY10:
		MOV TMOD,#01
		MOV TL0,#053H
		MOV TH0,#06fH
		SETB TCON.4	
AGAIN9:	JNB TCON.5,AGAIN9
		CLR TCON.4
		CLR TCON.5
		RET	
	}
LCD_WriteCtrl(0x01);
delay(100);
break;
	
}
}

void automatic()
{
	char f;


	dtemp();
	tempconv();
	LED_Fah();
	
loop5:;
	for(;;)
	{
	f=Scankeypad();
	if (f == '#')
	{
	break;
	}
	else
	{
	goto loop5;
	}
	}

}
	
void dtemp()
{
	IE=0x81;
	LCD_Init();
	LCD_writectrl(0x02);
	LCD_printf("Automatic Mode ");
	LCD_writectrl(0xC0);
	LCD_printf("Desired Temp in F:");
	t=0;
	for(;;)
	{
		tempc = Scankeypad();
		if (tempc)
		{
			if (tempc == '#')
			break;
			if ( tempc != '#')
			{
				utemp[t] = tempc - 48;
				LCD_printf("%d", utemp[t]);
				t++;
			}
		}
	}
}

void tempconv()
{
	uTenTemp = utemp[0] * 10;
	uptemp = (uTenTemp + utemp[1]);
	LCD_WriteCtrl(0x01);
	LCD_WriteCtrl(0x02);
	LCD_printf("Desired Temp F: %d", uptemp);
	delay(1000);	
}

void LED_Fah()
{
	
begin:;

	temperature = Convert();
	light = Convert1(2);
	LCD_WriteCtrl(0x02);
	LCD_printf("Current Temp:   %d   ", temperature);
	LCD_WriteCtrl(0xC0);
	LCD_printf("Intensity:      %c   ", light);
	h = ScanKeypad();
	if (h == '#')
	lcdselection();
		
	for(;;)
	{
	h = ScanKeypad();
	if (light == 'H')
	{
		s=1;
		aimplement(s);
		goto begin;
	}
	h = ScanKeypad();
	if (temperature > uptemp)// over temp so 100%
	{
		s=1;
		aimplement(s);
		goto begin;
	}
		h = ScanKeypad();
	if (light == 'L')
	{
		s=5;
		aimplement(s);
		goto begin;
	}
	h = ScanKeypad();
	if (temperature < uptemp)
	{
		s=5;
		aimplement(s);
		goto begin;
	}
	h = ScanKeypad();
	
}
}



void servoboot()
{
	
	setbit(P3.3);
	clrbit(P3.1);
	aimplement(1);
	setbit(P3.1);
}