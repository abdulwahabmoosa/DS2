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
     
     Stack();//Default constructor()
     
     //Destructor
     ~Stack();
     
     void push(const Transaction& transaction);//Push a transaction onto the stack
     
     //Remove the transaction from the top of the stack(return if empty)
     Transaction pop();

     //View the transaction at the top(without removing it/return if empty)
     Transaction peek() const;
     
     //Check if the stack is empty
     bool isEmpty() const;
     
    //Get the current size of the stack
     int getSize() const;
     
     //Search for transactions by customer ID
     void searchByCustomer(const string& customerID) const;
     
    //Search for transactions by ticket ID
     void searchByTicket(const string& ticketID) const;
     
     
     //Prints the specified number of most recent transactions(default: 10)
     void displayRecent(int count = 10) const;
     void displayAll() const;
 };
 
 #endif