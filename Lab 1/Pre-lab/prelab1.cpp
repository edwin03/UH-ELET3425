#include<iostream>

int i,t,j,n,max;	
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
	
	 
	max = vals[0];
	
	for (j=0 ; j<10 ; j++)
	{
		for (i=0 ; i<9 ; i++)
		{
			if (vals[i] < vals[i+1])
			{
				t = vals[i];
				vals[i] = vals[i+1];
				vals[i +1] = t;
			}
		}
	}
	
	printf ("\n Desending Order: ");
	for(i=0; i<10; i++)
	{
		printf ("%d ", vals[i]);
		
	}
	
	while(1);	
}
