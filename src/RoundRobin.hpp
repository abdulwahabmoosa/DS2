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

    // Rotate the queue (shift all elements except the first)
        void rotate() {
            //exploiting the dynamic nature of circular queues to make the games
            if (size <= 1) return;
            
            string last = player[rear].PlayerName;
            
            for(int i=rear; i>front+1 ; i--){
                player[i].PlayerName=player[i-1].PlayerName;
            }
            
            // Re-add the second player to the second position to achieve accurate rounding
            player[front+1].PlayerName=last;
        }


        void RoundRobinMatches(ofstream& file) {
            int rounds = PlayerNumber();
            int count = 0;
        
            for (int i = 0; i < rounds - 1; i++) { // N-1 rounds for N players
                int left = front; // Start pointer (front)
                int right = rear; // End pointer (rear)
                for (int j = 0; j < rounds / 2; j++) { 

                    if (left < 0 || right < 0 || left >= capacity || right >= capacity) {
                        cerr << "Error: Invalid player index!" << endl;
                        continue;
                    }
                    // Dequeue players from both ends
                    string player1 = player[left].PlayerName;
                    string player2 = player[right].PlayerName;
                    
                    if (player1.empty() || player2.empty()) {
                        cerr << "Error: Empty player name detected!" << endl;
                        continue;
                    }        
                    // Create a match ID
                    string matchID = "ROU" + to_string(count+1)+to_string(j+1);
                    file << matchID << ", " << player1 << ", " << player2 << ", Unconfirmed\n";
                    // Move pointers inward
                    left = (left + 1) % capacity;
                    right = (right - 1 + capacity) %capacity;
                }
                count++;
        
                rotate();
            }
        }

        
        // Display the current queue
        void display() {
            if (isEmpty()) {
                cout << "Queue is empty!" << endl;
                return;
            }
            int index = front;
            for (int i = 0; i < size; i++) {
                cout << player[index].PlayerName << " "<<index << endl;
                index = (index + 1) % capacity;
            }
            cout << endl;
        }
    };
    


