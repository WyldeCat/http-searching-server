#include <cstdio>
#include <string>

#include "_http_server.h"

int main( )
{
	_http_server server(NULL, "192.168.1.15", 4000, 1024);
	server.start();
	while(1);
	return 0;
}
