#include <iostream>
using namespace std;

const int TABLE_SIZE = 10;

// Definición de la clase Nodo
class Node {
public:
    int key;
    int value;
    Node* next;

    Node(int k, int v) {
        key = k;
        value = v;
        next = nullptr;
    }
};

// Definición de la clase HashTable
class HashTable {
private:
    Node** table;

public:
    HashTable() {
        table = new Node*[TABLE_SIZE];
        for (int i = 0; i < TABLE_SIZE; i++) {
            table[i] = nullptr;
        }
    }

    ~HashTable() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            Node* current = table[i];
            while (current != nullptr) {
                Node* temp = current;
                current = current->next;
                delete temp;
            }
        }
        delete[] table;
    }

    // Función de hash simple
    int hashFunction(int key) {
        return key % TABLE_SIZE;
    }

    // Función para insertar un elemento en la tabla hash
    void insert(int key, int value) {
        int index = hashFunction(key);

        Node* newNode = new Node(key, value);

        if (table[index] == nullptr) {
            table[index] = newNode;
        } else {
            Node* current = table[index];
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }
    }

    // Función para buscar un elemento en la tabla hash
    Node* find(int key) {
        int index = hashFunction(key);

        Node* current = table[index];
        while (current != nullptr) {
            if (current->key == key) {
                return current;
            }
            current = current->next;
        }
        return nullptr;
    }

    // Función para eliminar un elemento de la tabla hash
    void remove(int key) {
        int index = hashFunction(key);

        Node* current = table[index];
        Node* previous = nullptr;

        while (current != nullptr && current->key != key) {
            previous = current;
            current = current->next;
        }

        if (current != nullptr) {
            if (previous == nullptr) {
                table[index] = current->next;
            } else {
                previous->next = current->next;
            }
            delete current;
        }
    }
};

// Ejemplo de uso
int main() {
    HashTable hashTable;

    // Insertar elementos
    hashTable.insert(1, 10);
    hashTable.insert(2, 20);
    hashTable.insert(11, 30);
    hashTable.insert(21, 40);

    // Buscar elementos
    Node* node1 = hashTable.find(1);
    if (node1 != nullptr) {
        cout << "Elemento encontrado: " << node1->value << endl;
    } else {
        cout << "Elemento no encontrado" << endl;
    }

    Node* node3 = hashTable.find(11);
    if (node3 != nullptr) {
        cout << "Elemento encontrado: " << node3->value << endl;
    } else {
        cout << "Elemento no encontrado" << endl;
    }

    // Eliminar un elemento
    hashTable.remove(2);

    Node* node2 = hashTable.find(2);
    if (node2 != nullptr) {
        cout << "Elemento encontrado: " << node2->value << endl;
    } else {
        cout << "Elemento no encontrado" << endl;
    }

    return 0;
}
