#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "queue.h"
#include "server_handle_cli.h"
#include "log.h" 

Queue* create_queue(int thread_count) {
    Queue *queue = (Queue *)malloc(sizeof(Queue));
    if (!queue) return NULL; // Manejar error de malloc

    queue->front = NULL;
    queue->rear = NULL;
    queue->thread_count = thread_count;

    // Inicializa el mutex y la variable de condición
    if (pthread_mutex_init(&queue->lock, NULL) != 0) return NULL; // Manejar error
    if (pthread_cond_init(&queue->cond, NULL) != 0) return NULL; // Manejar error

    // Crea el pool de hilos
    queue->thread_pool = malloc(thread_count * sizeof(pthread_t));
    if (!queue->thread_pool) return NULL; // Manejar error
    for (int i = 0; i < thread_count; i++) {
        pthread_create(&queue->thread_pool[i], NULL, handle_multithread, (void*)queue);
    }

    return queue;
}

void destroy_queue(Queue* queue) {
    for (int i = 0; i < queue->thread_count; i++) {
        pthread_cancel(queue->thread_pool[i]);
        pthread_join(queue->thread_pool[i], NULL); // Espera a que los hilos terminen
    }
    free(queue->thread_pool);
    free(queue);
}

// Enqueue y Dequeue (modificados para usar mutex y condiciones)
void enqueue(Queue* queue, int connfd) {
    Node* new_node = (Node *)malloc(sizeof(Node));
    if (!new_node) {
        log_event("Error al asignar memoria para el nuevo nodo."); // Manejar error
        return; // Salir si no se puede asignar memoria
    }
    new_node->connfd = connfd;
    new_node->next = NULL;

    pthread_mutex_lock(&queue->lock);
    if (queue->rear) {
        queue->rear->next = new_node;
    } else {
        queue->front = new_node; // Si la cola estaba vacía
    }
    queue->rear = new_node;
    pthread_cond_signal(&queue->cond); // Señala que hay un nuevo elemento
    pthread_mutex_unlock(&queue->lock);
}

int dequeue(Queue* queue) {
    pthread_mutex_lock(&queue->lock);
    while (!queue->front) { // Espera hasta que haya un elemento
        pthread_cond_wait(&queue->cond, &queue->lock);
    }

    Node* temp = queue->front;
    int connfd = temp->connfd;
    queue->front = queue->front->next;
    if (!queue->front) {
        queue->rear = NULL; // La cola se ha quedado vacía
    }
    free(temp);
    pthread_mutex_unlock(&queue->lock);
    return connfd;
}

void* handle_multithread(void *arg) {
    Queue *queue = (Queue *)arg;
    while (1) {
        int connfd = dequeue(queue);
        if (connfd >= 0) {
            srv_handle_client(connfd); // Maneja la conexión
            close(connfd); // Cierra el file descriptor
        }
    }
}


