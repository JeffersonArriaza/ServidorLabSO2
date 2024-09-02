#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "lib/server_functions.h"
#include "lib/log.h"

#define PORT 5050

int main() {

    int server_fd, client_fd;
    server_fd = srv_init(PORT);

    while (1) {
        
        client_fd = srv_accept_client(server_fd);

        if (client_fd < 0) {
            continue;
        }
    sleep(3); //Espera 3 segundos para no cerrar la conexion inmediatamente
    close(client_fd);
    }

 close(server_fd);
 return 0;
} 