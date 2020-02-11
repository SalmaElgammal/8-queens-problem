#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include "State.h"
#include "Queen.h"
using namespace std ::chrono;

State get_best_using_hill_climbing(State init);
State get_best_using_k_beams(int k);
State get_best_using_genetic();
State get_best_using_backtracking();
State generate_random();
void display_state(State s);


int main()
{
    Queen temp;
    temp.x=0;
    vector<Queen> v;
    for(int i=0;i<8;i++){
        temp.y=i;
        v.push_back(temp);
    }
    State init(v);
    init.calculate_h();
    auto start = high_resolution_clock::now();
    //State best=get_best_using_hill_climbing(init);
    //State best=get_best_using_k_beams(1);
    //State best=get_best_using_genetic();
    State best=get_best_using_backtracking();

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Running time: "<<duration.count() << " microseconds"<< endl;
    display_state(best);
    return 0;
}

/*function to display the final state*/
void display_state(State s){
    vector<Queen> v=s.board;
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            if(v.at(j).x==i){
                cout<<"Q ";
            }else{
                cout<<"# ";
            }
        }
        cout<<endl;
    }
}

/*function called by get_best_using_hill_climbing,
to get the best successor among 56 successors which has least number of conflicting queens*/
State get_best_successsor(State init){
    State temp,best;
    temp=init;
    best=init;
    int x,y;
    for(int i=0;i<8;i++){
        y=temp.board.at(i).y;

        //try to move the queen in all rows of that column and find best state with best h
        for(int j=0;j<y;j++){
              temp.board.at(i).move_up();
              temp.calculate_h();
              if(temp.h<best.h){
                best=temp;
              }
        }
        for(int j=y;j<8;j++){
            temp.board.at(i).move_down();
            temp.calculate_h();
            if(temp.h<best.h){
                best=temp;
            }
        }
        temp=init;
    }
    return best;
}

/*function that solves the N queens problem using hill climbing algorithm*/
State get_best_using_hill_climbing(State init){
    State last_best=init;
    State best=init;
    int i=0;
    while(1){
        i++;
        best=get_best_successsor(last_best);
        if(best.h==0){
            cout<<"The cost to the final board: "<<i<<endl;
            return best;
        }else if(best.h==last_best.h){
            //random restart
            int rand_y = rand() % (8);
            int rand_x=rand()%(8);
            last_best.board.at(rand_y).x=rand_x;

        }else{
            last_best=best;
        }

    }

}

/*function that generates random state,
that is called by get_best_using_k_beams and get_best_using_genetic*/
State generate_random(){
    State s;
    vector<Queen> v;
    Queen q;
    for(int i=0;i<8;i++){
        q.y=i;
        q.x=rand()%(8);
        v.push_back(q);
    }
    s.board=v;
    s.calculate_h();
    return s;
}

/*function that solves the N queens problem using k-beams algorithm*/
State get_best_using_k_beams(int k){
    State best;
    State init[k];
    State best_successors[k];
    int i=0;
    //generate k-random inital states
    for(int i=0;i<k;i++){
        init[i]=generate_random();
    }

    //each iteration, get the best k-successors
    while(1){
        i++;
        for(int i=0;i<k;i++){
            best_successors[i]=get_best_successsor(init[i]);
            if(best_successors[i].h==0){
                cout<<"The cost to the final board: "<<i<<endl;
                return best_successors[i];
            }else if(best_successors[i].h==init[i].h){
            //random restart
                int rand_y = rand() % (8);
                int rand_x=rand()%(8);
                init[i].board.at(rand_y).x=rand_x;

            }else{
                init[i]=best_successors[i];
            }
        }

    }
}



/*function to swap two states, used in sorting*/
void swap(State* a, State* b)
{
    State t = *a;
    *a = *b;
    *b = t;
}

/*used in sorting*/
int partition (State arr[], int low, int high)
{
    int pivot = arr[high].h; // pivot
    int i = (low - 1); // Index of smaller element

    for (int j = low; j <= high - 1; j++)
    {
        // If current element is smaller than the pivot
        if (arr[j].h < pivot)
        {
            i++; // increment index of smaller element
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

/*function used to sort the states according to its h*/
void quickSort(State arr[], int low, int high)
{
    if (low < high)
    {
        /* pi is partitioning index, arr[p] is now
        at right place */
        int pi = partition(arr, low, high);

        // Separately sort elements before
        // partition and after partition
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

/*function that solves the N queens problem using genetic algorithm*/
State get_best_using_genetic(){
    State init[150];

    //generate 150 random initial states
    for(int i=0;i<150;i++){
        init[i]=generate_random();
    }
    int i=0;
    while(1){
        //sort states
        quickSort(init,0,149);
        i++;
        //choose two parents randomly from first 10 states

        int p1_ind = rand() % (10);
        int p2_ind=rand()%(10);

        State p1=init[p1_ind];
        State p2=init[p2_ind];

        //create cross over point
        int cross_over = rand() % (8);
        State child_1;
        State child_2;

        for(int i=0;i<cross_over;i++){
            child_1.board.push_back(p1.board[i]);
            child_2.board.push_back(p2.board[i]);
        }
        for(int i=cross_over;i<8;i++){
            child_1.board.push_back(p2.board[i]);
            child_2.board.push_back(p1.board[i]);

        }
        child_1.calculate_h();
        child_2.calculate_h();

        if(child_1.h==0){
            cout<<"The cost to the final board: "<<i<<endl;
            return child_1;
        }else if(child_2.h==0){
            cout<<"The cost to the final board: "<<i<<endl;
            return child_2;
        }else{
            init[p1_ind]=child_1;
            init[p2_ind]=child_2;
        }
    }

}

/*function used to check if we can place a queen in board[row][col] without conflicting other queens*/
bool isSafe(int N,vector<Queen> *board, int row, int col)
{
    int i, j;
    /* Check this row on left side */
    for (i = 0; i < col; i++)
        if (board->at(i).x==row)
            return false;

    /* Check upper diagonal on left side */
    for (i = row-1, j = col-1; i >= 0 && j >= 0; i--, j--){
        if (board->at(j).x==i && board->at(j).y==j){
            return false;
        }
    }
    /* Check lower diagonal on left side */
    for (i = row+1, j = col-1; j >= 0 && i < N; i++, j--){
        if (board->at(j).x==i && board->at(j).y==j)
            return false;
    }
    return true;
}

/* A recursive utility function to solve N
   Queen problem */
bool solveNQUtil(int N,vector<Queen> *board, int col)
{
    /* base case: If all queens are placed
      then return true */
    if (col >= N)
        return true;

    /* Consider this column and try placing
       this queen in all rows one by one */
    for (int i = 0; i < N; i++) {
        /* Check if the queen can be placed on
          board[i][col] */
        if (isSafe(N,board, i, col)) {
            /* Place this queen in board[i][col] */
            Queen q;
            q.x=i;
            q.y=col;
            board->push_back(q);

            /* recur to place rest of the queens */
            if (solveNQUtil(N,board, col + 1))
                return true;

            /* If placing queen in board[i][col]
               doesn't lead to a solution, then
               remove queen from board[i][col] */
            board->pop_back(); // BACKTRACK
        }
    }

    /* If the queen cannot be placed in any row in
        this colum col  then return false */
    return false;
}

/*function that solves the N queens problem using backtracking algorithm*/
State get_best_using_backtracking(){
    State best;
    vector<Queen> best_board;
    Queen q;

    if(solveNQUtil(8,&best_board,0)){
        best.board=best_board;
        best.calculate_h();

    }
    return best;
}


