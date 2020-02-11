#include <vector>
#include <chrono>
#include "Queen.h"
#ifndef STATE_H
#define STATE_H
using namespace std::chrono;

class State
{
    public:
        vector<Queen> board;
        int h;

        State(){
        }
        State(vector<Queen> init_board){
            board=init_board;
        }


        void calculate_h(){
            h=0;
            Queen q1,q2;
            for(int i=0;i<8;i++){
                q1=board.at(i);
                for(int j=i+1;j<8;j++){
                    q2=board.at(j);
                    if(q1.x==q2.x || (q1.x-q2.x)==(q1.y-q2.y) || (q1.x-q2.x)==(q2.y-q1.y)){
                        h++;

                    }
                }
            }

        }



    protected:

    private:
};

#endif // STATE_H
