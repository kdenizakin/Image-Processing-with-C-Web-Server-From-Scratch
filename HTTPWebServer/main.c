#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "Server.h"
#include "Request.h"
#include "Response.h"
#include "ProcessImageWithMagickWand.h"

void initiate(struct ServerProperties *serverProp, struct RequestProperties* request);
int download();
void handle_operation(int sockett, struct RequestProperties* request);


int main()
{
    //download();

    struct ServerProperties* server = (struct ServerProperties*) malloc(sizeof(struct ServerProperties));
    server = server_constructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 3000, 10);

    struct RequestProperties* request;

    initiate(server, request);



    return 0;
}


void initiate(struct ServerProperties *serverProp, struct RequestProperties* request){

    char requestBuffer[40000];
    int address_length = sizeof(serverProp->addr);
    int sockett;

    listen(serverProp->port, 5);
    printf("Server is listening on port %d...\n", serverProp->port);
    printf("Waiting for the connection...\n");

    while(1){

        sockett = accept(serverProp->socket, (struct sockaddr *) &(serverProp->addr), (socklen_t*)(&address_length));

        read(sockett, requestBuffer, 40000); //requesti okur.

        //Şimdi requesti parse ederek Request objesi oluşturulacak. Bunun için parse_request();
        printf("Buffer is: %s\n", requestBuffer);

        struct RequestProperties* request = parse_request(requestBuffer, request); //Request.c'deki methodu çağıracak.

        handle_operation(sockett, request);

        free(request); //Artık "request" objesiyle işimiz kalmadı o yüzden deallocate.

        close(sockett);
    }

}

int download(char* url_without_protocol){


    char wget_command[400] = "wget --tries=3 -O indirilen_resim.jpg ";
    strcat(wget_command, url_without_protocol);
    system(wget_command);

    return 0;
}

void handle_operation(int sockett, struct RequestProperties* request){

    if(request == NULL)
        return;

    printf("method name: %s\n", request->http_method);
    printf("operation mi :%s\n", request->command);
    printf("image_url mi :%s\n", request->image_url);
    printf("width mi :%d\n", request->width);
    printf("height mi :%d\n", request->height);
    printf("x_offset mi :%d\n", request->x_offset);
    printf("y_offset mi :%d\n", request->y_offset);

    if(request->image_url == NULL && (request->width == 0 || request->height == 0)
    && request->command == NULL){
        send_warning_to_client(sockett, "URL, command, width and height values correctly!");
        return;
    }

    else if(request->image_url != NULL && (request->width == 0 || request->height == 0)
    && request->command == NULL){
        send_warning_to_client(sockett, "command, width and height parameters correctly!");
        return;
    }
    else if(request->image_url == NULL && (request->width == 0 || request->height == 0)
    && request->command != NULL){
        send_warning_to_client(sockett, "URL, width and height parameters correctly!");
        return;
    }
    else if(request->image_url != NULL && (request->width != 0 || request->height != 0)
    && request->command == NULL){
        send_warning_to_client(sockett, "command value correctly!");
        return;
    }
    else if(request->image_url == NULL && (request->width != 0 || request->height != 0)
    && request->command != NULL){
        send_warning_to_client(sockett, "URL value correctly!");
        return;
    }
    //RESIZE İÇİN
    else if(request->image_url != NULL && (request->width == 0 || request->height == 0)
    && request->command != NULL && (strcmp(request->command, "resize")== 0)){
        send_warning_to_client(sockett, "width and height parameters correctly for RESIZE!");
        return;
    }
    //CROP İÇİN
    else if(request->image_url != NULL && (request->width == 0 || request->height == 0
    || request->x_offset == 0|| request->y_offset == 0)
    && request->command != NULL && (strcmp(request->command, "crop")== 0)){
        send_warning_to_client(sockett, "width, height, x_offset and y_offset parameters correctly for CROP!");
        return;
    }
    //ROTATE İÇİN
    else if(request->image_url != NULL && (request->rotate_amount == 0)
    && request->command != NULL && (strcmp(request->command, "rotate")== 0)){
        send_warning_to_client(sockett, "rotate amount for ROTATE!");
        return;
    }

    //GRAYSCALE İÇİN
    else if(request->image_url != NULL && (request->width != 0 || request->height != 0)
    && request->command != NULL && (strcmp(request->command, "grayscale")== 0)){
        send_warning_to_client(sockett, "no parameters should be given for operation GRAYSCALE!");
        return;
    }


    if(strstr(request->image_url, "indirilen_resim.jpg")){
        printf("HERE 1\n");
        sendImageResponse(sockett,"indirilen_resim.jpg");
    }
    else if(strstr(request->image_url, "process_edilen_resim.jpg")){
        printf("HERE 2\n");
        sendImageResponse(sockett,"process_edilen_resim.jpg");
    }
    else if(strcmp(request->command, "resize") == 0){
        printf("HERE 3\n");
        download(request->image_url);
        resizee_image("indirilen_resim.jpg", "process_edilen_resim.jpg", request->width, request->height);
        sendWelcomePageResponse(sockett, request->command);
    }
    else if(strcmp(request->command, "crop") == 0){
        printf("HERE 4\n");
        download(request->image_url);
        crop_image("indirilen_resim.jpg", "process_edilen_resim.jpg", request->width, request->height, request->x_offset, request->y_offset);
        sendWelcomePageResponse(sockett, request->command);
    }
    else if(strcmp(request->command, "grayscale") == 0){
        printf("HERE 5\n");
        download(request->image_url);
        grayscale_image("indirilen_resim.jpg", "process_edilen_resim.jpg");
        sendWelcomePageResponse(sockett, request->command);
    }
     else if(strcmp(request->command, "rotate") == 0){
        printf("HERE 5\n");
        download(request->image_url);
        rotate_image("indirilen_resim.jpg", "process_edilen_resim.jpg", request->rotate_amount);
        sendWelcomePageResponse(sockett, request->command);
    }
    else{
        char *command_not_found_str = (char*) malloc((strlen("command not found '%s'!") + strlen(request->command))*sizeof(char));
        sprintf(command_not_found_str, "command not found '%s'!", request->command);
        send_warning_to_client(sockett, command_not_found_str);

        free(command_not_found_str);

    }




}







