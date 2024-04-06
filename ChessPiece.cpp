//
// Created by Sam Gurarie on 3/28/24.
//

#include "ChessPiece.h"

bool notInRange(short num) {
    return num < 0 || num > 7;
}

bool King::underCheck(vector<ChessPiece> &otherPieces) {

    for(ll i = 0; i < otherPieces.size(); i++) {
        if(otherPieces[i].attackingOtherPiece(this->x, this->y)) {
            return true;
        }
    }

    return false;
}

bool ChessPiece::operator<(const ChessPiece &o) const {

    if(x != o.x) {
        return x < o.x;
    }

    return y <= o.y;
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
    for(ll i = 0; i < 8; i++) {
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

bool Rook::attackingOtherPiece(short x, short y) {

    if(this->x == x) {

        short xDir = this->x - x > 0;
        short newX = this->x + xDir;
        while(newX != x) {
            if(board[y][newX] != nullptr) return false;
            newX += xDir;
        }
        return true;
    }

    if(this->y == y) {

        short yDir = this->y - y > 0;
        short newY = this->y + yDir;
        while(newY != y) {
            if(board[newY][x] != nullptr) return false;
            newY += yDir;
        }
        return true;
    }

    return false;
}

bool Knight::attackingOtherPiece(short x, short y) {

    short diffX = this->x - x, diffY = this->y - y;
    if(abs(diffX) < 1 || abs(diffX) > 2 || abs(diffY) < 1 || abs(diffY) > 2) return false;
    for(ll i = 0; i < 8; i++) {
        if(diffX == knightMoves[i][0] && diffY == knightMoves[i][1]) {
            return true;
        }
    }

    return false;
}

bool Bishop::attackingOtherPiece(short x, short y) {

    if(abs(this->x - x) == abs(this->y - y)) {

        short xDir = this->x - x > 0, yDir = this->y - y > 0;
        short newX = this->x + xDir, newY = this->y + yDir;
        while(newX != x) {
            if(board[newY][newX] != nullptr) return false;
            newY += yDir;
            newX += xDir;
        }
        return true;
    }

    return false;
}

bool Queen::attackingOtherPiece(short x, short y) {

    if(this->x == x) {

        short xDir = this->x - x > 0;
        short newX = this->x + xDir;
        while(newX != x) {
            if(board[y][newX] != nullptr) return false;
            newX += xDir;
        }
        return true;
    }

    if(this->y == y) {

        short yDir = this->y - y > 0;
        short newY = this->y + yDir;
        while(newY != y) {
            if(board[newY][x] != nullptr) return false;
            newY += yDir;
        }
        return true;
    }

    if(abs(this->x - x) == abs(this->y - y)) {

        short xDir = this->x - x > 0, yDir = this->y - y > 0;
        short newX = this->x + xDir, newY = this->y + yDir;
        while(newX != x) {
            if(board[newY][newX] != nullptr) return false;
            newY += yDir;
            newX += xDir;
        }
        return true;
    }

    return false;
}

bool King::attackingOtherPiece(short x, short y) {
    return abs(this->x - x) == 1 || abs(this->y - y) == 1;
}

bool Pawn::attackingOtherPiece(short x, short y) {

    short dir = this->isWhite ? 1 : -1;
    if(abs(this->x - x) == 1) {
        if(this->y + dir == y) {
            return true;
        }
    }

    return false;
}

bool enPassant(short x1, short y1, short x2, short y2, bool advanceSecond) {

    if(!advanceSecond) return false;

    if(abs(x1 - x2) != 1 || y1 != y2) return false;
    return true;
}