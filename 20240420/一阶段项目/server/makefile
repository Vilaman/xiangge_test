.PHONY: all clean

CC=gcc

all:server_main.o server_info.h
	@$(CC) pthread_pool.o server_function.o server_main.o -o server -lsqlite3 -lpthread
	./server
server_main.o:server_function.o pthread_pool.o
	@$(CC) server_main.c -c -lsqlite3 -lpthread
server_info.o:
	$(CC) server_function.c -c -lsqlite3 -lpthread
pthread_pool.o:
	$(CC) pthread_pool.c -c -lpthread
clean:
	rm -f *.o
