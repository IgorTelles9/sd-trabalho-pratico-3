#include <iostream>

template<typename T>
class Queue{

    private:
        struct Node {
            T data;
            Node* next;
        };

        Node* first;
        Node* last;

    public:
        Queue();
        ~Queue();
        bool isEmpty() const;
        T peek() const; 
        void push(const T& item);
        void pop();
};