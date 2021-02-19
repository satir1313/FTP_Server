/*
File: connection.h
purpose: Contains functions regarding establishing a TCP socket

author: Sha Jalili
date: 2020.10.10
last modification: 2020.10.18

*/

#define SERVER_PORT 41420
#define BUFFER_SIZE 1024

/*
Purpose: Establishes a TCP socket that accepts connections on all network interfaces 
		 to be used as a server socket
Return:
	1) -1 if socket failed to be created or,
	2) -2 if binding address to socket failed or,
	3) socket file descriptor if successful
*/
int connect_to_TCP();


