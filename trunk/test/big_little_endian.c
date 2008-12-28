#include   <stdio.h>  
int  main()
{  
	int i= 0x12345678;  
	if(*(char *)&i == 0x12)  
		printf("Big endian\n");  
	else if(*(char *)&i == 0x78)  
		printf("Little endian\n"); 
	return 0; 
}
