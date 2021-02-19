#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>

#include "token.h"
#include "stream.h"
#include "commands.h"



#define SERVER_PORT 41420
#define MAX_BLOCK_SIZE (1024*15)

char commandToOption(char command[], int size){
	char options = 0;
	
	if(command[size-1] == '\n'){
		command[size-1] = '\0';
		--size;
	}
	
	if (strstr(command, "put") != NULL)
		options = 'P';
	if (strstr(command, "get") != NULL)
		options = 'G';
	if (strcmp(command, "pwd") == 0)
		options = 'W';
	if (strcmp(command, "dir") == 0)
		options = 'D';
	if (strstr(command, "cd") != NULL)
		options = 'C';
	if (strstr(command, "lcd") != NULL)
		options = 'U';
	if (strstr(command, "lpwd") != 0)
		options = 'L';
	if (strstr(command, "ldir") != 0)
		options = 'I';
return options;
}


int main(int argc, char **argv){                      

	int sd; // server socket descriptor
	int n;
	int reader; // read from socket descriptor
	int writer; // write from socket descriptor
	int i = 0; // counter		
	int fileHandler;
	int sizeOfFile, status; // hold the size of file
	
	
	char fileName[25]; // hold the name of file to ttransfer
	char buffer[BUFFER_SIZE]; // hold data to transfer via socket
	char host[BUFFER_SIZE]; //hold the host name

	
	struct stat fileStat;
	
	struct sockaddr_in serverAddress;
	struct hostent *hp;
	
	if(argc == 1){ // if user doesn't specify the host name, assume the local host
		strcpy(host, "localhost");
	}
	else if (argc == 2){ // if user specify the local host name
		strcpy(host, argv[1]);
	}
	else{ // invlid inout from user
		printf("Invalid host name: %s [<server host name>]\n", argv[0]);
		exit(1);
	}
	
	// if user input is valid continue to make a connection to the server
	bzero((char *) &serverAddress, sizeof(serverAddress));
	
	// define address properties
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(SERVER_PORT);
	
	// get the local host name
	if ((hp = gethostbyname(host)) == NULL){ // if can't find host name
		printf("host %s not found!\n", host);
		exit(1);
	}
	
	// else host address has found
	serverAddress.sin_addr.s_addr = * (u_long *) hp->h_addr;
	
	// create TCP socket to connect
	if ((sd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("server socket failed!\n");
		exit(1);
	}
	
	// else if socket created, make coonection to server
	if ((connect(sd, (struct sockaddr *) &serverAddress, sizeof(serverAddress))) < 0){
		perror("connection failed\n");
		exit(1);
	}else { printf("Connected to server\n");}
	
	// counting number of client with i
	

	while(1){

		printf("Available Commands:\n1- get\n2- put\n3- pwd\n4- dir\n5- cd\n6- ldir\n7- lcd\n8- lpwd\n9- quit\n");
		//display prompt
		printf("> ");
		fgets(buffer, BUFFER_SIZE, stdin);
		
		
		if (strcmp(buffer, "quit\n") == 0){
			printf("Bye from client\n");
			close(sd);
			exit(0);
		}
		
		reader = strlen(buffer);

		char option = ' ';
		option = commandToOption(buffer, reader);
		//char code;
		//FILE *file;
		int pp;
		int gg = 0;
		int pw;
		int dd;
		int cc;
		
		
		switch(option){
		
			case 'P': // case for put command
				
				pp = put(sd, buffer);
				
				if(pp == -1)
					perror("Error opening file");
				if(pp == -4)
					perror("cannot transfer the file");
				if(pp == -6)
					printf("file already exist!");
			break;			
			
			/* **************************** Get *********************** */
			case 'G': // case for get command

				gg = get(sd, buffer);
				
				if(gg == -1)
					perror("write error");
				if(gg == -4)
					perror("cannot transfer the file");
				if(gg == -6)
					perror("write error");
				if(gg == -7)
					perror("write error 2");
			break;

			
			/* **************************** pwd *********************** */
			case 'W': // case for pwd command

				pw = pwd_c(sd, 'W');
				
				if(pw == -2)
					perror("cannot access the path");

			break;
			
			
			/* **************************** dir *********************** */
			case 'D': // case for dir command
				dd = dir(sd, 'D');
				
				if(dd == -2)
				perror("cannot access the path");
			break;
			
			
			/* **************************** cd ************************ */
			case 'C': // case for cd command

				cc = cd(sd, buffer);
				
				if(cc == -1)
					perror("write error");
				if(cc == -2)
					perror("error writing path to server");
				if(cc == -3)
					perror("could not change the directory");
			break;
			
			case 'U':
				lcd(buffer);
			break;
			
			case 'L':
				lpwd();
			break;
			
			case 'I':
				ldir();
			break;
			
			default:
				puts("Invalid command.");
			break;
			
		} // end switch
	
	} // end while
		
return 0;
}







