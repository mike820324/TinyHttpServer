#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <ctype.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/wait.h>

#include "lib/stringToken.h"
#include "lib/httpHeader.h"
#include "lib/fileHandle.h"

unsigned int
getHTML(enum http_header header_type, const char *html_file_name){
	unsigned int filesize = 0;
	
	if(header_type == http_200){
		printf("using http 200\n");
		filesize = createBuffer2Send(http_header_200, html_file_name);
		return filesize;
	}else if(header_type == http_302){
		printf("using http 302\n");
		filesize = createBuffer2Send(http_header_302, html_file_name);
		return filesize;
	}else if(header_type == http_404){
		printf("using http 404\n");
		filesize = createBuffer2Send(http_header_404, html_file_name);
		return filesize;
	}else{
		printf("using http 501\n");
		filesize = createBuffer2Send(http_header_501, html_file_name);
		return filesize;
	}		
}

void 
accept_request(int client){
	extern char *buffer2send;	/* extern from fileHandle.c */
	
	char buf[0x400];	
	char *path;	
	int numchars;	
	token query_string;	
	token request_token;
	struct stat st;

	#ifdef DEBUGME
	unsigned int k;
	#endif

	/* get the input from the client */
	numchars = get_line(client, buf, sizeof(buf));
	if(numchars >0){
		/* change the input string into several tokens */		
		get_token(&request_token, buf, " \n\r");
		
		#ifdef DEBUGME
		PRINT_TOKEN(k, request_token);
		#endif			
		
		/* check if the method is supported by our server */
		if (strcasecmp(request_token.token[0], "GET")){
			getHTML(http_501, "test.html");						
		}else{	/* if the method is a GET method */
			get_token(&query_string, request_token.token[1], "?");	
			
			#ifdef DEBUGME
			printf("query_string: %s\n",query_string.token[0]);
			#endif		
			
			path = setPath("./", query_string.token[0]);	
			if(stat(path, &st) == 0 ){
				if(!getHTML(http_200, path)){
					printf("file open error\n");
					close(client);
					return;	
				}
			}else{
				if(!getHTML(http_404, "notfound.html")){
					printf("file open error\n");
					close(client);
					return;	
				}
			}
			free(path);
			free_token(&query_string);
		}
		free_token(&request_token);		
		SENDBUFFER2SEND(client);
		/* free the malloc of the token */		
		
				
	}
}
char *
setPath(const char *global_path, const char *request_path){
	unsigned int total_path_size = strlen(global_path) + strlen(request_path) +1; /* +1 for null*/
	char *path = (char *)malloc(sizeof(char)*total_path_size);
	memset(path, 0, total_path_size);
	strncpy(path, global_path, strlen(global_path));
	strncat(path, request_path, strlen(request_path));
	/* total_path_size -1 == string length of both path */
	path[total_path_size-1] = '\0';
	return path;
}

int 
get_line(int sock, char *buf, int size){
	int i = 0;
	char c = '\0';
	int n;

	while ((i < size - 1) && (c != '\n')){
		n = recv(sock, &c, 1, 0);
		//printf("%02X\n", c);
		if (n > 0){
			if (c == '\r'){
				n = recv(sock, &c, 1, MSG_PEEK);
				//printf("%02X\n", c);
				if ((n > 0) && (c == '\n'))
					recv(sock, &c, 1, 0);
				else
					c = '\n';
			}
		buf[i] = c;
		i++;
		}
		else
			c = '\n';
	}
	buf[i] = '\0';
	return(i);
}

/*
 * When Some error happened, print error message and die.
 */
void 
error_die(const char *sc){
	perror(sc);
	exit(1);
}

/*
 * Start the Server.
 */
int 
server_start(u_short port){
	int httpd = 0;
	struct sockaddr_in name;
	httpd = socket(PF_INET, SOCK_STREAM, 0);

	if (httpd == -1)
 		error_die("socket can not open");

	memset(&name, 0, sizeof(name));
	name.sin_family = AF_INET;
	name.sin_port = htons(port);
	name.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(httpd, (struct sockaddr *)&name, sizeof(name)) < 0)
		error_die("process can not bind to the socket"); 
	if (listen(httpd, 5) < 0)
		error_die("process can't listen to the socket");
	return(httpd);
}
