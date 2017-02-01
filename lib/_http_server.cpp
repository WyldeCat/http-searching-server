#include "_http_server.h"
#include "http_server.hpp"

#include <thread>
#include <cstdio>

#include <pthread.h>


_http_server::_http_server(http_response* (*t)(http_request*), const char *ip, unsigned short port, unsigned int size)
{
	server_pointer = new http_server(t, ip, port, size);
}

_http_server::~_http_server( )
{
	delete server_pointer;
}

int _http_server::start()
{
	server_pointer->start();
}

int _http_server::stop()
{
	server_pointer->stop();
}
