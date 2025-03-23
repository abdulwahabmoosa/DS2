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
     
    // Constructor 
    Queue() : front(nullptr), rear(nullptr), size(0), gateNumber(0), maxSize(0) {
    }
    
    Queue(int gateNum, int maxCapacity) 
        : front(nullptr), rear(nullptr), size(0), gateNumber(gateNum), maxSize(maxCapacity) {
    }
  
  //Destructor - clean up all allocated memory
    ~Queue() {
       //Free all nodes in the queue
       while (!isEmpty()) {
           dequeue();
       }
    }
  
  // Add a spectator to the back of the queue
  bool enqueue(const string& customerID, const string& ticketID) {
   // Check if queue is full
   if (isFull()) {
       cerr << "Error: Cannot enqueue, gate " << gateNumber << " queue is full" <<endl;
       return false;
   }
   
   // Create new node
   Node* newNode = new Node(customerID, ticketID);
   
   // Case 1: Queue is empty
   if (isEmpty()) {
       front = newNode;
       rear = newNode;
   }
   // Case 2: Queue has at least one element
   else {
       rear->next = newNode;
       rear = newNode;
   }
   
   // Increment size counter
   size++;
   return true;
  }
  
  // Remove the spectator from the front of the queue
  EntryRecord dequeue() {
  // Check if queue is empty
  if (isEmpty()) {
      cerr << "Error: Cannot dequeue from empty queue at gate " << gateNumber <<endl;
      return EntryRecord(); //Return empty record
  }
  
  // Save front node data
  string customerID = front->customerID;
  string ticketID = front->ticketID;
  
  // Save pointer to current front
  Node* temp = front;
  
  // Update front pointer
  front = front->next;
  
  // If this was the last node, update rear as well
  if (front == nullptr) {
      rear = nullptr;
  }
  
  // Delete the old front node
  delete temp;
  
  // Decrement size counter
  size--;
  
  return EntryRecord(customerID, ticketID);
  }
  
  // Check if queue is empty
  bool isEmpty() const {
   return size == 0;
  }
  
  // Check if queue is full
  bool isFull() const {
   return size >= maxSize;
  }
  
  // Get current size of queue
  int getSize() const {
   return size;
  }
  
  // Get gate number
  int getGateNumber() const {
   return gateNumber;
  }
  
  // Display all spectators in the queue
  void display() const {
   // Check if queue is empty
   if (isEmpty()) {
       cout << "Gate " << gateNumber << " queue is empty." <<endl;
       return;
   }
   
   // Print header
   cout << "\n----- Gate " << gateNumber << " Entry Queue -----" <<endl;
   cout << "Total spectators in line: " << size << " (Max: " << maxSize << ")" <<endl;
   cout << setw(5) << "Pos" << " | " 
             <<setw(15) << "Customer ID" << " | " 
             <<setw(15) << "Ticket ID" <<endl;
          cout <<string(45, '-') <<endl;
   
   // Traverse queue and print each node
   Node* current = front;
   int position = 1;
   
   while (current != nullptr) {
          cout << setw(5) << position << " | " 
                 << setw(15) << current->customerID << " | " 
                 << setw(15) << current->ticketID <<endl;
       
       current = current->next;
       position++;
   }
   
      cout << string(45, '-') <<endl;
  }
 };
 
 #endif // QUEUE_HPP