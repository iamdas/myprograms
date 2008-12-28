#include <libnet.h>

void main(int argc,char *argv[])
{
	if(argc!=2)
	{
		printf("用法错误：需要添加目的IP参数\n");
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
	
	u_char arp_sha[6]={0x00,0x1e,0x90,0xc7,0xe3,0xbb};
	u_char arp_dha[6]={0x00,0x00,0x00,0x00,0x00,0x00};
	char *arp_src_ip="192.168.1.6";
	u_int arp_spa=libnet_name2addr4(l,arp_src_ip,LIBNET_RESOLVE);
	char *arp_dst_ip=argv[1];
	u_int arp_dpa=libnet_name2addr4(l,arp_dst_ip,LIBNET_RESOLVE);
	if(libnet_build_arp(ARPHRD_ETHER,ETHERTYPE_IP,6,4,ARPOP_REQUEST,arp_sha,(u_int8_t *)&arp_spa,arp_dha,(u_int8_t *)&arp_dpa,NULL,0,l,0)==-1)
	{
		printf("构造ARP错误：%s\n",libnet_geterror(l));
		libnet_destroy(l);
		return;
	}
	
	u_char ether_sha[6]={0x00,0x1e,0x90,0xc7,0xe3,0xbb};
	u_char ether_dha[6]={0xff,0xff,0xff,0xff,0xff,0xff};
	if(libnet_build_ethernet(ether_dha,ether_sha,ETHERTYPE_ARP,NULL,0,l,0)==-1)
	{
		printf("构造以太错误：%s\n",libnet_geterror(l));
		libnet_destroy(l);
		return;
	}
	//libnet_diag_dump_pblock(l);
	//libnet_diag_dump_context(l);
	libnet_write(l);
	libnet_destroy(l);
	return;
}
