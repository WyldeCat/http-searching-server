#include <cstdio>
#include <string>

#include "http_server.h"

int main( )
{
	{
		_http_server server(NULL, "127.0.0.1", 4000);
		//fprintf(stderr,"%p\n",&server);
	}
	fprintf(stderr,"hihi\n");
	return 0;
}
