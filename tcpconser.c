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

int _sockfd;
void sig_handler(int signo)
{
  if (signo == SIGINT)
    printf("Received SIGINT\nClosing server.\n");
	
}



int main(){

	int sockfd,len,new_sockfd;
	struct sockaddr_in serv_addr;
	struct sockaddr_in cli_addr;
	int addr_len,port,errorno;
	char buffer[100]="dis",clientname[90],yesno;
	char special_char[]={1,1,1,1,0};
	int once = 1,pid;

	signal(SIGINT, sig_handler);	//setting up signal handler

	if( (sockfd = socket(AF_INET,SOCK_STREAM,0)) == 0)
	{
		printf("socket creation failed");
	}

	prompt("Enter port number for server to communicate");
	scanf("%d",&port);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if ( (errorno = bind(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))) < 0){
		printf("bind failed error num - %d",errorno);
	}

	if( (errorno = listen(sockfd,10)) <0)
		printf("listen failed-%d\n",errorno);

	addr_len = sizeof(cli_addr);

	
do{
	println("Waiting for client to connect ...");
	if( (new_sockfd = accept(sockfd,(struct sockaddr *)&cli_addr,&addr_len)) <0 )
			printf("accept failed %d\n",errorno);

	if( (pid = fork()) <0){
		println("fork error");
		return 0;
	}

	if(pid == 0){

	
	printf("cli address is %s:%d\n",inet_ntoa(cli_addr.sin_addr),ntohs(cli_addr.sin_port));
	
	
	/*while( strcasecmp(buffer, "dis") != 0 ){
		printf("%s\n", buffer);
		len = recv(new_sockfd,buffer,1024,0);
		buffer[len] = 0;
		if( strcasecmp(buffer, "dis") != 0 ){
			printf("message received from the client is \"%s\"\n",buffer);	
			send(new_sockfd,"byebye",6,0);
		}

	}
	printf("%s\n", buffer);
	strcpy(buffer,"conn");*/
	len = recv(new_sockfd,buffer,1024,0);
	buffer[len] = 0;
	printf("message received from the client is \"%s\"\n",buffer);	
	send(new_sockfd,"byebye",6,0);
	close(new_sockfd);




	}

	if(pid == 0)
		break;
}while(1);
	
	//char *strAdd2 = inet_ntoa(serv_addr.sin_addr);
	// printf("Client with address %s has been connected to server\n",;
	// printf("Client Adress = %s",inet_ntop(AF_INET,&cli_addr.sin_addr,
                    // clientname,clientname, INET6_ADDRSTRLEN));
	
	//send(new_sockfd,"connected",9,0);
	
	
	

	//clean_stdin();

	// prompt("Wish to continue?(Y/N)");
	// yesno = getchar();
	// if(yesno == 'n' || yesno == 'N'){
	// 	break;
	// }

	close(sockfd);
	close(new_sockfd);
	return 0;
}