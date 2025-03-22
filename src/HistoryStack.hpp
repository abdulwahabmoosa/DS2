#ifndef HISTORYSTACK_HPP
#define HISTORYSTACK_HPP

#include <iostream>
#include <string>

using namespace std;

// Node structure for the match history records
struct MatchHistoryNode {
    string matchID;
    string player1;
    string player2;
    int score1;
    int score2;
    string winner;
    string matchType;
    MatchHistoryNode* next;
};

// Stack implementation for match history using linked list
class HistoryStack {
private:
    MatchHistoryNode* top;
    int size;

public:
    // Constructor
    HistoryStack() {
        top = nullptr;
        size = 0;
    }

    // Destructor
    ~HistoryStack() {
        while (!isEmpty()) {
            pop();
        }
    }

    // Check if stack is empty
    bool isEmpty() {
        return top == nullptr;
    }

    // Get size of stack
    int getSize() {
        return size;
    }

    // Push a new match record to the stack
    void push(string matchID, string player1, string player2, int score1, int score2, string winner, string matchType) {
        MatchHistoryNode* newNode = new MatchHistoryNode;
        newNode->matchID = matchID;
        newNode->player1 = player1;
        newNode->player2 = player2;
        newNode->score1 = score1;
        newNode->score2 = score2;
        newNode->winner = winner;
        newNode->matchType = matchType;
        newNode->next = top;
        top = newNode;
        size++;
    }

    // Pop a match record from the stack
    MatchHistoryNode* pop() {
        if (isEmpty()) {
            cout << "Stack is empty!" << endl;
            return nullptr;
        }
        
        MatchHistoryNode* temp = top;
        top = top->next;
        temp->next = nullptr;
        size--;
        return temp;
    }

    // Peek at the top element without removing it
    MatchHistoryNode* peek() {
        if (isEmpty()) {
            cout << "Stack is empty!" << endl;
            return nullptr;
        }
        return top;
    }

    // Display all match records in the stack
    void displayHistory() {
        if (isEmpty()) {
            cout << "No match history available!" << endl;
            return;
        }

        MatchHistoryNode* current = top;
        int count = 1;
        
        cout << "\n" << string(80, '=') << endl;
        cout << "MATCH HISTORY (Most Recent First)" << endl;
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
            cout << "No match history available!" << endl;
            return;
        }

        MatchHistoryNode* current = top;
        int count = 0;
        
        cout << "\n" << string(80, '=') << endl;
        cout << "MATCH HISTORY FOR " << matchType << " (Most Recent First)" << endl;
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