#include "http.hpp"

#include <thread>
#include <cstdio>

#include <pthread.h>
#include <unistd.h>

http_server::http_server(http_response (*t)(http_request*), const char *ip, unsigned short port):handler(t)
{
	server_sockfd = new tcp_socket(ip,port);
}

http_server::~http_server()
{
}

int http_server::start()
{
	server_th = new std::thread(&http_server::routine, this);
	return 1;
}

int http_server::stop()
{
	return pthread_cancel(server_th->native_handle());
}

void http_server::routine()
{
	while(1)
	{
		fprintf(stderr,"routine running..!\n");
		sleep(3);
	}
}

