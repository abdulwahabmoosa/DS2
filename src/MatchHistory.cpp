#include <iostream>
#include <fstream>
#include <limits>

using namespace std;

class MatchHistory
{

    public:
        MatchHistory(){

        }
        ~MatchHistory(){}

        void changeGameStatus(){
            fstream file("csv/TennisTournament.csv", ios::in | ios::out);
            
            if (!file) {
                cout << "File not found or cannot be opened" << endl;
                return;
            }
            
            string line;
            
            // Skip the header line
            getline(file, line);
            
            // Start position after the header
            streampos position = file.tellg();
            
            // Process each line
            while (getline(file, line)) {
                // Find the position of "Unconfirmed" in the line
                size_t pos = line.find("Unconfirmed");
                
                if (pos != string::npos) {
                    // Move back to the beginning of this line
                    file.seekp(position);
                    
                    // Replace "Unconfirmed" with "Confirmed" (same length)
                    line.replace(pos, 11, "Confirmed  ");
                    
                    // Write the updated line back
                    file << line;
                }
                
                // Save the position at the beginning of the next line
                position = file.tellg();
            }
            
            file.close();
            cout << "All matches confirmed successfully" << endl;
        
        }
        void Menu(){
            
        }
    

};

int main(){
    MatchHistory match;
    match.changeGameStatus();
}