# E2.-Actividad-Integradora-2
Escribe en C++ un programa que ayude a una empresa que quiere incursionar en los servicios de Internet respondiendo a la situación problema 2.

El programa debe:

1. leer un archivo de entrada que contiene la información de un grafo representado en forma de una matriz de adyacencias con grafos ponderados
El peso de cada arista es la distancia en kilómetros entre colonia y colonia, por donde es factible meter cableado.

El programa debe desplegar cuál es la forma óptima de cablear con fibra óptica conectando colonias de tal forma que se pueda compartir información entre cuales quiera dos colonias.

2. Debido a que las ciudades apenas están entrando al mundo tecnológico, se requiere que alguien visite cada colonia para ir a dejar estados de cuenta físicos, publicidad, avisos y notificaciones impresos. por eso se quiere saber ¿cuál es la ruta más corta posible que visita cada colonia exactamente una vez y al finalizar regresa a la colonia origen?
El programa debe desplegar la ruta a considerar, tomando en cuenta que la primera ciudad se le llamará A, a la segunda B, y así sucesivamente

3. El programa también debe leer otra matriz cuadrada de N x N datos que representen la capacidad máxima de transmisión de datos entre la colonia i y la colonia j. Como estamos trabajando con ciudades con una gran cantidad de campos electromagnéticos, que pueden generar interferencia, ya se hicieron estimaciones que están reflejadas en esta matriz.

La empresa quiere conocer el flujo máximo de información del nodo inicial al nodo final. Esto debe desplegarse también en la salida estándar.

Por último

4. Teniendo en cuenta la ubicación geográfica de varias "centrales" a las que se pueden conectar nuevas casas, la empresa quiere contar con una forma de decidir, dada una nueva contratación del servicio, cuál es la central más cercana geográficamente a esa nueva contratación. No necesariamente hay una central por cada colonia. Se pueden tener colonias sin central, y colonias con más de una central.

Entrada:
Un numero entero N que representa el número de colonias en la ciudad
matriz cuadrada de N x N que representa el grafo con las distancias en kilómetros entre las colonias de la ciudad
matriz cuadrada de N x N que representa las capacidades máximas de flujo de datos entre colonia i y colonia j
lista de N pares ordenados de la forma (A,B) que representan la ubicación en un plano coordenado de las centrales

Salida:
1. Forma de cablear las colonias con fibra
(lista de arcos de la forma (A,B))
2. ruta a seguir por el personal que reparte correspondencia, considerando inicio y fin en al misma colonia.
3. valor de flujo máximo de información del nodo inicial al nodo final
4. lista de polígonos (cada elemento es una lista de puntos de la forma (x,y))


Ejemplo de entrada:
4

 0 16 45 32
16  0 18 21
45 18  0  7
32 21  7  0

 0 48  12  18
52  0 42 32
18 46  0 56
24 36 52  0

(200,500)
(300,100)
(450,150)
(520,480)
