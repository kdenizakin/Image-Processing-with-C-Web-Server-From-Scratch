

#define Server_h
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

struct ServerProperties{
    int domain;
    int service;
    int protocol;
    u_long interface;
    int port;
    int backlog;
    int socket;

    struct sockaddr_in addr;

};


struct ServerProperties* server_constructor(int domain, int service, int protocol,
    u_long interface, int port, int backlog);
