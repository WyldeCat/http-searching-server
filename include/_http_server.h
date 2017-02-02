#ifndef _HTTP_SERVER_
#define _HTTP_SERVER_

class http_response;
class http_request;
class http_server;

class _http_server {

private:
  http_server* server_pointer;

public:
  _http_server(http_response* (*t)(http_request*),const char *ip, unsigned short port, unsigned int size, int cnt_threads);
  ~_http_server();

  int start();
  int stop();

};

#endif

