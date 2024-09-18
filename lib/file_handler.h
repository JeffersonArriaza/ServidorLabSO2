#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#define HTML_ROOT "html"  //Directorio raíz de los archivos

//Funcion que obtiene el contenido de un archivo
char* get_file_contents(const char* url, int* http_code);

#endif
