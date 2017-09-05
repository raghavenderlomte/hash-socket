
struct packet{
int protocol;
char src_addr[50];
char dest_addr[50];
char time_udp[28];
char time_tcp[28];
int tcp_count,udp_count;
unsigned long int src,dest;
int src_port,dest_port;
};
void hash_insert(struct packet *,int);

