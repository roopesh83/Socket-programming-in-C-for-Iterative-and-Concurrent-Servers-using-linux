#include<stdio.h>
#include "print.h"
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<time.h>
#include "time_measure.h"
int main(){

	int n = 0,display_response = 1,port,upper_bound,pid=1,client_id=1,child_processes_ids[2000],status;
	char cmd_line_string[80],display_choice[]="false",server_address_string[20],text[80];			//we make an assumption that the current working directory has the executable files necessary
	FILE *fp;

	printf("---------Welcome---------\n");
	prompt("Enter the number of clients to be generated.");
	scanf("%d",&n);

	prompt("Enter the port for client to communicate");
	scanf("%d",&port);
	clean_stdin();

	prompt("Enter the server address for client to communicate");
	scanf("%s",server_address_string);
	clean_stdin();

	prompt("Enter upper bound for random number");
  	scanf("%d",&upper_bound);

  	clean_stdin();
  	prompt("Would you like to display the response messages received from the server on terminal/file? (1 == yes)");
  	scanf("%d",&display_response);

  	(display_response == 1 ? strcpy(display_choice,"true"):strcpy(display_choice,"false"));

  	fp = fopen("observations.txt", "w");	//just to reset the previous contents
	fclose(fp);

	srand(time(0));

  	//system("echo \"\" > ./observations.log");	//clear the observations.log 
  	start_timer();
  	for(int j = 1;j<=n;j++)
  	if(pid > 0){
  		
  		if((pid = fork()) <0 )		//fork n child process: where n is number of clients
  		println("fork error");

  		if(pid > 0)
		child_processes_ids[j] = pid;

  		client_id = j;
  		if(pid == 0)
  		   break;

  		
  	}
  	

	//for(int i=0;i<n;i++){
  	if(pid == 0){
  		sprintf(cmd_line_string,"./bonus %d %s %d %s %d",port,server_address_string,upper_bound,display_choice,client_id);
		system(cmd_line_string);
  	}
	
	//if(pid > 0)
	//	wait(NULL);
	if(pid > 0)
	for(int j = 0;j < n;j++){
		waitpid(child_processes_ids[j],&status,0);
		//printf("%d\n",child_processes_ids[j]);
	}
	
	//printf("process pid exiting- %d\n",pid);	
	//}
	if(pid > 0){
		sprintf(text,"Time consumed for generating %d clients is ",n);
		stop_timer(text,stdout);	
	}
	
	exit(0);
	//return 0;
}
