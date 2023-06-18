#include <iostream>

using namespace std;

// Estructura de un nodo del Fenwick Tree
struct Node {
    int value;
    Node* parent;
    Node* left;
    Node* right;
};

// Función para crear un nuevo nodo del Fenwick Tree
Node* createNode(int value) {
    Node* newNode = new Node;
    newNode->value = value;
    newNode->parent = nullptr;
    newNode->left = nullptr;
    newNode->right = nullptr;
    return newNode;
}

// Función para agregar un elemento al Fenwick Tree
void addElement(Node* node, int index, int value) {
    while (index <= value) {
        node->value += value;
        index += index & (-index);
    }
}

// Función para eliminar un elemento del Fenwick Tree
void removeElement(Node* node, int index, int value) {
    while (index <= value) {
        node->value -= value;
        index += index & (-index);
    }
}

// Función para obtener la suma acumulativa hasta un índice dado
int getSum(Node* node, int index) {
    int sum = 0;
    while (index > 0) {
        sum += node->value;
        index -= index & (-index);
    }
    return sum;
}

// Función para buscar un elemento en el Fenwick Tree
bool searchElement(Node* node, int index, int value) {
    while (index <= value) {
        if (node->value == value) {
            return true;
        } else if (value < node->value && node->left != nullptr) {
            node = node->left;
        } else if (value > node->value && node->right != nullptr) {
            node = node->right;
        } else {
            break;
        }
    }
    return false;
}

int main() {
    Node* root = createNode(0);  // Crear el nodo raíz con valor 0
    
    // Agregar elementos al Fenwick Tree
    addElement(root, 1, 2);
    addElement(root, 2, 4);
    addElement(root, 3, 6);
    addElement(root, 4, 8);
    addElement(root, 5, 10);

    // Obtener la suma acumulativa hasta el índice 3
    int sum = getSum(root, 3);
    cout << "Suma acumulativa hasta el índice 3: " << sum << endl;

    // Eliminar el elemento con índice 4
    removeElement(root, 4, 8);

    // Buscar el elemento con valor 6
    bool found = searchElement(root, 1, 6);
    if (found) {
        cout << "El elemento con valor 6 se encuentra en el Fenwick Tree" << endl;
    } else {
        cout << "El elemento con valor 6 no se encuentra en el Fenwick Tree" << endl;
    }

    return 0;
}
