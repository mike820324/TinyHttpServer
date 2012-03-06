/*
 * This is the file handle module. All the stuff that is associated with 
 * file operation is in this file.
 * I try to make it a generic module, that is, it is also work in other os.
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/fileHandle.h"

/* buffer2send is a string that will contains the http header and html contains */
char *buffer2send;	

/*
 * caculate the html file size according the html_file_name
 * and return filesize.
 */
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

/*
 * read the html file to a tempoary buffer.
 * will later string cat to the buffer2send.
 * Return a char pointer which points to the html string.
 */
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

/*
 * combine the http headers and html file.
 * return the total size to the caller.
 * If 0 is return, it means can't find the html file.
 */
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
