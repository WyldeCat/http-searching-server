#include <sys/shm.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <stdlib.h>

#include <iostream>
#include <cstdio>
#include <vector>
#include <string>

#include "_http_server.h"
#include "trie.hpp"
#include "shared_stl_allocator.hpp"
#include "char_codec.hpp"
