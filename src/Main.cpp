#include <iostream>
// #include "TournamentScheduler.h"  // Header file for tournament scheduling
// #include "TicketManager.h"        // Header file for ticket sales and spectator management
// #include "PlayerWithdrawals.h"    // Header file for handling player withdrawals
// #include "MatchHistory.h"         // Header file for match history tracking

using namespace std;

void displayMenu() {
    cout << "\n=====================================" << endl;
    cout << " Asia Pacific University Tennis Championship " << endl;
    cout << "=====================================" << endl;
    cout << "1. Tournament Scheduling & Player Progression" << endl;
    cout << "2. Ticket Sales & Spectator Management" << endl;
    cout << "3. Handling Player Withdrawals" << endl;
    cout << "4. Match History Tracking" << endl;
    cout << "5. Exit" << endl;
    cout << "Select an option: ";
}

int main() {
    int choice;
    // TournamentScheduler scheduler;
    // TicketManager ticketManager;
    // PlayerWithdrawals withdrawalManager;
    // MatchHistory matchHistory;

    do {
        displayMenu();
        cin >> choice;
        
        switch (choice) {
            case 1:
                // scheduler.manageSchedule();
                break;
            case 2:
                // ticketManager.processTickets();
                break;
            case 3:
                // withdrawalManager.handleWithdrawals();
                break;
            case 4:
                // matchHistory.recordMatch();
                break;
            case 5:
                cout << "Exiting the system. Goodbye!" << endl;
                break;
            default:
                cout << "Invalid option. Please try again." << endl;
        }
    } while (choice != 5);
    
    return 0;
}
