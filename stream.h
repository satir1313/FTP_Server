/*
File: stream.h
purpose: to contain functions to be use reeading and writing bytes from/to a TCP socket and files in the binary format
assumption: the socket used in these function is a blocking TCP socket.

author: Sha Jalili
date: 2020.10.10
last modification: 2020.10.21

*/

#define MAX_BLOCK_SIZE (1024*15)

/*
Purpose: Write a one byte ASCII opcode code to the socket wsd
Return:
	1) -1 if writing opcode to wsd failed or,
	2) 1  if writing opcode to wsd was successful
*/
int writeOpcode(int wsd, char code);

/*
Purpose: Read a one byte opcode from the given socket osd
Return:
	1) Char 'Q' if reading opcode failed or,
	2) the opcode read from the socket osd as a char
*/
char readOpcode(int osd);

/*
Purpose: Read two bytes from the given socket named sock
Return:
	1) -1 if failed to read 2 bytes from the given socket named sock or,
	2) the value of the two bytes read as a short
*/
short read_two_byte(int sock);

/*
Purpose: read files in the binary format and write the bytes to the given socket
Return:
	1) -1 if failed to write to socket in the binary forman with given size of bytes or,
	2) -2 if failed to read file in the binary forman with given size of bytes or,
	3) 1 if success
*/
int send_data(int fp, int sd, int size_file);

/*
Purpose: read bytes from given socket and write them in the binary format to a given file
Return:
	1) -1 if failed to write to file in the binary forman with given size of bytes or,
	2) -2 if failed to read bytes from socket with given size of bytes or,
	3) 1 if success
*/
int write_file(int sockfd, int fp, int nbytes);

/*
Purpose: Determine if a file with name specified by *file_name exists
Return (char):
	1) 1 if the file already exists or,
	2) 0 if file failed to open
	3) 2 in all other cases
*/
char file_exist_handler(char *file_name);

/*
Purpose: Determine if a file with name specified by *file_name can be opened
Return (char):
	1) 0 if the file was able to be opened successfully and exists or,
	2) 2 if the file was unable to be opened or,
	3) 1 in all other cases
*/
char file_get_handler(char *file_name);

/*
Purpose: Write two bytes to a the given socket named sd
Return:
	1) -1 if two bytes were unable to be written or,
	2) the total value of bytes written as a short
*/
size_t write_two_byte (int sd, short length);
