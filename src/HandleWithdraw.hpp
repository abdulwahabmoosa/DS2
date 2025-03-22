#ifndef HANDLEWITHDRAW_HPP
#define HANDLEWITHDRAW_HPP

#include <iostream>
#include <fstream>
#include <limits>
#include <string>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <iomanip>

#include "WithdrawalQueue.hpp"

using namespace std;

class HandleWithdraw
{
private:
    int injuredCount = 0; // Counter for injured players in qualifiers
    
    // Helper functions
    bool Checking(int value, int lower, int higher) {
        return value <= higher && value >= lower;
    }
    
    void clearScreen() {
        cout << "\033[2J\033[H";
    }
    
    // Check if a round has already been played (has winners)
    // Check if ALL matches in a round have already been played (have winners)
    bool hasWinners(string matchPrefix) {
        ifstream csvFile("csv/TennisTournament.csv");
        if (!csvFile) {
            cout << "Error: Could not open TennisTournament.csv file." << endl;
            return false;
        }
        
        string headerLine;
        getline(csvFile, headerLine);
        
        // Check if the file has winner column
        bool hasWinnerColumn = (headerLine.find("Winner") != string::npos);
        if (!hasWinnerColumn) {
            csvFile.close();
            return false;
        }
        
        string line;
        int matchCount = 0;
        int winnerCount = 0;
        
        while (getline(csvFile, line)) {
            stringstream ss(line);
            string matchID, player1, player2, status, score1, score2, winner;
            
            getline(ss, matchID, ',');
            
            // Check if this match belongs to the specified prefix
            if (matchID.find(matchPrefix) != string::npos) {
                matchCount++; // Found a match with this prefix
                
                getline(ss, player1, ',');
                getline(ss, player2, ',');
                getline(ss, status, ',');
                
                // Try to get winner column
                if (getline(ss, score1, ',') && getline(ss, score2, ',') && getline(ss, winner, ',')) {
                    if (!winner.empty()) {
                        winnerCount++; // This match has a winner
                    }
                }
            }
        }
        
        csvFile.close();
        
        // Only return true if ALL matches of this type have winners
        return matchCount > 0 && matchCount == winnerCount;
    }
    
    // Check if matches exist for a specified round
    bool matchesExist(string matchPrefix) {
        ifstream csvFile("csv/TennisTournament.csv");
        if (!csvFile) {
            cout << "Error: Could not open TennisTournament.csv file." << endl;
            return false;
        }
        
        string headerLine;
        getline(csvFile, headerLine);
        
        string line;
        bool found = false;
        
        while (getline(csvFile, line)) {
            stringstream ss(line);
            string matchID;
            getline(ss, matchID, ',');
            
            if (matchID.find(matchPrefix) != string::npos) {
                found = true;
                break;
            }
        }
        
        csvFile.close();
        return found;
    }
    
    // Mark a player as withdrawn and update match status
    void markPlayerWithdrawn(string matchID, string player) {
        ifstream inFile("csv/TennisTournament.csv");
        ofstream outFile("csv/TennisTournament_temp.csv");
        
        if (!inFile || !outFile) {
            cout << "Error opening files for update." << endl;
            return;
        }
        
        string line, headerLine;
        bool matchFound = false;
        
        // Copy header
        getline(inFile, headerLine);
        outFile << headerLine << endl;
        
        // Process each line
        while (getline(inFile, line)) {
            stringstream ss(line);
            string currentMatchID, player1, player2, status;
            
            getline(ss, currentMatchID, ',');
            getline(ss, player1, ',');
            getline(ss, player2, ',');
            getline(ss, status, ',');
            
            // Trim whitespace
            player1 = player1.substr(player1.find_first_not_of(" "));
            player2 = player2.substr(player2.find_first_not_of(" "));
            
            if (currentMatchID == matchID) {
                // This is the match to update
                matchFound = true;
                
                // Update the player name to "Withdrew"
                if (player1 == player) {
                    player1 = "Withdrew";
                } else if (player2 == player) {
                    player2 = "Withdrew";
                }
                
                // Write updated line with player withdrawal and confirmed status
                outFile << currentMatchID << ", " << player1 << ", " << player2 << ", Confirmed";
                
                // Copy any remaining fields (like scores if they exist)
                string restOfLine;
                if (getline(ss, restOfLine)) {
                    outFile << restOfLine;
                }
                
                outFile << endl;
            } else {
                // Copy line as is
                outFile << line << endl;
            }
        }
        
        inFile.close();
        outFile.close();
        
        if (!matchFound) {
            cout << "Warning: Match ID " << matchID << " not found in CSV file." << endl;
            return;
        }
        
        // Replace original file with updated file
        remove("csv/TennisTournament.csv");
        rename("csv/TennisTournament_temp.csv", "csv/TennisTournament.csv");
        
        cout << "\nMatch updated successfully with player withdrawal!" << endl;
    }

public:
    HandleWithdraw() {
        // Seed the random number generator for injury probability
        srand(static_cast<unsigned int>(time(nullptr)));
    }
    
    ~HandleWithdraw() {}
    
    // Handle qualifier withdrawals
    void qualifyingConfirm() {
        cout << "REVIEWING QUALIFIER MATCHES FOR PLAYER WITHDRAWALS" << endl;
        cout << string(80, '-') << endl;
        
        // Check if qualifiers exist
        if (!matchesExist("QUL_")) {
            cout << "No qualifier matches found in the tournament schedule." << endl;
            return;
        }
        
        // Check if qualifiers have already been played
        if (hasWinners("QUL_")) {
            cout << "Qualifier matches have already been played. Cannot process withdrawals." << endl;
            return;
        }
        
        // Step 1: First store all qualifier matches in static arrays (no STL containers)
        const int MAX_MATCHES = 8; // Max expected qualifier matches
        string matchIDs[MAX_MATCHES];
        string player1s[MAX_MATCHES];
        string player2s[MAX_MATCHES];
        string statuses[MAX_MATCHES];
        int matchCount = 0;
        
        // Open the CSV file for reading
        ifstream csvFile("csv/TennisTournament.csv");
        if (!csvFile) {
            cout << "Error: Could not open TennisTournament.csv file." << endl;
            return;
        }
        
        // Skip the header line
        string headerLine;
        getline(csvFile, headerLine);
        
        // Read all qualifier matches into our arrays
        string line;
        while (getline(csvFile, line) && matchCount < MAX_MATCHES) {
            stringstream ss(line);
            string matchID, player1, player2, status;
            
            getline(ss, matchID, ',');
            
            // Process only qualifier matches
            if (matchID.find("QUL_") != string::npos) {
                getline(ss, player1, ',');
                getline(ss, player2, ',');
                getline(ss, status, ',');
                
                // Trim whitespace
                player1 = player1.substr(player1.find_first_not_of(" "));
                player2 = player2.substr(player2.find_first_not_of(" "));
                
                // Store match data
                matchIDs[matchCount] = matchID;
                player1s[matchCount] = player1;
                player2s[matchCount] = player2;
                statuses[matchCount] = status;
                matchCount++;
            }
        }
        
        csvFile.close();
        
        if (matchCount == 0) {
            cout << "No qualifier matches found in the tournament schedule." << endl;
            return;
        }
        
        // Display all matches first
        cout << "\nAll Qualifier Matches:" << endl;
        cout << string(80, '-') << endl;
        for (int i = 0; i < matchCount; i++) {
            cout << i+1 << ". Match " << matchIDs[i] << ": " << player1s[i] << " vs " << player2s[i] 
                 << " (Status: " << statuses[i] << ")" << endl;
        }
        cout << string(80, '-') << endl;
        
        // Step 2: Determine how many injuries (1 guaranteed, possibility of 2)
        int injuriesToGenerate = 1; // Always at least 1 injury
        
        // 30% chance of having a second injury
        if (rand() % 100 < 30) {
            injuriesToGenerate = 2;
        }
        
        cout << "\nAfter medical evaluations, we have determined there " 
             << (injuriesToGenerate == 1 ? "is 1 player who has" : "are 2 players who have")
             << " withdrawn due to injuries." << endl;
        
        // Step 3: Select random matches for injuries, ensuring they're different matches
        int injuredMatchIndices[2] = {-1, -1};
        int playerIndices[2] = {-1, -1}; // 0 for player1, 1 for player2
        
        for (int i = 0; i < injuriesToGenerate; i++) {
            int matchIndex;
            do {
                matchIndex = rand() % matchCount;
            } while (matchIndex == injuredMatchIndices[0]); // Ensure different matches
            
            injuredMatchIndices[i] = matchIndex;
            
            // Randomly choose player1 or player2 (0 or 1)
            playerIndices[i] = rand() % 2;
        }
        
        // Track which matches have been confirmed by the user
        bool matchConfirmed[MAX_MATCHES] = {false};
        string injuredPlayers[MAX_MATCHES] = {""};
        
        // Step 4: Process each match one by one for user confirmation
        for (int i = 0; i < matchCount; i++) {
            // Skip already confirmed matches
            if (statuses[i].find("Confirmed") != string::npos) {
                cout << "\nMatch " << matchIDs[i] << " is already confirmed. Skipping." << endl;
                continue;
            }
            
            // Check if this match has an injured player
            bool hasInjury = false;
            int injuryIndex = -1;
            
            for (int j = 0; j < injuriesToGenerate; j++) {
                if (injuredMatchIndices[j] == i) {
                    hasInjury = true;
                    injuryIndex = j;
                    break;
                }
            }
            
            cout << "\nReviewing Match: " << matchIDs[i] << endl;
            cout << "Players: " << player1s[i] << " vs " << player2s[i] << endl;
            
            if (hasInjury) {
                string injuredPlayer = (playerIndices[injuryIndex] == 0) ? player1s[i] : player2s[i];
                cout << "ALERT: " << injuredPlayer << " has been injured and needs to withdraw!" << endl;
                injuredPlayers[i] = injuredPlayer;
            } else {
                cout << "No injuries reported for this match." << endl;
            }
            
            cout << "Confirm this match? (Y/N): ";
            char choice;
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            if (toupper(choice) == 'Y') {
                matchConfirmed[i] = true;
                cout << "Match " << matchIDs[i] << " confirmed." << endl;
            } else {
                cout << "Match " << matchIDs[i] << " remains unconfirmed." << endl;
            }
        }
        
        // Step 5: Update the CSV file with all confirmed matches
        ifstream inFile("csv/TennisTournament.csv");
        ofstream outFile("csv/TennisTournament_temp.csv");
        
        if (!inFile || !outFile) {
            cout << "Error opening files for update." << endl;
            return;
        }
        
        // Copy header
        string headerLine2;
        getline(inFile, headerLine2);
        outFile << headerLine2 << endl;
        
        // Process each line in the CSV
        while (getline(inFile, line)) {
            stringstream ss(line);
            string currentMatchID;
            getline(ss, currentMatchID, ',');
            
            // Find if this is one of our qualifier matches
            int matchIndex = -1;
            for (int i = 0; i < matchCount; i++) {
                if (currentMatchID == matchIDs[i]) {
                    matchIndex = i;
                    break;
                }
            }
            
            // If this is a qualifier match and it was confirmed by the user
            if (matchIndex != -1 && matchConfirmed[matchIndex]) {
                // Parse the match details
                stringstream matchSS(line);
                string matchID, player1, player2, status, rest;
                
                getline(matchSS, matchID, ',');
                getline(matchSS, player1, ',');
                getline(matchSS, player2, ',');
                getline(matchSS, status);
                
                // Trim whitespace
                player1 = player1.substr(player1.find_first_not_of(" "));
                player2 = player2.substr(player2.find_first_not_of(" "));
                
                // Check if there was an injured player in this match
                if (injuredPlayers[matchIndex] != "") {
                    if (injuredPlayers[matchIndex] == player1) {
                        player1 = "Withdrew";
                    } else if (injuredPlayers[matchIndex] == player2) {
                        player2 = "Withdrew";
                    }
                }
                
                // Write the updated line with confirmed status
                outFile << matchID << ", " << player1 << ", " << player2 << ", Confirmed" << endl;
            } else {
                // Write the line as is
                outFile << line << endl;
            }
        }
        
        inFile.close();
        outFile.close();
        
        // Replace original file with updated file
        remove("csv/TennisTournament.csv");
        rename("csv/TennisTournament_temp.csv", "csv/TennisTournament.csv");
        
        cout << "\nQualifier matches review completed." << endl;
        
        // Count and report actual withdrawals processed
        int actualWithdrawals = 0;
        for (int i = 0; i < matchCount; i++) {
            if (matchConfirmed[i] && injuredPlayers[i] != "") {
                actualWithdrawals++;
            }
        }
        
        cout << "Total withdrawals processed in qualifier round: " << actualWithdrawals << endl;
    }
    
    // Handle round robin withdrawals
    // void roundRobinConfirm() {
    //     cout << "REVIEWING ROUND ROBIN MATCHES FOR PLAYER WITHDRAWALS" << endl;
    //     cout << string(80, '-') << endl;
        
    //     // Check if round robin matches exist
    //     if (!matchesExist("ROU")) {
    //         cout << "No round robin matches found in the tournament schedule." << endl;
    //         return;
    //     }
        
    //     // Check if round robin matches have already been played
    //     if (hasWinners("ROU")) {
    //         cout << "Round robin matches have already been played. Cannot process withdrawals." << endl;
    //         return;
    //     }
        
    //     cout << "\nRound Robin withdrawal processing will be implemented in a future update." << endl;
    //     cout << "This functionality is currently under development." << endl;
        
    //     // Space for future implementation of round robin withdrawals
    // }
    
    void roundRobinConfirm() {
        cout << "REVIEWING ROUND ROBIN MATCHES FOR PLAYER WITHDRAWALS" << endl;
        cout << string(80, '-') << endl;
        
        // Check if round robin matches exist
        if (!matchesExist("ROU")) {
            cout << "No round robin matches found in the tournament schedule." << endl;
            return;
        }
        
        // Check if round robin matches have already been played
        if (hasWinners("ROU")) {
            cout << "Round robin matches have already been played. Cannot process withdrawals." << endl;
            return;
        }
    
        // Step 1: Read all Round Robin matches
        const int MAX_MATCHES = 32; // Adjust based on tournament structure
        string matchIDs[MAX_MATCHES];
        string player1s[MAX_MATCHES];
        string player2s[MAX_MATCHES];
        string statuses[MAX_MATCHES];
        int matchCount = 0;
    
        ifstream csvFile("csv/TennisTournament.csv");
        if (!csvFile) {
            cout << "Error: Could not open TennisTournament.csv file." << endl;
            return;
        }
        
        string headerLine;
        getline(csvFile, headerLine); // Skip header
        
        string line;
        while (getline(csvFile, line) && matchCount < MAX_MATCHES) {
            stringstream ss(line);
            string matchID, player1, player2, status;
            getline(ss, matchID, ',');
            if (matchID.find("ROU") != string::npos) {
                getline(ss, player1, ',');
                getline(ss, player2, ',');
                getline(ss, status, ',');
                
                // Trim whitespace
                player1 = player1.substr(player1.find_first_not_of(" "));
                player2 = player2.substr(player2.find_first_not_of(" "));
                
                matchIDs[matchCount] = matchID;
                player1s[matchCount] = player1;
                player2s[matchCount] = player2;
                statuses[matchCount] = status;
                matchCount++;
            }
        }
        csvFile.close();
    
        if (matchCount == 0) {
            cout << "No round robin matches found in the tournament schedule." << endl;
            return;
        }
    
        // Step 2: Initialize withdrawal queue with ROU qualifiers from CSV
        WithdrawalQueue queue(50); // Capacity based on expected losers
        ifstream losersFile("csv/losers.csv");
        if (losersFile) {
            string loserHeader;
            getline(losersFile, loserHeader); // Skip header
            
            string loserLine;
            while (getline(losersFile, loserLine)) {
                stringstream ss(loserLine);
                string name, qualifier;
                int score;
                
                getline(ss, name, ',');
                ss >> score;
                getline(ss, qualifier); // Read until end of line
                qualifier = qualifier.substr(qualifier.find_first_not_of(", "), 
                                            qualifier.find_last_not_of(" ") + 1);
                
                if (qualifier.find("QUL") == 0 && name != "Withdrew") {
                    queue.enqueue(name, score, qualifier);
                }
            }
            losersFile.close();
        }
    
        // Determine maximum possible injuries
        int maxInjuries = queue.size();
        int injuriesToGenerate = min(maxInjuries, matchCount);
        if (injuriesToGenerate == 0) {
            cout << "No eligible replacements available in the queue." << endl;
            return;
        }
    
        cout << "\nAfter medical evaluations, we have determined there are " 
             << injuriesToGenerate << " players who need to withdraw." << endl;
    
        // Track replacements and confirmations
        struct Replacement {
            string originalPlayer;
            string replacementPlayer;
            int matchIndex;
        } replacements[MAX_MATCHES] = {};
    
        // Process matches for injuries
        int injuredMatchIndices[MAX_MATCHES] = {0};
        int playerIndices[MAX_MATCHES] = {0};
        int injuryCount = 0;
        
        while (injuryCount < injuriesToGenerate) {
            int matchIndex = rand() % matchCount;
            bool isDuplicate = false;
            
            for (int i = 0; i < injuryCount; ++i) {
                if (injuredMatchIndices[i] == matchIndex) {
                    isDuplicate = true;
                    break;
                }
            }
            
            if (!isDuplicate) {
                injuredMatchIndices[injuryCount] = matchIndex;
                playerIndices[injuryCount] = rand() % 2;
                injuryCount++;
            }
        }
    
        // Step 3: Process each match with user confirmation
        for (int i = 0; i < matchCount; i++) {
            if (statuses[i].find("Confirmed") != string::npos) {
                cout << "\nMatch " << matchIDs[i] << " is already confirmed. Skipping." << endl;
                continue;
            }
    
            bool hasInjury = false;
            int injuryIndex = -1;
            for (int j = 0; j < injuriesToGenerate; j++) {
                if (injuredMatchIndices[j] == i) {
                    hasInjury = true;
                    injuryIndex = j;
                    break;
                }
            }
    
            cout << "\nReviewing Match: " << matchIDs[i] << endl;
            cout << "Players: " << player1s[i] << " vs " << player2s[i] << endl;
            
            if (hasInjury) {
                string injuredPlayer = (playerIndices[injuryIndex] == 0) ? player1s[i] : player2s[i];
                replacements[injuryIndex] = {injuredPlayer, "", i};
                
                cout << "ALERT: " << injuredPlayer << " has been injured and needs to withdraw!" << endl;
    
                if (queue.size() == 0) {
                    cout << "No eligible replacements remaining in the queue." << endl;
                    continue;
                }
    
                int bestIndex = 0;
                int maxScore = queue[0]->score;
                
                for (int j = 1; j < queue.size(); j++) {
                    if (queue[j]->score > maxScore) {
                        maxScore = queue[j]->score;
                        bestIndex = j;
                    }
                }
    
                replacements[injuryIndex].replacementPlayer = queue[bestIndex]->name;
                queue.removeAt(bestIndex);
                
                cout << "Replacement player: " << replacements[injuryIndex].replacementPlayer 
                     << " selected (Score: " << maxScore << ")" << endl;
            }
    
            cout << "Confirm this match? (Y/N): ";
            char choice;
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            
            if (toupper(choice) == 'Y') {
                statuses[i] = "Confirmed";
                cout << "Match " << matchIDs[i] << " confirmed." << endl;
            }
        }
    
        // Step 4: Update CSV file with replacements
        ifstream inFile("csv/TennisTournament.csv");
        ofstream outFile("csv/TennisTournament_temp.csv");
        
        if (!inFile || !outFile) {
            cout << "Error opening files for update." << endl;
            return;
        }
    
        getline(inFile, headerLine);
        outFile << headerLine << endl;
        
        while (getline(inFile, line)) {
            stringstream ss(line);
            string currentMatchID;
            getline(ss, currentMatchID, ',');
            
            int matchIndex = -1;
            for (int i = 0; i < matchCount; i++) {
                if (currentMatchID == matchIDs[i]) {
                    matchIndex = i;
                    break;
                }
            }
    
            if (matchIndex != -1 && statuses[matchIndex] == "Confirmed") {
                stringstream matchSS(line);
                string matchID, player1, player2, status, rest;
                getline(matchSS, matchID, ',');
                getline(matchSS, player1, ',');
                getline(matchSS, player2, ',');
                getline(matchSS, status, ',');
                
                // Apply replacements
                for (int j = 0; j < injuriesToGenerate; j++) {
                    if (replacements[j].matchIndex == matchIndex) {
                        if (player1 == replacements[j].originalPlayer) {
                            player1 = replacements[j].replacementPlayer;
                        }
                        if (player2 == replacements[j].originalPlayer) {
                            player2 = replacements[j].replacementPlayer;
                        }
                    }
                }
    
                outFile << matchID << ", " << player1 << ", " << player2 << ", Confirmed";
                if (getline(matchSS, rest)) outFile << rest;
                outFile << endl;
            } else {
                outFile << line << endl;
            }
        }
    
        inFile.close();
        outFile.close();
        remove("csv/TennisTournament.csv");
        rename("csv/TennisTournament_temp.csv", "csv/TennisTournament.csv");
        
        cout << "\nRound Robin matches review completed." << endl;
        int actualReplacements = 0;
        for (int i = 0; i < injuriesToGenerate; i++) {
            if (!replacements[i].replacementPlayer.empty()) actualReplacements++;
        }
        cout << "Total replacements processed: " << actualReplacements << "/" << injuriesToGenerate << endl;
    }
    
    void semisConfirm() {
        cout << "REVIEWING SEMI-FINAL MATCHES FOR PLAYER WITHDRAWALS" << endl;
        cout << string(80, '-') << endl;
        
        if (!matchesExist("Semi_")) {
            cout << "No semi-final matches found." << endl;
            return;
        }
        if (hasWinners("Semi_")) {
            cout << "Semi-finals already played. Cannot process withdrawals." << endl;
            return;
        }
    
        // Load semi-final matches
        const int MAX_MATCHES = 8;
        string matchIDs[MAX_MATCHES], player1s[MAX_MATCHES], player2s[MAX_MATCHES], statuses[MAX_MATCHES];
        int matchCount = 0;
        ifstream csvFile("csv/TennisTournament.csv");
        if (!csvFile) {
            cout << "Error opening CSV file." << endl;
            return;
        }
        string headerLine;
        getline(csvFile, headerLine);
        string line;
        while (getline(csvFile, line) && matchCount < MAX_MATCHES) {
            stringstream ss(line);
            string matchID, p1, p2, status;
            getline(ss, matchID, ',');
            if (matchID.find("Semi_") != string::npos) {
                getline(ss, p1, ',');
                getline(ss, p2, ',');
                getline(ss, status, ',');
                p1 = p1.substr(p1.find_first_not_of(" "));
                p2 = p2.substr(p2.find_first_not_of(" "));
                matchIDs[matchCount] = matchID;
                player1s[matchCount] = p1;
                player2s[matchCount] = p2;
                statuses[matchCount] = status;
                matchCount++;
            }
        }
        csvFile.close();
    
        if (matchCount == 0) {
            cout << "No semi-final matches found." << endl;
            return;
        }
    
        // Initialize queue with ROU losers NOT in semis
        WithdrawalQueue queue(50);
        ifstream losersFile("csv/losers.csv");
        if (losersFile) {
            string loserHeader;
            getline(losersFile, loserHeader);
            string loserLine;
            while (getline(losersFile, loserLine)) {
                stringstream ss(loserLine);
                string name, qualifier;
                int score;
                getline(ss, name, ',');
                ss >> score;
                getline(ss, qualifier);
                qualifier = qualifier.substr(qualifier.find_first_not_of(", "), 
                                            qualifier.find_last_not_of(" ") + 1);
                
                // Check if player is already in semis
                bool inSemis = false;
                for (int i = 0; i < matchCount; i++) {
                    if (name == player1s[i] || name == player2s[i]) {
                        inSemis = true;
                        break;
                    }
                }
                
                if (qualifier.find("ROU") == 0 && name != "Withdrew" && !inSemis) {
                    queue.enqueue(name, score, qualifier);
                }
            }
            losersFile.close();
        }
    
        // Determine injuries (1 guaranteed, 30% chance for 2)
        int baseInjuries = 1;
        if (rand() % 100 < 30 && queue.size() >= 2) {
            baseInjuries = 2;
        }
        int injuriesToGenerate = min(baseInjuries, queue.size());
        if (injuriesToGenerate == 0) {
            cout << "No eligible replacements." << endl;
            return;
        }
    
        cout << "\nAfter medical evaluations, " << injuriesToGenerate << " players need to withdraw." << endl;
    
        struct Replacement {
            string original;
            string replacement;
            int matchIndex;
            int playerIndex;
        } replacements[MAX_MATCHES] = {};
    
        // Track injured matches
        int injuredMatchIndices[MAX_MATCHES] = {0};
        int injuryCount = 0;
        while (injuryCount < injuriesToGenerate) {
            int matchIndex = rand() % matchCount;
            bool duplicate = false;
            for (int i = 0; i < injuryCount; ++i) {
                if (injuredMatchIndices[i] == matchIndex) {
                    duplicate = true;
                    break;
                }
            }
            if (!duplicate) {
                injuredMatchIndices[injuryCount] = matchIndex;
                injuryCount++;
            }
        }
    
        // Process each match
        for (int i = 0; i < matchCount; i++) {
            if (statuses[i].find("Confirmed") != string::npos) {
                cout << "\nMatch " << matchIDs[i] << " already confirmed. Skipping." << endl;
                continue;
            }
    
            bool hasInjury = false;
            int injuryIndex = -1;
            for (int j = 0; j < injuriesToGenerate; j++) {
                if (injuredMatchIndices[j] == i) {
                    hasInjury = true;
                    injuryIndex = j;
                    break;
                }
            }
    
            cout << "\nReviewing Match: " << matchIDs[i] << endl;
            cout << "Players: " << player1s[i] << " vs " << player2s[i] << endl;
    
            if (hasInjury) {
                int playerIndex = rand() % 2;
                string injuredPlayer = (playerIndex == 0) ? player1s[i] : player2s[i];
                replacements[injuryIndex] = {injuredPlayer, "", i, playerIndex};
                cout << "ALERT: " << injuredPlayer << " has withdrawn!" << endl;
    
                if (queue.size() == 0) {
                    cout << "No replacements available." << endl;
                    continue;
                }
    
                // Find replacement with least frequency and not in semis
                int bestIndex = -1;
                int minFreq = INT_MAX;
                int bestScore = -1;
    
                for (int j = 0; j < queue.size(); j++) {
                    Node* candidate = queue[j];
                    bool inSemis = false;
                    for (int k = 0; k < matchCount; k++) {
                        if (candidate->name == player1s[k] || candidate->name == player2s[k]) {
                            inSemis = true;
                            break;
                        }
                    }
                    if (inSemis) continue;
    
                    int freq = 0;
                    for (int k = 0; k < queue.size(); k++) {
                        if (queue[k]->name == candidate->name) {
                            freq++;
                        }
                    }
    
                    if (freq < minFreq || 
                        (freq == minFreq && candidate->score > bestScore) ||
                        (freq == minFreq && candidate->score == bestScore && j < bestIndex)) {
                        minFreq = freq;
                        bestScore = candidate->score;
                        bestIndex = j;
                    }
                }
    
                if (bestIndex == -1) {
                    cout << "No eligible replacements." << endl;
                    continue;
                }
    
                replacements[injuryIndex].replacement = queue[bestIndex]->name;
                queue.removeAt(bestIndex);
    
                // Update in-memory arrays immediately
                if (playerIndex == 0) {
                    player1s[i] = replacements[injuryIndex].replacement;
                } else {
                    player2s[i] = replacements[injuryIndex].replacement;
                }
                
                cout << "Replacement: " << replacements[injuryIndex].replacement 
                     << " (Frequency: " << minFreq << ", Score: " << bestScore << ")" << endl;
            }
    
            cout << "Confirm? (Y/N): ";
            char choice;
            cin >> choice;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            if (toupper(choice) == 'Y') {
                statuses[i] = "Confirmed";
                cout << "Match confirmed." << endl;
            }
        }
    
        // Update CSV
        ifstream inFile("csv/TennisTournament.csv");
        ofstream outFile("csv/TennisTournament_temp.csv");
        if (!inFile || !outFile) {
            cout << "Error updating CSV." << endl;
            return;
        }
    
        getline(inFile, headerLine);
        outFile << headerLine << endl;
    
        while (getline(inFile, line)) {
            stringstream ss(line);
            string currentID;
            getline(ss, currentID, ',');
            int matchIndex = -1;
            for (int i = 0; i < matchCount; i++) {
                if (currentID == matchIDs[i]) {
                    matchIndex = i;
                    break;
                }
            }
    
            if (matchIndex != -1 && statuses[matchIndex] == "Confirmed") {
                stringstream matchSS(line);
                string id, p1, p2, status, rest;
                getline(matchSS, id, ',');
                getline(matchSS, p1, ',');
                getline(matchSS, p2, ',');
                getline(matchSS, status, ',');
                p1 = p1.substr(p1.find_first_not_of(" "));
                p2 = p2.substr(p2.find_first_not_of(" "));
    
                for (int j = 0; j < injuriesToGenerate; j++) {
                    if (replacements[j].matchIndex == matchIndex) {
                        if (replacements[j].playerIndex == 0) {
                            p1 = replacements[j].replacement;
                        } else {
                            p2 = replacements[j].replacement;
                        }
                    }
                }
    
                outFile << id << ", " << p1 << ", " << p2 << ", Confirmed";
                if (getline(matchSS, rest)) outFile << rest;
                outFile << endl;
            } else {
                outFile << line << endl;
            }
        }
    
        inFile.close();
        outFile.close();
        remove("csv/TennisTournament.csv");
        rename("csv/TennisTournament_temp.csv", "csv/TennisTournament.csv");
    
        cout << "\nSemi-finals review completed." << endl;
        int processed = 0;
        for (int i = 0; i < injuriesToGenerate; i++) {
            if (!replacements[i].replacement.empty()) processed++;
        }
        cout << "Total replacements: " << processed << "/" << injuriesToGenerate << endl;
    }
    
    void finalConfirm() {
        cout << "REVIEWING FINAL MATCH FOR CONFIRMATION" << endl;
        cout << string(80, '-') << endl;
        
        // Check if final match exists
        if (!matchesExist("FINAL")) {
            cout << "No final match found in the tournament schedule." << endl;
            return;
        }
        
        // Check if final match has already been played
        if (hasWinners("FINAL")) {
            cout << "Final match has already been played. Cannot process confirmation." << endl;
            return;
        }
    
        // Read final match details
        string matchID, player1, player2, status;
        ifstream csvFile("csv/TennisTournament.csv");
        if (!csvFile) {
            cout << "Error: Could not open TennisTournament.csv file." << endl;
            return;
        }
        
        string headerLine;
        getline(csvFile, headerLine); // Skip header
        
        bool found = false;
        string line;
        while (getline(csvFile, line)) {
            stringstream ss(line);
            string currentID;
            getline(ss, currentID, ',');
            if (currentID.find("FINAL") != string::npos) {
                found = true;
                getline(ss, player1, ',');
                getline(ss, player2, ',');
                getline(ss, status, ',');
                
                // Trim whitespace
                player1 = player1.substr(player1.find_first_not_of(" "));
                player2 = player2.substr(player2.find_first_not_of(" "));
                break;
            }
        }
        csvFile.close();
    
        if (!found) {
            cout << "No final match found in the tournament schedule." << endl;
            return;
        }
    
        cout << "\nReviewing Final Match: " << matchID << endl;
        cout << "Players: " << player1 << " vs " << player2 << endl;
        
        cout << "Confirm this match? (Y/N): ";
        char choice;
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        if (toupper(choice) == 'Y') {
            // Update CSV file
            ifstream inFile("csv/TennisTournament.csv");
            ofstream outFile("csv/TennisTournament_temp.csv");
            
            if (!inFile || !outFile) {
                cout << "Error opening files for update." << endl;
                return;
            }
    
            getline(inFile, headerLine);
            outFile << headerLine << endl;
            
            while (getline(inFile, line)) {
                stringstream ss(line);
                string currentID;
                getline(ss, currentID, ',');
                
                if (currentID.find("FINAL") != string::npos) {
                    stringstream matchSS(line);
                    string id, p1, p2, oldStatus;
                    getline(matchSS, id, ',');
                    getline(matchSS, p1, ',');
                    getline(matchSS, p2, ',');
                    getline(matchSS, oldStatus, ',');
                    
                    // Write confirmed status
                    outFile << id << ", " << p1 << ", " << p2 << ", Confirmed";
                    string rest;
                    if (getline(matchSS, rest)) outFile << rest;
                    outFile << endl;
                } else {
                    outFile << line << endl;
                }
            }
    
            inFile.close();
            outFile.close();
            remove("csv/TennisTournament.csv");
            rename("csv/TennisTournament_temp.csv", "csv/TennisTournament.csv");
            
            cout << "\nFinal match confirmed successfully!" << endl;
        } else {
            cout << "Final match remains unconfirmed." << endl;
        }
    }
    
    // Main menu for player withdrawals
    void Menu() {
        int choice;
        do {
            cout << string(80, '=') << left << endl;
            cout << "HANDLING PLAYER WITHDRAWALS" << endl;
            cout << string(80, '=') << left << endl;
            cout << " 1. Qualifying Confirmation & Withdrawals" << endl;
            cout << " 2. Round Robin Confirmation & Withdrawals" << endl;
            cout << " 3. Semi-Finals Confirmation & Withdrawals" << endl;
            cout << " 4. Final Confirmation" << endl; // New option
            cout << " 5. Clear Screen" << endl;
            cout << " 6. Return to Main Menu" << endl; // Renumbered
            cout << string(80, '=') << left << endl;
            cout << "Choice: ";
            while (!(cin >> choice) || !(Checking(choice, 1, 6))) { // Updated to 6
                cout << "Invalid Input. Please enter your choice again correctly: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            switch (choice) {
                case 1:
                    qualifyingConfirm();
                    break;
                case 2:
                    roundRobinConfirm();
                    break;
                case 3:
                    semisConfirm();
                    break;
                case 4:
                    finalConfirm(); // New case
                    break;
                case 5:
                    clearScreen();
                    break;
                case 6:
                    cout << "Returning to main menu..." << endl;
                    break;
                default:
                    cout << "Undefined action" << endl;
                    break;
            }
        } while (choice != 6); // Updated condition
    }
};

#endif