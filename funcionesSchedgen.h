#ifndef estructura_schedgen
#define estructura_schedgen
#include <stdbool.h> 
#define MAXSTR 50

void func_help(void);
bool generar_procesos(int nProceso, float pa, float pb, char *nombre);
#endif