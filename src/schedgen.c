#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
 #include "../include/funcionesSchedgen.h"
#include <string.h>

int main (int argc, char *argv[]){
	char *nombre_archivo;
    float pa, pb;
    int nProceso;

	//muestra el menu help si se usa el comando -h solamente
	// if( argc>2 && strcmp(argv[1],"-h") == 0){
	// 	func_help();
	// }
	
	//infica el uso del comando cuando hay parametros insuficientes
	 if (argc != 5 ) {
        fprintf(stderr, "Uso: %s COMANDO ARCHIVO  ...\n", argv[0]);
        // func_help();
    }
    //si el archivo es valido permite utilizar los comandos 
	if(argc == 5){
        nProceso = atoi(argv[1]);
        pa = atof(argv[2]);
        pb = atof(argv[3]);
        nombre_archivo = argv[4];
        if((pa >= 0 && pa<=1) && (pb >= 0 && pb<=1)){
            bool retorno = generar_procesos(nProceso, pa, pb, nombre_archivo);
            printf(retorno ? "Archivo generado con exito\n" : "Error al generar el archivo\n");
        }
        else{
            printf("Error");
        }
	}
	return 0;
}