struct ethernet_header
{
	u_int8_t dhost[6];
	u_int8_t shost[6];
	u_int16_t ether_type;
};

