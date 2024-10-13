#include "log.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

void log_event(const char *message) {
    FILE *file;
    time_t now;
    char timestamp[40];
    char filename[70]; 
    struct stat st = {0};

    //Verificar si el directorio "logs" existe, si no, crearlo
    if (stat("logs", &st) == -1) {
        if (mkdir("logs", 0700) != 0) {
            perror("Error al crear el directorio de logs");
            return;
        }
    }

    time(&now);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d", localtime(&now));

    //Imprime el evento en stdout
    printf("[%s] Evento: %s\n", timestamp, message);

    //Genera el nombre del archivo
    snprintf(filename, sizeof(filename), "logs/%s.log", timestamp);

    //Abre el archivo en modo append (a)
    file = fopen(filename, "a");
    if (file == NULL) {
        perror("Error al abrir el archivo de log de eventos");
        return;
    }

    //Escribe el evento en el archivo
    fprintf(file, "[%s] Evento: %s\n", timestamp, message);
    fclose(file);
}

void log_error(const char *message) {
    FILE *file;
    time_t now;
    char timestamp[40];
    char filename[70];  
    struct stat st = {0};

    //Verificar si el directorio "logs" existe, si no, crearlo
    if (stat("logs", &st) == -1) {
        if (mkdir("logs", 0700) != 0) {
            perror("Error al crear el directorio de logs");
            return;
        }
    }

    time(&now);
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d", localtime(&now));

    // Imprime el error en stderr
    fprintf(stderr, "[%s] Error: %s\n", timestamp, message);

    // Genera el nombre del archivo
    snprintf(filename, sizeof(filename), "logs/%s-errors.log", timestamp);

    //Abre el archivo en modo append (a)
    file = fopen(filename, "a");
    if (file == NULL) {
        perror("Error al abrir el archivo de log de errores");
        return;
    }

    //Escribe el error en el archivo
    fprintf(file, "[%s] Error: %s\n", timestamp, message);
    fclose(file);
}