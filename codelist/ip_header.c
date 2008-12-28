#include <netinet/in.h>

struct ip_header
{
#ifdef WORDS_BIGENDIAN
	u_int8_t ip_version:4,		//IP版本
		 ip_header_len:4;	//IP首部长度
#else
	u_int8_t ip_header_len:4,
		 ip_version:4;
#endif
	u_int8_t  ip_TOS;	//TOS
	u_int16_t ip_len;	//IP封包总长
	u_int16_t ip_id;	//IP标识
	u_int16_t ip_off;	//偏移
	u_int8_t  ip_ttl;	//Time To Live 封包经过一个路由器，存活时间减一
	u_int8_t  ip_protocol;	//协议
	u_int16_t ip_checksum;	//首部校验和
	struct in_addr src_ip;
	struct in_addr dst_ip;
}
