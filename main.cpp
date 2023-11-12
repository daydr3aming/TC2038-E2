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
// Rogelio Guzman Cruzado - A01639914
// David Alejandro González Ibarra - A01633817 

// Algoritmo de Prim para MST. Complejidad: O(V^2)
// Crea un solo arbol en vez de crear un bosque como Kruskal
void MST(vector<vector<int>> nbhs){
    int V = nbhs.size();
    vector<int> key(V, INT_MAX);
    vector<int> parent(V, -1);
    vector<bool> inMST(V, false);

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    key[0] = 0;

    pq.push({0, 0});

    while (!pq.empty()){
        int u = pq.top().second;
        pq.pop();
        inMST[u] = true;

        // Exploramos todo lo adjacente a 
        for (int v = 0; v < V; ++v) {
            if (!inMST[v] && nbhs[u][v] < key[v]) {
                key[v] = nbhs[u][v];
                parent[v] = u;
                pq.push({key[v], v});
            }
        }
    }
    for (int i = 1; i < V; ++i) {
        cout << "(" << parent[i] << "," << i << ")" << " KM: " <<nbhs[i][parent[i]] << endl;
    }
}

// Algoritmo de TSP con DP (Held Karp). Complejidad: O(2^n * n^2)
// Debido a la naturaleza NP-Hard del problema, no existe un algoritmo que lo resuelva en tiempo polinomial
// Por lo que utilizaremos un approach de programacion dinamica para reducir la complejidad factorial de la fuerza bruta a exponencial
// Held Karp da una solucion exacta y optima al problema de TSP pero en complejidad temporal muy elevada, por lo que sirve para entradas pequeñas
// En este caso esperamos entradas y grafos pequeños por lo que consideramos que es una buena opcion
int tsp_dp(vector<vector<int>>& nbhs, int mask, int pos, vector<vector<int>>& dp, vector<vector<int>>& parent, int n) {
    if (mask == (1 << n) - 1) {
        return nbhs[pos][0];  // Return to the starting city
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

    parent[mask][pos] = next_city;  // Record the next city for reconstruction
    dp[mask][pos] = ans;
    return ans;
}

void printRoute(vector<vector<int>>& parent, int start, int n) {
    int mask = 1;  // Start with the mask for the initial state
    int pos = start;

    cout << "Ruta tomada: " << char(start + 'A') << " -> ";

    while (mask != (1 << n) - 1) {
        int next_city = parent[mask][pos];
        cout << char(next_city + 'A') << " -> ";
        mask |= (1 << next_city);
        pos = next_city;
    }

    cout << "A" << endl;
}

// La aplicacion de Edmonds-Karp necesita un BFS para encontrar los caminos mas cortos o augmenting paths
// Complejidad: O(V^2) por que usamos una matriz de adyacencia
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

    // Inicializamos n
    int n;
    ifstream file("input.txt");
    file >> n;

    // Esta matriz para TSP y MST
    vector<vector<int>> nbhs(n, vector<int>(n));
    for(int i = 0; i < n; i++){
        cout << endl;
        for(int j = 0; j < n; j++){
            file >> nbhs[i][j];
        }
    }

    // Esta matriz para el algoritmo del flujo maximo
    vector<vector<int>> maxData(n, vector<int>(n));
    for(int i = 0; i < n; i++){
        cout << endl;
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
    cout << "Cableado de todas las colonias (MST): " << endl;
    MST(nbhs);

    vector<vector<int>> dp(1 << n, vector<int>(n, -1));
    vector<vector<int>> parent(1 << n, vector<int>(n, -1));

    // DP TSP Salida 2

    int min_path = tsp_dp(nbhs, 1, 0, dp, parent, n);  // La mask representa las ciudades visitadas, implementacion con mascaras de bits

    cout << "Km minimos para visitar todas las colonias y volver al origen: " << endl;
    cout << min_path << endl;

    int start = 0;  // Assuming the start vertex is 0 (A)
    printRoute(parent, start, n);

    // Fin Salida 2

    // Flujo Maximo Salida 3
    int source = 0;
    int sink = 3;

    int maxFlow = MaxFlow(maxData, source, sink);
    cout << "Flujo maximo de: " << source << " a " << sink << endl;
    cout << maxFlow << endl;

    return 0;
}
