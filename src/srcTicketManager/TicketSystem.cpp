/*
 This file implements the TicketSystem class which integrates all components
 of the ticket sales and spectator management system.
 */

 #include "../HeaderFiles/tickectManager/TicketSystem.hpp"
 #include <iostream>
 #include <iomanip>
 #include <fstream>
 #include <sstream>
 #include <algorithm>
 #include <ctime>
 using namespace std;
 // Constructor
 TicketSystem::TicketSystem(const string& dataDir, int gates)
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
 TicketSystem::~TicketSystem() {
     // Save data to files
     saveData();
     
     //Clean up allocated memory
     delete[] entryGates;
     
     if (venueCapacity != nullptr) {
         delete venueCapacity;
     }
 }
 
 // Initialize the system
 bool TicketSystem::initialize() {
     // Create data directory if it doesn't exist
     //string command = "mkdir -p " + dataDirectory;
     string command = "mkdir -p " + dataDirectory + "../csv";
     system(command.c_str());
     cout << "Data directory created: " << command << endl;
     
     
     // Load data from files
     return loadData();
 }
 
 // Load data from files
 bool TicketSystem::loadData() {
    // Clear existing data
    customers.clear();
    matches.clear();
    tickets.clear();
    
    // Load customers
    ifstream customersFile(dataDirectory + "csv/customers.csv");
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
    
    // Load matches from TennisTournament.csv
    ifstream matchesFile(dataDirectory + "csv/TennisTournament.csv");
    cout << dataDirectory + "/TennisTournament.csv" << endl;
    if (matchesFile.is_open()) {
        string line;
        // Skip header
        getline(matchesFile, line);
        
        while (getline(matchesFile, line)) {
            stringstream ss(line);
            string matchID, player1, player2, status, score1Str, score2Str, winner;
            
            getline(ss, matchID, ',');
            getline(ss, player1, ',');
            getline(ss, player2, ',');
            getline(ss, status, ',');
            getline(ss, score1Str, ',');
            getline(ss, score2Str, ',');
            getline(ss, winner, ',');
            
            // Convert scores to integers (with error handling)
            int score1 = 0, score2 = 0;
            try {
                score1 = stoi(score1Str);
                score2 = stoi(score2Str);
            } catch (...) {
                // Handle conversion errors gracefully
                cerr << "Error converting scores for match " << matchID << endl;
            }
            
            // Create match with fixed venue capacity (100 seats)
            Match match(matchID, player1, player2, status, score1, score2, winner, 100);
            
            matches.push_back(match);
        }
        
        matchesFile.close();
    }
    
    // Load tickets
    ifstream ticketsFile(dataDirectory + "scv/tickets.csv");
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
    
    // If no matches loaded, notify user
    if (matches.size() == 0) {
        cerr << "Warning: No matches found in TennisTournament.csv" << endl;
        cerr << "Please ensure the file exists in the data directory." << endl;
    }
    
    // If no customers, create some sample data
    if (customers.empty()) {
        addCustomer("John Smith", "john@example.com", "555-1234", 3); // VIP
        addCustomer("Sarah Jones", "sarah@example.com", "555-5678", 2); // Early-Bird
        addCustomer("Mike Johnson", "mike@example.com", "555-9012", 1); // General
    }
    
    cout << "Loaded " << customers.size() << " customers, " 
              << matches.size() << " matches, and " 
              << tickets.size() << " tickets." << endl;
    
    return true;
}



 
 // Save data to CSV files
 /*bool TicketSystem::saveData() {
     // Save customers
     ofstream customersFile(dataDirectory + "csv/customers.csv");
     if (customersFile.is_open()) {
         // Write header
         customersFile << "CustomerID,Name,Email,Phone,PriorityLevel" << endl;
         
         // Write data
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
     
     // Save tickets
     ofstream ticketsFile(dataDirectory + "csv/tickets.csv");
     if (ticketsFile.is_open()) {
         // Write header
         ticketsFile << "TicketID,MatchID,CustomerID,Category,Price,SeatNumber,Status,PurchaseTime" << endl;
         
         // Write data
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
     
     // Save transactions
     ofstream transactionsFile(dataDirectory + "csv/transactions.csv");
     if (transactionsFile.is_open()) {
         //extract transactions from the stack
         // Since we can't iterate through a stack without modifying it,
         // use a temporary stack
         Stack tempStack = transactionHistory;
         
         // Write header
         transactionsFile << "TransactionID,CustomerID,TicketID,Type,Timestamp" << endl;
         
         // Write data
         while (!tempStack.isEmpty()) {
             Transaction transaction = tempStack.pop();
             transactionsFile << transaction.transactionID << ","
                             << transaction.customerID << ","
                             << transaction.ticketID << ","
                             << transaction.type << ","
                             << transaction.timestamp << endl;
         }
         
         transactionsFile.close();
     } else {
         cerr << "Error: Could not open transactions.csv for writing" << endl;
         return false;
     }
     
     cout << "Saved " << customers.size() << " customers, " 
               << tickets.size() << " tickets, and transaction history." << endl;
     
     return true;
 }*/

 bool TicketSystem::saveData() {
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

    cout << "Saved " << customers.size() << " customers, " 
         << tickets.size() << " tickets, and transaction history." << endl;

    return true;
}

 
 // Generate a unique ticket ID
 string TicketSystem::generateTicketID() {
     string id = "T" + to_string(nextTicketID++);
     return id;
 }
 
 // Generate a unique transaction ID
 string TicketSystem::generateTransactionID() {
     string id = "TR" + to_string(nextTransactionID++);
     return id;
 }
 
 // Find a customer by ID
 Customer* TicketSystem::findCustomer(const string& customerID) {
     for (auto& customer : customers) {
         if (customer.customerID == customerID) {
             return &customer;
         }
     }
     return nullptr;
 }
 
 // Find a match by ID
 Match* TicketSystem::findMatch(const string& matchID) {
     for (auto& match : matches) {
         if (match.matchID == matchID) {
             return &match;
         }
     }
     return nullptr;
 }
 
 // Find a ticket by ID
 Ticket* TicketSystem::findTicket(const string& ticketID) {
     for (auto& ticket : tickets) {
         if (ticket.ticketID == ticketID) {
             return &ticket;
         }
     }
     return nullptr;
 }
 
 // Check if a customer can buy tickets in current sales phase
 bool TicketSystem::isCustomerAllowedInPhase(int customerPriority) const {
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
 void TicketSystem::recordTransaction(const string& customerID, const string& ticketID, const string& type) {
     Transaction transaction(generateTransactionID(), customerID, ticketID, type);
     transactionHistory.push(transaction);
 }
 
 // Update a match's available seats
 bool TicketSystem::updateMatchAvailableSeats(const string& matchID, int change) {
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
 void TicketSystem::setSalesPhase(int phase) {
     if (phase < 1 || phase > 3) {
         cerr << "Error: Invalid sales phase: " << phase << endl;
         return;
     }
     
     currentSalesPhase = phase;
     
     string phaseNames[] = {"", "General Sales", "Early-Bird Sales", "VIP Sales"};
     cout << "Sales phase set to: " << phaseNames[phase] << endl;
 }
 
 // Get current sales phase
 int TicketSystem::getSalesPhase() const {
     return currentSalesPhase;
 }



 
 // Add a new customer
 string TicketSystem::addCustomer(const string& name, const string& email, 
                                      const string& phone, int priority) {
     string customerID = "C" + to_string(customers.size() + 1);
     customers.push_back(Customer(customerID, name, email, phone, priority));
     
     cout << "Added new customer: " << name << " (" << customerID << ")" << endl;
     return customerID;
 }
 

 // Check if a match is sellable
 bool TicketSystem::isMatchSellable(const Match* match) const {
    if (match == nullptr) {
        return false;
    }
    
    // Create copies of strings to work with
    string status = match->status;
    string winner = match->winner;
    
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
    
    
    //Matches are sellable if status is "Confirmed" and winner is empty
    return (status == "Confirmed" && winner.empty());
}




 bool TicketSystem::requestTicket(const string& customerID, const string& matchID) {
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
 bool TicketSystem::processNextRequest() {
     // Check if queue is empty
     if (ticketRequests.isEmpty()) {
         cout << "No ticket requests to process." << endl;
         return false;
     }
     
     // Get next request
     TicketRequest request = ticketRequests.dequeue();
     
     // Validate match again (in case it changed since request was made)
     Match* match = findMatch(request.matchID);
     if (match == nullptr) {
         cerr << "Error: Match not found: " << request.matchID << endl;
         return false;
     }
     
     // Use isMatchSellable to check if match is available
    if (!isMatchSellable(match)) {
        cerr << "Error: Tickets are not available for this match: " << request.matchID << endl;
        return false;
    }
     
     // Check if match has available seats
     if (match->availableSeats <= 0) {
         cerr << "Error: Match is sold out: " << request.matchID << endl;
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
     
     // Allocate a seat
     if (venueCapacity == nullptr) {
         venueCapacity = new CircularQueue(100, match->matchID == "M1" ? 3 : (match->matchID == "M2" ? 2 : 1));
     }
     
     int seatNumber = venueCapacity->getNextAvailableSeat(request.customer.priorityLevel);
     if (seatNumber == -1) {
         cerr << "Error: No suitable seats available for priority level: " << request.customer.priorityLevel << endl;
         return false;
     }
     
     // Create ticket
     string ticketID = generateTicketID();
     Ticket ticket(ticketID, request.matchID, request.customer.customerID, request.customer.priorityLevel, price, to_string(seatNumber));
     tickets.push_back(ticket);
     
     // Allocate seat in the venue
     venueCapacity->allocateSeat(seatNumber, request.customer.customerID, ticketID);
     
     // Update match available seats
     updateMatchAvailableSeats(request.matchID, -1);
     
     // Record transaction
     recordTransaction(request.customer.customerID, ticketID, "PURCHASE");
     
     cout << "Processed ticket request for " << request.customer.name 
               << " - Ticket: " << ticketID 
               << " - Seat: " << seatNumber 
               << " - Price: $" << price << endl;
     
     return true;
 }
 
 // Process all ticket requests
 int TicketSystem::processAllRequests() {
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
 bool TicketSystem::refundTicket(const string& ticketID) {
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
     
     // Check if match is already completed
     if (match->status == "COMPLETED") {
         cerr << "Error: Cannot refund ticket for completed match: " << ticket->matchID << endl;
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
 bool TicketSystem::processEntry(const string& ticketID, int gateNumber) {
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
     
     // Check if match is scheduled or in progress
     if (match->status != "SCHEDULED" && match->status != "IN_PROGRESS") {
         cerr << "Error: Match is not active: " << match->status << endl;
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
 
 // Add spectator to entry queue
 bool TicketSystem::addToEntryQueue(const string& customerID, const string& ticketID, int gateNumber) {
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
     
     // Check if customer matches ticket
     if (ticket->customerID != customerID) {
         cerr << "Error: Ticket does not belong to customer: " << customerID << endl;
         return false;
     }
     
     // Add to queue
     bool success = entryGates[gateNumber - 1].enqueue(customerID, ticketID);
     
     if (success) {
         cout << "Added spectator to gate " << gateNumber << " queue." << endl;
     }
     
     return success;
 }
 
 // Process next spectator at a gate
 bool TicketSystem::processNextEntry(int gateNumber) {
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


 
void TicketSystem::displayMatches() const {
    // Check if matches vector is empty
    if (matches.size() == 0) {
        cout << "No matches available." << endl;
        return;
    }
    
    // Count sellable matches and collect them
    DynamicArray<Match> sellableMatches(10);
    int sellableCount = 0;
    
    for (int i = 0; i < matches.size(); i++) {
        Match match = matches[i];
        
        // Trim and check the winner field
        string winner = match.winner;
        winner.erase(0, winner.find_first_not_of(" \t\r\n|")); // Left trim
        if (winner.length() > 0) {
            winner.erase(winner.find_last_not_of(" \t\r\n|") + 1); // Right trim
        }
        
        // Matches are sellable if status is "Confirmed" and no winner
        if (match.status.find("Confirmed") != string::npos && winner.empty()) {
            sellableMatches.push_back(match);
            sellableCount++;
        }
    }
    
    if (sellableCount == 0) {
        cout << "No matches available for ticket sales." << endl;
        return;
    }
    
    // Print header
    cout << "\n============= MATCHES AVAILABLE FOR TICKET SALES =============" << endl;
    cout << setw(8) << "Match ID" << " | " 
              << setw(15) << "Player 1" << " | " 
              << setw(15) << "Player 2" << " | " 
              << setw(10) << "Status" << " | " 
              << setw(10) << "Available" << endl;
    cout << string(70, '-') << endl;
    
    // Display each sellable match
    for (int i = 0; i < sellableCount; i++) {
        Match match = sellableMatches[i];
        cout << setw(8) << match.matchID << " | " 
                  << setw(15) << match.player1 << " | " 
                  << setw(15) << match.player2 << " | " 
                  << setw(10) << match.status << " | " 
                  << setw(10) << match.availableSeats << endl;
    }
    
    cout << string(70, '-') << endl;
    cout << "Available matches: " << sellableCount << endl;
}


//display customers
void TicketSystem::displayCustomers() const {
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
void TicketSystem::displayTickets() const {
    // Check if tickets vector is empty
    if (tickets.size() == 0) {
        cout << "No tickets available." << endl;
        return;
    }
    
    // Print header
    cout << "\n============================= TICKETS =============================" << endl;
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
                  << setw(10) << "$" << fixed << setprecision(2) << ticket.price << " | " 
                  << setw(10) << ticket.seatNumber << " | " 
                  << setw(10) << ticket.status << endl;
    }
    
    cout << string(90, '-') << endl;
    cout << "Total tickets: " << tickets.size() << endl;
}


 void TicketSystem::displayRequestQueue() const {
     ticketRequests.display();
 }
 
 void TicketSystem::displayEntryQueues() const {
     for (int i = 0; i < numGates; i++) {
         entryGates[i].display();
     }
 }
 
 void TicketSystem::displayVenueSeating(const string& matchID) const {
     if (venueCapacity == nullptr) {
         cout << "No venue seating data available." << endl;
         return;
     }
     
     venueCapacity->displaySeating();
 }
 
 void TicketSystem::displayTransactionHistory(int count) const {
     transactionHistory.displayRecent(count);
 }
 
 void TicketSystem::searchTransactionsByCustomer(const string& customerID) const {
     transactionHistory.searchByCustomer(customerID);
 }
 
 void TicketSystem::searchTransactionsByTicket(const string& ticketID) const {
     transactionHistory.searchByTicket(ticketID);
 }