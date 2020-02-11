#ifndef QUEEN_H
#define QUEEN_H

using namespace std;
class Queen
{
    public:
        Queen(){

        }
        int x,y;
        Queen(int init_x,int init_y){
            x=init_x;
            y=init_y;
        }
        void move_up(){
            if(x>0){
                x--;
            }
        }
        void move_down(){
            if(x<7){
                x++;
            }
        }

    protected:

    private:
};

#endif // QUEEN_H
