#ifndef KNOCKOUT_HPP
#define KNOCKOUT_HPP
#include <iostream>
#include <string>
#include <limits>
using namespace std;

struct WinningPlayers
{
    string playerName;
    WinningPlayers * next;
};

class Knockouts_Stack
{
    private:
        WinningPlayers* head; 
        int size=0;
    public:
        Knockouts_Stack(){
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
            WinningPlayers * newNode= new WinningPlayers;
            newNode ->playerName=PlName;
            newNode -> next=head;
            head=newNode;
            size++;
        }
        
        string pop(){
            WinningPlayers * temp=head;
            
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
        
        string top(){
            if(head==nullptr)return "Stack is empty";
            return head ->playerName;
        }
        
        ~Knockouts_Stack(){
            cout << "Players Stacks is deleted.." <<endl;
        }

        int capacity(){
            return size;
        }
    
        
    };
    
#endif