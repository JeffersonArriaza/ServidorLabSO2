#ifndef HTTP_STATUS_CODES_H
#define HTTP_STATUS_CODES_H

//Estructura para almacenar el codigo y la descripcion de un codigo HTTP
typedef struct {
    int code;
    const char *description;
} HttpStatus;

extern const HttpStatus HTTP_STATUSES[];

//Funcion para obtener la descripción de un codigo HTTP
const char* get_http_status_description(int code);

#endif
