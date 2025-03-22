/*
 Defines all data structures needed for the ticket management system
 This file contains the structure definitions that model real world entities
 in the APU Tennis Championship ticket system, including customers, matches,
 tickets, and transactions.
 */

 #ifndef MODELS_HPP
 #define MODELS_HPP
 
 #include <string>
 #include <ctime>

 using namespace std;
 
 /**
    
    Stores all customer related information including their priority level,
    which is essential for the Priority Queue implementation.
  */

 struct Customer {
    string customerID;    
    string name;          
    string email;         
    string phone;        
    int priorityLevel; //3=VIP, 2=Early Bird, 1=General
     
     // Default constructor with initializer list
     Customer() : customerID(""), name(""), email(""), phone(""), priorityLevel(1) {}
     
     //Parameterized constructor
     Customer(string id, string n, string e, string p, int priority) 
         : customerID(id), name(n), email(e), phone(p), priorityLevel(priority) {}
 };
 
  struct Match {
      string matchID;       
      string player1;       
      string player2;       
      string status;        
      int score1;                
      int score2;                
      string winner;        
      int totalSeats;            
      int availableSeats;        
      
      // Default constructor
      Match() : matchID(""), player1(""), player2(""), status(""), 
                score1(0), score2(0), winner(""), totalSeats(100), availableSeats(100) {}
      
      // Parameterized constructor
      Match(string id, string p1, string p2, string s, 
            int s1, int s2, string w, int seats = 100) 
          : matchID(id), player1(p1), player2(p2), status(s), 
            score1(s1), score2(s2), winner(w), totalSeats(seats), availableSeats(seats) {}
  };


 /*
  Contains all ticket details including price,seat assignment and status.
  */
 struct Ticket {
     string ticketID;      
     string matchID;       
     string customerID;    
     int category; //3=VIP, 2=Early-Bird, 1=General
     double price;              
     string seatNumber;    
     string status; //SOLD, USED, REFUNDED
     time_t purchaseTime;//Time of purchase (seconds since epoch)
     
     // Default constructor
     Ticket() : ticketID(""), matchID(""), customerID(""), category(1), 
                price(0.0), seatNumber(""), status("SOLD"), purchaseTime(time(nullptr)) {}
     
     // Parameterized constructor
     Ticket(string tid, string mid, string cid, int cat, 
            double p, string seat) 
         : ticketID(tid), matchID(mid), customerID(cid), category(cat), 
           price(p), seatNumber(seat), status("SOLD"), purchaseTime(time(nullptr)) {}
 };
 
    /**
    Represents a request to purchase tickets
    Used in the priority queue to process requests based on customer priority.
  */
 struct TicketRequest {
     Customer customer;     
     string matchID;       
     time_t requestTime;       
     
     // Default constructor
     TicketRequest() : customer(), matchID(""), requestTime(time(nullptr)) {}
     
     // Parameterized constructor
     TicketRequest(Customer c, string m) 
         : customer(c), matchID(m), requestTime(time(nullptr)) {}
 };
 
 /*
  Records a ticket related transaction
  Provides an audit trail of all system activities.
  */
 struct Transaction {
     string transactionID; 
     string customerID;    
     string ticketID;      
     string type;        //PURCHASE, REFUND, ENTRY, EXIT
     time_t timestamp;  //When the transaction occurred
     
     // Default constructor
     Transaction() : transactionID(""), customerID(""), ticketID(""), 
                    type(""), timestamp(time(nullptr)) {}
     
     // Parameterized constructor
     Transaction(string id, string cid, string tid, string t) 
         : transactionID(id), customerID(cid), ticketID(tid), type(t), 
           timestamp(time(nullptr)) {}
 };

 //represent a record of spectator in the enrty queue
 struct EntryRecord{
     string customerID;
     string tickectID;

    // Default constructor
     EntryRecord() : customerID(""), tickectID("") {}
    // Parameterized constructor
     EntryRecord(string cid, string tid): customerID(cid), tickectID(tid) {}
 };
 
 #endif