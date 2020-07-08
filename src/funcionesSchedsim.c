#include "../include/funcionesSchedgen.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

typedef struct{
    int proceso;
    int arrTime;
    int burTime;
}procesos;

procesos *procc;

int cont = 0;

void func_help(void){
    printf("schedsim nameFile [tipoAlgoritmo] [quatum] \n"
           "Tipos de algoritmos:\n"
           "\t fcfs: correr con algoritmo First Come First Served \n"
           "\t sjf: correr con algoritmo Shortest-Job-First \n"
           "\t rr: correr con algotimo Round-robin \n");
}

bool validacionArchivo(char *nombre){
    FILE *fd1;
    fd1 = fopen(nombre,"r");
    int pro = 1;
    char buffer[100];
    char temp[1024];
    bool retorno;
    bool ba;
    if(fd1 == NULL){
        perror("File cannot be opened");
        retorno = false;
    }
    else{
        while(!feof(fd1)){
            fgets(temp, sizeof(temp), fd1);
            cont++;
        }
        retorno = true;
    }
    rewind(fd1);
    procc = (procesos *)malloc(cont*sizeof(procesos));
    if(procc == NULL)
        retorno = false;
    else{
        ba = true;
        while(fgets(buffer,sizeof(buffer), fd1)){
            char *ptr = strtok(buffer, " ");
            procc[pro-1].proceso = pro;
            while(ptr != NULL){
                if(ba == true){
                    procc[pro-1].arrTime = atoi(ptr);
                    ba = false;
                }
                else{
                    procc[pro-1].burTime = atoi(ptr);
                    ba = true;
                }
                ptr = strtok(NULL, " ");
            }
            printf("%d %d %d \n", procc[pro-1].proceso, procc[pro-1].arrTime, procc[pro-1].burTime);
            pro += 1;
        }
    }
    fclose(fd1);
    return retorno;
}

void func_fcfs(char *nombre){
    printf("\n %d \n", cont);
    printf("%d %d %d \n", procc[cont-1].proceso, procc[cont-1].arrTime, procc[cont-1].burTime);
            
    /*FILE *fd1;
    char buffer[100];
    int pro = 1;
    int idx = 0;
    bool ba;
    int procesos[50], bt[50], at[50];
    fd1 = fopen(nombre,"r"); 
    if(fd1 == NULL){
        perror("File cannot be opened");
    }
    else{
        ba = true;
        while(fgets(buffer,sizeof(buffer), fd1)){
            char *ptr = strtok(buffer, " ");
            procesos[pro-1] = pro;
            while(ptr != NULL){
                if(ba == true){
                    at[idx] = atoi(ptr);
                    ba = false;
                }
                else{
                    bt[idx] = atoi(ptr);
                    ba = true;
                }
                ptr = strtok(NULL, " ");
            }
            pro += 1;
            idx += 1;
        }
        for(int i=0; i<5; i++){
            printf("%d %d %d \n", procesos[i], at[i], bt[i]);
        }
    }
    fclose(fd1);
    */
}