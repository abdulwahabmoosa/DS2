#ifndef KNOCKOUT_HPP
#define KNOCKOUT_HPP
#include <iostream>
#include <string>
#include <fstream>
#include <limits>
using namespace std;

struct Qualified_Players
{
    string playerName;
    Qualified_Players * next;
};
    
    class Qualifiers_Queue
    {
        private:
        Qualified_Players *front;
        Qualified_Players *rear;
        int size;
        
        public:
        Qualifiers_Queue(){
            front=nullptr;
            rear=nullptr;
            size=0;
        }
        
        bool isEmpty(){
            return front==nullptr;
        }
        
        int capacity(){
            return size;
        }
        void enqueue(string PlName){
            Qualified_Players * newNode=new Qualified_Players();
            newNode ->playerName=PlName;
            newNode ->next=nullptr;
            if(front==nullptr){
                front=rear=newNode;
            }
            else{
                rear -> next=newNode;
                rear=newNode;
            }
            size++;
        }
        
        string dequeue(){
            if(isEmpty()){
                return "";
            }
            Qualified_Players *temp=front;
            front =front -> next;
            if(front==nullptr){
                rear=nullptr;
            }
            string player=temp ->playerName;
            delete temp;
            size--;
            return player;
        }

        void displayQueue(){
            Qualified_Players * temp=front;
            while(temp!=nullptr){
                cout << temp ->playerName << endl;
                temp=temp -> next;
            }
        }
        
        ~Qualifiers_Queue(){
            cout << "Players Queue has been successfully deleted "<<endl;
        }
    };
    



#endif