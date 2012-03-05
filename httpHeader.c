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

/* the http header for the server */
const char http_header_ok[] =
"HTTP/1.1 200 OK\r\n"
"Server: JokerBot-Sentinel\r\n"
"Content-Type: text/html\r\n"
"\r\n";

const char http_header_302[] =
"HTTP/1.1 302 Found\r\n"
"Location: http://www.google.com/\r\n"
"Server: JokerBot-Sentinel\r\n"
"Contnet-Type: text/html\r\n"
"\r\n";

const char http_header_unimplement[] = 
"HTTP/1.1 501 Method Not Implemented\r\n"
"Server: JokerBot-Sentinel\r\n"
"Content-Type: text/html\r\n"
"\r\n";

const char http_header_404[] = 
"HTTP/1.1 404 NOT FOUND\r\n"
"Server: JokerBot-Sentinel\r\n"
"Content-Type: text/html\r\n"
"\r\n";

unsigned int
getHTML(enum http_header header_type, const char *html_file_name){
	unsigned int filesize = 0;
	
	if(header_type == http_200){
		printf("using http 200\n");
		filesize = createBuffer2Send(http_header_ok, html_file_name);
		return filesize;
	}
	else if(header_type == http_302){
		printf("using http 302\n");
		filesize = createBuffer2Send(http_header_302, html_file_name);
		return filesize;
	}
	else if(header_type == http_404){
		printf("using http 404\n");
		filesize = createBuffer2Send(http_header_404, html_file_name);
		return filesize;
	}
	else{
		printf("using http 501\n");
		filesize = createBuffer2Send(http_header_unimplement, html_file_name);
		return filesize;
	}		
}
void accept_request(int client){
	extern char *buffer2send;
	char buf[1024];
	int numchars;
	char method[255];
	char url[255];
	char path[512];	
	struct stat st;
	char *query_string = NULL;
	token request_token;
	unsigned int k;

	/* get the input from the client */
	numchars = get_line(client, buf, sizeof(buf));
	if(numchars >0){
		/* change the input string into several tokens */
		request_token.token_num = get_token_num(buf," \n\r");
		get_token(&request_token, buf, " \n\r");
		PRINT_TOKEN(k, request_token);
		
		/* get the method from the input tokens */
		if(strlen(request_token.token[0]) > sizeof(method)-1 ){
			error_die("method out of bound\n");
		}
		strncpy(method, request_token.token[0], strlen(request_token.token[0]));
		method[strlen(request_token.token[0])] = '\0';
		/* check if the method is supported by our server */
		if (strcasecmp(method, "GET")){
			getHTML(http_501, "test.html");
			send(client, buffer2send, strlen(buffer2send), 0);
			close(client);
			return;
		}
		/* get the url from the input tokens */
		if(strlen(request_token.token[1]) > sizeof(url)-1 ){
			error_die("url out of bound\n");
		}
		strncpy(url, request_token.token[1], strlen(request_token.token[1]));
		url[strlen(request_token.token[1])] = '\0';

		
		/* If the client request is a GET method */
		if (strcasecmp(method, "GET") == 0){
			query_string = url;
			while ((*query_string != '?') && (*query_string != '\0'))
				query_string++;
			if (*query_string == '?'){
				*query_string = '\0';
				query_string++;
			}
		}
		printf("query_string: %s\n",url);
		if(!getHTML(http_200, "test.html")){
			printf("file can't be found\n");
			return;
		}	
		send(client, buffer2send, strlen(buffer2send), 0);
		close(client);
		/* free the malloc of the token */
		free_token(&request_token);
		free(buffer2send);
	}
}

/* 
 * check if the query_string is a command or not 
 * if the string is a command, return true.
 * else return false. 
 */
int is_cmd(const char *url){
	
}

int get_line(int sock, char *buf, int size){
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
void error_die(const char *sc){
	perror(sc);
	exit(1);
}

/*
 * Start the Server.
 */

int server_start(u_short port){
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
