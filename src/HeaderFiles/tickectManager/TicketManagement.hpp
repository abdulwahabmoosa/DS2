#ifndef TICKET_MANAGEMENT_HPP
#define TICKET_MANAGEMENT_HPP

#include "../HeaderFiles/tickectManager/TicketSystem.hpp"


  //This class provides the interface to the ticket system that can be called
  //from the main application menu.
 
class TicketManagement {
private:
    TicketSystem ticketSystem;
    
    // Private helper methods
    void displayMainMenu();
    void handleTicketSales();
    void handleTicketRefunds();
    void handleEntryManagement();
    void handleAdminFunctions();
    void handleReporting();
    void clearScreen();
    void pauseScreen();

public:
   // Constructor for the TicketManagement class
    TicketManagement();
    
    
     //Main entry point for the ticket management system
      //Displays menu and handles user interactions with the ticket system
    void Menu();
};

#endif 