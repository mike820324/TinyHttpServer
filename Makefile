CC:=gcc

all: stringToken.o httpd.o main.o fileHandle.o
	${CC} -g -o main.out stringToken.o httpd.o main.o fileHandle.o

stringtoken.o: stringToken.c lib/stringToken.h
	${CC} -g -c -o stringToken.o stringtoken.c

httpd.o: httpHeader.c lib/stringToken.h lib/fileHandle.h
	${CC} -g -c -o httpd.o httpHeader.c

main.o: http_main.c lib/stringToken.h lib/httpHeader.h
	${CC} -g -c -o main.o http_main.c

fileHandle.o: fileHandle.c lib/fileHandle.h
	${CC} -g -c -o fileHandle.o fileHandle.c
clean: 
	rm *.o
