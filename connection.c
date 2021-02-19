/*
file: connection.c
author: Sha Jalili
date: 2020.10.10
last modification: 2020.10.18
*/

#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <dirent.h>
#include <errno.h>
#include <strings.h>

#include "connection.h"

int connect_to_TCP(){

	int sd; // hold the description of server socket
	
	struct sockaddr_in serverAddress; // hold the server address struct

	// create server socket to communicate with client	
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ){ // if socket faild to be created
		return (-1);
	}
	//else if socket successfully created, then define the socket properties
	bzero((char *) &serverAddress, sizeof(serverAddress)); // initialise all serverAddress properties to null
	
	serverAddress.sin_family = AF_INET; // define the type of connection environment
	serverAddress.sin_port = htons(SERVER_PORT); // define type and number of port is used in the program
	//define what interfaces this server will accept to connect, in this case any address
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	
	// binding addr to socket	
	if ((bind(sd, (struct sockaddr *) &serverAddress, sizeof(serverAddress))) < 0 ){ // if binding failed
		return (-2);
	}
	
	return (sd);
}
