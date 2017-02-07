CXX = g++ 

LIB_PATH = ./lib
SRC_PATH = ./src
INCLUDE_PATH = ./include 

CPPFLAGS = -g -std=c++11

LIB_SRCS = $(LIB_PATH)/http_server/tcp_socket.cpp  $(LIB_PATH)/http_server/event_handler.cpp $(LIB_PATH)/http_server/http_server.cpp $(LIB_PATH)/http_server/_http_server.cpp
LIB_OBJS = $(LIB_SRCS:%.cpp=%.o)
STATIC_LIB = $(LIB_PATH)/libhttp_server.a

SRCS = $(SRC_PATH)/search.cpp
OBJS = $(SRCS:%.cpp=%.o)

MSRCS = $(SRC_PATH)/mongo.cpp
MOBJS = $(MSRCS:%.cpp=%.o)

HTTP = search
MONGO = mongo


all : $(MONGO) $(HTTP)
mongo : $(MONGO)
http : $(HTTP)

# Need to be moved
$(LIB_OBJS) : $(LIB_SRCS)
	$(CXX) -c -o $@ $(@:%.o=%.cpp) $(CPPFLAGS) -I$(INCLUDE_PATH)

$(MOBJS): $(MSRCS)
	$(CXX) -c -o $@ $(MSRCS) $(CPPFLAGS) -I/usr/local/include/mongocxx/v_noabi -I/usr/local/include/libmongoc-1.0 -I/usr/local/include/bsoncxx/v_noabi -I/usr/local/include/libbson-1.0 

$(OBJS) : $(SRCS)
	$(CXX) -c -o $@ $(@:%.o=%.cpp) $(CPPFLAGS) -I$(INCLUDE_PATH) 


$(STATIC_LIB) : $(LIB_OBJS)
	ar crv $@ $(LIB_OBJS)

$(HTTP) : $(OBJS) $(STATIC_LIB)
	$(CXX) -o $@ $(OBJS) $(CPPFLAGS) $(STATIC_LIB) -lpthread

$(MONGO) : $(MOBJS)
	$(CXX) -o $@ $(MOBJS) $(CPPFLAGS)  -L/usr/local/lib -lmongocxx -lbsoncxx

clean :
	rm -f $(LIB_OBJS) $(OBJS) $(MOBJS) $(HTTP) $(MONGO)
	rm $(LIB_PATH)/.* || true
	rm $(MSRC_PATH)/.* || true
	rm $(SRC_PATH)/.* || true
	rm ./.* || true

depend:
	gccmakedep $(SRCS)


