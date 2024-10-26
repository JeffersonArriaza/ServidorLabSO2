#include "server_handle_cli.h"
#include "http_status_codes.h"
#include "file_handler.h"
#include "log.h"
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

void parse_request(const char *request, http_req *req) {
    int i = 0, j = 0;

    // Extraer el método (primera palabra)
    while (request[i] != ' ' && request[i] != '\0') {
        req->method[j++] = request[i++];
    }
    req->method[j] = '\0';  // Fin de cadena del método
    i++;  // Pasar el espacio

    // Reiniciar el índice para la URL
    j = 0;

    // Extraer la URL (segunda palabra)
    while (request[i] != ' ' && request[i] != '\0') {
        req->url[j++] = request[i++];
    }
    req->url[j] = '\0';  // Fin de cadena de la URL
}

void http_response(int client_fd, int http_code, const char* content_type, const char* body, size_t body_size) {
    const char* status_description = get_http_status_description(http_code);
    char headers[512];

    snprintf(headers, sizeof(headers),
             "HTTP/1.1 %d %s\r\n"
             "Content-Type: %s\r\n"
             "Content-Length: %zu\r\n"
             "Server: SimpleHTTPServer\r\n"
             "\r\n",
             http_code, status_description, content_type, body_size);

    // Enviar los headers
    send(client_fd, headers, strlen(headers), 0);

    // Enviar el cuerpo de la respuesta
    if (body && body_size > 0) {
        send(client_fd, body, body_size, 0);
    }
}

// Función para manejar la solicitud del cliente
void srv_handle_client(int client_fd) {
    char buffer[BUFFER_SIZE];
    http_req request;

    // Configuración de la dirección del cliente
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char client_ip[INET_ADDRSTRLEN];
    int client_port;
    int has_content = 0; // Bandera para controlar el free()

    // Obtener la información del cliente
    getpeername(client_fd, (struct sockaddr *)&client_addr, &addr_len);
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
    client_port = ntohs(client_addr.sin_port);

    // Leer la solicitud del cliente
    int read_bytes = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (read_bytes <= 0) {
        log_error("No se puede leer la solicitud del cliente.");
        close(client_fd);
        log_event("Conexión cerrada con el cliente.");
        return;
    }

    buffer[read_bytes] = '\0';  // Asegurarse de que la cadena esté terminada en nulo
    parse_request(buffer, &request);  // Analizar la solicitud HTTP
    int http_code;
    char *content = NULL;
    const char* content_type = NULL; // Inicializar content_type
    size_t content_size = 0; // Inicializar el tamaño del contenido

    // Verificar si el método es GET
    if (strcmp(request.method, "GET") != 0) {
        log_error("Método no corresponde a GET.");
        http_code = 405;  // Method Not Allowed
        content = "<h1>405 Method Not Allowed</h1>";
        content_type = "text/html";  // Establecer tipo de contenido por defecto
        content_size = strlen(content); // Establecer el tamaño del contenido
    } else {
        // Obtener el contenido del archivo solicitado
        content = get_file_contents(request.url, &http_code, &content_type, &content_size);  // Cambiado aquí
        if (content == NULL) {
            http_code = 404;
            content = "<h1>404 Not Found</h1>";
            content_type = "text/html";
            content_size = strlen(content); // Establecer el tamaño del contenido para error
            log_error("El archivo solicitado no fue encontrado.");
        } else {
            has_content = 1;  // Marcar como contenido asignado dinámicamente
        }
    }

    // Enviar la respuesta al cliente
    http_response(client_fd, http_code, content_type, content, content_size);

    // Registrar la solicitud en los logs de eventos
    char log_message[1024];
    snprintf(log_message, sizeof(log_message), "El cliente %s:%d solicita conexión en %s con el método %s",
             client_ip, client_port, request.url, request.method);
    log_event(log_message);

    if (has_content) {  // Liberar el contenido del archivo
        free(content);
    } 
    log_event("Conexión cerrada con el cliente.");
    close(client_fd); // Cerrar el socket del cliente
}


