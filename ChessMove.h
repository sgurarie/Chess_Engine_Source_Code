//
// Created by Sam Gurarie on 4/5/24.
//

#ifndef CTEMPLATE_CHESSMOVE_H
#define CTEMPLATE_CHESSMOVE_H

#include "ChessPiece.h"



class ChessMove {

public:
    bool operator==(const ChessMove &o) const;

public:
    ChessPiece* m1;
    ChessPiece* m2;
    ChessPiece* captured;
    bool castled = false;

    ~ChessMove() {
        delete m1;
        delete m2;
        delete captured;
    }

    ChessMove* deepCopy() {
        ChessMove* newMove = new ChessMove();
        newMove->m1 = m1->deepCopy();
        newMove->m2 = m2->deepCopy();
        if(newMove->captured != nullptr) newMove->captured = captured->deepCopy();
        newMove->castled = castled;
        return newMove;
    }
};

class ChessMoveTree {
public:
    vector<pair<ChessMove*, ChessMoveTree*>> tree;
    ll evaluation;

    void deleteChildren() {
        for(ll i = 0; i < tree.size(); i++) {
            tree[i].second->deleteChildren();
            delete tree[i].second;
            delete tree[i].first;
        }
    }

    ChessMoveTree* addMove(ChessMove *o, ll eval) {
        ChessMoveTree* newTree = new ChessMoveTree;
        tree.push_back({o, newTree});
        newTree->evaluation = eval;
        return tree[tree.size() - 1].second;
    }

    ChessMoveTree* traverse(ChessMove *o) {

        ll index = 0;
        for(ll i = 0; i < tree.size(); i++) {
            if(*tree[i].first == *o) {
                index = i;
                currentEvaluation = evaluation;
                continue;
            }

            tree[i].second->deleteChildren();
            delete tree[i].second;
        }

        return tree[index].second;
    }
};

class RootChessMoveTree {



public:
    ChessMoveTree *tree;
    RootChessMoveTree() {
        tree = new ChessMoveTree();
    }

    void clearMemory() {
        delete tree;
        tree = new ChessMoveTree();
    }

    void traverse(ChessMove *o) {
        ChessMoveTree *newTree = tree->traverse(o);
        delete tree;
        tree = newTree;
    }

    ChessMove* createMoveFromChords(int x1, int y1, int x2, int y2, ChessPiece::PieceType promotion) {

        ChessPiece* newPiece;
        if(promotion != ChessPiece::PieceType::NONE) {
            if(promotion == ChessPiece::PieceType::QUEEN) {
                Queen *newPieceIn = new Queen;
                newPieceIn->setup(x2, y2, playerWhite, false, false);
                newPiece = newPieceIn;
            } else if(promotion == ChessPiece::PieceType::ROOK) {
                Rook *newPieceIn = new Rook;
                newPieceIn->setup(x2, y2, playerWhite, false, false);
                newPiece = newPieceIn;
            } else if(promotion == ChessPiece::PieceType::BISHOP) {
                Bishop *newPieceIn = new Bishop;
                newPieceIn->setup(x2, y2, playerWhite, false, false);
                newPiece = newPieceIn;
            } else {
                Knight *newPieceIn = new Knight;
                newPieceIn->setup(x2, y2, playerWhite, false, false);
                newPiece = newPieceIn;
            }
        } else {
            newPiece = board[y1][x1]->deepCopy();
        }

        newPiece->y = y2;
        newPiece->x = x2;
        ChessMove *newMove = new ChessMove();
        newMove->m1 = board[y1][x1]->deepCopy();
        newMove->m2 = newPiece;

        if(board[y2][x2] == nullptr) newMove->captured = nullptr;
        else newMove->captured = board[y2][x2]->deepCopy();
        newMove->m2->firstMove = false;
        return newMove;
    }

    bool playerMove(ChessMove &newMove) {

        for(int i = 0; i < tree->tree.size(); i++) {
            if(*(tree->tree[i].first) == (newMove)) {
                traverse(tree->tree[i].first);
                return true;
            }
        }

        return false;
    }

    ChessMove *computerMove() {
        ll maxEval, index;
        bool first = true;
        for(int i = 0; i < tree->tree.size(); i++) {
            if(first || tree->tree[i].second->evaluation > maxEval) {
                first = false;
                maxEval = tree->tree[i].second->evaluation;
                index = i;
            }
        }

        ChessMove* result = tree->tree[index].first; //error chess pieces went out of scope
        result->m1 = result->m1->deepCopy();
        result->m2 = result->m2->deepCopy();
        if(result->captured != nullptr)
            result->captured = result->captured->deepCopy();
        traverse(tree->tree[index].first);
        return result;
    }

    bool noNextMoves() const {
        return tree->tree.empty();
    }

    ~RootChessMoveTree() {
        tree->deleteChildren();
        delete tree;
    }
};



#endif //CTEMPLATE_CHESSMOVE_H