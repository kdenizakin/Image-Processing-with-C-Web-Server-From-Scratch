//Bu class kendi yazdığım Request.h kütüphanesini import ediyor. BU kütüphanede RequestProperties structı var.
//Hem bir Request objesi oluşturmaya yarıyor hem de clienttan gönderilen requesti parse etmeye yarayan
//parse_request methodu var.


#include "Request.h"
#include <string.h>


struct RequestProperties* request_constructor(char *http_method, char *command,
            char *image_url, int width, int height, int x_offset, int y_offset, double rotate_amount){

    struct RequestProperties* request = (struct RequestProperties*)
            malloc(sizeof(struct RequestProperties));

    strcpy(request->http_method, http_method);
    strcpy(request->command, command);
    strcpy(request->image_url, image_url);

    request->width = width;
    request->height = height;
    request->x_offset = x_offset;
    request->y_offset = y_offset;
    request->rotate_amount = rotate_amount;


    return request;
}


struct RequestProperties* parse_request(char* request_buffer, struct RequestProperties* request){

    //GET /favicon.ico HTTP/1.1


    char *http_method = (char*) malloc(10*sizeof(char));
    strcpy(http_method, strtok(request_buffer," "));

    char *uri = (char*) malloc(1000*sizeof(char));
    strcpy(uri, strtok(NULL," "));

    char *version = (char*) malloc(100*sizeof(char));
    strcpy(version, strtok(NULL,"\n"));


    // /<hostname>:<port>/v1/<operation>:<parameters>/.../url:<image url without the protocol>

    if (strstr(uri, "v1") != NULL) { //Eğer v1 urlde varsa ona göre uri bir kez daha parse edilmeli.

        // "/v1/<operation>:<parameters>/.../url:<image url without the protocol>"
        //http://localhost:3234/v1/resize:300x200/url:picsum.photos/id/241/200/300


        if(strstr(uri, "resize")){
            request = parse_request_resize(request_buffer, request, http_method, uri);
        }
        else if(strstr(uri, "crop")){
            request = parse_request_crop(request_buffer, request, http_method, uri);
        }
        else if(strstr(uri, "grayscale")){
            request = parse_request_grayscale(request_buffer, request, http_method, uri);
        }
        else if(strstr(uri, "rotate")){
            request = parse_request_rotate(request_buffer, request, http_method, uri);
        }

        //Şimdi Heapten hepsini deallocate...
        free(http_method);
        free(uri);
        free(version);


        return request;
    }

    free(http_method);
    free(uri);
    free(version);

    return NULL;
}

struct RequestProperties* parse_request_resize(char* request_buffer, struct RequestProperties* request,
                                                char *http_method, char *uri){

    char *operation = (char*) malloc(200*sizeof(char));
    char *image_url = (char*) malloc(500*sizeof(char));
    char *parameters = (char*) malloc(200*sizeof(char));
    int width = 0;
    int height = 0;


    strtok(uri, "/"); //URL'deki "v1" kısmı için. Es geçiyorum.


    //Aşağıdaki 3 satır command değerini urlden almak için
    char* char_pointer_operation = strtok(NULL, "/:");
    if(char_pointer_operation != NULL) strcpy(operation, char_pointer_operation); //operation = resize olmalı.
    else *operation = '\0';


    //Aşağıdaki 3 satır width ve height parameterelerini aynı anda tutan string için
    char* char_pointer_parameters = strtok(NULL, "/:");
    if(char_pointer_parameters != NULL) strcpy(parameters, char_pointer_parameters);
    else *parameters = '\0';

    strtok(NULL, "/:"); //"url" keywordünü es geçmek için.

    //image urlsi için
    char* char_pointer_url = strtok(NULL, " ");
    if(char_pointer_url != NULL) strcpy(image_url, char_pointer_url);
    else *image_url = '\0';


    //şimdi parametreleri tutan stringi width ve height integerlarına ayırmak için
    if(parameters != NULL){
        char* width_str = strtok(parameters, "Xx");

        if(width_str !=NULL) width = atoi(width_str);

        char* height_str = strtok(NULL, " ");
        if(height_str !=NULL) height = atoi(height_str);
    }


    request = request_constructor(
        http_method, operation, image_url, width, height,
        0, 0, 0); //x_offset, y_offset ve rotate_amount değerleri resize ile alakasız ondan 0.


    free(operation);
    free(image_url);
    free(parameters);
    return request;

}


//Bu method için url ve parametreler aşağıdaki gibidir:
//http://localhost:3234/v1/crop:300x200x10x10/url:picsum.photos/id/241/200/300
//burada width = 300, height = 200, x_offset = 10 ve y_offset = 10'dur.
struct RequestProperties* parse_request_crop( char* request_buffer, struct RequestProperties* request,
                                                char *http_method, char *uri){

    char *operation = (char*) malloc(200*sizeof(char));
    char *image_url = (char*) malloc(500*sizeof(char));
    char *parameters = (char*) malloc(200*sizeof(char));
    int width = 0;
    int height = 0;
    int x_offset = 0;
    int y_offset = 0;


    strtok(uri, "/"); //URL'deki "v1" kısmı için. Es geçiyorum.


    //Aşağıdaki 3 satır command değerini urlden almak için
    char* char_pointer_operation = strtok(NULL, "/:");
    if(char_pointer_operation != NULL) strcpy(operation, char_pointer_operation); //operation = crop olmalı.
    else *operation = '\0';


    //Aşağıdaki 3 satır width ve height parameterelerini aynı anda tutan string için
    char* char_pointer_parameters = strtok(NULL, "/:");
    if(char_pointer_parameters != NULL) strcpy(parameters, char_pointer_parameters);
    else *parameters = '\0';

    strtok(NULL, "/:"); //"url" keywordünü es geçmek için.

    //image urlsi için
    char* char_pointer_url = strtok(NULL, " ");
    if(char_pointer_url != NULL) strcpy(image_url, char_pointer_url);
    else *image_url = '\0';



    if(parameters != NULL){
        char* width_str = strtok(parameters, "Xx");
        if(width_str !=NULL) width = atoi(width_str);

        char* height_str = strtok(NULL, "Xx");
        if(height_str !=NULL) height = atoi(height_str);

        char* x_offset_str = strtok(NULL, "Xx");
        if(x_offset_str !=NULL) x_offset = atoi(x_offset_str);

        char* y_offset_str = strtok(NULL, " ");
        if(y_offset_str !=NULL) y_offset = atoi(y_offset_str);
    }


    request = request_constructor(
        http_method, operation, image_url, width, height,
        x_offset, y_offset, 0);


    free(operation);
    free(image_url);
    free(parameters);
    return request;


}


//Bu method için url ve parametreler aşağıdaki gibidir:
//http://localhost:3234/v1/grayscale/url:picsum.photos/id/241/200/300
struct RequestProperties* parse_request_grayscale( char* request_buffer, struct RequestProperties* request,
                                                char *http_method, char *uri){

    char *operation = (char*) malloc(200*sizeof(char));
    char *image_url = (char*) malloc(500*sizeof(char));

    strtok(uri, "/"); //URL'deki "v1" kısmı için. Es geçiyorum.


    //Aşağıdaki 3 satır command değerini urlden almak için
    char* char_pointer_operation = strtok(NULL, "/:x");
    if(char_pointer_operation != NULL) strcpy(operation, char_pointer_operation); //operation = grayscale olmalı.
    else *operation = '\0';


    strtok(NULL, "/:"); //"url" keywordünü es geçmek için.

    //image urlsi için
    char* char_pointer_url = strtok(NULL, " ");
    if(char_pointer_url != NULL) strcpy(image_url, char_pointer_url);
    else *image_url = '\0';


    request = request_constructor(
        http_method, operation, image_url, 0, 0,
        0, 0, 0);

    free(operation);
    free(image_url);

    return request;


}

struct RequestProperties* parse_request_rotate(char* request_buffer, struct RequestProperties* request,
                                                char *http_method, char *uri){
    char *operation = (char*) malloc(200*sizeof(char));
    char *image_url = (char*) malloc(500*sizeof(char));
    char *parameters = (char*) malloc(200*sizeof(char));
    double rotate_amount = 0;

    strtok(uri, "/"); //URL'deki "v1" kısmı için. Es geçiyorum.


    //Aşağıdaki 3 satır command değerini urlden almak için
    char* char_pointer_operation = strtok(NULL, "/:");
    if(char_pointer_operation != NULL) strcpy(operation, char_pointer_operation); //operation = rotate olmalı.
    else *operation = '\0';

    char* char_pointer_parameters = strtok(NULL, "/");
    if(char_pointer_parameters != NULL) strcpy(parameters, char_pointer_parameters);
    else *parameters = '\0';

    strtok(NULL, "/:"); //"url" keywordünü es geçmek için.

    //image urlsi için
    char* char_pointer_url = strtok(NULL, " ");
    if(char_pointer_url != NULL) strcpy(image_url, char_pointer_url);
    else *image_url = '\0';

    if(parameters != NULL){
        rotate_amount = atof(parameters);
    }
    request = request_constructor(
        http_method, operation, image_url, 0, 0,
        0, 0, rotate_amount);


    free(operation);
    free(image_url);
    free(parameters);

    return request;

}



