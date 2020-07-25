import matplotlib.pyplot as plt

archivos = ['schednturns.dat','schedturns.dat','schedwaits.dat']
lablX = 'Burst Time'
labelsY = ['Normalized Turnaround Time', 'Turnaround Time', 'Waiting Time']
savePlot = ['schednturns.png','schedturns.png','schedwaits.png']
for j in range(len(archivos)):
    archivo = open(archivos[j], "r")
    fcfs = sjf = rr1 = rr4 = []
    for i, linea in enumerate(archivo.readlines()):
        lista = linea[:-1].split(" ")
        if(i == 1):
            fcfs = sorted(list(map(float, lista)))
        elif (i == 2):
            sjf = sorted(list(map(float, lista)))
        elif (i == 3):
            rr1 = sorted(list(map(float, lista)))
        elif (i == 4):
            rr4 = sorted(list(map(float, lista)))
    archivo.close()
    fcfs = sorted(list(map(float, fcfs)))
    plt.plot(fcfs, label='FCFS')
    plt.plot(sjf, label='SJF')
    plt.plot(rr1, label='RR1')
    plt.plot(rr4, label='RR4')
    plt.xlabel("Burst Time")
    plt.ylabel(labelsY[j])
    plt.title(archivos[j])
    plt.legend()
    plt.savefig(savePlot[j])
    plt.show()
