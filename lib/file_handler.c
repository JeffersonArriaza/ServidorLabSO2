#include "file_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "mime_types.h"
#include "log.h"

char* get_file_contents(const char* url, int* http_code, const char** mime_type, size_t* content_size) {
    char filepath[1024];
    FILE *file;
    struct stat st;

    // Cargar index.html si no se proporciona una URL específica.
    if (strcmp(url, "/") == 0) {
        url = "/index.html";
    }

    snprintf(filepath, sizeof(filepath), "%s%s", HTML_ROOT, url);

    // Verificar si el archivo existe.
    if (stat(filepath, &st) != 0 || (file = fopen(filepath, "rb")) == NULL) {
        log_error("Archivo no encontrado");
        *http_code = 404;
        return NULL;
    }

    // Obtener el tipo MIME del archivo.
    *mime_type = get_mime_type(url);

    // Si el archivo es de tipo binario (como imagen)
    if (strncmp(*mime_type, "image/", 6) == 0) {
        fclose(file);  // Cerrar el archivo antes de abrirlo en binario.
        long file_size;
        unsigned char* binary_content = read_binary_file(filepath, &file_size);

        if (!binary_content) {
            *http_code = 500;  // Error interno del servidor
            return NULL;
        }

        *content_size = (size_t)file_size;
        *http_code = 200;
        return (char*)binary_content;  // Convertir a char* para retorno
    }

    // Si el archivo es de texto, leerlo en el buffer.
    char buffer[1024];
    size_t bytesRead;
    size_t totalSize = 0;
    char *content = NULL;

    // Lectura del archivo en fragmentos
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        char *newContent = realloc(content, totalSize + bytesRead + 1);
        if (newContent == NULL) {
            free(content);
            fclose(file);
            *http_code = 500;  // Error interno del servidor
            return NULL;
        }
        content = newContent;
        memcpy(content + totalSize, buffer, bytesRead);
        totalSize += bytesRead;
    }

    if (content) {
        content[totalSize] = '\0';  // Agregar el terminador nulo
    }

    fclose(file);
    log_event("OK");
    *content_size = totalSize;  // Asignar el tamaño del contenido al puntero
    *http_code = 200;
    return content;
}

// Función para leer archivos en binario
unsigned char* read_binary_file(const char *path, long *file_size) {
    FILE *file = fopen(path, "rb");
    if (!file) {
        log_error("Error al abrir el archivo en modo binario");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    *file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    unsigned char *buffer = malloc(*file_size);
    if (!buffer) {
        log_error("Error al asignar memoria para el contenido del archivo");
        fclose(file);
        return NULL;
    }

    fread(buffer, 1, *file_size, file);
    fclose(file);
    return buffer; // Retornar el buffer de bytes
}

