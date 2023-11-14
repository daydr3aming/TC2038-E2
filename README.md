# E2.-Actividad-Integradora-2
Escribe en C++ un programa que ayude a una empresa que quiere incursionar en los servicios de Internet respondiendo a la situación problema 2.



# Pasos para compilar/usar la librería 
Creamos una nueva carpeta para nuestros programas,
en este caso test.

Ahi colocamos el archivo main.cpp

En el la terminal, estando dentro del directorio de test ponemos los siguientes comandos:

path/to/cgal/scripts/cgal_create_CMakeLists -s your_program (sin terminación cpp)

(en path/to/cgal es el directorio en donde está CGAL-5.6 cuando se descargue 
el repositorio)

Después ponemos en la terminal: cmake -DCMAKE_BUILD_TYPE=Release .

Finalmente: make




## TODO:
* ~~Codigo de lectura de archivos, el archivo es una matriz de adyacencia de grafos ponderados. (cada peso es la distancia en KM entre colonias)~~

* ~~1. Forma de cablear las colonias con fibra (lista de arcos de la forma (A,B)) ~~Algoritmo desconocido por ahora~~ *Edit: SI ES EL MST con Kruskal o Prim*~~
  
* ~~Implementar TSP para solucionar punto 2, la ruta mas corta para que alguien visiste todas las colonias una vez y al final regrese a la colonia origen.~~

* ~~Leer otra matriz de n x n que represente la maxima transmision de datos entre i y j~~

* ~~Usamos el algoritmo de flujo *Edit: Es Ford-Fulkerson (buscar en diapositivas) para poder imprimir esto~~

* ~~Leemos los pares con el formato (x,y) y los guardamos~~

* Instalar librerias de geometria computacional y sacar lista de poligonos utilizando Graham Scan, Voronoi Diagram, etc.

#### Con esto terminamos.

