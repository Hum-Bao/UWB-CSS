#ifndef BSTREE_H_
#define BSTREE_H_
#include <iostream>
#include <string>
#include <vector>
#include "account.h"

class BSTree {
public:
    BSTree();
    BSTree(const BSTree& tree);
    ~BSTree();

    bool Insert(Account* account);

    // retrieve object, first parameter is the ID of the account
    // second parameter holds pointer to found object, NULL if not found
    bool Retrieve(const int& account_id, Account*& account) const;

    // Delete object, first parameter is the ID of the account
    // second parameter holds pointer to found object, NULL if not found
    bool Delete(const int& account_id, Account*& account);

    BSTree& operator=(const BSTree& tree);

    // displays the contents of a tree to cout;
    // you could also overload operator<<
    void Display() const;
    void DisplayTree() const;
    int Size() const;

private:
    struct Node {
        Node() : p_acct(nullptr), left(nullptr), right(nullptr) {}

        Node(Account* account, Node* left, Node* right)
            : p_acct(account), left(left), right(right) {}

        explicit Node(Account* account)
            : p_acct(account), left(nullptr), right(nullptr) {}

        Node(const Node& node)
            : p_acct(nullptr), left(nullptr), right(nullptr) {
            if (node.p_acct != nullptr) {
                p_acct = new Account(*(node.p_acct));
            }
            if (node.left != nullptr) {
                left = new Node(*(node.left));
            }
            if (node.right != nullptr) {
                right = new Node(*(node.right));
            }
        }

        Account* p_acct;
        Node* right;
        Node* left;
    };

    Node* root_;
    int size_ = 0;

    bool RecursiveInsert(Account* account, Node*& root);
    void RecursiveDisplay(const Node* node) const;
    void RecursiveDisplayTree(const std::string& prefix, const Node* node,
                              bool left_node) const;
    bool RecursiveRetrieve(const int& account_id, Account*& account,
                           Node* node) const;
    bool RecursiveDelete(Node* node, const int& account_id, Account*& account);
    Node* GetMinNode(Node* node) const;
    void RecursiveDestructor(Node* node);
    Node* Copy(const Node* node) const;
};
#endif
