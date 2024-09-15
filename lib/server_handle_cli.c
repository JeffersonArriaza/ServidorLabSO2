#include "server_handle_cli.h"
#include "log.h"
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

//Funcion para manejar la solicitud del cliente
void srv_handle_client(int client_fd) {
    char buffer[BUFFER_SIZE] = {0};
    http_req request;
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char client_ip[INET_ADDRSTRLEN];
    char log_message[512];

    //Obtener la información del cliente
    getpeername(client_fd, (struct sockaddr *)&client_addr, &addr_len); //recupera la direccion del mismo nivel al que está conectado un socket.
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
    int client_port = ntohs(client_addr.sin_port);

    //Leer la solicitud del cliente
    ssize_t read_bytes = read(client_fd, buffer, sizeof(buffer) - 1);
    if (read_bytes <= 0) {
        log_error("No se puede leer la solicitud del cliente.");
        return;
    }

    //Analizar la solicitud HTTP
    parse_request(buffer, &request);

    //Verificar si el metodo es GET
    if (strcmp(request.method, "GET") != 0) {
        log_error("Metodo no corresponde a GET.");
        return;
    }

    //Registrar la solicitud en los logs de eventos
    snprintf(log_message, sizeof(log_message), "El cliente %s:%d solicita conexion en %s con el método %s", client_ip, client_port, request.url, request.method);
    log_event(log_message);
}

//Funcion para extraer el metodo y la URL
void parse_request(const char *request, http_req *req) {
    int i = 0, j = 0;
    
    //Extraer el metodo (primera palabra)
    while (request[i] != ' ' && request[i] != '\0') {
        req->method[j++] = request[i++];
    }
    req->method[j] = '\0';  //Fin de cadena del metodo
    i++; 

    //Reiniciar el índice para la URL
    j = 0;

    //Extraer la URL (segunda palabra)
    while (request[i] != ' ' && request[i] != '\0') {
        req->url[j++] = request[i++];
    }
    req->url[j] = '\0';  //Fin de cadena de la URL
}