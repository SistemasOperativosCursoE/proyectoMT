#include "./funcionesSchedgen.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h> 
#include <fcntl.h>
#include <unistd.h>

// guia para pedir ayuda al progrma de como ejecutar en schedgen
void func_help(void){
    printf("schedgen N pa pb nameFile\n"
            "N-> Numero de procesos\n"
            "pa-> Probabilidad para generar el Arrival Time\n"
            "pb-> Probabilidad para generar el Burst Time\n"
            "nameFile-> Nombre del archivo a generar con extencion .dat\n"
            "Ejemplo: \n"
           "\t ./schedgen 100 0.5 0.4 schedtimes2.dat \n");
}

/*para generar el archivo en base a parametro
    N numero de procesos a crearse
    pa probabilidad para un arrival time
    pb probabilidad para un burst time
    nombre es el nombre del fichero a crearse*/
bool generar_procesos(int nProceso, float pa1, float pb1, char *nombre){
    bool flag1 = true, flag2 = true, retorno = false;
    int ta = 0, tb = 1, contador = 0;
    float pa = pa1, pb = pb1;
    FILE *fd;
    char at[100], bt[100];
    char linea[100];
    //Sirve para genenerar numero aleaatorios
    srand((unsigned) time(NULL));
    // se crea el fichero del fichero
    fd = fopen(nombre, "w");
    while(flag1){
        // se genera la probabilidad para rrival time
        float a1 = (float) rand() / RAND_MAX;
        if(a1 < pa){
            while(flag2){
                // probabilida para burst time
                float a2 = (float) rand() / RAND_MAX;
                if(a2 < pb){
                    contador++;
                    // casting para variables
                    sprintf(at, "%d", ta);
                    sprintf(bt, "%d", tb);
                    strcat(linea, at);
                    //separador por espacio
                    strcat(linea, " ");
                    strcat(linea, bt);
                    // en caso de que sea el ultimo valor se da un salto de linea
                    if(contador<nProceso)
                        strcat(linea, "\n");
                    fputs(linea, fd);
                    //reiniciar variables
                    at[0] = '\0';
                    bt[0] = '\0';
                    linea[0] = '\0';
                    //bandera para salir del ciclo
                    flag2 = false;
                }
                //aumentar el valor para burst time
                tb++;
            }
            tb = 1;
            flag2 = true;
        }
        //aumentar valor para arrival time
        ta++;
        //bandera para salir del proceso
        if(contador == nProceso){
            flag1 = false;
            retorno = true;
        }
    }
    //cerrar el fichero creado
    fclose(fd);
    return retorno; // retorno true si se creo el archivo, caso contario false
}