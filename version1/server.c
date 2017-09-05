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


void hash(char *,char *,int);
void hash_lookup(char *);
 
int main(int argc , char *argv[])  
{  
	    time_t rawtime;
	    struct tm *timeinfo;
	    int opt = 1;  
	    int rtrn,master_socket , addrlen , new_socket , client_socket[30] ,udp_fd, 
	          max_clients = 30 , activity, i , valread , sd;  
	    int max_sd,tcp,udp;  
	    struct sockaddr_in address,client_addr,udp_address;  
        
	    char buffer[100];
	    char *hash_timedata,*client_ip,*tic; 
	    fd_set readfds;  
	    char *message = "hi i am th eserver\n";
    		tcp=0;
		udp=1;
  
	    memset(client_socket,0,sizeof(client_socket));
	  
	    if( (master_socket = socket(AF_INET , SOCK_STREAM , 0)) <= 0)  
	    {  
	        perror("TCP socket failed");  
	        return -1;
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
	    address.sin_addr.s_addr = INADDR_ANY;  
	    address.sin_port = htons(8080);  

/*	    udp_address.sin_family = AF_INET;  
	    udp_address.sin_addr.s_addr = INADDR_ANY;  
	    udp_address.sin_port = htons(8080);        */

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
                
            //highest file descriptor number, need it for the select function 
		            if(sd > max_sd)  
		                max_sd = sd;  
		        }  		
    
        //wait for an activity on one of the sockets , timeout is NULL , 
        //so wait indefinitely 
		        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);  
      
        		if ((activity < 0) && (errno!=EINTR))  
		        {  
		            printf("select error");  
		        }  
            
        //If something happened on the master socket , 
        //then its an incoming connection 
		        if ((FD_ISSET(master_socket, &readfds)))
        		{  
		            new_socket =accept(master_socket,(struct sockaddr *)&client_addr, (socklen_t*)&addrlen);
				 if(new_socket == -1)  
			            {  
			                perror("tcp_accept");  
			                exit(EXIT_FAILURE);  
			            }  
			            getsockname(new_socket,(struct sockaddr *)&client_addr,&addrlen);
			            //inform user of socket number - used in send and receive commands 
			            printf("New connection , socket fd is %d , ip is : %s\n", new_socket ,inet_ntoa(client_addr.sin_addr));  
                
			            //add new socket to array of sockets 
			            for (i = 0; i < max_clients; i++)  
			            {  
			                //if position is empty 
			                if( client_socket[i] == 0 )  
			                {  
			                    client_socket[i] = new_socket;  
			//	             void hash(char *,char *);       
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
				//rtrn=printf("%s\n",asctime(timeinfo));
				hash_timedata=(char *)asctime(timeinfo);//time stamp is stored in vrbl

				 getpeername(sd , (struct sockaddr*)&client_addr,(socklen_t*)&addrlen); 
				 printf("hash_time %s\n",hash_timedata);
				client_ip=inet_ntoa(client_addr.sin_addr);
		//		printf("udp_client_ip %s\n",client_ip);
				hash(hash_timedata,client_ip,udp);   //hash function invctn
				

        	    		//getsockname(new_socket,(struct sockaddr *)&client_addr,&addrlen);

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
					//	printf("data prsnrt %s\n",buffer);
						time(&rawtime);
						timeinfo=localtime(&rawtime);
						rtrn=printf("%s\n",asctime(timeinfo));
						hash_timedata=(char *)asctime(timeinfo);//time stamp is stored in vrbl

						 getpeername(sd , (struct sockaddr*)&client_addr,(socklen_t*)&addrlen); 

					//	printf("hash_time %s\n",hash_timedata);
						client_ip=inet_ntoa(client_addr.sin_addr);

		
					//	printf("client ip %s\n",client_ip);
						if(strncmp(buffer,"look",4) == 0)
						{
						//	printf("in the if%ld\n",strlen(buffer));
							rtrn=read(sd,buffer,strlen(buffer)); //client is sending the ip to look_up
							if(rtrn == -1)
							{
								perror("read error\n");
								return -1;
							}
						//	printf("after read\n");
						//	printf("%s\n",buffer);
						//	tic=buffer;
							
							//strcpy(buffer,buffer+5);
						//	printf("%s",tic);
							hash_lookup((buffer+5));
							break;						
						}
						hash(hash_timedata,client_ip,tcp);   //hash function invctn
			
						printf("data from the client:%s\n",buffer);
						

			                }  
			            }  
		        }  
    }  
        
    return 0;  
}  
