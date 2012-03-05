/*
 * A JokerBot sentinel program. This http server will only support the get method.
 * If somebody connect to the server with other method, It will return a http error status code.
 * Reference from Tinyhttpd.
 * 
 * Author: MicroMike
 */
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

#include "lib/httpHeader.h"

#define MAX_BUFFER 1024

int main(void){
	int server_sock = -1;
	u_short port = 8080;
	int client_sock = -1;
	struct sockaddr_in client_name;
	int client_name_len = sizeof(client_name);
	
	server_sock = server_start(port);
	printf("httpd running on port %d\n", port);
	
	while (1){
		client_sock = accept(server_sock,
		                     (struct sockaddr *)&client_name,
		                     &client_name_len);
		if (client_sock == -1)
			error_die("accept");
		accept_request(client_sock);
	}
	close(server_sock);
	return(0);
}


