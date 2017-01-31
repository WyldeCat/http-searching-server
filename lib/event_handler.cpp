#include "event_handler.hpp"

#include <stdlib.h>

event::event(int _type, tcp_socket *_socket):type(_type),socket(_socket)
{
	epoll_ev = (epoll_event*)malloc(sizeof(epoll_event));
}
event::event(){}



event_handler::event_handler(int _size):size(size)
{
	events = new event[_size];
	epoll_fd = epoll_create(_size);
}


int event_handler::add(event* ev)
{
	return epoll_ctl(epoll_fd, EPOLL_CTL_ADD, ev->socket->get_file_descriptor(), ev->epoll_ev);
}
int event_handler::add(int type, tcp_socket *socket)
{
	int state;
	event *tmp = new event(type, socket);
	state = event_handler::add(tmp);

	delete(tmp);

	return state;
}

int event_handler::del(event* ev)
{
	return epoll_ctl(epoll_fd, EPOLL_CTL_DEL, ev->socket->get_file_descriptor(), NULL);
}
int event_handler::del(int type, tcp_socket *socket)
{
	int state;
	event *tmp = new event(type, socket);
	state = event_handler::del(tmp);

	delete(tmp);

	return state;
}

int event_handler::wait(int timeout)
{
	int state;
	sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);

	epoll_event* _events = (epoll_event*)malloc(size * sizeof(epoll_event));
	state = epoll_wait(epoll_fd, _events, size, timeout);

	for(int i=0;i<state;i++)
	{
		events[i].type = _events[i].events;

		delete(events[i].socket);
		
		getpeername(_events[i].data.fd, (struct sockaddr*)&client_addr, &client_len);
		events[i].socket = new tcp_socket(_events[i].data.fd, &client_addr); 
	}

	free(_events);

	return state;
}

event* event_handler::get_ith_event(int i)
{
	return &events[i];
}

