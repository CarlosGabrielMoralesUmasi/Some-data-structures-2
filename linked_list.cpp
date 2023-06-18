#include <iostream>
using namespace std;
struct Node {
    int data;
    Node* next;
};

class LinkedList {
private:
    Node* head;

public:
    LinkedList() {
        head = nullptr;
    }

    void insert(int value) {
        Node* newNode = new Node;
        newNode->data = value;
        newNode->next = nullptr;

        if (head == nullptr) {
            head = newNode;
        } else {
            Node* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }
    }

    void remove(int value) {
        if (head == nullptr) {
            return;
        }

        if (head->data == value) {
            Node* temp = head;
            head = head->next;
            delete temp;
            return;
        }

        Node* current = head;
        Node* previous = nullptr;

        while (current != nullptr && current->data != value) {
            previous = current;
            current = current->next;
        }

        if (current != nullptr) {
            previous->next = current->next;
            delete current;
        }
    }

    bool find(int value) {
        Node* current = head;
        while (current != nullptr) {
            if (current->data == value) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    void display() {
        Node* current = head;
        while (current != nullptr) {
            cout << current->data << " ";
            current = current->next;
        }
        cout << endl;
    }
};

int main() {
    LinkedList list;

    // Insertar elementos en la lista
    list.insert(10);
    list.insert(20);
    list.insert(30);
    list.insert(40);

    // Mostrar la lista
    list.display();

    // Buscar un elemento
    int valueToFind = 20;
    if (list.find(valueToFind)) {
        cout << valueToFind << " encontrado en la lista." << endl;
    } else {
        cout << valueToFind << " no encontrado en la lista." << endl;
    }

    // Eliminar un elemento
    int valueToRemove = 30;
    list.remove(valueToRemove);

    // Mostrar la lista después de eliminar un elemento
    list.display();

    return 0;
}
