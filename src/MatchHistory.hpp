#ifndef MATCHHISTORY_HPP
#define MATCHHISTORY_HPP
#include <iostream>
#include <fstream>
#include <limits>
#include <string>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include "HistoryStack.hpp"
#include "HistoryQueue.hpp"

using namespace std;

class MatchHistory
{
private:
    // History stack to store match records
    HistoryStack historyStack;
    HistoryQueue historyQueue;
    
public:
    MatchHistory() {
        srand(time(nullptr));
    }
    ~MatchHistory() {}
    
    // Helper functions
    //----------------------------------
    bool Checking(int value, int lower, int higher) {
        return value <= higher && value >= lower;
    }
    
    void clearScreen() {
        cout << "\033[2J\033[H";
    }

           //Change game status to confirmed
           void changeGameStatus(){
            ifstream inFile("csv/TennisTournament.csv");
            ofstream outFile("csv/TennisTournament_temp.csv");
            
            if (!inFile) {
                cout << "Input file not found" << endl;
                return;
            }
            
            if (!outFile) {
                cout << "Cannot create output file" << endl;
                inFile.close();
                return;
            }
            
            string line;
            
            // Copy the header line as is
            getline(inFile, line);
            outFile << line << endl;
            
            // Process each data line
            while (getline(inFile, line)) {
                // Replace "Unconfirmed" with "Confirmed" in the current line
                size_t pos = line.find("Unconfirmed");
                if (pos != string::npos) {
                    line.replace(pos, 11, "Confirmed");
                }
                
                // Write the modified line to the new file
                outFile << line << endl;
            }
            
            // Close both files
            inFile.close();
            outFile.close();
            
            // Replace the original file with the new one
            remove("csv/TennisTournament.csv");
            rename("csv/TennisTournament_temp.csv", "csv/TennisTournament.csv");
            
            cout << "All matches confirmed successfully" << endl;
        
        }
    // Check if all qualifier matches have been scored
bool allQualifiersScored() {
    ifstream csvFile("csv/TennisTournament.csv");
    if (!csvFile) {
        cout << "Error: Could not open TennisTournament.csv file." << endl;
        return false;
    }
    
    string headerLine;
    getline(csvFile, headerLine);
    
    // Check if the file has score columns
    bool hasScoreColumns = (headerLine.find("Score 1") != string::npos);
    if (!hasScoreColumns) {
        csvFile.close();
        return false;
    }
    
    string line;
    bool allScored = true;
    bool qualifiersExist = false;
    
    while (getline(csvFile, line)) {
        stringstream ss(line);
        string matchID, player1, player2, status;
        string score1, score2, winner;
        
        getline(ss, matchID, ',');
        getline(ss, player1, ',');
        getline(ss, player2, ',');
        getline(ss, status, ',');
        
        // Check if this is a qualifier match
        if (matchID.find("QUL_") != string::npos) {
            qualifiersExist = true;
            
            // Try to get score columns
            if (!(getline(ss, score1, ',') && getline(ss, score2, ',') && getline(ss, winner, ','))) {
                allScored = false;
                break;
            }
            
            // Check if scores are empty
            if (score1.empty() || score2.empty() || winner.empty()) {
                allScored = false;
                break;
            }
        }
    }
    
    csvFile.close();
    return qualifiersExist && allScored;
}

// Check if all Round Robin matches have been scored
bool allRoundRobinScored() {
    ifstream csvFile("csv/TennisTournament.csv");
    if (!csvFile) {
        cout << "Error: Could not open TennisTournament.csv file." << endl;
        return false;
    }
    
    string headerLine;
    getline(csvFile, headerLine);
    
    // Check if the file has score columns
    bool hasScoreColumns = (headerLine.find("Score 1") != string::npos);
    if (!hasScoreColumns) {
        csvFile.close();
        return false;
    }
    
    string line;
    bool allScored = true;
    bool roundRobinExists = false;
    
    while (getline(csvFile, line)) {
        stringstream ss(line);
        string matchID, player1, player2, status;
        string score1, score2, winner;
        
        getline(ss, matchID, ',');
        getline(ss, player1, ',');
        getline(ss, player2, ',');
        getline(ss, status, ',');
        
        // Check if this is a round robin match
        if (matchID.find("ROU") != string::npos) {
            roundRobinExists = true;
            
            // Try to get score columns
            if (!(getline(ss, score1, ',') && getline(ss, score2, ',') && getline(ss, winner, ','))) {
                allScored = false;
                break;
            }
            
            // Check if scores are empty
            if (score1.empty() || score2.empty() || winner.empty()) {
                allScored = false;
                break;
            }
        }
    }
    
    csvFile.close();
    return roundRobinExists && allScored;
}

// Function to record or update loser information to a CSV file
void recordLoserInfo(string playerName, int roundsWon, string matchID) {
    // Check if file exists
    ifstream checkFile("csv/losers.csv");
    bool fileExists = checkFile.good();
    
    if (!fileExists) {
        // File doesn't exist, create and add header
        ofstream newFile("csv/losers.csv");
        if (newFile.is_open()) {
            newFile << "Player Name,Rounds Won,Match ID" << endl;
            newFile << playerName << "," << roundsWon << "," << matchID << endl;
            newFile.close();
            return;
        }
    } else {
        // File exists, read all lines into an array
        const int MAX_LINES = 100; // Adjust as needed
        string lines[MAX_LINES];
        int lineCount = 0;
        
        // Read header
        string header;
        getline(checkFile, header);
        lines[lineCount++] = header;
        
        // Read all data lines
        string line;
        bool matchFound = false;
        
        while (getline(checkFile, line) && lineCount < MAX_LINES) {
            stringstream ss(line);
            string name, rounds, id;
            
            getline(ss, name, ',');
            getline(ss, rounds, ',');
            getline(ss, id);
            
            // Check if this is the match we're updating
            if (id == matchID) {
                // Replace with new loser information
                lines[lineCount++] = playerName + "," + to_string(roundsWon) + "," + matchID;
                matchFound = true;
            } else {
                // Keep this line as is
                lines[lineCount++] = line;
            }
        }
        
        checkFile.close();
        
        // If match not found, add new entry
        if (!matchFound && lineCount < MAX_LINES) {
            lines[lineCount++] = playerName + "," + to_string(roundsWon) + "," + matchID;
        }
        
        // Write all lines back to file
        ofstream loserFile("csv/losers.csv");
        if (loserFile.is_open()) {
            for (int i = 0; i < lineCount; i++) {
                loserFile << lines[i] << endl;
            }
            loserFile.close();
        } else {
            cout << "Error: Could not open losers.csv file for writing." << endl;
        }
    }
}

    // Score recording functions
    //----------------------------------
    // Modified function to set random scores for qualifier matches
void setQualifiersScores() {
    cout << "Recording Qualifier Match Scores" << endl;
    cout << string(80, '-') << endl;
    
    // Open the CSV file for reading
    ifstream csvFile("csv/TennisTournament.csv");
    if (!csvFile) {
        cout << "Error: Could not open TennisTournament.csv file." << endl;
        return;
    }
    
    // Store the header line and all content for rewriting later
    string headerLine;
    getline(csvFile, headerLine);
    
    // Check if the file already has score columns
    bool hasScoreColumns = (headerLine.find("Score 1") != string::npos);
    
    // If score columns don't exist, update the header
    if (!hasScoreColumns) {
        headerLine += ", Score 1, Score 2, Winner";
    }
    
    // Read all lines into an array
    const int MAX_LINES = 100; // Adjust as needed
    string lines[MAX_LINES];
    int lineCount = 0;
    
    string line;
    while (getline(csvFile, line) && lineCount < MAX_LINES) {
        lines[lineCount++] = line;
    }
    
    csvFile.close();
    
    // Process qualifier matches
    bool qualifierMatches = false;
    int processedCount = 0;
    
    for (int i = 0; i < lineCount; i++) {
        stringstream ss(lines[i]);
        string matchID, player1, player2, status;
        string scoreStr1 = "", scoreStr2 = "", winnerStr = "";
        
        // Parse the CSV line
        getline(ss, matchID, ',');
        getline(ss, player1, ',');
        getline(ss, player2, ',');
        getline(ss, status, ',');
        
        // Trim whitespace
        player1 = player1.substr(player1.find_first_not_of(" "));
        player2 = player2.substr(player2.find_first_not_of(" "));
        
        // Check if this is a qualifier match
        if (matchID.find("QUL_") != string::npos) {
            qualifierMatches = true;
            
            // Check if scores are already recorded
            if (hasScoreColumns) {
                getline(ss, scoreStr1, ',');
                getline(ss, scoreStr2, ',');
                getline(ss, winnerStr, ',');
            }
            
            // If match is already scored, skip it
            if (scoreStr1 != "" && scoreStr2 != "" && winnerStr != "") {
                cout << "\nMatch " << matchID << " already has scores recorded: " << scoreStr1 << "-" << scoreStr2 << endl;
                continue;
            }
            
            // Check if match is confirmed or not
            if (status.find("Unconfirmed") != string::npos) {
                cout << "\nMatch " << matchID << " is still unconfirmed. Cannot enter scores." << endl;
                cout << player1 << " VS " << player2 << endl;
                
                // Ask user if they want to confirm this match
                char choice;
                cout << "Do you want to confirm this match? (Y/N): ";
                cin >> choice;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                
                if (toupper(choice) != 'Y') {
                    cout << "Match remains unconfirmed." << endl;
                    continue;
                }
                
                // Update status to Confirmed
                status = "Confirmed";
                cout << "Match confirmed successfully." << endl;
            }
            
            // Display match details
            cout << "\nMatch ID: " << matchID << endl;
            cout << player1 << " VS " << player2 << endl;
            
            // Generate random scores (0-3) ensuring no draw
            int score1, score2;
            do {
                score1 = rand() % 4; // 0-3
                score2 = rand() % 4; // 0-3
            } while (score1 == score2); // Ensure no draw
            
            // Determine winner
            string winner;
            if (score1 > score2) {
                winner = player1;
            } else {
                winner = player2;
            }
            


            cout << "Generated Scores: " << player1 << " " << score1 << " - " << score2 << " " << player2 << endl;
            cout << "Winner: " << winner << endl;
            
            string loser = (winner == player1) ? player2 : player1;
            int loserScore = (winner == player1) ? score2 : score1;
            recordLoserInfo(loser, loserScore, matchID);

            // Update the line with scores
            lines[i] = matchID + ", " + player1 + ", " + player2 + ", " + status + ", " + 
                      to_string(score1) + ", " + to_string(score2) + ", " + winner;
            
            // Add to history stack
            historyStack.push(matchID, player1, player2, score1, score2, winner, "Qualifier");
            historyQueue.enqueue(matchID, player1, player2, score1, score2, winner, "Qualifier");
            processedCount++;
        }
    }
    
    if (!qualifierMatches) {
        cout << "No qualifier matches found in the CSV file." << endl;
        return;
    }
    
    if (processedCount == 0) {
        cout << "\nNo qualifier match scores were updated." << endl;
        cout << "This could be because all matches were either already scored or remain unconfirmed." << endl;
        return;
    }
    
    // Write back to the CSV file
    ofstream outFile("csv/TennisTournament.csv");
    if (!outFile) {
        cout << "Error: Could not open file for writing." << endl;
        return;
    }
    
    // Write the header
    outFile << headerLine << endl;
    
    // Write all lines
    for (int i = 0; i < lineCount; i++) {
        outFile << lines[i] << endl;
    }
    
    outFile.close();
    
    cout << "\nQualifier scores have been recorded successfully!" << endl;
}

// Modified function to set random scores for round robin matches, going round by round
void setRoundRobinScores() {
    cout << "Recording Round Robin Match Scores" << endl;
    cout << string(80, '-') << endl;
    
    // Check if all qualifier matches have been scored first
    if (!allQualifiersScored()) {
        cout << "Error: All qualifier matches must be scored before recording Round Robin matches." << endl;
        cout << "Please complete all qualifier match scores first." << endl;
        return;
    }
    
    // Open the CSV file for reading
    ifstream csvFile("csv/TennisTournament.csv");
    if (!csvFile) {
        cout << "Error: Could not open TennisTournament.csv file." << endl;
        return;
    }
    
    // Store the header line and all content for rewriting later
    string headerLine;
    getline(csvFile, headerLine);
    
    // Check if the file already has score columns
    bool hasScoreColumns = (headerLine.find("Score 1") != string::npos);
    
    // If score columns don't exist, update the header
    if (!hasScoreColumns) {
        headerLine += ", Score 1, Score 2, Winner";
    }
    
    // Read all lines into an array
    const int MAX_LINES = 100; // Adjust as needed
    string lines[MAX_LINES];
    int lineCount = 0;
    
    string line;
    while (getline(csvFile, line) && lineCount < MAX_LINES) {
        lines[lineCount++] = line;
    }
    
    csvFile.close();
    
    // Find the next round to process
    int nextRound = 1;
    bool foundIncomplete = false;
    
    // First, check for any unconfirmed matches in any round
    for (int round = 1; round <= 7; round++) {
        string roundPrefix = "ROU" + to_string(round);
        bool allConfirmed = true;
        bool roundExists = false;
        
        for (int i = 0; i < lineCount; i++) {
            stringstream ss(lines[i]);
            string matchID, player1, player2, status;
            
            getline(ss, matchID, ',');
            if (matchID.find(roundPrefix) != string::npos) {
                roundExists = true;
                getline(ss, player1, ',');
                getline(ss, player2, ',');
                getline(ss, status, ',');
                
                if (status.find("Unconfirmed") != string::npos) {
                    allConfirmed = false;
                    if (!foundIncomplete) {
                        nextRound = round;
                        foundIncomplete = true;
                    }
                    break;
                }
            }
        }
        
        if (!roundExists) {
            // If we've reached a round that doesn't exist, break
            break;
        }
        
        if (!allConfirmed) {
            // Found a round with unconfirmed matches
            break;
        }
        
        // Check if all matches in this round are scored
        bool allScored = true;
        for (int i = 0; i < lineCount; i++) {
            stringstream ss(lines[i]);
            string matchID, player1, player2, status;
            string score1, score2, winner;
            
            getline(ss, matchID, ',');
            if (matchID.find(roundPrefix) != string::npos) {
                getline(ss, player1, ',');
                getline(ss, player2, ',');
                getline(ss, status, ',');
                
                if (hasScoreColumns) {
                    if (!(getline(ss, score1, ',') && getline(ss, score2, ',') && getline(ss, winner, ','))) {
                        allScored = false;
                        break;
                    }
                    if (score1.empty() || score2.empty() || winner.empty()) {
                        allScored = false;
                        break;
                    }
                } else {
                    allScored = false;
                    break;
                }
            }
        }
        
        if (!allScored) {
            if (!foundIncomplete) {
                nextRound = round;
                foundIncomplete = true;
            }
            break;
        }
        
        // If all matches in this round are confirmed and scored, move to next round
        nextRound = round + 1;
    }
    
    // Process the next round to be scored
    string roundPrefix = "ROU" + to_string(nextRound);
    bool roundExists = false;
    bool hasUnconfirmed = false;
    int processedCount = 0;
    
    cout << "Processing Round Robin Round " << nextRound << endl;
    
    // First check if any match in this round is unconfirmed
    for (int i = 0; i < lineCount; i++) {
        stringstream ss(lines[i]);
        string matchID, player1, player2, status;
        
        getline(ss, matchID, ',');
        if (matchID.find(roundPrefix) != string::npos) {
            roundExists = true;
            getline(ss, player1, ',');
            getline(ss, player2, ',');
            getline(ss, status, ',');
            
            if (status.find("Unconfirmed") != string::npos) {
                hasUnconfirmed = true;
                cout << "\nMatch " << matchID << " is unconfirmed: " << player1 << " VS " << player2 << endl;
                
                // Ask user if they want to confirm this match
                char choice;
                cout << "Do you want to confirm this match? (Y/N): ";
                cin >> choice;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                
                if (toupper(choice) != 'Y') {
                    cout << "Match remains unconfirmed. Cannot proceed to scoring until all matches are confirmed." << endl;
                    return;
                }
                
                // Update status to Confirmed
                lines[i] = matchID + ", " + player1 + ", " + player2 + ", Confirmed";
                cout << "Match confirmed successfully." << endl;
            }
        }
    }
    
    if (!roundExists) {
        cout << "No Round Robin matches found for round " << nextRound << "." << endl;
        return;
    }
    
    // Now proceed with scoring all confirmed matches in this round
    for (int i = 0; i < lineCount; i++) {
        stringstream ss(lines[i]);
        string matchID, player1, player2, status;
        string scoreStr1 = "", scoreStr2 = "", winnerStr = "";
        
        // Parse the CSV line
        getline(ss, matchID, ',');
        
        // Check if this is a match from the current round
        if (matchID.find(roundPrefix) != string::npos) {
            getline(ss, player1, ',');
            getline(ss, player2, ',');
            getline(ss, status, ',');
            
            // Trim whitespace
            player1 = player1.substr(player1.find_first_not_of(" "));
            player2 = player2.substr(player2.find_first_not_of(" "));
            
            // Check if scores are already recorded
            if (hasScoreColumns) {
                getline(ss, scoreStr1, ',');
                getline(ss, scoreStr2, ',');
                getline(ss, winnerStr, ',');
            }
            
            // If match is already scored, skip it
            if (scoreStr1 != "" && scoreStr2 != "" && winnerStr != "") {
                cout << "\nMatch " << matchID << " already has scores recorded: " << scoreStr1 << "-" << scoreStr2 << endl;
                continue;
            }
            
            // Check if match is confirmed
            if (status.find("Unconfirmed") != string::npos) {
                cout << "\nMatch " << matchID << " is still unconfirmed. Cannot enter scores." << endl;
                continue;
            }
            
            // Display match details
            cout << "\nMatch ID: " << matchID << endl;
            cout << player1 << " VS " << player2 << endl;
            
            // Generate random scores (0-3) ensuring no draw
            int score1, score2;
            do {
                score1 = rand() % 4; // 0-3
                score2 = rand() % 4; // 0-3
            } while (score1 == score2); // Ensure no draw
            
            // Determine winner
            string winner;
            if (score1 > score2) {
                winner = player1;
            } else {
                winner = player2;
            }

            string loser = (winner == player1) ? player2 : player1;
            int loserScore = (winner == player1) ? score2 : score1;
            recordLoserInfo(loser, loserScore, matchID);
            
            cout << "Generated Scores: " << player1 << " " << score1 << " - " << score2 << " " << player2 << endl;
            cout << "Winner: " << winner << endl;
            
            // Update the line with scores
            lines[i] = matchID + ", " + player1 + ", " + player2 + ", Confirmed, " + 
                      to_string(score1) + ", " + to_string(score2) + ", " + winner;
            
            // Add to history stack
            historyStack.push(matchID, player1, player2, score1, score2, winner, "Round Robin");
            historyQueue.enqueue(matchID, player1, player2, score1, score2, winner, "Round Robin");
            processedCount++;
        }
    }
    
    if (processedCount == 0) {
        cout << "\nNo Round Robin match scores were updated for round " << nextRound << "." << endl;
        cout << "This could be because all matches were either already scored or still unconfirmed." << endl;
        return;
    }
    
    // Write back to the CSV file
    ofstream outFile("csv/TennisTournament.csv");
    if (!outFile) {
        cout << "Error: Could not open file for writing." << endl;
        return;
    }
    
    // Write the header
    outFile << headerLine << endl;
    
    // Write all lines
    for (int i = 0; i < lineCount; i++) {
        outFile << lines[i] << endl;
    }
    
    outFile.close();
    
    cout << "\nRound Robin scores for round " << nextRound << " have been recorded successfully!" << endl;
}
    
    void setKnockoutScores() {
        cout << "Recording Knockout Match Scores" << endl;
        cout << string(80, '-') << endl;
        
        // Check if all Round Robin matches have been scored first
        if (!allRoundRobinScored()) {
            cout << "Error: All Round Robin matches must be scored before recording Knockout matches." << endl;
            cout << "Please complete all Round Robin match scores first." << endl;
            return;
        }
        
        // Open the CSV file for reading
        ifstream csvFile("csv/TennisTournament.csv");
        if (!csvFile) {
            cout << "Error: Could not open TennisTournament.csv file." << endl;
            return;
        }
        
        // Store the header line and all content for rewriting later
        string headerLine;
        getline(csvFile, headerLine);
        
        // Check if the file already has score columns
        bool hasScoreColumns = (headerLine.find("Score 1") != string::npos);
        
        // If score columns don't exist, update the header
        if (!hasScoreColumns) {
            headerLine += ", Score 1, Score 2, Winner";
        }
        
        // Read all lines into an array
        const int MAX_LINES = 100; // Adjust as needed
        string lines[MAX_LINES];
        int lineCount = 0;
        
        string line;
        while (getline(csvFile, line) && lineCount < MAX_LINES) {
            lines[lineCount++] = line;
        }
        
        csvFile.close();
        
        // Check if there are semi-final and final matches
        bool hasSemiFinals = false;
        bool hasFinal = false;
        
        for (int i = 0; i < lineCount; i++) {
            stringstream ss(lines[i]);
            string matchID;
            getline(ss, matchID, ',');
            
            if (matchID.find("Semi_") != string::npos) {
                hasSemiFinals = true;
            } else if (matchID.find("FINAL") != string::npos) {
                hasFinal = true;
            }
        }
        
        if (!hasSemiFinals && !hasFinal) {
            cout << "No Knockout matches found in the CSV file." << endl;
            return;
        }
        
        // Ask user which stage they want to score
        int stageChoice;
        cout << "Knockout Stages Available:" << endl;
        if (hasSemiFinals) cout << "1. Semi-Finals" << endl;
        if (hasFinal) cout << "2. Final" << endl;
        cout << "3. Return to previous menu" << endl;
        cout << "Which stage would you like to score? ";
        
        int maxChoice = hasFinal ? 3 : 2;
        
        while (!(cin >> stageChoice) || !(Checking(stageChoice, 1, maxChoice))) {
            cout << "Invalid Input. Please enter a valid choice: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        
        if (stageChoice == 3) {
            cout << "Returning to previous menu." << endl;
            return;
        }
        
        // Determine which match prefix to look for
        string stagePrefix = (stageChoice == 1) ? "Semi_" : "FINAL";
        string stageName = (stageChoice == 1) ? "Semi-Finals" : "Final";
        
        // Process each line for the selected knockout stage
        bool stageMatches = false;
        int processedCount = 0;
        
        for (int i = 0; i < lineCount; i++) {
            stringstream ss(lines[i]);
            string matchID, player1, player2, status;
            string scoreStr1 = "", scoreStr2 = "", winnerStr = "";
            
            // Parse the CSV line
            getline(ss, matchID, ',');
            getline(ss, player1, ',');
            getline(ss, player2, ',');
            getline(ss, status, ',');
            
            // Trim whitespace
            player1 = player1.substr(player1.find_first_not_of(" "));
            player2 = player2.substr(player2.find_first_not_of(" "));
            
            // Check if this is a match from the selected stage
            if (matchID.find(stagePrefix) != string::npos) {
                stageMatches = true;
                
                // Check if scores are already recorded
                if (hasScoreColumns) {
                    getline(ss, scoreStr1, ',');
                    getline(ss, scoreStr2, ',');
                    getline(ss, winnerStr, ',');
                }
                
                // If match is already scored, skip it
                if (scoreStr1 != "" && scoreStr2 != "" && winnerStr != "") {
                    cout << "\nMatch " << matchID << " already has scores recorded: " << scoreStr1 << "-" << scoreStr2 << endl;
                    continue;
                }
                
                // Check if match is confirmed or not
                if (status.find("Unconfirmed") != string::npos) {
                    cout << "\nMatch " << matchID << " is still unconfirmed. Cannot enter scores." << endl;
                    cout << player1 << " VS " << player2 << endl;
                    continue;
                }
                
                // Display match details
                cout << "\nMatch ID: " << matchID << endl;
                cout << player1 << " VS " << player2 << endl;
                
                // Ask user if they want to enter scores for this match
                char choice;
                cout << "Do you want to enter scores for this match? (Y/N): ";
                cin >> choice;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                
                if (toupper(choice) != 'Y') {
                    cout << "Skipping this match." << endl;
                    continue;
                }
                
                // Get scores from user
                int score1, score2;
                
                cout << "Enter score for " << player1 << ": ";
                while (!(cin >> score1) || score1 < 0) {
                    cout << "Invalid input. Please enter a non-negative number: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                
                cout << "Enter score for " << player2 << ": ";
                while (!(cin >> score2) || score2 < 0) {
                    cout << "Invalid input. Please enter a non-negative number: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                
                // Determine winner
                string winner;
                if (score1 > score2) {
                    winner = player1;
                } else if (score2 > score1) {
                    winner = player2;
                } else {
                    winner = "Tie";
                }

                // After determining the winner
                string loser = (winner == player1) ? player2 : player1;
                int loserScore = (winner == player1) ? score2 : score1;
                recordLoserInfo(loser, loserScore, matchID);
                
                cout << "Winner: " << winner << endl;
                
                // Update the line with scores
                lines[i] = matchID + ", " + player1 + ", " + player2 + ", Confirmed, " + 
                          to_string(score1) + ", " + to_string(score2) + ", " + winner;
                
                // Add to history stack
                historyStack.push(matchID, player1, player2, score1, score2, winner, "Knockout");
                historyQueue.enqueue(matchID, player1, player2, score1, score2, winner, "Knockout");
                processedCount++;
            }
        }
        
        if (!stageMatches) {
            cout << "No matches found for " << stageName << "." << endl;
            return;
        }
        
        if (processedCount == 0) {
            cout << "\nNo " << stageName << " match scores were updated." << endl;
            cout << "This could be because all matches were either already scored, unconfirmed, or skipped." << endl;
            return;
        }
        
        // Write back to the CSV file
        ofstream outFile("csv/TennisTournament.csv");
        if (!outFile) {
            cout << "Error: Could not open file for writing." << endl;
            return;
        }
        
        // Write the header
        outFile << headerLine << endl;
        
        // Write all lines
        for (int i = 0; i < lineCount; i++) {
            outFile << lines[i] << endl;
        }
        
        outFile.close();
        
        cout << "\n" << stageName << " scores have been recorded successfully!" << endl;
    }
    
    // Update scores for the most recent match
    void updateScores() {
        cout << "Updating Match Scores for Most Recent Match" << endl;
        cout << string(80, '-') << endl;
        
        if (historyStack.isEmpty()) {
            cout << "No match history available to update!" << endl;
            return;
        }
        
        // Get the most recent match (top of stack)
        MatchHistoryNode* recentMatch = historyStack.peek();
        if (!recentMatch) {
            cout << "Error retrieving most recent match." << endl;
            return;
        }
        
        // Display current match details
        cout << "Most Recent Match Selected for Update:" << endl;
        cout << "Match ID: " << recentMatch->matchID << endl;
        cout << "Match Type: " << recentMatch->matchType << endl;
        cout << "Players: " << recentMatch->player1 << " VS " << recentMatch->player2 << endl;
        cout << "Current Score: " << recentMatch->score1 << " - " << recentMatch->score2 << endl;
        cout << "Current Winner: " << recentMatch->winner << endl;
        
        // Confirm if user wants to update this match
        char choice;
        cout << "\nDo you want to update the score for this match? (Y/N): ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        if (toupper(choice) != 'Y') {
            cout << "Update cancelled." << endl;
            return;
        }
        
        // Get new scores
        int newScore1, newScore2;
        
        cout << "Enter new score for " << recentMatch->player1 << ": ";
        while (!(cin >> newScore1) || newScore1 < 0) {
            cout << "Invalid input. Please enter a non-negative number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        
        cout << "Enter new score for " << recentMatch->player2 << ": ";
        while (!(cin >> newScore2) || newScore2 < 0) {
            cout << "Invalid input. Please enter a non-negative number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        
        // Determine new winner based on scores
        string newWinner;
        if (newScore1 > newScore2) {
            newWinner = recentMatch->player1;
        } else if (newScore2 > newScore1) {
            newWinner = recentMatch->player2;
        } else {
            newWinner = "Tie";
        }
        
        // Update the match in the CSV file
        bool csvUpdated = updateMatchInCSV(recentMatch->matchID, newScore1, newScore2, newWinner);
        
        if (csvUpdated) {
            // Update the match in the stack
            recentMatch->score1 = newScore1;
            recentMatch->score2 = newScore2;
            recentMatch->winner = newWinner;
            
            // We also need to update the corresponding match in the queue
            // This is more complex since we can't directly access queue elements
            // For simplicity, we'll recreate the queue from CSV file
            
            cout << "\nMatch scores updated successfully!" << endl;
            cout << "New Score: " << recentMatch->score1 << " - " << recentMatch->score2 << endl;
            cout << "New Winner: " << recentMatch->winner << endl;
            
            string loser = (newWinner == recentMatch->player1) ? recentMatch->player2 : recentMatch->player1;
            int loserScore = (newWinner == recentMatch->player1) ? newScore2 : newScore1;
            recordLoserInfo(loser, loserScore, recentMatch->matchID);


        } else {
            cout << "\nFailed to update match in CSV file. No changes were made." << endl;
        }
    }
    
    // Helper function to update match in CSV file
    bool updateMatchInCSV(string matchID, int score1, int score2, string winner) {
        ifstream inFile("csv/TennisTournament.csv");
        ofstream outFile("csv/TennisTournament_temp.csv");
        
        if (!inFile || !outFile) {
            cout << "Error opening files for update." << endl;
            return false;
        }
        
        string line, headerLine;
        bool matchFound = false;
        
        // Copy header
        getline(inFile, headerLine);
        outFile << headerLine << endl;
        
        // Process each line
        while (getline(inFile, line)) {
            stringstream ss(line);
            string currentMatchID, rest;
            
            getline(ss, currentMatchID, ',');
            
            if (currentMatchID == matchID) {
                // This is the match to update
                matchFound = true;
                
                // Extract parts up to the score columns
                string player1, player2, status;
                getline(ss, player1, ',');
                getline(ss, player2, ',');
                getline(ss, status, ',');
                
                // Write updated line with new scores
                outFile << matchID << ", " << player1 << ", " << player2 << ", " << status << ", "
                       << score1 << ", " << score2 << ", " << winner << endl;
            } else {
                // Copy line as is
                outFile << line << endl;
            }
        }
        
        inFile.close();
        outFile.close();
        
        if (!matchFound) {
            cout << "Warning: Match ID " << matchID << " not found in CSV file." << endl;
            return false;
        }
        
        // Replace original file with updated file
        remove("csv/TennisTournament.csv");
        rename("csv/TennisTournament_temp.csv", "csv/TennisTournament.csv");
        
        return true;
    }

    
void displayCurrentReport() {
    cout << "Current Tournament Report" << endl;
    cout << string(80, '-') << endl;
    
    ifstream csvFile("csv/TennisTournament.csv");
    if (!csvFile) {
        cout << "Error: Could not open TennisTournament.csv file." << endl;
        return;
    }
    
    // Skip header line
    string line;
    getline(csvFile, line);
    
    // Variables to track statistics
    int totalMatches = 0;
    int qualifierMatches = 0;
    int roundRobinMatches = 0;
    int knockoutMatches = 0;
    int scoredMatches = 0;
    int unconfirmedMatches = 0;
    
    // Read and count each match
    while (getline(csvFile, line)) {
        totalMatches++;
        
        stringstream ss(line);
        string matchID, player1, player2, status;
        
        getline(ss, matchID, ',');
        getline(ss, player1, ',');
        getline(ss, player2, ',');
        getline(ss, status, ',');
        
        // Count match types
        if (matchID.find("QUL_") != string::npos) {
            qualifierMatches++;
        } else if (matchID.find("ROU") != string::npos) {
            roundRobinMatches++;
        } else if (matchID.find("Semi_") != string::npos || matchID.find("FINAL") != string::npos) {
            knockoutMatches++;
        }
        
        // Count match statuses
        if (status.find("Unconfirmed") != string::npos) {
            unconfirmedMatches++;
        } else {
            // Try to read score columns to determine if match is scored
            string score1, score2, winner;
            if (getline(ss, score1, ',') && getline(ss, score2, ',') && getline(ss, winner, ',')) {
                if (!score1.empty() && !score2.empty() && !winner.empty()) {
                    scoredMatches++;
                }
            }
        }
    }
    
    csvFile.close();
    
    // Display the tournament summary
    cout << "\nTournament Summary:" << endl;
    cout << string(80, '-') << endl;
    cout << "Total Matches: " << totalMatches << endl;
    cout << "Qualifier Matches: " << qualifierMatches << endl;
    cout << "Round Robin Matches: " << roundRobinMatches << endl;
    cout << "Knockout Matches: " << knockoutMatches << endl;
    cout << string(80, '-') << endl;
    cout << "Matches with Scores: " << scoredMatches << endl;
    cout << "Unconfirmed Matches: " << unconfirmedMatches << endl;
    cout << string(80, '-') << endl;
    
    // Show tournament progress
    cout << "\nTournament Stage Progress:" << endl;
    cout << string(80, '-') << endl;
    
    double qualifierProgress = (qualifierMatches > 0) ? 
                              ((double)min(scoredMatches, qualifierMatches) / qualifierMatches * 100) : 0;
    double roundRobinProgress = (roundRobinMatches > 0) ? 
                               ((double)min(max(0, scoredMatches - qualifierMatches), roundRobinMatches) / roundRobinMatches * 100) : 0;
    double knockoutProgress = (knockoutMatches > 0) ? 
                             ((double)min(max(0, scoredMatches - qualifierMatches - roundRobinMatches), knockoutMatches) / knockoutMatches * 100) : 0;
    
    cout << "Qualifiers Stage: " << fixed << setprecision(1) << qualifierProgress << "% complete" << endl;
    cout << "Round Robin Stage: " << fixed << setprecision(1) << roundRobinProgress << "% complete" << endl;
    cout << "Knockout Stage: " << fixed << setprecision(1) << knockoutProgress << "% complete" << endl;
    
    cout << "\nOverall Tournament Progress: " << fixed << setprecision(1) << 
        (totalMatches > 0 ? ((double)scoredMatches / totalMatches * 100) : 0) << "% complete" << endl;
    
    // If we have scored matches in our history stack, show the most recent
    if (historyStack.getSize() > 0) {
        cout << "\nMost Recent Matches (LIFO Order):" << endl;
        historyStack.displayHistory();
    }
}
    
void displayHistoricalLogs() {
    cout << "Match History Logs" << endl;
    cout << string(80, '-') << endl;
    
    int choice;
    
    do {
        cout << "\nView Match History:" << endl;
        cout << "1. Most Recent First (LIFO - Stack)" << endl;
        cout << "2. Oldest First (FIFO - Queue)" << endl;
        cout << "3. Filter by Match Type" << endl;
        cout << "4. Return to Previous Menu" << endl;
        cout << "Choice: ";
        
        while (!(cin >> choice) || !(Checking(choice, 1, 4))) {
            cout << "Invalid Input. Please enter your choice again correctly: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        
        switch (choice) {
            case 1:
                // Display using stack (LIFO)
                historyStack.displayHistory();
                break;
            case 2:
                // Display using queue (FIFO)
                historyQueue.displayHistory();
                break;
            case 3: {
                // Filter by match type submenu
                int typeChoice;
                cout << "\nSelect Match Type:" << endl;
                cout << "1. Qualifier Matches" << endl;
                cout << "2. Round Robin Matches" << endl;
                cout << "3. Knockout Matches" << endl;
                cout << "Choice: ";
                
                while (!(cin >> typeChoice) || !(Checking(typeChoice, 1, 3))) {
                    cout << "Invalid Input. Please enter your choice again correctly: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                
                cout << "\nView Order:" << endl;
                cout << "1. Most Recent First (LIFO)" << endl;
                cout << "2. Oldest First (FIFO)" << endl;
                cout << "Choice: ";
                
                int orderChoice;
                while (!(cin >> orderChoice) || !(Checking(orderChoice, 1, 2))) {
                    cout << "Invalid Input. Please enter your choice again correctly: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
                
                string matchType;
                switch (typeChoice) {
                    case 1: matchType = "Qualifier"; break;
                    case 2: matchType = "Round Robin"; break;
                    case 3: matchType = "Knockout"; break;
                    default: matchType = "Qualifier";
                }
                
                if (orderChoice == 1) {
                    // LIFO - Stack
                    historyStack.displayHistoryByType(matchType);
                } else {
                    // FIFO - Queue
                    historyQueue.displayHistoryByType(matchType);
                }
                break;
            }
            case 4:
                cout << "Returning to previous menu..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 4);
}
    
    // Set Scores Sub-Menu
    //----------------------------------
    void setScoresMenu() {
        int choice;
        
        do {
            changeGameStatus();
            cout << string(80, '=') << left << endl;
            cout << "SET MATCH SCORES" << endl;
            cout << string(80, '=') << left << endl;
            cout << " 1. Qualifiers Matches" << endl;
            cout << " 2. Round Robin Matches" << endl;
            cout << " 3. Knockout Matches" << endl;
            cout << " 4. Return to Match History Menu" << endl;
            cout << string(80, '=') << left << endl;
            cout << "Choice: ";
            
            while (!(cin >> choice) || !(Checking(choice, 1, 4))) {
                cout << "Invalid Input. Please enter your choice again correctly: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            
            switch (choice) {
                case 1:
                    setQualifiersScores();
                    break;
                case 2:
                    setRoundRobinScores();
                    break;
                case 3:
                    setKnockoutScores();
                    break;
                case 4:
                    cout << "Returning to main menu..." << endl;
                    break;
                default:
                    cout << "Undefined action" << endl;
                    break;
            }
        } while (choice != 4);
    }
    
    // Main Menu
    //----------------------------------
    void Menu() {
        int choice;
        
        do {
            cout << string(80, '=') << left << endl;
            cout << "MATCH HISTORY TRACKING" << endl;
            cout << string(80, '=') << left << endl;
            cout << " 1. Set Scores" << endl;
            cout << " 2. Update Scores" << endl;
            cout << " 3. Current Tournament Report" << endl;
            cout << " 4. Historical Logs" << endl;
            cout << " 5. Clear Screen" << endl;
            cout << " 6. Return to Main Menu" << endl;
            cout << string(80, '=') << left << endl;
            cout << "Choice: ";
            
            while (!(cin >> choice) || !(Checking(choice, 1, 6))) {
                cout << "Invalid Input. Please enter your choice again correctly: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            
            switch (choice) {
                case 1:
                    setScoresMenu();
                    break;
                case 2:
                    updateScores();
                    break;
                case 3:
                    displayCurrentReport();
                    break;
                case 4:
                    displayHistoricalLogs();
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
        } while (choice != 6);
    }
};

#endif