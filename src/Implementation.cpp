#include <iostream>
#include <limits>
#include <fstream>
#include "Qualifiers.hpp"
#include "Knockout.hpp"
#include "RoundRobin.hpp"
using namespace std;
class PlayersProgression
{   
    private:
        string* players;
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
        //Helper Functions
        //----------------------------
        // string* data(){
        //     return players;
        // }

        // int playersNum(){
        //     return playersNum;
        // }

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
        
        //Round Robin
        //-----------------------------------------------------
        //Helper temporary to assign the players into the stack
        void assignPlayersRounds(){
            for(int i=0; i <playersNum/2; i++){
                roundRobin.enqueue(players[i]);
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
            for(int i=0; i <playersNum/4; i++){
                Kstack.push(players[i]);
            }
        }
        
        
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
            for(int i=0; i <playersNum/8; i++){
                Kstack.push(players[i]);
            }
        }

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
        
        
        //Player Tracking
        


        //Manu
        void clearScreen() {
            cout << "\033[2J\033[H";
        }

        void Menu(){
            assignPlayersQualifiers();
            int choice;
            bool Qdrawn=false;
            bool Rdrawn=false;
            bool Kdrawn=false;
            bool Kfinal=false;
            do{

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
                                assignPlayersRounds();
                                Qdrawn=true;
                            }
                            continue;
                        
                        case 2:
                            if(Rdrawn){
                                cout << " The Action has already been executed"<<endl;
                                continue;
                            }
                            else if(roundRobin.isEmpty()){
                                cout << "Qualifiers matches hasn't been played or drew"<<endl <<
                                "Please come back later after the completion of qualifiers games" << endl;
                                continue;
                            }
                            else{
                                handleRounds();
                                Semi_Final();
                                Rdrawn=true;
                                continue;
                            }
                        
                        case 3:
                            if(Kdrawn && Kfinal){
                                cout << " The Action has already been executed"<<endl;
                                continue;
                            }
                            else if(Kstack.isEmpty() && (!Kdrawn && !Kfinal)){
                                cout << "RoundRobin matches hasn't been played or drew"<<endl <<
                                "Please come back later after the completion of Round Robin games" << endl;
                                continue;
                            }
                            else if(Kstack.isEmpty() && (Kdrawn && !Kfinal)){
                                cout << "Semi Final matches hasn't been played or drew"<<endl <<
                                "Please come back later after the completion of Semi Final games" << endl;
                                continue;
                            }
                            else if(!Kfinal && !Kdrawn){
                                HandleKnockOuts();
                                Final();
                                Kdrawn=true;
                                continue;
                            }
                            else{
                                HandleFinal();
                                Kfinal=true;
                                continue;
                            }
                       
                        case 4:
                            cout << "Player Tracking"<<endl;
                            continue;

                        case 5: 
                            clearScreen();
                            continue;
                        default:
                            cout << "Indefined action "<<endl;
                            break;
                    }
            } while(choice!=6);
        }

        
};
int main(){
    PlayersProgression Pl;
    Pl.Menu();
    // Pl.assignPlayersQualifiers();
    // Pl.HandleQualifiers();
    // Pl.assignPlayersRounds();
    // Pl.handleRounds();
 
}
  