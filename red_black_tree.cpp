#include <iostream>
using namespace std;

enum Color { RED, BLACK };

struct Node {
    int data;
    Color color;
    Node* left;
    Node* right;
    Node* parent;

    explicit Node(int value) {
        data = value;
        color = RED;
        left = nullptr;
        right = nullptr;
        parent = nullptr;
    }
};

class RedBlackTree {
private:
    Node* root;

    void leftRotate(Node* node) {
        Node* y = node->right;
        node->right = y->left;
        if (y->left != nullptr) {
            y->left->parent = node;
        }
        y->parent = node->parent;
        if (node->parent == nullptr) {
            root = y;
        } else if (node == node->parent->left) {
            node->parent->left = y;
        } else {
            node->parent->right = y;
        }
        y->left = node;
        node->parent = y;
    }

    void rightRotate(Node* node) {
        Node* x = node->left;
        node->left = x->right;
        if (x->right != nullptr) {
            x->right->parent = node;
        }
        x->parent = node->parent;
        if (node->parent == nullptr) {
            root = x;
        } else if (node == node->parent->right) {
            node->parent->right = x;
        } else {
            node->parent->left = x;
        }
        x->right = node;
        node->parent = x;
    }

    void fixViolation(Node* node) {
        Node* parent = nullptr;
        Node* grandParent = nullptr;

        while ((node != root) && (node->color != BLACK) && (node->parent->color == RED)) {
            parent = node->parent;
            grandParent = parent->parent;

            if (parent == grandParent->left) {
                Node* uncle = grandParent->right;

                if (uncle != nullptr && uncle->color == RED) {
                    grandParent->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    node = grandParent;
                } else {
                    if (node == parent->right) {
                        leftRotate(parent);
                        node = parent;
                        parent = node->parent;
                    }

                    rightRotate(grandParent);
                    swap(parent->color, grandParent->color);
                    node = parent;
                }
            } else {
                Node* uncle = grandParent->left;

                if (uncle != nullptr && uncle->color == RED) {
                    grandParent->color = RED;
                    parent->color = BLACK;
                    uncle->color = BLACK;
                    node = grandParent;
                } else {
                    if (node == parent->left) {
                        rightRotate(parent);
                        node = parent;
                        parent = node->parent;
                    }

                    leftRotate(grandParent);
                    swap(parent->color, grandParent->color);
                    node = parent;
                }
            }
        }

        root->color = BLACK;
    }

    Node* searchNode(Node* node, int value) {
        if (node == nullptr || node->data == value) {
            return node;
        }

        if (value < node->data) {
            return searchNode(node->left, value);
        }

        return searchNode(node->right, value);
    }

    Node* findMinimum(Node* node) {
        while (node->left != nullptr) {
            node = node->left;
        }
        return node;
    }

    Node* findSuccessor(Node* node) {
        if (node->right != nullptr) {
            return findMinimum(node->right);
        }

        Node* parent = node->parent;
        while (parent != nullptr && node == parent->right) {
            node = parent;
            parent = parent->parent;
        }
        return parent;
    }

    void deleteNode(Node* node) {
        Node* child = (node->left != nullptr) ? node->left : node->right;
        if (child != nullptr) {
            child->parent = node->parent;
        }

        if (node->parent == nullptr) {
            root = child;
        } else if (node == node->parent->left) {
            node->parent->left = child;
        } else {
            node->parent->right = child;
        }

        if (node->color == BLACK) {
            fixDoubleBlack(child);
        }

        delete node;
    }

    void fixDoubleBlack(Node* node) {
        if (node == root) {
            return;
        }

        Node* sibling = nullptr;
        bool leftChild = false;
        if (node->parent->left == nullptr || node->parent->left == node) {
            sibling = node->parent->right;
            leftChild = true;
        } else {
            sibling = node->parent->left;
            leftChild = false;
        }

        if (sibling->color == RED) {
            node->parent->color = RED;
            sibling->color = BLACK;
            if (leftChild) {
                leftRotate(node->parent);
            } else {
                rightRotate(node->parent);
            }
            fixDoubleBlack(node);
        } else {
            if ((sibling->left != nullptr && sibling->left->color == RED) ||
                (sibling->right != nullptr && sibling->right->color == RED)) {
                if (sibling->left != nullptr && sibling->left->color == RED) {
                    if (leftChild) {
                        sibling->left->color = sibling->color;
                        sibling->color = node->parent->color;
                        rightRotate(node->parent);
                    } else {
                        sibling->left->color = node->parent->color;
                        rightRotate(sibling);
                        leftRotate(node->parent);
                    }
                } else {
                    if (leftChild) {
                        sibling->right->color = node->parent->color;
                        leftRotate(sibling);
                        rightRotate(node->parent);
                    } else {
                        sibling->right->color = sibling->color;
                        sibling->color = node->parent->color;
                        leftRotate(node->parent);
                    }
                }
                node->parent->color = BLACK;
            } else {
                sibling->color = RED;
                if (node->parent->color == BLACK) {
                    fixDoubleBlack(node->parent);
                } else {
                    node->parent->color = BLACK;
                }
            }
        }
    }

    void inorderTraversal(Node* node) {
        if (node == nullptr) {
            return;
        }

        inorderTraversal(node->left);
        cout << node->data << " ";
        inorderTraversal(node->right);
    }

public:
    RedBlackTree() {
        root = nullptr;
    }

    void insert(int value) {
        Node* newNode = new Node(value);
        if (root == nullptr) {
            root = newNode;
            root->color = BLACK;
            return;
        }

        Node* current = root;
        Node* parent = nullptr;
        while (current != nullptr) {
            parent = current;
            if (value < current->data) {
                current = current->left;
            } else if (value > current->data) {
                current = current->right;
            } else {
                cout << "Duplicate value: " << value << endl;
                delete newNode;
                return;
            }
        }

        newNode->parent = parent;
        if (value < parent->data) {
            parent->left = newNode;
        } else {
            parent->right = newNode;
        }

        fixViolation(newNode);
    }

    void remove(int value) {
        Node* node = searchNode(root, value);
        if (node == nullptr) {
            cout << "Node not found: " << value << endl;
            return;
        }

        deleteNode(node);
    }

    void search(int value) {
        Node* node = searchNode(root, value);
        if (node != nullptr) {
            cout << "Node found: " << value << endl;
        } else {
            cout << "Node not found: " << value << endl;
        }
    }

    void printInorder() {
        cout << "Inorder traversal: ";
        inorderTraversal(root);
        cout << endl;
    }
};

int main() {
    RedBlackTree tree;

    tree.insert(7);
    tree.insert(3);
    tree.insert(18);
    tree.insert(10);
    tree.insert(22);
    tree.insert(8);
    tree.insert(11);
    tree.insert(26);
    tree.insert(2);
    tree.insert(6);
    tree.insert(13);

    tree.printInorder();

    tree.search(11);
    tree.search(15);

    tree.remove(13);
    tree.remove(22);

    tree.printInorder();

    return 0;
}
