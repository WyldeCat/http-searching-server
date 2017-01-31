#include <iostream>
#include <string>

#include "http_server.h"

http_response tmp(http_request *a)
{
	http_response t;
	return t;
}

int main( )
{
	http_server server(tmp, "127.0.0.1", 4000);

	return 0;
}
