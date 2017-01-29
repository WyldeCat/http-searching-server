#include "tcp.hpp"

#include <thread>

class http_response {
};

class http_request {
};

class http_server {

public:

	http_server(http_response (*)(http_request*), const char *ip, unsigned short port);
	~http_server();
	int start();
	int stop();

private:

	void routine();

private:

	std::thread *server_th;
	tcp_socket *server_sockfd;
	http_response (*handler)(http_request*);

};
