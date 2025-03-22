/**
  This file defines a custom Priority Queue implementation for processing ticket requests
  according to customer priority levels. The queue ensures that higher priority customers
  (VIPs, Early-Bird) are served before lower priority customers (General Public).
 
 **JUSTIFICATION FOR PRIORITY QUEUE:
  A priority queue is the ideal data structure for ticket sales because:
  1. It naturally models the business requirement of serving VIPs first
  2. Within each priority level, it maintains FIFO ordering (first come,first served)
  3. It efficiently handles insertion and extraction based on priority
  4. It allows dynamic prioritization of requests based on multiple factors
 */

 #ifndef PRIORITY_QUEUE_HPP
 #define PRIORITY_QUEUE_HPP
 
 #include "Models.hpp"
 
 /**
  priority queue uses a linked list approach where nodes are
  always inserted in the correct position based on their priority.
  Higher priority values indicate higher priority in the queue.
  */

 class PriorityQueue {
 private:
     /*
      Each node contains a ticket request and its calculated priority value,
      as well as a pointer to the next node in the queue.
      */
     struct Node {
         TicketRequest data;   
         int priority;         
         Node* next;       
         
         
          //Constructor
         Node(TicketRequest tr, int p) : data(tr), priority(p), next(nullptr) {}
     };

     //Pointer to the front of the queue
     Node* front;
     int size;//Number of elements in the queue
     
     /**
      Priority is calculated based on:
      1 Customer priority level (VIP=3, Early-Bird=2, General=1)
      2 Request timestamp (earlier requests get higher priority within same level)
      */
     int calculatePriority(const TicketRequest& request) const;
     
 public:
     
      //Default constructor
     PriorityQueue();
     
     
      //Properly deallocates all nodes in the queue
     ~PriorityQueue();
     

     void enqueue(const TicketRequest& request);//Insert a request into the queue
     
     
     TicketRequest dequeue();//Remove the highest priority request

     
     
     bool isEmpty() const;//Check if the queue is empty
     
     
     int getSize() const;//Get the current size of the queue
     
     
     TicketRequest peek() const;//View the highest priority element without removing it

     
     void display() const;//Display all elements in the queue
 };
 
 #endif 