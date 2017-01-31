
#ifndef _HTTP_SERVER_
#define _HTTP_SERVER_

class http_response {
};

class http_request {
};

class http_server {

public:
	http_server(http_response (*)(http_request*), const char *ip, unsigned short port);
	int start();
	int stop();
};

#endif
