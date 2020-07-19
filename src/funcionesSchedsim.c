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
    temp = (temporales *)malloc(cont*sizeof(temporales));
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
    for(int y=0; y<cont; y++){
        temp[y].proceso = procc[y].proceso;
        temp[y].arrTime = procc[y].arrTime;
        temp[y].burTime = procc[y].burTime;
        temp[y].respTime = procc[y].respTime;
    }
}

void func_fcfs(char *nombre){
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
}

void func_rr(char *nombre, int qt){
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
            temp[i].termTime = time;
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
    promWT = sumWT / cont;
    promTT = sumTT / cont;
    promNTT = sumNTT / cont;
    printf("\nAverage turnaround time = %0.2f \nAverage normalized turnaround time = %0.2f \nAverage waiting time = %0.2f \n",
        promTT, promNTT, promWT);
}

void func_sjf(char *nombre){
    /*int sumBurst = 0, sumWT=0, sumTT=0;
    float promTT, promNTT, promWT, sumNTT=0;
    int s,remain=0,time;
    ordenar_procesos();
    p[cont].rt= 999;
    for(time=0; remain!=n; time++)
    {
        s=cont;
        for(int i=0; i<n; i++)
            if(procc[i].arrTime<=time && procc[i].respTime < p[s].rt && p[i].rt>0)
                s=i;
        p[s].rt--;
        if(p[s].rt == 0)
        {
            remain++;
            p[s].ct = time + 1;
            p[s].tat = p[s].ct - p[s].at;
            avgtat += p[s].tat;
            p[s].wt = p[s].tat - p[s].bt;
            avgwt += p[s].wt;
            printf("P%d\t\t%d\t%d\t%d\t%d\t%d\n",p[s].no,p[s].at,p[s].bt,p[s].ct,p[s].tat,p[s].wt);
        }
    }*/
}