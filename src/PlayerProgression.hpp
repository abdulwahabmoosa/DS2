#ifndef PLAYERPROGRESSION_HPP
#define PLAYERPROGRESSION_HPP
#include <iostream>
#include <limits>
#include <fstream>
#include "Qualifiers.hpp"
#include "Knockout.hpp"
#include "RoundRobin.hpp"
#include <sstream>
using namespace std;
class PlayersProgression
{   
    private:
        string* players;
        bool Qdrawn=false;
        bool Rdrawn=false;
        bool Kdrawn=false;
        bool Kfinal=false;
        int playersNum; 
    public:
        Knockouts_Stack Kstack;
        Qualifiers_Queue Qqueue;
        CircularQueue roundRobin;
        
        PlayersProgression(): roundRobin(16){
            playersNum=16;
            players=new string[playersNum]{"Roger Federer","Rafael Nadal","Novak Djokovic","Pete Sampras","Bjorn Borg","Rod Laver",
            "Andre Agassi","Jimmy Connors","John McEnroe","Ivan Lendl","Stefan Edberg",
            "Boris Becker","Andy Murray","Stan Wawrinka","Goran Ivanisevic","Mats Wilander"
            };
        }
                
        ~PlayersProgression(){
            delete[] players;
        }

        //Qualifiers
        //----------------------------------------------
        //Assign Players for the stack and Queue for the Qualifiers
        void assignPlayersQualifiers(){
            for(int i=0; i <playersNum; i++){
                Qqueue.enqueue(players[i]);
            }
        }
        //Conducting the draw
        void HandleQualifiers(){
            int iterations=(Qqueue.capacity()/2);
            ofstream file("csv/TennisTournament.csv");
            if(!file){
                cout << "file not there";
            } 
            string MatchID;
            file << "Match ID, Player 1, Player 2, Match Status\n";
            
            for (int i=0; i<iterations;i++){
                string player1=Qqueue.dequeue();
                string player2=Qqueue.dequeue();
                MatchID="QUL_"+to_string(i+1);
                cout << "Match "<<i+1<<": "<< endl;
                cout<<player1 << " VS " <<player2 <<endl;
                file << MatchID <<", " << player1 << ", " <<player2<<", Unconfirmed\n";
            }
        }
        
        //Checking if Score have been updated
        //---------------------------
        //Semi Final, Round Robin, Final Match Flag
        bool ScoreUpdatedKnockouts(string match, int length){
            ifstream file("csv/TennisTournament.csv");
    
            if(!file){
                cout  << "File cannot be openend";
                return false;
            }
            string line;
            getline(file, line);
            int counts=0;
            while(getline(file, line)){
                istringstream ss(line);
                string matchID, p1, p2, status, s1, s2, winner;

                // Read all fields (expecting 7 columns)
                if (!(getline(ss, matchID, ',') &&
                      getline(ss, p1, ',') &&
                      getline(ss, p2, ',') &&
                      getline(ss, status, ',') &&
                      getline(ss, s1, ',') &&
                      getline(ss, s2, ',') &&
                      getline(ss, winner))) {
                    continue; // Skip malformed lines
                }
        
                // Trim winner field
                winner = winner.substr(winner.find_first_not_of(' '));
        
                // Count only if matchID is ROU and winner is not empty
                if (matchID.find(match) == 0 && !winner.empty()) {
                    counts++;
                }
            }
            return counts==length;
        }

        //Round Robin
        //-----------------------------------------------------
        //Helper temporary to assign the players into the stack
        void assignPlayersRounds(){
            ifstream file("csv/TennisTournament.csv");

            if(!file){
                cout  << "File cannot be openend";
                return;
            }
            string line;
            getline(file, line);
            string tennisPlayer;
            string matchID;
            string field;
            bool matchExists=false;
            while(getline(file, line)){
                istringstream ss(line);
                ss >> matchID;
                if(matchID.find("QUL_")!=0){
                    return;
                    //if there is any other match exist -> for example round robin matches in case
                }
                while(getline(ss, field, ',')){
                    tennisPlayer=field.substr(field.find_first_not_of(' '));
                }
                roundRobin.enqueue(tennisPlayer);
                //to check if the correct qualifier match exist
                matchExists=true;
            }
            if(!matchExists){
                cout << "Match Doesn't exist " <<endl;
            }
        }
        
        void handleRounds(){
            ofstream file("csv/TennisTournament.csv", ios::app);
            roundRobin.RoundRobinMatches(file);        
        }
        
        bool Checking(int value,int lower, int higher){
            return value<=higher && value >=lower;
        }
        
        // KnockOuts
        //---------------------------------
        //Semi-Final
        //----------------------------------
        void Semi_Final(){
            ifstream file("csv/TennisTournament.csv");

            if(!file){
                cout  << "File cannot be openend";
                return;
            }
            string line;
            getline(file, line);
            string tennisPlayer;
            string matchID;
            string field;
            bool matchExists=false;
            while(getline(file, line)){
                istringstream ss(line);
                ss >> matchID;
                if(matchID.find("QUL_")==0 || matchID.find("ROU")!=0){
                    continue;
                    //if there is any other match exist -> for example round robin matches in case
                }
                while(getline(ss, field, ',')){
                    tennisPlayer=field.substr(field.find_first_not_of(' '));
                }
                Kstack.push(tennisPlayer);
                //to check if the correct Round Robin match exist
                matchExists=true;
            }
            if(!matchExists){
                cout << "Match Doesn't exist " <<endl;
            }
        }
        // Counting the winning times for the round robin stage
        // Sorting the data in descending order
        //Picking the highest 4 and add them to the queue then
        //------------------------
        void SelectTopFour(){
            string playerArray[28];
            int wins[28];
            int playerCount=0;
            while(!Kstack.isEmpty()){
                string name=Kstack.pop();
                bool Plfound=false;          
                for(int i=0; i <playerCount; i++){
                    if(playerArray[i]==name){
                        wins[i]++;
                        Plfound=true;
                        break;
                    }
                }
                if(!Plfound && playerCount <28){
                    playerArray[playerCount]=name;
                    wins[playerCount]=1;
                    playerCount++;
                }
            }
            //Insertion Sort to sort the players with their wins in descending order
            for(int i=1; i<playerCount; i++){
                string playerName=playerArray[i];
                int freq=wins[i];
                int j=i-1;

                while(j >= 0 && wins[j]<freq){
                    wins[j+1]=wins[j];
                    playerArray[j+1]=playerArray[j];
                    j--;
                }
                playerArray[j+1]=playerName;
                wins[j+1]=freq;
            }

            //Now copy the element back to stack
            for(int i=0; i<4; i++){
                Kstack.push(playerArray[i]);
            }

        }
        //Pop the elements and draw the match
        
        void HandleKnockOuts(){
            int iterations=(Kstack.capacity()/2);
            ofstream file("csv/TennisTournament.csv", ios::app);
            if(!file){
                cout << "file not there";
            } 
            string MatchID;
            cout << "Semi Final"<<endl;
            cout << string(80,'=') <<left<<endl;
            for (int i=0; i<iterations;i++){
                string player1=Kstack.pop();
                string player2=Kstack.pop();
                MatchID="Semi_"+to_string(i+1);
                cout << "Match "<<i+1<<": "<< endl;
                cout<<player1 << " VS " <<player2 <<endl;
                file << MatchID <<", " << player1 << ", " <<player2<<", Unconfirmed\n";
            }
        }


        //Final 
        //---------------------------------
        void Final(){
            ifstream file("csv/TennisTournament.csv");

            if(!file){
                cout  << "File cannot be openend";
                return;
            }
            string line;
            getline(file, line);
            string tennisPlayer;
            string matchID;
            string field;
            bool matchExists=false;
            while(getline(file, line)){
                istringstream ss(line);
                ss >> matchID;
                if(matchID.find("QUL_")==0 || matchID.find("ROU_")==0 || matchID.find("Semi_")!=0){
                    continue;
                    //if there is any other match exist -> for example round robin matches in case
                }
                while(getline(ss, field, ',')){
                    tennisPlayer=field.substr(field.find_first_not_of(' '));
                }
                Kstack.push(tennisPlayer);
                //to check if the correct Semi match exist
                matchExists=true;
            }
            if(!matchExists){
                cout << "Match Doesn't exist " <<endl;
            }
            
        }
        //Poping the two players for final game
        void HandleFinal(){
            ofstream file("csv/TennisTournament.csv", ios::app);
            if(!file){
                cout << "file not there";
            } 
            string MatchID;
            string player1=Kstack.pop();
            string player2=Kstack.pop();
            MatchID="FINAL";
            cout << "Final"<<endl;
            cout <<string(80, '=') <<left <<endl;
            cout<<player1 << " VS " <<player2 <<endl;
            file << MatchID <<", " << player1 << ", " <<player2<<", Unconfirmed\n";
        }
        //-----------------------------------------
        
        //Player Tracking
        //--------------------
        void TrackingPlayers(string PlayerName){
            ifstream file("csv/TennisTournament.csv");
    
            if(!file){
                cout  << "File cannot be openend";
                return;
            }
            string line;
            getline(file, line);//Skip the header
            int RoundRobinW=0;
            bool IsQuali=false, IsRounded=false, ReachedSemi=false, ReachedFinal=false;
            cout << "Track Player's Preformance"<<endl;
            cout << string(80,'=') <<left<<endl;
            while(getline(file, line)){
                istringstream ss(line);
                string matchID, player1, player2, sc1, sc2, winner, status;
                getline(ss, matchID, ',');
                getline(ss, player1, ',');
                getline(ss, player2, ',');
                getline(ss, status, ',');
                getline(ss, sc1, ',');
                getline(ss, sc2, ',');
                getline(ss, winner);
                
                player1 = (player1.find_first_not_of(' ') != string::npos) ? player1.substr(player1.find_first_not_of(' ')) : "";
                player2 = (player2.find_first_not_of(' ') != string::npos) ? player2.substr(player2.find_first_not_of(' ')) : "";
                winner  = (winner.find_first_not_of(' ')  != string::npos) ? winner.substr(winner.find_first_not_of(' '))  : "";
                

                if(matchID.find("QUL_")==0 && (player1==PlayerName || player2==PlayerName)){
                    cout << "Qualification Stage:"<<endl;
                    cout << "Qualifiers Oponent: " << ((player1==PlayerName)?player2:player1) << endl;
                    cout << "Score: " << sc1 << " - "<<sc2 << endl;
                    cout << "Result: " << ((winner==PlayerName)? "Win":"Loss") <<endl;
                    cout <<string(80, '-') <<left<<endl;
                    IsQuali=true;
                }
                else if(matchID.find("ROU")==0 && (player1==PlayerName || player2==PlayerName)){
                    if(winner==PlayerName){
                        RoundRobinW++;
                    }
                    IsRounded=true;
                }
                else if(matchID.find("Semi_")==0 && (player1==PlayerName || player2==PlayerName)){
                    cout << "Semi Final Stage: "<<endl;
                    cout << "Semi Final Oponent: " << ((player1==PlayerName)?player2:player1) << endl;
                    cout << "Score: " << sc1 << " - "<<sc2 << endl;
                    cout << "Result: " << ((winner==PlayerName)? "Win":"Loss") << endl;
                    cout <<string(80, '-') <<left<<endl;
                    ReachedSemi=true;
                }
                else if(matchID.find("FINAL")==0 && (player1==PlayerName || player2==PlayerName)){
                    cout << "Final Stage: "<<endl;
                    ReachedFinal=true;
                    cout << "Final Oponent: " << ((player1==PlayerName)?player2:player1) << endl;
                    cout << "Score: " << sc1 << " - "<<sc2 << endl;
                    cout << "Result: " << ((winner==PlayerName)? "Win":"Loss") << endl;
                    cout <<string(80, '-') <<left<<endl;
                }
            }
            if(IsRounded){
                cout << "Round Robin Stage"<<endl;;
                cout << "Number of RoudRobin Wins: " << RoundRobinW << endl;
                cout <<string(80, '-') <<left<<endl;
            }
            if(!IsQuali && !IsRounded && !ReachedSemi && !ReachedFinal){
                cout <<"No matches for the player: " << PlayerName <<endl;
            }
            cout <<string(80, '=')<<left <<endl;

        }
        //------------------------------------------------------------------------------------
        
        
        //Knockouts Menu
        void knockoutsMenu(){
            int choice;
            do{
                cout << "1. Semi Final. "<<endl;
                cout << "2. Final" << endl;
                cout << "3. Back"<<endl;
                cout << string(80, '=') << left <<endl;
                while(!(cin >> choice) || !Checking(choice, 1,3)){
                    cout << "Invalid Input please your choice again correctly: ";
                    cin.clear();
                    cin.ignore(numeric_limits <streamsize> ::max(),'\n');
                }

                switch (choice)
                {
                case 1:
                    if(!ScoreUpdatedKnockouts("ROU", 28)){
                        cout << "The match hasn't started yet.. wait for the score to be revealed"<<endl;
                        continue;
                    }else if (ScoreUpdatedKnockouts("ROU", 28) && !Kdrawn){
                        Semi_Final();
                        SelectTopFour();
                    }
                    else if(Kdrawn){
                        cout << " The Action has already been executed"<<endl;
                        continue;
                    }
                    else if(Kstack.isEmpty()){
                        cout << "RoundRobin matches hasn't been played or drew"<<endl <<
                        "Please come back later after the completion of Round Robin games" << endl;
                        continue;
                    }
                    HandleKnockOuts();
                    Kdrawn=true;
                    break;
                case 2:
                    if(!ScoreUpdatedKnockouts("Semi_", 2)){
                        cout << "The match hasn't started yet.. wait for the score to be revealed"<<endl;
                        continue;
                    }else if (ScoreUpdatedKnockouts("Semi_", 2) && !Kfinal){
                        Final();
                    }
                    else if(Kfinal){
                        cout << " The Action has already been executed"<<endl;
                        continue;
                    }
                    else if(Kstack.isEmpty()){
                        cout << "Semi Final matches hasn't been played or drew"<<endl <<
                        "Please come back later after the completion of Semi Final games" << endl;
                        continue;
                    }
                    HandleFinal();
                    Kfinal=true;
                    break;

                case 3: 
                    break;  

                default:
                    cout << "Incorrect input" <<endl;
                    break;
                }
            }while(choice!=3);
        }
        
        //Manu
        //---------------------
        void clearScreen() {
            cout << "\033[2J\033[H";
        }


        

        void Menu(){
            assignPlayersQualifiers();
            int choice;
            do{
                string PlayerTrackingN;
                cout << string(80, '=')<<left << endl;
                cout << "TOURNAMENT SCHEDULING AND PLAYER PROGRESSION" << endl;
                cout << string(80, '=') << left << endl;
                cout << " 1. Qualifiers Draw. " <<endl;
                cout << " 2. Round Robin Draw. " << endl;
                cout << " 3. KnockOuts Draw. " <<endl;
                cout << " 4. Track Players Progression." << endl;
                cout << " 5. clear output." << endl;
                cout << " 6. return to the actual Menu." << endl;
                cout << string(80, '=') << left<< endl;
                cout << "Chioce: ";
                while(!(cin >> choice) || !(Checking(choice, 1, 6))){
                    cout << "Invalid Input please your choice again correctly: ";
                    cin.clear();
                    cin.ignore(numeric_limits <streamsize> ::max(),'\n');
                }
                    switch(choice){
                        case 1:
                            if(Qdrawn){
                                cout << " The Action has already been executed"<<endl;
                                continue;
                            }
                            else if(Qqueue.isEmpty()){
                                cout << "The Queue still empty there is an issue" << endl;
                                continue;
                            }

                            else{
                                HandleQualifiers();
                                Qdrawn=true;
                                continue;
                            }
                            break;
                        
                        case 2:
                            if(!ScoreUpdatedKnockouts("QUL_", 8)){
                                cout << "The match hasn't started yet.. wait for the score to be revealed"<<endl;
                                continue;
                             }else if(ScoreUpdatedKnockouts("QUL_", 8) && !Rdrawn){
                                assignPlayersRounds();
                            }
                            else if(Rdrawn){
                                cout << " The Action has already been executed"<<endl;
                                continue;
                            }
                            else if(roundRobin.isEmpty()){
                                cout << "Qualifiers matches hasn't been played or drew"<<endl <<
                                "Please come back later after the completion of qualifiers games" << endl;
                                continue;
                            }
                            
                            handleRounds();
                            Rdrawn=true;
                            break;
                        case 3:
                            knockoutsMenu();
                            break;
                        case 4:
                            cout << "Enter the Name of the player that you want to track the performance for: "<<endl;
                            cin.ignore(numeric_limits<streamsize> ::max(), '\n'); //skip reading the input buffer and clear it
                            getline(cin,PlayerTrackingN);
                            TrackingPlayers(PlayerTrackingN);
                            break;
                        case 5: 
                            clearScreen();
                            break;        
                        case 6:
                            break;
                        
                        default:
                            cout << "Indefined action "<<endl;
                            break;
                    }
            } while(choice!=6);
        }

        
};
#endif