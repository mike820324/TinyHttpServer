#ifndef HTTPHEADER_H
#define HTTPHEADER_H

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
#include "stringToken.h"

/* the http header option */
enum http_header{
	http_200,
	http_302,
	http_404,
	http_501
};
#define SENDBUFFER2SEND(client) \
do{\
	send(client, buffer2send, strlen(buffer2send), 0);\
	close(client);\
	free(buffer2send);\
}while(0)

/* the following data are the standard http headers */
extern const char http_header_200[];
extern const char http_header_501[];
extern const char http_header_302[];
extern const char http_header_404[];

/* function prototype */
extern unsigned int getHTML(enum http_header header_type, const char *html_file_name);
extern char *setPath(const char *global_path, const char *request_path);
extern void error_die(const char *sc);
extern int server_start(u_short port);
extern void accept_request(int client);
extern int get_line(int sock, char *buf, int size);

#endif
