/*
 * This file implements all methods defined in Stack.hpp.
 * The stack maintains a history of all ticket-related transactions.
 */

#include "../HeaderFiles/tickectManager/Stack.hpp"
 #include <iostream>
 #include <iomanip>
 using namespace std;
 
 
 Stack::Stack() : top(nullptr), size(0) {
     // Initialize an empty stack
 }
 

 Stack::~Stack() {
     // Free all nodes in the stack
     while (!isEmpty()) {
         pop();
     }
 }
 
 // Add a transaction to the top of the stack
 void Stack::push(const Transaction& transaction) {
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
 Transaction Stack::pop() {
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
 Transaction Stack::peek() const {
     // Check if stack is empty
     if (isEmpty()) {
         cerr << "Error: Cannot peek empty stack" <<endl;
         return Transaction(); // Return empty transaction
     }
     
     return top->data;
 }
 
 // Check if the stack is empty
 bool Stack::isEmpty() const {
     return top == nullptr;
 }
 
 // Get the current size of the stack
 int Stack::getSize() const {
     return size;
 }
 
 // Helper function to format time_t as a string
 string formatTime(time_t timestamp) {
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
 void printTransaction(const Transaction& transaction) {
     cout << setw(12) << transaction.transactionID << " | " 
               << setw(15) << transaction.customerID << " | " 
               << setw(15) << transaction.ticketID << " | " 
               << setw(10) << transaction.type << " | " 
               << formatTime(transaction.timestamp) <<endl;
 }
 
 // Search for transactions by customer ID
 void Stack::searchByCustomer(const string& customerID) const {
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
 void Stack::searchByTicket(const string& ticketID) const {
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
 void Stack::displayRecent(int count) const {
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
 void Stack::displayAll() const {
     displayRecent(size);
 }