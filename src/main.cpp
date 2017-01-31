#include <cstdio>
#include <string>

#include "_http_server.h"

int main( )
{
	_http_server server(NULL, "127.0.0.1", 4000);

	return 0;
}
