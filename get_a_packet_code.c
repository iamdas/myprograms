#include "pcap.h"

int main()
{
	char error_content[PCAP_ERRBUF_SIZE];
	struct pcap_pkthdr protocol_header;
	pcap_t *pcap_handle;
	struct bpf_program bpf_filter;
	char bpf_filter_string[]="";
	const u_char *packet_content;
	bpf_u_int32 net_mask;
	bpf_u_int32 net_ip;
	char *net_interface;
	
	net_interface=pcap_lookupdev(error_content);
	pcap_lookupnet(net_interface,&net_ip,&net_mask,error_content);
	pcap_handle=pcap_open_live(net_interface,BUFSIZ,1,0,error_content);
	pcap_compile(pcap_handle,&bpf_filter,bpf_filter_string,0,net_ip);	//编译过滤规则
	pcap_setfilter(pcap_handle,&bpf_filter);				//设置过滤规则

	packet_content=pcap_next(pcap_handle,&protocol_header);			//捕获并返回值

	printf("Capture a packet from : %s\n",net_interface);
	printf("The packet length is : %d\n",protocol_header.len);
	
	pcap_close(pcap_handle);

	return 0;
}

