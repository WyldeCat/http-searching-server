#include <iostream>
#include <cstdio>
#include <set>
#include <vector>

#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>

#include "trie.hpp"


struct user_info {
  std::string image;
  std::string _id;
};

trie<user_info> trie_user;

int main( )
{
  int cnt = 0;

  mongocxx::instance inst{};
  mongocxx::client client{mongocxx::uri{"mongodb://192.168.1.208:27017"}};
  mongocxx::database db = client["arture"];
  mongocxx::collection coll = db["users"];
  //mongocxx::collection coll_node;
  
  auto cursor = coll.find(bsoncxx::builder::stream::document{} << bsoncxx::builder::stream::finalize);
  
  std::string key;
  std::string name;
  user_info tmp;

  for(auto doc : cursor) {
    for(auto elem : doc) 
    {
      // std::string type = bsoncxx::to_string(elem.type());
      key = elem.key().to_string();

      if(key == "name") name = elem.get_utf8().value.to_string();
      else if(key == "_id")
      {
        tmp._id = elem.get_oid().value.to_string();
      }
      else if(key == "image")
      {
        tmp.image = elem.get_utf8().value.to_string();
      }
    }
    trie_user.insert((char*)name.c_str(),tmp);
    cnt++;
  }

  for(trie<user_info>::bfs_iterator it = trie_user.start(); it != trie_user.end(); it++)
  {
    std::cout << it.get_caption() << std::endl;
  }
  std::cout << cnt << std::endl;

  return 0;
}
