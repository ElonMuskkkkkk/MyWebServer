#ifndef SERVER_H_
#define SERVER_H_

#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <memory.h>
#include <fcntl.h>
#include <stdio.h>
#include "error.h"

#define IP "192.168.109.131"
#define PORT 9090

class SERVER
{
private:
      int socketfd;
      int epollFd;
      int port;
      struct sockaddr_in family;
      struct epoll_event ev;

public:
      SERVER();
      ~SERVER();
      void start_up();

private:
      void s_socket();
      void s_bind();
      void s_listen();
      int s_accept();
      void addfd();
      void setnonblock(int fd = -1);
      void wait();
};

#endif