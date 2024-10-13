#include "file_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "mime_types.h"
#include "log.h"

char* get_file_contents(const char* url, int* http_code, const char** mime_type) {
    char filepath[1024];
    FILE *file;
    struct stat st;

    // Si no se proporciona una URL, cargará el index.html.
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

    // Obtener el tipo MIME del archivo
    *mime_type = get_mime_type(url);

    // Lectura del archivo en buffer.
    char buffer[1024];
    size_t bytesRead;
    size_t totalSize = 0;
    char *content = NULL;

    // Lectura del archivo en fragmentos
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        char *newContent = realloc(content, totalSize + bytesRead + 1);
        if (newContent == NULL) {
            free(content);  // Liberar el contenido anterior en caso de error
            fclose(file);
            *http_code = 500;  // Error interno del servidor
            return NULL;
        }
        content = newContent;
        memcpy(content + totalSize, buffer, bytesRead);
        totalSize += bytesRead;
    }

    if (content) {
        content[totalSize] = '\0';  // Cadena final
        
    }

    fclose(file);
    log_event("OK");
    *http_code = 200;
    return content;
}

