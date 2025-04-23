#include <algorithm>
#include <iostream>
#include <vector>

struct Node {
  int val;
  Node *left;
  Node *right;
  Node(int v) : val(v), left(nullptr), right(nullptr) {}
};

// Krok 1: Zbierz wszystkie wartości w inorder
void storeInorder(Node *root, std::vector<int> &vals) {
  if (!root)
    return;
  storeInorder(root->left, vals);
  vals.push_back(root->val);
  storeInorder(root->right, vals);
}

// Krok 3: Przypisz posortowane wartości w inorder
void arrayToBST(Node *root, std::vector<int> &vals, int &idx) {
  if (!root)
    return;
  arrayToBST(root->left, vals, idx);
  root->val = vals[idx++];
  arrayToBST(root->right, vals, idx);
}

// Funkcja konwertująca drzewo binarne na BST
void binaryTreeToBST(Node *root) {
  std::vector<int> vals;
  storeInorder(root, vals);            // Krok 1
  std::sort(vals.begin(), vals.end()); // Krok 2
  int idx = 0;
  arrayToBST(root, vals, idx); // Krok 3
}

// Pomocnicza funkcja inorder do sprawdzenia wyniku
void printInorder(Node *root) {
  if (!root)
    return;
  printInorder(root->left);
  std::cout << root->val << " ";
  printInorder(root->right);
}

// Przykład użycia
int main() {
  /*
          10
         /  \
        2    7
       / \
      8   4
  */
  Node *root = new Node(10);
  root->left = new Node(2);
  root->right = new Node(7);
  root->left->left = new Node(8);
  root->left->right = new Node(4);

  printInorder(root);
  std::cout << std::endl;

  binaryTreeToBST(root);

  printInorder(root); // Powinno wypisać: 2 4 7 8 10
  std::cout << std::endl;

  // Zwolnienie pamięci pominięte dla uproszczenia
  return 0;
}
