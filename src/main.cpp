#include <iostream>
#include <cstdio>
#include <vector>
#include <string>

#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>


#include "_http_server.h"

int handler(_http_request *req)
{
  _http_response tmp(req);
  std::vector<std::string> *url; 
  url = req->get_url();

  tmp.set_status("200 OK");
  tmp.set_body(req->get_url()->front().c_str());
  tmp.send();
  return 0;
}

int main( )
{
  mongocxx::instance inst{};
  mongocxx::client client{mongocxx::uri{}};
  //mongocxx::database db = client["arture"];
  
  _http_server server(handler, "192.168.1.210", 4000, 4096, 8);
  server.start();
  return 0;
}
