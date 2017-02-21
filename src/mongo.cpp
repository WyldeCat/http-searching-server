#include "mongo_precompiled.hpp"

#define SHARED_POINTER1 0xcaffe1e000
#define SHARED_POINTER2 0xcbffe1e000
#define SHARED_POINTER3 0xccffe1e000

shared_stl_allocator<char, SHARED_POINTER3> tt;

struct user_info {
  std::basic_string<char, std::char_traits<char>, shared_stl_allocator<char,SHARED_POINTER3> > name;
  std::basic_string<char, std::char_traits<char>, shared_stl_allocator<char,SHARED_POINTER3> > image;
  std::basic_string<char, std::char_traits<char>, shared_stl_allocator<char,SHARED_POINTER3> > _id;
};

int cnt_shm=1;
int shm_id[1];
trie<user_info, SHARED_POINTER1, SHARED_POINTER2> *trie_user[1];
key_t shm_key[1] = {1234};

mongocxx::instance inst{};
mongocxx::client client{mongocxx::uri{"mongodb://192.168.1.209:27017/"}};
mongocxx::database db = client["arture"];
mongocxx::collection coll = db["users_user"];


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
  int cnt=0;
  std::string key,name;
  user_info tmp;

  for(auto doc : cursor) 
  {
    for(auto elem : doc) 
    {
      key = elem.key().to_string();
      if(key == "name")
      {
        name = elem.get_utf8().value.to_string();
        tmp.name.clear();
        for(auto &c : elem.get_utf8().value.to_string()) 
          tmp.name.push_back(c);
        fprintf(stderr,"tmp.name!! %p\n",&(tmp.name[0]));
      }
      else if(key == "_id") 
      {
        tmp._id.clear();
        for(auto &c : elem.get_oid().value.to_string()) tmp._id.push_back(c);
      }
      else if(key == "pic") 
      {
        tmp.image.clear();
        for(auto &c : elem.get_utf8().value.to_string()) tmp.image.push_back(c);
      }
    }
    cnt++;
    std::string t = name;
    char_codec::encode((char*)name.c_str());
    //if(name.c_str()[0] == 19)
    {
      fprintf(stderr,"%s %s\n",t.c_str(),tmp._id.c_str());
      for(int i=0;i<strlen(name.c_str());i++)
      {
        fprintf(stderr,"%d ",name.c_str()[i]);
      }
      fprintf(stderr,"\n");
    }
    trie_user[x]->insert((char*)name.c_str(),tmp);
    if(cnt==200) break;
  }
}

void detach_shm(int x)
{
  shmdt((void*)trie_user[x]);
}

int main( )
{
  shared_stl_allocator<char,SHARED_POINTER3>::attach();
  for(int i=0;i<cnt_shm;i++)
  {
    set_shm(i);
    set_trie(i);
  }
  auto *map = &trie_user[0]->root.children;
  return 0;
}
