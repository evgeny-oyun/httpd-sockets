# Socket binding address wrapper
## Usage
```c
#include "sockets.c"

socket_t *app_socket = socket_init(
  "localhost:1234", // or "/full/path/to/unix.socket"
  1024, // backlog size
  60, // read/write timeout
  4096 // buffer size
);

// get socket value
printf("Socket is %d\n", app_socket->socket);

socket_free(app_socket); // release used memory
```
