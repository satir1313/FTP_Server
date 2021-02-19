/*
File: commands.h
purpose: to execute commands given by client on server or on the local host machine, using TCP socket trough the intenet connection
assumption: the socket used in these function is a blocking TCP socket.
			- file must exist on the local host to be transferred to server 
			- client has all permission to make changes and create files and 					navigate on the server

author: Sha Jalili
date: 2020.10.10
last modification: 2020.10.21

*/

#define BUFFER_SIZE 1024

/*
Purpose: send file through TCP socket from client to server 
Return:
	1) -1 if reading socket for two bytes of size of file name failed,
	2) -2 if reading socket for file name failed,
	3) -3 if writing the opcode failed,
	4) -4 if writing the existance file opcode in failed,
	5) -5 if reading socket for four bytes of size of file failed,
	6) -6 if opening file to create failed,
	7) -7 if writing to file failed,
	8) -10 (LOG_ERR) if logging failed
	9) 1 if success
*/

int put(int nsd, char code, char * ipv4addr);

/*
Purpose: receive file through TCP socket from server 
Return:
	1) -1 if reading socket for two bytes of size of file name failed,
	2) -2 if reading socket for file name failed,
	3) -3 if writing the opcode failed,
	4) -4 if writing the existance file opcode in failed,
	5) -5 if reading socket for four bytes of size of file failed,
	6) -6 if opening file to create failed,
	7) -7  if sen_data function return negative number,
	8) -10 (LOG_ERR) if logging failed
	9) 2 if success but file does not exist
	10) 1 if success
*/
int get(int nsd, char code, char * ipv4addr);

/*
Purpose: send the apth of current directory in server to client 
Return:
	1) -1 if getting current directory path in server failed,
	2) -2 if writing opcode failed,
	3) -3 if writing length of path failed,
	4) -4 if writing the path to socket failed,
	5) -10 (LOG_ERR) if logging failed
	6) 1 if success
*/
int pwd_c(int nsd, char code, char * ipv4addr);

/*
Purpose: send the list of files and directory in the current path in server to client
Return:
	1) -1 if writing opcode failed,
	2) -2 if  executing command "ls" failed,
	3) -3 if writing length of path failed,
	4) -4 if writing the path to socket failed,
	5) -10 (LOG_ERR) if logging failed
	6) 1 if success
*/
int dir(int nsd, char code, char * ipv4addr);

/*
Purpose: change the directory in server to given derectory from client
Return:
	1) -1 if reading socket for two bytes of size of new directory path failed,
	2) -2 if reading socket for length of dir name failed,
	3) -3 if getting current pathfailed,
	4) -4 if changing directory failed,
	5) -5 if writing size of message to socket failed,
	6) -6 if writing the response message to socket failed,
	6) -10 (LOG_ERR) if logging failed
	7) 1 if success
*/
int cd(int nsd, char code, char * ipv4addr);
