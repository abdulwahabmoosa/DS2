#ifndef TICKET_MANAGEMENT_HPP
#define TICKET_MANAGEMENT_HPP

#include "TicketSystem.hpp"


  //This class provides the interface to the ticket system that can be called
  //from the main application menu.
 
class TicketManagement {
private:
    TicketSystem ticketSystem;
public:    

    // Constructor
  TicketManagement() {
    if (!ticketSystem.initialize()) {
        cerr << "Error initializing ticket system." << endl;
    }
  }
  
  // Main menu method that will be called from the central application
  void Menu() {
    int choice = 0;
    
    while (choice != 6) {
        clearScreen();
        cout << "=============================================" << endl;
        cout << "  APU TENNIS CHAMPIONSHIP MANAGEMENT SYSTEM  " << endl;
        cout << "        Ticket Sales & Entry System          " << endl;
        cout << "=============================================" << endl;
        
        displayMainMenu();
        
        // Get user choice
        cout << "Enter your choice: ";
        cin >> choice;
        
        // Clear input buffer
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        // Process choice
        switch (choice) {
            case 1:
                handleTicketSales();
                break;
            case 2:
                handleTicketRefunds();
                break;
            case 3:
                handleEntryManagement();
                break;
            case 4:
                handleAdminFunctions();
                break;
            case 5:
                handleReporting();
                break;
            case 6:
                cout << "Returning to main menu..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                pauseScreen();
        }
    }
  }
  
  // Display main menu
  void displayMainMenu() {
    cout << "\nMAIN MENU:" << endl;
    cout << "1. Ticket Sales" << endl;
    cout << "2. Ticket Refunds" << endl;
    cout << "3. Entry Management" << endl;
    cout << "4. Admin Functions" << endl;
    cout << "5. Reporting" << endl;
    cout << "6. Return to Main Menu" << endl;
  }
  
  // Handle ticket sales
  void handleTicketSales() {
    int choice = 0;
    
    while (choice != 5) {
        clearScreen();
        cout << "============= TICKET SALES =============" << endl;
        cout << "Current Sales Phase: ";
        switch (ticketSystem.getSalesPhase()) {
            case 3:
                cout << "VIP Only";
                break;
            case 2:
                cout << "Early-Bird";
                break;
            case 1:
                cout << "General Public";
                break;
        }
        cout << endl;
        
        cout << "\n1. View Available Matches" << endl;
        cout << "2. View Customers" << endl;
        cout << "3. Request Ticket" << endl;
        cout << "4. Process Ticket Requests" << endl;
        cout << "5. Return to Main Menu" << endl;
        
        cout << "\nEnter your choice: ";
        cin >> choice;
        
        // Clear input buffer
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        string customerID, matchID;
        
        switch (choice) {
            case 1:
                // View available matches
                ticketSystem.displayMatches();
                pauseScreen();
                break;
            
            case 2:
                // View customers
                ticketSystem.displayCustomers();
                pauseScreen();
                break;
            
            case 3:
                // Request ticket
                ticketSystem.displayMatches();
                cout << "\nEnter match ID: ";
                getline(cin, matchID);
                
                ticketSystem.displayCustomers();
                cout << "\nEnter customer ID: ";
                getline(cin, customerID);
                
                if (ticketSystem.requestTicket(customerID, matchID)) {
                    cout << "Ticket request added successfully." << endl;
                }
                pauseScreen();
                break;
            
            case 4:
                // Process ticket requests
                cout << "\nCurrent request queue:" << endl;
                ticketSystem.displayRequestQueue();
                
                cout << "\n1. Process Next Request" << endl;
                cout << "2. Process All Requests" << endl;
                cout << "Enter choice: ";
                
                int processChoice;
                cin >> processChoice;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                
                if (processChoice == 1) {
                    ticketSystem.processNextRequest();
                } else if (processChoice == 2) {
                    ticketSystem.processAllRequests();
                }
                pauseScreen();
                break;
            
            case 5:
                // Return to main menu
                break;
            
            default:
                cout << "Invalid choice. Please try again." << endl;
                pauseScreen();
        }
    }
  }
  
  // Handle ticket refunds
  void handleTicketRefunds() {
    clearScreen();
    cout << "============= TICKET REFUNDS =============" << endl;
    
    // Display all tickets
    ticketSystem.displayTickets();
    
    string ticketID;
    cout << "\nEnter ticket ID to refund (or 'x' to cancel): ";
    getline(cin, ticketID);
    
    if (ticketID != "x") {
        if (ticketSystem.refundTicket(ticketID)) {
            cout << "Ticket refunded successfully." << endl;
        }
    }
    
    pauseScreen();
  }
  
  // Updated Entry Management handling
  void handleEntryManagement() {
    int choice = 0;
    
    while (choice != 4) {
        clearScreen();
        cout << "============= ENTRY MANAGEMENT =============" << endl;
        
        cout << "\n1. View Entry Queues" << endl;
        cout << "2. Add Spectator to Entry Queue" << endl;
        cout << "3. Process Next Entry" << endl;
        cout << "4. Return to Main Menu" << endl;
        
        cout << "\nEnter your choice: ";
        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            pauseScreen();
            continue;
        }
        
        // Clear input buffer
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        string customerID, ticketID;
        int gateNumber;
        
        switch (choice) {
            case 1:
                // View entry queues
                ticketSystem.displayEntryQueues();
                pauseScreen();
                break;
            
            case 2:{
                // Add spectator to entry queue
                ticketSystem.displayTickets();
                
                bool validInput = false;
                while (!validInput) {
                    // Validate ticket ID
                    cout << "\nEnter ticket ID (or 'x' to cancel): ";
                    getline(cin, ticketID);
                    
                    if (ticketID == "x") {
                        cout << "Operation cancelled." << endl;
                        pauseScreen();
                        break; // Exit the while loop and the case
                    }
                    
                    if (ticketID.empty()) {
                        cout << "Error: Ticket ID cannot be empty. Please try again." << endl;
                        continue; // Skip to next iteration, asking for input again
                    }
                    
                    // Check if Ticket ID follows the correct format
                    if (ticketID[0] != 'T') {
                        cout << "Error: Invalid Ticket ID format. Ticket ID should start with 'T'. Please try again." << endl;
                        continue; // Skip to next iteration, asking for input again
                    }
                    
                    // Ticket ID looks valid, now get customer ID
                    cout << "Enter customer ID: ";
                    getline(cin, customerID);
                    
                    if (customerID.empty()) {
                        cout << "Error: Customer ID cannot be empty. Please try again." << endl;
                        continue; // Start over from ticket ID
                    }
                    
                    // Check if Customer ID follows the correct format
                    if (customerID[0] != 'C') {
                        cout << "Error: Invalid Customer ID format. Customer ID should start with 'C'. Please try again." << endl;
                        continue; // Start over from ticket ID
                    }
                    
                    // Both inputs are valid, proceed to gate selection
                    cout << "Enter desired gate number (1-4):" << endl;
                    cout << "Note: The system will automatically redirect to the appropriate gate" << endl;
                    cout << "      Gate 1: VIP tickets only" << endl;
                    cout << "      Gates 2-3: Early-Bird tickets" << endl;
                    cout << "      Gate 4: General tickets" << endl;
                    
                    if (!(cin >> gateNumber) || gateNumber < 1 || gateNumber > 4) {
                        cout << "Invalid gate number. Please enter a number between 1 and 4." << endl;
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        continue; // Start over from ticket ID
                    }
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    
                    // All inputs validated successfully
                    validInput = true;
                }
                
                // If we got here without breaking, inputs are valid
                if (validInput) {
                    // Process the request
                    if (ticketSystem.addToEntryQueue(customerID, ticketID, gateNumber)) {
                        cout << "Spectator added to queue successfully." << endl;
                    }
                }
                
                pauseScreen();
                break;
                }
            
            case 3:
                // Process next entry
                ticketSystem.displayEntryQueues();
                
                // Validate gate number
                cout << "\nEnter gate number (1-4): ";
                if (!(cin >> gateNumber) || gateNumber < 1 || gateNumber > 4) {
                    cout << "Invalid gate number. Please try again." << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    pauseScreen();
                    break;
                }
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                
                // Process entry
                if (ticketSystem.processNextEntry(gateNumber)) {
                    cout << "Entry processed successfully." << endl;
                }
                pauseScreen();
                break;
            
            case 4:
                // Return to main menu
                break;
            
            default:
                cout << "Invalid choice. Please try again." << endl;
                pauseScreen();
        }
    }
  }
  
  // Handle admin functions
  void handleAdminFunctions() {
    int choice = 0;
    
    while (choice != 3) {  // Changed from 4 to 3 since we removed addMatch
        clearScreen();
        cout << "============= ADMIN FUNCTIONS =============" << endl;
        cout << "Current Sales Phase: ";
        switch (ticketSystem.getSalesPhase()) {
            case 3:
                cout << "VIP Only";
                break;
            case 2:
                cout << "Early-Bird";
                break;
            case 1:
                cout << "General Public";
                break;
        }
        cout << endl;
        
        cout << "\n1. Change Sales Phase" << endl;
        cout << "2. Add New Customer" << endl;
        cout << "3. Return to Main Menu" << endl;  // Changed from 4 to 3
        
        cout << "\nEnter your choice: ";
        cin >> choice;
        
        // Clear input buffer
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (choice) {
            case 1:
                // Change sales phase
                cout << "\n1. VIP Only Phase" << endl;
                cout << "2. Early-Bird Phase" << endl;
                cout << "3. General Public Phase" << endl;
                cout << "Enter new phase: ";
                
                int phase;
                cin >> phase;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                
                if (phase >= 1 && phase <= 3) {
                    ticketSystem.setSalesPhase(4 - phase); // Convert UI values to internal values
                    cout << "Sales phase updated." << endl;
                } else {
                    cout << "Invalid phase." << endl;
                }
                pauseScreen();
                break;
            
            case 2:
                // Add new customer
                {
                    string name, email, phone;
                    int priority;
                    
                    cout << "\nEnter customer name: ";
                    getline(cin, name);
                    
                    cout << "Enter customer email: ";
                    getline(cin, email);
                    
                    cout << "Enter customer phone: ";
                    getline(cin, phone);
                    
                    cout << "Enter customer priority (3=VIP, 2=Early-Bird, 1=General): ";
                    cin >> priority;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    
                    string customerID = ticketSystem.addCustomer(name, email, phone, priority);
                    cout << "New customer added with ID: " << customerID << endl;
                }
                pauseScreen();
                break;
            
            case 3:  // Changed from 4 to 3
                // Return to main menu
                break;
            
            default:
                cout << "Invalid choice. Please try again." << endl;
                pauseScreen();
        }
    }
  }
  
  // Handle reporting
  void handleReporting() {
    int choice = 0;
    
    while (choice != 5) {
        clearScreen();
        cout << "============= REPORTING =============" << endl;
        
        cout << "\n1. View All Tickets" << endl;
        cout << "2. View Venue Seating" << endl;
        cout << "3. View Recent Transactions" << endl;
        cout << "4. Search Transactions" << endl;
        cout << "5. Return to Main Menu" << endl;
        
        cout << "\nEnter your choice: ";
        cin >> choice;
        
        // Clear input buffer
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        string matchID, customerID, ticketID;
        
        switch (choice) {
            case 1:
                // View all tickets
                ticketSystem.displayTickets();
                pauseScreen();
                break;
            
            case 2:
                // View venue seating
                ticketSystem.displayMatches();
                cout << "\nEnter match ID: ";
                getline(cin, matchID);
                
                ticketSystem.displayVenueSeating(matchID);
                pauseScreen();
                break;
            
            case 3:
                // View recent transactions
                cout << "\nEnter number of transactions to display: ";
                int count;
                cin >> count;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                
                ticketSystem.displayTransactionHistory(count);
                pauseScreen();
                break;
            
            case 4:
                // Search transactions
                cout << "\n1. Search by Customer ID" << endl;
                cout << "2. Search by Ticket ID" << endl;
                cout << "Enter choice: ";
                
                int searchChoice;
                cin >> searchChoice;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                
                if (searchChoice == 1) {
                    cout << "Enter Customer ID: ";
                    getline(cin, customerID);
                    ticketSystem.searchTransactionsByCustomer(customerID);
                } else if (searchChoice == 2) {
                    cout << "Enter Ticket ID: ";
                    getline(cin, ticketID);
                    ticketSystem.searchTransactionsByTicket(ticketID);
                }
                pauseScreen();
                break;
            
            case 5:
                // Return to main menu
                break;
            
            default:
                cout << "Invalid choice. Please try again." << endl;
                pauseScreen();
        }
    }
  }
  
  // Clear the screen
  void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
  }
  
  // Pause the screen
  void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.get();
  }
};

#endif 