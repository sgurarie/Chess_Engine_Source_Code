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

    short x, y;
    bool isWhite;
    void generatePossibleMoves();
    char printFileTypeChar();
    char32_t character;
    enum class PieceType {ROOK, KNIGHT, BISHOP, PAWN, QUEEN, KING} pieceType;
    void setup1(ll x, ll y, bool isWhite) {
        this->x = x;
        this->y = y;
        this->isWhite = isWhite;
    }
};

extern ChessPiece *board[8][8];
extern bool playerWhite;
extern vector<ChessPiece> playerPieces, computerPieces;
extern short pawnMoves[2] = {1, -1};
extern short endPoint[2] = {8, 0};
extern short knightMoves[8][2] = {{2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};
extern short bishopMoves[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}}, rookMoves[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

class Rook : public ChessPiece {
public:
    void generatePossibleMoves();
    void setup(ll x, ll y, bool isWhite) {
        setup1(x, y, isWhite);
        if(isWhite) {
            character = u'♖';
        } else {
            character = u'♜';
        }
        pieceType = PieceType::ROOK;
    }
    char printFileTypeChar() {
        return 'r';
    };
};

class Knight : public ChessPiece {
public:
    void generatePossibleMoves();
    void setup(ll x, ll y, bool isWhite) {
        setup1(x, y, isWhite);
        if(isWhite) {
            character = u'♘';
        } else {
            character = u'♞';
        }
        pieceType = PieceType::KNIGHT;
    }
    char printFileTypeChar() {
        return 'n';
    };
};

class Bishop : public ChessPiece {
public:
    void generatePossibleMoves();
    void setup(ll x, ll y, bool isWhite) {
        setup1(x, y, isWhite);
        if(isWhite) {
            character = u'♗';
        } else {
            character = u'♝';
        }
        pieceType = PieceType::BISHOP;
    }
    char printFileTypeChar() {
        return 'b';
    };
};

class Queen : public ChessPiece {
public:
    void generatePossibleMoves();
    void setup(ll x, ll y, bool isWhite) {
        setup1(x, y, isWhite);
        if(isWhite) {
            character = u'♕';
        } else {
            character = u'♛';
        }
        pieceType = PieceType::QUEEN;
    }
    char printFileTypeChar() {
        return 'q';
    };
};

class King : public ChessPiece {
public:
    void generatePossibleMoves();
    void setup(ll x, ll y, bool isWhite) {
        setup1(x, y, isWhite);
        if(isWhite) {
            character = u'♔';
        } else {
            character = u'♚';
        }
        pieceType = PieceType::KING;
    }
    char printFileTypeChar() {
        return 'k';
    };
};

class Pawn : public ChessPiece {
public:
    void generatePossibleMoves();
    bool firstMove = true;
    bool advancedTwo = false;
    void setup(ll x, ll y, bool isWhite) {
        setup1(x, y, isWhite);
        if(isWhite) {
            character = u'♙';
        } else {
            character = u'♟';
        }
        pieceType = PieceType::PAWN;
    }
    char printFileTypeChar() {
        return 'p';
    };
};


#endif //CTEMPLATE_CHESSPIECE_H
