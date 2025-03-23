/*
 This file defines a custom Circular Queue implementation for managing
 seat allocations and venue capacity for tennis matches.
 
 JUSTIFICATION FOR CIRCULAR QUEUE:
    A circular queue is the ideal data structure for venue capacity management because:
    1. It efficiently utilizes fixed memory for a fixed capacity venue
    2. It handles the cyclical nature of seat allocation/deallocation
    3. It prevents memory fragmentation that would occur with dynamic structures
    4. It models the physical layout of sequential seats in a venue
 */

 #ifndef CIRCULAR_QUEUE_HPP
 #define CIRCULAR_QUEUE_HPP
 #include <string>
 using namespace std;
 
 
   //This circular queue tracks seat availability and allocations for tennis matches.
  //It uses a circular buffer to efficiently manage fixed capacity venues.
  
 class TicketCircularQueue {
 private:
     
      //Structure to track information about each seat
     struct SeatInfo {
         int seatNumber;           
         bool isAllocated;         
         string customerID;   
         string ticketID;    
         
         // Constructor
         SeatInfo() : seatNumber(0), isAllocated(false), customerID(""), ticketID("") {}
     };
     
     SeatInfo* seats;//Array of seat information
     int capacity;       
     int front;
     int rear;
     int matchCategory;//3=VIP, 2=Early-Bird, 1=General match
     
     
      // Get the next index in circular fashion 
     int getNextIndex(int index) const;
     
     
      //Get the previous index in circular fashion
     int getPrevIndex(int index) const;
     
 public:
     
      //Initializes a circular queue with the specified capacity and match category
     TicketCircularQueue(int capacity = 100, int category = 1);
     
     
      //Properly deallocates the seats array
     ~TicketCircularQueue();
     
     
      //Allocate a specific seat
     bool allocateSeat(int seatNumber, const string& customerID, const string& ticketID);
     
     
      //Deallocate (free) a specific seat
     bool deallocateSeat(int seatNumber);
     
     //Check if a specific seat is allocated
     bool isSeatAllocated(int seatNumber) const;
     
     
    //  Get the next available seat for a specific category
     int getNextAvailableSeat(int category) const;

     // Get the ticket ID for a specific seat
     string getTicketIDForSeat(int seatNumber) const;
     
     //Get the customer ID for a specific seat
     string getCustomerIDForSeat(int seatNumber) const;
     
     //Get the number of allocated seats
     int getAllocatedSeatsCount() const;
     
     //Get the number of available seats
     int getAvailableSeatsCount() const;
     
     //Check if venue is full
     bool isFull() const;
     
     //Check if the queue is empty
     bool isEmpty() const;
     
     //Display seating layout
     void displaySeating() const;
 };
 
 #endif