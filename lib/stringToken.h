/*
 * This library defines some macros and functions that will seperate a string
 * into serveral tokens.
 */
#ifndef STRINGTOKEN_H
#define STRINGTOKEN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
typedef struct str_token token;
struct str_token{
	char *string_buffer;	/* The string that will be cut */
	char **token;		/* tokens of the string */
	unsigned int token_num;	/* token number of the string */
};

/* Macro defination */
#define PRINT_TOKEN(counter, token_struct) \
do{\
	for(counter = 0; counter<(token_struct).token_num; counter++)\
		printf("token[%d]: %s\n",counter, (token_struct).token[counter]);\
}while(0)

/* functions prototype */
extern void get_token(token *test, const char *buffer, const char *seper);
extern unsigned int get_token_num(const char *buffer, const char *seper);
extern void free_token(token *input);
extern int tokencmp(token *input, const char *buffer);
extern int tokencasecmp(token *input, const char *buffer);

#endif

