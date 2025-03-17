#ifndef QUALIFIERS_HPP
#define QUALIFIERS_HPP
#include <iostream>
#include <string>
#include <limits>
using namespace std;

struct Qualified_Players
{
    string playerName;
    Qualified_Players * next;
};

class Qualifiers_Stack
{
    private:
        Qualified_Players* head; 
        int size=0;
    public:
        Qualifiers_Stack(){
            head=nullptr; 
            size=0;
        }

        bool isEmpty(){
            if(head==nullptr){
                return true;
            }
            return false;
        }

        void push(string PlName){
            Qualified_Players * newNode= new Qualified_Players;
            newNode ->playerName=PlName;
            newNode -> next=head;
            head=newNode;
            size++;
        }
        
        string pop(){
            Qualified_Players * temp=head;
            
            if(!head){
                cout << "Stack underflow" << endl;
                return "";
            }
            else{
                string player= temp -> playerName;
                head=head ->next;
                delete temp;
                return player;
            }
        }

        void displayStack(){
            Qualified_Players * temp=head;
            while(temp!=nullptr){
                cout << temp ->playerName << endl;
                temp=temp -> next;
            }
        }
        
        string top(){
            if(head==nullptr)return "Stack is empty";
            return head ->playerName;
        }
        
        ~Qualifiers_Stack(){
            while(!isEmpty()){
                pop();
            }
            cout << "Players Stacks is deleted.." <<endl;
        }

        int capacity(){
            return size;
        }
        
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
            while(!isEmpty()){
                dequeue();
            }
            cout << "Players Queue has been successfully deleted "<<endl;
        }
    };
    
    
#endif