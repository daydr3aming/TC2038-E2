#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <string>
#include <vector>
#include <queue>

using namespace std;

void MST(vector<vector<int>> &nbhs){
    // Prims Algorithm
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

void TSP(){
    // Algoritmo de Held-Karp
}

void MaxFlow(){
    // Algoritmo de Ford-Fulkerson
}

void Geom(){
    // Algoritmo de Graham Scan
}

int main() {

    // Esto es n 
    int n;
    ifstream file("input.txt");
    file >> n;
    cout << n;

    // Esta matriz para TSP y MST
    vector<vector<int>> nbhs(n, vector<int>(n));
    for(int i = 0; i < n; i++){
        cout << endl;
        for(int j = 0; j < n; j++){
            file >> nbhs[i][j];
            cout << nbhs[i][j] << " ";
        }
    }
    cout << endl;

    // Esta matriz para el algoritmo del flujo maximo
    vector<vector<int>> maxData(n, vector<int>(n));
    for(int i = 0; i < n; i++){
        cout << endl;
        for(int j = 0; j < n; j++){
            file >> maxData[i][j];
            cout << maxData[i][j] << " ";
        }
    }
    cout << endl;
    cout << endl;
    
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

    for (const auto& pair : pairs) {
        std::cout << "(" << pair.first << "," << pair.second << ")" << std::endl;
    }

    // Aqui mandamos a llamar e imprimir los algoritmos y todo gucci
    // Por cierto, puse muchos endl y couts para debugear por ahora, se pueden borrar

    // MST Prim
    cout << "Como conectar toda la colonia de forma optima: " << endl;
    MST(nbhs);

    return 0;
}
