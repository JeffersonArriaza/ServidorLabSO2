#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "lib/server_functions.h"
#include "lib/log.h"
#include "lib/server_handle_cli.h"
#include <pthread.h>
#include "lib/queue.h"

#define PORT 5050
#define THREAD_POOL_SIZE 10 // Tamaño del pool de hilos

Queue *connection_queue; // Cola para manejar las conexiones

int main() {
    int server_fd, client_fd;
    server_fd = srv_init(PORT);
    connection_queue = create_queue(THREAD_POOL_SIZE); // Inicializar la cola

    // Mantener el programa en ejecución, no es necesario manejar la finalización aquí por ahora
    while (1) {
        client_fd = srv_accept_client(server_fd);

        if (client_fd < 0) {
            log_event("Error al aceptar la conexión");
            continue; // Continúa esperando nuevas conexiones
        }

        enqueue(connection_queue, client_fd); // Encolar el file descriptor
    }

    close(server_fd);
    destroy_queue(connection_queue); // Liberar memoria si es necesario
    return 0;
}