#include <stdio.h>
#include <time.h>
int main()
{
	int i = time(0);
	printf("%d\n",i>>5);
	return 0;
}
