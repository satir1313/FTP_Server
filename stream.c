/*
file: stream.c
author: Sha Jalili
date: 2020.10.10
last modification: 2020.10.21
*/


#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>  
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#include <strings.h>
#include <arpa/inet.h>


#include "stream.h"




int writeOpcode(int wsd, char code){

	int te;
	if ((te = (write(wsd, (char *) &code, 1))) < 0) {
		return -1;
	}
return 1;
} // end write opcode function



// function to read opcode 
char readOpcode(int osd){
	char code; 
		if ((read(osd, (char *) &code, 1)) != 1){ // if reading failed
			code = 'Q';
		} // end if
	return code;
}

// reading two bytes
short read_two_byte(int sock){

	short sizeOfFile;
	if (read(sock, (char *) &sizeOfFile, 1) != 1){
		perror("first byte of file size"); 
		return (-1);
	}
	if (read(sock, (char *) (&sizeOfFile)+1, 1) != 1){
		perror("second byte of file size"); 
		return (-1);
	}	
	
return sizeOfFile;
}


// data sender
int send_data(int fp, int sd, int file_size){
	
	int n, nr, nw;
	char *buffer = malloc(sizeof(char) * file_size);

	for (n=0; n<file_size; n += nr) {
		if((nr = read(fp, buffer+n, file_size-n)) <= 0)
			return (-2);
	}

	for (n=0; n<file_size; n += nw) {
		if ((nw = write(sd, buffer+n, file_size-n)) <= 0) 
			return (-1);
	}

free(buffer);
	
return 1;
}



// writing dta to file
int write_file(int sockfd, int fp, int nbytes){

	int n, nr, nw;
	char *buffer = malloc(sizeof(char) * nbytes);

	for (n=0; n<nbytes; n += nr) {
		if((nr = read(sockfd, buffer+n, nbytes-n)) <= 0)
			return (-2);
	}

	for (n=0; n<nbytes; n += nw) {
		if ((nw = write(fp, buffer+n, nbytes-n)) <= 0) 
			return (-1);
	}

free(buffer);
return 1;
}


// file existance handler
char file_exist_handler(char *file_name){
				
	opendir(".");
	//umask(0777);

	int des_fd;
	
	des_fd = open(file_name, O_WRONLY , 0644);
	
	if(errno == EEXIST)
		return '1';
	else if(des_fd < 0) 
		return '0';
	//else if(!des_fd)
		//return '3';
	else {
		printf("file successfully evaluated for existance : %s\n", file_name);
		return '2';
	}
	close(des_fd);
}// end file exist handler






char file_get_handler(char *file_name){
	
	opendir(".");
	//umask(0777);

	int des_fd;
	
	//des_fd = open(file_name, O_WRONLY, 0777);
	
	if((des_fd = open(file_name, O_WRONLY, 0644)) > 0)
	{
		return '0';
	}
	else if(des_fd < 0) 
		return '1';
	else {
		return '2';
	}
	close(des_fd);
}


size_t write_two_byte (int sd, short length){

	length = htons(length);
	
	if (write(sd, (char *) &length, 1) != 1){
		return (-1);
	}   
	if (write(sd, (char *) (&length)+1, 1) != 1){
		return (-1);
	}
	
return (length);		
}






































