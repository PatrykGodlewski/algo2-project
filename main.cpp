#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

struct Node {
  int val;
  Node *left;
  Node *right;
  Node(int v) : val(v), left(nullptr), right(nullptr) {}
};

void freeTree(Node *root) {
  if (root) {
    freeTree(root->left);
    freeTree(root->right);
    delete root;
  }
}

Node *loadTreeFromString(const std::string &str, size_t &pos) {
  if (pos >= str.length() || str[pos] == ';' || str[pos] == ']') {
    return nullptr;
  }

  size_t initial_val_pos = pos;
  while (pos < str.length() && std::isdigit(str[pos])) {
    pos++;
  }

  if (pos == initial_val_pos) {
    throw std::runtime_error(
        "Nieprawidlowy format danych: oczekiwano wartosci wezla w pozycji " +
        std::to_string(initial_val_pos));
  }

  std::string val_str = str.substr(initial_val_pos, pos - initial_val_pos);
  int value;
  try {
    value = std::stoi(val_str);
  } catch (const std::out_of_range &oor) {
    throw std::runtime_error("Nieprawidlowy format danych: wartosc '" +
                             val_str + "' poza zakresem");
  } catch (const std::invalid_argument &ia) {
    throw std::runtime_error(
        "Nieprawidlowy format danych: nieprawidlowa wartosc liczbowa '" +
        val_str + "'");
  }

  Node *node = new Node(value);

  if (pos < str.length() && str[pos] == '[') {
    pos++;

    node->left = loadTreeFromString(str, pos);

    if (pos < str.length() && str[pos] == ';') {
      pos++;
      node->right = loadTreeFromString(str, pos);
    }

    if (pos < str.length() && str[pos] == ']') {
      pos++;
    } else {
      freeTree(node);
      throw std::runtime_error(
          "Nieprawidlowy format danych: brak zamykajacego ']' po parsowaniu "
          "dzieci dla wartosci " +
          val_str);
    }
  }
  return node;
}

Node *loadTreeFromFile(const std::string &filename) {
  std::ifstream file(filename);

  if (!file.is_open()) {
    throw std::runtime_error("Nie mozna otworzyc pliku: " + filename);
  }

  std::string content;
  if (!std::getline(file, content)) {
    if (file.eof() && content.empty())
      return nullptr;
    throw std::runtime_error("Plik jest pusty lub nie mozna odczytac linii: " +
                             filename);
  }
  if (content.empty())
    return nullptr;

  size_t pos = 0;
  Node *root = loadTreeFromString(content, pos);

  if (pos < content.length()) {
    std::string trailing_chars = content.substr(pos);
    if (!std::all_of(trailing_chars.begin(), trailing_chars.end(), isspace)) {
      freeTree(root);
      throw std::runtime_error(
          "Nieprawidlowy format danych: dodatkowe znaki '" + trailing_chars +
          "' po strukturze drzewa");
    }
  }
  return root;
}

void storeInorder(Node *root, std::vector<int> &vals) {
  if (!root)
    return;
  storeInorder(root->left, vals);
  vals.push_back(root->val);
  storeInorder(root->right, vals);
}

void arrayToBST(Node *root, const std::vector<int> &vals, int &idx) {
  if (!root)
    return;
  arrayToBST(root->left, vals, idx);
  if (idx < static_cast<int>(vals.size())) {
    root->val = vals[idx++];
  }
  arrayToBST(root->right, vals, idx);
}

void binaryTreeToBST(Node *root) {
  if (!root)
    return;
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

void printVisualTreeRecursive(Node *node, const std::string &prefix,
                              bool isLeftChild, bool isRoot) {
  if (node == nullptr) {
    return;
  }

  std::string rightChildPrefixContinuation =
      isRoot ? "    " : (isLeftChild ? "│   " : "    ");
  printVisualTreeRecursive(node->right, prefix + rightChildPrefixContinuation,
                           false, false);

  std::cout << prefix;
  if (!isRoot) {
    std::cout << (isLeftChild ? "└── " : "┌── ");
  }
  std::cout << node->val << std::endl;

  std::string leftChildPrefixContinuation =
      isRoot ? "    " : (isLeftChild ? "    " : "│   ");
  printVisualTreeRecursive(node->left, prefix + leftChildPrefixContinuation,
                           true, false);
}

void renderTreeInTerminal(Node *root) {
  if (!root) {
    std::cout << "(Drzewo jest puste)" << std::endl;
    return;
  }
  printVisualTreeRecursive(root, "", false, true);
}

int main(int argc, char *argv[]) {
  Node *root = nullptr;

  try {
    std::string filename;
    if (argc > 1) {
      filename = argv[1];
    } else {
      std::cout << "Podaj nazwe pliku: ";
      std::getline(std::cin, filename);
      if (filename.empty()) {
        std::cerr << "Nazwa pliku nie moze byc pusta." << std::endl;
        return 1;
      }
    }

    root = loadTreeFromFile(filename);

    if (!root) {
      std::cout << "Drzewo jest puste lub nie udalo sie wczytac." << std::endl;
    } else {
      std::cout << "Drzewo przed konwersja (inorder): ";
      printInorder(root);
      std::cout << std::endl;

      std::cout << "Struktura drzewa przed konwersja:" << std::endl;
      renderTreeInTerminal(root);
      std::cout << std::endl;

      binaryTreeToBST(root);

      std::cout << "Drzewo po konwersji na BST (inorder): ";
      printInorder(root);
      std::cout << std::endl;

      std::cout << "Struktura drzewa po konwersji na BST:" << std::endl;
      renderTreeInTerminal(root);
      std::cout << std::endl;
    }
    freeTree(root);
    root = nullptr;

  } catch (const std::exception &e) {
    std::cerr << "Blad: " << e.what() << std::endl;
    if (root) {
      freeTree(root);
    }
    return 1;
  }

  std::cout << "Nacisnij Enter, aby zakonczyc...";
  std::cin.clear();
  std::cin.get();

  return 0;
}
