## Proyecto MidTerm

MidTerm es el proyecto del primer parcial de la materia de sistemas operativos, el objetivo es ejecutar 3
algoritmos de planificación de procesos, tales como: FCFS, SJF y RR.

## Estructura

El proyecto MidTerm está estructurado de la siguiente manera:

>> extension .h: "Incluye las caberas y funciones protipos para su implementación en los archivos de funciones .c.

>> extension .c: "Incluye el código fuente para schedsim, schedgen, schedstats y los archivos funciones para cada programa ya mencionado"

## Compilación

Para crear los archivos ejecutables schedsim, schedgen, schedstats realizar lo siguiente:

Construir Makefile
```bash
make
```

Eliminar archivos que fueron construidos con Makefile:
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
nameFile-> es el nombre del archivo con que se va a leer para obtener los procesos
fcfs -> Simulador para el algoritmo First Come First Served
sjf -> Simulador para el algoritmo Shortest Job First
rr -> Simulador para el algoritmo Round Robin
q -> es el quatum que se va a ejecutar en el algoritmo Roun Robin
```bash
./schedsim nameFile.dat [fcfs|sjf|rr] [q]
```

Ejecutar el generador de procesos:
N-> es el numero de procesos que se desean generar.
pa-> probabilidad para generar un valor para arrive time en un proceso.
pb-> probabilidad para generar un valor para burst time en un proceso.
nameFile-> es el nombre del archivo con que se va a crear para guardar la generacion de los procesos.
```bash
./schedgen N pa pb nameFile.dat
```

Ejecutar el generador de archivos para graficos:
nameFile-> es el nombre del archivo de donde se van a leer los procesos, puede usarse cualquier extenxion que tenga el archivo de lectura tal como txt.
```bash
./schedstats nameFile.dat
```

## Generar gráficos con el archivo python a partir de los 3 archivos
Con generadorGraficosStats.py se generara automaticamente los archivos png de los graficos.