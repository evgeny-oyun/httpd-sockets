#ifndef _HTTP_SOCKETS_C
#define _HTTP_SOCKETS_C

#include "sockets.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void
httpd_socket_set_nonblock(int fd)
{
  fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

extern int
httpd_socket_unix(httpd_socket_t *st, const char *path)
{
  int                 len, fd;
  struct sockaddr_un  local;
  const unsigned int  on = 1;
  struct timeval      tmout;

  tmout.tv_sec        = (time_t)st->timeout;
  tmout.tv_usec       = 0;

  if((fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
  {
    return -1;
  }

  memset(&local, 0, sizeof(struct sockaddr_un));

  local.sun_family = AF_UNIX;
  strncpy(local.sun_path,path,sizeof(local.sun_path));

  unlink(local.sun_path);

  len = strlen(local.sun_path) + sizeof(local.sun_family)+1;

  if((setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(const unsigned int)))==-1)
  {
    return -1;
  }

  if(setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &st->buffer_size, sizeof(int)) == -1)
  {
    return -1;
  }

  // @TODO: make it as optional
  if(setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (struct timeval*)&tmout,sizeof(tmout)) == -1)
  {
    return -1;
  }

  // @TODO: make it as optional
  if(setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, (struct timeval*)&tmout,sizeof(tmout)) == -1)
  {
    return -1;
  }

  if (bind(fd, (struct sockaddr *)&local,len) == -1)
  {
    return -1;
  }

  if (listen(fd,st->backlog) == -1)
  {
    return -1;
  }

  chmod(path,0777);

  return fd;
}

extern int
httpd_socket_tcp(httpd_socket_t *st, const char *l_host, const char *l_port)
{
  /* local variables */
  int                 fd;
  const unsigned int  on = 1;
  struct timeval      tmout;
  struct addrinfo     hints, *res;

  tmout.tv_sec        = (time_t)st->timeout;
  tmout.tv_usec       = 0;

  memset(&hints, 0, sizeof hints);

  hints.ai_family     = AF_UNSPEC;
  hints.ai_socktype   = SOCK_STREAM;
  //hints.ai_flags = AI_PASSIVE;

  getaddrinfo(l_host, l_port, &hints, &res);

  /* init socket */
  if((fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1)
  {
    freeaddrinfo(res);
    return -1;
  }


  #if(defined __FreeBSD__)
  if((setsockopt(fd, IPPROTO_TCP, TCP_NOPUSH, &on, sizeof(const unsigned int)))==-1)
  {
    freeaddrinfo(res);
    return -1;
  }
  #endif

  #if defined(__linux__)
  if((setsockopt(fd, IPPROTO_TCP, TCP_CORK, &on, sizeof(const unsigned int)))==-1)
  {
    freeaddrinfo(res);
    return -1;
  }
  #endif

  if((setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(const unsigned int)))==-1)
  {
    freeaddrinfo(res);
    return -1;
  }

  if (setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &st->buffer_size, sizeof(int)) == -1)
  {
    freeaddrinfo(res);
    return -1;
  }

  if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tmout,sizeof(tmout)) == -1)
  {
    freeaddrinfo(res);
    return -1;
  }

  if (setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, (char *)&tmout,sizeof(tmout)) == -1)
  {
    freeaddrinfo(res);
    return -1;
  }

  /* binding */
  if (bind(fd, res->ai_addr, res->ai_addrlen) == -1)
  {
    freeaddrinfo(res);
    return -1;
  }

  /* listening */
  if(fd == -1 || -1 == (listen(fd,st->backlog)))
  {
    freeaddrinfo(res);
    return -1;
  }

  freeaddrinfo(res);

  return fd;
}

extern httpd_socket_t*
httpd_socket_init(const char *address, int backlog, int timeout, int buffer_size)
{
  char              key[256], value[512];
  int               i = 0, l = 0, k = 0,
                    v = 0, f = 0, t = 0;
  httpd_socket_t   *s = NULL;

  if(address == NULL || (l = strlen(address)) <= 4)
  {
    return NULL;
  }

  if((s = (httpd_socket_t*)calloc(1, sizeof(httpd_socket_t))) == NULL)
  {
    return NULL;
  }

  // defaults
  s->backlog      = backlog;
  s->buffer_size  = buffer_size;
  s->timeout      = timeout;
  s->socket       = -1;

  for(i = 0; i < l; i++)
  {
    if(isspace(address[i]))
    {
      continue;
    }

    if(address[i] == ':')
    {
      if(t == 0)
      {
          key[k] = '\0';
      }
      t++;
    }
    else
    {
      if(t > 0 && isdigit(address[i]))
      {
        value[v] = address[i];
        v++;
      }
      else if(t == 0)
      {
        if(address[i] == '/')
        {
          f = 1;
        }

        key[k] = address[i];
        k++;
      }
    }
  }

  key[k] = '\0';
  value[v] = '\0';

  if(f == 0 && v > 0)
  {
    // seems like a tcp socket
    s->socket = httpd_socket_tcp(s, key, value);
  }
  else
  {
    // seems like a unix socket
    s->socket = httpd_socket_unix(s, key);
  }

  if(s->socket == -1)
  {
    free(s);
    return NULL;
  }

  return s;
}

extern void httpd_socket_free(httpd_socket_t *st)
{
  if(st == NULL)
  {
    return;
  }

  free(st);
}

#ifdef __cplusplus
}
#endif

#endif // _HTTP_SOCKETS_C
