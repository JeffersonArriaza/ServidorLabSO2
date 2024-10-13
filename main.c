#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "lib/server_functions.h"
#include "lib/log.h"
#include "lib/server_handle_cli.h"

#define PORT 5050

int main() {

    int server_fd, client_fd;
    server_fd = srv_init(PORT);

    while (1) {
        
        client_fd = srv_accept_client(server_fd);

        if (client_fd < 0) {
            continue;
        }

    srv_handle_client(client_fd); //Función para manejar la solicitud HTTP del cliente
    close(client_fd); //Cerrar el socket del cliente
    }
    
 log_event("Conexión cerrada con el cliente.");
 close(server_fd);
 return 0;
} 