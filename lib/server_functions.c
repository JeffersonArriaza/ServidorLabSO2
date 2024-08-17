#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <dirent.h>
#include "server_functions.h"

int srv_init(int port) {

    int server_fd; //Declara el File Descriptor
    SA_IN address;  //Declara la Direccion de

    //Declara el Socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Ha ocurrido un error al crear el socket\n");
        exit(EXIT_FAILURE);
    }

    //Configurar el Socket
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    //Bindear el Socket con la dirección y puerto
    if (bind(server_fd, (SA *)&address, sizeof(address)) < 0) {
        perror("Ha ocurrido un error en el bindeo del Socket\n");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    //Colocar el socket en modo escucha/pasivo
    if (listen(server_fd, 3) < 0) { //Cambiar en el futuro
        perror("Ha ocurrido un error al configurar el Socket en el modo escucha/pasivo\n");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("El servidor funciona correctamente.Puerto: %d\n", port);
    return server_fd;
}

int srv_accept_client(int server_fd) {
    int client_fd;
    SA_IN client_address;  
    socklen_t client_len = sizeof(client_address);

    //Aceptar una conexión
    if ((client_fd = accept(server_fd, (SA *)&client_address, &client_len)) < 0) {
        perror("Error al aceptar una conexion");
        return -1;

    }else{

        printf("Nueva conexión aceptada desde %s:%d\n", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
        return client_fd;
    }
}

