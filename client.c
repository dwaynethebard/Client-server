/*
Name:Darren Ytsma
student number:301171125
SFU username:dytsma
Lecture section: CPMT 300 D100
instructor: Brian Booth
TA: Scott Kristjanson
*/
#include <netdb.h>
#include <netinet/in.h>
#include "decrypt.h" 
#include <sys/types.h>
#include <ifaddrs.h>
#include <string.h>
#define _GNU_SOURCE 
#include <stdio.h> //allows fget,fputs,foef,fopen
#include <stdlib.h> // for free and malloc
#include  <fcntl.h>
#include <unistd.h> // getpid
#include <sys/wait.h> //wait
#include <time.h>
#include <unistd.h>// # of core
#include "memwatch.h"
#include "FUNctions.h" 
#include "socketfunctions.h" 


int main(int argc, char *argv[]) 
{
	//Set up connect to socket
	mwInit(); //start memwatch
	char *response; //pointer for file
	int sockfd; //socket
	int PortNum; //Port Number
	int n; //loop counter
	struct sockaddr_in serv_addr;// socket address information
	struct hostent *server; //server information
	inputsClient(argc);//checks for the right number of inputs
	
   PortNum = atoi(argv[2]);//port Number
   	char IP[NI_MAXHOST];
    getIP(IP); //get computers IP
   
   // Create a socket
   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   
   if (sockfd < 0) { //make sure socket opens correctly
      printf("[%s] ERROR: failed to open socket for client %s\n",getTime(),IP);
      exit(1);
   }
	
   server = gethostbyname(argv[1]);
   
   if (server == NULL) { //make sure there is a server to connect to
      fprintf(stderr,"[%s] ERROR, no such host for client %s to conncect to \n",getTime(),IP);
      exit(0);
   }
   
   //Socket information
   bzero((char *) &serv_addr, sizeof(serv_addr));
   serv_addr.sin_family = AF_INET;
   bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
   serv_addr.sin_port = htons(PortNum);
   

   	//  Set the socket to non blocking, so if we put the wrong ip address we son't be stuck waiting
    long nonblock;
	nonblock = fcntl(sockfd, F_GETFL, NULL);


   /* Now connect to the server */
   if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
      printf("[%s]ERROR no socket with that IP address or port number available for client %s to conncect to \n",getTime(),IP);
      exit(1);
   }
   
   
   // Now read server response
   char  *buffer;
   buffer =(char *) malloc(2049);
   memset(buffer,'\0',2049);
   n = read(sockfd, buffer, 255);
   free(buffer);

   check(n);//check to see if message was read correctly
	

//Done setting up socket


// start forking and setting up children and pipes
		int CPU_cores; //number of cores
		
		CPU_cores=sysconf(_SC_NPROCESSORS_ONLN); //how many cores
		CPU_cores=CPU_cores-1; //the parent is one core
		int i; //loop counter for forking and pipes

		pid_t forkpid; //pid
		pid_t pid; // for waiting

		int pipend; //which pipe end a child has
		int childpids[CPU_cores];
		pipend=0; 
		
		int Parentread[CPU_cores][2]; //pipe for the parent to read from
		int Childread[CPU_cores][2]; //pipe for the child to read from
		int LOGMessage[2];//pipe to pass message from child to parent to server
		for(i=0; i<CPU_cores; i=i+1) //create pipes
		{
			if(pipe(Parentread[i])==-1)
			{
				printf("[%s]Error Unable to create pipe for client %s\n",getTime(),IP);
				exit(EXIT_FAILURE);
			}
			if(pipe(Childread[i])==-1)
			{
				printf("[%s]Error Unable to create pipe for client %s\n",getTime(),IP);
				exit(EXIT_FAILURE);
			}
			if(pipe(LOGMessage)==-1)
			{
				printf("[%s]Error Unable to create pipe for client %s\n",getTime(),IP);
				exit(EXIT_FAILURE);
			}

		}


		printf("[%s] lyrebird client: PID  %d connect to server %s on port %d.\n" , getTime(),getpid(),argv[1],htons(PortNum));


		for(i=0; i<CPU_cores; i=i+1) //forking loop
		{
			forkpid  = fork( );
			childpids[i]=forkpid;//save pid of every child in case i need it
			if(forkpid == 0)
			{
				break;
			}
			pipend=pipend+1;// each child will have what pipe number they have 
		}

			if(forkpid>0) //  PARENT///
			{
				int working[CPU_cores];//check to see if child has just competed a job and has a message
				for(i=0; i<CPU_cores; i=i+1) //close pipes ends
				{
					close(Parentread[i][1]);
					close(Childread[i][0]);
					working[i]=0;

				}
				close(LOGMessage[1]);
				int M;
				M=0;

				//loop of reading
  				while(1)
 				{
 					int bytes; //for checking successful read
 					int Length=0; 
 					response = (char *) malloc(3000); //for the encrypted file
 					memset(response,'\0',3000);// make sure the array is clear
 					memset(buffer,'\0',2049);// make sure the array is clear
	 				bytes=read(Parentread[0][0],&pipend,sizeof(pipend));
	 				check(bytes);//check to see if message was read correctly
	 				if(working[pipend]==1)//check to see if there is a logmessage from child
	 				{
	 					bytes=read(LOGMessage[0],response,3000);
	 					check(bytes);//check to see if message was read correctly
	 					write(sockfd,response,strlen(response));
	 				}
	 				write(sockfd, "ready", 5);//tell server there ready
	 				bytes=read(sockfd, &Length, sizeof(Length));//find the length of the incoming message
	 				check(bytes);//check to see if message was read correctly
	 				buffer =(char *) malloc(Length); //make array
				   	n = read(sockfd, buffer , strlen(buffer));//read file location and write location
				   	check(n);//check to see if message was read correctly
				   	if(n<=10) //check to see if it was a stop message
				   	{
				   		free(buffer);
				   		free(response);
				   		break;
				   	}
				   	write(Childread[pipend][1],buffer,strlen(buffer));//give message to child
				   	working[pipend]=1; //that child has message
				   	free(response);
				   	free(buffer);
				 }
				int CL;
				int stp;
				for(CL=0; CL<CPU_cores; CL=CL+1) //close pipe to stop children from reading from them
				{
					close(Parentread[CL][0]);
					close(Childread[CL][1]);

				}
				
				while(1) //read all Log messages from the children
				{
					int bytes=1;
					response = (char *) malloc(3000); //for the encrypted file
					bytes=read(LOGMessage[0],response,3000);
					if(bytes<=0)
	 				{
	 					free(response);
	 					break;
	 				}
					write(sockfd,response,strlen(response));
	 				free(response);
				}
				
				close(LOGMessage[0]);// close

			}
			else if(forkpid==0) //child
			{
				close(LOGMessage[0]);
				char *Read_loc; //dynamic memory for the strings
				close(Parentread[pipend][0]);
				close(Parentread[0][0]);
				close(Childread[pipend][1]);//close unwanted pipes
				int i;

				for(i=1; i<CPU_cores; i=i+1)
				{
					if(i!=pipend)
					{
						close(Parentread[i][0]);
						close(Parentread[i][1]);
						close(Childread[i][1]);
						close(Childread[i][0]);
					}

				}
	   			int bytes;
	   			bytes=1;
	   			FILE * encryptweet; //pointer for file
	   			char temp[1024];
	   			char  *buff; //string
				while(bytes>0) //while there is somthing to read
				{	
					response = (char *) malloc(3000); //for the encrypted file
					buff = (char *) malloc(2049); //for the encrypted file
					memset(response,'\0',3000);// make sure the array is clear
					memset(buff,'\0',2049);// make sure the array is clear
					write(Parentread[0][1],&pipend,sizeof(pipend)); //tell the parent which pipe to write to
					bytes=read(Childread[pipend][0],buff,2049);
					//check(bytes);//check to see if message was read correctly
					if(bytes<13) //make sure wasn't trying to read somthing they should
	        		{// smallest file size should be ./1.txt ./2.txt so thats length 16
	        			break;
	        		}
					Read_loc = (char *) malloc(1024); //for the encrypted file
					memset(Read_loc,'\0',1024); // make sure the array is clear
	        		Read_loc=strtok(buff," "); //break apart the array

	        		Read_loc=strtok(NULL," "); //break apart array
	        		fixchar(Read_loc,temp); // make sure array is just file and not extra character
	        		Read_loc=strtok(Read_loc,"\n");
	  		  		decode(buff,Read_loc); //decode the files
	  		  		encryptweet=fopen(buff,"r"); //opens ecrypted file
	  		  		if(encryptweet!=NULL)// makes sure the files is there
  					{
  						
	  		  			sprintf(response,"[%s] The lyrebird client %s has successfully decrypted %s in process %d.\n" , getTime(),IP,buff,getpid());
	  		  		}
	  		  		else
	  		  		{
	  		  			
	  		  			sprintf(response,"[%s] The lyrebird client %s have encountered an error: Unable to open %s in process %d.\n" , getTime(),IP,buff,getpid());
	  		  		}
	  		  		write(LOGMessage[1],response,3000);//send the respons to partent
	  		  		free(response);
	  		  		free(buff);
				}
				free(buff);
				free(response);
				write(Parentread[pipend][1],"DONE",4);
				close(Parentread[pipend][1]);
				close(Childread[pipend][0]);
				close(LOGMessage[1]);
				exit(0);
			}
			getIP(IP);
			printf("[%s] lyrebird client: PID  %d completed its tasks and is exiting successfully.\n" , getTime(),getpid());
   			mwTerm();
   			return 0;
}