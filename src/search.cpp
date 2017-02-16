#include "search_precompiled.hpp"

#define SHARED_POINTER1 0xcaffe1e000
#define SHARED_POINTER2 0xcafee1e000
#define SHARED_POINTER3 0xcafde1e000

struct user_info {
  std::basic_string<char, std::char_traits<char>, shared_stl_allocator<char,SHARED_POINTER3> > name;
  std::basic_string<char, std::char_traits<char>, shared_stl_allocator<char,SHARED_POINTER3> > image;
  std::basic_string<char, std::char_traits<char>, shared_stl_allocator<char,SHARED_POINTER3> > _id;
};

int cnt_shm=1, shm_id[1];
trie<user_info, SHARED_POINTER1, SHARED_POINTER2> *trie_user[1];
key_t shm_key[1] = {1234};

bool check_shm(int x);
void set_shm(int x);

int handler(_http_request *req)
{
  _http_response tmp(req);
  std::vector<std::string> &url = *req->get_url(); 

  if(url.size() != 2 || url.front() != "search") 
  {
    tmp.set_status("404 Bad");
    tmp.set_body("You are wrong.");
    tmp.send();
  }
  else 
  {
    int cnt = 0;
    std::string result;

    char_codec::url2utf8((char*)url[1].c_str());
    char_codec::encode((char*)url[1].c_str());

    // TODO : find 
    std::vector<user_info> rt;
    trie_user[0]->findAll((char*)url[1].c_str(),rt);

    for(auto &user : rt)
    {
      for(auto &c : user.name) result.push_back(c);
      result+='\n';
    }

    tmp.set_status("200 OK"); 
    tmp.set_body(result.c_str());
    tmp.send();
  }

  return 0;
}

int main( )
{
  if(!check_shm(0)) 
  {
    fprintf(stderr,"There is no shared memory.\n");
    exit(0);
  }
  set_shm(0);

 _http_server server(handler, "192.168.1.210", 4000, 4096, 8);
  server.start(); 
  return 0;
}

bool check_shm(int x)
{ // Check shared memory
  int ret = shmget(shm_key[x], 4096, 0666 | IPC_CREAT | IPC_EXCL);
  if(ret != -1)
  {
    ret = shmctl(ret, IPC_RMID, NULL);
    if(ret == -1)
    {
      perror("shmctl");
      exit(0);
    }
    return false;
  }
  return true;
}

void set_shm(int x)
{ // Setting shared memory
  shm_id[x] = shmget(shm_key[x], 4096, 0666 | IPC_CREAT);
  
  if(shm_id[x] == -1)
  {
    perror("shmget");
    exit(0);
  }

  trie_user[x] = (trie<user_info, SHARED_POINTER1, SHARED_POINTER2>*)shmat(shm_id[x], (void *)0, 0);

  if((void *)trie_user[x] == (void *)-1)
  {
    perror("shmat");
    exit(0);
  }
  trie_user[x]->attach();
}


