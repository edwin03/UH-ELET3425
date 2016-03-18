#include <8051io.h>
#include <8051bit.h>            /* Bit set/clear macros */
#include <8051reg.h>

int global_var;

void main()
{
	int i, x, y;
	serinit(9600);
	x=0;
	y=0;
	for(i=0; i<10 ; i++)
	{
	global_var = i;
	x = x+3;
	printf("Value of x is = %d\n", x);
	y=x;
	printf("The updated value of y is = %d\n", y);
	}
	for(;;);
}