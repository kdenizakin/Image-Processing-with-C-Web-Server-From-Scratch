
#include "Server.h"



struct ServerProperties* server_constructor(int domain, int service, int protocol,
    u_long interface, int port, int backlog){

    struct ServerProperties* serverProp = (struct ServerProperties*) malloc(sizeof(struct ServerProperties));

    serverProp->domain = domain;
    serverProp->service = service;
    serverProp->protocol = protocol;
    serverProp->interface = interface;
    serverProp->port = port;
    serverProp->backlog = backlog;

    serverProp->addr.sin_family = domain;
    serverProp->addr.sin_port = htons(port); //htons() fonksiyonu integer olan
    //port addresini 16-bit network byte tipine convert eder.
    serverProp->addr.sin_addr.s_addr = htonl(interface); //htonl() htonsa benzer şekilde long tipinden
    //network byte tipine convert eder. Ancak bu 32-bittir.


    //Şimdi socket tanımlanacak. Socket Operating system ve network arasındaki API'dır. Clientlar socketin
    //olduğu porta bağlanırlar.
    //socket() fonksiyonu kullanılacak. Bu fonksiyon socketin oluşturulduğu port numarasını return eder.
    serverProp->socket = socket(serverProp->domain, serverProp->service, serverProp->protocol);

    if(serverProp->socket == 0){

        perror("Socket oluşturmada hata!\n");
        exit(1);
    }


    int bindingResult = bind(serverProp->socket, (struct sockaddr *) &serverProp->addr, sizeof(serverProp->addr));

    if(bindingResult <0){

        perror("Binding sırasında hata!\n");
        exit(1);
    }


    //Şimdi de server, socketi dinlemeye başlayacak. Buradaki backlog = kaç tane bağlantının sırada beklediği.
    int listenResult = listen(serverProp->socket, serverProp->backlog);

    if(listenResult<0){

        perror("Dinlemede hata!\n");
        exit(1);

    }

    return serverProp;

}
