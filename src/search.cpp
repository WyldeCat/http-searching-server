#include "search_precompiled.hpp"

struct user_info {
  std::string image;
  std::string _id;
};

int cnt_shm=1;
int shm_id[1];
trie<user_info> *trie_user[1];
key_t shm_key[1] = {1234};

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

  trie_user[x] = (trie<user_info> *)shmat(shm_id[x], (void *)0, 0);

  if((void *)trie_user[x] == (void *)-1)
  {
    perror("shmat");
    exit(0);
  }
}

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
  if(!check_shm(0)) 
  {
    fprintf(stderr,"There is no shared memory.\n");
    exit(0);
  }
  set_shm(0);
  auto &map = trie_user[0]->root.children;
  printf("%d\n",map.size());
  printf("%c\n",map.begin()->first);
    /*
  for(trie<user_info>::bfs_iterator it = trie_user[0]->begin(); it != trie_user[0]->end(); it++)
  {
    printf("%s\n",it.get_caption().c_str());
  }
  */

  _http_server server(handler, "192.168.1.210", 4000, 4096, 8);
  server.start();
  return 0;
}
