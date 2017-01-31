/*

wrapping BSD tcp socket as tcp socket 

*/

#ifndef _TCP_SOCKET_H_
#define _TCP_SOCKET_H_

#include <netinet/in.h>

class tcp_socket {

public:
	tcp_socket(const char* _ip,unsigned short _port);
	tcp_socket(int fd, sockaddr_in *addr);
	~tcp_socket();

	tcp_socket* accept();
	int tcp_readn();
	int tcp_write();
	int get_file_descriptor();

private:
	int file_descriptor;
	const char* ip;
	unsigned short port;

};

#endif
