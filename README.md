# Tennis Tournament Ticket Management System
You can add these instructions to your README.md file like this:

```markdown
## Building the Project

To compile the Ticket Management System:

1. Navigate to the `src` directory:
   ```bash
   cd /path/to/your/src/directory
   ```

2. Run the compilation command:
   ```bash
   g++ Main.cpp srcTicketManager/TicketManagement.cpp srcTicketManager/CircularQueue.cpp srcTicketManager/PriorityQueue.cpp srcTicketManager/QUEUE.cpp srcTicketManager/Stack.cpp srcTicketManager/TicketSystem.cpp -o Main
   ```

3. Run the executable:
   ```bash
   ./Main
   ```
```

This format provides clear instructions with proper markdown formatting, including code blocks for the commands (which are formatted with ```bash to indicate they're bash shell commands). This makes the instructions easy to read and follow for anyone using your repository.
## Overview
The Ticket Management System is a module of the APU Tennis Championship Management System that handles ticket sales and spectator management. This subsystem provides functionality for managing ticket sales across different customer priority levels, handling entry of spectators through gates, and processing refunds.

## Features
- **Ticket Sales Management**
  - Multiple sales phases (VIP, Early-Bird, General Public)
  - Priority-based ticket allocation
  - Ticket request queuing

- **Spectator Entry Management**
  - Multiple entry gates with different access levels
  - Digital ticket verification
  - Real-time entry processing

- **Administration Functions**
  - Customer management
  - Sales phase control
  - Transaction tracking and reporting

- **Reporting Capabilities**
  - View all tickets
  - View venue seating arrangements
  - Transaction history
  - Customer and ticket search

## Data Structures Implemented
- **Priority Queue**: For processing ticket requests based on customer priority
- **Queue**: For managing entry gates
- **Circular Queue**: For venue capacity and seat allocation
- **Stack**: For tracking transaction history 
- **Dynamic Array**: For storing customer, match, and ticket data

## Usage
The system is integrated into the main APU Tennis Championship Management System. Access it through the main menu by selecting option 2 "Ticket Sales & Spectator Management".

## File Structure
- `TicketManagement.hpp/.cpp`: Main interface class
- `TicketSystem.hpp/.cpp`: Core ticket system implementation
- `PriorityQueue.hpp/.cpp`: Priority queue implementation
- `Queue.hpp/.cpp`: Queue implementation
- `CircularQueue.hpp/.cpp`: Circular queue implementation
- `Stack.hpp/.cpp`: Stack implementation
- `Models.hpp`: Data models for tickets, customers, matches, etc.

## Dependencies
- C++ Standard Library
- Integration with other modules of the tournament management system

## Contributors
Abdirashid Mohamed Akira Ticket Management System
