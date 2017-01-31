#include "http_server.hpp"

#include <thread>
#include <cstdio>

#include <unistd.h>
#include <pthread.h>

/* -------------------------------------- */

http_server::http_server(http_response (*t)(http_request*), const char *ip, unsigned short port, unsigned int _size):handler(t),size(_size)
{
	server_sock = new tcp_socket(ip,port);
}

http_server::~http_server()
{
	delete(server_sock);
}


int http_server::start()
{
	main_handler = new event_handler(size);	
	main_handler->add(event::READ, server_sock);
	server_th = new std::thread(&http_server::routine, this);
	return (main_handler!=NULL && server_th!=NULL) ? 0 : -1;
}

int http_server::stop()
{
	return pthread_cancel(server_th->native_handle());
}


void http_server::routine()
{
	tcp_socket *client_sock;
	int server_sockfd = server_sock->get_file_descriptor();
	fprintf(stderr,"server sockfd : %d\n",server_sockfd);

	while(1)
	{
		int n = main_handler->wait(-1);
		event* tmp;

		for(int i=0;i<n;i++)
		{
			tmp = main_handler->get_ith_event(i);
			if(server_sockfd == tmp->get_socket()->get_file_descriptor())
			{
				client_sock = server_sock->accept();
				main_handler->add(event::READ, client_sock);
				delete(client_sock);
			}
			else 
			{	
			}
		}
	}
}

