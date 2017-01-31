#include "tcp.hpp"

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

tcp_socket::tcp_socket(const char* _ip,unsigned short _port):ip(_ip),port(_port)
{	
	file_descriptor = socket(PF_INET, SOCK_STREAM, 0);

	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);

	inet_pton(AF_INET, ip, &server_addr.sin_addr.s_addr);

	bind(file_descriptor,(struct sockaddr*)&server_addr, sizeof server_addr);
	listen(file_descriptor, 5);
}

tcp_socket::tcp_socket(int fd, sockaddr_in *addr):file_descriptor(fd)
{
	ip = inet_ntoa(addr->sin_addr);
	port = addr->sin_port;
}

tcp_socket* tcp_socket::accept()
{
	sockaddr_in client_addr;
	socklen_t client_len;

	int fd = ::accept(file_descriptor, (struct sockaddr*)&client_addr, &client_len);

	return new tcp_socket(fd, &client_addr);
}

int tcp_socket::get_file_descriptor()
{
	return file_descriptor;
}
