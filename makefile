# Dependencias
dependencias = lib/server_functions.c main.c

# Regla para generar el ejecutable
servidor: server_functions.o main.o
	gcc server_functions.o main.o -o servidor

# Regla para compilar el archivo objeto server_functions.o
server_functions.o: lib/server_functions.c
	gcc -c lib/server_functions.c -o server_functions.o

# Regla para compilar el archivo objeto main.o
main.o: main.c
	gcc -c main.c -o main.o


