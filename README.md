# Socket binding address wrapper
## Usage
```c
#include "http_sockets.c"

httpd_socket_t *http_socket = httpd_socket_init(
  "localhost:1234", // or "/full/path/to/unix.socket"
  1024, // backlog size
  60, // read/write timeout
  4096 // buffer size
);

// get socket value
printf("Socket is %d\n", http_socket->socket);

httpd_socket_free(http_socket); // release used memory
```
