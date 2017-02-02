#include "http_server.hpp"

#include <cstdio>
#include <cstring>
#include <thread>

#include <unistd.h>
#include <signal.h>
#include <pthread.h>

/* -----------http request----------- */

void http_response::send(tcp_socket* socket)
{
  int fd = socket->get_file_descriptor();

  // TODO : socket write

  // XXX tmp
  const char *message1 = "HTTP/1.1 200 OK\r\n";
  const char *message2 = "Content-Type: application/json; charset=utf-8\r\n";
  const char *message3 = "Content-Length: 10\r\n";
  const char *message4 = "Connection: close\r\n\r\n";
  const char *message5 = "1234567890";

  if(write(fd, message1, strlen(message1))==-1) perror("write");
  if(write(fd, message2, strlen(message2))==-1) perror("write");
  if(write(fd, message3, strlen(message3))==-1) perror("write");
  if(write(fd, message4, strlen(message4))==-1) perror("write");
  if(write(fd, message5, strlen(message5))==-1) perror("write");
  

}

/* -----------http request----------- */

http_request::http_request(tcp_socket* socket)
{
  int fd = socket->get_file_descriptor(), readn;
  char buf[1024]={0,};
  char* i,* j;
  char* end_line;

  // TODO : socket read
  readn = read(fd, buf, 1024);

  if(readn<=0)
  {
    method = ERR;
    return;
  }

  if(strlen(buf)!=0) 
  {
    end_line = i = buf; 
    while(*end_line!='\r' && *end_line!=0) end_line++;

    // METHOD

    switch(*i)
    {
      case 'G':
        method = GET;
        break;
      case 'P':
        method = (*(i+1)=='O') ? POST : PUT;
        break;
      case 'D':
        method = DELETE;
        break;
      default:
        method = ERR;
        break;
    }
    while((*i)!=' ') i++;
    i++;

    // URL

    char* st=NULL;

    while(1)
    {
      if((*i)=='/')
      {
        if(st!=NULL)
        {
          url.push_back(std::string(st,i-st));
        }
        st = i+1;
      }
      else if((*i)==' ')
      {
        if(*(i-1)!='/')
        {
          url.push_back(std::string(st,i-st));
        }
        break;
      }
      i++;
    }
    i++;

    // TODO : Update
  }
}

/* -----------http server----------- */

http_server::http_server(http_response* (*t)(http_request*), const char *ip, unsigned short port, unsigned int _size, int _cnt_threads):handler(t),size(_size),cnt_threads(_cnt_threads)
{
  server_sock = new tcp_socket(ip,port);
  if(cnt_threads>1) server_th = new std::thread*[cnt_threads-1];
}

http_server::~http_server()
{
  delete(server_sock);
}


int http_server::start()
{
  main_handler = new event_handler(size, cnt_threads);  
  main_handler->add(0, event::READ, server_sock);

  for(int i=0;i<cnt_threads-1;i++)
    server_th[i] = new std::thread(&http_server::routine, this, i+1);
  this->routine(0);

  return (main_handler!=NULL) ? 0 : -1;
}

int http_server::stop()
{
  /*
    TODO
    pthread_cancel(server_th->native_handle());
  */

  return 1;
}


void http_server::routine(int thread_idx)
{
  // TODO : check
  //signal(SIGPIPE, SIG_IGN);

  tcp_socket *client_sock;

  http_request* req;
  http_response* res;

  int server_sockfd = server_sock->get_file_descriptor();
  fprintf(stderr,"server sockfd : %d\n",server_sockfd);

  while(1)
  {
    //fprintf(stderr,"\n\n\nwating..!\n");
    int n = main_handler->wait(thread_idx, -1);
    //fprintf(stderr,"wating end..!\n");

    event* evnt;
    tcp_socket* tmp;

    for(int i=0;i<n;i++)
    {
      evnt = main_handler->get_ith_event(thread_idx, i);
      tmp = evnt->get_socket();

      if(server_sockfd == tmp->get_file_descriptor())
      {
        while(1)
        {
          client_sock = server_sock->accept();
          if(client_sock->get_file_descriptor() == -1) break;
          main_handler->add(thread_idx, event::READ, client_sock);
          delete(client_sock);
        }
      }
      else 
      { 
        while(1)
        {
          req = new http_request(tmp);  
          //res = handler(req);

          if(req->method == http_request::ERR)
          {
            main_handler->del(evnt);
            tmp->close_socket();

            delete req;
            break;
          }
            
          res = new http_response();
          res->send(tmp);

          delete res;
          delete req;
        }
      }
    }
  }
}

