#ifndef WITHDRAWALQUEUE_HPP
#define WITHDRAWALQUEUE_HPP

#include <string>
using namespace std;

struct Node {
    string name;
    int score;
    string qualifier;
    Node* next;

    Node(const string& name, int score, const string& qualifier)
        : name(name), score(score), qualifier(qualifier), next(nullptr) {}
};

class WithdrawalQueue {
private:
    Node* front;
    Node* rear;
    int count;
    int capacity;

public:
    WithdrawalQueue(int size) : capacity(size), front(nullptr), rear(nullptr), count(0) {}

    ~WithdrawalQueue() {
        while (!isEmpty()) {
            dequeue();
        }
    }

    void enqueue(const string& name, int score, const string& qualifier) {
        if (isFull()) {
            dequeue(); // Remove oldest element to maintain capacity
        }
        
        Node* newNode = new Node(name, score, qualifier);
        if (isEmpty()) {
            front = rear = newNode;
            rear->next = front;
        } else {
            rear->next = newNode;
            rear = newNode;
            rear->next = front;
        }
        count++;
    }

    void dequeue() {
        if (isEmpty()) return;

        Node* temp = front;
        if (front == rear) {
            front = rear = nullptr;
        } else {
            front = front->next;
            rear->next = front;
        }
        delete temp;
        count--;
    }

    bool isFull() const {
        return count == capacity;
    }

    bool isEmpty() const {
        return count == 0;
    }

    Node* operator[](int index) const {
        if (index < 0 || index >= count) return nullptr;
        
        Node* current = front;
        for (int i = 0; i < index; ++i) {
            current = current->next;
        }
        return current;
    }

    void removeAt(int index) {
        if (index < 0 || index >= count || isEmpty()) return;

        if (index == 0) {
            dequeue();
            return;
        }

        Node* prev = front;
        for (int i = 0; i < index - 1; ++i) {
            prev = prev->next;
        }

        Node* toDelete = prev->next;
        prev->next = toDelete->next;
        
        if (toDelete == rear) {
            rear = prev;
        }

        delete toDelete;
        count--;
        
        if (count > 0) {
            rear->next = front;
        }
    }

    int size() const {
        return count;
    }
};

#endif