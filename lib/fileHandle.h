#ifndef FILEHANDLE_H
#define FILEHANDLE_H
extern unsigned int calculate_file_size(const char *html_file_name);
extern char* read2Buffer(const char *html_file_name);
extern unsigned int createBuffer2Send(const char *http_header_type, const char * html_file_name);
#endif
