#include "http_conn.h"

HTTP_CONN::HTTP_CONN(int skfd, int epfd)
{
      m_socketfd = skfd;
      m_epollfd = epfd;
      init();
}

HTTP_CONN::~HTTP_CONN()
{
      close(m_socketfd);
      close(m_epollfd);
      m_socketfd = -1;
      m_epollfd = -1;
}

void HTTP_CONN::init()
{
      memset(write_buffer, 0, WRITE_BUFFER_SIZE);
      memset(read_buffer, 0, READ_BUFFER_SIZE);
      w_idx = 0;
      r_idx = 0;
      check_idx = 0;
      start_idx = 0;
      content_len = 0;
      m_file_addr = NULL;
      m_version = NULL;
      m_url = NULL;
      m_host = NULL;
      addfd();
}

void HTTP_CONN::addfd()
{
      epoll_event h_ev;
      h_ev.data.fd = m_socketfd;
      h_ev.events = EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLONESHOT;
      epoll_ctl(m_epollfd, EPOLL_CTL_ADD, m_socketfd, &h_ev);
}