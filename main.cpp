#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include "voronoi.h"

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

/* Utilizamos Fortunes Algorithm para generar el diagrama de Voronoi
   Complejidad: O(nlogn) 
   Recuperado de: https://www.cs.hmc.edu/~mbrubeck/voronoi.html#:~:text=The%20visualization%20shows%20a%20line,each%20point%20it%20has%20passed.
*/

// ---------- INICIO SECCION: Funciones de Voronoi -----------

    priority_queue<point,  vector<point>,  gt> points; // site events
    priority_queue<event*, vector<event*>, gt> events; // circle events
    vector<vector<double>> voronoiVertices; // vertices of the voronoi diagram

    void process_point()
    {
    // Get the next point from the queue.
    point p = points.top();
    points.pop();

    // Add a new arc to the parabolic front.
    front_insert(p);
    }

    void process_event()
    {
    // Get the next event from the queue.
    event *e = events.top();
    events.pop();

    if (e->valid) {
        // Start a new edge.
        seg *s = new seg(e->p);

        // Remove the associated arc from the front.
        arc *a = e->a;
        if (a->prev) {
            a->prev->next = a->next;
            a->prev->s1 = s;
        }
        if (a->next) {
            a->next->prev = a->prev;
            a->next->s0 = s;
        }

        // Finish the edges before and after a.
        if (a->s0) a->s0->finish(e->p);
        if (a->s1) a->s1->finish(e->p);

        // Recheck circle events on either side of p:
        if (a->prev) check_circle_event(a->prev, e->x);
        if (a->next) check_circle_event(a->next, e->x);
    }
    delete e;
    }

    void front_insert(point p)
    {
    if (!root) {
        root = new arc(p);
        return;
    }

    // Find the current arc(s) at height p.y (if there are any).
    for (arc *i = root; i; i = i->next) {
        point z, zz;
        if (intersect(p,i,&z)) {
            // New parabola intersects arc i.  If necessary, duplicate i.
            if (i->next && !intersect(p,i->next, &zz)) {
                i->next->prev = new arc(i->p,i,i->next);
                i->next = i->next->prev;
            }
            else i->next = new arc(i->p,i);
            i->next->s1 = i->s1;

            // Add p between i and i->next.
            i->next->prev = new arc(p,i,i->next);
            i->next = i->next->prev;

            i = i->next; // Now i points to the new arc.

            // Add new half-edges connected to i's endpoints.
            i->prev->s1 = i->s0 = new seg(z);
            i->next->s0 = i->s1 = new seg(z);

            // Check for new circle events around the new arc:
            check_circle_event(i, p.x);
            check_circle_event(i->prev, p.x);
            check_circle_event(i->next, p.x);

            return;
        }
    }

    // Special case: If p never intersects an arc, append it to the list.
    arc *i;
    for (i = root; i->next; i=i->next) ; // Find the last node.

    i->next = new arc(p,i);
    // Insert segment between p and i
    point start;
    start.x = X0;
    start.y = (i->next->p.y + i->p.y) / 2;
    i->s1 = i->next->s0 = new seg(start);
    }

    // Look for a new circle event for arc i.
    void check_circle_event(arc *i, double x0)
    {
    // Invalidate any old event.
    if (i->e && i->e->x != x0)
        i->e->valid = false;
    i->e = NULL;

    if (!i->prev || !i->next)
        return;

    double x;
    point o;

    if (circle(i->prev->p, i->p, i->next->p, &x,&o) && x > x0) {
        // Create new event.
        i->e = new event(x, o, i);
        events.push(i->e);
    }
    }

    // Find the rightmost point on the circle through a,b,c.
    bool circle(point a, point b, point c, double *x, point *o)
    {
    // Check that bc is a "right turn" from ab.
    if ((b.x-a.x)*(c.y-a.y) - (c.x-a.x)*(b.y-a.y) > 0)
        return false;

    // Algorithm from O'Rourke 2ed p. 189.
    double A = b.x - a.x,  B = b.y - a.y,
            C = c.x - a.x,  D = c.y - a.y,
            E = A*(a.x+b.x) + B*(a.y+b.y),
            F = C*(a.x+c.x) + D*(a.y+c.y),
            G = 2*(A*(c.y-b.y) - B*(c.x-b.x));

    if (G == 0) return false;  // Points are co-linear.

    // Point o is the center of the circle.
    o->x = (D*E-B*F)/G;
    o->y = (A*F-C*E)/G;

    // o.x plus radius equals max x coordinate.
    *x = o->x + sqrt( pow(a.x - o->x, 2) + pow(a.y - o->y, 2) );
    return true;
    }

    // Will a new parabola at point p intersect with arc i?
    bool intersect(point p, arc *i, point *res)
    {
    if (i->p.x == p.x) return false;

    double a,b;
    if (i->prev) // Get the intersection of i->prev, i.
        a = intersection(i->prev->p, i->p, p.x).y;
    if (i->next) // Get the intersection of i->next, i.
        b = intersection(i->p, i->next->p, p.x).y;

    if ((!i->prev || a <= p.y) && (!i->next || p.y <= b)) {
        res->y = p.y;

        // Plug it back into the parabola equation.
        res->x = (i->p.x*i->p.x + (i->p.y-res->y)*(i->p.y-res->y) - p.x*p.x)
                    / (2*i->p.x - 2*p.x);

        return true;
    }
    return false;
    }

    // Where do two parabolas intersect?
    point intersection(point p0, point p1, double l)
    {
    point res, p = p0;

    if (p0.x == p1.x)
        res.y = (p0.y + p1.y) / 2;
    else if (p1.x == l)
        res.y = p1.y;
    else if (p0.x == l) {
        res.y = p0.y;
        p = p1;
    } else {
        // Use the quadratic formula.
        double z0 = 2*(p0.x - l);
        double z1 = 2*(p1.x - l);

        double a = 1/z0 - 1/z1;
        double b = -2*(p0.y/z0 - p1.y/z1);
        double c = (p0.y*p0.y + p0.x*p0.x - l*l)/z0
                - (p1.y*p1.y + p1.x*p1.x - l*l)/z1;

        res.y = ( -b - sqrt(b*b - 4*a*c) ) / (2*a);
    }
    // Plug back into one of the parabola equations.
    res.x = (p.x*p.x + (p.y-res.y)*(p.y-res.y) - l*l)/(2*p.x-2*l);
    return res;
    }

    void finish_edges()
    {
    // Advance the sweep line so no parabolas can cross the bounding box.
    double l = X1 + (X1-X0) + (Y1-Y0);

    // Extend each remaining segment to the new parabola intersections.
    for (arc *i = root; i->next; i = i->next)
        if (i->s1)
            i->s1->finish(intersection(i->p, i->next->p, l*2));
    }

    void create_output()
    {
    // Each output segment in four-column format.
    vector<seg*>::iterator i;
    for (i = output.begin(); i != output.end(); i++) {
        point p0 = (*i)->start;
        point p1 = (*i)->end;
        voronoiVertices.push_back({p0.x, p0.y, p1.x, p1.y});
    }
}

// ---------- FIN DE SECCION: Diagrama de Voronoi -----------


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

    int start = 0;  // Asumimos que el vecindario inicial es 0
    printRoute(parent, start, n);
    cout << endl;

    // Flujo Maximo Salida 3
    int source = 0;
    int sink = 3;

    int maxFlow = MaxFlow(maxData, source, sink);
    cout << "Flujo maximo de informacion de " << source << " a " << sink << ":" <<endl;
    cout << maxFlow << endl;

    // Diagrama de Voronoi Salida 4

    point p;
    for(int i = 0 ; i < pairs.size(); i++){
        p.x = pairs[i].first;
        p.y = pairs[i].second;
        points.push(p);
        // Keep track of bounding box size.
        if (p.x < X0) X0 = p.x;
        if (p.y < Y0) Y0 = p.y;
        if (p.x > X1) X1 = p.x;
        if (p.y > Y1) Y1 = p.y;
    }

    // Add margins to the bounding box.
    double dx = (X1-X0+1)/5.0, dy = (Y1-Y0+1)/5.0;
    X0 -= dx;  X1 += dx;  Y0 -= dy;  Y1 += dy;

    // Process the queues; select the top element with smaller x coordinate.
    while (!points.empty())
        if (!events.empty() && events.top()->x <= points.top().x)
            process_event();
        else
            process_point();

    // After all points are processed, do the remaining circle events.
    while (!events.empty())
        process_event();

    finish_edges(); // Clean up dangling edges.
    ofstream output_file("output.txt");
    cout << "Caja limitante: " << endl;
    cout << X0 << " "<< X1 << " " << Y0 << " " << Y1 << endl;
    output_file << X0 << " "<< X1 << " " << Y0 << " " << Y1 << endl;
    create_output(); // Output the voronoi diagram.
    cout << "Vertices de poligonos de Voronoi: " << endl;
    for(int i = 0; i < voronoiVertices.size(); i++){
        cout << voronoiVertices[i][0] << " " << voronoiVertices[i][1] << " " << voronoiVertices[i][2] << " " << voronoiVertices[i][3] << endl;
        output_file << voronoiVertices[i][0] << " " << voronoiVertices[i][1] << " " << voronoiVertices[i][2] << " " << voronoiVertices[i][3] << endl;
    }

    return 0;
}
