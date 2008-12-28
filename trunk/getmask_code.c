#include <stdio.h>
#include <pcap.h>
#include <arpa/inet.h>

int main()
{
	char error_buf[PCAP_ERRBUF_SIZE];
	char *dev;
	if((dev=pcap_lookupdev(error_buf))==NULL)
	{
		printf("网络设备查找失败：%s\n",error_buf);
		return 1;
	}
	printf("设备： %s\n",dev);

	bpf_u_int32 net_ip;
	bpf_u_int32 net_mask;
	if(pcap_lookupnet(dev,&net_ip,&net_mask,error_buf)==-1)
	{
		printf("网络查找失败：%s\n",error_buf);
		return 1;
	}

	struct in_addr net_ip_address;
	struct in_addr net_mask_address;
	net_ip_address.s_addr=net_ip;
	net_mask_address.s_addr=net_mask;

	char* net_ip_string=0;
	char* net_mask_string=0;
	net_ip_string=inet_ntoa(net_ip_address);
	printf("ip ： %s\n",net_ip_string);	//此处输出是“192.168.0.0”
	net_mask_string=inet_ntoa(net_mask_address);
	//printf("ip ： %s\n",net_ip_string);	//输出竟是“255.255.255.0”，这个值是net_mask_string的值！
						//原因是：The string is returned in a statically allocated buffer,
						//       which subsequent calls will overwrite.
	printf("掩码： %s\n",net_mask_string);

	return 0;
}
