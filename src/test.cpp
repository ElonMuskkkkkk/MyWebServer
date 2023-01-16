#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <sys/epoll.h>
#include <unistd.h>
#include "error.h"
#include "fcntl.h"

using namespace std;

class server
{
public:
      server(in_addr_t iIp, uint16_t usPort)
      {
            this->iSockFd = -1;
            this->iEpollFd = -1;
            memset(&oAddr, 0, sizeof(oAddr));
            memset(&oEvent, 0, sizeof(oEvent));
            oAddr.sin_family = AF_INET;
            oAddr.sin_addr.s_addr = iIp;
            oAddr.sin_port = htons(usPort);
      }

      void start_up()
      {
            s_socket();
            s_bind();
            s_listen();
            addfd();
            setnonblock();
            wait();
      }
      void s_socket()
      {
            if ((iSockFd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
                  error("socket", POS);
      }

      void s_bind()
      {
            if (bind(iSockFd, (sockaddr *)&oAddr, sizeof(oAddr)) < 0)
            {
                  cout << strerror(errno) << endl;
                  error("bind", POS);
            }
      }

      void s_listen()
      {
            if (listen(iSockFd, 100) < 0)
            {
                  error("listen", POS);
            }
      }

      void addfd()
      {
            iEpollFd = epoll_create(1024);
            if (iEpollFd < 0)
                  error("epoll_create", POS);
            oEvent.events = EPOLLIN;
            oEvent.data.fd = iSockFd;
            if (epoll_ctl(iEpollFd, EPOLL_CTL_ADD, iSockFd, &oEvent) < 0)
            {
                  error("epoll_ctl", POS);
            }
      }

      void wait()
      {
            epoll_event aoEvents[1024];
            while (true)
            {
                  int iFdCnt = epoll_wait(iEpollFd, aoEvents, 1024, -1);
                  if (iFdCnt < 0)
                  {
                        error("epoll_wait", POS);
                  }
                  else
                        cout << "success!" << endl;
            }
      }

      void setnonblock(int fd = -1)
      {
            if (fd < 0)
            {
                  fd = iSockFd;
            }
            int flag = fcntl(fd, F_GETFL);
            flag |= O_NONBLOCK;
            fcntl(fd, F_SETFL, flag);
      }

private:
      int iSockFd;
      int iEpollFd;
      sockaddr_in oAddr;
      epoll_event oEvent;
};

int main(int argc, char *argv[])
{
      if (argc != 3)
      {
            cout << "usage: " << argv[0] << " ip port" << endl;
            return -1;
      }

      char *szIp = argv[1];
      in_addr_t iIp = inet_addr(szIp);
      if (iIp == INADDR_NONE)
      {
            cerr << "fail to parse ip: " << szIp << endl;
            return -1;
      }
      char *pEnd = NULL;
      uint16_t usPort = strtoul(argv[2], &pEnd, 10);
      if (*pEnd != '\0')
      {
            cerr << "fail to parse port: " << argv[2] << endl;
            return -1;
      }

      server server(iIp, usPort);
      server.start_up();
}