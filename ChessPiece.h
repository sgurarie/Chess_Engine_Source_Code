//
// Created by Sam Gurarie on 3/28/24.
//

#ifndef CTEMPLATE_CHESSPIECE_H
#define CTEMPLATE_CHESSPIECE_H
#include <vector>
#include <iostream>
#include <set>

using namespace std;
typedef long long ll;



class ChessPiece {
public:

    int x, y;
    bool isWhite;
    bool firstMove = true;
    bool advancedTwo = false;
    bool notInRange(int num) const;
    virtual vector<pair<pair<int, int>, pair<int, int>>> generatePossibleMoves() const = 0;
    virtual bool attackingOtherPiece(int x, int y) const = 0;

    virtual string printBoard() const = 0;


    virtual ChessPiece* deepCopy() const = 0;

    virtual char printFileTypeChar() const = 0;

    bool operator<(const ChessPiece &o) const;
    bool operator==(const ChessPiece &o) const;
    enum class PieceType {ROOK, KNIGHT, BISHOP, PAWN, QUEEN, KING, NONE} pieceType = PieceType::NONE;
    void setup1(int x, int y, bool isWhite, bool firstMove, bool advancedTwo) {
        this->x = x;
        this->y = y;
        this->isWhite = isWhite;
        this->firstMove = firstMove;
        this->advancedTwo = advancedTwo;
    }
    virtual ~ChessPiece() = default;
};

extern int pieceValue[7];
extern int pieceValueDebug[7];
extern int pawnCloseValue[7];
extern int pawnMoves[2];
extern int endPoint[2];
extern int knightMoves[8][2];
extern int bishopMoves[4][2];
extern int rookMoves[4][2];

class Rook : public ChessPiece {
public:

    vector<pair<pair<int, int>, pair<int, int>>> generatePossibleMoves() const override;
    void setup(ll x, ll y, bool isWhite, bool firstMove, bool advancedTwo) {
        setup1(x, y, isWhite, firstMove, advancedTwo);

        pieceType = PieceType::ROOK;
    }
    char printFileTypeChar() const override {
        return 'r';
    };
    string printBoard() const override {
        if(isWhite) {
            return "♖";
        }
        return "♜";
    }
    bool attackingOtherPiece(int x, int y) const override;

    ChessPiece* deepCopy() const override;
    virtual ~Rook() = default;

};

class Knight : public ChessPiece {
public:
    vector<pair<pair<int, int>, pair<int, int>>> generatePossibleMoves() const override;
    void setup(ll x, ll y, bool isWhite, bool firstMove, bool advancedTwo) {
        setup1(x, y, isWhite, firstMove, advancedTwo);
        pieceType = PieceType::KNIGHT;
    }
    char printFileTypeChar() const override {
        return 'n';
    };
    string printBoard() const override {
        if(isWhite) {
            return "♘";
        }
        return "♞";
    }
    bool attackingOtherPiece(int x, int y) const override;
    ChessPiece* deepCopy() const override;
    virtual ~Knight() = default;
};

class Bishop : public ChessPiece {
public:
    vector<pair<pair<int, int>, pair<int, int>>> generatePossibleMoves() const override;
    void setup(ll x, ll y, bool isWhite, bool firstMove, bool advancedTwo) {
        setup1(x, y, isWhite, firstMove, advancedTwo);
        pieceType = PieceType::BISHOP;
    }
    char printFileTypeChar() const override {
        return 'b';
    };
    string printBoard() const override {
        if(isWhite) {
            return "♗";
        }
        return "♝";
    }
    bool attackingOtherPiece(int x, int y) const override;
    ChessPiece* deepCopy() const override;
    virtual ~Bishop() = default;
};

class Queen : public ChessPiece {
public:
    vector<pair<pair<int, int>, pair<int, int>>> generatePossibleMoves() const override;
    void setup(ll x, ll y, bool isWhite, bool firstMove, bool advancedTwo) {
        setup1(x, y, isWhite, firstMove, advancedTwo);
        pieceType = PieceType::QUEEN;
    }
    char printFileTypeChar() const override {
        return 'q';
    };
    string printBoard() const override {
        if(isWhite) {
            return "♕";
        }
        return "♛";
    }
    bool attackingOtherPiece(int x, int y) const override;
    ChessPiece* deepCopy() const override;
    virtual ~Queen() = default;
};

struct ChessPieceCompare {
    bool operator()(const ChessPiece* o1, const ChessPiece* o2) const {
        // define an ordering (not just equality!)
        // e.g. compare by type, then by position
        if (o1->pieceType != o2->pieceType) return o1->pieceType < o2->pieceType;
        if(o1->x != o2->x) return o1->x < o2->x;
        if(o1->y != o2->y) return o1->y < o2->y;
        if(o1->isWhite != o2->isWhite) return o1->isWhite < o2->isWhite;
        if(o1->advancedTwo != o2->advancedTwo) return o1->advancedTwo < o2->advancedTwo;
        return o1->firstMove < o2->firstMove;
    }
};

class King : public ChessPiece {
public:
    vector<pair<pair<int, int>, pair<int, int>>> generatePossibleMoves() const override;
    bool underCheck(set<ChessPiece*, ChessPieceCompare> *otherPieces) const;
    bool underAttack(ll x, ll y, set<ChessPiece*, ChessPieceCompare> *otherPieces) const;
    void setup(ll x, ll y, bool isWhite, bool firstMove, bool advancedTwo) {
        setup1(x, y, isWhite, firstMove, advancedTwo);
        pieceType = PieceType::KING;
    }
    char printFileTypeChar() const override {
        return 'k';
    };
    string printBoard() const override {
        if(isWhite) {
            return "♔";
        }
        return "♚";
    }
    bool attackingOtherPiece(int x, int y) const override;
    ChessPiece* deepCopy() const override;
    virtual ~King() = default;
};

class Pawn : public ChessPiece {
public:
    vector<pair<pair<int, int>, pair<int, int>>> generatePossibleMoves() const override;
    void setup(ll x, ll y, bool isWhite, bool firstMove, bool advancedTwo) {
        setup1(x, y, isWhite, firstMove, advancedTwo);
        pieceType = PieceType::PAWN;
    }
    char printFileTypeChar() const override {
        return 'p';
    };
    string printBoard() const override {
        if(isWhite) {
            return "♙";
        }
        return "♟";
    }
    ChessPiece* deepCopy() const override;
    bool attackingOtherPiece(int x, int y) const override;
    virtual ~Pawn() = default;
};




extern  ChessPiece *board[8][8];
extern  bool playerWhite, playerTurn;
extern  set<ChessPiece*, ChessPieceCompare> playerPieces, computerPieces;
extern  King whiteKing, blackKing;
extern  ll currentEvaluation;

#endif //CTEMPLATE_CHESSPIECE_H
