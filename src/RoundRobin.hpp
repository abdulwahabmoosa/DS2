#include <iostream>
#include <string>
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

        void rotation(ofstream& file, int round){
             //example just
           int count=0;
           for(int i=0; i<round; i++){
                string player=dequeue();
                int iterations=round-1;
                for(int j=0; j <iterations ; j++){
                    //size here is the size of the queue that gets decremented after dequeuing
                    string matchID="ROU"+to_string(count);
                    string player2=dequeue();
                    file << matchID << ", " <<player<<", "<< player2 << ", Unconfirmed\n";
                    enqueue(player2);
                    count++;
                } 
                iterations--;
            }
            //exploiting the dynamic nature of circular queues to make the games
        }
};



    // // Rotate the queue (shift all elements except the first)
    // void rotate() {
    //     if (size <= 1) return; // No rotation needed for single player

    //     // Store the first player
    //     string first = dequeue();

    //     // Rotate the remaining players
    //     enqueue(dequeue());

    //     // Re-add the first player to the front
    //     front = (front - 1 + capacity) % capacity;
    //     players[front] = first;
    //     size++;
    // }

    // // Display the current queue
    // void display() {
    //     if (isEmpty()) {
    //         cout << "Queue is empty!" << endl;
    //         return;
    //     }
    //     int index = front;
    //     for (int i = 0; i < size; i++) {
    //         cout << "[" << players[index] << "] ";
    //         index = (index + 1) % capacity;
    //     }
    //     cout << endl;
    // }