/*
File: logging.h
purpose: To contain functions related to the logging of client initiated actions and
		 major server operations (i.e. startup or shutdown).

author: Sha Jalili
date: 2020.10.10
last modification: 2020.10.18

*/

#define LOG_FILE "/tmp/client_log"

/*
Purpose: Create a log in /tmp/client_log with a client ip address specified by ipv4addr
		 and a message specified by msg
Return:
	1) -1 if /tmp/client_log could not be opened or,
	2) 1 if entry was logged successfully
*/
int log_client(char * ipv4addr, char *msg);
