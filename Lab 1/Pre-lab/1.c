#include <8051io.h>
#include <8051bit.h>            /* Bit set/clear macros */
#include <8051reg.h>


	int i,max;
	
	
void main()
{
	int vals[10];
	vals[0] = 10;
	vals[1] = 45;
	vals[2] = 80;
	vals[3] = 70;
	vals[4] = 12;
	vals[5] = 5;
	vals[6] = 1;
	vals[7] = 55;
	vals[8] = 90;
	vals[9] = 85;
	serinit(9600);
	 
	max = vals[0];
	
	for(i=1; i<10; i++){
		if (vals[i] > max)
		{
			max = vals[i];
		}
	}		
	
    printf("The maximum number is: %d", max);
	
	
	while(1);

	
}