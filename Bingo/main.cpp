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
    
    Ball *chooseFrom(char s) { // s is either 'B', 'I', 'N', 'G', or 'O'
        // returns a ball beginning with the letter from s
        Ball *chosenBall = new Ball("");
        set<Ball *>::iterator it;
        int count;
        count = 0;
        int rint;
        rint = randomNumber(0, 9);
        for (it = balls.begin(); it != balls.end(); it++) {
            chosenBall = *it;
            if (chosenBall->label[0] == s) { // if the ball we chose starts with the correct letter
                if (count == rint) { // if we have gone a random number of times
                    Ball *b = new Ball(chosenBall->label);
                    balls.erase(chosenBall);
                    chosenBalls.insert(b);
                    return b;
                }
                count++; // otherwise
            }
        }
        return new Ball("BAD");
    }
    
    Ball *choose() {
        // calling this returns a ball chosen at random from the barrel
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
    Marker *squares[5][5];
    bool won;
    
    Card () {
        // the card object constructor sets the random labels
        // by using the chooseFrom function
        Barrel *barrel = new Barrel();
        char ss[5] = {'B','I','N','G','O'};
        won = false;
        for (int col = 0; col < 5; col++) {
            for (int row = 0; row < 5; row++) {
                squares[col][row]=new Marker();
                if ((row == 2) && (col == 2)) {
                    Ball *b = new Ball("FRE");
                    squares[col][row]->covered = true;
                    squares[col][row]->covering = b->label;
                } else {
                    Ball *b = new Ball("");
                    b=barrel->chooseFrom(ss[col]);
                    squares[col][row]->covered = false;
                    squares[col][row]->covering = b->label;
                }
            }
        }
    }
    
    void show() {
        // displays the card on stdout with covered squares in brackets []
        for (int row=0; row<5; row++) {
            for (int col=0; col<5; col++) {
                if (squares[col][row]->covered) {
                    cout << "[" << squares [col][row]->covering << "]\t";
                } else {
                    cout << " " << squares[col][row]->covering << " \t";
                }
            }
            cout << endl;
        }
    }
    void checkFor(Ball *b) {
        // checks for a square with the same label as the ball and covers the square if it is the same
        for (int col=0; col<5;col++) {
            for (int row=0; row < 5; row++) {
                if (b->label == squares[col][row]->covering) {
                    squares[col][row]->covered = true;
                }
            }
        }
    }
    
    bool Bingo() {
        // calling this returns true if we have Bingo
        for (int row = 0 ; row < 5; row++) {
            if (RowCovered(row)) {
                return true;
            }
        }
        for (int col = 0 ; col < 5; col++) {
            if (ColCovered(col)) {
                return true;
            }
        }
        if (Diag1() || Diag2()) {
            return true;
        }
        return false;
    }
    
    bool Diag1() {
        // returns true if we have the diagonal from upper left
        for (int i = 0; i < 5; i++) {
            if (!squares[i][i]->covered) {
                return false;
            }
        }
        return true;
    }
    bool Diag2() {
        // returns true if we have the diagonal from the lower left
        for (int j = 0; j < 5; j++) {
            if (!squares[j][4-j]->covered) {
                return false;
            }
        }
        return true;
    }
    
    bool ColCovered(int col) {
        // returns true if any of the columns are covered
        for (int row = 0; row < 5; row++) {
            if (! squares[col][row]->covered) {
                return false;
            }
        }
        return true;
    }
    
    bool RowCovered(int row) {
        // returns true if any of the rows are covered
        for (int col = 0; col < 5; col++) {
            if (!squares[col][row]->covered) {
                return false;
            }
        }
        return true;
    }
};


int main(int argc, const char * argv[]) {
    Barrel *barrel = new Barrel();
    Ball *b;
    Card *card = new Card();
    int nBalls = 0;
    while (!card->Bingo() && (barrel->balls.size()>0)) {
        nBalls++;
        b = barrel->choose();
        card->checkFor(b);
        cout << b->label << endl;
    }
    if (card->Bingo()) {
        cout << "Bingo!" << endl;
        card->show();
        cout << "it took " << nBalls << " balls." << endl;
    }
    return 0;
}
