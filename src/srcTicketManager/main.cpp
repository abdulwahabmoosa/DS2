/*
    This file implements the main program interface for the ticket sales
    and spectator management system. It provides a menu driven interface
    that demonstrates all implemented data structures.
 */

 #include "../HeaderFiles/tickectManager/TicketSystem.hpp"
 #include <iostream>
 #include <string>
 #include <limits>
 
 // Function prototypes
 void displayMainMenu();
 void handleTicketSales(TicketSystem& system);
 void handleTicketRefunds(TicketSystem& system);
 void handleEntryManagement(TicketSystem& system);
 void handleAdminFunctions(TicketSystem& system);
 void handleReporting(TicketSystem& system);
 void clearScreen();
 void pauseScreen();
 using namespace std;
 
 
 int main() {
     // Initialize ticket system
     TicketSystem ticketSystem;
     
     // Initialize the system
     if (!ticketSystem.initialize()) {
         cerr << "Error initializing ticket system. Exiting." <<endl;
         return 1;
     }
     
     // Main menu loop
     int choice = 0;
     
     while (choice != 6) {
         clearScreen();
         cout << "=============================================" <<endl;
         cout << "  APU TENNIS CHAMPIONSHIP MANAGEMENT SYSTEM  " <<endl;
         cout << "        Ticket Sales & Entry System          " <<endl;
         cout << "=============================================" <<endl;
         
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
                 handleTicketSales(ticketSystem);
                 break;
             case 2:
                 handleTicketRefunds(ticketSystem);
                 break;
             case 3:
                 handleEntryManagement(ticketSystem);
                 break;
             case 4:
                 handleAdminFunctions(ticketSystem);
                 break;
             case 5:
                 handleReporting(ticketSystem);
                 break;
             case 6:
                 cout << "Exiting system. Thank you!" << endl;
                 break;
             default:
                 cout << "Invalid choice. Please try again." << endl;
                 pauseScreen();
         }
     }
     
     return 0;
 }
 
 //main menu
 void displayMainMenu() {
     cout << "\nMAIN MENU:" << endl;
     cout << "1. Ticket Sales" << endl;
     cout << "2. Ticket Refunds" << endl;
     cout << "3. Entry Management" << endl;
     cout << "4. Admin Functions" << endl;
     cout << "5. Reporting" << endl;
     cout << "6. Exit" << endl;
 }
 
 //ticket sales
 void handleTicketSales(TicketSystem& system) {
     int choice = 0;
     
     while (choice != 5) {
         clearScreen();
         cout << "============= TICKET SALES =============" << endl;
         cout << "Current Sales Phase: ";
         switch (system.getSalesPhase()) {
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
                 system.displayMatches();
                 pauseScreen();
                 break;
             
             case 2:
                 // View customers
                 system.displayCustomers();
                 pauseScreen();
                 break;
             
             case 3:
                 // Request ticket
                 system.displayMatches();
                 cout << "\nEnter match ID: ";
                 getline(cin, matchID);
                 
                 system.displayCustomers();
                 cout << "\nEnter customer ID: ";
                 getline(cin, customerID);
                 
                 if (system.requestTicket(customerID, matchID)) {
                     cout << "Ticket request added successfully." << endl;
                 }
                 pauseScreen();
                 break;
             
             case 4:
                 // Process ticket requests
                 cout << "\nCurrent request queue:" << endl;
                 system.displayRequestQueue();
                 
                 cout << "\n1. Process Next Request" << endl;
                 cout << "2. Process All Requests" << endl;
                 cout << "Enter choice: ";
                 
                 int processChoice;
                 cin >> processChoice;
                 cin.clear();
                 cin.ignore(numeric_limits<streamsize>::max(), '\n');
                 
                 if (processChoice == 1) {
                     system.processNextRequest();
                 } else if (processChoice == 2) {
                     system.processAllRequests();
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
 
//ticket refunds
 void handleTicketRefunds(TicketSystem& system) {
     clearScreen();
     cout << "============= TICKET REFUNDS =============" << endl;
     
     // Display all tickets
     system.displayTickets();
     
     string ticketID;
     cout << "\nEnter ticket ID to refund (or 'x' to cancel): ";
     getline(cin, ticketID);
     
     if (ticketID != "x") {
         if (system.refundTicket(ticketID)) {
             cout << "Ticket refunded successfully." << endl;
         }
     }
     
     pauseScreen();
 }
 
 //entry management
 void handleEntryManagement(TicketSystem& system) {
     int choice = 0;
     
     while (choice != 4) {
         clearScreen();
         cout << "============= ENTRY MANAGEMENT =============" << endl;
         
         cout << "\n1. View Entry Queues" << endl;
         cout << "2. Add Spectator to Entry Queue" << endl;
         cout << "3. Process Next Entry" << endl;
         cout << "4. Return to Main Menu" << endl;
         
         cout << "\nEnter your choice: ";
         cin >> choice;
         
         // Clear input buffer
         cin.clear();
         cin.ignore(numeric_limits<streamsize>::max(), '\n');
         
         string customerID, ticketID;
         int gateNumber;
         
         switch (choice) {
             case 1:
                 // View entry queues
                 system.displayEntryQueues();
                 pauseScreen();
                 break;
             
             case 2:
                 // Add spectator to entry queue
                 system.displayTickets();
                 cout << "\nEnter ticket ID: ";
                 getline(cin, ticketID);
                 
                 cout << "Enter customer ID: ";
                 getline(cin, customerID);
                 
                 cout << "Enter gate number (1-4): ";
                 cin >> gateNumber;
                 cin.clear();
                 cin.ignore(numeric_limits<streamsize>::max(), '\n');
                 
                 if (system.addToEntryQueue(customerID, ticketID, gateNumber)) {
                     cout << "Spectator added to entry queue successfully." << endl;
                 }
                 pauseScreen();
                 break;
             
             case 3:
                 // Process next entry
                 system.displayEntryQueues();
                 cout << "\nEnter gate number (1-4): ";
                 cin >> gateNumber;
                 cin.clear();
                 cin.ignore(numeric_limits<streamsize>::max(), '\n');
                 
                 if (system.processNextEntry(gateNumber)) {
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
 

 void handleAdminFunctions(TicketSystem& system) {
    int choice = 0;
    
    while (choice != 3) {  // Changed from 4 to 3 since we removed addMatch
        clearScreen();
        cout << "============= ADMIN FUNCTIONS =============" << endl;
        cout << "Current Sales Phase: ";
        switch (system.getSalesPhase()) {
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
                    system.setSalesPhase(4 - phase); // Convert UI values to internal values
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
                    
                    string customerID = system.addCustomer(name, email, phone, priority);
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

 //reporting
 void handleReporting(TicketSystem& system) {
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
                 system.displayTickets();
                 pauseScreen();
                 break;
             
             case 2:
                 // View venue seating
                 system.displayMatches();
                 cout << "\nEnter match ID: ";
                 getline(cin, matchID);
                 
                 system.displayVenueSeating(matchID);
                 pauseScreen();
                 break;
             
             case 3:
                 // View recent transactions
                 cout << "\nEnter number of transactions to display: ";
                 int count;
                 cin >> count;
                 cin.clear();
                 cin.ignore(numeric_limits<streamsize>::max(), '\n');
                 
                 system.displayTransactionHistory(count);
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
                     system.searchTransactionsByCustomer(customerID);
                 } else if (searchChoice == 2) {
                     cout << "Enter Ticket ID: ";
                     getline(cin, ticketID);
                     system.searchTransactionsByTicket(ticketID);
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