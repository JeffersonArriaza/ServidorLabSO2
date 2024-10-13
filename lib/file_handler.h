#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

// Define el directorio raíz de los archivos
#define HTML_ROOT "html"  

// Declaración de la función que obtiene el contenido de un archivo
char* get_file_contents(const char* url, int* http_code, const char** mime_type);

#endif // FILE_HANDLER_H
