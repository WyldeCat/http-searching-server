#include <iostream>
#include <set>
#include <vector>

#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>

int main( )
{
  mongocxx::instance inst{};
  
  mongocxx::uri uri("mongodb://localhost:27017");
  mongocxx::client client(uri);
  mongocxx::database db = client["tests"];
  mongocxx::collection coll = db["test"];
  
  auto cursor = coll.find(bsoncxx::builder::stream::document{} << bsoncxx::builder::stream::finalize);
  
  for(auto doc : cursor) {
    for(auto elem : doc) 
    {
      std::string type = bsoncxx::to_string(elem.type());
      std::cout << type << std::endl;
      std::string key = elem.key().to_string();
      if(key == "name") 
      {
        std::string name = elem.get_utf8().value.to_string();
        std::cout << key << ":" << name << std::endl;
      }
      else if(key == "_id")
      {
        std::string id = elem.get_oid().value.to_string();
        std::cout << key << ":" << id << std::endl;
      }
      else if(key == "age")
      {
        double age = elem.get_double().value;
        std::cout << key << ":" << age << std::endl;
      }
    }
    std::cout << std::endl;
  }
  return 0;
}
