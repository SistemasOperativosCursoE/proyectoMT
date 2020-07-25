#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "./funcionesSchedsim.h"
#include "./funcionesSchedstats.h"
#include <string.h>

int main (int argc, char *argv[]){
	bool arValido;
	char *nombre_archivo;

	//muestra el menu help si se usa el comando -h solamente
	if( argc != 2 || strcmp(argv[1],"-h") == 0){
		fprintf(stderr, "Uso: %s COMANDO ARCHIVO  ...\n", argv[0]);
        func_help_Stats();
	}

	if(argc == 2 && strcmp(argv[1],"-h") != 0){
        nombre_archivo = argv[1];
        arValido =  validacionArchivo(nombre_archivo);
        //fputs(arValido ? "true" : "false", stdout);
    }
    //si el archivo es valido permite utilizar los comandos 
	if(arValido == true){
        func_fcfs(nombre_archivo, 'o');
        func_sjf(nombre_archivo, 'o');
        func_rr(nombre_archivo, 'o',1);
        func_rr(nombre_archivo, 'o',4);
	}
	return 0;
}