CXX = g++ 

LIB_PATH = ./lib
SRC_PATH = ./src
INCLUDE_PATH = ./include 

CPPFLAGS = -g -std=c++11 -lpthread -I $(INCLUDE_PATH)

LIB_SRCS = $(LIB_PATH)/tcp_socket.cpp  $(LIB_PATH)/event_handler.cpp $(LIB_PATH)/http_server.cpp $(LIB_PATH)/_http_server.cpp
LIB_OBJS = $(LIB_SRCS:%.cpp=%.o)

SRCS = $(SRC_PATH)/main.cpp
OBJS = $(SRCS:%.cpp=%.o)

TARGET = main

.SUFFIXES : .cpp .o

all : $(TARGET)


$(TARGET) : $(OBJS) $(LIB_OBJS)
	$(CXX) -g -o $@ $(LIB_OBJS) $(OBJS) -lpthread

	

clean :
	rm -f $(LIB_OBJS) $(OBJS) $(TARGET) 
	rm $(LIB_PATH)/.* || true
	rm $(SRC_PATH)/.* || true
	rm ./.* || true

depend:
	gccmakedep $(SRCS)
