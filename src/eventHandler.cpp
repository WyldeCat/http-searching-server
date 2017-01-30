#include "eventHandler.hpp"

event::event(int _type, tcp_socket *_socket):type(_type),socket:(_socket)
{
	epoll_ev = (epoll_event*)malloc(sizeof epoll_event);
}



event_handler::event_handler(int _size):size(size)
{
	events = new event[_size];
	epoll_fd = epoll_create(_size);
}
event_handler::event_handler(){}


int event_handler::add(event* ev)
{
	return epoll_ctl(epoll_fd, EPOLL_CTL_ADD, ev->epoll_fd, ev->epoll_ev);
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
	return epoll_ctl(epoll_fd, EPOLL_CTL_DEL, ev->epoll_fd, NULL);
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
	epoll_event* _events = (epoll_event*)malloc(size * sizeof epoll_event);
	state = epoll_wait(epoll_fd, _events, size, timeout);

	for(int i=0;i<state;i++)
	{
		events[i].type = _events[i].events;

		delete(events[i].socket);
		events[i].socket = NULL; /* TODO */
		// Did I delete this well?
	}

	free(_events);

	return state;
}

event* event_handler::get_ith_event(int i)
{
	return &events[i];
}

