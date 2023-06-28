#include <iostream>
#include <utility>
#include <vector>
#include <stdint.h>
#include "BST.h"

BinarySearchTree::Node::Node(Key key, 
                            Value value, 
                            Node* parent, 
                            Node* left, 
                            Node* right)
    : keyValuePair{key, value}, parent{parent}, left{left}, right{right} {}

BinarySearchTree::~BinarySearchTree() {
    tree_delete(_root);
}

BinarySearchTree::BinarySearchTree(const BinarySearchTree& other) : _size{other._size} {
    _root = tree_copy(other._root);
}

BinarySearchTree& BinarySearchTree::operator=(const BinarySearchTree& other) {
    if (this == &other) {
        return *this;
    }
    this->~BinarySearchTree();
    _root = tree_copy(other._root);
    _size = other._size;
    return *this;
}

BinarySearchTree::BinarySearchTree(BinarySearchTree&& other) noexcept 
    : _size{other._size}, _root{other._root} {
    other._size = 0;
    other._root = nullptr;
}

BinarySearchTree& BinarySearchTree::operator=(BinarySearchTree&& other) noexcept {
    if (this == &other) {
        return *this;
    }
    this->~BinarySearchTree();
    _root = other._root;
    _size = other._size;
    other._root = nullptr;
    other._size = 0;
    return *this;
}

BinarySearchTree::Iterator::Iterator(Node* node) : _node{node} {}

std::pair<Key, Value>& BinarySearchTree::Iterator::operator*() {
    return _node->keyValuePair;
}

const std::pair<Key, Value>& BinarySearchTree::Iterator::operator*() const {
    return _node->keyValuePair;
}

std::pair<Key, Value>* BinarySearchTree::Iterator::operator->() {
    return &_node->keyValuePair;
}

const std::pair<Key, Value>* BinarySearchTree::Iterator::operator->() const {
    return &_node->keyValuePair;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++() {
    if (_node->right != nullptr) {
        _node = _node->right; // tree minimum
        while (_node->left != nullptr) {
            _node = _node->left;
        }
    }else{
        while ((_node->parent != nullptr) && (_node == _node->parent->right)) {
            _node = _node->parent;
        }
        _node = _node->parent;
    }
    return *this;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator++(int) {
    Iterator result(*this);
    ++(*this);
    return result;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--() {
    if (_node->left != nullptr) {
        _node = _node->left; // tree maximum
        while (_node->right != nullptr) {
            _node = _node->right;
        }
    }else{
        while ((_node->parent != nullptr) && (_node == _node->parent->left)) {
            _node = _node->parent;
        }
        _node = _node->parent;
    }
    return *this;
}

BinarySearchTree::Iterator BinarySearchTree::Iterator::operator--(int) {
    Iterator result(*this);
    --(*this);
    return result;
}

bool BinarySearchTree::Iterator::operator==(const Iterator& other) const {
    return _node == other._node;
}

bool BinarySearchTree::Iterator::operator!=(const Iterator& other) const {
    return _node != other._node;
}

BinarySearchTree::ConstIterator::ConstIterator(const Node* node) : _node{node} {}

const std::pair<Key, Value>& BinarySearchTree::ConstIterator::operator*() const {
    return _node->keyValuePair;
}

const std::pair<Key, Value>* BinarySearchTree::ConstIterator::operator->() const {
    return &_node->keyValuePair;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++() {
    if (_node->right != nullptr) {
        _node = _node->right; // tree minimum
        while (_node->left != nullptr) {
            _node = _node->left;
        }
    }else{
        while ((_node->parent != nullptr) && (_node == _node->parent->right)) {
            _node = _node->parent;
        }
        _node = _node->parent;
    }
    return *this;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator++(int) {
    ConstIterator result(*this);
    ++(*this);
    return result;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--() {
    if (_node->left != nullptr) {
        _node = _node->left; // tree maximum
        while (_node->right != nullptr) {
            _node = _node->right;
        }
    }else{
        while ((_node->parent != nullptr) && (_node == _node->parent->left)) {
            _node = _node->parent;
        }
        _node = _node->parent;
    }
    return *this;
}

BinarySearchTree::ConstIterator BinarySearchTree::ConstIterator::operator--(int) {
    ConstIterator result(*this);
    --(*this);
    return result;
}

bool BinarySearchTree::ConstIterator::operator==(const ConstIterator& other) const {
    return _node == other._node;
}

bool BinarySearchTree::ConstIterator::operator!=(const ConstIterator& other) const {
    return _node != other._node;
}

int BinarySearchTree::height(Node *p) {
    if (p == nullptr) {
        return 0;
    }
    return p->height;
}

int BinarySearchTree::balanceFactor(Node *p) {
    if (p == nullptr) {
        return 0;
    }
    return height(p->left) - height(p->right);
}

void BinarySearchTree::fixHeight(Node *p) {
    if (p != nullptr) {
        int val = 1;
        if (p->left != nullptr) { 
            val = p->left->height + 1;
        }
        if (p->right != nullptr) {
            val = std::max(val, p->right->height + 1);
        }
        p->height = val;
    }
}

BinarySearchTree::Node * BinarySearchTree::rightRotation(Node *p) {
    Node *q = p->left;
    Node *T2 = q->right;

    if (q->right != nullptr) {
        q->right->parent = p;
    }

    p->left = T2;
    q->right = p;

    q->parent = p->parent;
    p->parent = q;

    if (q->parent != nullptr) {
        if (p->keyValuePair.first < q->parent->keyValuePair.first) {
            q->parent->left = q;
        }else{
            q->parent->right = q;
        }
    }

    p = q;
   
    fixHeight(p->left);
    fixHeight(p->right);
    fixHeight(p);
    fixHeight(p->parent);

    return p;
}

BinarySearchTree::Node * BinarySearchTree::leftRotation(Node *q) {
    Node *p = q->right;
    Node *T2 = p->left;

    q->right = T2;

    if (p->left != nullptr) {
        p->left->parent = q;
    }
    p->left = q;

    p->parent = q->parent;
    q->parent = p;

    if (p->parent != nullptr) {
        if (q->keyValuePair.first < p->parent->keyValuePair.first) {
            p->parent->left = p;
        }else{
            p->parent->right = p;
        }
    }

    fixHeight(q->left);
    fixHeight(q->right);
    fixHeight(q);
    fixHeight(q->parent);

    return p;
}

BinarySearchTree::Node *BinarySearchTree::insertToNode(Node *parent, Node *root, const Key& key, const Value& value) {
    if (root == nullptr) {
        Node *node = new Node(key, value, parent);
        return node;
    }
    if (key < root->keyValuePair.first) {
        root->left = insertToNode(root, root->left, key, value);
    }else if (key > root->keyValuePair.first) {
        root->right = insertToNode(root, root->right, key, value);
    }else if (key == root->keyValuePair.first) {
        if (value < root->keyValuePair.second) {
            root->left = insertToNode(root, root->left, key, value);
        }else if (value > root->keyValuePair.second) {
            root->right = insertToNode(root, root->right, key, value);
        }
    }

    int balance = balanceFactor(root);

    // Left Left Case

    if (balance > 1 && key < root->left->keyValuePair.first) {
        return rightRotation(root);
    }

    // Right Right Case

    if (balance < -1 && key > root->right->keyValuePair.first) {
        return leftRotation(root);
    }

    // Left Right Case

    if (balance > 1 && key > root->left->keyValuePair.first){
        root->left = leftRotation(root->left);
        return rightRotation(root);
    }
    
    // Right Left Case

    if (balance < -1 && key < root->right->keyValuePair.first){
        root->right = rightRotation(root->right);
        return leftRotation(root);
    }

    fixHeight(root);

    return root;
}

void BinarySearchTree::insert(const Key& key, const Value& value) {
    _root = insertToNode(nullptr, _root, key, value);
    _size++;
}

BinarySearchTree::Node * BinarySearchTree::deleteANode(Node *root, const Key& key, bool & deletedTheNode) {
    if (root == nullptr) {
        return root;
    }
    if (key < root->keyValuePair.first) {
        root->left = deleteANode(root->left, key, deletedTheNode);
    }else if (key > root->keyValuePair.first) {
        root->right = deleteANode(root->right, key, deletedTheNode);
    }else{
        if ((root->left == nullptr) && (root->right == nullptr)) {
            if (root->parent != nullptr) {
                if (root->parent->keyValuePair.first < root->keyValuePair.first) {
                    root->parent->right = nullptr;
                }else if (root->parent->keyValuePair.first > root->keyValuePair.first) {
                    root->parent->left = nullptr;
                }else if (root->parent->keyValuePair.first == root->keyValuePair.first) {
                    if (root->parent->keyValuePair.second < root->keyValuePair.second) {
                        root->parent->right = nullptr;
                    }else if (root->parent->keyValuePair.second > root->keyValuePair.second) {
                        root->parent->left = nullptr;
                    }
                }
                fixHeight(root->parent);
            }
            delete root;
            root = nullptr;
            deletedTheNode = true;
        }else if ((root->left != nullptr) && (root->right == nullptr)) {
            Node *tmp = root->left;
            if (root->parent != nullptr) {
                if (root->parent->keyValuePair.first < root->keyValuePair.first) {
                    root->parent->right = root->left;
                }else if (root->parent->keyValuePair.first > root->keyValuePair.first) {
                    root->parent->left = root->left;
                }else if (root->parent->keyValuePair.first == root->keyValuePair.first) {
                    if (root->parent->keyValuePair.second < root->keyValuePair.second) {
                        root->parent->right = root->left;
                    }else if (root->parent->keyValuePair.second > root->keyValuePair.second) {
                        root->parent->left = root->left;
                    }
                }
                fixHeight(root->parent);
            }
            root->left->parent = root->parent;
            delete root;
            root = tmp;
            deletedTheNode = true;
        }else if ((root->left == nullptr) && (root->right != nullptr)) {
            Node *tmp = root->right;
            if (root->parent != nullptr) {
                if (root->parent->keyValuePair.first < root->keyValuePair.first) {
                    root->parent->right = root->right;
                }else if (root->parent->keyValuePair.first > root->keyValuePair.first) {
                    root->parent->left = root->right;
                }else if (root->parent->keyValuePair.first == root->keyValuePair.first) {
                    if (root->parent->keyValuePair.second < root->keyValuePair.second) {
                        root->parent->right = root->right;
                    }else if (root->parent->keyValuePair.second > root->keyValuePair.second) {
                        root->parent->left = root->right;
                    }
                }
                fixHeight(root->parent);
            }
            root->right->parent = root->parent;
            delete root;
            root = tmp;
            deletedTheNode = true;
        }else{
            Node * tmp = tree_minimum(root->right);
            root->keyValuePair = tmp->keyValuePair;
            root->right = deleteANode(root->right, tmp->keyValuePair.first, deletedTheNode);
        }
    }

    if (root == nullptr) {
        return root;
    }

    fixHeight(root);
    
    int balance = balanceFactor(root);

    // Left Left Case

    if (balance > 1 && balanceFactor(root->left) >= 0) {
        return rightRotation(root);
    }

    // Left Right Case

    if (balance > 1 && balanceFactor(root->left) < 0) {
        root->left = leftRotation(root->left);
        return rightRotation(root);
    }

    // Right Right Case

    if (balance < -1 && balanceFactor(root->right) <= 0) {
        return leftRotation(root);
    }

    // Right Left Case

    if (balance < -1 && balanceFactor(root->right) > 0){
        root->right = rightRotation(root->right);
        return leftRotation(root);
    }

    return root;
}

void BinarySearchTree::erase(const Key& key) {
    bool deletedTheNode = false;

    _root = deleteANode(_root, key, deletedTheNode);
    if (deletedTheNode) {
        _size--;
    }

    while (deletedTheNode) {
        deletedTheNode = false;
        _root = deleteANode(_root, key, deletedTheNode);
        if (deletedTheNode) {
            _size--;
        }
    }
}

BinarySearchTree::ConstIterator BinarySearchTree::find(const Key& key) const {
    Node *node = tree_find(_root, key);
    ConstIterator iter(node);
    return iter; 
}

BinarySearchTree::Iterator BinarySearchTree::find(const Key& key) {
    Node *node = tree_find(_root, key);
    Iterator iter(node);
    return iter; 
}

std::pair<BinarySearchTree::Iterator, BinarySearchTree::Iterator> BinarySearchTree::equalRange(const Key& key) {
    Iterator start = find(key);
    Iterator end = start;
    while (end->first == key) {
        end++;
    }
    return std::pair<BinarySearchTree::Iterator, BinarySearchTree::Iterator> {start, end};
}

std::pair<BinarySearchTree::ConstIterator, BinarySearchTree::ConstIterator> BinarySearchTree::equalRange(const Key& key) const {
    ConstIterator start = find(key);
    ConstIterator end = start;
    while (end->first == key) {
        end++;
    }
    return std::pair<BinarySearchTree::ConstIterator, BinarySearchTree::ConstIterator> {start, end};
}

BinarySearchTree::ConstIterator BinarySearchTree::min(const Key& key) const {
    Node *found_tree = tree_find(_root, key);
    ConstIterator iter(tree_minimum(found_tree));
    return iter;
}

BinarySearchTree::ConstIterator BinarySearchTree::max(const Key& key) const {
    Node *found_tree = tree_find(_root, key);
    ConstIterator iter(tree_maximum(found_tree));
    return iter;
}

BinarySearchTree::Iterator BinarySearchTree::begin() {
    Iterator iter(tree_minimum(_root));
    return iter;
}

BinarySearchTree::Iterator BinarySearchTree::end() {
    Iterator iter(tree_maximum(_root));
    return iter;
}

BinarySearchTree::ConstIterator BinarySearchTree::cbegin() const {
    ConstIterator iter(tree_minimum(_root));
    return iter;
}

BinarySearchTree::ConstIterator BinarySearchTree::cend() const {
    ConstIterator iter(tree_maximum(_root));
    return iter;
}

size_t BinarySearchTree::size() const {
    return _size;
}

// использование бесспорной надобности

void BinarySearchTree::preorderPrint(Node *root) {
    if (root == nullptr) {
        return;
    }
    std::cout << root->keyValuePair.first << ' ' << root->keyValuePair.second << '\n';
    preorderPrint(root->left);
    preorderPrint(root->right);
}

void BinarySearchTree::preorderWalk(Node *root, std::vector<Key> & vecKey, std::vector<Value> & vecVal) {
    if (root == nullptr) {
        return;
    }
    vecKey.push_back(root->keyValuePair.first);
    vecVal.push_back(root->keyValuePair.second);
    preorderWalk(root->left, vecKey, vecVal);
    preorderWalk(root->right, vecKey, vecVal);
}

void BinarySearchTree::inorderPrint(Node *root) {
    if (root == nullptr) {
        return;
    }
    inorderPrint(root->left);
    std::cout << root->keyValuePair.first << ' ' << root->keyValuePair.second << '\n';
    inorderPrint(root->right);
}

void BinarySearchTree::postorderPrint(Node *root) {
    if (root == nullptr) {
        return;
    }
    postorderPrint(root->left);
    postorderPrint(root->right);
    std::cout << root->keyValuePair.first << ' ';
}

BinarySearchTree::Node * BinarySearchTree::tree_minimum(Node *root) {
    while (root->left != nullptr) {
        root = root->left;
    }
    return root;
}

BinarySearchTree::Node * BinarySearchTree::tree_minimum(Node *root) const{
    while (root->left != nullptr) {
        root = root->left;
    }
    return root;
}

BinarySearchTree::Node * BinarySearchTree::tree_maximum(Node *root) {
    while (root->right != nullptr) {
        root = root->right;
    }
    return root;
}

BinarySearchTree::Node * BinarySearchTree::tree_maximum(Node *root) const {
    while (root->right != nullptr) {
        root = root->right;
    }
    return root;
}

BinarySearchTree::Node * BinarySearchTree::tree_successor(Node *x) {
    if (x->right != nullptr) {
        return tree_minimum(x->right);
    }
    Node *y = x->parent;
    while ((y != nullptr) && (x == y->right)) {
        x = y;
        y = y->parent;
    }
    return y;
}

BinarySearchTree::Node * BinarySearchTree::tree_predecessor(Node *x) {
    if (x->left != nullptr) {
        return tree_maximum(x->left);
    }
    Node *y = x->parent;
    while ((y != nullptr) && (x == y->left)) {
        x = y;
        y = y->parent;
    }
    return y;
}

BinarySearchTree::Node * BinarySearchTree::tree_find(Node *root, const Key& key) {
    if (root == nullptr) {
        return nullptr;
    }
    if (key == root->keyValuePair.first) {
        return root;
    }else if (key < root->keyValuePair.first) {
        return tree_find(root->left, key);
    }
    return tree_find(root->right, key);
}

BinarySearchTree::Node * BinarySearchTree::tree_find(Node *root, const Key& key) const {
    if (root == nullptr) {
        return nullptr;
    }
    if (key == root->keyValuePair.first) {
        return root;
    }else if (key < root->keyValuePair.first) {
        return tree_find(root->left, key);
    }
    return tree_find(root->right, key);
}

void BinarySearchTree::tree_delete(Node *root) {
    // postorder traversal deletion

    if (root == nullptr) {
        return;
    }
    tree_delete(root->left);
    tree_delete(root->right);

    bool deletedTheNode = false;

    if (root) {
        root = deleteANode(root, root->keyValuePair.first, deletedTheNode);
    }
}

BinarySearchTree::Node * BinarySearchTree::tree_copy(Node *root) {
    if (root == nullptr) {
        return nullptr;
    }
    Node *tmp = new Node(root->keyValuePair.first, root->keyValuePair.second);
    tmp->height = root->height;
    tmp->left = tree_copy(root->left);
    if (tmp->left) {
        tmp->left->parent = tmp;
    }
    tmp->right = tree_copy(root->right);
    if (tmp->right) {
        tmp->right->parent = tmp;
    }
    return tmp;
}

// int main() {
//     BinarySearchTree tree;
//     tree.insert(1, 2);
//     tree.insert(1, 3);
//     tree.insert(1, 4);
//     tree.insert(1, 1);
//     tree.insert(1121, 1);
//     tree.insert(10, 1);
//     tree.insert(1324, 1);
//     tree.insert(32451, 1);
//     tree.insert(1458, 1);
//     tree.insert(1213, 1);
//     tree.insert(1890, 1);
//     tree.insert(1234, 1);
//     tree.insert(2341, 1);
//     // tree.inorderPrint(tree._root);
//     // tree.preorderPrint(tree._root);
//     // std::cout << tree._size << '\n';
//     // std::cout << tree._root->height << '\n';
//     // tree.erase(1);
//     // tree.erase(1);
//     // tree.erase(1);
//     // tree.erase(1);
//     // tree.erase(1121);
//     // tree.erase(10);
//     // tree.erase(1324);
//     // tree.erase(32451);
//     // tree.erase(1458);
//     // tree.erase(1213);
//     // tree.erase(1890);
//     // tree.erase(1234);
//     // tree.erase(2341);


//     // tree.erase(32451);
//     // tree.erase(1);
//     // tree.erase(1213);
//     // tree.erase(1);
//     // tree.erase(1121);
//     // tree.erase(1);
//     // tree.erase(10);
//     // tree.erase(1458);
//     // tree.erase(1);
//     // tree.erase(1324);
//     // tree.erase(2341);
//     // tree.erase(1890);
//     // std::cout <<tree._root << ' ' << tree._root->keyValuePair.first << '\n';
//     // tree.erase(1234);
//     // std::cout << tree._root << ' ' << tree._size << '\n';
//     // // std::cout << tree._root << ' ' << tree._root->keyValuePair.first << '\n';
//     // // tree.inorderPrint(tree._root);
//     // // std::cout << tree._size << '\n';

//     // tree.insert(1, 1);
//     // tree.insert(1, 2);
//     // tree.insert(1, 3);
//     // tree.insert(1, 4);
//     // tree.inorderPrint(tree._root);
//     // std::cout << tree._root << ' ' << tree._size << '\n';

//     tree.inorderPrint(tree._root);
//     // tree.erase(32451);
//     // tree.erase(1);
//     // tree.erase(1213);
//     // tree.erase(1121);
//     // tree.erase(10);
//     // tree.erase(1458);
//     // tree.erase(1324);
//     // tree.erase(2341);
//     // tree.erase(1890);
//     // tree.erase(1234);
//     std::cout << '\n';
//     // std::cout << tree._root << ' ' << tree._size << '\n';
//     std::cout << '\n';
//     // tree.insert(1, 2);
//     // tree.insert(1, 3);
//     // tree.insert(1, 4);
//     // tree.insert(1, 1);
//     // tree.insert(1121, 1);
//     // tree.insert(10, 1);
//     // tree.insert(1324, 1);
//     // tree.insert(32451, 1);
//     // tree.insert(1458, 1);
//     // tree.insert(1213, 1);
//     // tree.insert(1890, 1);
//     // tree.insert(1234, 1);
//     // tree.insert(2341, 1);
//     // tree.inorderPrint(tree._root);
//     // BinarySearchTree::Iterator iter(tree._root);
//     std::cout << '\n';
//     // std::cout << iter->first << ' ' << iter->second << '\n';
//     // std::cout << tree.equalRange(1).first->first << ' ' << tree.equalRange(1).second->first << '\n';
//     BinarySearchTree new_tree(tree);
//     std::cout << '\n';
//     std::cout << new_tree._root << ' ' << new_tree._size << '\n';
//     new_tree.inorderPrint(new_tree._root);
//     return 0;
// }