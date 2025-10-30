/*
File name: threadedBST.h
Name: Mikhail Ermolenko, Mike Jansen, Tyler Tran
Date: December 11, 2021
This header file contains signatures for all functions and variables.
*/
#pragma once
#ifndef THREADEDBST_H
#define THREADEDBST_H
#include <iostream>
#include <vector>
using namespace std;

class Node {
    friend class ThreadedBST;

 public:
    Node();
    explicit Node(int val);
    Node(int val, Node* left, Node* right, bool thread);

 private:
    int value;
    Node* leftPtr = nullptr;
    Node* rightPtr = nullptr;
    bool isThread = false;  // points at thread or child
};

class ThreadedBST {
    // output overload
    friend ostream& operator<<(ostream& out, const ThreadedBST& bst);

 public:
    // Rebalances tree
    void rebalance();

    // Add val to tree
    void add(int value);

    // Get total number of nodes in tree
    int numberOfNodes() const;

    // assignment overload for copying
    ThreadedBST& operator=(const ThreadedBST& bst);
    // Equality operator overload
    bool operator==(const ThreadedBST& other) const;

    //default constructor
    ThreadedBST();

    // constructor
    // n equals the total number of nodes in the tree
    explicit ThreadedBST(int n);

    // copy constructor
    ThreadedBST(const ThreadedBST& tbst);

    // destructor
    ~ThreadedBST();

    // removes specific element
    Node* remove(int value);

    // iterator to do inorder traversal of the tree
    vector<int> inorderTraversal() const;

    // iterator to do preorder traversal of the tree
    vector<int> preorderTraversal() const;

    // iterator to do postorder traversal of the tree
    vector<int> postorderTraversal() const;

    // Copy constructor helper
    Node* CopyTree(ThreadedBST const& tbst);

 private:
    // Helper for add()
    void insert(int value);

    // Get height of tree
    int getHeight(Node* root);

    // Helper for equality operator
    bool isEqual(Node* node1, Node* node2) const;

    //clear helper function to recursively delete TBST
    void clearHelper(Node* node);

    // remove() method helper
    Node* removeHelper(Node* node, int value);

    // postorder traversal helper
    void postorderHelper(Node* node, vector<int>& postorder) const;

    // preorder traversal helper
    void preorderHelper(Node* node, vector<int>& preorder) const;

    // Get the leftmost node
    static Node* LeftMost(Node* node);

    // helper function to thread a tree
    void threadTree(Node* headPtr);

    // clear helper function to recursively delete TBST
    void clear(Node* node);

    // returns true if empty, false if not
    bool isEmpty() const;

    // searchs Tree for a node with specified value
    // returns Node if found, nullptr if not found
    Node* getEntry(int n) const;
    // helper function to build subtrees recursively
    // returns a Node* to make recursion possible
    Node* buildSubTree(const vector<int>& nums, int lower, int upper);
    // root pointer (top of tree)
    Node* headPtr = nullptr;
    // total number of nodes in the tree
    int totalNodes;
};
#endif
