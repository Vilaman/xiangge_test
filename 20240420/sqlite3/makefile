.PHONY:debug clean
CC=gcc
RM=rm
RF=-rf
EXE=simple
$(EXE):debug clean
debug:
	@$(CC) -o $(LX) $(LX).c -lpthread -lsqlite3
	@./$(LX)
clean:
	@$(RM) $(RF) $(LX)

