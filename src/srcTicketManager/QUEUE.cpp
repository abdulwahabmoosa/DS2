/*
 This file contains the implementation of all methods defined in Queue.hpp.
 The queue is implemented as a linked list with both front and rear pointers
 for efficient operations at both ends.
 */

 #include "../HeaderFiles/tickectManager/QUEUE.hpp"
 #include <iostream>
 #include <iomanip>
 using namespace std;
 
 // Constructor 
 Queue::Queue(int gateNum, int maxCapacity) 
     : front(nullptr), rear(nullptr), size(0), gateNumber(gateNum), maxSize(maxCapacity) {
 }
 
 //Destructor - clean up all allocated memory
 Queue::~Queue() {
     //Free all nodes in the queue
     while (!isEmpty()) {
         dequeue();
     }
 }
 
 // Add a spectator to the back of the queue
 bool Queue::enqueue(const string& customerID, const string& ticketID) {
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
EntryRecord Queue::dequeue() {
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
 bool Queue::isEmpty() const {
     return size == 0;
 }
 
 // Check if queue is full
 bool Queue::isFull() const {
     return size >= maxSize;
 }
 
 // Get current size of queue
 int Queue::getSize() const {
     return size;
 }
 
 // Get gate number
 int Queue::getGateNumber() const {
     return gateNumber;
 }
 
 // Display all spectators in the queue
 void Queue::display() const {
     // Check if queue is empty
     if (isEmpty()) {
         cout << "Gate " << gateNumber << " queue is empty." <<endl;
         return;
     }
     
     // Print header
     cout << "\n----- Gate " << gateNumber << " Entry Queue -----" <<endl;
     cout << "Total spectators in line: " << size << " (Max: " << maxSize << ")" <<endl;
     cout << std::setw(5) << "Pos" << " | " 
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