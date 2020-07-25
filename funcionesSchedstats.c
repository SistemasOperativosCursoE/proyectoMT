#include <stdio.h>
#include "./funcionesSchedstats.h"

// guia para pedir ayuda al progrma de como ejecutar en schedstats
void func_help_Stats(void){
    printf("schedstats nameFile \n"
           "nameFile-> es el nombre del archivo a leer con extension .dat \n"
           "Ejemplo: \n"
           "\t ./schedstats schedtimes.dat\n");
}