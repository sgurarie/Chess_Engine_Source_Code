//
// Created by Sam Gurarie on 3/28/24.
//

#include "ChessPiece.h"

ChessPiece* board[8][8];
vector<ChessPiece> playerPieces, computerPieces;
bool playerWhite;

bool notInRange(short num) {
    return num < 0 || num > 7;
}

void Rook::generatePossibleMoves() {

    short newX, newY;
    for(ll i = 0; i < 2; i++) {
        newX = this->x;
        while (true) {
            newX += rookMoves[i][0];
            if(notInRange( newX) || (board[newY][this->x] != nullptr && board[newY][this->x]->isWhite == isWhite)) break;
            //check validity of move and evaluate
        }
    }

    for(ll i = 0; i < 2; i++) {
        newY = this->y;
        while (true) {
            newY += rookMoves[i][1];
            if(notInRange( newY) || (board[newY][this->x] != nullptr && board[newY][this->x]->isWhite == isWhite)) break;
            //check validity of move and evaluate
        }
    }
}

void Bishop::generatePossibleMoves() {

    short newX = this->x, newY = this->y;
    for(ll i = 0; i < 4; i++) {
        while (true) {
            newX += bishopMoves[i][0];
            newY += bishopMoves[i][1];
            if(notInRange(newX) || notInRange(newY) || (board[newY][this->x] != nullptr && board[newY][this->x]->isWhite == isWhite)) break;
            //check validity of move and evaluate
        }
        newX = this->x; newY = this->y;
    }
}

void Knight::generatePossibleMoves() {

    short newX = this->x, newY = this->y;
    for(ll i = 0; i < 4; i++) {
        while (true) {
            newX += knightMoves[i][0];
            newY += knightMoves[i][1];
            if(notInRange(newX) || notInRange(newY) || (board[newY][this->x] != nullptr && board[newY][this->x]->isWhite == isWhite)) break;
            //check validity of move and evaluate
        }
        newX = this->x; newY = this->y;
    }
}

void Queen::generatePossibleMoves() {
    short newX, newY;
    for(ll i = 0; i < 2; i++) {
        newX = this->x;
        while (true) {
            newX += rookMoves[i][0];
            if(notInRange( newX) || (board[newY][this->x] != nullptr && board[newY][this->x]->isWhite == isWhite)) break;
            //check validity of move and evaluate
        }
    }

    for(ll i = 0; i < 2; i++) {
        newY = this->y;
        while (true) {
            newY += rookMoves[i][1];
            if(notInRange( newY) || (board[newY][this->x] != nullptr && board[newY][this->x]->isWhite == isWhite)) break;
            //check validity of move and evaluate
        }
    }

    newX = this->x; newY = this->y;
    for(ll i = 0; i < 4; i++) {
        while (true) {
            newX += bishopMoves[i][0];
            newY += bishopMoves[i][1];
            if(notInRange(newX) || notInRange(newY) || (board[newY][this->x] != nullptr && board[newY][this->x]->isWhite == isWhite)) break;
            //check validity of move and evaluate
        }
        newX = this->x; newY = this->y;
    }
}

void King::generatePossibleMoves() {
    short newX, newY;
    for(ll i = 0; i < 2; i++) {
        newX = this->x;
        newX += rookMoves[i][0];
        if(notInRange( newX) || (board[newY][this->x] != nullptr && board[newY][this->x]->isWhite == isWhite)) break;
        //check validity of move and evaluate
    }

    for(ll i = 0; i < 2; i++) {
        newY = this->y;
        newY += rookMoves[i][1];
        if(notInRange( newY) || (board[newY][this->x] != nullptr && board[newY][this->x]->isWhite == isWhite)) break;
        //check validity of move and evaluate
    }

    newX = this->x; newY = this->y;
    for(ll i = 0; i < 4; i++) {
        newX += bishopMoves[i][0];
        newY += bishopMoves[i][1];
        if(notInRange(newX) || notInRange(newY) || (board[newY][this->x] != nullptr && board[newY][this->x]->isWhite == isWhite)) break;
        //check validity of move and evaluate
        newX = this->x; newY = this->y;
    }
}

void Pawn::generatePossibleMoves() {

    short index = this->isWhite ? 0 : 1;
    ll newY = this->y + pawnMoves[index];

    if(board[newY][this->x] == nullptr) {
        if(newY == endPoint[index]) {
            //check validity of move and evaluate
        } else {

            //check validity of move and evaluate
            if(this->firstMove) {
                newY += pawnMoves[index];
                if(board[newY][this->x] == nullptr) {
                    //check validity of move and evaluate
                }
            }

        }
    }

    ll newX = this->x;
    for(ll i = 0; i < 2; i++) {
        newX += pawnMoves[i];
        if(notInRange(newX) || (board[newY][this->x] != nullptr && board[newY][this->x]->isWhite == isWhite)) continue;
        //check validity of move and evaluate
        newX = this->x;
    }

    //En Passant
    if(newY == endPoint[index] - pawnMoves[index] * 3) {
        for(ll i = 0; i < 2; i++) {
            newX = this->x + pawnMoves[i];
            if(notInRange(newX)
            || (board[newY][this->x] != nullptr
            && board[newY][this->x]->isWhite == isWhite)
            || board[newY][this->x]->pieceType != PieceType::PAWN) continue;
            if(!((Pawn) (*board[newY][this->x])).advancedTwo) continue;
            //check validity of move and evaluate
        }
    }

}
