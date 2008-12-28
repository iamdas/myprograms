#include <pcap.h>

struct ether_hdr
{
	u_int8_t ether_dhost[6];	//目的以太网地址
	u_int8_t ether_shost[6];	//源以太网地址
	u_int16_t ether_type;		//以太网类型
};
void printf_mac(u_char *mac)
{
	int i;
	for(i=0;i!=5;i++)
	{
		printf("%02x:",*(mac+i));
	}
	printf("%02x\n",*(mac+i));
}
void printf_ether_type(u_short *e_type)
{
	printf("%04x  ",*e_type);
	switch(*e_type)
	{
		case 0x0800: printf("IP\n");break;
		case 0x0806: printf("ARP\n");break;
		case 0x8035: printf("RARP\n");break;
		default:printf("其他协议格式\n");break; 
	}
}

void call_back(u_char *argument,const struct pcap_pkthdr *packet_header,const u_char *packet_content)
{
	struct ether_hdr *ether_header=(struct ether_hdr *)packet_content; //重要！！！
	u_char *mac_dst=ether_header->ether_dhost;
	u_char *mac_src=ether_header->ether_shost;
	u_short ethernet_type=ntohs(ether_header->ether_type);

	printf("****************************\n");
	printf("目的MAC:\n\t");
	printf_mac(mac_dst);
	printf("源自MAC:\n\t");
	printf_mac(mac_src);
	printf("数据包类型：\n\t");
	printf_ether_type(&ethernet_type);
	printf("****************************\n");
}
int main ()
{
	//--
	char error_content[PCAP_ERRBUF_SIZE];
	char *net_interface=pcap_lookupdev(error_content);
	if(net_interface==NULL) 
	{
		printf("设备查找错误：%s",error_content);
	}
	bpf_u_int32 net_ip,net_mask;
	if(pcap_lookupnet(net_interface,&net_ip,&net_mask,error_content)==-1)
	{
		printf("网络查找错误：%s",error_content);
		return 1;
	}
	pcap_t *pcap_handle=pcap_open_live(net_interface,BUFSIZ,1,0,error_content);
	//打开网络接口，并获取句柄
	
	//--
	if(pcap_datalink(pcap_handle)!=DLT_EN10MB) 
	{
		printf("非以太网数据链接");
		return 1;
	}
	//查看数据链接类型是否为EN10MB
		
	//--
	struct bpf_program bpf_filter;
	char bpf_filter_string[]="ip";
	if(pcap_compile(pcap_handle,&bpf_filter,bpf_filter_string,0,net_ip)==-1)
	{
		printf("过滤规则编译错误：%s\n",pcap_geterr(pcap_handle));
		//pcap_geterr(pcap_handle);
		return 1;
	}
	if(pcap_setfilter(pcap_handle,&bpf_filter)!=0)
	{
		printf("过滤规则设置错误：%s\n",pcap_geterr(pcap_handle));
		return 1;
	}
	//设置规则
		
	//--
	int packets_num=-1;	//-1为一直抓包
	if(pcap_loop(pcap_handle,packets_num,call_back,NULL)<0)
	{
		printf("抓包错误:%s",pcap_geterr(pcap_handle));
		return 1;
	}
	//循环抓包
	
	//--
	pcap_close(pcap_handle);
	//关闭网络
	
	return 0;
}
