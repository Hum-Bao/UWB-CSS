#ifndef NODE_H_
#define NODE_H_

template <typename T>
struct Node {
    Node(T* data, Node<T>* prevNode, Node<T>* nextNode)
        : data(data), prev(prevNode), next(nextNode) {}

    Node(T* data)
        : Node(data, nullptr, nullptr) {}

    Node()
        : Node(nullptr, nullptr, nullptr) {}
    T* data;
    Node* next;
    Node* prev;
};
#endif
