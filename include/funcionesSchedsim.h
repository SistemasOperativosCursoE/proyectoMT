#ifndef estructura_schedgen
#define estructura_schedgen
#include <stdbool.h> 
#define MAXSTR 50

void func_help(void);
bool validacionArchivo(char *nombre);
void func_fcfs(char *nombre);
void ordenar_procesos();
void func_sjf(char *nombre);
void func_rr(char *nombre, int qt);
void clonarProcesos();
/*int func_rr(char *nombre);*/
#endif