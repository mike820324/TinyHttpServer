#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lib/stringToken.h"

unsigned int 
get_token(token *test, const char *buffer, const char *seper){
	unsigned int counter;
	char *p;
	char *temp_buffer;
	
	/* calculate the total token number */
	test->token_num = get_token_num(buffer, seper);
	
	test->string_buffer = (char *)malloc(sizeof(char)*strlen(buffer)+1);
	strncpy(test->string_buffer, buffer, strlen(buffer));
	test->string_buffer[strlen(buffer)] = '\0';

	#ifdef DEBUGME
	printf("%s\n",test->string_buffer);
	#endif
	
	/* double check just in case something went wrong when string copy */
	if( test->token_num != get_token_num(test->string_buffer, seper) ){
		printf("something wrong with the token number.\n");
		return 0;
	}

	test->token = (char **)malloc(sizeof(char *)*(test->token_num));
	test->token_length = (unsigned int *)malloc(sizeof(unsigned int)*(test->token_num));
	p = strtok(test->string_buffer, seper);
	for(counter = 0; p != NULL; counter++ ){
		test->token[counter] = p;
		test->token_length[counter] = strlen(test->token[counter]);
		p = strtok(NULL, seper);
	}
	
	return 1;	
}

unsigned int 
get_token_num(const char *buffer, const char *seper){
	char *p;
	char *temp;

	temp = (char *)malloc(sizeof(char)*strlen(buffer));
	strncpy(temp, buffer, strlen(buffer));
	temp[strlen(buffer)] = '\0';

	unsigned int counter = 0;
	p = strtok(temp,seper);
	while(p != NULL){
		counter ++;
		p = strtok(NULL, seper);
	}
	return counter;
}

int 
tokencmp(token *input, const char *buffer){
	unsigned int i;
	for(i = 0 ; i < input->token_num ; i++){
		if(strncmp(input->token[i], buffer, strlen(buffer)) == 0) return 0;
		else continue;
	}	
	return 1;
}

int 
tokencasecmp(token *input, const char *buffer){
	unsigned int i;
	for(i = 0 ; i < input->token_num ; i++){
		if(strncasecmp(input->token[i], buffer, strlen(buffer)) == 0) return 0;
		else continue;
	}	
	return 1;
}

void 
free_token(token *input){
	free(input->string_buffer);
	free(input->token);
}
