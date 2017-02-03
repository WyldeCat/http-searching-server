#include <cstdio>
#include <string>

#include "_http_server.h"

int main( )
{
  _http_server server(NULL, "192.168.1.210", 4000, 4096, 8);
  server.start();
  return 0;
}
