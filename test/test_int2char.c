int main()
{	
	int a=0xff00ffff;
	unsigned char *b=(char *)&a;
	printf("%d\n",*(b+2));
	return 0;
}

/* 结论：
	当char型指针加 1 ，就向高阶位（左），移动一个字节的距离。
*/
