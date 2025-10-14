//
// Created by Sam Gurarie on 6/28/24.
//

#include "AI.h"
#include<cmath>


vector<ChessMove*>* AI::computerSearch() {

    vector<ChessMove*>* returnValue = new vector<ChessMove*>;
    for(auto it = computerPieces.begin(); it != computerPieces.end(); it++) {
        vector<pair<pair<int, int>, pair<int, int>>> result = (*it)->generatePossibleMoves();

        vector<ChessMove*>* adding = addFromList(result);
        for(int i = 0; i < adding->size(); i++) {
            returnValue->push_back((*adding)[i]);
        }
        delete adding;
    }

    return returnValue;
}

vector<ChessMove*>* AI::playerSearch() {

    vector<ChessMove*>* returnValue = new vector<ChessMove*>;
    for(auto it = playerPieces.begin(); it != playerPieces.end(); it++) {
        vector<pair<pair<int, int>, pair<int, int>>> result = (*it)->generatePossibleMoves();
        vector<ChessMove*>* adding = addFromList(result);
        for(int i = 0; i < adding->size(); i++) {
            returnValue->push_back((*adding)[i]);
        }
        delete adding;
    }

    return returnValue;
}

vector<ChessMove*>* AI::addFromList(vector<pair<pair<int, int>, pair<int, int>>> &array) {
    auto adding = new vector<ChessMove*>;
    for(int i = 0; i < array.size(); i++) {
        ChessMove *newMove = new ChessMove;
        newMove->m1 = board[array[i].first.second][array[i].first.first]->deepCopy();
        newMove->m2 = newMove->m1->deepCopy();
        if(board[array[i].second.second][array[i].second.first] != nullptr)
            newMove->captured = board[array[i].second.second][array[i].second.first]->deepCopy();
        newMove->m2->x = array[i].second.first;
        newMove->m2->y = array[i].second.second;
        newMove->m2->firstMove = false;
        newMove->m2->advancedTwo = (newMove->m1->pieceType == ChessPiece::PieceType::PAWN && abs(newMove->m2->y - newMove->m1->y) == 2);
        if(newMove->m1->pieceType == ChessPiece::PieceType::KING && abs(array[i].first.first - array[i].second.first) > 1) {
            newMove->castled = true;
        }


        if(newMove->m2->pieceType == ChessPiece::PieceType::PAWN
           && newMove->m2->y == endPoint[newMove->m2->isWhite ? 0 : 1]) {
            Queen* queen = new Queen;
            queen->setup(newMove->m2->x, newMove->m2->y, newMove->m2->isWhite, false, false);
            delete newMove->m2;
            newMove->m2 = (ChessPiece*) queen;
            adding->push_back(newMove);

            newMove = newMove->deepCopy();
            Rook* rook = new Rook;
            rook->setup(newMove->m2->x, newMove->m2->y, newMove->m2->isWhite, false, false);
            newMove->m2 = (ChessPiece*) rook;
            adding->push_back(newMove);

            newMove = newMove->deepCopy();
            Knight* knight = new Knight;
            knight->setup(newMove->m2->x, newMove->m2->y, newMove->m2->isWhite, false, false);
            newMove->m2 = (ChessPiece*) knight;
            adding->push_back(newMove);

            newMove = newMove->deepCopy();
            Bishop* bishop = new Bishop;
            bishop->setup(newMove->m2->x, newMove->m2->y, newMove->m2->isWhite, false, false);
            newMove->m2 = (ChessPiece*) bishop;
            adding->push_back(newMove);
        } else {
            adding->push_back(newMove);
        }
    }

    return adding;
}

void AI::lookAhead(ll depth, ChessMoveTree &tree) {

    if(depth >= maxDepth) return;
    vector<ChessMove*>* possibleMoves;
    if(playerTurn) {
        possibleMoves = playerSearch();
    } else {
        possibleMoves = computerSearch();
    }

    bool currentTurn = playerTurn;
    for(int i = 0; i < possibleMoves->size(); i++) {
        makeMove(*(*possibleMoves)[i]);
        if ((!currentTurn || !playerKing->underCheck(&computerPieces)) &&
            (currentTurn || !computerKing->underCheck(&playerPieces))) { //if statement fucked up
            lookAhead(depth + 1, *tree.addMove((*possibleMoves)[i], findEvaluation()));
        }
        undoMove(*(*possibleMoves)[i]);

        if(playerPieces.size() > 16) {
            cout << "Uh oh player";
        }

        if(computerPieces.size() > 16) {
            cout << "Uh oh computer";
        }
    }

}

ll AI::findEvaluation() {

    ll score = 0;
    for(auto it = computerPieces.begin(); it != computerPieces.end(); it++) {
        score += pieceValueDebug[(ll) (*it)->pieceType];
        score += min(min((ll) (*it)->x, 7ll - ((ll) (*it)->x)), min((ll) (*it)->y, 7ll - ((ll) (*it)->y)));
    }

    for(auto it = playerPieces.begin(); it != playerPieces.end(); it++) {
        score -= pieceValueDebug[(ll) (*it)->pieceType];
        score -= min(min((ll) (*it)->x, 7ll - ((ll) (*it)->x)), min((ll) (*it)->y, 7ll - ((ll) (*it)->y)));
    }

    return score;
}


#include "ChessMove.h"

void AI::makeMove(ChessMove &move) {

    playerTurn = move.m1->isWhite ^ playerWhite;
    if(board[move.m1->y][move.m1->x] != nullptr) delete board[move.m1->y][move.m1->x];
    board[move.m1->y][move.m1->x] = nullptr;
    board[move.m2->y][move.m2->x] = move.m2->deepCopy();

    if(move.m2->pieceType == ChessPiece::PieceType::KING) {
        if(move.m2->isWhite) whiteKing = *(King*) move.m2;
        else blackKing = *(King*) move.m2;

        if(move.castled) {
            ChessMove moveRook;
            moveRook.m1 = board[endPoint[move.m1->isWhite ? 1 : 0]][move.m2->x < 4 ? 0 : 7]->deepCopy();
            moveRook.m2 = moveRook.m1->deepCopy();
            moveRook.m2->x = move.m2->x < 4 ? 3 : 5;
            moveRook.captured = nullptr;
            makeMove(moveRook);
        }
    }

    if(move.captured != nullptr) {
        if (move.captured->isWhite == playerWhite) {
            playerPieces.erase(move.captured);
        } else {
            computerPieces.erase(move.captured);
        }
    }

    if(move.m1->isWhite == playerWhite) {
        playerPieces.erase(move.m1);
        playerPieces.insert(move.m2->deepCopy());
    } else {
        computerPieces.erase(move.m1);
        computerPieces.insert(move.m2->deepCopy());
    }

    if(move.castled) {

        if(move.m2->x == 2) {
            ChessPiece *rook = board[move.m2->y][0]->deepCopy();

            if(rook->isWhite == playerWhite) {
                playerPieces.erase(rook);
                rook->x = 2;
                playerPieces.insert(rook);
            } else {
                computerPieces.erase(rook);
                rook->x = 2;
                computerPieces.insert(rook);
            }

            board[move.m2->y][0] = nullptr;
            board[move.m2->y][rook->x] = rook;
        } else {
            ChessPiece *rook = board[move.m2->y][7]->deepCopy();

            if(rook->isWhite == playerWhite) {
                playerPieces.erase(rook);
                rook->x = 5;
                playerPieces.insert(rook);
            } else {
                computerPieces.erase(rook);
                rook->x = 5;
                computerPieces.insert(rook);
            }

            board[move.m2->y][7] = nullptr;
            board[move.m2->y][rook->x] = rook;
        }
    }
}

void AI::undoMove(ChessMove &move) {

    if(board[move.m2->y][move.m2->x] != nullptr) delete board[move.m2->y][move.m2->x];
    board[move.m2->y][move.m2->x] = nullptr;
    board[move.m1->y][move.m1->x] = move.m1->deepCopy();

    playerTurn = !(move.m1->isWhite ^ playerWhite);
    if(move.captured != nullptr) {
        board[move.captured->y][move.captured->x] = move.captured;
        if(move.captured->isWhite == playerWhite) {
            playerPieces.insert(move.captured->deepCopy());
        } else {
            computerPieces.insert(move.captured->deepCopy());
        }
    }

    if(move.m1->pieceType == ChessPiece::PieceType::KING) {
        if(move.m1->isWhite) whiteKing = *(King*) move.m1;
        else blackKing = *(King*) move.m1;

        if(move.castled) {
            ChessMove moveRook;
            moveRook.m2 = board[endPoint[move.m1->isWhite ? 1 : 0]][move.m1->x < 4 ? 3 : 5]->deepCopy();
            moveRook.m1 = moveRook.m2->deepCopy();
            moveRook.m1->x = move.m1->x < 4 ? 0 : 7;
            moveRook.captured = nullptr;
            undoMove(moveRook);
        }
    }

    if(move.m1->isWhite == playerWhite) {
        playerPieces.erase(move.m2);
        playerPieces.insert(move.m1->deepCopy());
    } else {
        computerPieces.erase(move.m2);
        computerPieces.insert(move.m1->deepCopy());
    }

    if(move.castled) {

        if(move.m2->x == 2) {
            ChessPiece *rook = board[move.m2->y][2]->deepCopy();
            if(rook->isWhite == playerWhite) {
                playerPieces.erase(rook);
                rook->x = 0;
                playerPieces.insert(rook->deepCopy());
            } else {
                computerPieces.erase(rook);
                rook->x = 0;
                computerPieces.insert(rook->deepCopy());
            }
            board[move.m2->y][2] = nullptr;
            board[move.m2->y][rook->x] = rook;
        } else {
            ChessPiece *rook = board[move.m2->y][5]->deepCopy();
            if(rook->isWhite == playerWhite) {
                playerPieces.erase(rook);
                rook->x = 7;
                playerPieces.insert(rook->deepCopy());
            } else {
                computerPieces.erase(rook);
                rook->x = 7;
                computerPieces.insert(rook->deepCopy());
            }
            board[move.m2->y][5] = nullptr;
            board[move.m2->y][rook->x] = rook;
        }
    }
}