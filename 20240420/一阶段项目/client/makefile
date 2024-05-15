.PHONY: all clean

CC=gcc

all:client_main.o client_info.h
	@$(CC) client_error.o client_function.o client_main.o -o client
	./client
client_main.o:client_function.o client_error.o
	@$(CC) client_main.c -c
client_info.o:
	$(CC) client_function.c -c
client_error.o:
	$(CC) client_error.c -c
clean:
	rm -f *.o
