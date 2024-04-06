//
// Created by Sam Gurarie on 4/5/24.
//

#ifndef CTEMPLATE_CHESSMOVE_H
#define CTEMPLATE_CHESSMOVE_H

#include "ChessPiece.h"


class ChessMove {

public:
    ChessPiece m1, m2;
    ChessPiece captured;
    ll evaluation;
    bool castled = false;
    void makeMove();
    void undoMove();
    void findEvaluation();
};

class ChessMoveTree {

public:
    vector<pair<ChessMove, ChessMoveTree>*> children;

    ChessMoveTree* advance(short i) {

        for(ll j = 0; j < children.size(); j++) {
            if(j == i) {
                continue;
            }
            delete children[i];
        }

        return &children[i]->second;
    }

    short findMaximum() {

        ll maximum;
        short index;
        bool first = true;
        for(ll j = 0; j < children.size(); j++) {
            ll eval = children[j]->first.evaluation;
            if(first || eval > maximum) {
                first = false;
                maximum = eval;
                index = j;
            }
        }

        return index;
    }

    short playerMove(short x1, short y1, short x2, short y2) {
        for(ll j = 0; j < children.size(); j++) {
            ChessMove &move = children[j]->first;
            if(move.m1.x == x1 && move.m1.y == y1 && move.m2.x == x2 && move.m2.y == y2) {
                return j;
            }
        }

        return -1;
    }
};

class RootChessMoveTree {

    ChessMoveTree *root;
    void advance(short i) {
        ChessMoveTree* newRoot = root->advance(i);
        delete root;
        root = newRoot;
    }

    void findMaximum() {
        advance(root->findMaximum());
    }

    bool playerMove(short x1, short y1, short x2, short y2) {
        short result = root->playerMove(x1, y1, x2, y2);
        if(result == -1) return false;
        advance(result);
        return true;
    }
};



#endif //CTEMPLATE_CHESSMOVE_H