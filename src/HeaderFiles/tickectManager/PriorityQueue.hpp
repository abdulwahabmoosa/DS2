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
public:     
     // Constructor to Initialize an empty priority queue
     PriorityQueue() : front(nullptr), size(0) {
         
     }
     
     //clean up all allocated memory
     ~PriorityQueue() {
         //Free all nodes in the queue
         while (!isEmpty()) {
             dequeue();
         }
     }
     
     /*
      * Calculate priority based on customer level and request time
      This is the core logic that determines ticket processing order
      */
     int calculatePriority(const TicketRequest& request) const {
         // Base priority from customer level (VIP=3, Early Bird=2, General=1)
         int basePriority = request.customer.priorityLevel * 1000;
         
         
         //subtract because earlier requests should have higher priority
         int timePriority = -(static_cast<int>(request.requestTime) / 100);//ividing by 100 to reduce its impact
    
         return basePriority + timePriority;//Combined priority value
     }
     
     // Add a ticket request to the queue with appropriate priority
     void enqueue(const TicketRequest& request) {
         // Calculate priority for this request
         int priority = calculatePriority(request);
         
         // Create new node
         Node* newNode = new Node(request, priority);
         
         // Case 1: Empty queue or new request has higher priority than front
         if (isEmpty() || priority > front->priority) {
             newNode->next = front;
             front = newNode;
         }
         // Case 2: Need to find the correct position in the queue
         else {
             // Traverse the queue to find insertion point
             Node* current = front;
             
             // Continue until we find the position where the next node has lower priority
             // or we reach the end of the queue
             while (current->next != nullptr && current->next->priority >= priority) {
                 current = current->next;
             }
             
             // Insert newNode between current and current->next
             newNode->next = current->next;
             current->next = newNode;
         }
         
         // Increment size counter
         size++;
     }
     
     // Remove and return the highest priority request
     TicketRequest dequeue() {
         // Check if queue is empty
         if (isEmpty()) {
             cerr << "Error: Cannot dequeue from empty priority queue" <<endl;
             return TicketRequest(); // Return empty request
         }
         
         // Save front node and its data
         Node* temp = front;
         TicketRequest request = temp->data;
         
         // Update front pointer to next node
         front = front->next;
         
         // Delete the old front node
         delete temp;
         
         // Decrement size counter
         size--;
         
         return request;
     }
     
     // Check if queue is empty
     bool isEmpty() const {
         return front == nullptr;
     }
     
     // Get current size of queue
     int getSize() const {
         return size;
     }
     
     // Get highest priority request without removing it
     TicketRequest peek() const {
         // Check if queue is empty
         if (isEmpty()) {
             cerr << "Error: Cannot peek empty priority queue" <<endl;
             return TicketRequest(); // Return empty request
         }
         
         return front->data;
     }
     
     // Display all requests in queue (for debugging/admin view)
     void display() const {
         // Check if queue is empty
         if (isEmpty()) {
             cout << "Priority Queue is empty." <<endl;
             return;
         }
         
         // Print header
         cout << "\n----- Ticket Request Queue -----" << endl;
         cout << setw(10) << "Priority" << " | " 
                   << setw(15) << "Customer" << " | " 
                   << setw(10) << "Match ID" << " | " 
                   << setw(20) << "Request Time" << endl;
         cout << string(60, '-') << endl;
         
         // Traverse queue and print each node
         Node* current = front;
         while (current != nullptr) {
             // Convert time_t to string
             char timeStr[26];
             //ctime_r(&current->data.requestTime, timeStr);
             ctime_s(timeStr, sizeof(timeStr), &current->data.requestTime);
             string timeString(timeStr);
             if (!timeString.empty() && timeString[timeString.size() - 1] == '\n') {
                 timeString.resize(timeString.size() - 1);
             }
             
             // Print node data
             cout << setw(10) << current->priority << " | " 
                       << setw(15) << current->data.customer.name << " | " 
                       << setw(10) << current->data.matchID << " | " 
                       << setw(20) << timeString << endl;
             
             current = current->next;
         }
         
         cout << string(60, '-') << endl;
     }
 };
 
 #endif 