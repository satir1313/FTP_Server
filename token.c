/*
file: token.c
author: HX
date: 2006.9.14
last modification: 2020.10.03
*/

#include <string.h>
#include "token.h"

int tokeniser(char line[], char *token[]){
	
	char *tk;
	int i = 0;

	tk = strtok(line, tokenSeparator);
	
	while(tk != NULL){
		++i;
		if (i >= 1000){
			i = -1;
			break;
		} // end while
		
		tk = strtok(NULL, tokenSeparator);
		token[i] = tk;
	} // end while
	
	return i;
} // end tokeniser
