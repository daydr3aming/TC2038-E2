#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <string>
#include <vector>
#include <queue>

// compilacion: g++ -std=c++11 main.cpp -o main
// ejecucion: ./main.exe o ./main.out
// Tiene que existir un archivo input.txt en la misma carpeta que el main.exe al correr

using namespace std;

// E2. Actividad Integradora 2
// Situacion Problema: Ayudar a una empresa a incursionarse en los servicios de internet
// Rogelio Guzman Cruzado - A01639914
// David Alejandro González Ibarra - A01633817 

// Algoritmo de Prim para MST. Complejidad: O(V^2)
// La complejidad temporal puede reducirse a O(ElogV) si el grafo a leer se representa con una lista de adyacencia
// Debido a que en una lista de adyacencia importa mas el numero de E que de V
void MST(vector<vector<int>> nbhs){
    int V = nbhs.size();
    vector<int> key(V, INT_MAX);
    vector<int> parent(V, -1);
    vector<bool> inMST(V, false);

    // Q <- 0
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    // DECREASE KEY (Q, u, 0)
    key[0] = 0;

    // INSERT (Q, u)
    pq.push({0, 0});

    // Encontramos los light edges 
    while (!pq.empty()){
        // Extraer minimo key value
        int u = pq.top().second;
        pq.pop();
        inMST[u] = true;
        for (int v = 0; v < V; ++v) {
            if (!inMST[v] && nbhs[u][v] < key[v]) {
                key[v] = nbhs[u][v]; // DECREASE KEY
                parent[v] = u; 
                pq.push({key[v], v}); // DECREASE KEY
            }
        }
    }
    for (int i = 1; i < V; ++i) {
        cout << "(" << parent[i] << "," << i << ")" << endl;
    }
}

// Algoritmo de TSP con DP (Held-Karp). Complejidad: O(2^n * n^2)
// Debido a la naturaleza NP-Hard del problema, no existe un algoritmo que de una solucion exacta o optima en tiempo polinomial
// Por lo que utilizaremos un approach de programacion dinamica para reducir la complejidad factorial de la fuerza bruta a exponencial
// Held Karp da una solucion exacta y optima al problema de TSP pero con una complejidad temporal muy elevada, por lo que nos sirve entradas "pequeñas"
// En este caso esperamos casos de prueba con vecindarios < 25, por lo que Held-Karp es relativamente aceptable
int tsp_dp(vector<vector<int>>& nbhs, int mask, int pos, vector<vector<int>>& dp, vector<vector<int>>& parent, int n) {
    // En los algoritmos de TSP es mas comun guardar los nodos visitados en una mascara de bits
    // Esto para tener la mayor eficiencia en memoria y computacional posible debido a la intensidad de este problema
    // en la mascara de bits el numero en i representa si la ciudad i fue visitada o no dependiendo de si es 1 o 0
    if (mask == (1 << n) - 1) {
        return nbhs[pos][0];  // Regresamos a la ciudad de inicio
    }

    if (dp[mask][pos] != -1) {
        return dp[mask][pos];
    }

    int ans = INT_MAX;
    int next_city = -1;

    for (int next = 0; next < n; next++) {
        if (!(mask & (1 << next))) {
            int current_cost = nbhs[pos][next] + tsp_dp(nbhs, mask | (1 << next), next, dp, parent, n);
            if (current_cost < ans) {
                ans = current_cost;
                next_city = next;
            }
        }
    }
    parent[mask][pos] = next_city;  // Guardamos la ruta
    dp[mask][pos] = ans;
    return ans;
}

void printRoute(vector<vector<int>>& parent, int start, int n) {
    int mask = 1;  // Mascara de bits para el estado inicial
    int pos = start;

    cout << "Con ruta: " << char(start + 'A') << " -> ";

    while (mask != (1 << n) - 1) {
        int next_city = parent[mask][pos];
        cout << char(next_city + 'A') << " -> ";
        mask |= (1 << next_city);
        pos = next_city;
    }

    cout << "A" << endl;
}

// La aplicacion de Edmonds-Karp necesita un BFS para encontrar los caminos mas cortos o augmenting paths
// Complejidad: O(V^2)
bool bfs(vector<vector<int>>& residualGraph, vector<int>& parent, int source, int sink) {
    int vertices = residualGraph.size();
    vector<bool> visited(vertices, false);
    queue<int> q;
    q.push(source);
    visited[source] = true;
    parent[source] = -1;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v = 0; v < vertices; v++) {
            if (!visited[v] && residualGraph[u][v] > 0) {
                q.push(v);
                parent[v] = u;
                visited[v] = true;
                if (v == sink) {
                    return true; // True si se encontro un augmenting path
                }
            }
        }
    }
    return false; // False si no se encontro un augmenting path
}

// Algoritmo de Ford-Fulkerson con aplicacion Edmonds-Karp. Complejidad: O(VE^2)
int MaxFlow(vector<vector<int>>& maxData, int source, int sink){
    int vertices = maxData.size();
    vector<vector<int>> residualGraph = maxData; // Grafo Residual
    vector<int> parent(vertices, -1);
    // Empezamos con flujo 0 para todos las conexiones
    int maxFlow = 0;

    // En cada iteracion aumentamos el valor de flujo encontrando un augmenting path
    while (bfs(residualGraph, parent, source, sink)) {
        int pathFlow = INT_MAX;
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            pathFlow = min(pathFlow, residualGraph[u][v]);
        }

        // Actualizamos las capacidades residuales del grafo
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            residualGraph[u][v] -= pathFlow;
            residualGraph[v][u] += pathFlow;
        }

        // Flujo resultante 
        maxFlow += pathFlow;
    }

    return maxFlow;
}

void Geom(){
    // Algoritmo de Graham Scan
}

int main() {

    // Numero de vecindarios
    int n;
    ifstream file("input.txt");
    file >> n;

    // Matriz de vecindarios donde cada conexion representa la distancia en km entre dos vecindarios
    vector<vector<int>> nbhs(n, vector<int>(n));
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            file >> nbhs[i][j];
        }
    }

    // Matriz de vecindarios donde cada conexion representa el flujo maximo de fibra entre dos vecindarios
    vector<vector<int>> maxData(n, vector<int>(n));
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            file >> maxData[i][j];
        }
    }

    // Pares para lo de geometria con las librerias de c++
    vector<pair<int, int>> pairs;
    string line;

    while (getline(file, line)) {
        // Se tiene que respetar a la perfeccion el formato "(x,y)"
        if (line.front() == '(' && line.back() == ')') {
            line = line.substr(1, line.size() - 2); // Removemos parentesis
            int x, y;
            stringstream ss(line);
            char comma; // Descartamos la coma
            ss >> x >> comma >> y;
            pairs.push_back(make_pair(x, y));
        }
    }


    // MST Prim Salida 1
    cout << "Conectar todas las colonias con fibra: " << endl;
    MST(nbhs);
    cout << endl;   

    vector<vector<int>> dp(1 << n, vector<int>(n, -1));
    vector<vector<int>> parent(1 << n, vector<int>(n, -1));

    // DP TSP Salida 2
    int min_path = tsp_dp(nbhs, 1, 0, dp, parent, n);  // La mask representa las ciudades visitadas, implementacion con mascaras de bits

    cout << "Kilometros minimos para visitar todas las colonias y volver al origen: " << endl;
    cout << min_path << endl;

    int start = 0;  // Assuming the start vertex is 0 (A)
    printRoute(parent, start, n);
    cout << endl;

    // Flujo Maximo Salida 3
    int source = 0;
    int sink = 3;

    int maxFlow = MaxFlow(maxData, source, sink);
    cout << "Flujo maximo de " << source << " a " << sink << endl;
    cout << maxFlow << endl;

    return 0;
}
