#include <iostream>
#include <string>
using namespace std;

// Custom Circular Queue Class
class CircularQueue {
private:
    string* players; // Array to hold players
    int front;       // Front index
    int rear;        // Rear index
    int capacity;    // Max size of the queue
    int size;        // Current number of elements

public:
    // Constructor
    CircularQueue(int groupSize) {
        capacity = groupSize;
        players = new string[capacity];
        front = rear = -1;
        size = 0;
    }

    // Destructor
    ~CircularQueue() {
        delete[] players;
    }

    // Check if the queue is empty
    bool isEmpty() {
        return size == 0;
    }

    // Check if the queue is full
    bool isFull() {
        return size == capacity;
    }

    // Add a player to the queue
    void enqueue(string player) {
        if (isFull()) {
            cout << "Queue is full!" << endl;
            return;
        }
        if (isEmpty()) front = 0;
        rear = (rear + 1) % capacity;
        players[rear] = player;
        size++;
    }

    // Remove a player from the queue
    string dequeue() {
        if (isEmpty()) {
            cout << "Queue is empty!" << endl;
            return "";
        }
        string player = players[front];
        if (front == rear) {
            front = rear = -1;
        } else {
            front = (front + 1) % capacity;
        }
        size--;
        return player;
    }

    // Rotate the queue (shift all elements except the first)
    void rotate() {
        if (size <= 1) return; // No rotation needed for single player

        // Store the first player
        string first = dequeue();

        // Rotate the remaining players
        enqueue(dequeue());

        // Re-add the first player to the front
        front = (front - 1 + capacity) % capacity;
        players[front] = first;
        size++;
    }

    // Display the current queue
    void display() {
        if (isEmpty()) {
            cout << "Queue is empty!" << endl;
            return;
        }
        int index = front;
        for (int i = 0; i < size; i++) {
            cout << "[" << players[index] << "] ";
            index = (index + 1) % capacity;
        }
        cout << endl;
    }
};

// Function to generate matches for the current rotation
void generateMatches(CircularQueue& group) {
    // Assume even number of players
    int mid = group.getSize() / 2;
    for (int i = 0; i < mid; i++) {
        string player1 = group.dequeue();
        string player2 = group.dequeue();
        cout << "Match: " << player1 << " vs " << player2 << endl;
        // Re-enqueue players for the next rotation
        group.enqueue(player1);
        group.enqueue(player2);
    }
}

int main() {
    // Example: Group of 4 players
    CircularQueue group(4);
    group.enqueue("P1");
    group.enqueue("P2");
    group.enqueue("P3");
    group.enqueue("P4");

    // Simulate Round-Robin rotations
    int rounds = group.getSize() - 1;
    for (int i = 0; i < rounds; i++) {
        cout << "\nRound " << i + 1 << " Matches:" << endl;
        generateMatches(group);
        group.rotate();
    }

    return 0;
}