#include <iostream>
#include <limits>
#include <fstream>
#include "Qualifiers.hpp"
#include "RoundRobin.hpp"
using namespace std;
class PlayersProgression
{   
    private:
        string* players;
        int playersNum;
    public:
        Qualifiers_Stack Qstack;
        // Qualifiers_Queue Qqueue;
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
                Qstack.push(players[i]);
            }
        }
        //Conducting the draw
        void HandleQualifiers(){
            int iterations=(Qstack.capacity()/2);
            ofstream file("csv/TennisTournament.csv");
            if(!file){
                cout << "file not there";
            } 
            string MatchID;
            file << "Match ID, Player 1, Player 2, Match Status\n";
            
            for (int i=0; i<iterations;i++){
                string player1=Qstack.pop();
                string player2=Qstack.pop();
                MatchID="QUL_"+to_string(i+1);
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
        
};
int main(){
    PlayersProgression Pl;
    Pl.assignPlayersQualifiers();
    Pl.HandleQualifiers();
    Pl.assignPlayersRounds();
    Pl.handleRounds();

}
  