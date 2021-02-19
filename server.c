#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <strings.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>  
#include <fcntl.h>
#include <dirent.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <syslog.h>
#include <time.h>


#include "stream.h"
#include "connection.h"
#include "commands.h"
#include "logging.h"


#define MAX_BLOCK_SIZE (1024*15)


// terminate child process to avoid creating zombie processes
void claim_children(){
	pid_t pid = 1; // set to 1 to start collection child process from first child process
	
	while (pid > 0){
		pid = waitpid(0, (int *) 0, WNOHANG);
	}
} // end claim children

// create daemon process to serve many client at the same time
void daemon_init(void){
	pid_t pid;
	struct sigaction act;
	
	if ((pid = fork()) < 0){
		perror("fork failed!");
		exit(1);
	}
	
	else if (pid > 0)
		exit(0);

	setsid(); // make child the session leader
	chdir(".");
	umask(0);
	
	act.sa_handler = claim_children;
	sigemptyset (&act.sa_mask);
	act.sa_flags = SA_NOCLDSTOP;
	sigaction (SIGCHLD, (struct sigaction *)&act, (struct sigaction *) 0);
} // end daemon_init



// handle the client requests
void serveClient(int nsd, char * ipv4addr){
	char code = '\0';	

	while(1){
/* reading opcode no matter what it is to determin what the command is*/ 
		code = readOpcode(nsd);
/* ****************************************** put ********************************* */
/* command is put and opcode is P */
		if( code == 'P'){	
		
			int p = put(nsd, code, ipv4addr);
			if(p == -1)
				perror("reading size of file name");
			if(p == -2)
				perror("reading file name");
			if(p == -3)
				perror("write opcode");
			if(p == -4)
				perror("write file exist opcode");
			if(p == -5)
				perror("reading size of file");
			if(p == -6)
				perror("opening file");
			if(p == -7)
				perror("write to file");
			if(p == -10)
				perror("logging");
			if (p == 1)
				printf("file successfully created to server\n");
			
		} // end if 'P'


/* ****************************************** get ******************************** */

		if( code == 'G'){
					
			int g = get(nsd, code, ipv4addr);	
			if(g == -1)
				perror("reading size of file name");
			if(g == -2)
				perror("reading file name");
			if(g == -3)
				perror("write opcode");
			if(g == -4)
				perror("write file exist opcode");
			if(g == -5)
				perror("reading size of file");
			if(g == -6)
				perror("opening file");
			if(g == -7)
				perror("write to file");
			if(g == -10)
				perror("logging");
			if(g == 2)
				perror("File does not exist");			
			if(g == 1)
				printf("file successfully created to server\n");	
		} // end if 'G'


/* ****************************************** pwd ******************************** */
		if( code == 'W'){			

			int pw = pwd_c(nsd, code, ipv4addr);	
			if(pw == -1)
				perror("get current path error");
			if(pw == -2)
				perror("write path name opcode");
			if(pw == -3)
				perror("write path length");
			if(pw == -4)
				printf("write path");
			if(pw == -10)
				perror("logging");
		} // end if 'W'

/* ************************************ dir *********************************/
		if( code == 'D'){			

			int d = dir(nsd, code, ipv4addr);	
			if(d == -1)
				perror("write opcode");
			if(d == -2)
				perror("execute command");
			if(d == -3)
				perror("write path length");
			if(d == -4)
				printf("write path");
			if(d == -10)
				perror("logging");
		} // end if 'D'
		
/* ************************************ cd *********************************/
		if( code == 'C'){			
		
			int c = cd(nsd, code, ipv4addr);
			if(c == -1)
				perror("write size of directory name");
			if(c == -2)
				perror("read file name length");
			if(c == -3)
				perror("current directory path");
			if(c == -4)
				perror("directory not found");
			if(c == -5)
				perror("directory change successful - error writing ackode to client socket");
			if(c == -6)
				perror("directory change failed- error writing ackode to client socket");
			if(c == -10)
				perror("logging");
		} // end if 'cd'	
		
	/*********** if reading code failed *****************************/
		if(code == 'Q')
		{
			char *msg = "disconnected";
			if((log_client(ipv4addr, msg)) < 0)
			{
				puts("Error writing to log file - client disconnect.");
			}
			
			// log server shutdown
			FILE *file;
			if((file = fopen(LOG_FILE, "a+")) < 0){
				perror("log file - serving server shutdown");
				return;
			}
			time_t now = time(NULL);
			fprintf(file, "Server serving %s shutdown at %s", ipv4addr, ctime(&now));
			fclose(file);
			return;
		}	
	} // end while
} // end serveClient




/* ************************************* main *************************** */

int main(int argc, char *argv[]){
	
	if(argc == 2)
	{
		if(chdir(argv[1]) == 0)
		{
			printf("Server started in %s\n", argv[1]);
		}
		else
		{
			perror("Unable to start server in specified directory");
		}
	}
	else if (argc > 3)
	{
		printf("Usage: ./%s or ./%s <initial working directory>", argv[0], argv[0]);
	}
	
	pid_t pid;
	
	int nsd; // child socket
	socklen_t cli_addr_len; // hold the length of client address length
	struct sockaddr_in clientAddress; // hold the client address struct
	char client_ip[INET_ADDRSTRLEN]; // hold client ipv4 address
	
	// turn program to daemon
	daemon_init(); 
	
	printf("server started with pid = %d\n", getpid());
	// log server startup to log file
	FILE *file;
	if((file = fopen(LOG_FILE, "a+")) < 0){
		perror("log file");
		return 1;
	}
	time_t now = time(NULL);
	char currentPath[256];
	if(getcwd(currentPath, sizeof(currentPath)) == NULL)
	{
		perror("main - getcwd");
		return 2;
	}
	fprintf(file, "Server started in directory %s at %s", currentPath, ctime(&now));
	fclose(file);
	
	int sd;
	sd = connect_to_TCP();
	
	// else if biding was successful, make the server socket a listening socket for any client to connect
	listen(sd, 5);
	
	// keep server running in an infinit loop
	while(1){
		
		cli_addr_len = sizeof(clientAddress);
		
		//accept the client request for connection
		if ((nsd = accept(sd, (struct sockaddr *) &clientAddress, &cli_addr_len)) < 0 ){ // if (connection) failed
			if (errno == EINTR)
				continue;
			else{
			perror("accept failed!\n");
				exit(1);
			}
		}
		else{
			printf("client accepted!\n");

/* storing client ip address in */				
			struct sockaddr_in* cli_ipv4 = (struct sockaddr_in*)&clientAddress;
			struct in_addr ipAddr = cli_ipv4->sin_addr;
			
			inet_ntop( AF_INET, &ipAddr, client_ip, INET_ADDRSTRLEN );
			printf("client IPv4 address is: %s\n", client_ip);
		}
		//else if server accept client connection, create child process to handle the client
		if ((pid = fork()) < 0 ){ // if fork failed
			perror("server sub-process failed!\n");
			exit(1);
		}
		else if (pid > 0){ // parent process continue to listen to new client and leave the ret to child process
			close(nsd);
			printf("server listening for next client\n");
			continue;
		}
		
		// child handle the connection request from client
		close(sd);
		
		// log client connected
		char *msg = "connected";
		if((log_client(client_ip, msg)) < 0)
		{
			puts("Error writing to log file - client disconnect.");
		}
		
		serveClient(nsd, client_ip);
		printf("server listening for next client\n");
		exit(0);
	} // end while
	return 0;
} // end main

























