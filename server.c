#include <stdio.h>
#include <string.h>   //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>   //close
#include <arpa/inet.h>    //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <netdb.h>
#include <netinet/in.h>
#include "FUNctions.h" 
#include "socketfunctions.h" 
#include <sys/types.h>
#define TRUE   1
#define FALSE  0
#include <time.h>
#include "memwatch.h"
#define _GNU_SOURCE




int main(int argc , char *argv[])
{
    inputsServer(argc);//check to see right number of inputs
    FILE * config_file; //pointer for file
    OpenReadFile(argv[1]); //check to see if config file exists
    config_file=fopen(argv[1],"r");
    FILE * log_file; //pointer for file
    log_file=fopen(argv[2],"w+"); //opens ecrypted file
    int opt = TRUE;
    int NewSockets ; //the main socket
    int addrlen ; //address
    int new_socket ; //new sockets
    int client_socket[300] ; //array of sockets
    int max_clients = 300 ; //max number of clients
    int activity; //select variable checker
    int i ;//loop
    int ReadingSize; //reading value
    int sd; //file number
	int max_sd; //make file number for select
    struct sockaddr_in address; //address information
     
     
    fd_set readfds; //the set of sockets for select

    //initialise all client_socket[] to 0 so not checked array of sockets
    for (i = 0; i < max_clients; i++) 
    {
        client_socket[i] = 0;
    }
     
    //create the socket
    if( (NewSockets = socket(AF_INET , SOCK_STREAM , 0)) == 0) 
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
 
    //set socket to allow multiple connections 
    if( setsockopt(NewSockets, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0 )
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    

    char IP[NI_MAXHOST];
    getIP(IP); //get ip information

    //type of socket created
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(IP);
    address.sin_port = htons( 0 );
     
    //bind the socket to localhost port 2525
    if (bind(NewSockets, (struct sockaddr *)&address, sizeof(address))<0) 
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
	
    //try to specify maximum of 25 pending connections for the master socket
    if (listen(NewSockets, 25) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
     
    //accept the incoming connection
    addrlen = sizeof(address);
    char  *buffer; 
    int clientNumber=0;
    getsockname(NewSockets , (struct sockaddr*)&address , (socklen_t*)&addrlen);
    printf("[%s] lyrebird.server: PID %d on host \n%s, port %d.\n" , getTime(),getpid(),IP,ntohs(address.sin_port));


	while(TRUE) 
    {
        buffer =(char *) malloc(3000);
        memset(buffer,'\0',3000);
        //clear the socket set
        FD_ZERO(&readfds);
 
        //add New socket to set
        FD_SET(NewSockets, &readfds);
        max_sd = NewSockets;
		
        //add child sockets to set
        for ( i = 0 ; i < max_clients ; i++) 
        {
            //socket descriptor
			sd = client_socket[i];
            
			//if valid socket descriptor then add to read list
			if(sd > 0)
				FD_SET( sd , &readfds);
            
            //highest file descriptor number, need it for the select function
            if(sd > max_sd)
				max_sd = sd;
        }
        if(feof(config_file))
        {
            if(clientNumber==0)
            {
                break;
            }
        }
        //wait for an activity on one of the sockets , timeout is NULL , so wait indefinitely
        activity = select( max_sd + 1 , &readfds , NULL , NULL , NULL);
   
        if ((activity < 0) && (errno!=EINTR)) 
        {
            printf("select error");
        }
         
        //If something happened on the master socket , then its an incoming connection
        if (FD_ISSET(NewSockets, &readfds)) 
        {
            if ((new_socket = accept(NewSockets, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }
          fprintf(log_file,"[%s] Successfully connect to lyrebird client %s \n" , getTime(),inet_ntoa(address.sin_addr));
          fflush(log_file);  //inform user of socket number - used in send and receive commands
       
       
            //send new connection greeting message
            write(new_socket, "Welcome",7 ); 
            //add new socket to array of sockets
            for (i = 0; i < max_clients; i++) 
            {
                //if position is empty
				if( client_socket[i] == 0 )
                {
                    client_socket[i] = new_socket;
                    
					clientNumber=clientNumber+1;
					break;
                }
            }
        }
        //else its some IO operation on some other socket :)
        for (i = 0; i < max_clients; i++) 
        {
            sd = client_socket[i]; 
             
            if (FD_ISSET( sd , &readfds)) //check to see if this cilent sent us a message
            {
                if(feof(config_file))//if all file have been givem out then messages are either dissconects or Log message
                {
                    ReadingSize = read( sd , buffer, 1024);
                    if ( strcmp(buffer,"")==0) //if dissconect
                    {
                         clientNumber=clientNumber-1; //lower the number of clients
                         getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen);
                         fprintf(log_file,"[%s] lyrebird client %s has disconnected expectedly \n" ,getTime(), inet_ntoa(address.sin_addr));
                        //Close the socket and mark as 0 in list for reuse
                        close( sd );
                        client_socket[i] = 0;
                        memset(buffer,'\0',3000);
                    }
                 
                    // a completion message from client
                    else
                    {
                        if(ReadingSize==5) //if it was a message asking for more tell them to stop
                        {
                           int length=4;
                           write(sd , &length , sizeof(length));
                           write(sd , "STOP" , 4);  
                        }
                        else
                        {
                            	fprintf(log_file, "%s",buffer ); //print to Log file
                           
                        }
                        memset(buffer,'\0',3000);// make sure the array is clear
                    }

                }
                else
                { 
                    //Check if it was for closing
                    if ((ReadingSize = read( sd , buffer, 1024)) == 0)
                    {
                        //Somebody disconnected , get his details and print
                        clientNumber=clientNumber-1; //lower the number of clients
                        getpeername(sd , (struct sockaddr*)&address , (socklen_t*)&addrlen);
                        fprintf(log_file,"[%s]lyrebird client %s has disconnected unexpectedly \n" ,getTime(), inet_ntoa(address.sin_addr));
                         
                        //Close the socket and mark as 0 in list for reuse
                        close( sd );
                        client_socket[i] = 0;
                        memset(buffer,'\0',3000);// make sure the array is clear
                    }
                    else
                    {
                        if(ReadingSize==5) //if was asking for new files
                        {
                            if(fgets(buffer,3000,config_file)!=NULL) //check to see if file was empty
                            {
                                int length=0;
                                length=strlen(buffer);
                                write(sd , &length , sizeof(length));
                                write(sd , buffer , length); 
                                buffer=strtok(buffer," ");
                                fprintf(log_file,"[%s] The lyrebird client %s has been given the task of decrypting %s \n" , getTime(),inet_ntoa(address.sin_addr),buffer);
                                fflush(log_file);  //inform user of socket number - used in send and receive commands
                            }
                            else //we got nothing from file so stop
                            {
                             int length=4;
                           write(sd , &length , sizeof(length));
                           write(sd , "STOP" , 4);  
                            }
                        }
                        else //if the it was a competion message to put in log file
                        {
                           fprintf(log_file, "%s",buffer );
                        }
                        memset(buffer,'\0',3000);// make sure the array is clear

                    }
                }
            }
        }
        free(buffer);
    }
    free(buffer);
    printf("[%s] lyrebird.server: PID %d completed its\n tasks and is exiting successfully.\n" , getTime(),getpid());

    return 0;
}