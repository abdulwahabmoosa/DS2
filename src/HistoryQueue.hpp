#ifndef HISTORYQUEUE_HPP
#define HISTORYQUEUE_HPP

#include <iostream>
#include <string>

using namespace std;

// Node structure for the match history records
struct MatchHistoryQueueNode {
    string matchID;
    string player1;
    string player2;
    int score1;
    int score2;
    string winner;
    string matchType;
    MatchHistoryQueueNode* next;
};

// Queue implementation for match history using linked list
class HistoryQueue {
private:
    MatchHistoryQueueNode* front;
    MatchHistoryQueueNode* rear;
    int size;

public:
    // Constructor
    HistoryQueue() {
        front = nullptr;
        rear = nullptr;
        size = 0;
    }

    // Destructor
    ~HistoryQueue() {
        while (!isEmpty()) {
            dequeue();
        }
    }

    // Check if queue is empty
    bool isEmpty() {
        return front == nullptr;
    }

    // Get size of queue
    int getSize() {
        return size;
    }

    // Enqueue a new match record to the queue
    void enqueue(string matchID, string player1, string player2, int score1, int score2, string winner, string matchType) {
        MatchHistoryQueueNode* newNode = new MatchHistoryQueueNode;
        newNode->matchID = matchID;
        newNode->player1 = player1;
        newNode->player2 = player2;
        newNode->score1 = score1;
        newNode->score2 = score2;
        newNode->winner = winner;
        newNode->matchType = matchType;
        newNode->next = nullptr;
        
        if (isEmpty()) {
            front = newNode;
            rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
        
        size++;
    }

    // Dequeue a match record from the queue
    MatchHistoryQueueNode* dequeue() {
        if (isEmpty()) {
            cout << "Queue is empty!" << endl;
            return nullptr;
        }
        
        MatchHistoryQueueNode* temp = front;
        front = front->next;
        
        if (front == nullptr) {
            rear = nullptr;
        }
        
        temp->next = nullptr;
        size--;
        return temp;
    }

    // Display all match records in the queue in FIFO order
    void displayHistory() {
        if (isEmpty()) {
            cout << "No match history available in chronological order!" << endl;
            return;
        }

        MatchHistoryQueueNode* current = front;
        int count = 1;
        
        cout << "\n" << string(80, '=') << endl;
        cout << "MATCH HISTORY (Oldest First - FIFO)" << endl;
        cout << string(80, '=') << endl;
        
        while (current != nullptr) {
            cout << count << ". Match ID: " << current->matchID << endl;
            cout << "   Match Type: " << current->matchType << endl;
            cout << "   " << current->player1 << " VS " << current->player2 << endl;
            cout << "   Score: " << current->score1 << " - " << current->score2 << endl;
            cout << "   Winner: " << current->winner << endl;
            cout << string(80, '-') << endl;
            
            current = current->next;
            count++;
        }
    }

    // Display history for a specific match type (Qualifiers, Round Robin, Knockout)
    void displayHistoryByType(string matchType) {
        if (isEmpty()) {
            cout << "No match history available in chronological order!" << endl;
            return;
        }

        MatchHistoryQueueNode* current = front;
        int count = 0;
        
        cout << "\n" << string(80, '=') << endl;
        cout << "MATCH HISTORY FOR " << matchType << " (Oldest First - FIFO)" << endl;
        cout << string(80, '=') << endl;
        
        while (current != nullptr) {
            if (current->matchType == matchType) {
                count++;
                cout << count << ". Match ID: " << current->matchID << endl;
                cout << "   " << current->player1 << " VS " << current->player2 << endl;
                cout << "   Score: " << current->score1 << " - " << current->score2 << endl;
                cout << "   Winner: " << current->winner << endl;
                cout << string(80, '-') << endl;
            }
            
            current = current->next;
        }
        
        if (count == 0) {
            cout << "No matches found for type: " << matchType << endl;
        }
    }
};

#endif