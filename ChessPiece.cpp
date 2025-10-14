////
//// Created by Sam Gurarie on 3/28/24.
////
//

#include "ChessPiece.h"



int pieceValue[7] = {563, 305, 333, 100, 950, 0, 0};
int pieceValueDebug[7] = {500, 300, 300, 100, 900, 0, 0};
int pawnCloseValue[7] = {0, 8, 15, 31, 62, 125, 250};
int pawnMoves[2] = {-1, 1};
int endPoint[2] = {0, 7};
int knightMoves[8][2] = {{2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};
int bishopMoves[4][2] = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
int rookMoves[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

ChessPiece *board[8][8];
bool playerWhite, playerTurn;
set<ChessPiece*, ChessPieceCompare> playerPieces, computerPieces;
King whiteKing, blackKing;
ll currentEvaluation;

bool ChessPiece::notInRange(int num) const {
    return num < 0 || num > 7;
}

bool King::underCheck(set<ChessPiece*, ChessPieceCompare> *otherPieces) const {

    return underAttack(x, y, otherPieces);
}

bool ChessPiece::operator<(const ChessPiece &o) const {

    if(x != o.x) {
        return x < o.x;
    }

    return y <= o.y;
}

bool ChessPiece::operator==(const ChessPiece &o) const {
    return x == o.x && y == o.y && isWhite == o.isWhite && o.pieceType == pieceType;
}

bool King::underAttack(ll x, ll y, set<ChessPiece*, ChessPieceCompare> *otherPieces) const {
    for(auto i = otherPieces->begin(); i != otherPieces->end(); i++) {
        if((*i)->attackingOtherPiece(this->x, this->y)) {
            return true;
        }
    }

    return false;
}

ChessPiece* Rook::deepCopy() const {
    ChessPiece* newCopy = new Rook();
    newCopy->x = this->x;
    newCopy->y = this->y;
    newCopy->isWhite = this->isWhite;
    newCopy->advancedTwo = this->advancedTwo;
    newCopy->firstMove = this->firstMove;
    newCopy->pieceType = PieceType::ROOK;
    return newCopy;
}

ChessPiece* Queen::deepCopy() const {
    ChessPiece* newCopy = new Queen();
    newCopy->x = this->x;
    newCopy->y = this->y;
    newCopy->isWhite = this->isWhite;
    newCopy->firstMove = this->firstMove;
    newCopy->advancedTwo = this->advancedTwo;
    newCopy->pieceType = PieceType::QUEEN;
    return newCopy;
}


ChessPiece* Knight::deepCopy() const {
    ChessPiece* newCopy = new Knight();
    newCopy->x = this->x;
    newCopy->y = this->y;
    newCopy->isWhite = this->isWhite;
    newCopy->firstMove = this->firstMove;
    newCopy->advancedTwo = this->advancedTwo;
    newCopy->pieceType = PieceType::KNIGHT;
    return newCopy;
}

ChessPiece* Bishop::deepCopy() const {
    ChessPiece* newCopy = new Bishop();
    newCopy->x = this->x;
    newCopy->y = this->y;
    newCopy->isWhite = this->isWhite;
    newCopy->firstMove = this->firstMove;
    newCopy->advancedTwo = this->advancedTwo;
    newCopy->pieceType = PieceType::BISHOP;
    return newCopy;
}

ChessPiece* King::deepCopy() const {
    ChessPiece* newCopy = new King();
    newCopy->x = this->x;
    newCopy->y = this->y;
    newCopy->isWhite = this->isWhite;
    newCopy->firstMove = this->firstMove;
    newCopy->advancedTwo = this->advancedTwo;
    newCopy->pieceType = PieceType::KING;
    return newCopy;
}

ChessPiece* Pawn::deepCopy() const {
    ChessPiece* newCopy = new Pawn();
    newCopy->x = this->x;
    newCopy->y = this->y;
    newCopy->isWhite = this->isWhite;
    newCopy->firstMove = this->firstMove;
    newCopy->advancedTwo = this->advancedTwo;
    newCopy->pieceType = PieceType::PAWN;
    return newCopy;
}



vector<pair<pair<int, int>, pair<int, int>>> Rook::generatePossibleMoves() const {

    vector<pair<pair<int, int>, pair<int, int>>> returnValue;
    int newX = this->x, newY = this->y;
    for (ll i = 0; i < 4; i++) {
        while (true) {
            newX += rookMoves[i][0];
            newY += rookMoves[i][1];
            if (notInRange(newX) || notInRange(newY) ||
                (board[newY][newX] != nullptr && (board[newY][newX]->isWhite == isWhite || board[newY][newX]->pieceType == PieceType::KING)))
                break;
            returnValue.push_back({{x, y}, {newX, newY}});
        }
        newX = this->x;
        newY = this->y;
    }

    return returnValue;
}

vector<pair<pair<int, int>, pair<int, int>>> Bishop::generatePossibleMoves() const {

    vector<pair<pair<int, int>, pair<int, int>>> returnValue;
    int newX = this->x, newY = this->y;
    for(ll i = 0; i < 4; i++) {
        while (true) {
            newX += bishopMoves[i][0];
            newY += bishopMoves[i][1];
            if(notInRange(newX) || notInRange(newY) || (board[newY][newX] != nullptr && (board[newY][newX]->isWhite == isWhite || board[newY][newX]->pieceType == PieceType::KING))) break;
            returnValue.push_back({{x, y}, {newX, newY}});
        }
        newX = this->x; newY = this->y;
    }

    return returnValue;
}

vector<pair<pair<int, int>, pair<int, int>>> Knight::generatePossibleMoves() const {

    vector<pair<pair<int, int>, pair<int, int>>> returnValue;
    int newX, newY;
    for(ll i = 0; i < 8; i++) {
        newX = this->x + knightMoves[i][0];
        newY = this->y + knightMoves[i][1];
        if(notInRange(newX) || notInRange(newY) || (board[newY][this->x] != nullptr && (board[newY][this->x]->isWhite == isWhite || board[newY][newX]->pieceType == PieceType::KING))) {
            continue;
        }
        returnValue.push_back({{x, y}, {newX, newY}});
    }
    return returnValue;
}

vector<pair<pair<int, int>, pair<int, int>>> Queen::generatePossibleMoves() const {
    vector<pair<pair<int, int>, pair<int, int>>> returnValue;
    int newX, newY;
    newX = this->x; newY = this->y;
    for(ll i = 0; i < 4; i++) {
        while (true) {
            newX += rookMoves[i][0];
            newY += rookMoves[i][1];
            if(notInRange(newX) || notInRange(newY) || (board[newY][newX] != nullptr && (board[newY][newX]->isWhite == isWhite || board[newY][newX]->pieceType == PieceType::KING))) break;
            returnValue.push_back({{x, y}, {newX, newY}});
        }
        newX = this->x; newY = this->y;
    }

    newX = this->x; newY = this->y;
    for(ll i = 0; i < 4; i++) {
        while (true) {
            newX += bishopMoves[i][0];
            newY += bishopMoves[i][1];
            if(notInRange(newX) || notInRange(newY) || (board[newY][newX] != nullptr && board[newY][newX]->isWhite == isWhite)) break;
            returnValue.push_back({{x, y}, {newX, newY}});
        }
        newX = this->x; newY = this->y;
    }

    return returnValue;
}

vector<pair<pair<int, int>, pair<int, int>>> King::generatePossibleMoves() const {
    vector<pair<pair<int, int>, pair<int, int>>> returnValue;
    int newX, newY;
    newY = this->y;
    for(ll i = 0; i < 2; i++) {
        newX = this->x + rookMoves[i][1];
        if(notInRange( newX) || (board[newY][newX] != nullptr && (board[newY][newX]->isWhite == isWhite || board[newY][newX]->pieceType == PieceType::KING))) continue;
        returnValue.push_back({{x, y}, {newX, newY}});
    }

    newX = this->x;
    for(ll i = 0; i < 2; i++) {
        newY = this->y + rookMoves[i][1];
        if(notInRange( newY) || (board[newY][newX] != nullptr && (board[newY][newX]->isWhite == isWhite || board[newY][newX]->pieceType == PieceType::KING))) continue;
        returnValue.push_back({{x, y}, {newX, newY}});
    }

    for(ll i = 0; i < 4; i++) {
        newX = this->x + bishopMoves[i][0];
        newY = this->y + bishopMoves[i][1];
        if(notInRange(newX) || notInRange(newY) || (board[newY][newX] != nullptr && (board[newY][newX]->isWhite == isWhite || board[newY][newX]->pieceType == PieceType::KING))) continue;
        returnValue.push_back({{x, y}, {newX, newY}});
    }

    ll startRow = endPoint[isWhite ? 1 : 0];
    auto otherPieces = isWhite ^ playerWhite ? &computerPieces : &playerPieces;
    if(firstMove
        && board[0][startRow] != nullptr
        && board[0][startRow]->pieceType == ChessPiece::PieceType::ROOK
        && board[0][startRow]->firstMove
        && board[1][startRow] == nullptr
        && board[2][startRow] == nullptr
        && board[3][startRow] == nullptr
        && !this->underCheck(otherPieces)
        && !underAttack(0, 0, otherPieces)
        && !underAttack(1, 0, otherPieces)
        && !underAttack(2, 0, otherPieces)
        && !underAttack(3, 0, otherPieces)) {
        returnValue.push_back({{x, y}, {2, y}});
    }
    if(firstMove
        && board[7][startRow] != nullptr
        && board[7][startRow]->pieceType == ChessPiece::PieceType::ROOK
        && board[7][startRow]->firstMove
        && board[6][startRow] == nullptr
        && board[5][startRow] == nullptr
        && !this->underCheck(otherPieces)
        && !underAttack(7, 0, otherPieces)
        && !underAttack(6, 0, otherPieces)
        && !underAttack(5, 0, otherPieces)) {
        returnValue.push_back({{x, y}, {6, y}});
    }

    return returnValue;
}

vector<pair<pair<int, int>, pair<int, int>>> Pawn::generatePossibleMoves() const {

    vector<pair<pair<int, int>, pair<int, int>>> returnValue;
    int index = this->isWhite ? 0 : 1;
    int newY = this->y + pawnMoves[index];

    if(board[newY][this->x] == nullptr) {

        returnValue.push_back({{x, y}, {this->x, newY}});
        if (this->firstMove) {
            if (board[newY + pawnMoves[index]][this->x] == nullptr) {
                returnValue.push_back({{x, y}, {this->x, newY + pawnMoves[index]}});
            }
        }

    }

    int newX;
    for(ll i = 0; i < 2; i++) {
        newX = pawnMoves[i] + this->x;
        if(notInRange(newX) || board[newY][newX] == nullptr || board[newY][newX]->isWhite == isWhite || board[newY][newX]->pieceType == PieceType::KING) continue;

        returnValue.push_back({{x, y}, {newX, newY}});

        newX = this->x;
    }

    //En Passant
    if(newY == endPoint[index] - pawnMoves[index] * 3) {
        for(ll i = 0; i < 2; i++) {
            newX = this->x + pawnMoves[i];
            if(notInRange(newX)
            || board[this->y][newX] == nullptr
            || board[this->y][newX]->isWhite == isWhite
            || board[this->y][newX]->pieceType != PieceType::PAWN
            || !board[this->y][newX]->firstMove
            || !board[this->y][newX]->advancedTwo
            || board[newY][newX] != nullptr) continue;

            //check validity of move and evaluate
            returnValue.push_back({{x, y}, {newX, newY}});
        }
    }

    return returnValue;
}

bool Rook::attackingOtherPiece(int x, int y) const {

    if(this->x == x) {
        int yDir = this->y - y > 0 ? -1 : 1;
        int newY = this->y + yDir;
        while(newY != y) {
            if(board[newY][x] != nullptr) return false;
            newY += yDir;
        }
        return true;
    }

    if(this->y == y) {
        int xDir = this->x - x > 0 ? -1 : 1;
        int newX = this->x + xDir;
        while(newX != x) {
            if(board[y][newX] != nullptr) return false;
            newX += xDir;
        }
        return true;
    }

    return false;
}

bool Knight::attackingOtherPiece(int x, int y) const {

    int diffX = this->x - x, diffY = this->y - y;
    if(abs(diffX) < 1 || abs(diffX) > 2 || abs(diffY) < 1 || abs(diffY) > 2) return false;
    for(ll i = 0; i < 8; i++) {
        if(diffX == knightMoves[i][0] && diffY == knightMoves[i][1]) {
            return true;
        }
    }

    return false;
}

bool Bishop::attackingOtherPiece(int x, int y) const {

    if(abs(this->x - x) == abs(this->y - y)) {

        int xDir = this->x - x > 0 ? -1 : 1, yDir = this->y - y > 0 ? -1 : 1;
        int newX = this->x + xDir, newY = this->y + yDir;
        while(newX != x) {
            if(board[newY][newX] != nullptr) return false;
            newY += yDir;
            newX += xDir;
        }
        return true;
    }

    return false;
}

bool Queen::attackingOtherPiece(int x, int y) const {

    if(this->x == x) {
        int yDir = this->y - y > 0 ? -1 : 1;
        int newY = this->y + yDir;
        while(newY != y) {
            if(board[newY][x] != nullptr) return false;
            newY += yDir;
        }
        return true;
    }

    if(this->y == y) {
        int xDir = this->x - x > 0 ? -1 : 1;
        int newX = this->x + xDir;
        while(newX != x) {
            if(board[y][newX] != nullptr) return false;
            newX += xDir;
        }
        return true;
    }

    if(abs(this->x - x) == abs(this->y - y)) {

        int xDir = this->x - x > 0 ? -1 : 1, yDir = this->y - y > 0 ? -1 : 1;
        int newX = this->x + xDir, newY = this->y + yDir;
        while(newX != x) {
            if(board[newY][newX] != nullptr) return false;
            newY += yDir;
            newX += xDir;
        }
        return true;
    }

    return false;
}

bool King::attackingOtherPiece(int x, int y) const {
    return abs(this->x - x) <= 1 && abs(this->y - y) <= 1;
}

bool Pawn::attackingOtherPiece(int x, int y) const {

    int dir = this->isWhite ? 1 : -1;
    if(abs(this->x - x) == 1) {
        if(this->y + dir == y) {
            return true;
        }
    }

    if(board[y][x]->pieceType == PieceType::PAWN && board[y][x]->firstMove
    && ((dir == 1 && y == 4) || (dir == -1 && y == 3)) && y == this->y && abs(x - this->x) == 1) return true; //en passant

    return false;
}