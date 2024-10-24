
#include <stdlib.h>
#include <stdio.h>

struct RequestProperties{
    char http_method[10];
    char command[10]; //resize, rotate, crop, grayscale gibi commandler gelecek.
    char image_url[1000]; //Resim bu urlden "wget" commandi ile download edilecektir.
    int width;
    int height;
    int x_offset;
    int y_offset;
    double rotate_amount;

};

struct RequestProperties* request_constructor(
    char *http_method,
    char *command,
    char *image_url,
    int width,
    int height,
    int x_offset,
    int y_offset,
    double rotate_amount
);

struct RequestProperties* parse_request(
    char* request_buffer,
    struct RequestProperties* request);



struct RequestProperties* parse_request_resize(
    char* request_buffer, struct RequestProperties* request,
    char *http_method, char *uri);


struct RequestProperties* parse_request_crop(
    char* request_buffer, struct RequestProperties* request,
    char *http_method, char *uri);


struct RequestProperties* parse_request_grayscale(
    char* request_buffer, struct RequestProperties* request,
    char *http_method, char *uri);

struct RequestProperties* parse_request_rotate(
    char* request_buffer, struct RequestProperties* request,
    char *http_method, char *uri);


