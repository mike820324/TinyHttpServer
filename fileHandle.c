#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/fileHandle.h"

char *buffer2send;
unsigned int 
calculate_file_size(const char *html_file_name){
	FILE *input = fopen(html_file_name, "rb");
	if(input == NULL) return 0;

	unsigned int filesize = 0;
	fseek(input, 0, SEEK_END);
	filesize = ftell(input);
	fclose(input);
	return filesize;
}

char* read2Buffer(const char *html_file_name){
	unsigned int filesize = calculate_file_size(html_file_name);
	FILE *input = fopen(html_file_name, "rb");
	if(input == NULL)	return NULL;

	char *buffer;
	unsigned int counter = 0;
	buffer = (char *)malloc(sizeof(char)*filesize);
	memset(buffer, 0, filesize);
	
	while(1){
		if(feof(input)) break;	
		buffer[counter++] = fgetc(input);
	}
	buffer[filesize] = '\0';		
	fclose(input);
	return buffer;
}

unsigned int 
createBuffer2Send(const char *http_header_type, const char * html_file_name){
	char *buffer = read2Buffer(html_file_name);
	if(buffer == NULL) return 0;

	unsigned int filesize = calculate_file_size(html_file_name);

	filesize += strlen(http_header_type);
	buffer2send = (char *)malloc(sizeof(char) * filesize+10);
	memset(buffer2send, 0, filesize+10);
	strncpy(buffer2send, http_header_type, strlen(http_header_type));
	strncat(buffer2send, buffer, strlen(buffer));

	free(buffer);
	return filesize;
}
