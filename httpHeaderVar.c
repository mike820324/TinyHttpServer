#include "lib/httpHeader.h"

/* the http header for the server */
const char http_header_200[] =
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

const char http_header_501[] = 
"HTTP/1.1 501 Method Not Implemented\r\n"
"Server: JokerBot-Sentinel\r\n"
"Content-Type: text/html\r\n"
"\r\n";

const char http_header_404[] = 
"HTTP/1.1 404 NOT FOUND\r\n"
"Server: JokerBot-Sentinel\r\n"
"Content-Type: text/html\r\n"
"\r\n";

