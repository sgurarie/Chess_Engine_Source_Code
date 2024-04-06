//
// Created by Sam Gurarie on 3/28/24.
//

#ifndef CTEMPLATE_CHESSPIECE_H
#define CTEMPLATE_CHESSPIECE_H
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
using namespace std;
typedef long long ll;
class ChessPiece {
public:

    short x, y;
    bool isWhite;
    void generatePossibleMoves();
    bool attackingOtherPiece(short x, short y);
    char printFileTypeChar();
    bool operator<(const ChessPiece &o) const;
    char32_t character;
    enum class PieceType {ROOK, KNIGHT, BISHOP, PAWN, QUEEN, KING, NONE} pieceType = PieceType::NONE;
    void setup1(short x, short y, bool isWhite) {
        this->x = x;
        this->y = y;
        this->isWhite = isWhite;
    }
};

extern ChessPiece *board[8][8];
extern bool playerWhite;
extern set<ChessPiece> playerPieces, computerPieces;
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
    bool attackingOtherPiece(short x, short y);
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
    bool attackingOtherPiece(short x, short y);
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
    bool attackingOtherPiece(short x, short y);
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
    bool attackingOtherPiece(short x, short y);
};

class King : public ChessPiece {
public:
    void generatePossibleMoves();
    bool underCheck(vector<ChessPiece> &otherPieces);
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
    bool attackingOtherPiece(short x, short y);
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
    bool attackingOtherPiece(short x, short y);
};


#endif //CTEMPLATE_CHESSPIECE_H
