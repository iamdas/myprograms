#include <stdio.h>

/*
template <typename T>
int type_long(const T t)
{
	return sizeof(t);
}
*/
int main()
{
	//printf("%ld\n",sizeof(int));
	/*char a;
	printf("char\t类型长度：%ld\n",type_long(a));
	wchar_t b;
	printf("wchar_t\t类型长度：%ld\n",type_long(b));
	short c;
	printf("short\t类型长度：%ld\n",type_long(c));
	int d;
	printf("int\t类型长度：%ld\n",type_long(d));
	long e;
	printf("long\t类型长度：%ld\n",type_long(e));
	float f;
	printf("float\t类型长度：%ld\n",type_long(f));
	double g;
	printf("double\t类型长度：%ld\n",type_long(g));
	long double h;
	printf("long double\t类型长度：%ld\n",type_long(h));
	*/
	printf("%s\t类型长度：\t%ld\n","char",sizeof(char));
	printf("%s\t类型长度：\t%ld\n","short",sizeof(short));
	printf("%s\t类型长度：\t%ld\n","int",sizeof(int));
	printf("%s\t类型长度：\t%ld\n","long",sizeof(long));
	printf("\n");
	printf("%s\t类型长度：\t%ld\n","wchar_t",sizeof(wchar_t));
	printf("%s\t类型长度：\t%ld\n","float",sizeof(float));
	printf("%s\t类型长度：\t%ld\n","double",sizeof(double));
	printf("%s\t类型长度：\t%ld\n","long double",sizeof(long double));
	printf("\n");
	printf("%s\t类型长度：\t%ld\n","指针",sizeof(void *));
	
	return 0;
}


