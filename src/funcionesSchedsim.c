#include "../include/funcionesSchedsim.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#define MAX 9999

typedef struct{
    int proceso;
    int arrTime;
    int burTime;
    int respTime;
    int waiTime;
    int turnATime;
    float normTurnATime;
}procesos;
procesos *procc;

typedef struct{
    int proceso;
    int arrTime;
    int burTime;
    int respTime;
    int termTime;
    int waiTime;
    int turnATime;
    float normTurnATime;
}temporales;
temporales *temp;

int cont = 0, flag=0,remaining, tempo;

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
    char buffer[100], temp[1024];
    bool retorno, ba;
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
                    procc[pro-1].respTime = atoi(ptr);
                    ba = true;
                }
                ptr = strtok(NULL, " ");
            }
            pro += 1;
        }
    }
    fclose(fd1);
    return retorno;
}

void ordenar_procesos(){
    int i, j;
    temp = (temporales *)malloc((cont+1)*sizeof(temporales));
    for(i=0; i<cont-1; i++){
        for(j=0; j<cont-i-1; j++) {   
            if( procc[j].arrTime > procc[j+1].arrTime ){
                temp[0].proceso = procc[j].proceso;
                temp[0].arrTime = procc[j].arrTime;
                temp[0].burTime = procc[j].burTime;
                temp[0].respTime = procc[j].respTime;

                procc[j].proceso = procc[j+1].proceso;
                procc[j].arrTime = procc[j+1].arrTime;
                procc[j].burTime = procc[j+1].burTime;
                procc[j].respTime = procc[j+1].respTime;

                procc[j+1].proceso = temp[0].proceso;
                procc[j+1].arrTime = temp[0].arrTime;
                procc[j+1].burTime = temp[0].burTime;
                procc[j+1].respTime = temp[0].respTime;
            }
        } 
    }
    remaining = cont;
    tempo = cont;
}

void clonarProcesos(){
    for(int y=0; y<=cont; y++){
        temp[y].proceso = procc[y].proceso;
        temp[y].arrTime = procc[y].arrTime;
        temp[y].burTime = procc[y].burTime;
        temp[y].respTime = procc[y].respTime;
        if(y==cont){
            temp[y].proceso = -1;
            temp[y].arrTime = -1;
            temp[y].burTime = -1;
            temp[y].respTime = -1;
        }
    }
}

void func_fcfs(void){
    int sumBurst = 0, sumWT=0, sumTT=0, tmp;
    float promTT, promNTT, promWT, sumNTT=0;
    ordenar_procesos();
    for(int x=0;x<cont;x++){
        tmp = sumBurst;
        procc[x].respTime = sumBurst - procc[x].arrTime;
        sumBurst += procc[x].burTime;
        procc[x].turnATime = sumBurst - procc[x].arrTime; 
        procc[x].waiTime = procc[x].turnATime - procc[x].burTime;
        procc[x].normTurnATime = procc[x].turnATime / procc[x].burTime;
        sumWT += procc[x].waiTime;
        sumTT += procc[x].turnATime;
        sumNTT += procc[x].normTurnATime;
        printf("%d: runs %d-%d -> end = %d, (arr = %d), turn = %d, (burst = %d), wait = %d\n",
            procc[x].proceso, tmp, sumBurst, sumBurst, procc[x].arrTime, procc[x].turnATime, procc[x].burTime, procc[x].waiTime);
    }
    promWT = (float) sumWT / cont;
    promTT = (float) sumTT / cont;
    promNTT = sumNTT / (float) cont;
    printf("\nAverage turnaround time = %f \nAverage normalized turnaround time = %f \nAverage waiting time = %f \n",
        promTT, promNTT, promWT);
    free(procc);
    free(temp);
}

void func_rr(int qt){
    int i, sumWT=0, sumTT=0, t=0, vAnterior = -1;
    float promTT, promNTT, promWT, sumNTT=0;
    int quatum = qt;
    ordenar_procesos();
    clonarProcesos();
    int time, totalProcess;
    totalProcess = cont;
    for(time=0,i=0;tempo!=0;){
        if(procc[i].respTime<=quatum && procc[i].respTime>0){
            time+= procc[i].respTime;
            procc[i].respTime=0;
            t=1;
        }
        else if(procc[i].respTime>0){
            procc[i].respTime-=quatum;
            time+=quatum;
        }
        if(procc[i].respTime==0 && t==1){
            tempo--;
            procc[i].turnATime = time-procc[i].arrTime; 
            procc[i].waiTime = time - procc[i].arrTime - procc[i].burTime;
            procc[i].normTurnATime = procc[i].turnATime / procc[i].burTime;
            printf("%d: runs %d-%d -> end = %d, (arr = %d), turn = %d, (burst = %d), wait = %d\n",
                procc[i].proceso, time - quatum, time, time, procc[i].arrTime, procc[i].turnATime, procc[i].burTime, procc[i].waiTime);
            sumWT += procc[i].waiTime;
            sumTT += procc[i].turnATime;
            sumNTT += procc[i].normTurnATime;
            t=0;
        }
        else{
            if(vAnterior != time){
                printf("%d: runs %d-%d \n",
                    procc[i].proceso, time - quatum, time);
                vAnterior = time;
            }
        }
        if(i==totalProcess-1)
            i=0;
        else if(procc[i+1].arrTime<=time)
            i++;
        else
            i=0;
    }
    promWT = (float) sumWT / cont;
    promTT = (float) sumTT / cont;
    promNTT = sumNTT / (float) cont;
    printf("\nAverage turnaround time = %0.2f \nAverage normalized turnaround time = %0.2f \nAverage waiting time = %0.2f \n",
        promTT, promNTT, promWT);
    free(procc);
    free(temp);
}

void func_sjf(void){
    int s,remain=0,time;
    int sumWT=0, sumTT=0;
    float promTT, promNTT, promWT, sumNTT=0;
    ordenar_procesos();
    clonarProcesos();
    temp[cont].respTime = MAX;
    int contador = 0;
    for(time=0;remain!=cont;time++){
        s=cont;
        for(int i=0;i<cont;i++)
            if(temp[i].arrTime<=time&&temp[i].respTime<temp[s].respTime&&temp[i].respTime>0)
                s=i;
        temp[s].respTime--;
        if(temp[s].respTime == 0){
            remain++;
            temp[s].termTime=time+1;
            temp[s].turnATime=temp[s].termTime-temp[s].arrTime;
            temp[s].waiTime=temp[s].turnATime-temp[s].burTime;
            temp[s].normTurnATime = temp[s].turnATime / temp[s].burTime;
            printf("%d: runs %d-%d -> end = %d, (arr = %d), turn = %d, (burst = %d), wait = %d\n",
                temp[s].proceso, time-contador, time+1, time+1, temp[s].arrTime, temp[s].turnATime, temp[s].burTime, temp[s].waiTime);
            sumWT += temp[s].waiTime;
            sumTT += temp[s].turnATime;
            sumNTT += temp[s].normTurnATime;
            contador = 0;
        }
        else{
            printf("%d: runs %d-%d \n",
                temp[s].proceso, time, time+1);
            contador++;
        }
    }
    promWT = (float) sumWT / cont;
    promTT = (float) sumTT / cont;
    promNTT = sumNTT / (float) cont;
    printf("\nAverage turnaround time = %0.2f \nAverage normalized turnaround time = %0.2f \nAverage waiting time = %0.2f \n",
        promTT, promNTT, promWT);
    free(procc);
    free(temp);
}