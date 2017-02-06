#include <iostream>
#include <cstdio>
#include <vector>
#include <string>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>

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
  mongocxx::client conn{mongocxx::uri{}};

  bsoncxx::builder::stream::document document{};

  auto collection = conn["testdb"]["testcollection"];
  document << "hello" << "world";

  collection.insert_one(document.view());
  auto cursor = collection.find({});

  for(auto&& doc : cursor) {
    std::cout << bsoncxx::to_json(doc) << std::endl;
  }

  
  _http_server server(handler, "192.168.1.210", 4000, 4096, 8);
  server.start();
  return 0;
}
