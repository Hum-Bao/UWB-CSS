#include "bstree.h"
#include <string>
#include <vector>

BSTree::BSTree() : root_(nullptr) {}

BSTree::BSTree(const BSTree& tree) : root_(nullptr) {
    if (this != &tree) {
        this->~BSTree();
        root_ = Copy(tree.root_);
    }
}

void BSTree::RecursiveDestructor(Node* node) {
    if (node != nullptr) {
        RecursiveDestructor(node->left);
        RecursiveDestructor(node->right);
        delete node->p_acct;
        delete node;
        node = nullptr;
    }
}

BSTree::~BSTree() {
    RecursiveDestructor(root_);
    root_ = nullptr;
}

bool BSTree::Insert(Account* account) {
    return RecursiveInsert(account, root_);
}

bool BSTree::RecursiveInsert(Account* account, Node*& node) {
    if (node == nullptr) {
        node = new Node(account);
        size_++;
        return true;
    }
    if (account->GetID() == node->p_acct->GetID()) {
        return false;
    }
    if (account->GetID() < node->p_acct->GetID()) {
        return RecursiveInsert(account, node->left);
    }
    return RecursiveInsert(account, node->right);
}

// retrieve object, first parameter is the ID of the account
// second parameter holds pointer to found object, NULL if not found
bool BSTree::Retrieve(const int& account_id, Account*& account) const {
    return RecursiveRetrieve(account_id, account, root_);
}

bool BSTree::RecursiveRetrieve(const int& account_id, Account*& account,
                               Node* node) const {
    if (node == nullptr) {
        return false;
    }
    if (node->p_acct->GetID() == account_id) {
        account = node->p_acct;
        return true;
    }
    if (account_id < node->p_acct->GetID()) {
        return RecursiveRetrieve(account_id, account, node->left);
    }
    return RecursiveRetrieve(account_id, account, node->right);
}

BSTree::Node* BSTree::GetMinNode(Node* node) const {
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}

// Delete object, first parameter is the ID of the account
// second parameter holds pointer to found object, NULL if not found
bool BSTree::Delete(const int& account_id, Account*& account) {
    return RecursiveDelete(root_, account_id, account);
}

bool BSTree::RecursiveDelete(Node* node, const int& account_id,
                             Account*& account) {
    if (node == nullptr) {
        return false;
    }

    if (account_id < node->p_acct->GetID()) {
        return RecursiveDelete(node->left, account_id, account);
    } else if (account_id > node->p_acct->GetID()) {
        return RecursiveDelete(node->right, account_id, account);
    } else {
        account = node->p_acct;
        Node* temp;
        if (node->left == nullptr) {
            temp = node->right;
            delete node;
            node = temp;
        } else if (node->right == nullptr) {
            temp = node->left;
            delete node;
            node = temp;
        } else {
            Node* min_right = GetMinNode(node->right);
            node->p_acct = min_right->p_acct;
            RecursiveDelete(node->right, min_right->p_acct->GetID(), account);
        }
        size_--;
        return true;
    }
}

BSTree::Node* BSTree::Copy(const Node* node) const {
    if (node == nullptr) {
        return nullptr;
    }
    Node* temp = new Node(*node);
    temp->left = Copy(node->left);
    temp->right = Copy(node->right);
    return temp;
}

BSTree& BSTree::operator=(const BSTree& tree) {
    if (this != &tree) {
        this->~BSTree();
        root_ = Copy(tree.root_);
    }
    return *this;
}

// displays the contents of a tree to cout;
// you could also overload operator<<
void BSTree::Display() const {
    RecursiveDisplay(root_);
}

void BSTree::RecursiveDisplay(const Node* node) const {
    if (node != nullptr) {
        RecursiveDisplay(node->left);
        std::cout << *node->p_acct << std::endl;
        RecursiveDisplay(node->right);
    }
}

void BSTree::DisplayTree() const {
    RecursiveDisplayTree("", root_, false);
}

void BSTree::RecursiveDisplayTree(const std::string& prefix, const Node* node,
                                  bool left_node) const {
    if (node != nullptr) {
        std::cout << prefix;
        if (left_node) {
            std::cout << "├──";
        } else {
            std::cout << "└──";
        }
        std::cout << node->p_acct->GetID() << std::endl;
        RecursiveDisplayTree(prefix + (left_node ? "│   " : "    "), node->left,
                             true);
        RecursiveDisplayTree(prefix + (left_node ? "│   " : "    "),
                             node->right, false);
    }
}

int BSTree::Size() const {
    return size_;
}
