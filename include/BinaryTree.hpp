
// Copyright 2019 Dan Kokin d_kokin@inbox.ru
#ifndef INCLUDE_SET_HPP_
#define INCLUDE_SET_HPP_
#include <exception>
#include <functional>
#include <iterator>
#include <utility>

template <class T, class Cmp = std::less<>>
class set {
private:
    enum Color {
        RED, BLACK
    };

    struct Node {
        Node *Parent;
        Node *Left;
        Node *Right;
        Color color;
        T key;
        Node() : Parent(nullptr), Left(nullptr),
                 Right(nullptr), color(BLACK), key() {}
        Node(Node *oth, Node *parent, Node *TNUll, Node* oldTNUll)
                : Parent(parent), color(oth->color), key(oth->key) {
            Left = (oth->Left != oldTNUll) ?
                   new Node(oth->Left, this, TNUll, oldTNUll) : TNUll;
            Right = (oth->Right != oldTNUll) ?
                    new Node(oth->Right, this, TNUll, oldTNUll) : TNUll;
        }
        explicit Node(const T& val) {
            Parent = nullptr;
            Left = nullptr;
            Right = nullptr;
            color = RED;
            key = val;
        }
    }* root;
    Node* TNULL;
    Cmp cmp;
    size_t size_;

public:
    class iterator : public std::iterator<std::random_access_iterator_tag, T> {
    private:
        friend class set;
        Node *pNode;
        iterator() : pNode(nullptr) {}
        Node *TreeMin(Node *node) {
            Node* it = node;
            while (it->Left->Left != nullptr) {
                it = it->Left;
            }
            return it;
        }
        Node *TreeMax(Node *node) {
            Node* it = node;
            while (it->Right->Right != nullptr) {
                it = it->Right;
            }
            return it;
        }

    public:
        explicit iterator(Node *pointer) : pNode(pointer) {}

        iterator(const iterator &iter) : pNode(iter.pNode) {}

        iterator& operator++() {
            if (pNode->Right->Right != nullptr) {
                pNode = pNode->Right;
                pNode = TreeMin(pNode);
                return *this;
            } else {
                while (pNode->Parent->Right == pNode) {
                    pNode = pNode->Parent;
                }
                pNode = pNode->Parent;
                return *this;
            }
        }
        iterator& operator--() {
            if (pNode->Left->Left != nullptr) {
                pNode = pNode->Left;
                pNode = TreeMax(pNode);
                return *this;
            } else {
                while (pNode->Parent->Left == pNode) {
                    pNode = pNode->Parent;
                }
                pNode = pNode->Parent;
                return *this;
            }
        }
        T operator*() const {
            return pNode->key;
        }
        iterator &operator=(iterator &rhs) {
            if (this != &rhs) {
                pNode = rhs.pNode;
            }
            return *this;
        }
        bool operator==(const iterator &rhs) const {
            return pNode == rhs.pNode;
        }
        bool operator!=(const iterator &rhs) const {
            return pNode != rhs.pNode;
        }
    };
    set() {
        TNULL = new Node;
        TNULL->Left = nullptr;
        TNULL->Right = nullptr;
        TNULL->color = BLACK;
        root = TNULL;
        size_ = 0;
    }
    ~set() {
        if (root != nullptr) {
            if (root->Parent != nullptr) {
                delete root->Parent;
            }
            if (root != TNULL) {
                DeleteTree(root);
            }
            root = nullptr;
        }
        if (TNULL != nullptr) {
            delete TNULL;
            TNULL = nullptr;
        }
    }
    set(const set &oth)
            : size_(oth.size_) {
        TNULL = new Node;
        TNULL->Left = nullptr;
        TNULL->Right = nullptr;
        TNULL->color = BLACK;
        root = (oth.root != oth.TNULL) ?
               new Node(oth.root, nullptr, TNULL, oth.TNULL) : TNULL;
    }
    set& operator=(const set& rhs) {
        if (this != &rhs) {
            delete root;
            this->size_ = rhs.size_;
            TNULL = new Node;
            TNULL->Left = nullptr;
            TNULL->Right = nullptr;
            TNULL->color = BLACK;
            root = (rhs.root != rhs.TNULL) ?
                   new Node(rhs.root, nullptr, TNULL, rhs.TNULL) : TNULL;
        }
        return *this;
    }
    void insert(const T& val) {
        Node* newEl = new Node(val);
        newEl->Left = TNULL;
        newEl->Right = TNULL;
        ++size_;
        Node* parent = nullptr;
        Node* counter = root;
        while (counter != TNULL) {
            parent = counter;
            if (cmp(newEl->key, counter->key)) {
                counter = counter->Left;
            } else if (!cmp(newEl->key, counter->key)) {
                counter = counter->Right;
            } else if (newEl->key == counter->key) {
                return;
            }
        }
        newEl->Parent = parent;
        if (parent == nullptr) {
            root = newEl;
        } else if (cmp(newEl->key, parent->key)) {
            parent->Left = newEl;
        } else {
            parent->Right = newEl;
        }
        if (newEl->Parent == nullptr){
            newEl->color = BLACK;
            return;
        }
        if (newEl->Parent->Parent == nullptr) {
            return;
        }
        rb_insert_fixup(newEl);
    }
    void erase(const T& val) {
        Node* delete_node = find(root, val);
        if (delete_node == TNULL) {
            return;
        }
        Node* instead = delete_node;
        Color var_orig_color = instead->color;
        Node* replace_val = nullptr;
        if (delete_node->Left == TNULL) {
            replace_val = delete_node->Right;
            transplant(delete_node, delete_node->Right);
        } else if (delete_node->Right == TNULL) {
            replace_val = delete_node->Left;
            transplant(delete_node, delete_node->Left);
        } else {
            instead = tree_min(delete_node->Right);
            var_orig_color = instead->color;
            replace_val = instead->Right;
            if (instead->Parent == delete_node) {
                replace_val->Parent = instead;
            } else {
                transplant(instead, instead->Right);
                instead->Right = delete_node->Right;
                instead->Right->Parent = instead;
            }
            transplant(delete_node, instead);
            instead->Left = delete_node->Left;
            instead->Left->Parent = instead;
            instead->color = delete_node->color;
        }
        --size_;
        delete delete_node;
        if (var_orig_color == BLACK) {
            rb_delete_fixup(replace_val);
        }
    }
    iterator begin() {
        return iterator(tree_min(root));
    }
    iterator end() {
        if (root->Parent == nullptr) {
            Node* end = new Node;
            root->Parent = end;
            end->Left = root;
            return iterator(end);
        }
        return iterator(root->Parent);
    }
    iterator find(const T &value) {
        auto it = root;
        while (it) {
            if (value == it->key) {
                return iterator(it);
            }
            it = (cmp(value, it->key)) ? it->Left : it->Right;
        }
        return end();
    }
    size_t size() {
        return size_;
    }
    bool empty() {
        return (size_ == 0);
    }
    bool has(const T &value) const {
        Node* node = root;
        if (node == nullptr) {
            return false;
        }
        while (node) {
            if (node->key == value) {
                return node;
            } else if (cmp(node->key, value)) {
                node = node->Right;
            } else {
                node = node->Left;
            }
        }
        return false;
    }
    void clear() {
        if (root != nullptr)
            DeleteTree(root);
        if (TNULL != nullptr)
            delete TNULL;
        root = nullptr;
        TNULL = nullptr;
        size_ = 0;
    }


private:
    void rotate_left(Node* node) {
        Node* swap_node = node->Right;
        node->Right = swap_node->Left;
        if (swap_node->Left != TNULL)
            swap_node->Left->Parent = node;
        swap_node->Parent = node->Parent;
        if (node->Parent == nullptr) {
            root = swap_node;
        } else if (node == node->Parent->Left) {
            node->Parent->Left = swap_node;
        } else {
            node->Parent->Right = swap_node;
        }
        swap_node->Left = node;
        node->Parent = swap_node;
    }
    void rotate_right(Node *node) {
        Node* swap_node = node->Left;
        node->Left = swap_node->Right;
        if (swap_node->Right != TNULL)
            swap_node->Right->Parent = node;
        swap_node->Parent = node->Parent;
        if (node->Parent == nullptr) {
            root = swap_node;
        } else if (node == node->Parent->Left) {
            node->Parent->Left = swap_node;
        } else {
            node->Parent->Right = swap_node;
        }
        swap_node->Right = node;
        node->Parent = swap_node;
    }
    void rb_insert_fixup(Node* node) {
        Node* uncle;
        while (node->Parent->color == RED) {
            if (node->Parent == node->Parent->Parent->Right) {
                uncle = node->Parent->Parent->Left;
                if (uncle->color == RED) {
                    uncle->color = BLACK;
                    node->Parent->color = BLACK;
                    node->Parent->Parent->color = RED;
                    node = node->Parent->Parent;
                } else {
                    if (node == node->Parent->Left) {
                        node = node->Parent;
                        rotate_right(node);
                    }
                    node->Parent->color = BLACK;
                    node->Parent->Parent->color = RED;
                    rotate_left(node->Parent->Parent);
                }
            } else {
                uncle = node->Parent->Parent->Right;
                if (uncle->color == RED) {
                    uncle->color = BLACK;
                    node->Parent->color = BLACK;
                    node->Parent->Parent->color = RED;
                    node = node->Parent->Parent;
                } else {
                    if (node == node->Parent->Right) {
                        node = node->Parent;
                        rotate_left(node);
                    }
                    node->Parent->color = BLACK;
                    node->Parent->Parent->color = RED;
                    rotate_right(node->Parent->Parent);
                }
            }
            if (node == root) {
                break;
            }
        }
        root->color = BLACK;
    }
    void transplant(Node* first, Node* second) {
        if (first->Parent == nullptr) {
            root = second;
        } else if (first == first->Parent->Left) {
            first->Parent->Left = second;
        } else {
            first->Parent->Right = second;
        }
        second->Parent = first->Parent;
    }
    Node* tree_min(Node* node) {
        auto n = node;
        while (n->Left->Left != nullptr) {
            n = n->Left;
        }
        return n;
    }
    void rb_delete_fixup(Node* node) {
        Node* uncle;
        while (node != root && node->color == BLACK) {
            if (node == node->Parent->Left) {
                uncle = node->Parent->Right;
                if (uncle->color == RED) {
                    uncle->color = BLACK;
                    node->Parent->color = RED;
                    rotate_left(node->Parent);
                    uncle = node->Parent->Right;
                }
                if (uncle->Left->color == BLACK &&
                    uncle->Right->color == BLACK) {
                    uncle->color = RED;
                    node = node->Parent;
                } else {
                    if (uncle->Right->color == BLACK) {
                        uncle->Left->color = BLACK;
                        uncle->color = RED;
                        rotate_right(uncle);
                        uncle = node->Parent->Right;
                    }
                    uncle->color = node->Parent->color;
                    node->Parent->color = BLACK;
                    uncle->Right->color = BLACK;
                    rotate_left(node->Parent);
                    node = root;
                }
            } else {
                uncle = node->Parent->Left;
                if (uncle->color == RED) {
                    uncle->color = BLACK;
                    node->Parent->color = RED;
                    rotate_right(node->Parent);
                    uncle = node->Parent->Left;
                }
                if (uncle->Left->color == BLACK &&
                    uncle->Right->color == BLACK) {
                    uncle->color = RED;
                    node = node->Parent;
                } else  {
                    if (uncle->Left->color == BLACK) {
                        uncle->Right->color = BLACK;
                        uncle->color = RED;
                        rotate_left(uncle);
                        uncle = node->Parent->Left;
                    }
                    uncle->color = node->Parent->color;
                    node->Parent->color = BLACK;
                    uncle->Left->color = BLACK;
                    rotate_right(node->Parent);
                    node = root;
                }
            }
        }
        node->color = BLACK;
    }
    Node* find(Node* node, const T& val) {
        while (node != TNULL) {
            if (node->key == val) {
                return node;
            } else if (val < node->key) {
                node = node->Left;
            } else {
                node = node->Right;
            }
        }
        return nullptr;
    }
    void DeleteTree(Node* node) {
        if (node == nullptr) {
            return;
        }
        if (node->Left != TNULL) {
            DeleteTree(node->Left);
        }
        if (node->Right != TNULL) {
            DeleteTree(node->Right);
        }
        delete node;
    }
};

#endif // INCLUDE_SET_HPP_
