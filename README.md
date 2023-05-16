<h1 align="center">Laboratorio 3 - Redes y Sistemas Distribuidos 2023</h1>


*Capa de Transporte* :whale:


**INTEGRANTES:**


* Alejo Corral: alejo.corral@mi.unc.edu.ar
* Guillermo De Ipola: guillermo.ipola@mi.unc.edu.ar
* Ignacio Martinez Goloboff: ignacio.martinez.goloboff@mi.unc.edu.ar

## Abstract
En este laboratiorio llevamos a cabo un estudio para evaluar el control de flujo y congestión en redes usando el simulador por eventos discretos OMNET++. Estudiamos dos casos concretos en el que se presentan distintas problemas que impactan en la carga útil de la red y luego proponemos un protocolo para mejorar el rendimiento en estos casos.


## Introducción

Vamos a usar el siguiente modelo:

![Estructura](/images/estructura_part1.png)

Nuestra red consta de un `generador` (nodeTx) y un `receptor` (nodeRx), el cual están conectados através de un nodo intermedio `queue`. 

### NodeTx 
Contiene dos componentes, un generador y una queue con un buffer interno. Este se encarga de generar paquetes en un cierto tiempo predefinido e irlos mandando a través de la queue. Los paquetes serán almacenados en un buffer y cuando esté lleno los mismos serán descartados.

### NodeRx
Contiene también una queue pero con un receptor. Su función es la de ir recibiendo paquetes por la queue e ir "consumiendolos". Los paquetes serán almacenados en un buffer y cuando esté lleno los mismos serán descartados.

### Queue (Nx)
Es la encargada de ir recibiendo los paquetes de un nodo e ir enviandoselos al otro a medida que se le es posible. 
Contiene un buffer interno en el cual va a ir guardando los paquetes y cuando el mismo se llene, los entrantes serán descartados.


Veremos luego que será necesario modificar nuestro modelo levemente para dotar al receptor de la capacidad de dar feedback al emisor, agregando una cola(`queue2`) en la dirección opuesta a `queue1`

![Estructura2](/images/estructura_part2.png)

De ahora en más nos referiremos únicamente a este último modelo modificado.

En este trabajo vamos a estudiar dos casos sobre el mismo modelo, en los que varían las tasas de transferencia entre nodos. 
- En el primero, el receptor no puede manejar la cantidad de datos que se le envían a tiempo y por ende termina descartando paquetes. 
- En el segundo, es la red la que no puede manejar la cantidad de paquetes que se le envían, por lo que surge congestión.

## Caso 1


| 0.1 | 0.2 | 0.3 |
|----------|----------|----------|
| ![a](/codigo1/graficas/buffers_caso1_0.1.png)    | ![a](/codigo1/graficas/buffers_caso1_0.2.png)   | ![a](/codigo1/graficas/buffers_caso1_0.3.png)   |
| ![a](/codigo1/graficas/env_rcv_caso1_0.1.png)    | ![a](/codigo1/graficas/env_rcv_caso1_0.2.png)   | ![a](/codigo1/graficas/env_rcv_caso1_0.3.png)   |
| ![a](/codigo1/graficas/retraso_caso1_0.1.png)    | ![a](/codigo1/graficas/retraso_caso1_0.2.png)   | ![a](/codigo1/graficas/retraso_caso1_0.3.png)   |

**Nx es el buffer del nodo queue1**

Se puede ver en la figura de ocupación de buffers del caso 0.1 que hay un problema de saturación en el nodo `queue1` y que a partir de este se reduce la carga útil. En los otros casos, no es tan aparente este problema ya que la red tiene capacidad suficiente para aprovechar los paquetes enviados.
### Carga Util vs Ofrecida

![Carga Util](/codigo1/graficas/carga.png)

### Aprovechamiento vs Intervalos de generación

![Carga Util](/codigo1/graficas/aprovechamiento.png)

## Caso 2

| 0.1 | 0.2 | 0.3 |
|----------|----------|----------|
| ![a](/codigo1/graficas/buffer_caso2_0.1.png)    | ![a](/codigo1/graficas/buffer_caso2_0.2.png)   | ![a](/codigo1/graficas/buffer_caso2_0.3.png)   |
| ![a](/codigo1/graficas/env_rcv_caso1_0.1.png)    | ![a](/codigo1/graficas/env_rcv_caso1_0.2.png)   | ![a](/codigo1/graficas/env_rcv_caso1_0.3.png)   |
| ![a](/codigo1/graficas/retraso_caso1_0.1.png)    | ![a](/codigo1/graficas/retraso_caso1_0.2.png)   | ![a](/codigo1/graficas/retraso_caso1_0.3.png)   |

Al igual que en el caso 1, tenemos un problema que se hace más aparente cuando el intervalo de generación es de 0.1s. Podemos ver en la gráfica de uso de buffers que el buffer de `queue1` se llena y empieza a descartar paquetes.

El resto de las gráficas es muy similar(si no idéntico) debido a que ambos casos producen los mismos efectos sobre el rendimiento.

## Método propuesto

Proponemos un algoritmo simple para mitigar el efecto de los cuellos de botella en ambos casos: que el receptor envíe feedback para que el emisor pueda decidir si enviar o no un paquete más. 

La idea del protocolo es asumir que existe una capacidad inicial del recepto, enviar esa cantidad de paquetes y luego casa vez que el receptor consuma paqutes  se envia un mensage avisando que hay lugar en el buffer receptor para un paquete mas.

Una potencial ventaja de enviar paquetes de feedback tan simples es que se reduce la probabilidad de que se corrompan los mensajes.

## Resultados

Al repetir las pruebas usando el algoritmo propuesto, obtuvimos los siguientes resultados:

## Caso 1


| 0.1 | 0.2 | 0.3 |
|----------|----------|----------|
| ![a](/codigo2/graficas/buffers_caso1_0.1.png)    | ![a](/codigo2/graficas/buffers_caso1_0.2.png)   | ![a](/codigo2/graficas/buffers_caso1_0.3.png)   |
| ![a](/codigo2/graficas/env_rcv_caso1_0.1.png)    | ![a](/codigo2/graficas/env_rcv_caso1_0.2.png)   | ![a](/codigo2/graficas/env_rcv_caso1_0.3.png)   |
| ![a](/codigo2/graficas/retraso_caso1_0.1.png)    | ![a](/codigo2/graficas/retraso_caso1_0.2.png)   | ![a](/codigo2/graficas/retraso_caso1_0.3.png)   |

Se puede ver que en ningún caso perdimos paquetes(nunca se llenó ningún buffer). En cuanto a performance de la red, obtuvimos una velocidad de transferencia similar. Sin embargo, al hacer mejor uso del buffer del emisor, cargamos la red de forma más eficiente. Esto se puede observar en la figura de aprovechamiento.

### Aprovechamiento vs Intervalos de generación

![Carga Util](/codigo2/graficas/aprovechamiento1.png)

El aprovechamiento mejoró ampliamente; del 50% al 98% en el caso de mayor exigencia a la red(intervalo de 0.1). En los otros casos la mejora no es aparente.


## Caso 2

| 0.1 | 0.2 | 0.3 |
|----------|----------|----------|
| ![a](/codigo2/graficas/buffers_caso2_0.1.png)    | ![a](/codigo2/graficas/buffers_caso2_0.2.png)   | ![a](/codigo2/graficas/buffers_caso2_0.3.png)   |
| ![a](/codigo2/graficas/env_rcv_caso1_0.1.png)    | ![a](/codigo2/graficas/env_rcv_caso1_0.2.png)   | ![a](/codigo2/graficas/env_rcv_caso1_0.3.png)   |
| ![a](/codigo2/graficas/retraso_caso1_0.1.png)    | ![a](/codigo2/graficas/retraso_caso1_0.2.png)   | ![a](/codigo2/graficas/retraso_caso1_0.3.png)   |

En este caso tampoco perdimos paquetes, aunque los resultados no fueron tan favorecedores como en el caso 1. Es decir, nuestro algoritmo funcionó mejor contra problemas de control de flujo que para control de congestión. Esto podemos verlo más claramente en la siguiente figura.

### Aprovechamiento vs Intervalos de generación

![Carga Util](/codigo2/graficas/aprovechamiento2.png)

El aprovechamiento mejoró, pero no en la misma medida que en el caso 1.


## Discusión

### Overhead
Nuestro algoritmo añade overhead en forma de espera desde el lado del transmisor, esto se ve reflejado en los gráficos de delay. En los casos de intervalos de transmisión 0.2 y 0.3 vemos que nuestro algoritmo solo sirvió para disminuir la velocidad de transferencia. 

### Dealock

Si llegaramos a perder paquetes de feedback, nuestro emisor no enviaría más paquetes y la transmisión nunca terminaría. Quizá podríamos mejorar esta situación con el uso de termporizadores. 

### Optimización del feedback
Quizá podríamos enviar menos paquetes de feedback que tengan más información sobre la capacidad actual del receptor.

## Referencias

  - Documentación de Omnet++
  - [Videos en el aula virtual de la materia: Redes y Sistemas Distribuídos 2023](https://famaf-nva.aulavirtual.unc.edu.ar/course/view.php?id=20)
  - Andrew S. Tanenbaum Redes de computadoras. 5ta edición
  - Kurose, J. F. and Ross, K. W. Computer Networking - A Top Down Approach. 7th Edition, Pearson, 2017


