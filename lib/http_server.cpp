#include "http_server.hpp"

#include <cstdio>
#include <cstring>
#include <thread>

#include <unistd.h>
#include <pthread.h>
#include <signal.h>

/* -----------http request----------- */

void http_response::send(tcp_socket* socket)
{
	int fd = socket->get_file_descriptor();

	// TODO : socket write

	// XXX tmp
	const char *message1 = "HTTP/1.1 200 OK\r\n";
	const char *message2 = "Content-Type: application/json; charset=utf-8\r\n";
	const char *message3 = "Content-Length: 10\r\n";
	const char *message4 = "Connection: close\r\n\r\n";
	const char *message5 = "1234567890";

	write(fd, message1, strlen(message1));
	write(fd, message2, strlen(message2));
	write(fd, message3, strlen(message3));
	write(fd, message4, strlen(message4));
	write(fd, message5, strlen(message5));
	
	fprintf(stderr,"write!!\n");

}

/* -----------http request----------- */

http_request::http_request(tcp_socket* socket)
{
	int fd = socket->get_file_descriptor(), readn;
	char buf[1024]={0,};
	char* i,* j;
	char* end_line;

	// TODO : socket read
	readn = read(fd, buf, 1024);

	if(readn<=0)
	{
		method = ERR;
		return;
	}

	if(strlen(buf)!=0) 
	{
		end_line = i = buf;	
		while(*end_line!='\r' && *end_line!=0) end_line++;

		// METHOD

		switch(*i)
		{
			case 'G':
				method = GET;
				break;
			case 'P':
				method = (*(i+1)=='O') ? POST : PUT;
				break;
			case 'D':
				method = DELETE;
				break;
			default:
				method = ERR;
				break;
		}
		while((*i)!=' ') i++;
		i++;

		// URL

		char* st=NULL;

		while(1)
		{
			if((*i)=='/')
			{
				if(st!=NULL)
				{
					url.push_back(std::string(st,i-st));
				}
				st = i+1;
			}
			else if((*i)==' ')
			{
				if(*(i-1)!='/')
				{
					url.push_back(std::string(st,i-st));
				}
				break;
			}
			i++;
		}
		i++;

		// TODO : Update
	}
}

/* -----------http server----------- */

http_server::http_server(http_response* (*t)(http_request*), const char *ip, unsigned short port, unsigned int _size):handler(t),size(_size)
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
	// TODO : check
	//signal(SIGPIPE, SIG_IGN);

	tcp_socket *client_sock;

	http_request* req;
	http_response* res;

	int server_sockfd = server_sock->get_file_descriptor();
	fprintf(stderr,"server sockfd : %d\n",server_sockfd);

	while(1)
	{
		fprintf(stderr,"\n\n\nwating..!\n");
		int n = main_handler->wait(-1);
		fprintf(stderr,"wating end..!\n");

		event* evnt;
		tcp_socket* tmp;

		for(int i=0;i<n;i++)
		{
			evnt = main_handler->get_ith_event(i);
			tmp = evnt->get_socket();

			if(server_sockfd == tmp->get_file_descriptor())
			{
				client_sock = server_sock->accept();
				fprintf(stderr,"added..! : %d\n",main_handler->add(event::READ, client_sock));
				delete(client_sock);
			}
			else 
			{	
				req = new http_request(tmp);	
				//res = handler(req);

				if(req->method == http_request::ERR)
				{
					tmp->close_socket();
					main_handler->del(evnt);

					delete req;
					continue;
				}
					
				res = new http_response();
				res->send(tmp);

				delete res;
				delete req;
			}
		}
	}
}

