#include "../include/funcionesSchedgen.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h> 
#include <fcntl.h>
#include <unistd.h>



void func_help(void){
    printf("schedsim nameFile [tipoAlgoritmo] [quatum] \n"
           "Tipos de algoritmos:\n"
           "\t fcfs: correr con algoritmo First Come First Served \n"
           "\t sjf: correr con algoritmo Shortest-Job-First \n"
           "\t rr: correr con algotimo Round-robin \n");
}

bool generar_procesos(int nProceso, float pa1, float pb1, char *nombre){
    bool flag1 = true, flag2 = true, retorno = false;
    int ta = 0, tb = 1, contador = 0;
    float pa = pa1, pb = pb1;
    FILE *fd;
    char at[100], bt[100], linea[50];
    srand((unsigned) time(NULL));
    fd = fopen(nombre, "w");
    while(flag1){
        float a1 = (float) rand() / RAND_MAX;
        if(a1 < pa){
            while(flag2){
                float a2 = (float) rand() / RAND_MAX;
                if(a2 < pb){
                    contador++;
                    sprintf(at, "%d", ta);
                    sprintf(bt, "%d", tb);
                    strcat(linea, at);
                    strcat(linea, " ");
                    strcat(linea, bt);
                    if(contador<nProceso)
                        strcat(linea, "\n");
                    fprintf(fd, linea);
                    at[0] = '\0';
                    bt[0] = '\0';
                    linea[0] = '\0';
                    flag2 = false;
                }
                tb++;
            }
            flag2 = true;
        }
        ta++;
        if(contador == nProceso){
            flag1 = false;
            retorno = true;
        }
    }
    fclose(fd);
    return retorno;
}