CC:=gcc
OBJ:=stringToken.o httpd.o httpHeaderVar.o main.o fileHandle.o

all: ${OBJ}
	${CC} -g -o main.out ${OBJ}

stringtoken.o: stringToken.c lib/stringToken.h
	${CC} -g -c -o stringToken.o stringtoken.c

httpd.o: httpHeader.c lib/stringToken.h lib/fileHandle.h lib/httpHeader.h
	${CC} -g -c -o httpd.o httpHeader.c

httpHeaderVar.o: httpHeaderVar.c lib/httpHeader.h
	${CC} -g -c -o httpHeaderVar.o httpHeaderVar.c

main.o: http_main.c lib/stringToken.h lib/httpHeader.h
	${CC} -g -c -o main.o http_main.c

fileHandle.o: fileHandle.c lib/fileHandle.h
	${CC} -g -c -o fileHandle.o fileHandle.c
clean: 
	rm *.o
