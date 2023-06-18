#include <iostream>
#include <queue>
#include <vector>

using namespace std;

// Función para realizar el recorrido BFS en el grafo
void BFS(vector<vector<int>>& graph, int startVertex) {
    int numVertices = graph.size();
    vector<bool> visited(numVertices, false); // Vector para realizar seguimiento de los nodos visitados

    // Crear una cola para el recorrido BFS
    queue<int> q;

    // Marcar el nodo de inicio como visitado y encolarlo
    visited[startVertex] = true;
    q.push(startVertex);

    cout << "Recorrido BFS desde el nodo " << startVertex << ": ";

    while (!q.empty()) {
        int currentVertex = q.front();
        q.pop();
        cout << currentVertex << " ";

        // Obtener los nodos adyacentes al nodo actual y encolarlos si no han sido visitados
        for (int adjacentVertex : graph[currentVertex]) {
            if (!visited[adjacentVertex]) {
                visited[adjacentVertex] = true;
                q.push(adjacentVertex);
            }
        }
    }

    cout << endl;
}

int main() {
    int numVertices, numEdges;
    cout << "Ingrese el número de vértices del grafo: ";
    cin >> numVertices;

    cout << "Ingrese el número de aristas del grafo: ";
    cin >> numEdges;

    // Crear un grafo utilizando listas de adyacencia
    vector<vector<int>> graph(numVertices);

    cout << "Ingrese las aristas del grafo (formato: nodo1 nodo2):" << endl;
    for (int i = 0; i < numEdges; i++) {
        int u, v;
        cin >> u >> v;

        // Agregar una conexión bidireccional entre los nodos u y v
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    int startVertex;
    cout << "Ingrese el nodo de inicio para el recorrido BFS: ";
    cin >> startVertex;

    BFS(graph, startVertex);

    return 0;
}
