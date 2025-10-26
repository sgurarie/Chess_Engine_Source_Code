//
// Created by Sam Gurarie on 10/19/25.
//

#ifndef CTEMPLATE_MOVEGENERATOR_H
#define CTEMPLATE_MOVEGENERATOR_H

#include <fstream>
#include <sstream>
#include <mach/mach.h>
#include "Bitboard.h"

using namespace std;
typedef long long ll;
typedef unsigned long long ull;

enum class PieceType {ROOK, KNIGHT, BISHOP, QUEEN, PAWN, KING, NONE};

struct BitboardMove {
    short x1, y1, x2, y2;
    ull startBoard, endBoard;
    PieceType piece;
    bool isWhite;
    ll currentLevel;
    ll nextMoveLevel;
    ll evaluation;

    bool operator==(const BitboardMove &other) const {
        return x1 == other.x1
        && x2 == other.x2
        && y1 == other.y1
        && y2 == other.y2
        && piece == other.piece
        && isWhite == other.isWhite;
    }
};

//size right now: 34 bytes, future size: 21 bytes
//Keep the data as part of the struct with a fixed size array or
// continue using vectors to store pointers to the next move
//if fixed size array must account for upper bound: 14 * 2 (2 rooks) + 14 * 2 (2 bishops) + 14 * 2 (1 queen)
// + 8 * 2 (2 knights) + 8 (king) + 4 * 8 (8 pawns) = 140; (NOT INCLUDING PROMOTIONS or CASTLING w/ rises to 302) 140 * 34 bytes = 4480 bytes or 140 * 21 = 2940 bytes

struct BasicChessPiece {
    short x, y;
    PieceType piece;
    bool isWhite;

    bool operator<(const BasicChessPiece &o) const {
        if(x == o.x) {
            if(y == o.y) {
                return isWhite < o.isWhite;
            }
            return y < o.y;
        }
        return x < o.x;
    }
};

struct ChessPieceHolder {
    BasicChessPiece chessPieces[2][6][10];
    ull limitCap[2][6];
};

struct BoardState {

    bool playerWhite; //arrays of size 2; first index black, second index white
    ull boardStates[2][6];
    ull totalBoard[2];
    ChessPieceHolder holder;

    bool advancedTwoPawnMove[2][8]; //en passant

    bool firstRookKingMove[2];

    ull pinnedPieces[2];
    ull attackPattern[2];
    ull generalAttackPattern[2]; //pieces don't block rook/bishop/queen movement to find pinned pieces
};

void printOut(ofstream &writer, BasicChessPiece &i);

class Arena {

private:
    vector<BitboardMove>* moveStorage;
    ll size = 1e6;
    ll startIndex = 0;
public:
    Arena();
    void addMove(BitboardMove &move);
    ll nextFreeIndex() const;
    ll currentFreeIndex() const;
    bool advanceMove(BitboardMove &move);
    BitboardMove& getMove(ll index);
    ll getCurrentEvaluation();
    BitboardMove findOptimalMove();
    bool hasNoMove();
};

struct ReturnValueDoMove {
    BasicChessPiece captured;
    bool wasFirstMove, wasAdvancedTwo;
    ull pinnedPieces;
    ull attackPattern;
    ull generalAttackPattern;
    PieceType piece;
};

class MoveGenerator {

private:
    BoardState board;
    Arena arena;
    BishopBitboard bishop;
    RookBitboard rook;
    KnightBitboard knight;
    KingBitboard king;
    WhitePawnBitboard whitePawn;
    BlackPawnBitboard blackPawn;
    QueenBitboard queen;
public:
    MoveGenerator();
    void recursiveMoveFunction(ll depth, bool whiteTurn);
    BitboardMove makeMoveFromChords(int x1, int y1, int x2, int y2, PieceType promotion);
    ReturnValueDoMove doMove(BitboardMove &move, ll i);
    void undoMove(BitboardMove &move, ReturnValueDoMove &returnValue, ll i);
    void printChar(short i, short j);
    void printBoard();
    void loadFile(string fileName);
    void saveFile(string fileName);
    void playGame();

    ll getEvaluation();
};







#endif //CTEMPLATE_MOVEGENERATOR_H
