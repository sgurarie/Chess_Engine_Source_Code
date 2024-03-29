//
// Created by Sam Gurarie on 3/28/24.
//

#ifndef CTEMPLATE_CHESSPIECE_H
#define CTEMPLATE_CHESSPIECE_H
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
typedef long long ll;
class ChessPiece {
public:

    ll x, y;
    bool isWhite;
    void generatePossibleMoves();
    string character;
    void setup1(ll x, ll y, bool isWhite) {
        this->x = x;
        this->y = y;
        this->isWhite = isWhite;
    }
};

extern ChessPiece (*board)[8][8];
extern bool playerWhite;
extern vector<ChessPiece> playerPieces, computerPieces;

class Rook : ChessPiece {
public:
    void generatePossibleMoves();
    void setup(ll x, ll y, bool isWhite) {
        setup1(x, y, isWhite);
        if(isWhite) {
            character = "♖";
        } else {
            character = "♜";
        }
    }
};

class Knight : ChessPiece {
public:
    void generatePossibleMoves();
    void setup(ll x, ll y, bool isWhite) {
        setup1(x, y, isWhite);
        if(isWhite) {
            character = "♘";
        } else {
            character = "♞";
        }
    }
};

class Bishop : ChessPiece {
public:
    void generatePossibleMoves();
    void setup(ll x, ll y, bool isWhite) {
        setup1(x, y, isWhite);
        if(isWhite) {
            character = "♗";
        } else {
            character = "♝";
        }
    }
};

class Queen : ChessPiece {
public:
    void generatePossibleMoves();
    void setup(ll x, ll y, bool isWhite) {
        setup1(x, y, isWhite);
        if(isWhite) {
            character = "♕";
        } else {
            character = "♛";
        }
    }
};

class King : ChessPiece {
public:
    void generatePossibleMoves();
    void setup(ll x, ll y, bool isWhite) {
        setup1(x, y, isWhite);
        if(isWhite) {
            character = "♔";
        } else {
            character = "♚";
        }
    }
};

class Pawn : ChessPiece {
public:
    void generatePossibleMoves();
    void setup(ll x, ll y, bool isWhite) {
        setup1(x, y, isWhite);
        if(isWhite) {
            character = "♙";
        } else {
            character = "♟";
        }
    }
};


#endif //CTEMPLATE_CHESSPIECE_H
