#include<stdio.h>
#include<stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<string.h>


struct tcpudp{
	char tcp[28];
	int tcp_count,udp_count;
	char udp[28];
};
struct tcpudp *tcp_udp,*temp;

void hash_lookup(char *);
void hash(char *,char *,int);
int hash_function(char *);


int *client_ip_index[65537];
void hash(char *packet_time,char *ip_address,int vrbl)
{
	int index;
	static int i=0;
	

	index=hash_function(ip_address);	
	if(i==0)
	{
		memset(client_ip_index,'\0',sizeof(client_ip_index));
		
		tcp_udp=(struct tcpudp *)malloc(sizeof(*tcp_udp));
			memset(tcp_udp,'\0',sizeof(*tcp_udp));
	
		if(vrbl == 0)
		{
			strcpy((tcp_udp->tcp),packet_time);
			(tcp_udp->tcp_count)++;
			printf("data in hash_table[%d].tcp is %s\n",i,tcp_udp->tcp);
			printf("number of tcp packets send yet from %s address %d\n",ip_address,tcp_udp->tcp_count);
			client_ip_index[index]=(int *)tcp_udp;
			i++;
		}
		else
		{
			strcpy((tcp_udp->udp),packet_time);
			(tcp_udp->udp_count)++;
			printf("data in hash_table[%d].udp is %s\n",i,tcp_udp->udp);
			printf("number of udp packets send yet from %s address %d\n",ip_address,tcp_udp->udp_count);
			client_ip_index[index]=(int *)tcp_udp;
			i++;
		}

	}
	else
	{
		if(client_ip_index[index]=='\0')
		{
	
			struct tcpudp *p=(struct tcpudp *)malloc(56);
			if(p==NULL)
			{
				perror("realloc error\n");
				exit(-1);
			}
			tcp_udp=p;
			
			
			if(vrbl == 0)
			{
				strcpy((tcp_udp->tcp),packet_time);
				(tcp_udp->tcp_count)++;
				printf("data in hash_table[%d].tcp is %s\n",i,tcp_udp->tcp);
				printf("number of tcp packets send yet from %s address %d\n",ip_address,tcp_udp->tcp_count);
				client_ip_index[index]=(int *)tcp_udp;
				i++;
			}
			else
			{
				strcpy((tcp_udp->udp),packet_time);
				(tcp_udp->udp_count)++;
				printf("data in hash_table[%d].udp is %s\n",i,(tcp_udp->udp));
				printf("number of udp packets send yet from %s address %d\n",ip_address,tcp_udp->udp_count);
				client_ip_index[index]=(int *)tcp_udp;
				i++;
			}

		}
		else
		{
	
			temp=(struct tcpudp *)client_ip_index[index];
			
			if(vrbl==0)
			{
				strcpy(temp->tcp,packet_time);
				(temp->tcp_count)++;
				printf("data in else hash_table[%d].tcp is %s\n",index,temp->tcp);
				printf("number of tcp packets send yet from %s address %d\n",ip_address,temp->tcp_count);
			}
			else
			{
				strcpy(temp->udp,packet_time);
				(temp->udp_count)++;
				printf("data in else hash_table[%d].udp is %s\n",index,temp->udp);
				printf("number of udp packets send yet from %s address %d\n",ip_address,temp->udp_count);
			}
		}	

	
	}
}


void hash_lookup(char look[])
{

	int op_index;

	op_index=hash_function(look);

	temp=(struct tcpudp *)client_ip_index[op_index];



	printf("ip: %s \ntcp_packet time_stamp:%s\nudp_packet time_stamp %s\ntcp packets count %d \nudp packets count %d\n",look,temp->tcp,temp->udp,temp->tcp_count,temp->udp_count);

}



int hash_function(char *ip)
{

	int index;


	index=0x0000ffff&htonl((inet_addr(ip)));

	return index;

}


