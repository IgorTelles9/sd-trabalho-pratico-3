#include <iostream>
#include "queue.h"

template <typename T>
Queue<T>::Queue() : first(nullptr), last(nullptr) {}

template <typename T>
Queue<T>::~Queue()
{
    while (!isEmpty())
    {
        pop();
    }
}

template <typename T>
bool Queue<T>::isEmpty() const
{
    return (first == nullptr);
}

template <typename T>
void Queue<T>::push(const T &item)
{
    Node *newNode = new Node;
    newNode->data = item;
    newNode->next = nullptr;

    if (isEmpty())
    {
        first = last = newNode;
    }
    else
    {
        last->next = newNode;
        last = newNode;
    }
}

template <typename T>
void Queue<T>::pop()
{
    if (isEmpty())
    {
        std::cout << "Queue is empty. Cannot pop." << std::endl;
        return;
    }

    Node *temp = first;
    first = first->next;

    if (first == nullptr)
    {
        last = nullptr;
    }

    delete temp;
}

template <typename T>
T Queue<T>::peek() const{
    if (isEmpty()){
        return 0;
    }
    return first->data;
}

template <typename T>
std::string Queue<T>::getQueue() {
    Node *curr = first; 
    std::string list = "[";
    while(1){
        if (curr == nullptr){
            break;
        }
        list.append(std::to_string(curr->data) + ",");
        curr = curr->next;
    }
    if (list.length() != 1){
        list.pop_back();
    }
    list.append("]");
    return list;
}

template class Queue<int>;