#include "http_server.hpp"

#include <thread>
#include <cstdio>

#include <pthread.h>

http_server::http_server(http_response (*t)(http_request*), const char *ip, unsigned short port):handler(t)
{
	server_sock = new tcp_socket(ip,port);
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
	tcp_socket *client_sock;

	while(1)
	{
		/*
			EPOLL
		*/
	}
}
