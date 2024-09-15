#ifndef SERVER_HANDLE_CLI_H
#define SERVER_HANDLE_CLI_H

#include <stdio.h>

//Longitud para la solicitud HTTP
#define BUFFER_SIZE 4096 //4KB, (Posibilidad de subir a 8KB)

//Estructura para almacenar el metodo y la URL de la petición HTTP
typedef struct {
    char method[8]; //valido para soportar GET, POST, PUT, DELETE, OPTIONS, HEAD, TRACE, CONNECT
    char url[512]; 
} http_req;

void srv_handle_client(int client_fd);
void parse_request(const char *request, http_req *req);

#endif
