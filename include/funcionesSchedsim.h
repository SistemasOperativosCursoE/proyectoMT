#ifndef estructura_schedsim
#define estructura_schedsim
#include <stdbool.h> 
#define MAXSTR 50

void func_help(void);
bool validacionArchivo(char *nombre);
void func_fcfs(void);
void ordenar_procesos();
void func_sjf(void);
void func_rr(int qt);
void clonarProcesos();
#endif