#include <iostream>
using namespace std;
struct Interval {
    int low;
    int high;
};

struct IntervalNode {
    Interval* interval;
    int max;
    IntervalNode* left;
    IntervalNode* right;
};

IntervalNode* createIntervalNode(Interval* interval) {
    IntervalNode* newNode = new IntervalNode;
    newNode->interval = interval;
    newNode->max = interval->high;
    newNode->left = nullptr;
    newNode->right = nullptr;
    return newNode;
}

IntervalNode* insert(IntervalNode* root, Interval* interval) {
    if (root == nullptr)
        return createIntervalNode(interval);
    
    int low = root->interval->low;
    
    if (interval->low < low)
        root->left = insert(root->left, interval);
    else
        root->right = insert(root->right, interval);
    
    if (root->max < interval->high)
        root->max = interval->high;
    
    return root;
}

bool doOverlap(Interval* i1, Interval* i2) {
    return (i1->low <= i2->high && i2->low <= i1->high);
}

Interval* searchOverlap(IntervalNode* root, Interval* interval) {
    if (root == nullptr)
        return nullptr;
    
    if (doOverlap(root->interval, interval))
        return root->interval;
    
    if (root->left != nullptr && root->left->max >= interval->low)
        return searchOverlap(root->left, interval);
    
    return searchOverlap(root->right, interval);
}

IntervalNode* minValueNode(IntervalNode* node) {
    IntervalNode* current = node;
    
    while (current->left != nullptr)
        current = current->left;
    
    return current;
}

IntervalNode* deleteInterval(IntervalNode* root, Interval* interval) {
    if (root == nullptr)
        return root;
    
    if (interval->low < root->interval->low)
        root->left = deleteInterval(root->left, interval);
    else if (interval->low > root->interval->low)
        root->right = deleteInterval(root->right, interval);
    else {
        if (root->left == nullptr) {
            IntervalNode* temp = root->right;
            delete root;
            return temp;
        }
        else if (root->right == nullptr) {
            IntervalNode* temp = root->left;
            delete root;
            return temp;
        }
        
        IntervalNode* temp = minValueNode(root->right);
        root->interval = temp->interval;
        root->right = deleteInterval(root->right, temp->interval);
    }
    
    return root;
}

void inorderTraversal(IntervalNode* root) {
    if (root != nullptr) {
        inorderTraversal(root->left);
        cout << "[" << root->interval->low << ", " << root->interval->high << "] ";
        inorderTraversal(root->right);
    }
}

int main() {
    Interval intervals[] = { {15, 20}, {10, 30}, {17, 19}, {5, 20}, {12, 15}, {30, 40} };
    int n = sizeof(intervals) / sizeof(intervals[0]);
    IntervalNode* root = nullptr;
    
    for (int i = 0; i < n; i++)
        root = insert(root, &intervals[i]);
    
    cout << "Interval Tree: ";
    inorderTraversal(root);
    cout << endl;

    Interval searchInterval = { 14, 16 };
    Interval* overlap = searchOverlap(root, &searchInterval);

    if (overlap != nullptr) {
        cout << "Overlap found: [" << overlap->low << ", " << overlap->high << "]" << endl;
    }
    else {
        cout << "No overlap found." << endl;
    }

    Interval intervalToDelete = { 15, 20 };
    root = deleteInterval(root, &intervalToDelete);

    cout << "Interval Tree after deletion: ";
    inorderTraversal(root);
    cout << endl;

    return 0;
}
