#include <pcap.h>

struct ether_header
{
	u_int8_t ether_dhost[6];	//目的以太网地址
	u_int8_t ether_shost[6];	//源以太网地址
	u_int16_t ether_type;		//以太网类型
};

int main()
{
	//
	char error_content[PCAP_ERRBUF_SIZE];
	char *net_interface=pcap_lookupdev(error_content);
	bpf_u_int32 net_ip,net_mask;
	pcap_lookupnet(net_interface,&net_ip,&net_mask,error_content);
	pcap_t *pcap_handle=pcap_open_live(net_interface,BUFSIZ,1,0,error_content);
	//打开网络接口,获取句柄
	
	//
	struct bpf_program bpf_filter;
	char bpf_filter_string[]="ip";
	pcap_compile(pcap_handle,&bpf_filter,bpf_filter_string,0,net_ip);
	pcap_setfilter(pcap_handle,&bpf_filter);
	//设置过滤规则
	
	if(pcap_datalink(pcap_handle)!=DLT_EN10MB) return 0;
	
	//
	struct	pcap_pkthdr packet_header;
	const u_char *packet_content=pcap_next(pcap_handle,&packet_header);
	//捕获一个数据包，返回此数据包的缓存，此缓存为被捕获包的字节流
	
	//
	printf("dev :%s\n",net_interface);
	//printf("time:%s\n",ctime((const time_t *)&packet_header.ts.tv_sec));
	//???????此段引起“段错误”
	printf("len :%d\n",packet_header.len);
	
	struct ether_header *ethernet_protocol;
	ethernet_protocol=(struct ether_header *)packet_content;	//重点！！！！！！！！
	
	u_char *mac_src_string=ethernet_protocol->ether_shost;
	u_char *mac_dst_string=ethernet_protocol->ether_dhost;
	u_short ethernet_type=ntohs(ethernet_protocol->ether_type);
	
	printf("mac source      is : %02x:%02x:%02x:%02x:%02x:%02x\n",
			*mac_src_string,*(mac_src_string+1),*(mac_src_string+2),
			*(mac_src_string+3),*(mac_src_string+4),*(mac_src_string+5));
	printf("mac destination is : %02x:%02x:%02x:%02x:%02x:%02x\n",
			*mac_dst_string,*(mac_dst_string+1),*(mac_dst_string+2),
			*(mac_dst_string+3),*(mac_dst_string+4),*(mac_dst_string+5));

	printf("Ethernet type   is :%04x\n",ethernet_type);
	switch(ethernet_type)
	{
		case 0x0800: printf("IP\n");break;
		case 0x0806: printf("ARP\n");break;
		case 0x8035: printf("RARP\n");break;

		default:break;	     
	}
	//输出相关信息。

	pcap_close(pcap_handle);

	return 0;
}
