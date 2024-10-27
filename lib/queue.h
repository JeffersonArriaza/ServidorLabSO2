#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>

#define QUEUE_SIZE 50 // Tamaño máximo de la cola

typedef struct Node {
    int connfd; 
    struct Node *next;
} Node;

typedef struct Queue {
    Node *front;          // Puntero Frontal de la cola
    Node *rear;           // Puntero Final de la cola
    pthread_mutex_t lock; // Mutex para la sincronización
    pthread_cond_t cond;  // Variable de condición
    pthread_t *thread_pool; 
    int thread_count;     // Contador de hilos
} Queue;

Queue* create_queue(int thread_count);
void destroy_queue(Queue* queue);
void enqueue(Queue* queue, int connfd);
int dequeue(Queue* queue);
void* handle_multithread(void *arg); // Prototipo de la función de manejo de hilos

#endif

