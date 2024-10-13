#Dependencias y objetos
dependencias = lib/server_functions.c main.c lib/log.c lib/server_handle_cli.c lib/http_status_codes.c lib/file_handler.c
objetos = lib/server_functions.o main.o lib/log.o lib/server_handle_cli.o lib/http_status_codes.o lib/file_handler.o

#Regla para generar el ejecutable en base a los objetos
servidor: $(objetos)
	gcc -o $@ $(objetos)

#Regla para crear archivos objeto a partir de sus respectivos archivos .c
lib/%.o: lib/%.c
	gcc -c $< -o $@

#Regla para crear el main.o a partir de su archivo .c
main.o: main.c
	gcc -c $< -o $@

clean:
	rm -f $(objetos) servidor
