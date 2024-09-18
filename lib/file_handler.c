#include "file_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

char* get_file_contents(const char* url, int* http_code) {
    char filepath[1024];
    FILE *file;
    struct stat st;

    //Si no se proporciona una URL, cargara el index.html.
    if (strcmp(url, "/") == 0) {
        url = "/index.html";
    }

    snprintf(filepath, sizeof(filepath), "%s%s", HTML_ROOT, url);

    //Verificar si el archivo existe.
    if (stat(filepath, &st) != 0 || (file = fopen(filepath, "rb")) == NULL) {
        *http_code = 404;
        return NULL;
    }

    //Lectura del archivo en bufer.
    char buffer[1024];
    size_t bytesRead;
    size_t totalSize = 0;
    char *content = NULL;

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        content = realloc(content, totalSize + bytesRead + 1);
        memcpy(content + totalSize, buffer, bytesRead);
        totalSize += bytesRead;
    }

    if (content) {
        content[totalSize] = '\0';  //Cadena final
    }

    fclose(file);
    *http_code = 200;
    return content;
}
