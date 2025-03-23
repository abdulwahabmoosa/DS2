/*
 This file implements all methods defined in TicketCircularQueue.hpp.
 The circular queue manages seat allocations for tennis matches.
 */

 #include "../HeaderFiles/tickectManager/CircularQueue.hpp"
 #include <iostream>
 #include <iomanip>
 #include <string>
 using namespace std;
 
 
 // Constructor
 TicketCircularQueue::TicketCircularQueue(int capacity, int category) 
     : capacity(capacity), front(0), rear(capacity - 1), matchCategory(category) {
     
     //Allocate memory for seats
     seats = new SeatInfo[capacity];
     
     //Initialize seat numbers
     for (int i = 0; i < capacity; i++) {
         seats[i].seatNumber = i + 1;//Seat numbers start at 1
     }
 }
 
 //Destructor
 TicketCircularQueue::~TicketCircularQueue() {
     //Free allocated memory
     delete[] seats;
 }
 
 //Get next index in circular fashion
 int TicketCircularQueue::getNextIndex(int index) const {
     return (index + 1) % capacity;
 }
 
 //Get previous index in circular fashion
 int TicketCircularQueue::getPrevIndex(int index) const {
     return (index - 1 + capacity) % capacity;
 }
 
 // Allocate a specific seat
 bool TicketCircularQueue::allocateSeat(int seatNumber, const string& customerID, const string& ticketID) {
     // Validate seat number
     if (seatNumber < 1 || seatNumber > capacity) {
         cerr << "Error: Invalid seat number " << seatNumber <<endl;
         return false;
     }
     
     // Adjust for 0 based indexing
     int index = seatNumber - 1;
     
     // Check if seat is already allocated
     if (seats[index].isAllocated) {
         cerr << "Error: Seat " << seatNumber << " is already allocated" <<endl;
         return false;
     }
     
     // Allocate seat
     seats[index].isAllocated = true;//Seat is now allocated
     seats[index].customerID = customerID;
     seats[index].ticketID = ticketID;
     
     return true;
 }
 
 // Deallocate (free) a specific seat
 bool TicketCircularQueue::deallocateSeat(int seatNumber) {
     // Validate seat number
     if (seatNumber < 1 || seatNumber > capacity) {
         cerr << "Error: Invalid seat number " << seatNumber <<endl;
         return false;
     }
     
     //Adjust for 0 based indexing
     int index = seatNumber - 1;
     
     // Check if seat is already free
     if (!seats[index].isAllocated) {
         cerr << "Error: Seat " << seatNumber << " is not allocated" <<endl;
         return false;
     }
     
     // Free seat
     seats[index].isAllocated = false;//Seat is now free
     seats[index].customerID = "";
     seats[index].ticketID = "";
     
     return true;
 }
 
 // Check if a specific seat is allocated
 bool TicketCircularQueue::isSeatAllocated(int seatNumber) const {
     // Validate seat number
     if (seatNumber < 1 || seatNumber > capacity) {
         cerr << "Error: Invalid seat number " << seatNumber <<endl;
         return false;
     }
     
     //0 based indexing
     int index = seatNumber - 1;
     
     return seats[index].isAllocated;
 }
 
 // Get the next available seat number based on ticket category
 int TicketCircularQueue::getNextAvailableSeat(int category) const {
     // Define seat ranges based on category
     int startSeat, endSeat;
     
     switch (category) {
         case 3: //VIP
             startSeat = 1;
             endSeat = 20;
             break;
         case 2: //Early-Bird
             startSeat = 21;
             endSeat = 60;
             break;
         case 1: // General
             startSeat = 61;
             endSeat = 100;
             break;
         default:
             cerr << "Error: Invalid category " << category <<endl;
             return -1;
     }
     
     // Make sure we don't go beyond capacity
     if (endSeat > capacity) {
         endSeat = capacity;
     }
     
     // Look for available seat in the specified range
     for (int i = startSeat; i <= endSeat; i++) {
         if (!isSeatAllocated(i)) {
             return i;
         }
     }
     
     //If no seat available in preferred range, look in other areas
     if (category == 1) {
         // General customers can't be upgraded
         return -1;
     } else if (category == 2) {
         // Early Bird can be downgraded to General
         for (int i = 61; i <= capacity; i++) {
             if (!isSeatAllocated(i)) {
                 return i;
             }
         }
     } else if (category == 3) {
         //VIP can be downgraded to Early Bird or General
         for (int i = 21; i <= capacity; i++) {
             if (!isSeatAllocated(i)) {
                 return i;
             }
         }
     }
     
     // No available seats
     return -1;
 }
 
 // Get ticket ID for a seat
 string TicketCircularQueue::getTicketIDForSeat(int seatNumber) const {
     // Validate seat number
     if (seatNumber < 1 || seatNumber > capacity) {
         cerr << "Error: Invalid seat number " << seatNumber <<endl;
         return "";
     }
     
     //for 0 based indexing
     int index = seatNumber - 1;
     
     return seats[index].ticketID;
 }
 
 // Get customer ID for a seat
 string TicketCircularQueue::getCustomerIDForSeat(int seatNumber) const {
     // Validate seat number
     if (seatNumber < 1 || seatNumber > capacity) {
         cerr << "Error: Invalid seat number " << seatNumber <<endl;
         return "";
     }
     
     
     int index = seatNumber - 1;
     
     return seats[index].customerID;//Return customer ID
 }
 
 // Count total allocated seats
 int TicketCircularQueue::getAllocatedSeatsCount() const {
     int count = 0;
     
     for (int i = 0; i < capacity; i++) {
         if (seats[i].isAllocated) {
             count++;
         }
     }
     
     return count;
 }
 
 // Count available seats
 int TicketCircularQueue::getAvailableSeatsCount() const {
     return capacity - getAllocatedSeatsCount();
 }
 
 // Check if venue is full
 bool TicketCircularQueue::isFull() const {
     return getAllocatedSeatsCount() == capacity;
 }
 
 // Check if venue is empty
 bool TicketCircularQueue::isEmpty() const {
     return getAllocatedSeatsCount() == 0;
 }
 
 // Display seating arrangement
 void TicketCircularQueue::displaySeating() const {
     // Print header
     cout << "\n----- Venue Seating Arrangement -----" << endl;
     cout << "Total Capacity: " << capacity << " seats" << endl;
     cout << "Allocated: " << getAllocatedSeatsCount() << " | Available: " << getAvailableSeatsCount() <<endl;
     
     // Define section names
     string vipSection = "VIP Section (1-20)";
     string earlyBirdSection = "Early-Bird Section (21-60)";
     string generalSection = "General Section (61-100)";
     
     // Display VIP section
     cout << "\n" << vipSection << ":" <<endl;
     cout <<string(vipSection.length(), '-') <<endl;
     for (int i = 0; i < 20 && i < capacity; i++) {
         cout <<setw(3) << seats[i].seatNumber << (seats[i].isAllocated ? "[X]" : "[ ]");
         if ((i + 1) % 5 == 0) {
             cout <<endl;
         }
     }
     cout <<endl;
     
     // Display Early-Bird section
     cout << "\n" << earlyBirdSection << ":" <<endl;
     cout <<string(earlyBirdSection.length(), '-') <<endl;
     for (int i = 20; i < 60 && i < capacity; i++) {
         cout <<setw(3) << seats[i].seatNumber << (seats[i].isAllocated ? "[X]" : "[ ]");
         if ((i + 1) % 5 == 0) {
             cout <<endl;
         }
     }
     cout <<endl;
     
     // Display General section  
     cout << "\n" << generalSection << ":" <<endl;
     cout <<string(generalSection.length(), '-') <<endl;
     for (int i = 60; i < capacity; i++) {
         cout << setw(3) << seats[i].seatNumber << (seats[i].isAllocated ? "[X]" : "[ ]");
         if ((i + 1) % 5 == 0) {
             cout <<endl;
         }
     }
     cout <<endl;
     
     // Legend
     cout << "Legend: [X] = Allocated, [ ] = Available" <<endl;
 }