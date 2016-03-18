#include <8051io.h>
#include <8051bit.h>           
#include <8051reg.h>

main()
{
	serinit(9600);
	
	printf( "\nHello World" );
	
	while(1);
	
}