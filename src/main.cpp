#include <cstdio>
#include <string>

#include "_http_server.h"

int handler(_http_request *req)
{
  _http_response tmp(req);

  tmp.set_status("200 OK");
  tmp.set_body("12349956asdfasd\nsafsasdf7890");

  tmp.send();
  return 0;
}

int main( )
{
  _http_server server(handler, "192.168.1.210", 4000, 4096, 8);
  server.start();
  return 0;
}
