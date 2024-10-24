
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "Response.h"
#include "Server.h"
#include "Request.h"


void sendWelcomePageResponse(int client_socket, char* request_command){
    char response_page_with_requested_image[1000];
    sprintf(response_page_with_requested_image,
         "HTTP/1.1 200 OK\nDate: Fri, 20 Sep 2024 12:28:53 GMT\n"
         "Last-Modified: Fri, 20 Sep 2024 12:28:53 GMT\n"
         "Content-Type: text/html\nConnection: Closed\n\n"
         "<!DOCTYPE html><html><body> <h1>Operation is: %s</h1><h2>Original Image</h2><img src='%s'>"
         "<h2>Processed Image</h2><img src='%s'></body></html>"
         , request_command, "indirilen_resim.jpg", "process_edilen_resim.jpg");

    send(client_socket , response_page_with_requested_image , strlen(response_page_with_requested_image), 0);

}

void sendImageResponse(int client_socket, char* image_name) {

    FILE *imageFile = fopen(image_name, "r");

    if (!imageFile) {

        printf("\n\ndosya yok!!\n\n");
        const char *notFoundResponse =
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Type: text/html\r\n"
            "Connection: Closed\r\n\r\n"
            "<h1>404 Not Found</h1>";
        write(client_socket, notFoundResponse, strlen(notFoundResponse));
        return;
    }

     // Resmin boyutunu bulmak.
    fseek(imageFile, 0, SEEK_END);
    unsigned long imageSize = ftell(imageFile);
    fseek(imageFile, 0, SEEK_SET);

    //Şimdi de headerlar gönderilecek.
    char responseHeader[512];
    sprintf(responseHeader,
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: image/jpeg\r\n"
             "Content-Length: %ld\r\n"
             "\r\n", imageSize);

    send(client_socket, responseHeader, strlen(responseHeader), 0); //önce headerlar yollanacak.

    //Şimdi de image dosyası fread() ile okunarak send() methoduyla clienta response olarak gönderilecektir.
    char buffer[9092];
    size_t bytes_read; // fread() fonksiyonu size_t tipinde veri return ediyor.
    while((bytes_read = fread(buffer, 1, sizeof(buffer), imageFile)) > 0){
        //fread() filedaki dataları okuyarak char bufferına kaydetmeye yarar.
        send(client_socket, buffer, bytes_read, 0);
    }

    fclose(imageFile);

}

void send_warning_to_client(int client_socket, char* message){

    char response_warning[1000];
    sprintf(response_warning,
         "HTTP/1.1 200 OK\nDate: Fri, 20 Sep 2024 12:28:53 GMT\n"
         "Last-Modified: Fri, 20 Sep 2024 12:28:53 GMT\n"
         "Content-Type: text/html\nConnection: Closed\n\n"
         "<!DOCTYPE html><html><body> <h1>Please provide %s</h1>"
         "</body></html>"
         , message);

    send(client_socket , response_warning , strlen(response_warning), 0);


}
