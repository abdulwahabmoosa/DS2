#include <iostream>
#include <string>
#include <fstream>
using namespace std;

struct RoundRobin{
    string PlayerName;
};

class CircularQueue
{
    private:
        RoundRobin * player;
        int front;
        int rear;
        int size=0;
        int capacity;
        bool Empty;
        public:
        CircularQueue(int PlayerNum){
            player=new RoundRobin[PlayerNum];
            size=0;
            front=rear=-1;
            Empty=false;
            capacity=PlayerNum;
        }
 
        ~CircularQueue(){
            cout << "Circular Queue has been deleted" <<endl;
        }

        
        bool isEmpty(){
            return front==-1;
        }

        bool isFull() {
            return size == capacity;
        }

        int PlayerNumber(){
            return size;
        }

        void enqueue(string PlName)
        {
            if(isFull()){
                cout << "queue is full" << endl;
                return;
            }

            if(isEmpty())front=0;
            rear=(rear+1) % capacity;
            player[rear].PlayerName=PlName;
            size++;
        }

        string dequeue(){
            if(isEmpty()){
                cout << "queue is already empty"<<endl;
                return "";
            }
            string Pl=player[front].PlayerName;
            if(front==rear)front=rear=-1;
            else{
                front=(front+1) % capacity;
            }
            size--;
            return Pl;
        }

        //Rotate the queue (shift all elements except the first)
        void rotate() {
            // exploiting the dynamic nature of circular queues to make the games
           if(PlayerNumber() <=1)return;
           string lastPlayer=player[rear].PlayerName;
           // 
           for(int i=rear; i>front+1; i--){
            player[i].PlayerName=player[i-1].PlayerName;
           }

           player[front+1].PlayerName=lastPlayer;

        }

        //Two Pointer approach
        void RoundRobinMatches(ofstream& file) {
            int rounds = PlayerNumber();
            int count = 0;
        
            for (int i = 0; i < rounds - 1; i++) { // N-1 rounds for N players
                int tempfront = front; // Start pointer (front)
                int temprear = rear; // End pointer (rear)
                cout << "Round "<<i+1 <<endl;
                cout << string(80,'=')<<left<<endl;
                for (int j = 0; j < rounds / 2; j++) { 
                    if (tempfront < 0 || temprear < 0 || tempfront >= capacity || temprear >= capacity) {
                        cerr << "Error: Invalid player index!" << endl;
                        continue;
                    }
                    // Dequeue players from both ends
                    string player1 = player[tempfront].PlayerName;
                    string player2 = player[temprear].PlayerName;
                    
                    if (player1.empty() || player2.empty()) {
                        cerr << "Error: Empty player name detected!" << endl;
                        continue;
                    }        
                    // Create a match ID
                    string matchID = "ROU" + to_string(count+1)+to_string(j+1);
                    cout << "Match " <<j+1 << ": " << endl;
                    cout << player1 << " VS " << player2 <<endl;
                    file << matchID << ", " << player1 << ", " << player2 << ", Unconfirmed\n";
                    // Move pointers inward
                    tempfront = (tempfront + 1) % capacity;
                    temprear = (temprear - 1 + capacity) %capacity;
                }
                count++;
                cout << string(80,'=')<<left<<endl;
                rotate();
            }
            finishRounding();
        }

        
        // Dequeue everything once done
        void finishRounding() {
            while(!isEmpty()){
                dequeue();
            }
        }
    };
    


