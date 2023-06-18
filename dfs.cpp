#include <iostream>
#include <vector>
#include <stack>

using namespace std;

// Clase para representar un grafo utilizando listas de adyacencia
class Graph {
    int numVertices; // Número de vértices
    vector<vector<int>> adjList; // Lista de adyacencia

public:
    Graph(int V) {
        numVertices = V;
        adjList.resize(numVertices);
    }

    // Función para agregar una arista al grafo
    void addEdge(int u, int v) {
        adjList[u].push_back(v);
    }

    // Función de recorrido en profundidad (DFS)
    void DFS(int startVertex) {
        // Vector para marcar los vértices visitados
        vector<bool> visited(numVertices, false);

        // Pila para realizar el recorrido en profundidad
        stack<int> stk;
        stk.push(startVertex);

        while (!stk.empty()) {
            int currVertex = stk.top();
            stk.pop();

            if (!visited[currVertex]) {
                visited[currVertex] = true;
                cout << currVertex << " ";

                // Agregar los vértices adyacentes no visitados a la pila
                for (int adjVertex : adjList[currVertex]) {
                    if (!visited[adjVertex]) {
                        stk.push(adjVertex);
                    }
                }
            }
        }
    }
};

int main() {
    // Crear un grafo con 6 vértices
    Graph g(6);

    // Agregar las aristas del grafo
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(2, 3);
    g.addEdge(2, 4);
    g.addEdge(3, 4);
    g.addEdge(3, 5);
    g.addEdge(4, 5);

    // Realizar el recorrido en profundidad (DFS) desde el vértice 0
    cout << "Recorrido en profundidad (DFS): ";
    g.DFS(0);

    return 0;
}
