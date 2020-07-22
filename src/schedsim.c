#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "../include/funcionesSchedsim.h"
#include <string.h>

int main (int argc, char *argv[]){
	bool arValido;
	char *nombre_archivo;
    char *algoritmo;

	//muestra el menu help si se usa el comando -h solamente
	if( argc>2 && strcmp(argv[1],"-h") == 0){
		func_help();
	}
	
	//infica el uso del comando cuando hay parametros insuficientes
	 if (argc < 3 || argc > 4) {
        fprintf(stderr, "Uso: %s COMANDO ARCHIVO  ...\n", argv[0]);
        func_help();
    }

	if(argc == 3 || argc == 4){
        nombre_archivo = argv[1];
        arValido =  validacionArchivo(nombre_archivo);
        //fputs(arValido ? "true" : "false", stdout);
    }
    //si el archivo es valido permite utilizar los comandos 
	if(arValido == true){
        algoritmo = argv[2];
        if(strcmp(algoritmo, "fcfs") == 0){
            printf("Algoritmo fcfs \n");
            func_fcfs('d');
        }
        else if(strcmp(algoritmo, "sjf") == 0){
            printf("Algoritmo  sjf \n");
            func_sjf('d');
        }
        else if(strcmp(algoritmo, "rr") == 0){
            printf("Algoritmo rr \n");
            func_rr('d',atoi(argv[3]));
        }
        else
            printf("Error \n");
	}
	return 0;
}