#include<unistd.h>
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include<sys/types.h>//socket
#include "print.h"
#include<arpa/inet.h>
#include<signal.h>
int _sockfd = -1,_newsockfd = -1;
int top = -1,global_count;
void sig_handler(int signo)
{
  if (signo == SIGINT)
  printf("Received SIGINT\nClosing server (server instance %d).\n",global_count);
  if(_sockfd >= 0)
  close(_sockfd);
  
  if(_newsockfd >= 0)
  close(_newsockfd);

  exit(1);
}
int main(){

	int sockfd,len,new_sockfd;
	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;
	int addr_len,port,errorno;
	char buffer[100]="dis",clientname[90],yesno,client_address_string[100];
	char special_char[]={1,1,1,1,0};
	int once = 1,count = 1,pid = -1,count1 = 1;

	signal(SIGINT, sig_handler);	//setting up signal handler
	if( (sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
	{
		printf("socket creation failed");
		exit(1);
	}
	_sockfd = sockfd;
	prompt("Enter port number for server to communicate");
	scanf("%d",&port);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if ( (errorno = bind(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))) < 0){
		printf("bind failed\n");
		exit(1);
	}

	if( (errorno = listen(sockfd,10)) <0){
			printf("listen failed\n");
			exit(1);
	}

	addr_len = sizeof(cli_addr);

	do{
	
	if( pid < 0) 		//first time in parent pid is -1
	printf("Waiting for client #%d  to connect ... \n",count);
	else if(pid > 0)
	printf("Waiting for client #%d  to connect in the background ...(concurrency) \n",count);
		
	if( (new_sockfd = accept(sockfd,(struct sockaddr *)&cli_addr,&addr_len)) <0 ){
				printf("accept failed %d\n",errorno);
				exit(1);
	}

	if( (pid = fork()) <0){
		printf("fork error");
		exit(1);
	}

	if(pid != 0)
		count++;	//increment count in parent process


	if( pid == 0 ){		// within the child process

	global_count = count;
	_newsockfd = new_sockfd;

	
	strcpy(client_address_string,inet_ntoa(cli_addr.sin_addr));
	printf("Client #%d with address  %s has connected\n",count,client_address_string);//,ntohs(cli_addr.sin_port));
	
	while( strcasecmp(buffer,special_char) != 0 ){
		len = recv(new_sockfd,buffer,1024,0);
		buffer[len] = 0;
		if( strcasecmp(buffer,special_char) == 0 ){
				printf("client #%d with address %s has quit\n",count, client_address_string);
				break;
		}

		printf("message received from the client #%d(with address %s)is \"%s\"\n",count,client_address_string,buffer);	
		send(new_sockfd,"byebye",6,0);	
	}
	
	buffer[0] = 0;	//resetting the buffer which had special char in it
	close(new_sockfd);
	exit(1); 	//child process exits once client has quit //"Closing server instance,count;

	}}while(1);
	
	close(sockfd);
	close(new_sockfd);
	return 0;
}