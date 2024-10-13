#ifndef MIME_TYPES_H
#define MIME_TYPES_H

#include <string.h>

// Definición de la estructura para los tipos MIME
typedef struct {
    const char* extension;
    const char* mime_type;
} MimeType;

// Tabla de tipos MIME
static const MimeType MIME_TYPES[] = {
    {".html", "text/html"},
    {".css", "text/css"},
    {".min.css", "text/css"},
    {".js", "application/javascript"},
    {".bundle", "application/javascript"},
    {".min.js", "application/javascript"},
    {".map", "application/json"},
    {".png", "image/png"},
    {".jpg", "image/jpeg"},
    {".jpeg", "image/jpeg"},
    {".gif", "image/gif"},
    {".svg", "image/svg+xml"},
    {".ico", "image/x-icon"},
    {".txt", "text/plain"},
    {".json", "application/json"},
    {".pdf", "application/pdf"},
     {".woff", "font/woff"},
    {".woff2", "font/woff2"},
    {".ttf", "font/ttf"},
    {".otf", "font/otf"},
};

// Función para obtener el tipo MIME basado en la extensión de la URL
const char* get_mime_type(const char *url) {
    const char *dot = strrchr(url, '.'); // Buscar el último punto en la URL
    if (!dot || dot == url) return "application/octet-stream"; // Tipo por defecto si no se encuentra una extensión

    // Comparar la extensión con los tipos MIME
    for (size_t i = 0; i < sizeof(MIME_TYPES) / sizeof(MIME_TYPES[0]); i++) {
        if (strcasecmp(dot, MIME_TYPES[i].extension) == 0) {
            return MIME_TYPES[i].mime_type; // Devuelve el tipo MIME correspondiente
        }
    }
    return "application/octet-stream"; // Tipo por defecto si no coincide
}

#endif // MIME_TYPES_H


