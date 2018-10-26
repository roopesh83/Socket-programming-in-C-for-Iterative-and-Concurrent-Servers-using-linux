#include<unistd.h>
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include<signal.h>
#define PORT 8080
#include "print.h"
#include<arpa/inet.h>
int _sockfd = -1;
char special_char[]={1,1,1,1,0};
void sig_handler(int signo)
{
  if (signo == SIGINT)
  printf("Received SIGINT\nClosing client.\n");
  send(_sockfd,special_char,sizeof(4),0);	
  if(_sockfd >= 0)	
  close(_sockfd);
  exit(1);
}

int main(){


	int socket_d,size,port,errorno,new_client=1;
	struct sockaddr_in serv_addr; 
	char buffer[100],msg[100],yesno;
	char special_char[]={1,1,1,1,0},server_address_string[20];
	

	//memset(&serv_addr, '0', sizeof(serv_addr)); 
	
	signal(SIGINT, sig_handler);	//setting up signal handler

	if( (socket_d = socket(AF_INET,SOCK_STREAM,0)) < 0 ){
		printf("socket error -%d\n",socket_d);
		exit(1);
	}
	_sockfd = socket_d;

	prompt("Enter the port for client to communicate");
	scanf("%d",&port);

	prompt("Enter the server address for client to communicate");
	scanf("%s",server_address_string);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(server_address_string);
	serv_addr.sin_port = htons(port);
	

	// if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
 //    { 
 //        printf("\nInvalid address/ Address not supported \n"); 
 //        return -1; 
 //    } 


	if( ( errorno = connect(socket_d,(struct sockaddr *)&serv_addr,sizeof(serv_addr))) <0){
			printf("Server not listening");
			exit(1);
			//return 2;
	}


	//clean_stdin();
	
	
	/*
		//printf("%s\n", buffer);
		while( strcasecmp(buffer,"connected") != 0){
			println("Trying to reach server...");
			size = recv(socket_d,buffer,9,0);
			buffer[size] = 0;
			//size = read(socket_d,buffer,9);
			//buffer[size] = 0;
			if( strcasecmp(buffer,"connected") == 0 ){
					println("connected");
					break;
			}
			//printf("%s\n", buffer);			//check if client is connected to server	
			//sleep(5);
		}

	*/



	do{
		prompt("Enter message to be transmitted to the server");
		clean_stdin();
		scanf("%[^\n]s",msg);

	// strcpy(msg,"msgtoserver");
	

		send(socket_d,msg,sizeof(msg),0);
		msg[0]=0;	//reset msg

		size = recv(socket_d,buffer,100,0);
		buffer[size] = '\0';



		if( strlen(buffer) == 0 && (strcasecmp(buffer,"byebye") != 0) ){	// if any one condition fails
			println("Server stoped abruptly/or is not responding");
			exit(1);
		}
		else
			printf("message received from server is \"%s\"\n",buffer);
	}while(1);				
	
	//prompt("Wish to continue?(Y/N)");
	//clean_stdin();
	//yesno = getchar();
	// if( yesno == 'N' || yesno == 'n'){
	// 	send(socket_d,"dis",3,0);	
	// 	printf("%d",socket_d);
	// 	close(socket_d);					//client is closing the connection with the server
	// 	break;
	// }
	//clean_stdin();
	
	

	

	
	
	close(socket_d);



	return 0;
}