/*
File: token.h
purpose: to separate the command line into sequence of tokens
assumption: the tokens in the line are all sepersted by at least one delimiter including white space, \t or \n

author: HX
date: 2006.09.14
last modification: 2020.10.03

not fully tested and may contain error 
*/

#define MAX_NUM_TOKENS 1000
#define tokenSeparator " \t\n"

/*
purpose: separate the line into tokens and remove the token separator and replaced by '\0' at the ith index of the array of tokens

return: the number of token it finds in the command line or -1 if the array of tokens in too small

assumption: the array passed to tokeniser function must has at least MAX_NUM_TOKENS elements

note: if return value ntoken >= 0, then the token[ntokens] is set to null
*/

int tokeniser (char line[], char*token[]);
