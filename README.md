
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

# Asia Pacific University Tennis Championship Management System

## Overview
This C++ console-based system manages the Asia Pacific University Tennis Championship, simulating the tournament's scheduling, player progression, ticketing, and match tracking. The system is designed using custom data structures without reliance on STL containers like `vector` or `list`.

The solution follows the constraints and structure provided by APU for Lab Evaluation Work #2 and demonstrates individual and group-based system functionalities.

## Features
### 1. Tournament Scheduling & Player Progression (Task 1)
- **Qualifiers Draw:**
  - Players are assigned into a queue and paired for matches.
  - Matches are stored in a CSV file with status tracking.

- **Round Robin Draw:**
  - Winning players from qualifiers are enqueued into a circular queue.
  - Round-robin scheduling creates balanced matches and updates the CSV.

- **Knockout Draw:**
  - Top 4 players from round-robin (based on most wins) are selected using a stack and sorted manually.
  - Semi-final and final matches are generated accordingly.

- **Player Progress Tracking:**
  - Users can input a player's name to view performance across all tournament stages.
  - Displays opponent, score, result, and round-robin win count.

### 2. Ticket Sales & Spectator Management (Task 2)
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

### 3. CSV-Based Match Management
- All matches are logged in a CSV file:
  - Columns: Match ID, Player 1, Player 2, Match Status, Score 1, Score 2, Winner
  - The system checks for match readiness by verifying the presence of the `Winner` field.

### 4. Input Validation & User Experience
- Input validation for menu navigation.
- Prevents multiple executions of the same stage (Qualifiers, Round Robin, Knockouts).
- Terminal screen clearing for better user interface.

## Data Structures Used
- **Queue:** For scheduling qualifier matches.
- **Circular Queue:** For managing round-robin matches and player rotation.
- **Stack:** For pushing winners and selecting top performers for knockout rounds.
- **Priority Queue:** For processing ticket requests based on customer priority.
- **Dynamic Array:** For storing customer, match, and ticket data.

## File Structure
```
src/
├── Main.cpp                 # Entry point, handles main menu and control flow
├── MatchHistory.hpp         # For Task 4 (to be implemented separately)
├── PlayerProgression.hpp    # Main logic for Task 1
├── RoundRobin.hpp           # Circular queue implementation
├── Qualifiers.hpp           # Basic queue for qualifiers
├── Knockout.hpp             # Stack implementation for knockout stages
├── TicketManagement.hpp     # Ticketing interface class (Task 2)
├── TicketSystem.hpp         # Core ticket logic implementation
├── PriorityQueue.hpp        # Priority queue for VIP and early ticket processing
├── Models.hpp               # Data models for tickets, customers, etc.
├── csv/
│   └── TennisTournament.csv # Match records and progression
```

## How to Compile
Ensure you have a C++17-compatible compiler installed (e.g., g++, clang++).

```bash
g++ src/Main.cpp -o TennisChampionship
./TennisChampionship
```

## Notes
- All containers are custom-built as per lab requirements.
- The system assumes scores are updated manually into the CSV between each round.
- Ensure `csv/TennisTournament.csv` exists before running the system.

## Team Members & Roles
- **Ahmed** - Task 1: Tournament Scheduling and Player Progression
- **Abdirashid Mohamed Akira** - Task 2: Ticket Management System

## Pending/Optional Features
- Handling player withdrawals (Task 3)
- Full implementation of match history (Task 4)

## License
This project is for educational use as part of the CT077-3-2-DSTR Lab Evaluation Work #2 at Asia Pacific University.

