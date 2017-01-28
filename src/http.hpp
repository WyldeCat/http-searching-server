#include "tcp.hpp"

class http_response {
};

class http_request {
};

class http_server {
public:
	http_server(http_response (*)(http_request*), const char *ip, unsigned short port);
	~http_server();
	void start();
	void stop();

private:
	tcp_socket *server_sockfd;
	http_response (*handler)(http_request*);
};
