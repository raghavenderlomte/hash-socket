#include <stdio.h> 
#include <string.h>   
#include <stdlib.h> 
#include <errno.h> 
#include <unistd.h>   
#include <arpa/inet.h>  
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <sys/time.h> 
#include <time.h>  
#define MAX 65535
unsigned long int *ky;
unsigned long int key[4];
int *ind[4][65535]={0};
int x,y;


struct packet *temp,*temp1;	


void hash_look(struct packet *data)
{
	printf("address %d\n",*(int *)data);
	printf("sorce address %s\n",data->src_addr);
	printf("destination address %s\n",data->dest_addr);
	printf("source port %d\n",data->src_port);
	printf("destination port %d\n",data->dest_port);
	printf("tcp time stamp %s\n",data->time_tcp);
	printf("udp time stamp %s\n",data->time_udp);
	

}


long int * hash_index(struct packet *pk)
{
	int l,m,z;
	l=pk->src;
	m=pk->dest;
	printf("protocol %d\n",x);
	z=(l+m)/2;
	key[0]=(z-pk->protocol);
	key[0]=(key[0]%MAX);
	z=(l-m)/2;

	key[1]=z+pk->protocol;
	key[1]=(key[1]%MAX);
	
	key[2]=(pk->src_port);
	key[2]=(key[2]%MAX);
	
	key[3]=(pk->dest_port);
	key[3]=(key[3]%MAX);
	
	printf("keys %ld %ld %ld %ld\n",key[0],key[1],key[2],key[3]);
	return key;
}



void hash_insert(struct packet *pck,int vrbl)
{


	int i;
	long int j;
	

	x=vrbl;
	ky=hash_index(pck);
	temp=(struct packet *)malloc(sizeof(struct packet));
	memset(temp,0,sizeof(struct packet));
	temp=pck;
	for(i=0;i<4;i++)
	{

		j=*(ky+i);
		
		
		printf("key[%d][%ld]\n",i,j);
		temp1=(struct packet *)ind[i][j];
		
		if(ind[i][j] == 0)
		{
			
			
			
			
			
			if(vrbl == 6)
			{
				
				
				
				temp->udp_count++;
				
			}
			else
			{

				
				
				temp->tcp_count++;
				
			}
			
			ind[i][j]= (int *)temp;
			
			break;
		}
		else if((temp1->src==pck->src)&&(temp1->dest==pck->dest)&&(temp1->src_port == pck->src_port)&&(temp1->dest_port == pck->dest_port))
		{
			
			
				if(vrbl == 6)
				{

				

					temp1->udp_count++;
					strcpy(temp1->time_udp,pck->time_udp);
					
					ind[i][j]=(int *)temp1;
					break;
	
				}
				else
				{
					temp1->tcp_count++;
				
					strcpy(temp1->time_tcp,pck->time_tcp);

					ind[i][j]=(int *)temp1;
					break;
					
				}
				
				
		}
			
		

	}		
				


		temp1=(struct packet *)ind[i][j];
		hash_look(temp1);

	

}

