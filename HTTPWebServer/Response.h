
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


void sendImageResponse(int client_socket, char* image_name);
void sendWelcomePageResponse(int client_socket, char* request_command);
void send_warning_to_client(int client_socket, char* message);
