#include "threadedBST.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

Node::Node()
    : value(-1), leftPtr(nullptr), rightPtr(nullptr), isThread(false) {}

Node::Node(int val)
    : value(val), leftPtr(nullptr), rightPtr(nullptr), isThread(false) {}

Node::Node(int val, Node* left, Node* right, bool thread)
    : value(val), leftPtr(left), rightPtr(right), isThread(thread) {}

//constructor
ThreadedBST::ThreadedBST() : headPtr(nullptr), totalNodes(0) {}

// constructor
// n equals the total number of nodes in the tree
ThreadedBST::ThreadedBST(int n) : totalNodes(n) {
    // vector for adding inputs
    vector<int> nums;
    for (int i = 1; i <= n; i++) {
        nums.push_back(i);
    }
    // assign headptr and start recursive call to build tree, then thread it
    headPtr = buildSubTree(nums, 0, nums.size() - 1);
    threadTree(headPtr);
}

// copy constructor
ThreadedBST::ThreadedBST(const ThreadedBST& tbst)
    : headPtr(CopyTree(tbst)), totalNodes(tbst.totalNodes) {
    cout << totalNodes << " | " << tbst.totalNodes;
}

// destructor
ThreadedBST::~ThreadedBST() {
    clear(headPtr);
}

// Assignment operator for copying
ThreadedBST& ThreadedBST::operator=(const ThreadedBST& bst) {
    headPtr = CopyTree(bst);
    totalNodes = bst.totalNodes;
    return *this;
}

// Copy constructor helper
Node* ThreadedBST::CopyTree(ThreadedBST const& tbst) {
    vector<int> inorder = tbst.inorderTraversal();
    Node* temp = buildSubTree(inorder, 0, inorder.size() - 1);
    threadTree(temp);
    return temp;
}

// Add val to tree
void ThreadedBST::add(int value) {
    insert(value);
    rebalance();
}

// Helper for add()
void ThreadedBST::insert(int value) {
    if (headPtr == nullptr) {
        headPtr = new Node(value);
        totalNodes = 1;
        return;
    }
    Node* current = headPtr;
    Node* parent = nullptr;
    while (current != nullptr) {
        parent = current;
        if (value < current->value) {
            if (current->leftPtr == nullptr) {
                current->leftPtr = new Node(value);
                current->leftPtr->rightPtr = current;
                current->leftPtr->isThread = true;
                totalNodes++;
                return;
            }
            current = current->leftPtr;
        } else if (value > current->value) {
            if (!current->isThread) {
                current = current->rightPtr;
            } else {
                Node* newNode = new Node(value);
                newNode->rightPtr = current->rightPtr;
                current->rightPtr = newNode;
                current->isThread = false;
                newNode->isThread = true;
                totalNodes++;
                return;
            }
        } else {
            return;
        }
    }
}

//Equality operator overload
bool ThreadedBST::operator==(const ThreadedBST& other) const {
    return isEqual(headPtr, other.headPtr);
}

//Helper for equality operator
bool ThreadedBST::isEqual(Node* node1, Node* node2) const {
    if (node1 == nullptr && node2 == nullptr) {
        return true;
    }

    if (node1 == nullptr || node2 == nullptr) {
        return false;
    }

    if (node1->value != node2->value) {
        return false;
    }

    return isEqual(node1->leftPtr, node2->leftPtr) &&
           isEqual(node1->rightPtr, node2->rightPtr);
}

//Get height of tree
int ThreadedBST::getHeight(Node* root) {
    int left, right;
    if (root == nullptr) {
        return -1;
    }
    left = getHeight(root->leftPtr);
    right = getHeight(root->rightPtr);
    left = left >= right ? left : right;
    return 1 + left;
}

// output overload
ostream& operator<<(ostream& out, const ThreadedBST& bst) {
    const vector<int> inorder = bst.inorderTraversal();
    for (int temp : inorder) {
        out << temp << " ";
    }
    return out;
}

// Get total number of nodes in tree
int ThreadedBST::numberOfNodes() const {
    vector<int> temp = inorderTraversal();
    return temp.size();
}

// Rebalances tree
void ThreadedBST::rebalance() {
    vector<int> temp = inorderTraversal();
    headPtr = buildSubTree(temp, 0, temp.size() - 1);
}

// adds threads to existing bst
void ThreadedBST::threadTree(Node* headPtr) {
    // Start from the leftmost node
    Node* currentNode = LeftMost(headPtr);

    // If the leftmost node has a right child, move to it
    if (currentNode->rightPtr != nullptr) {
        currentNode = currentNode->rightPtr;
    }

    // Traverse the tree and create threads when necessary
    while (currentNode != nullptr && currentNode->value < totalNodes) {
        if (currentNode->rightPtr == nullptr) {  // Thread needed
            // Create thread and follow it
            currentNode->rightPtr = getEntry(currentNode->value + 1);
            currentNode->isThread = true;
            currentNode = currentNode->rightPtr;
            // Move to the next node
            if (currentNode != nullptr) {
                currentNode = currentNode->rightPtr;
            }
        } else if (currentNode->leftPtr != nullptr) {
            // Explore left child/subtree
            currentNode = currentNode->leftPtr;
        } else {
            // Explore right child/subtree
            currentNode = currentNode->rightPtr;
        }
    }
}

// helper function to build subtrees recursively
// returns a Node* to make recursion possible
Node* ThreadedBST::buildSubTree(const vector<int>& nums, int lower, int upper) {
    if (lower > upper) {
        return nullptr;
    }
    int mid = (lower + upper) / 2;
    Node* root = new Node(nums[mid]);
    root->leftPtr = buildSubTree(nums, lower, mid - 1);
    root->rightPtr = buildSubTree(nums, mid + 1, upper);
    return root;
}

//clear helper function to recursively delete TBST
void ThreadedBST::clearHelper(Node* node) {
    if (node == nullptr) {
        return;
    }
    clearHelper(node->leftPtr);
    clearHelper(node->rightPtr);
    delete node;
}

//Recursively clears nodes from tree
void ThreadedBST::clear(Node* node) {
    clearHelper(headPtr);
    headPtr = nullptr;
}

// searches Tree for a node with specified value
// returns Node if found, nullptr if not found
Node* ThreadedBST::getEntry(int n) const {
    if (n == headPtr->value) {
        return headPtr;
    }
    Node* temp = headPtr;
    while (temp != nullptr) {
        if (temp->value == n) {
            break;
        }

        if (n < temp->value) {
            if (temp->leftPtr == nullptr) {
                break;
            }
            temp = temp->leftPtr;
        } else {
            if (temp->isThread) {
                break;
            }
            temp = temp->rightPtr;
        }
    }
    return temp;
}

// returns true if empty, false if not
bool ThreadedBST::isEmpty() const {
    return (headPtr == nullptr);
}

//Get the leftmost node
Node* ThreadedBST::LeftMost(Node* node) {
    if (node == nullptr) {
        return nullptr;
    }
    while (node->leftPtr != nullptr) {
        node = node->leftPtr;
    }
    return node;
}

//inorder traversal of tree using threads
vector<int> ThreadedBST::inorderTraversal() const {
    vector<int> inorder;
    if (headPtr == nullptr) {
        return inorder;
    }
    Node* temp = LeftMost(headPtr);

    while (temp != nullptr) {
        inorder.push_back(temp->value);
        if (temp->isThread) {
            temp = temp->rightPtr;
        } else {
            temp = LeftMost(temp->rightPtr);
        }
    }
    return inorder;
}

// preorder traversal using recursion
vector<int> ThreadedBST::preorderTraversal() const {
    vector<int> preorder;
    preorderHelper(headPtr, preorder);
    return preorder;
}

// preorder traversal helper
void ThreadedBST::preorderHelper(Node* node, vector<int>& preorder) const {
    if (node == nullptr) {
        return;
    }
    preorder.push_back(node->value);
    if (!node->isThread) {
        preorderHelper(node->leftPtr, preorder);
        preorderHelper(node->rightPtr, preorder);
    }
}

// postorder traversal helper
void ThreadedBST::postorderHelper(Node* node, vector<int>& postorder) const {
    if (node == nullptr) {
        return;
    }
    if (!node->isThread) {
        postorderHelper(node->leftPtr, postorder);
        postorderHelper(node->rightPtr, postorder);
    }
    postorder.push_back(node->value);
}

//postorder traversal using recursion
vector<int> ThreadedBST::postorderTraversal() const {
    vector<int> postorder;
    postorderHelper(headPtr, postorder);
    return postorder;
}

//remove() method helper
Node* ThreadedBST::removeHelper(Node* node, int value) {
    if (node == nullptr) {
        return nullptr;
    }

    if (value < node->value) {
        node->leftPtr = removeHelper(node->leftPtr, value);
    } else if (value > node->value) {
        node->rightPtr = removeHelper(node->rightPtr, value);
    } else {
        if (node->leftPtr == nullptr) {
            Node* temp = node->rightPtr;
            delete node;
            return temp;
        } else if (node->rightPtr == nullptr) {
            Node* temp = node->leftPtr;
            delete node;
            return temp;
        }

        // Node has two children, find inorder predecessor
        Node* temp = LeftMost(node->leftPtr);
        node->value = temp->value;
        node->leftPtr = removeHelper(node->leftPtr, temp->value);
    }
    return node;
}

//removes a node
Node* ThreadedBST::remove(int value) {
    Node* temp = removeHelper(headPtr, value);
    rebalance();
    return temp;
}
