#ifndef HTTP_CONN_H_
#define HTTP_CONN_H_

#include <sys/socket.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/uio.h>
#include <memory.h>

#define WRITE_BUFFER_SIZE 1024
#define READ_BUFFER_SIZE 1024

class HTTP_CONN
{
public:
      enum METHOD
      {
            GET = 0,
            POST,
            HEAD,
            PUT,
            DELETE,
            TRACE,
            OPTIONS,
            CONNECT,
            PATH
      };
      enum CHECK_STATE
      {
            CHECK_STATE_REQUESTLINE = 0,
            CHECK_STATE_HEADER,
            CHECK_STATE_CONTENT
      };
      enum HTTP_CODE
      {
            NO_REQUEST,
            GET_REQUEST,
            BAD_REQUEST,
            NO_RESOURCE,
            FORBIDDEN_REQUEST,
            FILE_REQUEST,
            INTERNAL_ERROR,
            CLOSED_CONNECTION
      };
      enum LINE_STATUS
      {
            LINE_OK = 0,
            LINE_BAD,
            LINE_OPEN
      };

public:
      HTTP_CONN(int m_socketfd, int m_epollfd);
      ~HTTP_CONN();

      // void process();

private:
      void init();
      void addfd();

private:
      int m_socketfd;
      int m_epollfd;

      char write_buffer[WRITE_BUFFER_SIZE];
      char read_buffer[READ_BUFFER_SIZE];
      int w_idx;
      int r_idx;
      int check_idx;
      int start_idx;

      char *m_file_addr;
      char *m_version;
      char *m_url;
      int content_len;
      char *m_host;
      struct iovec m_iv[2];
      struct stat m_file_state;
};

#endif