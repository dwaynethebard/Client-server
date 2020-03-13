

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <sys/time.h>
#include <sys/select.h>
#include <sys/types.h> 
#define _GNU_SOURCE     /* To get defns of NI_MAXSERV and NI_MAXHOST */
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <unistd.h>
#include <linux/if_link.h>

// specific input checks for server and client
void inputsServer(int inputs) //checks for the appropate number of inputs
{
    if (inputs != 3) 
    {
        fprintf(stderr,"usage: lyrebird.server config_file.txt log_file.txt\n");//change this line to match what is required
        exit(0);
    }
}

void inputsClient(int inputs) //checks for the appropate number of inputs
{
    if (inputs != 3) 
    {
        fprintf(stderr,"usage: lyrebird.client IPaddress port_Number\n");//change this line to match what is required
        exit(0);
    }
}


void getIP(char* IPaddress) //gets the internet IP address of the computer
{
	struct ifaddrs *ifaddr, *ifa;
    int family, s, u;

    if (getifaddrs(&ifaddr) == -1) //make sure it was called properly
    {
        printf("getifaddrs error, now exiting\n");
        exit(EXIT_FAILURE);
    }


    for (ifa = ifaddr, u = 0; ifa != NULL; ifa = ifa->ifa_next, u++) 
    {
        if (ifa->ifa_addr == NULL)
        {
            continue;
        }

        family = ifa->ifa_addr->sa_family;


        if (family == AF_INET || family == AF_INET6) 
        {
            if(u==3)
            {           
                s = getnameinfo(ifa->ifa_addr,
                (family == AF_INET) ? sizeof(struct sockaddr_in) :
                sizeof(struct sockaddr_in6),
                IPaddress, NI_MAXHOST,
                NULL, 0, NI_NUMERICHOST);
            }
        } 
    }
    freeifaddrs(ifaddr);
}

void check(int n) //make sure i read from file correctly
{
	   if (n < 0) 
	   { 
	      printf("ERROR: reading from socket");
	      exit(1);
      }
}
