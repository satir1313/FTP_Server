/*
file: stream.c
author: Sha Jalili
date: 2020.10.10
last modification: 2020.10.18
*/



#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h> 
#include <stdio.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>


#include "stream.h"
#include "commands.h"
#include "logging.h"

#define LOG_ERR -10 


int put(int nsd, char code, char * ipv4addr){
	
		if ((log_client(ipv4addr, "Client attempted put command")) < 0) {
			return (LOG_ERR);
		}
/* reading the size of file name*/ 
			short sizeOfFile;
			if((sizeOfFile = read_two_byte(nsd)) < 0)
				return (-1);
						
			short file_name_len = ntohs(sizeOfFile); 	
		
/* reading the file name*/ 			
			char file_name[25];

		  	int num_byte;
		  	if((num_byte = read(nsd, file_name, file_name_len)) <= 0)
		  		return (-2);
		  		
		  	file_name[num_byte] = '\0';

			
		  	char msg[BUFFER_SIZE] = "Client attempted to put file: ";
		  	strcat(msg, file_name);
  			if ((log_client(ipv4addr, msg)) < 0) {
				return (LOG_ERR);
			}

/* server responsing by one byte opcode and one byte ack */         
         
/* writing opcode to client */
			int writer;
			if((writer = write(nsd, (char *) &code, 1)) < 0)
				return (-3);
			
			char file_exist = file_exist_handler(file_name);

			
/* writing acknowledgemnet code to client */
			if ((writer = write(nsd, (char *) &file_exist, 1)) < 0)
			return (-4);


/* waitning for client to response */
			
/* reading opcode if it's A*/
			int code_res = readOpcode(nsd);

			if(code_res == 'A'){
				/* get the size of file */
				int size_file_to_write;

/* reading the size of file comes from client */				
				if((read(nsd, (char *) &size_file_to_write, 4)) < 0)
					return (-5);
					
				size_file_to_write = ntohl(size_file_to_write); 	
				
/* Reading content of file and save file*/
			    
				int fp = open(file_name, O_WRONLY | O_CREAT, 0777);
				if(!fp ){
					return (-6);
				}
				if((write_file(nsd, fp, size_file_to_write)) < 0)
					return (-7);
				close(fp);
				
				char temp[BUFFER_SIZE];
				strcpy(temp, file_name);
				strcpy(msg, " successfully transferred to server");
				strcat(temp, msg);
	  			if ((log_client(ipv4addr, temp)) < 0) {
					return (LOG_ERR);
				}

			}
return 1;		
}



int get(int nsd, char code, char * ipv4addr){

	if ((log_client(ipv4addr, "Client attempt get command")) < 0) {
		return (LOG_ERR);
	}
		
	struct stat fileStat;
	char temp[BUFFER_SIZE]; // stores error/success message for log file
		
/* reading the size of file name*/ 
	short sizeOfFile;
	if(( sizeOfFile = read_two_byte(nsd)) < 0)
	return (-1);
							
	short file_name_len = ntohs(sizeOfFile); 	

/* reading the file name*/ 			
	char g_file_name[25];

  	int num_byte;
  	if ((num_byte = read(nsd, g_file_name, file_name_len)) < 0)
  		return (-2);
  		
  	g_file_name[num_byte] = '\0';

	char msg[BUFFER_SIZE] = "Client attempted to get file: ";
  	strcat(msg, g_file_name);
	if ((log_client(ipv4addr, msg)) < 0) {
		return (LOG_ERR);
	}
/* server responsing by one byte opcode and one byte ack */         	

	char g_file_exist = file_get_handler(g_file_name);
	
				
	if(g_file_exist == '0'){
/* writing opcode G to client */ 		
		if((writeOpcode(nsd, 'G')) < 0)
			return (-3);
		
/* writing file existance to client */ 
		if((writeOpcode(nsd, g_file_exist)) < 0)
			return (-4);

/* dealing with file */
		stat(g_file_name, &fileStat);
		int g_file_size  = fileStat.st_size;
		int size = g_file_size;
		g_file_size = htonl(g_file_size); 
/* writing the file size to client */				
		if((write(nsd, (char *) &g_file_size, 4)) < 0)
			return (-5);
/* writing the file content to client */				
		int file = open(g_file_name, O_RDONLY , 0777);
		if(!file ){
			perror("opening file :");
			return (-6);
		}

		if ((send_data(file, nsd, size)) < 0)
			return (-7);
			
		close(file);
		
		strcpy(temp, g_file_name);
		strcpy(msg, " successfully transferred to client");
		strcat(temp, msg);
		if ((log_client(ipv4addr, temp)) < 0) {
			return (LOG_ERR);
		}
	}
	else{
		if((writeOpcode(nsd, 'G')) < 0)
		{
			perror("writing opcode - get");
		}
		if((writeOpcode(nsd, g_file_exist)) < 0)
		{
			perror("writing opcode - get failed");
		}
		
		strcpy(temp, g_file_name);
		strcpy(msg, " was unable to be transferred to client");
		strcat(temp, msg);
		if ((log_client(ipv4addr, temp)) < 0) {
			return (LOG_ERR);
		}
		return 2;
	}
return 1;
}



int pwd_c(int nsd, char code, char * ipv4addr){

	if ((log_client(ipv4addr, "Client attempted pwd command")) < 0) {
		return (LOG_ERR);
	}
	
	char curr_dir[BUFFER_SIZE];
	if ((getcwd(curr_dir, sizeof(curr_dir))) == NULL){
		return (-1);
	} // end if


/* writing lenght of file name*/
	if ((writeOpcode(nsd, 'W')) < 0)
	return (-2);			
	
/* writing lenght of file name*/
	int len = strlen(curr_dir);
	short length =  len;
	
	size_t wr;
	if((wr = write_two_byte(nsd, length)) == -1){
		return (-3);
	}
	
/* writing path*/

	if((write(nsd, curr_dir, len)) < 0)
	{
		if ((log_client(ipv4addr, " pwd unable to be sent to client")) < 0) {
			return (LOG_ERR);
		}	
		return (-4);
	}
	else {
		if ((log_client(ipv4addr, " pwd executed and send to client successfully")) < 0) {
			return (LOG_ERR);
		}	
	}
		
return 1;			
}



int dir(int nsd, char code, char * ipv4addr){

	if ((log_client(ipv4addr, "Client attempted dir command")) < 0) {
		return (LOG_ERR);
	}	
/* writing opcode to client*/
	if((writeOpcode(nsd, 'D')) < 0)
		return (-1);			
	
	char result[BUFFER_SIZE];
	
	FILE *ls = popen("ls", "r");
	if(ls == NULL){
		return (-2);
	}
	
	int n;
	while((n=fread(result, 1, sizeof(result)-1, ls)) > 0){
		result[n] = '\0';
	}
	pclose(ls);
/* writing lenght of file name*/
	int len = strlen(result);
	short length =  len;
	size_t wr;
	if((wr = write_two_byte(nsd, length)) == -1){
		return (-3);
	}
	
/* writing path*/
	if((write(nsd, result, len)) < 0)
	{
		if ((log_client(ipv4addr, " directory listing (dir) failed to be sent to client")) < 0) {
			return (LOG_ERR);
		}	
		return (-4);
	}
	else{
		if ((log_client(ipv4addr, " directory listing (dir) executed and send to client successfully")) < 0) {
			return (LOG_ERR);
		}	
	}
return 1;
}




int cd(int nsd, char code, char * ipv4addr){

	if ((log_client(ipv4addr, " Client attempted cd command")) < 0) {
		return (LOG_ERR);
	}	

/* reading the size of file name*/ 
	short sizeOfFile;
	if ((sizeOfFile= read_two_byte(nsd)) == -1)
		return (-1);
								
	short file_name_len = ntohs(sizeOfFile); 	

/* reading the path name*/ 			
	char path_name[BUFFER_SIZE];

  	int num_byte;
  	if((num_byte = read(nsd, path_name, file_name_len)) < 0)
  		return (-2);
  	
  	path_name[num_byte] = '\0';


	char cwd[BUFFER_SIZE];		

 	if ((getcwd(cwd, BUFFER_SIZE)) == NULL){
 		return (-3);
	}
	
	
	strcat(cwd, "/");
	strcat(cwd, path_name);
 	
 	char msg[BUFFER_SIZE];
/* change the directory */

	if(chdir(cwd) == 0){
		if((writeOpcode(nsd, '0')) < 0)
			return (-5);
		
		strcpy(msg, "Directory successfully changed to ");
		strcat(msg, path_name);
		if ((log_client(ipv4addr, msg)) < 0) {
			return (LOG_ERR);
		}
	}
	else{
		if((writeOpcode(nsd, '1')) < 0)
			return (-6);
		strcpy(msg, "Directory in server has not changed successfully!");
		if ((log_client(ipv4addr, msg)) < 0) {
			perror("Error writing to log - cd fail");
		}
		return (-4);		
	}
		
return 1;	
}
































