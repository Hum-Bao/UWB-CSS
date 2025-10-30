#ifndef LIST342_H_
#define LIST342_H_
#include <fstream>
#include <iostream>
#include <string>

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

template <typename T>
class List342 {
public:
    // Tail = Last element added, Head = First element added
    List342();
    List342(List342& list);
    List342(const List342& list);
    ~List342();

    void AddToHead(T* val);
    bool RemoveHead(T& result);
    void DeleteHead();
    bool PeekHead(T& result) const;

    void AddToTail(T* val);
    bool RemoveTail(T& result);
    bool PeekTail(T& result) const;

    bool Peek(T target, T& result) const;

    bool Insert(T* obj);

    bool Remove(T target, T& result);

    int size() const;

    bool BuildList(string file_name);
    void DeleteList();

    bool Merge(List342& list1);

    template <typename H>
    friend bool operator==(const List342<H>& list1, const List342<H>& list2);

    template <typename H>
    friend bool operator!=(const List342<H>& list1, const List342<H>& list2);

    List342<T>& operator+=(List342<T> const& list1);

    template <typename H>
    friend List342<H> operator+(List342<H> list1, const List342<H>& list2);

    List342<T>& operator=(List342<T> const& list1);

    template <typename H>
    friend std::ostream& operator<<(std::ostream& os, const List342<H>& list);

private:
    Node<T>* head_;
    Node<T>* tail_;
    int size_;
};
#endif

template <typename T>
List342<T>::List342()
    : head_(nullptr), tail_(nullptr), size_(0) {}

template <typename T>
List342<T>::List342(List342& list)
    : head_(nullptr), tail_(nullptr), size_(0) {
    Node<T>* temp = list.head_;
    while (temp != nullptr) {
        AddToTail(temp->data);
        temp = temp->next;
    }
}

template <typename T>
List342<T>::List342(List342 const& list)
    : head_(nullptr), tail_(nullptr), size_(0) {
    Node<T>* temp = list.head_;
    while (temp != nullptr) {
        AddToTail(temp->data);
        temp = temp->next;
    }
}

template <typename T>
List342<T>::~List342() {
    if (head_ == nullptr) {
        return;
    }

    while (head_ != nullptr) {
        DeleteHead();
    }
    head_ = nullptr;
    tail_ = nullptr;
}

template <typename T>
void List342<T>::AddToHead(T* val) {
    if (tail_ == nullptr && head_ == nullptr) {
        head_ = new Node<T>(val, nullptr, nullptr);
        tail_ = head_;
    } else {
        Node<T>* temp = new Node<T>(val, nullptr, head_);
        head_->prev = temp;
        head_ = temp;
        temp = nullptr;
        delete temp;
    }
    size_++;
}

template <typename T>
bool List342<T>::RemoveHead(T& result) {
    if (head_ == nullptr) {
        return false;
    }

    T val = *(head_->data);
    Node<T>* temp = head_;
    head_ = head_->next;

    if (head_ != nullptr) {
        head_->prev = nullptr;
    } else {
        tail_ = nullptr;
    }

    delete temp;
    temp = nullptr;

    size_--;
    return true;
}

template <typename T>
void List342<T>::DeleteHead() {
    if (head_ == nullptr) {
        return;
    }
    Node<T>* temp = head_;
    temp->prev = nullptr;
    head_ = head_->next;
    if (head_ != nullptr) {
        head_->prev = nullptr;
    } else {
        tail_ = nullptr;
    }
    temp->next = nullptr;
    temp->prev = nullptr;

    delete temp;
    temp = nullptr;
    size_--;
}

template <typename T>
bool List342<T>::PeekHead(T& result) const {
    if (head_ == nullptr) {
        return false;
    }
    result = *(head_->data);
    return true;
}

template <typename T>
void List342<T>::AddToTail(T* val) {
    if (tail_ == nullptr && head_ == nullptr) {
        head_ = new Node<T>(val, nullptr, nullptr);
        tail_ = head_;
    } else {
        Node<T>* temp = new Node<T>(val, tail_, nullptr);
        tail_->next = temp;
        tail_ = temp;
    }
    size_++;
}

template <typename T>
bool List342<T>::RemoveTail(T& result) {
    if (head_ == nullptr) {
        return false;
    }
    T val = *(tail_->data);
    Node<T>* temp = tail_;
    temp->prev = nullptr;
    tail_ = tail_->prev;
    if (tail_ != nullptr) {
        tail_->next = nullptr;

    } else {
        head_ = nullptr;
    }
    delete temp;
    temp = nullptr;
    size_--;
    return true;
}

template <typename T>
bool List342<T>::PeekTail(T& result) const {
    if (tail_ == nullptr) {
        return false;
    }
    result = *(tail_->data);
    return true;
}

template <typename T>
bool List342<T>::Peek(T target, T& result) const {
    Node<T>* temp = head_;
    while (temp != nullptr) {
        if (temp->data != nullptr && *(temp->data) == target) {
            if (temp == tail_) {
                return PeekTail(result);
            } else if (temp == head_) {
                return PeekHead(result);
            } else {
                result = *(temp->data);
                return true;
            }
        }
        temp = temp->next;
    }
    return false;
}

template <typename T>
bool List342<T>::Insert(T* obj) {
    if (head_ == nullptr || *obj < *head_->data) {
        AddToHead(obj);
        return true;
    }

    if (*obj > *tail_->data) {
        AddToTail(obj);
        return true;
    }

    Node<T>* temp = head_;
    while (temp != nullptr) {
        if (*temp->data == *obj) {
            return false;
        }
        if (*obj > *temp->data && *obj <= *temp->next->data) {
            if (*temp->next->data == *obj) {
                return false;
            }
            Node<T>* newNode = new Node<T>(obj, temp, temp->next);
            temp->next->prev = newNode;
            temp->next = newNode;
            size_++;
            return true;
        }
        temp = temp->next;
    }
    return true;
}

template <typename T>
bool List342<T>::Remove(T target, T& result) {
    Node<T>* temp = head_;
    while (temp != nullptr) {
        if (*(temp->data) == target) {
            if (temp == tail_) {
                return RemoveTail(result);
            } else if (temp == head_) {
                return RemoveHead(result);
            } else {
                result = *(temp->data);
                temp->prev->next = temp->next;
                temp->next->prev = temp->prev;
                delete temp;
                temp = nullptr;
                size_--;
                return true;
            }
        }
        temp = temp->next;
    }
    return false;
}

template <typename T>
int List342<T>::size() const {
    return size_;
}

template <class T>
bool List342<T>::BuildList(string file_name) {
    std::ifstream in_file(file_name);
    if (!in_file.is_open()) {
        // std::cerr << "Error: Unable to open file " << file_name << std::endl;
        return false;
    }

    T temp;
    while (in_file >> temp) {
        Insert(new T(temp));  // Assuming AddToTail is a member function of your List342 class
    }

    in_file.close();
    return true;
}

template <typename T>
void List342<T>::DeleteList() {
    while (head_ != nullptr) {
        DeleteHead();
    }
    head_ = nullptr;
    tail_ = nullptr;
}

template <typename T>
bool List342<T>::Merge(List342& list1) {
    if (list1 == *this) {
        return true;
    }

    if (list1.head_ == nullptr) {
        return true;
    }

    if (head_ == nullptr) {
        head_ = list1.head_;
        tail_ = list1.tail_;
        list1.head_ = nullptr;
        list1.tail_ = nullptr;
        return true;
    }

    if (*list1.head_->data > *tail_->data) {
        tail_->next = list1.head_;
        tail_ = list1.tail_;
        list1.head_ = nullptr;
        list1.tail_ = nullptr;
        return true;
    }

    Node<T>* temp = head_;
    while (list1.head_ != nullptr) {
        // Check if merging list head is less than first list head
        if (*list1.head_->data < *head_->data) {
            // Set head->prev to merging list head
            head_->prev = list1.head_;
            // Set merging list head to next node;
            list1.head_ = list1.head_->next;
            // Remove link to old merging head node
            list1.head_->prev = nullptr;
            // Decrement merging list size
            list1.size_--;
            // Set head->next to old head
            head_->prev->next = head_;
            // Set head pointer to new head
            head_ = head_->prev;
            // Remove new head pointer to prev node
            head_->prev = nullptr;
            // Increment list size
            size_++;
            continue;
        }

        if (*temp->data == *list1.head_->data) {
            list1.DeleteHead();
            continue;
        }

        // Check if merging list head is greater than current index
        if (*list1.head_->data > *temp->data) {
            // Check if at tail
            // if (temp->next == nullptr) {
            if (temp == tail_) {
                // set merging list head prev to tail;
                list1.head_->prev = tail_;
                // Set tail next to merging list head
                tail_->next = list1.head_;
                // Set tail to merging list tail
                tail_ = list1.tail_;
                // increment size by merging list size
                size_ += list1.size_;
                // set list1 pointers to null
                list1.head_ = nullptr;
                list1.tail_ = nullptr;
                return true;
            }

            // Check if not at tail, but in middle of list
            if (*list1.head_->data < *temp->next->data) {
                // Pointer to node after current
                Node<T>* nextNode = temp->next;

                temp->next = list1.head_;

                list1.head_ = list1.head_->next;
                list1.head_->prev = nullptr;

                temp->next->prev = temp;
                temp->next->next = nextNode;
                temp->next->next->prev = temp->next;
                size_++;
            }
        }

        temp = temp->next;
    }
    list1.head_ = nullptr;
    list1.tail_ = nullptr;
    return true;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const List342<T>& list) {
    if (list.head_ == nullptr) {
        return os;
    }
    Node<T>* temp = list.head_;
    while (temp != nullptr) {
        os << *(temp->data);
        temp = temp->next;
    }
    return os;
}

template <typename T>
bool operator==(const List342<T>& list1, const List342<T>& list2) {
    if (list1.size_ != list2.size_) {
        return false;
    }
    Node<T>* temp1 = list1.head_;
    Node<T>* temp2 = list2.head_;
    while (temp1 != nullptr && temp2 != nullptr) {
        if (*temp1->data != *temp2->data) {
            return false;
        }
        temp1 = temp1->next;
        temp2 = temp2->next;
    }
    return true;
}

template <typename T>
List342<T>& List342<T>::operator+=(List342<T> const& list1) {
    if (list1.head_ == nullptr || &list1 == this) {
        return *this;
    }

    if (head_ == nullptr) {
        *this = list1;
        return *this;
    }

    if (*list1.head_->data > *tail_->data) {
        Node<T>* temp = list1.head_;
        while (temp != nullptr) {
            AddToTail(temp->data);
            temp = temp->next;
        }
        return *this;
    }

    Node<T>* temp = head_;
    Node<T>* merging_node = list1.head_;
    while (merging_node != nullptr) {
        // Check if merging list head is less than first list head
        if (*merging_node->data < *head_->data) {
            AddToHead(merging_node->data);
            merging_node = merging_node->next;
            continue;
        }

        if (*temp->data == *merging_node->data) {
            merging_node = merging_node->next;
            continue;
        }

        // Check if merging list head is greater than current index
        if (*merging_node->data > *temp->data) {
            // Check if at tail
            if (temp == tail_) {
                while (merging_node != nullptr) {
                    AddToTail(merging_node->data);
                    merging_node = merging_node->next;
                }
                return *this;
            }

            // Check if not at tail, but in middle of list
            if (*merging_node->data < *temp->next->data) {
                // Pointer to node after current
                Node<T>* nextNode = temp->next;
                Node<T>* newNode = new Node<T>(merging_node->data, temp, temp->next);
                temp->next = newNode;
                newNode->next->prev = newNode;
                size_++;
            }
        }

        temp = temp->next;
    }
    return *this;
}

template <typename H>
bool operator!=(const List342<H>& list1, const List342<H>& list2) {
    return !(list1 == list2);
}

template <typename T>
List342<T> operator+(List342<T> list1, const List342<T>& list2) {
    list1 += list2;
    return std::move(list1);
}

template <typename T>
List342<T>& List342<T>::operator=(List342<T> const& list1) {
    DeleteList();
    Node<T>* temp = list1.head_;
    while (temp != nullptr) {
        AddToTail(temp->data);
        temp = temp->next;
    }
    return *this;
}