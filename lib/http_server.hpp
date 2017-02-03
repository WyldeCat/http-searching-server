
#ifndef _HTTP_SERVER_H_
#define _HTTP_SERVER_H_

#include "tcp_socket.hpp"
#include "event_handler.hpp"

#include <thread>
#include <mutex>
#include <vector>
#include <string>

class http_server;

class http_response {

public:
  void send(tcp_socket *socket);

private:
  char *status;
  char *content_type;
  char *content_length;
  char *connection;
  char *body;

};

class http_request {

public:
  typedef enum __method 
  {
    GET,
    POST,
    PUT,
    DELETE,
    ERR
  } _method;

  http_request();
  http_request(tcp_socket* _socket);
  void set_request(tcp_socket* _socket);

private:
  tcp_socket* socket;
  _method method;
  std::vector<std::string> url;

  friend http_server;
};


class http_server {

public:
  http_server(http_response* (*handler)(http_request*),const char *ip, unsigned short port,unsigned int _size, int _cnt_threads);
  ~http_server();
  int start();
  int stop();

private:
  void routine(int thread_idx);

private:
  int cnt_threads;
  int size;
  std::mutex* mutexes;
  std::thread** server_th;
  tcp_socket* server_sock;
  event_handler* main_handler;
  
  http_response* (*handler)(http_request*);

};

#endif
