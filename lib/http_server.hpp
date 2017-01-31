
#ifndef _HTTP_SERVER_H_
#define _HTTP_SERVER_H_

#include "tcp_socket.hpp"
#include "event_handler.hpp"

#include <thread>

class http_response {
};

class http_request {
};

class http_server {

public:

	http_server(http_response (*handler)(http_request*),const char *ip, unsigned short port,unsigned int _size);
	~http_server();
	int start();
	int stop();

private:
	void routine();

private:

	unsigned int size;
	std::thread* server_th;
	tcp_socket* server_sock;
	event_handler* main_handler;
	
	http_response (*handler)(http_request*);

};

#endif
