#include "./funcionesSchedsim.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX 9999
//La estructura procesos permitira guardar los procesos que estan el archivo
//respTime, waiTime, turnTime, normTurnATime se actualizara en base al algoritmo
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
//La estructura temporales permitira hacer guardar una copia de la estructura procesos
//Servira como alternativa a las colas
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
// Esta estructura permitira guardar los burst time unicos cuando se ejecute schedstats
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
// guia para pedir ayuda al progrma de como ejecutar en schedsim
void func_help(void){
    printf("schedsim nameFile [tipoAlgoritmo] [q] \n"
           "nameFile-> es el nombre del archivo a leer con extension .dat \n"
           "Tipos de algoritmos:\n"
           "\t fcfs: correr con algoritmo First Come First Served \n"
           "\t sjf: correr con algoritmo Shortest-Job-First \n"
           "\t rr: correr con algotimo Round-robin \n"
           "\t q: es el quatum con el que se va a correr Round Robin \n");
}
/*Esta funcion permite verificar si existe o no el archivo, de modo que si no existe no se ejecutara
ningun simulador*/
bool validacionArchivo(char *nombre){
    FILE *fd1;
    fd1 = fopen(nombre,"r");
    bool retorno;
    if(fd1 == NULL){
        perror("File cannot be opened");
        retorno = false;
    }
    else{
        retorno = true;
    }
    fclose(fd1);
    return retorno;
}
/*Aqui se cargaran los proceos del archivo schedtime.dat a la estructura proceso*/
void cargarProcesos(char *nombre){
    cont = 0;
    FILE *fd1;
    fd1 = fopen(nombre,"r");
    int pro = 1;
    char buffer[100], temp[1024];
    bool ba;
    if(fd1 == NULL){
        perror("File cannot be opened");
    }
    else{
        while(!feof(fd1)){//lee linea a linea el archivo
            fgets(temp, sizeof(temp), fd1);
            cont++;
        }
    }
    rewind(fd1);
    procc = (procesos *) malloc(cont*sizeof(procesos)); //permite reservar memoria de manera dinamica en base al numero de procesos que hay en e archivo
    if(procc != NULL){
        ba = true;
        while(fgets(buffer,sizeof(buffer), fd1)){
            char *ptr = strtok(buffer, " "); // se hace split del archivo
            procc[pro-1].proceso = pro;
            while(ptr != NULL){
                if(ba == true){
                    procc[pro-1].arrTime = atoi(ptr); //se accede al arrival time de linea y se lo guarda en la estructura
                    ba = false;
                }
                else{
                    procc[pro-1].burTime = atoi(ptr); // se accede al burst time de la linea y se lo guarda en la estructura
                    procc[pro-1].respTime = atoi(ptr);
                    ba = true; //bandera para salir del ciclo
                }
                ptr = strtok(NULL, " ");
            }
            pro += 1;
        }
    }
    fclose(fd1);
}

/*permite ordenar los procesos en caso de que no esten ordenados en el archivo,
es usado por seguridad porque el archivo si estan generados*/
void ordenar_procesos(){
    int i, j;
    //Se reserva memoria dinamica para para a estructura temporales
    temp = (temporales *) malloc((cont+1)*sizeof(temporales)); 
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
/*Se hace una copia de los procesos que estan en la estructura procc y se guarda en temproales*/
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
/*Simulacion del algoritmo first come first served los aprametros recibidos son:
        *nombre: es el nombre del archivo
        *modo: si se ejecuta en modo detalla u oculto dependiendo si se ejecuta schedsim o schedstats*/ 
void func_fcfs(char *nombre, char modo){
    int sumWT=0, sumTT=0;
    float promTT, promNTT, promWT, sumNTT=0;
    if(modo == 'o')
        printf("procesando fcfs...\n");
    cargarProcesos(nombre);
    ordenar_procesos();
    bool flagS = true;
    int a = 0, inicio = 0, end = 0, contP = 1;
    while(flagS){
        for(int y = 0; y < cont; y++){
            if(a == procc[y].arrTime){
                if(contP == cont)
                    flagS = false; // cuando se haya leido todos los procesos se sale por medio de la bandera
                if(end <= a){
                    inicio =  a; //permite guardar de donde se ejecuta el proceso
                    end = a + procc[y].burTime; // donde se finaliza el proceso
                }
                else if( end > a){
                    inicio = end;
                    end = end + procc[y].burTime;
                }
                procc[y].turnATime = end - procc[y].arrTime; 
                procc[y].waiTime = procc[y].turnATime - procc[y].burTime; //guarda el waiting time por proceso
                procc[y].normTurnATime = (float) procc[y].turnATime / (float) procc[y].burTime; //calcula el normalizado del proceso
                sumWT += procc[y].waiTime; //va haciendo la acumulacion de los waiting
                sumTT += procc[y].turnATime; //va haciendo la acumulacion de los turnaround time
                sumNTT += procc[y].normTurnATime; //va haciendo la acumulacion de los normalizados
                if(modo == 'd'){ //se ejecutara el detalle del proceso si es schedsim
                    printf("%d: runs %d-%d -> end = %d, (arr = %d), turn = %d, (burst = %d), wait = %d\n",
                        procc[y].proceso, inicio, end, end, procc[y].arrTime, procc[y].turnATime, procc[y].burTime, procc[y].waiTime);
                }
                contP++;
            }
        }
        a++;
    }
    promWT = (float) sumWT / cont;
    promTT = (float) sumTT / cont;
    promNTT = sumNTT / (float) cont;
    if(modo == 'd'){
        printf("\nAverage turnaround time = %f \nAverage normalized turnaround time = %f \nAverage waiting time = %f \n",
            promTT, promNTT, promWT);
    }
    /*Cuando se ejecuta el programa schedstats se ejecutara en modo oculto*/
    if(modo == 'o'){
        func_max_min(); // obtendra el maximo y minimo de la lista de procesos
        generar_promedio_rafaga('f'); //genera los archivos
    }
    /*liberar la memoria cargada con malloc*/
    free(burst);
    burst = NULL;
    free(procc);
    procc = NULL;
    free(temp);
    temp = NULL;
}
/*Simulacion del algoritmo Round Robin los aprametros recibidos son:
        *nombre: es el nombre del archivo
        *modo: si se ejecuta en modo detalla u oculto dependiendo si se ejecuta schedsim o schedstats
        * qt es el quatum*/ 
void func_rr(char *nombre, char modo, int qt){
    int i, sumWT=0, sumTT=0, t=0, vAnterior = -1;
    float promTT, promNTT, promWT, sumNTT=0;
    int quatum = qt;
    if(modo == 'o')
        printf("procesando rr = %d...\n", qt);
    cargarProcesos(nombre);
    ordenar_procesos();
    clonarProcesos();
    int time, totalProcess;
    totalProcess = cont;
    for(time=0,i=0;tempo!=0;){ // se ejecutara de manera infinita hasta que tempo sea sero que es cuando se han calculado todos los procesos
        if(procc[i].respTime<=quatum && procc[i].respTime>0){
            time+= procc[i].respTime;
            procc[i].respTime=0;
            t=1;
        }
        else if(procc[i].respTime>0){
            procc[i].respTime-=quatum; // se va restando los quatum
            time+=quatum; // se va aumentando el tiempo total de los burst
        }
        if(procc[i].respTime==0 && t==1){
            tempo-=1;
            procc[i].turnATime = time-procc[i].arrTime;  // calcula el turnaround en base al tiempo calculado menos el tiempo que arrivo el proceso
            procc[i].waiTime = time - procc[i].arrTime - procc[i].burTime;
            procc[i].normTurnATime = (float) procc[i].turnATime / (float) procc[i].burTime;
            if(modo == 'd'){
                printf("%d: runs %d-%d -> end = %d, (arr = %d), turn = %d, (burst = %d), wait = %d\n",
                    procc[i].proceso, time - quatum, time, time, procc[i].arrTime, procc[i].turnATime, procc[i].burTime, procc[i].waiTime);
            }
            sumWT += procc[i].waiTime;
            sumTT += procc[i].turnATime;
            sumNTT += procc[i].normTurnATime;
            t=0;
        }
        else{
            if(vAnterior != time){ // bandera para ejecutar si es que el proceso no ha finalizado
                if(modo == 'd'){
                    printf("%d: runs %d-%d \n",
                        procc[i].proceso, time - quatum, time);
                }
                vAnterior = time;
            }
        }
        if(i==totalProcess-1)
            i=0;
        else if(procc[i+1].arrTime<=time)
            i++; // permitira pasar de un proceso a otro
        else if(procc[i+1].arrTime > time)
            i++; // permitira pasar de un proceso a otro
        else
            i=0;
    }
    
    promWT = (float) sumWT / cont;
    promTT = (float) sumTT / cont;
    promNTT = sumNTT / (float) cont;
    if(modo == 'd'){
        printf("\nAverage turnaround time = %0.2f \nAverage normalized turnaround time = %0.2f \nAverage waiting time = %0.2f \n",
            promTT, promNTT, promWT);
    }
    if(modo == 'o'){
        func_max_min(); // obtendra el maximo y minimo de la lista de procesos
        generar_promedio_rafaga('r'); //genera los archivos
    }

    // se libera la memoria de las estructuras  cargada con malloc
    free(burst);
    burst = NULL;
    free(procc);
    procc = NULL;
    free(temp);
    temp = NULL;
}

/*Simulacion del algoritmo Shortest Job First los aprametros recibidos son:
        *nombre: es el nombre del archivo
        *modo: si se ejecuta en modo detalla u oculto dependiendo si se ejecuta schedsim o schedstats*/ 
void func_sjf(char *nombre, char modo){
    if(modo == 'o')
        printf("procesando sjf...\n");
    int s,remain=0,time;
    int sumWT=0, sumTT=0;
    float promTT, promNTT, promWT, sumNTT=0;
    cargarProcesos(nombre);
    ordenar_procesos();
    clonarProcesos();
    temp[cont].respTime = MAX; // se pone un valor por defecto para hacer los calculos del arrival time
    temp[cont].proceso = 0; // se inicializa un temporal
    int contador = 0;
    int end = 0;
    for(time=0;remain!=cont;time++){ 
        s=cont;
        for(int i=0;i<cont;i++)
            if(temp[i].arrTime<=time&&temp[i].respTime<temp[s].respTime&&temp[i].respTime>0) // se valida cual proceso se va a ejecutar
                s=i;
        temp[s].respTime--;
        if(temp[s].respTime == 0){
            remain++; // es el tiempo remanente que se va acumulando para el tiempo final del proceso
            temp[s].termTime=time+1;
            temp[s].turnATime=temp[s].termTime-temp[s].arrTime; //calculo del turnaround time
            temp[s].waiTime=temp[s].turnATime-temp[s].burTime;
            temp[s].normTurnATime = (float) temp[s].turnATime / (float) temp[s].burTime;
            if(modo == 'd'){ // se calcula el modo detallado para poder imrpimir
                if(temp[s].termTime-contador <= temp[s].arrTime || temp[s].proceso == 1){
                    end = time+1;
                    printf("%d: runs %d-%d -> end = %d, (arr = %d), turn = %d, (burst = %d), wait = %d\n",
                    temp[s].proceso, temp[s].arrTime, end, end, temp[s].arrTime, temp[s].turnATime, temp[s].burTime, temp[s].waiTime);
                }
                else{
                    end = time+1;
                    printf("%d: runs %d-%d -> end = %d, (arr = %d), turn = %d, (burst = %d), wait = %d\n",
                        temp[s].proceso, temp[s].termTime-contador, end, end, temp[s].arrTime, temp[s].turnATime, temp[s].burTime, temp[s].waiTime);
                }
            }
            /*se hace la acumulacion de los
                turnaround time
                waiting time
                normalized turnaround time*/
            sumWT += temp[s].waiTime;
            sumTT += temp[s].turnATime;
            sumNTT += temp[s].normTurnATime;
            contador = 0;
        }
        if(time < temp[s].turnATime && temp[s].turnATime - time -1 >=0 && temp[s-1].termTime < temp[s+1].arrTime && end>=temp[s+1].arrTime){
            if(modo == 'd'){
                printf("%d: runs %d-%d \n",
                    temp[s+1].proceso, end, end+1);
            }
        }
        contador++;
    }
    promWT = (float) sumWT / cont;
    promTT = (float) sumTT / cont;
    promNTT = sumNTT / (float) cont;
    if(modo == 'd'){
        printf("\nAverage turnaround time = %0.2f \nAverage normalized turnaround time = %0.2f \nAverage waiting time = %0.2f \n",
            promTT, promNTT, promWT);
    }
    //modo oculto
    if(modo == 'o'){
        func_max_min();
        generar_promedio_rafaga('s');
    }
    //liberar la memoria de las estructuras
    free(burst);
    burst = NULL;
    free(procc);
    procc = NULL;
    free(temp);
    temp = NULL;
}
/* esta funcion calcular el maximimo y minimo para gaurdar en una estrctura de burst time*/
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
    burst = (rafagas *) malloc(max *sizeof(rafagas)); // se reserva memoria para gaurdar los brust time unicos
    for(int x = 1; x <= max; x++){
        burst[x-1].id = x;
        burst[x-1].cont = 0;
        burst[x-1].waiTime = 0;
        burst[x-1].turnATime = 0;
        burst[x-1].normTurnATime = 0.0;
    }
}

/*generar promedios y archivos de rafagas para schedstats
    parmaretors: algortimo cual se va a ejecutar*/
void generar_promedio_rafaga(char algoritmo){
    FILE *fdt, *fdn, *fdw;
    float promTurn = 0.0, promNTurn = 0.0, promWait = 0.0;
    char promTT[100]= "", promNTT[100]= "", promWT[100]= "", bur[100]= "";
    char lineaTurn[1024]= "", lineaNTurn[1024]= "", lineaWait[1024]= "", lineaBurst[1024]= "";
    // se cren los directores de archivo y en modo a de apend
    fdt = fopen("schedturns.dat", "a");
    fdn = fopen("schednturns.dat", "a");
    fdw = fopen("schedwaits.dat", "a");
    // para ejecutar los achivos en base a los algortimosfcfs y rr
    if(algoritmo == 'f' || algoritmo == 'r'){
        for(int x = 0; x <= max; x++){
            for(int w = 0; w < cont; w++){
                if(burst[x].id == procc[w].burTime){
                    burst[x].cont += 1;
                    burst[x].waiTime += procc[w].waiTime;
                    burst[x].turnATime += procc[w].turnATime;
                    burst[x].normTurnATime += procc[w].normTurnATime;
                }
            }
        }
    }
    //para ejecutar el archivo  en base a sjf
    else if(algoritmo == 's'){
        for(int x = 0; x <= max; x++){
            for(int w = 0; w < cont; w++){
                if(burst[x].id == temp[w].burTime){
                    burst[x].cont += 1;
                    burst[x].waiTime += temp[w].waiTime;
                    burst[x].turnATime += temp[w].turnATime;
                    burst[x].normTurnATime += temp[w].normTurnATime;
                }
            }
        }
    }
    // se hace los promedios unicos de los burst time y se va guardando
    for(int x = 0; x < max; x++){   
        if(burst[x].cont > 0){
            promTurn = (float)burst[x].turnATime / (float)burst[x].cont;
            promNTurn = burst[x].normTurnATime / (float)burst[x].cont;
            promWait = (float)burst[x].waiTime / (float)burst[x].cont;
            // se hace casting de float a char
            sprintf(bur, "%d", burst[x].id);
            sprintf(promTT, "%0.2f", promTurn);
            sprintf(promNTT, "%0.2f", promNTurn);
            sprintf(promWT, "%0.2f", promWait);
            // se va concatenado linea
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
            bur[0] = '\0';
            promTT[0] = '\0';
            promNTT[0] = '\0';
            promWT[0] = '\0';
        }
    }
    if(algoritmo == 'f'){
        fputs(lineaBurst, fdt);
        fputs(lineaBurst, fdn);
        fputs(lineaBurst, fdw);
    }
    // se guarda las linea en cada archivo
    fputs(lineaTurn, fdt);
    fputs(lineaNTurn, fdn);
    fputs(lineaWait, fdw);
    // se reinicializa las variables
    lineaTurn[0] = '\0';
    lineaNTurn[0] = '\0';
    lineaWait[0] = '\0';
    lineaBurst[0] = '\0';
    fclose(fdt);
    fclose(fdn);
    fclose(fdw);
}