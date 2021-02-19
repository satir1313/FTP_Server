
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "logging.h"

int log_client(char * ipv4addr, char *msg){

	FILE *file;
	time_t now = time(NULL);
	char timebuf[30];
	
	if((file = fopen(LOG_FILE, "a+")) < 0){
		return -1;
	}
	
	strcpy(timebuf, ctime(&now));
	timebuf[strlen(timebuf)-1] = '\0';	// remove newline that ctime() appends just incase we want to add stuff after the time
	
	fprintf(file, "Client [ip: %s]: %s at %s\n", ipv4addr, msg, timebuf);
	fclose(file);
return 1;
}
