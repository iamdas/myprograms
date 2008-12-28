#include <pcap.h>
void call_back(u_char *argument,const struct pcap_pkthdr *packet_header,const u_char *packet_content)
{
	static int packet_num=1;
	printf("\t%d packets were captured.\n",packet_num++);
}
int main()
{
	//打开网络接口
	char error_content[PCAP_ERRBUF_SIZE];
	char *net_dev=pcap_lookupdev(error_content);	//返回是eth0
	net_dev="ppp0";					//如果是pppoe连接互联网，则使用ppp0接口
	bpf_u_int32 net_ip;
	bpf_u_int32 net_mask;
	pcap_lookupnet(net_dev,&net_ip,&net_mask,error_content);
	pcap_t *pcap_handle=pcap_open_live(net_dev,BUFSIZ,1,0,error_content);
	
	//设置过滤
	struct bpf_program bf;
	char bf_string[]="ip src";
	pcap_compile(pcap_handle,&bf,bf_string,0,net_mask);
	pcap_setfilter(pcap_handle,&bf);

	//循环抓包
	pcap_loop(pcap_handle,0,call_back,NULL);
	
	//关闭pcap
	pcap_close(pcap_handle);

	return 0;
}
