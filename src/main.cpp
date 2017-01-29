#include <iostream>
#include <string>

#include "http.hpp"

http_response tmp(http_request *a)
{
	http_response t;
	return t;
}

int main( )
{
	http_server server(tmp, "127.0.0.1", 4000);

	while(1)
	{
		int state = 0;
		std::string command;

		std::cout << "> ";
		std::cin >> command;
		
		if(command == "start") 
		{
			state = server.start();
			if(state>=0)
			{
				std::cout << "Started well..\n";
			}
		}

		else if(command == "stop")
		{
			state = server.stop();
			if(state>=0)
			{
				std::cout << "Stoped well..\n";	
			}
		}

		else if(command == "exit")
		{
			std::cout << "Bye.\n";	
			break;
		}

		else 
		{
			std::cout << "I don't know what you are talking about..\n";
		}

	}
	return 0;
}
