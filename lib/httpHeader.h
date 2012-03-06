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

/* the following data are the standard http headers */
extern const char http_header_ok[];
extern const char http_header_unimplement[];
extern const char http_header_302[];
extern const char http_header_404[];

/* function prototype */
extern unsigned int getHTML(enum http_header header_type, const char *html_file_name);
extern void error_die(const char *sc);
extern int server_start(u_short port);
extern void accept_request(int client);
extern int get_line(int sock, char *buf, int size);

#endif
