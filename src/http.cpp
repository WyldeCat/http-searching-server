#include "http.hpp"

http_server::http_server(http_response (*t)(http_request*), const char *ip, unsigned short port):handler(t)
{
	server_sockfd = new tcp_socket(ip,port);
}

http_server::~http_server()
{
}

void http_server::start()
{
}

void http_server::stop()
{
}
