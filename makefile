#Dependencias y objetos
dependencias = lib/server_functions.c main.c lib/log.c lib/server_handle_cli.c
objetos = lib/server_functions.o main.o lib/log.o lib/server_handle_cli.o

#Regla para generar el ejecutable en base a los objetos
servidor: $(objetos)
	gcc -o $@ $(objetos)

#Regla para crear archivos objeto a partir de sus respectivos archivos .c
lib/%.o: lib/%.c
	gcc -c $< -o $@

#Regla para crear el main.o a partir de su archivo .c
main.o: main.c
	gcc -c $< -o $@

