#include<unistd.h>
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include<signal.h>
#include<time.h>
#include<sys/time.h>
#include<stdlib.h>
#define PORT 8080
#include "print.h"
#include "time_measure.h"

// cmd line args in the order portno,server addr,upper bound for random number,display response

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

int main(int argc,char *argv[]){


	int socket_d,size,port,errorno,new_client=1,random_num,upper_bound,display_response = 1,client_id = 1;
	struct sockaddr_in serv_addr; 
	struct timeval tm;
	char buffer[100],msg[100] = "message from client",yesno,text[50];
	char special_char[]={1,1,1,1,0},server_address_string[20];
	time_t t;
	
	FILE *fp;
	//int myInt = 5;
	if(argc == 1)
		fp = stdout;
	else
		fp = fopen("observations.txt", "a");// "a" means that we are going to append data on this file
	
	//fprintf(fp, "This is being written in the file. This is an int variable: %d", myInt);
	// fclose(fp); 

	//memset(&serv_addr, '0', sizeof(serv_addr)); 
	if(argc != 1)
	{
		sscanf(argv[5],"%d",&client_id);
		fprintf(fp,"client id :%d \n",client_id);
	}
	
	if(argc >= 4){
		 sscanf(argv[1], "%d", &port);
		 strcpy(server_address_string,argv[2]);
		 sscanf(argv[3], "%d", &upper_bound);
		 if(argc >= 5){
		 	if(strcasecmp(argv[4],"false") == 0)	//checking if the 4th cmd-line arg is false
		 		display_response = 0;
		 }

		 /*if(argc >= 7){
		 	sscanf(argv[6],"%d",&client_id);
		 }*/
		 	

	}


	signal(SIGINT, sig_handler);	//setting up signal handler

	if( (socket_d = socket(AF_INET,SOCK_STREAM,0)) < 0 ){
		printf("socket error -%d\n",socket_d);
		exit(1);
	}
	_sockfd = socket_d;

	if(argc == 1){
		prompt("Enter the port for client to communicate");
		scanf("%d",&port);

		prompt("Enter the server address for client to communicate");
		scanf("%s",server_address_string);
		clean_stdin();	
	}
	

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
	if(argc == 1)
		srand(time(0)); 
	else{
		gettimeofday(&tm,NULL);
		srand(tm.tv_usec); //giving ms as param
	}

	if(argc == 1)
  	{
  		prompt("Enter upper bound for random number");
  		scanf("%d",&upper_bound);
  	}
	random_num = rand()%upper_bound;
	
	//if( argc >=7 )								//to display client id for 1000+ clients problem
		//printf("client_id - %d\n",client_id);
	if(argc == 1)
	printf("Random number of requests to be sent is - %d\n CONFIRM YOUR ACTION IN 3sec\n",random_num);
	else
	fprintf(fp,"Random number of requests to be sent is - %d\n",random_num);		
	
	if(argc == 1)	
	sleep(3);
	/*for(int i=3;i>=1;i--){
		printf(" %d..",i );
		sleep(1);
	}*/
	//clean_stdin();
	start_timer();			//timer to measure time taken to send all requests to client
	for(int i=0; i<random_num; i++){
		//prompt("Enter message to be transmitted to the server");
		//clean_stdin();
		//scanf("%[^\n]s",msg);

	// strcpy(msg,"msgtoserver");
	

		send(socket_d,msg,sizeof(msg),0);

		size = recv(socket_d,buffer,100,0);
		buffer[size] = '\0';



		if( strlen(buffer) == 0 ){
			println("Server stoped abruptly");
			exit(1);
		}
		else{
			if(display_response)
				fprintf(fp,"response #%d received from server is \"%s\"\n",i+1,buffer);

		}
	}
	sprintf(text,"Time taken for %d requests to be sent and responses to be received is :\n",random_num);
	stop_timer(text,fp);
	send(socket_d,special_char,sizeof(4),0);	//quit after random requests					
	
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
	
	if(argc != 1)
		fclose(fp);

	

	
		
	close(socket_d);
	exit(0);

}
