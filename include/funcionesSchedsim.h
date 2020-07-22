#ifndef estructura_schedsim
#define estructura_schedsim
#include <stdbool.h> 
#define MAXSTR 50

void func_help(void);
bool validacionArchivo(char *nombre);
void func_fcfs(char modo);
void ordenar_procesos();
void func_sjf(char modo);
void func_rr(char modo, int qt);
void func_max_min(void);
void generar_promedio_rafaga(char algoritmo);
void clonarProcesos();
#endif