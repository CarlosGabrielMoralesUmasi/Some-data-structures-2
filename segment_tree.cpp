#include <iostream>
#include <queue>
#include <vector>

using namespace std;

struct Node {
    int start_index;
    int final_index;
    int sum;
    Node* left;
    Node* right;

    Node(int start, int end) {
        start_index = start;
        final_index = end;
        sum = 0;
        left = nullptr;
        right = nullptr;
    }
};

Node* buildSegmentTree(int* arr, int start, int end) {
    if (start > end)
        return nullptr;

    Node* node = new Node(start, end);

    if (start == end) {
        node->sum = arr[start];
        return node;
    }

    int mid = (start + end) / 2;
    node->left = buildSegmentTree(arr, start, mid);
    node->right = buildSegmentTree(arr, mid + 1, end);

    node->sum = node->left->sum + node->right->sum;

    return node;
}

void printSegmentTree(Node* root) {
    if (root == nullptr)
        return;

    queue<Node*> q;
    q.push(root);

    while (!q.empty()) {
        int level_size = q.size();

        for (int i = 0; i < level_size; i++) {
            Node* node = q.front();
            q.pop();

            cout << "Depth " << node->start_index << "-" << node->final_index << ": ";
            cout << "[" << node->start_index << ", " << node->final_index << "] = " << node->sum << endl;

            if (node->left != nullptr)
                q.push(node->left);
            if (node->right != nullptr)
                q.push(node->right);
        }
    }
}


int main() {
    int arr[] = {1, 3, 5, 7, 9, 11};
    int size = sizeof(arr) / sizeof(arr[0]);

    Node* root = buildSegmentTree(arr, 0, size - 1);

    printSegmentTree(root);

    return 0;
}
