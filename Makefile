CXX = g++ 

LIB_PATH = ./lib
SRC_PATH = ./src
INCLUDE_PATH = ./include 

CPPFLAGS = -g -std=c++11

HTTP_MAKE_FILE = $(LIB_PATH)/http-server/Makefile
HTTP_LIB = $(LIB_PATH)/http-server/libhttp_server.a
HTTP_INCLUDE = $(LIB_PATH)/http-server/include

TRIE_MAKE_FILE = $(LIB_PATH)/trie/Makefile
TRIE_LIB = $(LIB_PATH)/trie/libtrie.a
TRIE_INCLUDE = $(LIB_PATH)/trie/include

SSRCS = $(SRC_PATH)/search.cpp
SOBJS = $(SSRCS:%.cpp=%.o)
SEARCH = ./search

MSRCS = $(SRC_PATH)/mongo.cpp
MOBJS = $(MSRCS:%.cpp=%.o)
MONGO = ./mongo

all : $(MONGO) $(SEARCH)
mongo : $(MONGO)
search : $(SEARCH)


###############################################

$(MOBJS): $(MSRCS)
	$(CXX) -c -o $@ $(@:%.o=%.cpp) $(CPPFLAGS) -I/usr/local/include/mongocxx/v_noabi -I/usr/local/include/libmongoc-1.0 -I/usr/local/include/bsoncxx/v_noabi -I/usr/local/include/libbson-1.0 -I$(INCLUDE_PATH) -I$(TRIE_INCLUDE)

$(SOBJS) : $(SSRCS)
	$(CXX) -c -o $@ $(@:%.o=%.cpp) $(CPPFLAGS) -I$(INCLUDE_PATH) -I$(HTTP_INCLUDE)

$(HTTP_LIB) : $(HTTP_MAKE_FILE)
	make -C $(LIB_PATH)/http-server all

$(TRIE_LIB) : $(TRIE_MAKE_FILE)
	make -C $(LIB_PATH)/trie all

###############################################

$(SEARCH) : $(SOBJS) $(HTTP_LIB)
	$(CXX) -o $@ $(SOBJS) $(CPPFLAGS) $(HTTP_LIB) -lpthread 

$(MONGO) : $(MOBJS) $(TRIE_LIB)
	$(CXX) -o $@ $(MOBJS) $(CPPFLAGS) $(TRIE_LIB)  -L/usr/local/lib -lmongocxx -lbsoncxx

clean_all :
	rm -f $(HTTP_LIB) $(TRIE_LIB)
	rm -f $(OBJS) $(MOBJS) $(MONGO) $(SEARCH)
	rm -f $(LIB_PATH)/.* || true
	rm -f $(SRC_PATH)/.* || true
	rm -f $(INCLUDE_PATH)/.* || true
	make -f $(TRIE_MAKE_FILE) clean
	make -f $(HTTP_MAKE_FILE) clean

clean :
	rm -f $(OBJS) $(MOBJS) $(MONGO) $(SEARCH)
	rm -f $(LIB_PATH)/.* || true
	rm -f $(SRC_PATH)/.* || true
	rm -f $(INCLUDE_PATH)/.* || true

depend:
	gccmakedep $(SRCS)

