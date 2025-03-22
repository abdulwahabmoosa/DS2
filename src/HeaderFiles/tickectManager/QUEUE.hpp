/* This file defines a custom Queue implementation for managing entry lines
   at stadium gates.The queue follows  (FIFO) order.
   
   **JUSTIFICATION FOR QUEUE:
   A queue is the ideal data structure for entry management because:
   1. Stadium entry lines naturally follow FIFO order (first to arrive is first to enter)
   2. Each category of ticket holder forms a separate line (VIP, Early Bird, General)
   3. It provides O(1) operations for both enqueue and dequeue operations
   4. It models the physical reality of spectator lines at entry gates
 */

 #ifndef QUEUE_HPP
 #define QUEUE_HPP
 
 #include "Models.hpp"
 #include <string>
 using namespace  std;
 
 
 /*
    This queue uses a linked list approach with both front and rear pointers
    for efficient enqueue and dequeue operations.
  */
 class Queue {
 private:
     
      //Node structure for the queue linked list
     struct Node {
         string customerID;  
         string ticketID;     
         Node* next;               
         
         Node(string cid, string tid) 
             : customerID(cid), ticketID(tid), next(nullptr) {}
     };
     
     Node* front;      // Pointer to the front of the queue (for dequeue)
     Node* rear;       // Pointer to the rear of the queue (for enqueue)
     int size;        
     int gateNumber;   
     int maxSize;      // Maximum capacity of this entry line
     
 public:
     
      //Initializes an empty queue with specified gate number and capacity
      Queue(int gateNum, int maxCapacity = 50);
     
      Queue() : front(nullptr), rear(nullptr), size(0), gateNumber(0), maxSize(50) {}
     
    
      //Properly deallocates all nodes in the queue
     ~Queue();
     
     
      //Add a spectator to the back of the queue
     bool enqueue(const string& customerID, const string& ticketID);

     //Remove the spectator from the front of the queue(dequeue)
     EntryRecord dequeue();
     //Check if the queue is empty (no spectators in the queue)
     bool isEmpty() const;
     //Check if the queue is full (reached its maximum capacity)
     bool isFull() const;
     //Get the current size of the queue (number of people in the queue)
     int getSize() const;
     //Get the gate number associated with this queue
     int getGateNumber() const;
     //Display all spectators in the queue
     void display() const;
 };
 
 #endif // QUEUE_HPP