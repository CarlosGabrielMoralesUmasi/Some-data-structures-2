#include <iostream>
using namespace std;
// Definición de la estructura de un nodo
struct Node {
    int data;
    Node* next;
};

// Clase Pila
class Stack {
private:
    Node* top;  // Puntero al nodo superior (cima) de la pila

public:
    // Constructor
    Stack() {
        top = nullptr;
    }

    // Función para comprobar si la pila está vacía
    bool isEmpty() {
        return (top == nullptr);
    }

    // Función para agregar un elemento a la pila
    void push(int value) {
        Node* newNode = new Node();
        newNode->data = value;
        newNode->next = top;
        top = newNode;
        cout << "Elemento " << value << " agregado a la pila.\n";
    }

    // Función para eliminar un elemento de la pila
    void remove(int value) {
        if (isEmpty()) {
            cout << "La pila está vacía. No se puede eliminar ningún elemento.\n";
            return;
        }

        Node* temp = top;
        Node* prev = nullptr;

        // Buscar el elemento a eliminar
        while (temp != nullptr && temp->data != value) {
            prev = temp;
            temp = temp->next;
        }

        // Si se encontró el elemento
        if (temp != nullptr) {
            // Si el elemento a eliminar es el nodo superior
            if (temp == top) {
                top = top->next;
            } else {
                prev->next = temp->next;
            }

            delete temp;
            cout << "Elemento " << value << " eliminado de la pila.\n";
        } else {
            cout << "Elemento " << value << " no encontrado en la pila.\n";
        }
    }

    // Función para buscar un elemento en la pila
    bool find(int value) {
        Node* temp = top;

        while (temp != nullptr) {
            if (temp->data == value) {
                return true;
            }
            temp = temp->next;
        }

        return false;
    }
};

// Ejemplo de uso
int main() {
    Stack stack;

    stack.push(10);
    stack.push(20);
    stack.push(30);

    stack.remove(20);
    stack.remove(40);

    cout << "Elemento 10 " << (stack.find(10) ? "encontrado" : "no encontrado") << " en la pila.\n";
    cout << "Elemento 40 " << (stack.find(40) ? "encontrado" : "no encontrado") << " en la pila.\n";

    return 0;
}
