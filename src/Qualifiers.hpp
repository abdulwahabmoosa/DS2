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
            cout << "Players Stacks is deleted.." <<endl;
        }

        int capacity(){
            return size;
        }
        
    };
    
#endif