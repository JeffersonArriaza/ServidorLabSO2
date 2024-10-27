#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <dirent.h>
#include "server_functions.h"
#include "log.h"

int srv_init(int port) {

    int server_fd; //Declara el File Descriptor
    SA_IN address;  //Declara la Direccion del Socket
    socklen_t address_len = sizeof(address);

    //Declara el Socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        log_error("Ha ocurrido un error al crear el socket\n");
        exit(EXIT_FAILURE);
    }

    //Configurar el Socket
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    //Bindear el Socket con la dirección y puerto
    if (bind(server_fd, (SA *)&address, sizeof(address)) < 0) {
        log_error("Ha ocurrido un error en el bindeo del Socket\n");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    //Colocar el socket en modo escucha/pasivo
    if (listen(server_fd, LISTENQ) < 0) { //LISTENQ es la cola de conexiones
        log_error("Ha ocurrido un error al configurar el Socket en el modo escucha/pasivo\n");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    log_event("El servidor está en modo escucha.");
    printf("El servidor funciona correctamente.Puerto: %d\n", port);
    return server_fd;
}

int srv_accept_client(int server_fd) {
    int client_fd;
    SA_IN client_address;  
    socklen_t client_len = sizeof(client_address);
     char message[100];

    //Aceptar una conexión
    if ((client_fd = accept(server_fd, (SA *)&client_address, &client_len)) < 0) {
        // Manejo de errores al aceptar conexión
        if (errno == ENOBUFS) {
            log_error("Error: La cola de conexiones está llena (ENOBUFS).");
        } else {
            char error_message[256];
            snprintf(error_message, sizeof(error_message), "Error al aceptar una conexión: %s", strerror(errno));
            log_error(error_message);
        }
        return -1; // Retorna -1 en caso de error
    }

    snprintf(message, sizeof(message), "Nueva conexión aceptada desde %s:%d",inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
    log_event(message);
    return client_fd;
}

