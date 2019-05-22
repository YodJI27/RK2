
#ifndef INCLUDE_HEADER_HPP__
#define INCLUDE_HEADER_HPP__

#include <fstream>
#include <vector>
#include <queue>

template <class T>
class BinarySearchTree
{
public:
    struct Node
    {
        Node* Left;
        Node* Right;
        T Value;
    };
    Node * Root;
public:
    void DeleteTree(Node* treeRoot)
    {
        if (treeRoot != nullptr)
        {
            DeleteTree(treeRoot->Left);
            DeleteTree(treeRoot->Right);
            delete treeRoot;
        }
    }
    BinarySearchTree()
    {
        Root = nullptr;
    }
    ~BinarySearchTree()
    {
        DeleteTree(Root);
    }
    void Add(const T& value)
    {
        BinarySearchTree<T>::Node* newElement =
            new BinarySearchTree<T>::Node{ nullptr, nullptr, value };
        if (Root == nullptr)
        {
            Root = newElement;
            return;
        }
        BinarySearchTree<T>::Node* it = Root;
        BinarySearchTree<T>::Node* parent = nullptr;
        while (it)
        {
            parent = it;
            if (it->Value == value) {
                return;
            } else if (it->Value > value) {
                it = it->Left;
            } else if (it->Value < value) {
                it = it->Right;
            }
        }
        if (parent->Value > value)
        {
            parent->Left = newElement;
        } else {
            parent->Right = newElement;
        }
    }
    Node* Find(const T& value)
    {
        BinarySearchTree<T>::Node* it = Root;
        if (it == nullptr) { return nullptr; }
        BinarySearchTree<T>::Node* parent = nullptr;
        while (it)
        {
            parent = it;
            if (it->Value == value) {
                return it;
            } else if (it->Value > value) {
                it = it->Left;
            } else if (it->Value < value) {
                it = it->Right;
            }
        }
        if (parent->Value == value)
        {
            return parent;
        } else {
            return nullptr;
        }
    }
    Node* FindParent(const T& value)
    {
        if (Root->Value == value)
        {
            return nullptr;
        }
        BinarySearchTree<T>::Node* it = Root;
        if (it == nullptr) { return nullptr; }
        BinarySearchTree<T>::Node* parent = it;
        if (it->Value > value)
        {
            it = it->Left;
        } else {
            it = it->Right;
        }
        while (it)
        {
            if (it->Value == value) {
                return parent;
            } else if (it->Value > value) {
                parent = it;
                it = it->Left;
            } else if (it->Value < value) {
                parent = it;
                it = it->Right;
            }
        }
        return parent;
    }
    void Remove(Node* removedElement)
    {
        if (removedElement == nullptr)
        {
            return;
        }
        if (this->Find(removedElement->Value) == nullptr)
        {
            return;
        }
        if (removedElement == Root)
        {
            auto minElement = Root->Right;
            while (minElement->Left != nullptr)
            {
                minElement = minElement->Left;
            }
            auto parentMinElemnt = this->FindParent(minElement->Value);
            parentMinElemnt->Left = nullptr;
            minElement->Right = removedElement->Right;
            minElement->Left = removedElement->Left;
            Root = minElement;
            delete removedElement;
            return;
        }
        BinarySearchTree<T>::Node* it =
            this->FindParent(removedElement->Value);
        if (removedElement->Right == nullptr &&
            removedElement->Left == nullptr)
        {
            if (it->Right == removedElement)
            {
                it->Right = nullptr;
            } else {
                it->Left = nullptr;
            }
        } else if ((removedElement->Right != nullptr) &&
            (removedElement->Left != nullptr)) {
            auto parentRemovedElement =
                this->FindParent(removedElement->Value);
            auto minElement = removedElement->Right;
            while (minElement->Left != nullptr)
            {
                minElement = minElement->Left;
            }
            auto parentMinElement =
                this->FindParent(minElement->Value);
            parentMinElement->Left = nullptr;
            minElement->Left = removedElement->Left;
            if (removedElement->Right != minElement) {
                minElement->Right = removedElement->Right;
            } else {
                minElement->Right = nullptr;
            }
            if (parentRemovedElement->Right == removedElement)
            {
                parentRemovedElement->Right = minElement;
            } else {
                parentRemovedElement->Left = minElement;
            }
        } else if (removedElement->Right != nullptr ||
            removedElement->Left != nullptr) {
            if (it->Left == removedElement)
            {
                if (removedElement->Left != nullptr)
                {
                    it->Left = removedElement->Left;
                } else {
                    it->Left = removedElement->Right;
                }
            } else {
                if (removedElement->Left != nullptr)
                {
                    it->Right = removedElement->Left;
                } else {
                    it->Right = removedElement->Right;
                }
            }
        }
        delete removedElement;
    }
    friend std::ostream& operator<< (std::ostream& out,
        const BinarySearchTree<T>& tree)
    {
        if (tree.Root == nullptr)
        {
            return out;
        }
        std::queue<typename BinarySearchTree<T>::Node*> queue;
        queue.push(tree.Root);
        size_t endls = 1;
        size_t it = 0;
        while (!q.empty()) {
            typename BinarySearchTree<T>::Node* temp = queue.front();
            queue.pop();
            if (temp) {
                out << temp->Value << " ";
            } else {
                out << "  ";
            }
            ++it;
            if (it == endls)
            {
                endls *= 2;
                it = 0;
                out << std::endl;
            }
            if (temp) {
                queue.push(temp->Left);
                queue.push(temp->Right);
            }
        }
        return out;
    }
    size_t NodeDepth(Node* node)
    {
        if (node == nullptr)
        {
            return 0;
        }
        size_t depthLeft = NodeDepth(node->Left);
        size_t depthRight = NodeDepth(node->Right);
        return (depthLeft > depthRight) ?
            (depthLeft + 1) : (depthRight + 1);
    }
    friend size_t Depth(BinarySearchTree<T>& tree)
    {
        return tree.NodeDepth(tree.Root);
    }
};


template <typename T>
struct TreeNode
{
    TreeNode* Parent;
    TreeNode* Left;
    TreeNode* Right;
    T Value;
};

template<typename T>
bool IsBinarySearchTree(TreeNode<T>* root) {
    auto* p = root;
    if (p != nullptr) {
        if ((p->Left != nullptr) &&
            (p->Left->Value < p->Value)) {
            auto* pred = p->Left;
            if (pred != p) {
                return false;
            }
            IsBinarySearchTree(p->Left);
            IsBinarySearchTree(p->Right);
        } else if ((p->Left != nullptr) &&
            (p->Left->Value >= p->Value)) {
            return false;
        }
        if ((p->Right != nullptr) && (p->Right->Value > p->Value)) {
            auto* pred = p->Right;
            if (pred != p) {
                return false;
            }
            IsBinarySearchTree(p->Left);
            IsBinarySearchTree(p->Right);
        } else if ((p->Right != nullptr) &&
            (p->Right->Value <= p->Value)) {
            return false;
        }
    }
    return true;
}

#endif // INCLUDE_HEADER_HPP__
