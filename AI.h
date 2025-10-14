//
// Created by Sam Gurarie on 6/28/24.
//

#ifndef CTEMPLATE_AI_H
#define CTEMPLATE_AI_H
#include "ChessMove.h"


class AI {
public:
    vector<ChessMove*>* computerSearch();
    vector<ChessMove*>* playerSearch();
    vector<ChessMove*>* addFromList(vector<pair<pair<int, int>, pair<int, int>>> &array);
    void lookAhead(ll depth, ChessMoveTree &tree);
    void makeMove(ChessMove &move);
    void undoMove(ChessMove &move);
    ll findEvaluation();
    ll maxDepth = 1;
    RootChessMoveTree currentTree;
    King *playerKing, *computerKing;

    void assignPieceList() {
        if(playerWhite) {
            playerKing = &whiteKing;
            computerKing = &blackKing;
        } else {
            playerKing = &blackKing;
            computerKing = &whiteKing;
        }
    }

private:
    RootChessMoveTree topLevelTree;
};


#endif //CTEMPLATE_AI_H
