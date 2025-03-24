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
     TicketCircularQueue* venueCapacity;
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
    
public:

    // Constructor
    TicketSystem() : numGates(4), currentSalesPhase(3), dataDirectory("../src/"),
    nextTicketID(1), nextTransactionID(1) {
    
        //Create entry gates (queues)
        entryGates = new Queue[numGates];
        for (int i = 0; i < numGates; i++) {
        entryGates[i] = Queue(i + 1, 50); //Each gate can hold up to 50 people
        }
        
        //Initialize venueCapacity as nullptr (will be created per match)
        venueCapacity = nullptr;
    }

    TicketSystem(const string& dataDir, int gates)
        : numGates(gates), currentSalesPhase(3), dataDirectory(dataDir),
          nextTicketID(1), nextTransactionID(1) {
        
        //Create entry gates (queues)
        entryGates = new Queue[numGates];
        for (int i = 0; i < numGates; i++) {
            entryGates[i] = Queue(i + 1, 50); //Each gate can hold up to 50 people
        }
        
        //Initialize venueCapacity as nullptr (will be created per match)
        venueCapacity = nullptr;
    }

    // Destructor
    ~TicketSystem() {
        // Save data to files
        saveData();
        
        //Clean up allocated memory
        delete[] entryGates;
        
        if (venueCapacity != nullptr) {
            delete venueCapacity;
        }
    }

    bool initialize() {   
         
        // Load data from files
        return loadData();
    }

     bool loadData() {
        // Clear existing data
        customers.clear();
        matches.clear();
        tickets.clear();
       
        // Load customers
        ifstream customersFile( dataDirectory + "csv/customers.csv");
    
        if (customersFile.is_open()) {
            string line;
            // Skip header
            getline(customersFile, line);
            
            while (getline(customersFile, line)) {
                stringstream ss(line);
                string id, name, email, phone, priorityStr;
                
                getline(ss, id, ',');
                getline(ss, name, ',');
                getline(ss, email, ',');
                getline(ss, phone, ',');
                getline(ss, priorityStr, ',');
                
                int priority = stoi(priorityStr);
                
                customers.push_back(Customer(id, name, email, phone, priority));
            }
    
            
            customersFile.close();
        }
        else {
            cerr << "Error: Could not open customers.csv for reading" << endl;
            return false;
        }
        
        // Load matches from TennisTournament.csv
        ifstream matchesFile(dataDirectory + "csv/TennisTournament.csv");
        // cout << dataDirectory + "csv/TennisTournament.csv" << endl;
        if (matchesFile.is_open()) {
            string line;
            // Skip header
            getline(matchesFile, line);
            
            // In your loadData() method, when reading matches from CSV:
            while (std::getline(matchesFile, line)) {
                std::stringstream ss(line);
                std::string matchID, player1, player2, status, winner;
                
                // Use getline with comma delimiter to properly handle columns
                std::getline(ss, matchID, ',');
                std::getline(ss, player1, ',');
                std::getline(ss, player2, ',');
                std::getline(ss, status, ',');
                
                // Handle potential missing winner field
                if (std::getline(ss, winner, ',')) {
                    // Winner field is present
                } else {
                    // No winner field, set to empty
                    winner = "";
                }
                
                // Trim whitespace
                while (!matchID.empty() && std::isspace(matchID.front())) matchID.erase(0, 1);
                while (!matchID.empty() && std::isspace(matchID.back())) matchID.pop_back();
                
                while (!status.empty() && std::isspace(status.front())) status.erase(0, 1);
                while (!status.empty() && std::isspace(status.back())) status.pop_back();
                
                // Create match with consistent initialization
                Match match;
                match.matchID = matchID;
                match.player1 = player1;
                match.player2 = player2;
                match.status = status;
                match.winner = winner;
                match.totalSeats = 100;
                match.availableSeats = 100;
                
                matches.push_back(match);
            }
            
            matchesFile.close();
        }
    
    
        // Load transactions
        ifstream transactionsFile(dataDirectory + "csv/transactions.csv");
        if (transactionsFile.is_open()) {
            string line;
            // Skip header
            getline(transactionsFile, line);
            
            while (getline(transactionsFile, line)) {
                stringstream ss(line);
                string id, customerID, ticketID, type, timestampStr;
                
                getline(ss, id, ',');
                getline(ss, customerID, ',');
                getline(ss, ticketID, ',');
                getline(ss, type, ',');
                getline(ss, timestampStr, ',');
                
                // Convert timestamp string to time_t
                time_t timestamp = stol(timestampStr);
                
                // Create transaction and push to stack
                Transaction transaction(id, customerID, ticketID, type);
                transaction.timestamp = timestamp;
                
                transactionHistory.push(transaction);
            }
        }
        
        // Load tickets
        ifstream ticketsFile(dataDirectory + "csv/tickets.csv");
        if (ticketsFile.is_open()) {
            string line;
            // Skip header
            getline(ticketsFile, line);
            
            while (getline(ticketsFile, line)) {
                stringstream ss(line);
                string id, matchID, customerID, categoryStr, priceStr, seat, status, timeStr;
                
                getline(ss, id, ',');
                getline(ss, matchID, ',');
                getline(ss, customerID, ',');
                getline(ss, categoryStr, ',');
                getline(ss, priceStr, ',');
                getline(ss, seat, ',');
                getline(ss, status, ',');
                getline(ss, timeStr, ',');
                
                int category = stoi(categoryStr);
                double price = stod(priceStr);
                time_t purchaseTime = stol(timeStr);
                
                Ticket ticket(id, matchID, customerID, category, price, seat);
                ticket.status = status;
                ticket.purchaseTime = purchaseTime;
                
                tickets.push_back(ticket);
                
                // Update next ticket ID
                int ticketNum = stoi(id.substr(1));
                if (ticketNum >= nextTicketID) {
                    nextTicketID = ticketNum + 1;
                }
                
                // Update available seats for the match
                updateMatchAvailableSeats(matchID, -1);
            }
            
            ticketsFile.close();
        }

        
        // If no customers, create some sample data
        if (customers.empty()) {
            addCustomer("John Smith", "john@example.com", "555-1234", 3); // VIP
            addCustomer("Sarah Jones", "sarah@example.com", "555-5678", 2); // Early-Bird
            addCustomer("Mike Johnson", "mike@example.com", "555-9012", 1); // General
        }
        
        return true;
    }
    
    
    
    
     bool saveData() {
        // Save customers
        ofstream customersFile(dataDirectory + "csv/customers.csv");
        if (customersFile.is_open()) {
            customersFile << "CustomerID,Name,Email,Phone,PriorityLevel" << endl;
            for (const Customer& customer : customers) {
                customersFile << customer.customerID << ","
                              << customer.name << ","
                              << customer.email << ","
                              << customer.phone << ","
                              << customer.priorityLevel << endl;
            }
            customersFile.close();
        } else {
            cerr << "Error: Could not open customers.csv for writing" << endl;
            return false;
        }
    
        //Save tickets 
        ofstream ticketsFile(dataDirectory + "csv/tickets.csv");
        if (ticketsFile.is_open()) {
            ticketsFile << "TicketID,MatchID,CustomerID,Category,Price,SeatNumber,Status,PurchaseTime" << endl;
            for (const Ticket& ticket : tickets) {
                ticketsFile << ticket.ticketID << ","
                            << ticket.matchID << ","
                            << ticket.customerID << ","
                            << ticket.category << ","
                            << ticket.price << ","
                            << ticket.seatNumber << ","
                            << ticket.status << ","
                            << ticket.purchaseTime << endl;
            }
            ticketsFile.close();
        } else {
            cerr << "Error: Could not open tickets.csv for writing" << endl;
            return false;
        }
    
        // Save transactions without copying the stack
        ofstream transactionsFile(dataDirectory + "csv/transactions.csv");
        if (transactionsFile.is_open()) {
            transactionsFile << "TransactionID,CustomerID,TicketID,Type,Timestamp" << endl;
    
            //Create a temporary array to hold transactions
            int transCount = transactionHistory.getSize(); 
            Transaction* transactions = new Transaction[transCount];
            int count = 0;
    
            //Use a temporary stack to restore original transactions later
            Stack tempStack;
    
            //Pop all transactions from the original stack
            while (!transactionHistory.isEmpty()) {
                Transaction trans = transactionHistory.pop();
                transactions[count++] = trans;
                tempStack.push(trans);
            }
    
            //Restore the original stack from tempStack
            while (!tempStack.isEmpty()) {
                transactionHistory.push(tempStack.pop());
            }
    
            //Write the transactions to file in reverse order (if needed)
            for (int i = count - 1; i >= 0; i--) {
                transactionsFile << transactions[i].transactionID << ","
                                   << transactions[i].customerID << ","
                                   << transactions[i].ticketID << ","
                                   << transactions[i].type << ","
                                   << transactions[i].timestamp << endl;
            }
            delete[] transactions;
            transactionsFile.close();
        } else {
            cerr << "Error: Could not open transactions.csv for writing" << endl;
            return false;
        }

    
        return true;
    }
    
     
     // Generate a unique ticket ID
     string generateTicketID() {
         string id = "T" + to_string(nextTicketID++);
         return id;
     }
     
     // Generate a unique transaction ID
     string generateTransactionID() {
         string id = "TR" + to_string(nextTransactionID++);
         return id;
     }
     
     // Find a customer by ID
     Customer* findCustomer(const string& customerID) {
         for (auto& customer : customers) {
             if (customer.customerID == customerID) {
                 return &customer;
             }
         }
         return nullptr;
     }
     
     // Find a match by ID
     Match* findMatch(const string& matchID) {
         for (auto& match : matches) {
             if (match.matchID == matchID) {
                 return &match;
             }
         }
         return nullptr;
     }
     
     // Find a ticket by ID
     Ticket* findTicket(const string& ticketID) {
         for (auto& ticket : tickets) {
             if (ticket.ticketID == ticketID) {
                 return &ticket;
             }
         }
         return nullptr;
     }
     
     // Check if a customer can buy tickets in current sales phase
     bool isCustomerAllowedInPhase(int customerPriority) const {
         // In phase 3 (VIP), only VIP customers can buy tickets
         if (currentSalesPhase == 3 && customerPriority < 3) {
             return false;
         }
         
         // In phase 2 (Early-Bird), only VIP and Early-Bird customers can buy tickets
         if (currentSalesPhase == 2 && customerPriority < 2) {
             return false;
         }
         
         // In phase 1 (General), all customers can buy tickets
         return true;
     }
     
     // Record a transaction
     void recordTransaction(const string& customerID, const string& ticketID, const string& type) {
         Transaction transaction(generateTransactionID(), customerID, ticketID, type);
         transactionHistory.push(transaction);
     }
     
     // Update a match's available seats
     bool updateMatchAvailableSeats(const string& matchID, int change) {
         Match* match = findMatch(matchID);
         if (match == nullptr) {
             cerr << "Error: Match not found: " << matchID << endl;
             return false;
         }
         
         // Check if we're trying to add more tickets than available
         if (match->availableSeats + change < 0) {
             cerr << "Error: Not enough seats available for match: " << matchID << endl;
             return false;
         }
         
         // Update available seats
         match->availableSeats += change;
         return true;
     }
     
     // Set sales phase
     void setSalesPhase(int phase) {
         if (phase < 1 || phase > 3) {
             cerr << "Error: Invalid sales phase: " << phase << endl;
             return;
         }
         
         currentSalesPhase = phase;
         
         string phaseNames[] = {"", "General Sales", "Early-Bird Sales", "VIP Sales"};
         cout << "Sales phase set to: " << phaseNames[phase] << endl;
     }
     
     // Get current sales phase
     int getSalesPhase() const {
         return currentSalesPhase;
     }
     
     // Add a new customer
     string addCustomer(const string& name, const string& email, 
                                          const string& phone, int priority) {
         string customerID = "C" + to_string(customers.size() + 1);
         customers.push_back(Customer(customerID, name, email, phone, priority));
         
         return customerID;
     }
     
    
    bool isMatchSellable(const Match* match) const {
        if (match == nullptr) {
            return false;
        }
        
        // Create copies of strings to work with
        string status = match->status;
        string winner = match->winner;
        string player1 = match->player1;
        string player2 = match->player2;
        
        // Trim whitespace from status
        while (!status.empty() && isspace(status.front())) {
            status.erase(0, 1);
        }
        while (!status.empty() && isspace(status.back())) {
            status.pop_back();
        }
        
        // Trim whitespace from winner
        while (!winner.empty() && (isspace(winner.front()) || winner.front() == '|')) {
            winner.erase(0, 1);
        }
        while (!winner.empty() && (isspace(winner.back()) || winner.back() == '|')) {
            winner.pop_back();
        }
        
        // Trim whitespace from player names
        while (!player1.empty() && isspace(player1.front())) {
            player1.erase(0, 1);
        }
        while (!player1.empty() && isspace(player1.back())) {
            player1.pop_back();
        }
        
        while (!player2.empty() && isspace(player2.front())) {
            player2.erase(0, 1);
        }
        while (!player2.empty() && isspace(player2.back())) {
            player2.pop_back();
        }
        
        return (winner.empty() && 
                status == "Confirmed" && 
                player1 != "Withdrew" && 
                player2 != "Withdrew");
    }
    
    
     bool requestTicket(const string& customerID, const string& matchID) {
        // Validate customer
        Customer* customer = findCustomer(customerID);
        if (customer == nullptr) {
            cerr << "Error: Customer not found: " << customerID << endl;
            return false;
        }
        
        //Validate match
        Match* match = findMatch(matchID);
        if (match == nullptr) {
            cerr << "Error: Match not found: " << matchID << endl;
            return false;
        }
        
        //Check if match is available for ticket sales
        if (!isMatchSellable(match)) {
            cerr << "Error: Tickets are not available for this match: " << matchID << endl;
            return false;
        }
        
        // Check if match has available seats
        if (match->availableSeats <= 0) {
            cerr << "Error: Match is sold out: " << matchID << endl;
            return false;
        }
        
        // Check if customer is allowed in current sales phase
        if (!isCustomerAllowedInPhase(customer->priorityLevel)) {
            cerr << "Error: Customer not allowed in current sales phase: " << customerID << endl;
            return false;
        }
        
        // Check if customer already has a ticket for this match
        for (int i = 0; i < tickets.size(); i++) {
            Ticket ticket = tickets[i];
            if (ticket.customerID == customerID && ticket.matchID == matchID && ticket.status != "REFUNDED") {
                cerr << "Error: Customer already has a ticket for this match: " << customerID << endl;
                return false;
            }
        }
        
        // Create ticket request
        TicketRequest request(*customer, matchID);
        
        // Add to priority queue
        ticketRequests.enqueue(request);
        
        cout << "Ticket request added for customer " << customer->name << " for match " << matchID << endl;
        return true;
    }
     
    
    
     //Process next ticket request
     bool processNextRequest() {
        try {
            // Check if queue is empty
            if (ticketRequests.isEmpty()) {
                std::cout << "No ticket requests to process." << std::endl;
                return false;
            }
            
            // Get next request
            TicketRequest request = ticketRequests.dequeue();
            std::cout << "Processing request for match: " << request.matchID << std::endl;
            
            // Validate match
            Match* match = findMatch(request.matchID);
            if (match == nullptr) {
                std::cerr << "Error: Match not found: " << request.matchID << std::endl;
                return false;
            }
            
            std::cout << "Match found: " << match->matchID << std::endl;
            
            // Check if match is sellable
            if (!isMatchSellable(match)) {
                std::cerr << "Error: Tickets are not available for this match: " << request.matchID << std::endl;
                return false;
            }
            
            std::cout << "Match is sellable" << std::endl;
            
            // Check if match has available seats
            if (match->availableSeats <= 0) {
                std::cerr << "Error: Match is sold out: " << request.matchID << std::endl;
                return false;
            }
            
            // Calculate ticket price based on customer priority
            double price;
            switch (request.customer.priorityLevel) {
                case 3: // VIP
                    price = 150.0;
                    break;
                case 2: // Early-Bird
                    price = 100.0;
                    break;
                default: // General
                    price = 50.0;
            }
            
            // Initialize venue capacity if needed
            if (venueCapacity == nullptr) {
                std::cout << "Initializing venue capacity..." << std::endl;
                // Use a consistent initialization regardless of match ID
                venueCapacity = new TicketCircularQueue(100, 1);
                std::cout << "Venue capacity initialized" << std::endl;
            }
            
            // Get next available seat
            std::cout << "Getting next available seat..." << std::endl;
            int seatNumber = venueCapacity->getNextAvailableSeat(request.customer.priorityLevel);
            if (seatNumber == -1) {
                std::cerr << "Error: No suitable seats available for priority level: " << request.customer.priorityLevel << std::endl;
                return false;
            }
            
            std::cout << "Seat allocated: " << seatNumber << std::endl;
            
            // Create ticket
            std::string ticketID = generateTicketID();
            Ticket ticket(ticketID, request.matchID, request.customer.customerID, request.customer.priorityLevel, price, std::to_string(seatNumber));
            tickets.push_back(ticket);
            
            // Allocate seat in the venue
            venueCapacity->allocateSeat(seatNumber, request.customer.customerID, ticketID);
            
            // Update match available seats
            updateMatchAvailableSeats(request.matchID, -1);
            
            // Record transaction
            recordTransaction(request.customer.customerID, ticketID, "PURCHASE");
            
            std::cout << "Processed ticket request for " << request.customer.name 
                      << " - Ticket: " << ticketID 
                      << " - Seat: " << seatNumber 
                      << " - Price: $" << price << std::endl;
            
            return true;
        }
        catch (const std::exception& e) {
            std::cerr << "Exception caught in processNextRequest: " << e.what() << std::endl;
            return false;
        }
        catch (...) {
            std::cerr << "Unknown exception caught in processNextRequest" << std::endl;
            return false;
        }
    }
     
     // Process all ticket requests
     int processAllRequests() {
         int count = 0;
         
         // Process requests until queue is empty
         while (!ticketRequests.isEmpty()) {
             if (processNextRequest()) {
                 count++;
             }
         }
         
         cout << "Processed " << count << " ticket requests." << endl;
         return count;
     }
     
     // Refund a ticket
     bool refundTicket(const string& ticketID) {
         // Find ticket
         Ticket* ticket = findTicket(ticketID);
         if (ticket == nullptr) {
             cerr << "Error: Ticket not found: " << ticketID << endl;
             return false;
         }
         
         // Check if ticket is already refunded or used
         if (ticket->status != "SOLD") {
             cerr << "Error: Ticket already " << ticket->status << ": " << ticketID << endl;
             return false;
         }
         
         // Find match
         Match* match = findMatch(ticket->matchID);
         if (match == nullptr) {
             cerr << "Error: Match not found: " << ticket->matchID << endl;
             return false;
         }
         
         // Use isMatchSellable method to check if match is still available for tickets
        if (!isMatchSellable(match)) {
            cerr << "Error: Cannot refund ticket - match is no longer available for ticket operations: " << ticket->matchID << endl;
            return false;
        }
         
         // Update ticket status
         ticket->status = "REFUNDED";
         
         // Deallocate seat
         if (venueCapacity != nullptr) {
             venueCapacity->deallocateSeat(stoi(ticket->seatNumber));
         }
         
         // Update match available seats
         updateMatchAvailableSeats(ticket->matchID, 1);
         
         // Record transaction
         recordTransaction(ticket->customerID, ticketID, "REFUND");
         
         cout << "Refunded ticket: " << ticketID << endl;
         return true;
     }
     
    
    
     // Process spectator entry
     bool processEntry(const string& ticketID, int gateNumber) {
         // Validate gate number
         if (gateNumber < 1 || gateNumber > numGates) {
             cerr << "Error: Invalid gate number: " << gateNumber << endl;
             return false;
         }
         
         // Find ticket
         Ticket* ticket = findTicket(ticketID);
         if (ticket == nullptr) {
             cerr << "Error: Ticket not found: " << ticketID << endl;
             return false;
         }
         
         // Check if ticket is valid
         if (ticket->status != "SOLD") {
             cerr << "Error: Invalid ticket status: " << ticket->status << endl;
             return false;
         }
         
         // Find match
         Match* match = findMatch(ticket->matchID);
         if (match == nullptr) {
             cerr << "Error: Match not found: " << ticket->matchID << endl;
             return false;
         }
         
         //Check if match is sellable (available for tickets and entry)
        if (!isMatchSellable(match)) {
            cerr << "Error: Match is not available for entry: " << match->status <<endl;
            return false;
        }
         
         // Check if gate is appropriate for ticket category
         bool validGate = false;
         switch (ticket->category) {
             case 3: // VIP
                 validGate = (gateNumber == 1); // VIP gate
                 break;
             case 2: // Early-Bird
                 validGate = (gateNumber == 2 || gateNumber == 3); // Early-Bird gates
                 break;
             case 1: // General
                 validGate = (gateNumber == 4); // General gate
                 break;
         }
         
         if (!validGate) {
             cerr << "Error: Invalid gate for ticket category: " << gateNumber << endl;
             return false;
         }
         
         // Update ticket status
         ticket->status = "USED";
         
         // Record transaction
         recordTransaction(ticket->customerID, ticketID, "ENTRY");
         
         cout << "Processed entry for ticket: " << ticketID << " at gate " << gateNumber << endl;
         return true;
     }
     
     
     bool addToEntryQueue(const string& customerID, const string& ticketID, int gateNumber) {
        // Find ticket
        Ticket* ticket = findTicket(ticketID);
        if (ticket == nullptr) {
            cerr << "Error: Ticket not found: " << ticketID << endl;
            return false;
        }
        
        // Check if ticket is valid
        if (ticket->status != "SOLD") {
            cerr << "Error: Invalid ticket status: " << ticket->status << endl;
            return false;
        }
        
        // Check if customer matches ticket
        if (ticket->customerID != customerID) {
            cerr << "Error: Ticket does not belong to customer: " << customerID << endl;
            return false;
        }
        
        // Determine appropriate gate based on ticket category
        int appropriateGate;
        string gateType;
        
        switch (ticket->category) {
            case 3: // VIP
                appropriateGate = 1;
                gateType = "VIP";
                break;
            case 2: // Early-Bird
                appropriateGate = (gateNumber == 2 || gateNumber == 3) ? gateNumber : 2;
                gateType = "Early-Bird";
                break;
            case 1: // General
                appropriateGate = 4;
                gateType = "General";
                break;
            default:
                cerr << "Error: Unknown ticket category: " << ticket->category << endl;
                return false;
        }
        
        // Check if specified gate is valid for this ticket category
        if (gateNumber != appropriateGate) {
            cout << "Notice: Redirecting from gate " << gateNumber 
                      << " to gate " << appropriateGate 
                      << " appropriate for " << gateType << " tickets." << endl;
            gateNumber = appropriateGate;
        }
        
        // Add to queue
        bool success = entryGates[gateNumber - 1].enqueue(customerID, ticketID);
        
        if (success) {
            cout << "Added spectator to gate " << gateNumber << " queue." << endl;
        }
        
        return success;
    }
     
     // Process next spectator at a gate
     bool processNextEntry(int gateNumber) {
         // Validate gate number
         if (gateNumber < 1 || gateNumber > numGates) {
             cerr << "Error: Invalid gate number: " << gateNumber << endl;
             return false;
         }
         
         // Check if queue is empty
         if (entryGates[gateNumber - 1].isEmpty()) {
             cout << "No spectators in gate " << gateNumber << " queue." << endl;
             return false;
         }
         
         // Dequeue next spectator
            EntryRecord spectator = entryGates[gateNumber - 1].dequeue();
            string customerID = spectator.customerID;
            string ticketID = spectator.tickectID;
         
         // Process entry
         if (processEntry(ticketID, gateNumber)) {
             cout << "Processed entry for next spectator at gate " << gateNumber << "." << endl;
             return true;
         }
         
         return false;
     }
    
    // Update your displayMatches method to handle all match types
    void displayMatches() const {
        const_cast<TicketSystem*>(this)->loadData();
     
        // Check if matches vector is empty
        if (matches.size() == 0) {
            std::cout << "No matches available." << std::endl;
            return;
        }
        
        // Count sellable matches without filtering by prefix
        int sellableCount = 0;
        for (int i = 0; i < matches.size(); i++) {
            Match match = matches[i];
            if (isMatchSellable(&match)) {
                sellableCount++;
            }
        }
        
        if (sellableCount == 0) {
            std::cout << "No matches available for ticket sales." << std::endl;
            return;
        }
        
        // Print header
        std::cout << "\n============= MATCHES AVAILABLE FOR TICKET SALES =============" << std::endl;
        std::cout << std::setw(8) << "Match ID" << " | " 
                  << std::setw(15) << "Player 1" << " | " 
                  << std::setw(15) << "Player 2" << " | " 
                  << std::setw(10) << "Status" << " | " 
                  << std::setw(10) << "Available" << std::endl;
        std::cout << std::string(70, '-') << std::endl;
        
        // Display each match that is sellable, regardless of ID format
        for (int i = 0; i < matches.size(); i++) {
            Match match = matches[i];
            
            // Check if match is sellable using our method
            if (isMatchSellable(&match)) {
                std::cout << std::setw(8) << match.matchID << " | " 
                          << std::setw(15) << match.player1 << " | " 
                          << std::setw(15) << match.player2 << " | " 
                          << std::setw(10) << match.status << " | " 
                          << std::setw(10) << match.availableSeats << std::endl;
            }
        }
        
        std::cout << std::string(70, '-') << std::endl;
        std::cout << "Available matches: " << sellableCount << std::endl;
    }
    
    //display customers
    void displayCustomers() const {
        // Check if customers vector is empty
        if (customers.size() == 0) {
            cout << "No customers available." << endl;
            return;
        }
        
        // Print header
        cout << "\n========================== CUSTOMERS ==========================" << endl;
        cout << setw(10) << "ID" << " | " 
                  << setw(20) << "Name" << " | " 
                  << setw(25) << "Email" << " | " 
                  << setw(15) << "Phone" << " | " 
                  << setw(10) << "Priority" << endl;
        cout << string(90, '-') << endl;
        
        // Display each customer using index-based loop
        for (int i = 0; i < customers.size(); i++) {
            const Customer& customer = customers[i];
            string priorityLabel;
            switch (customer.priorityLevel) {
                case 3:
                    priorityLabel = "VIP";
                    break;
                case 2:
                    priorityLabel = "Early-Bird";
                    break;
                case 1:
                    priorityLabel = "General";
                    break;
                default:
                    priorityLabel = "Unknown";
            }
            
            cout << setw(10) << customer.customerID << " | " 
                      << setw(20) << customer.name << " | " 
                      << setw(25) << customer.email << " | " 
                      << setw(15) << customer.phone << " | " 
                      << setw(10) << priorityLabel << endl;
        }
        
        cout << string(90, '-') << endl;
        cout << "Total customers: " << customers.size() << endl;
    }
    
    //display tickets
    void displayTickets() const {
        // Check if tickets vector is empty
        if (tickets.size() == 0) {
            cout << "No tickets available." << endl;
            return;
        }
        
        // Print header
        cout << "\n======================================== TICKETS =========================================" << endl;
        cout << setw(10) << "Ticket ID" << " | " 
                  << setw(10) << "Match ID" << " | " 
                  << setw(12) << "Customer ID" << " | " 
                  << setw(10) << "Category" << " | " 
                  << setw(10) << "Price" << " | " 
                  << setw(10) << "Seat" << " | " 
                  << setw(10) << "Status" << endl;
        cout << string(90, '-') << endl;
        
        // Display each ticket using index-based loop
        for (int i = 0; i < tickets.size(); i++) {
            const Ticket& ticket = tickets[i];
            string categoryLabel;
            switch (ticket.category) {
                case 3:
                    categoryLabel = "VIP";
                    break;
                case 2:
                    categoryLabel = "Early-Bird";
                    break;
                case 1:
                    categoryLabel = "General";
                    break;
                default:
                    categoryLabel = "Unknown";
            }
            
            cout << setw(10) << ticket.ticketID << " | " 
                      << setw(10) << ticket.matchID << " | " 
                      << setw(12) << ticket.customerID << " | " 
                      << setw(10) << categoryLabel << " | " 
                      //<< setw(10) << "$" << fixed << setprecision(2) << ticket.price << " | " 
                      << setw(10) << ("$" + to_string(int(ticket.price))) << " | "
                      << setw(10) << ticket.seatNumber << " | " 
                      << setw(10) << ticket.status << endl;
        }
        
        cout << string(90, '-') << endl;
        cout << "Total tickets: " << tickets.size() << endl;
    }
    
    
     void displayRequestQueue() const {
         ticketRequests.display();
     }
     
     void displayEntryQueues() const {
         for (int i = 0; i < numGates; i++) {
             entryGates[i].display();
         }
     }
     
     void displayVenueSeating(const string& matchID) const {
        // Validate match exists
        Match* match = nullptr;
        for (int i = 0; i < matches.size(); i++) {
            if (matches[i].matchID == matchID) {
                Match tempMatch = matches[i];
                match = &tempMatch;
                break;
            }
        }
        
        if (match == nullptr) {
            cout << "Error: Match with ID '" << matchID << "' was not found." << endl;
            return;
        }
        
        // For viewing venue seating without tickets sold, initialize with default capacity
        if (venueCapacity == nullptr) {
            // This cast is necessary because displayVenueSeating is const
            TicketSystem* nonConstThis = const_cast<TicketSystem*>(this);
            
            // Initialize with default category
            int category = 1;
            string prefix = matchID.substr(0, 3);
            if (prefix == "QUL") {
                category = 3;
            } else if (prefix == "ROU") {
                category = 2;
            } else if (prefix == "Semi" || prefix == "FINA") {
                category = 3;
            }
            
            // Create new venue capacity
            nonConstThis->venueCapacity = new TicketCircularQueue(100, category);
            
            // Load existing ticket data for this match
            for (int i = 0; i < tickets.size(); i++) {
                const Ticket& ticket = tickets[i];
                
                // Only allocate seats for tickets that are for this match and are SOLD (not REFUNDED or USED)
                if (ticket.matchID == matchID && ticket.status == "SOLD") {
                    // Convert seat number to integer
                    int seatNumber = stoi(ticket.seatNumber);
                    
                    // Allocate the seat in the venue
                    nonConstThis->venueCapacity->allocateSeat(seatNumber, ticket.customerID, ticket.ticketID);
                }
            }
            
            cout << "Initialized venue seating arrangement for match " << matchID 
                 << " with existing ticket data." << endl;
        }
        
        // Display the seating arrangement
        venueCapacity->displaySeating();
    }
    
    //display transaction history
    void displayTransactionHistory(int count) const {
        // Validate count parameter
        if (count <= 0) {
            cout << "Error: Number of transactions must be greater than zero." << endl;
            return;
        }
        
        // Check if transaction history exists
        if (transactionHistory.getSize() == 0) {
            cout << "No transaction history available." << endl;
            return;
        }
        
        // Display transaction history
        cout << "\n----- Recent Transaction History -----" << endl;
        transactionHistory.displayRecent(count);
    }
    
    //display customer transactions
    void searchTransactionsByCustomer(const string& customerID) const {
        // Validate customer ID
        if (customerID.empty()) {
            cout << "Error: Customer ID cannot be empty." << endl;
            return;
        }
        
        // Check if customer exists
        bool customerExists = false;
        for (int i = 0; i < customers.size(); i++) {
            if (customers[i].customerID == customerID) {
                customerExists = true;
                break;
            }
        }
        
        if (!customerExists) {
            cout << "Error: Customer with ID '" << customerID << "' was not found." << endl;
            return;
        }
        
        // Display customer transactions
        cout << "\n----- Transactions for Customer " << customerID << " -----" << endl;
        transactionHistory.searchByCustomer(customerID);
    }
    
    // display ticket transactions
    void searchTransactionsByTicket(const string& ticketID) const {
        // Validate ticket ID
        if (ticketID.empty()) {
            cout << "Error: Ticket ID cannot be empty." << endl;
            return;
        }
        
        // Check if ticket exists
        bool ticketExists = false;
        for (int i = 0; i < tickets.size(); i++) {
            if (tickets[i].ticketID == ticketID) {
                ticketExists = true;
                break;
            }
        }
        
        if (!ticketExists) {
            cout << "Error: Ticket with ID '" << ticketID << "' was not found." << endl;
            return;
        }
        
        // Display ticket transactions
        cout << "\n----- Transactions for Ticket " << ticketID << " -----" << endl;
        transactionHistory.searchByTicket(ticketID);
    }
 };
 
 #endif