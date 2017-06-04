#ifndef _SOCKETS_H
#define _SOCKETS_H

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
typedef struct socket_s{
  int backlog;
  int timeout;
  int buffer_size;
  int socket;
} socket_t;

#ifdef __cplusplus
extern "C" {
#endif

extern void socket_free(socket_t*);
extern socket_t *socket_init(const char*, int, int, int);
extern int socket_tcp(socket_t*, const char*, const char*);
extern int socket_unix(socket_t *st, const char *path);
extern void socket_set_nonblock(int);

#ifdef __cplusplus
}
#endif

#endif // _SOCKETS_H
