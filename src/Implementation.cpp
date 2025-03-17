#include <iostream>
#include <limits>
#include <fstream>
#include "Qualifiers.hpp"
using namespace std;
class PlayersProgression
{
    public:
        Qualifiers_Stack Qstack;
        Qualifiers_Queue Qqueue;
        
        PlayersProgression(){
        }

        void assignPlayers(){
            string players [16]= {"Roger Federer","Rafael Nadal","Novak Djokovic","Pete Sampras","Bjorn Borg","Rod Laver",
            "Andre Agassi","Jimmy Connors","John McEnroe","Ivan Lendl","Stefan Edberg",
            "Boris Becker","Andy Murray","Stan Wawrinka","Goran Ivanisevic","Mats Wilander"
            };
            int PlayerNum=sizeof(players)/sizeof(players[0]);
            for(int i=0; i <PlayerNum; i++){
                Qstack.push(players[i]);
                Qqueue.enqueue(players[i]);
            }
        }

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
                string player2=Qqueue.dequeue();
                cout << "Game " << (i + 1) << ": " << player1 << " vs " << player2 << endl;
                MatchID="QUL_"+to_string(i+1);
                file << MatchID <<", " << player1 << ", " <<player2<<", Unconfirmed\n";
            }
        }
        
        ~PlayersProgression(){
            cout << "Operations finished.... destructor called" <<endl;
        }
};
int main(){
    PlayersProgression Pl;
    Pl.assignPlayers();
    Pl.HandleQualifiers();
}