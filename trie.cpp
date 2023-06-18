#include <iostream>
#include <unordered_map>
using namespace std;

// Definición de la clase TrieNode
class TrieNode {
public:
    bool isEndOfWord;
    unordered_map<char, TrieNode*> children;
    
    TrieNode() {
        isEndOfWord = false;
    }
};

// Definición de la clase Trie
class Trie {
private:
    TrieNode* root;
    
public:
    Trie() {
        root = new TrieNode();
    }
    
    void insert(string word) {
        TrieNode* curr = root;
        
        for (char ch : word) {
            if (curr->children.find(ch) == curr->children.end()) {
                curr->children[ch] = new TrieNode();
            }
            curr = curr->children[ch];
        }
        
        curr->isEndOfWord = true;
    }
    
    bool search(string word) {
        TrieNode* curr = root;
        
        for (char ch : word) {
            if (curr->children.find(ch) == curr->children.end()) {
                return false;
            }
            curr = curr->children[ch];
        }
        
        return curr->isEndOfWord;
    }
    
    bool startsWith(string prefix) {
        TrieNode* curr = root;
        
        for (char ch : prefix) {
            if (curr->children.find(ch) == curr->children.end()) {
                return false;
            }
            curr = curr->children[ch];
        }
        
        return true;
    }
    
    void remove(string word) {
        removeHelper(root, word, 0);
    }
    
private:
    bool removeHelper(TrieNode* curr, string word, int index) {
        if (index == word.length()) {
            if (!curr->isEndOfWord) {
                return false;
            }
            curr->isEndOfWord = false;
            
            return curr->children.empty();
        }
        
        char ch = word[index];
        if (curr->children.find(ch) == curr->children.end()) {
            return false;
        }
        
        bool shouldDelete = removeHelper(curr->children[ch], word, index + 1);
        if (shouldDelete) {
            delete curr->children[ch];
            curr->children.erase(ch);
            
            return curr->children.empty();
        }
        
        return false;
    }
};

int main() {
    Trie trie;

    // Agregar palabras al Trie
    trie.insert("apple");
    trie.insert("banana");
    trie.insert("orange");
    
    // Buscar palabras en el Trie
    cout << trie.search("apple") << endl;      // Devuelve 1 (verdadero)
    cout << trie.search("banana") << endl;     // Devuelve 1 (verdadero)
    cout << trie.search("orange") << endl;     // Devuelve 1 (verdadero)
    cout << trie.search("grape") << endl;      // Devuelve 0 (falso)
    
    // Eliminar palabras del Trie
    trie.remove("banana");
    cout << trie.search("banana") << endl;     // Devuelve 0 (falso)
    
    return 0;
}
