// Konwersja Drzewa Binarnego na Drzewo BST
#include <algorithm>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <stdexcept>

struct Node {
  int val;
  Node *left;
  Node *right;
  Node(int v) : val(v), left(nullptr), right(nullptr) {}
};

// Funkcja do wczytywania drzewa z pliku
Node *loadTreeFromString(const std::string &str, size_t &pos) {
    if (pos >= str.length()) {
        return nullptr;
    }

    // Znajdź wartość węzła
    size_t end = str.find('[', pos);
    if (end == std::string::npos) {
        throw std::runtime_error("Nieprawidlowy format danych");
    }

    // Konwertuj string na int
    int value = std::stoi(str.substr(pos, end - pos));
    Node *node = new Node(value);

    pos = end + 1; // Przejdz za '['

    // Wczytaj lewe poddrzewo
    if (pos < str.length() && str[pos] != ';') {
        node->left = loadTreeFromString(str, pos);
    }

    // Przejdz za ';' jesli istnieje
    if (pos < str.length() && str[pos] == ';') pos++;

    // Wczytaj prawe poddrzewo
    if (pos < str.length() && str[pos] != ']') {
        node->right = loadTreeFromString(str, pos);
    }

    // Przejdz za ']'
    if (pos < str.length() && str[pos] == ']') pos++;

    return node;
}

Node *loadTreeFromFile(const std::string &filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Nie mozna otworzyc pliku: " + filename);
    }

    std::string content;
    std::getline(file, content);
    size_t pos = 0;
    return loadTreeFromString(content, pos);
}

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

void binaryTreeToBST(Node *root) {
    std::vector<int> vals;
    storeInorder(root, vals);
    std::sort(vals.begin(), vals.end());
    int idx = 0;
    arrayToBST(root, vals, idx);
}

void printInorder(Node *root) {
    if (!root)
        return;
    printInorder(root->left);
    std::cout << root->val << " ";
    printInorder(root->right);
}

void freeTree(Node *root) {
    if (root) {
        freeTree(root->left);
        freeTree(root->right);
        delete root;
    }
}

int main(int argc, char *argv[]) {
    try {
        std::string filename;
        if (argc > 1) {
            filename = argv[1];
        } else {
            std::cout << "Podaj nazwe pliku: ";
            std::getline(std::cin, filename);
        }

        std::cout << filename;
        Node *root = loadTreeFromFile(filename);

        std::cout << "Drzewo przed konwersja: ";
        printInorder(root);
        std::cout << std::endl;

        binaryTreeToBST(root);

        std::cout << "Drzewo po konwersji na BST: ";
        printInorder(root);
        std::cout << std::endl;

        // Zwolnienie pamięci
        freeTree(root);
    } catch (const std::exception &e) {
        std::cerr << "Blad: " << e.what() << std::endl;
        return 1;
    }

    std::cin.ignore();
    std::cin.get();
    return 0;
}
