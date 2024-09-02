#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <time.h>

#define LOG_DIR "logs"

//Funciones para registrar eventos y errores
void log_event(const char *message);
void log_error(const char *message);

#endif