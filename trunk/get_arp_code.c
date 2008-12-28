#include <pcap.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>
struct ethernet_header
{
	u_int8_t dhost[6];
	u_int8_t shost[6];
	u_int16_t ether_type;
};
struct arp_header
{
	u_int16_t hardware_type;
	u_int16_t protocol_type;
	u_int8_t  hardware_len;
	u_int8_t  protocol_len;
	u_int16_t operation_code;
	u_int8_t  src_ethernet[6];
	u_int8_t  src_ip[4];
	u_int8_t  dst_ethernet[6];
	u_int8_t  dst_ip[4];
};
/*struct in_addr
{
	u_int32_t s_addr;	//存放IP
};*/
void printf_mac(u_char *mac_str)
{
	int i;
	for(i=0;i!=5;i++)
	{
		printf("%02x:",*(mac_str+i));
	}
	printf("%02x\n",*(mac_str+i));
}

void call_back(u_char *argument,const struct pcap_pkthdr *pHeader,const u_char *pContent)
{
	//-----------------------------捕获信息分析-----------------------------------------------------------
	printf("=================================\n");
	printf("此包的长度:%d\n",pHeader->len);
	printf("捕获时间:%s",ctime(&pHeader->ts.tv_sec));
	/* 使用ctime()函数，转换为可读日期格式。<time.h>中定义了time_t: typedef __time_t time_t;
	 * 就是说，struct timval{} 中成员tv_sec类型是__time_t，即是time_t型
	 */
	//-----------------------------以太抱头分析-----------------------------------------------------------
	struct ethernet_header *eth_hdr=(struct ethernet_header *)pContent;
	u_char *dhost_string=eth_hdr->dhost;
	u_char *shost_string=eth_hdr->shost;
	u_short eth_type=eth_hdr->ether_type;
	printf("-----------以太头部分------------\n");
	printf("目标以太地址：\t");printf_mac(dhost_string);
	printf("源以太地址：\t");printf_mac(shost_string);
	printf("数据包类型：\t%04x\n",ntohs(eth_type));
	//-----------------------------arp包头分析------------------------------------------------------------
	struct arp_header *arp_hdr=(struct arp_header *)(pContent + 14);	//重点！！！
	u_short hd_type=ntohs(arp_hdr->hardware_type);
	u_short ptc_type=ntohs(arp_hdr->protocol_type);
	u_char hd_len=arp_hdr->hardware_len;
	u_char prt_len=arp_hdr->protocol_len;
	u_short oper_code=ntohs(arp_hdr->operation_code);
	u_char *src_ethernet_string=arp_hdr->src_ethernet;
	struct in_addr src_ip_address;
	memcpy((void *)&src_ip_address,(void *)&arp_hdr->src_ip,sizeof(struct in_addr));
	u_char *dst_ethernet_string=arp_hdr->dst_ethernet;
	struct in_addr dst_ip_address;
	memcpy((void *)&dst_ip_address,(void *)&arp_hdr->dst_ip,sizeof(struct in_addr));
	
	char oper_code_name[50];
	switch(oper_code)
	{
		case 1:	strcpy(oper_code_name,"ARP请求");break;
		case 2:	strcpy(oper_code_name,"ARP响应");break;
		case 3:	strcpy(oper_code_name,"RARP请求");break;
		case 4:	strcpy(oper_code_name,"RARP响应");break;
		default:strcpy(oper_code_name,"未知操作码类型");break;
	}

	printf("------------ARP部分--------------\n");
	printf("硬件类型：\t%d\n",hd_type);
	printf("协议类型：\t%d\n",ptc_type);
	printf("硬件地址长度：\t%d\n",hd_len);
	printf("协议地址长度：\t%d\n",prt_len);
	printf("操作码：\t%d  %s\n",oper_code,oper_code_name);
	printf("- - - - - - - - - - - - - - - - -\n");
	printf("源以太地址：\t");printf_mac(src_ethernet_string);
	printf("源ip地址：\t%s\n",inet_ntoa(src_ip_address));
	printf("目标以太地址:\t");printf_mac(dst_ethernet_string);
	printf("目的ip地址\t%s\n",inet_ntoa(dst_ip_address));
	printf("=================================\n");
}
int main()
{
	//--
	char error_content[PCAP_ERRBUF_SIZE];
	char *net_interface=pcap_lookupdev(error_content);
	if(net_interface==NULL) 
	{
		printf("设备查找错误：%s\n",error_content);
		return 1;
	}
	bpf_u_int32 net_ip,net_mask;
	if(pcap_lookupnet(net_interface,&net_ip,&net_mask,error_content)==-1)
	{
		printf("网络查找错误：%s\n",error_content);
		return 1;
	}
	pcap_t *pcap_handle=pcap_open_live(net_interface,BUFSIZ,1,0,error_content);
	//打开网络接口，并获取句柄
	
	//--
	if(pcap_datalink(pcap_handle)!=DLT_EN10MB) 
	{
		printf("非以太网数据链接\n");
		return 1;
	}
	//查看数据链接类型是否为EN10MB
		
	//--
	struct bpf_program bpf_filter;
	char bpf_filter_string[]="arp";
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
		printf("抓包错误:%s\n",pcap_geterr(pcap_handle));
		return 1;
	}
	//循环抓包
	
	//--
	pcap_close(pcap_handle);
	//关闭网络
	
	return 0;
}
