#include "http_status_codes.h"

//Lista de codigos HTTP y sus descripciones
const HttpStatus HTTP_STATUSES[] = {
    {200, "OK"},
    {404, "Not Found"},
    {408, "Request Timeout"},
    {405, "Method Not Allowed"},
    {414, "URI Too Long"},
    {500, "Internal Server Error"},
    {505, "HTTP Version Not Supported"}
};

//Funcion que devuelve la descripcion a partir del codigo.
const char* get_http_status_description(int code) {
    for (int i = 0; i < sizeof(HTTP_STATUSES) / sizeof(HttpStatus); i++) {
        if (HTTP_STATUSES[i].code == code) {
            return HTTP_STATUSES[i].description;
        }
    }
    return "Unknown Code";
}
