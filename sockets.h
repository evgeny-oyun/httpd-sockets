#ifndef _HTTP_SOCKETS_H
#define _HTTP_SOCKETS_H

/* headers */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/select.h>
#include <fcntl.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <sys/un.h>
#include <netinet/tcp.h>
#include <pwd.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <ctype.h>

/* structures */
typedef struct httpd_socket_s{
  int backlog;
  int timeout;
  int buffer_size;
  int socket;
} httpd_socket_t;

#ifdef __cplusplus
extern "C" {
#endif

extern void httpd_socket_free(httpd_socket_t*);
extern httpd_socket_t*httpd_socket_init(const char*, int, int, int);
extern int httpd_socket_tcp(httpd_socket_t*, const char*, const char*);
extern int httpd_socket_unix(httpd_socket_t *st, const char *path);
extern void httpd_socket_set_nonblock(int);

#ifdef __cplusplus
}
#endif

#endif // _HTTP_SOCKETS_H
