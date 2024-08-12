
/*
this was a cellular automata/BFS solution to
the "Princess in the Castle" puzzle
https://www.youtube.com/watch?v=mnZzXUYuj_E
(mainly an excuse to learn c++).

after fiddling with the problem on paper, i noticed
that the "uncertain" rooms acted as objects
that needed to be 100% removed from the list
before a solution could be valid.

the general idea of this solution was to model
uncertainty as a simple 2d cellular automata
that spread to adjacent squares and left its 
original space (unless another room spread there).

after, a BFS was done on all possible options
for rooms to check with a transposition
table.

in the end, given n rooms, the princess is 
guaranteed to be found in 2(n-2) nights.

TODO: a* search, but need an admissible heuristic
maybe number of uncertain rooms +1?
because stuff like the solution for n=3 
eliminates 2 rooms in one move
*/


#include <iostream>
#include <bitset>
#include <cmath>
#include <queue>
#include <unordered_map>
#include <string> 
#include <stack>

#define ROOMCNT 16

void printRooms(int roomCnt, size_t roomList){
    if (roomCnt<=0) return;
    
    std::cout<<"\n-";
    for (size_t i = 0; i < roomCnt; i++) std::cout << "--";
    std::cout<<"\n|";
    for (size_t i = 0; i < roomCnt; i++) {
        //should just use a bitset, but i wanted to learn about bitmasking
        int bitResult = roomList & (1<<(roomCnt-1-i));
        char printResult = (bitResult==0) ? '#':' ';
        std::cout << printResult <<"|";
    }
    std::cout<<"\n-";
    for (size_t i = 0; i < roomCnt; i++) std::cout << "--";
    std::cout<<"\n";
}


void printDecision(int roomCnt,int cnt){    
    std::cout<<" ";
    for (int i = roomCnt-1; i > cnt; i--) std::cout << ". ";
    std::cout << "X ";
    for (int i = cnt-1; i >=0; i--) std::cout << ". ";
}



int main() {
    long start =  0;
    long goal = pow(2,ROOMCNT)-1;

    /*
    states are modelled as a sequence of n bits
    stored as longs.
    - 0 means princess could be in there
    - 1 means princess could not possibly be there
    the goal is wbould be to get to all 1's
    meaning that all uncertainty has been eliminated
    */

    int totalMoves = 0;

    std::queue<long> q;
    std::unordered_map<long, long> hm;
    std::unordered_map<long, int> hmMoves;

    hm.insert({start,start});
    hmMoves.insert({start,0});

    q.push(start);

    while (!q.empty())
    {
        long cur = q.front();
        q.pop();

        if(cur==goal) break;

        std::bitset curBit = std::bitset<ROOMCNT>(cur);
        std::bitset newBit = std::bitset<ROOMCNT>(goal);        


        for (int i = 0; i < ROOMCNT; i++)
        {            
            if(!curBit.test(i)){
                if(i-1>=0) newBit.set(i-1,false);
                if(i+1<ROOMCNT) newBit.set(i+1,false);
            }
        }
        
        for (int i = 0; i < ROOMCNT; i++)
        {
            std::bitset chosenBit = std::bitset<ROOMCNT>(newBit);  
            chosenBit.set(i,true);

            long chosenNum = chosenBit.to_ulong();

            //need cpp 20
            if(hm.contains(chosenNum)) continue;

            //std::cout << "Added to queue and transposition table: "+chosenBit.to_string()+ '\n';  
            hm.insert({chosenNum,cur});
            hmMoves.insert({chosenNum,i});
            q.push(chosenNum);
        }
    }

    std::stack<long> s, sMoves;
    long lastStop = goal;

    while (lastStop!=start)
    {
        long next = hm.at(lastStop);
        s.push(lastStop);
        sMoves.push(hmMoves.at(lastStop));
        lastStop=next;
        totalMoves++;
    }
    std::cout << "It will take "+std::to_string(totalMoves)+" move(s) to guarantee the princess is found in a castle with "+std::to_string(ROOMCNT)+" room(s).\n";
    
    printRooms(ROOMCNT,start);
    while (!s.empty())
    {
        printDecision(ROOMCNT,sMoves.top());
        printRooms(ROOMCNT,s.top());
        s.pop();
        sMoves.pop();
    }
    
    return 0;
}