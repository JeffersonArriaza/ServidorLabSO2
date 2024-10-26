#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

// Define el directorio raíz de los archivos
#define HTML_ROOT "html"  
#include <stddef.h>
#include <stdio.h>

// Declaración de la función que obtiene el contenido de un archivo
char* get_file_contents(const char* url, int* http_code, const char** mime_type, size_t* content_size);

// Declaración de la función que lee un archivo binario
unsigned char* read_binary_file(const char *path, long *file_size);

#endif // FILE_HANDLER_H
