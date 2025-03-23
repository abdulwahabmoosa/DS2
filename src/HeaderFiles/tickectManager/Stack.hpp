/*
 This file defines a custom Stack implementation for tracking ticket
 sales, refunds, and entry/exit transactions.
 
 JUSTIFICATION FOR STACK:
 A stack is the ideal data structure for transaction history because:
    1 Recent transactions are most likely to need viewing/modification
    2 Provides perfect LIFO (Last-In-First-Out) order for transaction tracking
    3 Enables efficient undo operations for recent ticket actions
    4 Natural modeling of chronological actions with most recent on top
    5 O(1) push and pop operations for high-performance transaction logging
 */

 #ifndef STACK_HPP
 #define STACK_HPP
 
 #include "Models.hpp"
 #include <string>
 
 
  //This stack uses a linked list approach with top pointer for
  //efficient push and pop operations.
  
 class Stack {
 private:
     
      //Each node contains a transaction record and a pointer to the
      //next node in the stack.
     struct Node {
         Transaction data;   
         Node* next;
         
         
         Node(Transaction transaction) : data(transaction), next(nullptr) {}
     };
     
     Node* top;//Pointer to the top of the stack
     int size; // Current number of elements in the stack
     
 public:
     
     Stack() : top(nullptr), size(0) {
        // Initialize an empty stack
    }
    
    
    ~Stack() {
        // Free all nodes in the stack
        while (!isEmpty()) {
            pop();
        }
    }
    
    // Add a transaction to the top of the stack
    void push(const Transaction& transaction) {
        // Create new node
        Node* newNode = new Node(transaction);
        
        // Set its next pointer to current top
        newNode->next = top;
        
        // Update top to point to the new node
        top = newNode;
        
        // Increment size counter
        size++;
    }
    
    // Remove the transaction from the top of the stack
    Transaction pop() {
        if (isEmpty()) {
            cerr << "Error: Cannot pop from empty stack" <<endl;
            return Transaction(); // Return empty transaction
        }
        
        // Save top node and its data
        Node* temp = top;
        Transaction transaction = temp->data;
        
        // Update top pointer
        top = top->next;
        
        // Delete the old top node
        delete temp;
        
        // Decrement size counter
        size--;
        
        return transaction;
    }
    
    // View the transaction at the top without removing it
    Transaction peek() const {
        // Check if stack is empty
        if (isEmpty()) {
            cerr << "Error: Cannot peek empty stack" <<endl;
            return Transaction(); // Return empty transaction
        }
        
        return top->data;
    }
    
    // Check if the stack is empty
    bool isEmpty() const {
        return top == nullptr;
    }
    
    // Get the current size of the stack
    int getSize() const {
        return size;
    }
    
    // Helper function to format time_t as a string
    string formatTime(time_t timestamp) const {
        char timeStr[26];
        //ctime_r(&timestamp, timeStr);
        ctime_s(timeStr, sizeof(timeStr), &timestamp);
        string timeString(timeStr);
        if (!timeString.empty() && timeString[timeString.size() - 1] == '\n') {
            timeString.resize(timeString.size() - 1);
        }
        return timeString;
    }
    
    // Helper function to print a transaction
    void printTransaction(const Transaction& transaction) const {
        cout << setw(12) << transaction.transactionID << " | " 
                  << setw(15) << transaction.customerID << " | " 
                  << setw(15) << transaction.ticketID << " | " 
                  << setw(10) << transaction.type << " | " 
                  << formatTime(transaction.timestamp) <<endl;
    }
    
    // Search for transactions by customer ID
    void searchByCustomer(const string& customerID) const {
        // Check if stack is empty
        if (isEmpty()) {
            cout << "Transaction history is empty." << endl;
            return;
        }
        
        cout << "\n----- Transactions for Customer: " << customerID << " -----" <<endl;
        cout << setw(12) << "Transaction" << " | " 
                  << setw(15) << "Customer ID" << " | " 
                  << setw(15) << "Ticket ID" << " | " 
                  << setw(10) << "Type" << " | " 
                  << "Timestamp" << endl;
        cout << string(80, '-') << endl;
        
        // Counter for found transactions
        int count = 0;
        
        // Traverse stack and print matching transactions
        Node* current = top;
        while (current != nullptr) {
            if (current->data.customerID == customerID) {
                printTransaction(current->data);
                count++;
            }
            current = current->next;
        }
        
        cout << string(80, '-') << endl;
        cout << count << " transactions found." << endl;
    }
    
    // Search for transactions by ticket ID
    void searchByTicket(const string& ticketID) const {
        // Check if stack is empty
        if (isEmpty()) {
            cout << "Transaction history is empty." <<endl;
            return;
        }
        
        cout << "\n----- Transactions for Ticket: " << ticketID << " -----" <<endl;
        cout << setw(12) << "Transaction" << " | " 
                  << setw(15) << "Customer ID" << " | " 
                  << setw(15) << "Ticket ID" << " | " 
                  << setw(10) << "Type" << " | " 
                  << "Timestamp" << endl;
        cout << string(80, '-') <<endl;
        
        // Counter for found transactions
        int count = 0;
        
        // Traverse stack and print matching transactions
        Node* current = top;
        while (current != nullptr) {
            if (current->data.ticketID == ticketID) {
                printTransaction(current->data);
                count++;
            }
            current = current->next;
        }
        
        cout <<string(80, '-') <<endl;
        cout << count << " transactions found." <<endl;
    }
    
    // Display recent transactions
    void displayRecent(int count) const {
        // Check if stack is empty
        if (isEmpty()) {
            cout << "Transaction history is empty." <<endl;
            return;
        }
        
        // Limit count to actual size
        int displayCount = (count < size) ? count : size;
        
        cout << "\n----- Recent Transactions (Last " << displayCount << ") -----" <<endl;
        cout <<setw(12) << "Transaction" << " | " 
                  <<setw(15) << "Customer ID" << " | " 
                  <<setw(15) << "Ticket ID" << " | " 
                  <<setw(10) << "Type" << " | " 
                  << "Timestamp" <<endl;
        cout << string(80, '-') << endl;
        
        // Traverse stack and print transactions
        Node* current = top;
        int i = 0;
        
        while (current != nullptr && i < displayCount) {
            printTransaction(current->data);
            current = current->next;
            i++;
        }
        
        cout <<string(80, '-') <<endl;
    }
    
    // Display all transactions
    void displayAll() const {
        displayRecent(size);
    }
 };
 
 #endif