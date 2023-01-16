#include "server.h"
#include "http_conn.h"
#include <unordered_map>
#include <errno.h>

using namespace std;

unordered_map<int, HTTP_CONN> m_hash;

SERVER::SERVER()
{
      this->socketfd = -1;
      this->epollFd = -1;
      this->port = PORT;
      memset(&family, 0, sizeof(family));
      memset(&ev, 0, sizeof(ev));
      this->family.sin_addr.s_addr = inet_addr(IP);
      this->family.sin_family = AF_INET;
      this->family.sin_port = htons(PORT);
}

SERVER::~SERVER()
{
      close(socketfd);
      this->socketfd = -1;
      close(epollFd);
      this->epollFd = -1;
}

void SERVER::start_up()
{
      s_socket();
      // setnonblock();
      s_bind();
      s_listen();
      addfd();
      wait();
}

void SERVER::s_socket()
{
      int opt = 1;
      if ((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
            error("s_socket", POS);
      setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, (void *)&opt, sizeof(int));
}

void SERVER::s_bind()
{
      if (bind(socketfd, (sockaddr *)&family, sizeof(family)) < 0)
            error("s_bind", POS);
}

void SERVER::s_listen()
{
      if (listen(socketfd, 10) < 0)
            error("s_listen", POS);
}

int SERVER::s_accept()
{
      socklen_t len = 0;
      int fd = -1;
      if ((fd = accept(socketfd, (sockaddr *)&family, &len)) < 0)
            error("s_accept", POS);
      return fd;
}

void SERVER::addfd()
{
      if ((epollFd = epoll_create(10)) < 0)
            error("epoll_create", POS);
      ev.data.fd = socketfd;
      ev.events = EPOLLIN;
      if (epoll_ctl(epollFd, EPOLL_CTL_ADD, socketfd, &ev) < 0)
            error("epoll_ctl", POS);
}

void SERVER::wait()
{
      while (true)
      {
            int nfds = -1;
            epoll_event evs[10];
            if ((nfds = epoll_wait(epollFd, evs, 10, -1)) < 0)
            {
                  printf("%d\n", epollFd);
                  error("wait", POS);
            }
            for (int i = 0; i < nfds; i++)
            {
                  if (evs[i].data.fd == socketfd)
                  {
                        int fd = s_accept();
                        m_hash.insert({fd, HTTP_CONN(fd, epollFd)});
                  }
                  else if (evs[i].events & EPOLLIN)
                  {
                        printf("%s\n", "123");
                  }
            }
      }
}

void SERVER::setnonblock(int fd)
{
      if (fd < 0)
      {
            fd = socketfd;
      }
      int flag = fcntl(fd, F_GETFL);
      flag |= O_NONBLOCK;
      fcntl(fd, F_SETFL, flag);
}