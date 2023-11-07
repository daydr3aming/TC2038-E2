#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

void MST(){
    // Algoritmo de Kruskal o Prim
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

    return 0;
}
