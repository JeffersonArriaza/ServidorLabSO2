# Regla para generar el ejecutable
servidor: server_functions.o main.o log.o
	gcc server_functions.o main.o log.o -o servidor

# Regla para compilar el archivo objeto log.o
log.o: lib/log.c
	gcc -c lib/log.c -o log.o

# Regla para compilar el archivo objeto server_functions.o
server_functions.o: lib/server_functions.c
	gcc -c lib/server_functions.c -o server_functions.o

# Regla para compilar el archivo objeto main.o
main.o: main.c
	gcc -c main.c -o main.o

