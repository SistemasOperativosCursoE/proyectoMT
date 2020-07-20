## Proyecto MidTerm

MidTerm es el proyecto del primer parcial de la materia de sistemas operativos, el objetivo es ejecutar 3
algoritmos de planificación de procesos, tales como: FCFS, SJF y RR.

## Estructura

El proyecto MidTerm está estructurado de la siguiente manera:

>> include: "Incluye las caberas y funciones protipos para su implementación en los archivos de funciones en src.

>> src: "Incluye el código fuente para schedsim, schedgen, schedstats y los archivos funciones para cada programa sched__"

## Compilación

Para crear los archivos ejecutables schedsim, schedgen, schedstats realizar lo siguiente:

Construir Makefile
```bash
make
```

Eliminar archivos que fueron construidos con MAkefile:
```bash
make clean
```
## Compilación
Muestra una ayuda de como se deberia ejecutar los programas de simulacion, generador de procesos y estadísticas de planificación:
```bash
./schedsim -h
./schedgen -h
./schedstats -h
```

Ejecutar el simulador del procesos en base a un algoritmo:
```bash
./schedsim schedtimes.dat [fcfs|sjf|rr] [quatum]
```

Ejecutar el generador de procesos:
N-> es el numero de procesos que se desean generar.
pa-> probabilidad para generar un valor para arrive time en un proceso.
pb-> probabilidad para generar un valor para burst time en un proceso.
nameFile-> es el nombre del archivo con que se va a crear para guardar la generacion de los procesos.
```bash
./schedgen N pa pb nameFile.dat
```