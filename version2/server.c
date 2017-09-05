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
#include "header.h"


int vrbl=0;
struct packet *pckt;
int src_port,dest_port;

void hash(char *,char *,int);
int main()  
{  	int iphdrlen;
	char buffer[100];

	

	    time_t rawtime;
	    struct tm *timeinfo;
	    int opt = 1;  
	    int rtrn,master_socket , addrlen , new_socket , client_socket[30] ,udp_fd, 
	          max_clients = 30 , activity, i , valread , sd;  
	    int max_sd,tcp,udp;  
	    struct sockaddr_in address,client_addr,udp_address;  
        
	    
	    char hash_timedata[32],client_ip[32],*tic; 
	    fd_set readfds;  
	    char *message = "hi i am th eserver\n";
    		tcp=0;
		udp=1;
  
	    memset(client_socket,0,sizeof(client_socket));
	  
	    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) <= 0)  
	    {  
	        perror("TCP socket failed");  
	        return -1;memset(pckt,'\0',sizeof(struct packet));
	    }  
	      if( (udp_fd = socket(AF_INET , SOCK_DGRAM, 0)) <= 0)  	
	    {  
	        perror("UDP socket failed");  
	        return -1;
	    }  
	    if( setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,sizeof(opt)) < 0 )  
	    {  
	        perror("tcp setsockopt");  
	    	return -1;
	    }  
	    if( setsockopt(udp_fd, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,sizeof(opt)) < 0 )  
	    {
	        perror("udp setsockopt");  
	    	return -1;
    		}  
	    

	    address.sin_family = AF_INET;  
	    address.sin_addr.s_addr = inet_addr("192.168.131.31"); 
	    address.sin_port = htons(8080);  


	    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address))<0)  
	    {  
	        perror("tcp bind failed");  
		return -1;
	    }  
	    if (bind(udp_fd,(struct sockaddr *)&address, sizeof(address))<0)  
	    {  
	        perror("udp bind failed");  
		return -1;
	    }  

        

	    if (listen(master_socket, 3) < 0)  
	    {  

	        perror("listen");  
		return -1;
	    }  
        

	    addrlen = sizeof(client_addr);  
	    puts("Waiting for connections ...");  
        
	    while(1)  
	    {  
       
        		FD_ZERO(&readfds);  
  
       
		        FD_SET(master_socket, &readfds);  
			FD_SET(udp_fd,&readfds);
		        max_sd = udp_fd;  
            

       
        		for ( i = 0 ; i < max_clients ; i++)  
		        {  
       
		            sd = client_socket[i];  
                
       
		            if(sd > 0)  
                		FD_SET( sd , &readfds);  
                
  
		            if(sd > max_sd)  
		                max_sd = sd;  
		        }  		
    
   
		        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);  
      
        		if ((activity < 0) && (errno!=EINTR))  
		        {  
		            printf("select error");  
		        }  
            
   

		        if ((FD_ISSET(master_socket, &readfds)))
        		{  
		            new_socket =accept(master_socket,(struct sockaddr *)&client_addr, (socklen_t*)&addrlen);
				 if(new_socket == -1)  
			            {  
			                perror("tcp_accept");  
			                exit(EXIT_FAILURE);  
			            }  
			            getsockname(new_socket,(struct sockaddr *)&client_addr,&addrlen);
			
			            printf("New connection , socket fd is %d , ip is : %s\n", new_socket ,inet_ntoa(client_addr.sin_addr));  
                
			
			            for (i = 0; i < max_clients; i++)  
			            {  
			
			                if( client_socket[i] == 0 )  
			                {  
			                    client_socket[i] = new_socket;  
			
						printf("Adding to list of sockets as %d\n" , i);  
			                        break;
        			        }  
			            }  
		        }  
			else if((FD_ISSET(udp_fd,&readfds)))
			{
				recvfrom(udp_fd,buffer,sizeof(buffer),0,(struct sockaddr *)&client_addr,&addrlen);
				time(&rawtime);
				timeinfo=localtime(&rawtime);
			
				strcpy(hash_timedata,(char *)asctime(timeinfo));//time stamp is stored in vrbl
				 getpeername(sd , (struct sockaddr*)&client_addr,(socklen_t*)&addrlen); 
			

						pckt=(struct packet*)malloc(sizeof(struct packet)); 
						memset(pckt,'\0',sizeof(struct packet));
						strcpy(pckt->time_udp,hash_timedata); 
						
						
						pckt->src=(int)ntohl(client_addr.sin_addr.s_addr);
			
						strcpy(pckt->src_addr,inet_ntoa(client_addr.sin_addr));

			
						strcpy(pckt->dest_addr, inet_ntoa(address.sin_addr));
						pckt->dest=(int)ntohl(client_addr.sin_addr.s_addr);
			
 			
						pckt->src_port=(ntohs)(client_addr.sin_port);
			
						pckt->dest_port=ntohs(address.sin_port);
			
						pckt->udp_count=0;
						vrbl=6;
						pckt->protocol=6;
						hash_insert(pckt,vrbl); 
						strcpy(client_ip,inet_ntoa(client_addr.sin_addr));
		
		  		printf("New udp connection , socket fd is %d , ip is : %s\n", udp_fd ,inet_ntoa(client_addr.sin_addr));
				printf("data from the udp client is %s\n",buffer);
		            }
	       
        		for (i = 0; i < max_clients; i++)  
		        {  
		            sd = client_socket[i];  
                
		            if (FD_ISSET( sd , &readfds))  
            			{  
               
			                if ((valread = read( sd , buffer, 1024)) == 0)  
			                {  
			                    getsockname(new_socket,(struct sockaddr *)&client_addr,&addrlen);
			                    getpeername(sd , (struct sockaddr*)&client_addr,(socklen_t*)&addrlen);  
			                    printf("Host disconnected , ip %s\n",inet_ntoa(client_addr.sin_addr));  
             		           
                    
			                    close( sd );  
			                    client_socket[i] = 0;  
			   
            }  
                    
                
			                else
			                {  
                    
				                buffer[valread] = '\0';  
                    				read(sd,buffer,strlen(buffer));  

						time(&rawtime);
						timeinfo=localtime(&rawtime);
						rtrn=printf("%s\n",asctime(timeinfo));
						strcpy(buffer,(char *)asctime(timeinfo));

		
						 getpeername(sd , (struct sockaddr*)&client_addr,(socklen_t*)&addrlen);
					/*------------------------copying data into packet -----------------------*/
						
						pckt=(struct packet*)malloc(sizeof(struct packet)); 
						memset(pckt,'\0',sizeof(struct packet));						
						strcpy(pckt->time_tcp,buffer);
						memset(buffer,'\0',sizeof(buffer));
						strcpy(pckt->time_udp,buffer);
						strcpy(pckt->src_addr,inet_ntoa(client_addr.sin_addr));
						pckt->src=(int)ntohl(client_addr.sin_addr.s_addr);
				
						
				
						strcpy(pckt->dest_addr, inet_ntoa(address.sin_addr));
				
						pckt->dest=(int)ntohl(client_addr.sin_addr.s_addr);
				
						getsockname(new_socket,(struct sockaddr *)&client_addr,&addrlen);
						pckt->src_port=(ntohs)(client_addr.sin_port);
						
						pckt->dest_port=ntohs(address.sin_port);
					
						
						vrbl=17;
						pckt->protocol=17;
						hash_insert(pckt,vrbl);


						 getpeername(sd , (struct sockaddr*)&client_addr,(socklen_t*)&addrlen); 

					
						strcpy(client_ip,inet_ntoa(client_addr.sin_addr));

		
					
						if(strncmp(buffer,"look",4) == 0)
						{
					
							rtrn=read(sd,buffer,strlen(buffer));
							if(rtrn == -1)
							{
								perror("read error\n");
								return -1;
							}
							break;						
						}
//					
			
						printf("data from the client:%s\n",buffer);
						

			                }  
			            }  
		        }  
    }  
        
    return 0;  
}  

