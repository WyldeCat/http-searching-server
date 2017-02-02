#include "tcp_socket.hpp"

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

tcp_socket::tcp_socket(const char* _ip,unsigned short _port):ip(_ip),port(_port)
{	
	int option = 1;
	int flag;

	file_descriptor = socket(PF_INET, SOCK_STREAM, 0);
	setsockopt(file_descriptor, SOL_SOCKET, SO_REUSEADDR, &option, sizeof option);
	flag = fcntl(file_descriptor, F_GETFL, 0);
	fcntl(file_descriptor, F_SETFL, flag | O_NONBLOCK);

	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	inet_pton(AF_INET, ip, &server_addr.sin_addr.s_addr);

	bind(file_descriptor,(struct sockaddr*)&server_addr, sizeof server_addr);
	//perror("bind: ");
	listen(file_descriptor, 5);
	//perror("listen: ");
}

tcp_socket::tcp_socket(int fd, sockaddr_in *addr):file_descriptor(fd)
{
	ip = inet_ntoa(addr->sin_addr);
	port = addr->sin_port;
}


tcp_socket::~tcp_socket()
{
}

tcp_socket* tcp_socket::accept()
{
	sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	int t = ::accept(file_descriptor, (struct sockaddr*)&client_addr, &client_len);
	return new tcp_socket(t, &client_addr);
}

int tcp_socket::get_file_descriptor()
{
	return file_descriptor;
}
int tcp_socket::close_socket()
{
	close(file_descriptor);	
	return 1;
}

const char* tcp_socket::get_ip()
{
	return ip;
}
