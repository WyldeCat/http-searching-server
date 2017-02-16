CXX = g++ 

LIB_PATH = ./lib
SRC_PATH = ./src
INCLUDE_PATH = ./include

CPPFLAGS = -g -std=c++11 -w

HTTP_MAKE_FILE = $(LIB_PATH)/http-server/Makefile
HTTP_LIB = $(LIB_PATH)/http-server/libhttp_server.a
HTTP_INCLUDE = $(LIB_PATH)/http-server/include

TRIE_INCLUDE = $(LIB_PATH)/trie/include

SH = $(INCLUDE_PATH)/search_precompiled.hpp
SPCH = $(INCLUDE_PATH)/search_precompiled.hpp.gch
SSRCS = $(SRC_PATH)/search.cpp
SOBJS = $(SSRCS:%.cpp=%.o)
SEARCH = search

MH = $(INCLUDE_PATH)/mongo_precompiled.hpp
MPCH = $(INCLUDE_PATH)/mongo_precompiled.hpp.gch
MSRCS = $(SRC_PATH)/mongo.cpp
MOBJS = $(MSRCS:%.cpp=%.o)
MONGO = mongo

SHARED_INCLUDE = $(LIB_PATH)/shared-stl-allocator/include
CHAR_CODEC_INCLUDE = $(LIB_PATH)/char_codec/include

all : $(MONGO) $(SEARCH)


###############################################

$(MPCH): $(MH)
	$(CXX) $(CPPFLAGS) $(MH) -I/usr/local/include/mongocxx/v_noabi -I/usr/local/include/libmongoc-1.0 -I/usr/local/include/bsoncxx/v_noabi -I/usr/local/include/libbson-1.0 -I$(TRIE_INCLUDE) -I$(SHARED_INCLUDE) -I$(CHAR_CODEC_INCLUDE)

$(SPCH): $(SH) 
	$(CXX) $(CPPFLAGS) $(SH) -I$(HTTP_INCLUDE) -I$(TRIE_INCLUDE) -I$(SHARED_INCLUDE) -I$(CHAR_CODEC_INCLUDE)

$(MOBJS): %.o : %.cpp $(MPCH)
	$(CXX) -c -o $@ $(@:%.o=%.cpp) $ $(CPPFLAGS) -I$(INCLUDE_PATH)

$(SOBJS) : $(SSRCS) $(SPCH)
	$(CXX) -c -o $@ $(@:%.o=%.cpp) $(CPPFLAGS) -I$(INCLUDE_PATH)

$(HTTP_LIB) : $(HTTP_MAKE_FILE)
	make -C $(LIB_PATH)/http-server all

###############################################

$(SEARCH) : $(SOBJS) $(HTTP_LIB)
	$(CXX) -o $@ $(SOBJS) $(CPPFLAGS) $(HTTP_LIB) -lpthread -lrt

$(MONGO) : $(MOBJS) 
	$(CXX) -o $@ $(MOBJS) $(CPPFLAGS) $(TRIE_LIB)  -L/usr/local/lib -lmongocxx -lbsoncxx -lrt

clean_all :
	rm -f $(HTTP_LIB)
	rm -f $(OBJS) $(MOBJS) $(SOBJS) $(MONGO) $(SEARCH)
	rm -f $(LIB_PATH)/.* || true
	rm -f $(SRC_PATH)/.* || true
	rm -f $(INCLUDE_PATH)/.* || true
	make -f $(TRIE_MAKE_FILE) clean
	make -f $(HTTP_MAKE_FILE) clean

clean :
	rm -f $(OBJS) $(MOBJS) $(SOBJS) $(MONGO) $(SEARCH) $(SPCH) $(MPCH)
	rm -f $(LIB_PATH)/.* || true
	rm -f $(SRC_PATH)/.* || true
	rm -f $(INCLUDE_PATH)/.* || true

depend:
	gccmakedep $(SRCS)

