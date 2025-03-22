#include <iostream>
#include <limits>
#include "MatchHistory.hpp"              // Header file for match history tracking
#include "PlayerProgression.hpp"         // Header file for player progression
#include "HandleWithdraw.hpp"

using namespace std;

bool Checking(int value,int lower, int higher){
    return value<=higher && value >=lower;
}

void clearScreen() {
    cout << "\033[2J\033[H";
}

int main() {
    MatchHistory match;
    PlayersProgression player;
    HandleWithdraw withdraw;
    int choice;
    
    // Do your code in separate files including your menu and only call your main in the below code
   do {
        cout << string(80, '=')<<left << endl;
        cout << " ASIA PACIFIC UNIVERSITY TENNIS CHAMPIONSHIP " << endl;
        cout << string(80, '=')<<left << endl;
        cout << "1. Tournament Scheduling & Player Progression" << endl;
        cout << "2. Ticket Sales & Spectator Management" << endl;
        cout << "3. Handling Player Withdrawals" << endl;
        cout << "4. Match History Tracking" << endl;
        cout << "5. Clear Screen" << endl;
        cout << "6. Exit" << endl;
        cout << string(80, '=')<<left << endl;
        cout << "Select an option: ";
        while(!(cin>> choice) || !Checking(choice, 1, 6)){
            cout << "Invalid Input please your choice again correctly: ";
            cin.clear();
            cin.ignore(numeric_limits <streamsize> ::max(),'\n');
        }
        switch (choice) {
            case 1:
                //Ahmed
                player.Menu();
                break;
            case 2:
                // Akira
                break;
            case 3:
                // Kashave
                withdraw.Menu();
                break;
            case 4:
                //Abdulwahab
                match.Menu();
                break;
            case 5:
                clearScreen();
                break;
            case 6:
                cout << "Exiting the system. Goodbye!" << endl;
                break;
            default:
                cout << "Invalid option. Please try again." << endl;
        }
    } while (choice != 6);
    
    return 0;
}
