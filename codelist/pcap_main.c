#include <pcap.h>

void call_back(u_char *argument,const struct pcap_pkthdr *packet_header,const u_char *packet_content)
{
	//
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
	//关闭 pcap
	
	return 0;
}
