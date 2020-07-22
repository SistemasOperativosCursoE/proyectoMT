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

typedef struct{
    int id;
    int cont;
    int waiTime;
    int turnATime;
    float normTurnATime;
}rafagas;
rafagas *burst;

int max = 0, min = 0;
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

void func_fcfs(char modo){
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
        if(modo == 'd'){
            printf("%d: runs %d-%d -> end = %d, (arr = %d), turn = %d, (burst = %d), wait = %d\n",
                procc[x].proceso, tmp, sumBurst, sumBurst, procc[x].arrTime, procc[x].turnATime, procc[x].burTime, procc[x].waiTime);
        }
    }
    promWT = (float) sumWT / cont;
    promTT = (float) sumTT / cont;
    promNTT = sumNTT / (float) cont;
    if(modo == 'd'){
        printf("\nAverage turnaround time = %f \nAverage normalized turnaround time = %f \nAverage waiting time = %f \n",
            promTT, promNTT, promWT);
    }
    func_max_min();
    generar_promedio_rafaga('f');
    free(burst);
    free(procc);
    free(temp);
}

void func_rr(char modo, int qt){
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
            //if(modo == 'd'){
                printf("%d: runs %d-%d -> end = %d, (arr = %d), turn = %d, (burst = %d), wait = %d\n",
                    procc[i].proceso, time - quatum, time, time, procc[i].arrTime, procc[i].turnATime, procc[i].burTime, procc[i].waiTime);
            //}
            sumWT += procc[i].waiTime;
            sumTT += procc[i].turnATime;
            sumNTT += procc[i].normTurnATime;
            t=0;
        }
        else{
            if(vAnterior != time){
                //if(modo == 'd'){
                    printf("%d: runs %d-%d \n",
                        procc[i].proceso, time - quatum, time);
                //}
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
    //if(modo == 'd'){
        printf("\nAverage turnaround time = %0.2f \nAverage normalized turnaround time = %0.2f \nAverage waiting time = %0.2f \n",
            promTT, promNTT, promWT);
    //}
    func_max_min();
    generar_promedio_rafaga('r');
    free(burst);
    free(procc);
    free(temp);
}

void func_sjf(char modo){
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
            if(modo == 'd'){
                printf("%d: runs %d-%d -> end = %d, (arr = %d), turn = %d, (burst = %d), wait = %d\n",
                    temp[s].proceso, time-contador, time+1, time+1, temp[s].arrTime, temp[s].turnATime, temp[s].burTime, temp[s].waiTime);
            }
            sumWT += temp[s].waiTime;
            sumTT += temp[s].turnATime;
            sumNTT += temp[s].normTurnATime;
            contador = 0;
        }
        else{
            if(modo == 'd'){
                printf("%d: runs %d-%d \n",
                    temp[s].proceso, time, time+1);
            }
            contador++;
        }
    }
    promWT = (float) sumWT / cont;
    promTT = (float) sumTT / cont;
    promNTT = sumNTT / (float) cont;
    if(modo == 'd'){
        printf("\nAverage turnaround time = %0.2f \nAverage normalized turnaround time = %0.2f \nAverage waiting time = %0.2f \n",
            promTT, promNTT, promWT);
    }
    func_max_min();
    generar_promedio_rafaga('s');
    free(burst);            
    free(procc);
    free(temp);
}

void func_max_min(void){
    int maximo, minimo;
    maximo = procc[0].burTime;
    minimo = procc[0].burTime;
    for(int i=1; i<cont; i++){
        if(procc[i].burTime > maximo)
            maximo = procc[i].burTime;
        if(procc[i].burTime < minimo)
            minimo = procc[i].burTime;
    }
    max = maximo;
    min = minimo;
    burst = (rafagas *)malloc( max *sizeof(rafagas));
    for(int x = 1; x <= max; x++){
        burst[x-1].id = x;
        burst[x-1].cont = 0;
        burst[x-1].waiTime = 0;
        burst[x-1].turnATime = 0;
        burst[x-1].normTurnATime = 0.0;
    }
}

void generar_promedio_rafaga(char algoritmo){
    FILE *fdt, *fdn, *fdw;
    // float promTurn, promNTurn, promWait;
    // char promTT[100], promNTT[100], promWT[100], bur[100];
    char lineaTurn[1024], lineaNTurn[1024], lineaWait[1024], lineaBurst[1024];
    fdt = fopen("schedturns.dat", "a");
    fdn = fopen("schednturns.dat", "a");
    fdw = fopen("schedwaits.dat", "a");
    if(algoritmo == 'f' || algoritmo == 'r'){
        for(int x = 0; x <= max; x++){
            for(int w = 0; w < cont; w++){
                if(burst[x].id == procc[w].burTime){
                    burst[x].cont += 1;
                    burst[x].waiTime = procc[w].waiTime;
                    burst[x].turnATime = procc[w].turnATime;
                    burst[x].normTurnATime = procc[w].normTurnATime;
                }
            }
        }
    }
    else if(algoritmo == 's'){
        for(int x = 0; x <= max; x++){
            for(int w = 0; w < cont; w++){
                if(burst[x].id == temp[w].burTime){
                    burst[x].cont += 1;
                    burst[x].waiTime = temp[w].waiTime;
                    burst[x].turnATime = temp[w].turnATime;
                    burst[x].normTurnATime = temp[w].normTurnATime;
                }
            }
        }
    }
    for(int x = 0; x < max; x++){
        float promTurn, promNTurn, promWait;
        char promTT[100], promNTT[100], promWT[100], bur[100];
        if(burst[x].cont > 0){
            promTurn = (float)burst[x].turnATime / (float)burst[x].cont;
            promNTurn = burst[x].normTurnATime / (float)burst[x].cont;
            promWait = (float)burst[x].waiTime / (float)burst[x].cont;
            sprintf(bur, "%d", burst[x].id);
            sprintf(promTT, "%0.2f", promTurn);
            sprintf(promNTT, "%0.2f", promNTurn);
            sprintf(promWT, "%0.2f", promWait);
            strcat(lineaBurst, bur);
            strcat(lineaTurn, promTT);
            strcat(lineaNTurn, promNTT);
            strcat(lineaWait, promWT);
            if(burst[x].id != max){
                strcat(lineaBurst, " ");
                strcat(lineaNTurn, " ");
                strcat(lineaTurn, " ");
                strcat(lineaWait, " ");
            }
            else if(burst[x].id == max){
                strcat(lineaBurst, "\n");
                strcat(lineaTurn, "\n");
                strcat(lineaNTurn, "\n");
                strcat(lineaWait, "\n");
            }
            // bur[0] = '\0';
            // promTT[0] = '\0';
            // promNTT[0] = '\0';
            // promWT[0] = '\0';
        }
    }
    if(algoritmo == 'f'){
        fputs(lineaBurst, fdt);
        fputs(lineaBurst, fdn);
        fputs(lineaBurst, fdw);
    }
    fputs(lineaTurn, fdt);
    fputs(lineaNTurn, fdn);
    fputs(lineaWait, fdw);
    for(int x = 0; x < max; x++){
        printf("%d %d %d %0.2f %d \n", 
            burst[x].id, burst[x].waiTime, burst[x].turnATime, burst[x].normTurnATime, burst[x].cont);
    }
    fclose(fdt);
    fclose(fdn);
    fclose(fdw);
}