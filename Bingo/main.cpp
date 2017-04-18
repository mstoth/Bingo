//
//  main.cpp
//  Bingo
//
//  Created by Michael Toth on 4/16/17.
//  Copyright © 2017 Michael Toth. All rights reserved.
//

#include <iostream>
#include <string>
#include <stdlib.h>
#include <set>
#include <random>


using namespace std;

class Marker {
public:
    int row,col;
    bool covered;
    string covering;
};

int randomNumber(int low, int high) {
    // from
    // http://stackoverflow.com/questions/5008804/generating-random-integer-from-a-range
    random_device rd;     // only used once to initialise (seed) engine
    mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
    uniform_int_distribution<int> uni(low,high); // guaranteed unbiased
    auto random_integer = uni(rng);
    return random_integer;
    /////////
}



class Ball {
    public: string label;
    Ball(string s) {
        label = s;
    }
};

class Barrel {
public:
    set<Ball *> balls;
    set<Ball *> chosenBalls;

    Barrel() {
        reset();
    }
    
    void reset() {
        int i;
        balls.clear();
        chosenBalls.clear();
        
        for (i=1;i<16;i++) {
            balls.insert(new Ball("B" + to_string(i)));
        }
        for (i=16;i<31;i++) {
            balls.insert(new Ball("I" + to_string(i)));
        }
        for (i=31;i<46;i++) {
            balls.insert(new Ball("N" + to_string(i)));
        }
        for (i=46;i<61;i++) {
            balls.insert(new Ball("G" + to_string(i)));
        }
        for (i=61;i<76;i++) {
            balls.insert(new Ball("O" + to_string(i)));
        }
    }
    Ball *choose() {
        
        Ball *chosenBall = new Ball("");
        set<Ball *>::iterator it;
        int count;
        count = 0;
        int rint;
        rint = randomNumber(0, (uint32_t)balls.size()-1);
        for (it = balls.begin(); it != balls.end(); it++) {
            if (count == rint) {
                chosenBall = *it;
                break;
            }
            count++;
        }
        Ball *b = new Ball(chosenBall->label);
        balls.erase(chosenBall);
        chosenBalls.insert(b);
        return b;
    }
    
};


class Card {
public:
    Marker *squares[5][5]; // a 5x5 matrix
    bool won;
    
    Card (string s) {
        Barrel *barrel = new Barrel();
        string ss[5] = {"B","I","N","G","O"};
        int offset[5] = {0,16,32,48,64};
        
        won = false;
        for (int col = 0; col < 5; col++) {
            for (int row = 0; row < 5; row++) {
                Ball *b = new Ball("");
                b=barrel->choose();
                squares[col][row]->covered = false;
                squares[col][row]->covering = b->label;
                
            }
        }
    }
};


int main(int argc, const char * argv[]) {
    Barrel b;
    Ball *b1;
    return 0;
}
