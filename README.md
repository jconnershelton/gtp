# G Transfer Protocol

The G Transfer Protocol (GTP) is an open-source protocol and C library aimed at making sending messages between a client and server much easier. Under the hood, GTP uses traditional TCP/IP sockets but wraps them in a developer-friendly library that reduces the headaches, mistakes, and security flaws that often come about when dealing with raw sockets.

GTP is also a very lightweight, flexible, and intuitive solution for most request-response communication. Any data, from numbers to text to images to raw binary, can be encapsulated in a `GTPBuffer` along with a name that identifies it. This buffer can then be sent to a server running GTP.

## Install

Right now, the only method for installation is building the library yourself, but the process is simple.
**NOTE:** *Currently, only MacOS and Linux machines are supported. Attempting an install with the included Makefile on Windows will fail.*

To install GTP on your machine, simply navigate to the cloned GTP directory in a terminal and type:

`$ make install`

This will place both static and dynamic libraries in `/usr/local/lib` as well as all public header files in `/usr/local/include`.
If you only want to build and not install the library, type:

`$ make gtp-static` *or* `$ make gtp-dynamic`

This will compile the static or dynamic library and place it in the `lib` directory. All public header files are located in the `include` directory.

## Server Example

Hosting a server using GTP is very simple. This line will get you up and running:

```c
#include <GTP/serve.h>

GTP_Serve(unsigned short port, GTPBuffer *(*processBuffer)(const GTPBuffer *buffer, bool *serving));
```

The first argument is the port that the server will be hosted on (*in host byte order*), and the second argument is a function pointer that is called whenever the server receives a request. The function should take in the request buffer, process it in some arbitrary way, and return a new buffer to send back to the client. Additionally, this function can set the parameter `serving` to false to terminate the server.

## Client Example

Connecting and sending a request to a GTP server is also very easy with this line:

```c
#include <GTP/send.h>

GTPBuffer *response = GTP_Send(GTPBuffer *buffer, const char *ipAddress, unsigned short port);
```

The first argument is the buffer to be sent, while the second and third are the IP address and serving port of the destination machine. A pointer to the response buffer is returned, and elements can be fetched from it using `GTPBuffer_GetElement(GTPBuffer *buffer, const char *name)`.

**NOTE:** *The programmer is responsible for freeing allocated GTPBuffers with* `GTPBuffer_Free(GTPBuffer *buffer)` *to prevent memory leaks. Also, all GTPBuffers should be allocated using* `GTPBuffer_Alloc()` *to avoid passing stack pointers.*

### License

[MIT License](LICENSE)