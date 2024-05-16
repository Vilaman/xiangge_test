CC := aarch64-linux-gnu-g++
CFLAGS := -I.

TARGET := main

SRCS := main.cpp comServer.cpp SysGpio.cpp

OBJS := $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.cpp
	$(CC) -c $< -o $@ $(CFLAGS)

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
