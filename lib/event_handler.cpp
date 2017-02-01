#include "event_handler.hpp"

#include <stdlib.h>
#include <stdio.h>

event::event(int _type, tcp_socket *_socket):type(_type),socket(_socket)
{ 
	epoll_ev = (epoll_event*)malloc(sizeof(epoll_event));
	epoll_ev->events = type;
	epoll_ev->data.fd = socket->get_file_descriptor();
}
event::event()
{
	epoll_ev = (epoll_event*)malloc(sizeof(epoll_event));
}
tcp_socket* event::get_socket()
{
	return this->socket;
}


event_handler::event_handler(int _size):size(_size)
{
	_evnts = (epoll_event*)malloc(size * sizeof(*_evnts));
	evnts = new event[_size];
	evnt = new event;
	epoll_fd = epoll_create(_size);
}

int event_handler::add(event* ev)
{
	fprintf(stderr,"add : %d, %d\n",ev->socket->get_file_descriptor(),ev->epoll_ev->events);
	return epoll_ctl(epoll_fd, EPOLL_CTL_ADD, ev->socket->get_file_descriptor(), ev->epoll_ev);
}
int event_handler::add(int type, tcp_socket *socket)
{
	int state;
	evnt->type = evnt->epoll_ev->events = type;
	evnt->socket = socket;
	evnt->epoll_ev->data.fd = socket->get_file_descriptor();
	state = event_handler::add(evnt);
	return state;
}

int event_handler::del(event* ev)
{
	return epoll_ctl(epoll_fd, EPOLL_CTL_DEL, ev->socket->get_file_descriptor(), NULL);
}
int event_handler::del(int type, tcp_socket *socket)
{
	int state;
	evnt->type = evnt->epoll_ev->events = type;
	evnt->socket = socket;
	evnt->epoll_ev->data.fd = socket->get_file_descriptor();
	state = event_handler::del(evnt);
	return state;
}


int event_handler::wait(int timeout)
{
	int state;
	sockaddr_in client_addr;
	socklen_t client_len = sizeof(client_addr);
	// should remember..

	state = epoll_wait(epoll_fd, _evnts, size, timeout);
	perror("epoll_wait : ");

	for(int i=0;i<state;i++)
	{
		evnts[i].type = _evnts[i].events;
		if(evnts[i].socket != NULL) delete(evnts[i].socket);
		getpeername(_evnts[i].data.fd, (struct sockaddr*)&client_addr, &client_len);
		evnts[i].socket = new tcp_socket(_evnts[i].data.fd, &client_addr); 
	}

	return state;
}

event* event_handler::get_ith_event(int i)
{
	return &evnts[i];
}

