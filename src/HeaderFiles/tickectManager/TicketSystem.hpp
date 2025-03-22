/*
 This file defines the main TicketSystem class that integrates all data
 structures (Priority Queue, Queue, Circular Queue, Stack) to provide a
 complete ticket sales and spectator management system.
 */

 #ifndef TICKET_SYSTEM_HPP
 #define TICKET_SYSTEM_HPP
 
 #include "Models.hpp"
 #include "PriorityQueue.hpp"
 #include "Queue.hpp"
 #include "CircularQueue.hpp"
 #include "Stack.hpp"
 #include "DynamicArray.hpp"
 #include <string>
 #include <fstream>
 #include <sstream>
 #include <ctime>
 
 using namespace std;
 
 /*
    This class integrates all components of the ticket system and provides
    a unified interface for ticket sales, refunds, and entry management.
  */
 class TicketSystem {
 private:
     
     // For processing ticket requests
     PriorityQueue ticketRequests;
     // Array of entry gates (queues)
     Queue* entryGates;
     // For tracking seat availability
     CircularQueue* venueCapacity;
     // For tracking all transactions
     Stack transactionHistory;
     
     DynamicArray<Customer> customers;// All customers
     DynamicArray<Match> matches;// All matches
     DynamicArray<Ticket> tickets;         // All tickets    
     
     // System configuration
     int numGates;                         // Number of entry gates
     int currentSalesPhase;// 3=VIP only, 2=Early-Bird, 1=General
     string dataDirectory;// Directory for data files


     
     // IDs and counters
     int nextTicketID;                     
     int nextTransactionID;               
    
     bool loadData();// Load data from CSV files
     
     
     bool saveData();// Save data to CSV files
     
    // Generate a unique ticket ID
     string generateTicketID();
     
    // Generate a unique transaction ID
     string generateTransactionID();
     
     // Find a customer by ID
     Customer* findCustomer(const string& customerID);
     
    // Find a match by ID
     Match* findMatch(const string& matchID);
     // Find a ticket by ID
     Ticket* findTicket(const string& ticketID);
     // Check if a customer is allowed in the current sales phase
     bool isCustomerAllowedInPhase(int customerPriority) const;
     
     // Record a transaction
     void recordTransaction(const string& customerID, const string& ticketID, const string& type);

    // Update the available seats for a match
     bool updateMatchAvailableSeats(const string& matchID, int change);
     
 public:
     // Constructor
     TicketSystem(const string& dataDir = "../", int gates = 4);
     
     // Destructor
     ~TicketSystem();
     
     // Initialize the ticket system
     bool initialize();
     
     // Set the sales phase
     void setSalesPhase(int phase);
     // Get the sales phase
     int getSalesPhase() const;
     
     //Add a new customer
     string addCustomer(const string& name, const string& email, 
                            const string& phone, int priority);
     /*// Add a new match
     string addMatch(const string& player1, const string& player2,
                         const string& date, const string& time,
                         const string& court, int capacity);*/
     
     // Request a ticket for a match
     bool requestTicket(const string& customerID, const string& matchID);
     
     // Process the next request in the queue
     bool processNextRequest();
     
     // Process all requests in the queue
     int processAllRequests();
     
     // Refund a ticket
     bool refundTicket(const string& ticketID);
     
     // Process an entry
     bool processEntry(const string& ticketID, int gateNumber);
     
     // Add a spectator to an entry queue
     bool addToEntryQueue(const string& customerID, const string& ticketID, int gateNumber);
     
     // Process next entry
     bool processNextEntry(int gateNumber);
     
     // Display matches
     void displayMatches() const;
     
     // Display customers
     void displayCustomers() const;
     

    // Check if a match is sellable
    bool isMatchSellable(const Match* match) const;
        
     // Display tickets
     void displayTickets() const;
     
     // Display request queue
     void displayRequestQueue() const;
     
     // Display entry queues
     void displayEntryQueues() const;
     
     // Display venue seating
     void displayVenueSeating(const string& matchID) const;
     // Display transaction history
     void displayTransactionHistory(int count = 10) const;
     
     // Search transactions by customer
     void searchTransactionsByCustomer(const string& customerID) const;
     
     // Search transactions by ticket
     void searchTransactionsByTicket(const string& ticketID) const;
 };
 
 #endif