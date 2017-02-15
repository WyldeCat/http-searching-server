#include "mongo_precompiled.hpp"

#define SHARED_POINTER1 0xcaffe1e000
#define SHARED_POINTER2 0xcafee1e000

struct user_info {
  std::string image;
  std::string _id;
};

// Need a configuration file

int cnt_shm=1;
int shm_id[1];
trie<user_info, SHARED_POINTER1, SHARED_POINTER2> *trie_user[1];
key_t shm_key[1] = {1234};

mongocxx::instance inst{};
mongocxx::client client{mongocxx::uri{"mongodb://192.168.1.208:27017"}};
mongocxx::database db = client["arture"];
mongocxx::collection coll = db["users"];


void set_shm(int x)
{ // Setting shared memory
  shm_id[x] = shmget(shm_key[x], 4096, 0666 | IPC_CREAT);

  if(shm_id[x] == -1)
  {
    perror("shmget");
    exit(0);
  } 

  trie_user[x] = (trie<user_info, SHARED_POINTER1, SHARED_POINTER2> *)shmat(shm_id[x], (void *)0, 0);

  if((void *)trie_user[x] == (void *)-1)
  {
    perror("shmat");
    exit(0);
  }
}

void set_trie(int x)
{ // Setting trie
  fprintf(stderr,"set_trie()\n");
  *(trie_user[x]) = trie<user_info, SHARED_POINTER1, SHARED_POINTER2>();
  auto cursor = coll.find(bsoncxx::builder::stream::document{} << bsoncxx::builder::stream::finalize);
  std::string key,name;
  user_info tmp;

  for(auto doc : cursor) 
  {
    for(auto elem : doc) 
    {
      key = elem.key().to_string();
      if(key == "name") name = elem.get_utf8().value.to_string();
      else if(key == "_id") tmp._id = elem.get_oid().value.to_string();
      else if(key == "image") tmp.image = elem.get_utf8().value.to_string();
    }
    trie_user[x]->insert((char*)name.c_str(),tmp);
  }

}

void detach_shm(int x)
{
  shmdt((void*)trie_user[x]);
}

// Need to having input by arguments
int main( )
{
  for(int i=0;i<cnt_shm;i++)
  {
    set_shm(i);
    set_trie(i);
  }
  auto *map = &trie_user[0]->root.children;
  printf("map: %d\n",map->begin());
  printf("map: %c\n",map->begin()->first);
  return 0;
}
