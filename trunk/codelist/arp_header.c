struct arp_header
{
	u_int16_t hardware_type;	/* 硬件地址类型   需要ntohs()转换,0x0001表示以太网地址类型(可看作字节，高低位反向。) */
	u_int16_t protocol_type;	/* 协议地址类型   需要ntohs()转换,0x0800表示IP地址类型(可看作字节，高低位反向，。) */
	u_int8_t  hardware_len;		/* 硬件地址长度   以太网地址长度(为6字节) */
	u_int8_t  protocol_len;		/* 协议地址长度   IP地址长度(为4字节) */
	u_int16_t operation_code;	/* 操作类型       需要ntohs()转换,ARP请求为1，ARP响应为2，RARP请求为3，RARP响应为4(可看作字节，高低位反向 */
	u_int8_t  src_ethernet[6];	/* 源以太网地址   长度为hardware_len */
	u_int8_t  src_ip[4];		/* 源IP地址       长度为protocol_len */
	u_int8_t  dst_ethernet[6];	/* 目的以太网地址 长度为hardware_len */
	u_int8_t  dst_ip[4];		/* 目的IP地址     长度为protocol_len */
};

