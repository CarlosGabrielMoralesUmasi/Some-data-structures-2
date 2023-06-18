#include <iostream>

using namespace std;

// Definición de la estructura de un nodo
struct Node {
    int data;
    Node* next;
};

// Clase Cola
class Queue {
private:
    Node* front;
    Node* rear;

public:
    // Constructor
    Queue() {
        front = rear = nullptr;
    }

    // Destructor
    ~Queue() {
        while (!isEmpty()) {
            remove();
        }
    }

    // Función para verificar si la cola está vacía
    bool isEmpty() {
        return front == nullptr;
    }

    // Función para agregar un elemento a la cola
    void add(int value) {
        Node* newNode = new Node;
        newNode->data = value;
        newNode->next = nullptr;

        if (isEmpty()) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }

        cout << "Elemento " << value << " agregado a la cola." << endl;
    }

    // Función para eliminar un elemento de la cola
    void remove() {
        if (isEmpty()) {
            cout << "La cola está vacía. No se puede eliminar ningún elemento." << endl;
        } else {
            Node* temp = front;
            int removedValue = temp->data;

            if (front == rear) {
                front = rear = nullptr;
            } else {
                front = front->next;
            }

            delete temp;
            cout << "Elemento " << removedValue << " eliminado de la cola." << endl;
        }
    }

    // Función para buscar un elemento en la cola
    bool find(int value) {
        Node* temp = front;
        while (temp != nullptr) {
            if (temp->data == value) {
                return true;
            }
            temp = temp->next;
        }
        return false;
    }
};

// Función principal
int main() {
    Queue queue;

    queue.add(10);
    queue.add(20);
    queue.add(30);

    cout << "La cola " << (queue.find(20) ? "contiene" : "no contiene") << " el elemento 20." << endl;
    cout << "La cola " << (queue.find(40) ? "contiene" : "no contiene") << " el elemento 40." << endl;

    queue.remove();
    queue.remove();
    queue.remove();

    return 0;
}
