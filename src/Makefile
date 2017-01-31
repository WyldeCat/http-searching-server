CC = g++ -std=c++11

LIB_PATH = ../lib

SRCS = $(LIB_PATH)/tcp_socket.cpp $(LIB_PATH)/http_server.cpp $(LIB_PATH)/event_handler.cpp main.cpp
OBJS = $(SRCS:%.cpp=%.o)
TARGET = main

.SUFFIXES : .cpp .o

all : $(TARGET)

$(TARGET) : $(OBJS)
	$(CC) -o $@ $(OBJS)

clean :
	rm -f $(OBJS) $(TARGET)

depend:
	gccmakedep $(SRCS)
