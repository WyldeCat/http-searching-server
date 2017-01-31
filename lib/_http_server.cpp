#include "http_server.h"
#include "http_server.hpp"

#include <thread>
#include <cstdio>

#include <pthread.h>


_http_server::_http_server(http_response (*t)(http_request*), char *ip, unsigned short port)
{
	server_pointer = new http_server(t, ip, port);
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
