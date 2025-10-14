//
// Created by Sam Gurarie on 9/8/25.
//

#include "Bitboard.h"
#include "sstream"

string Bitboard::stringBitBoard(ull board) {

    stringstream bitStream;
    for(ll i = 7; i >= 0; i--) {
        for(ll j = 7; j >= 0; j--) {
            if(((1ull << ((8ull * i) + j)) & board) == 0) {
                bitStream << "■";
            } else {
                bitStream << "□";
            }

            if(j > 0) bitStream << " ";
        }
        bitStream << endl;
    }

    return bitStream.str();
}

void Bitboard::printBitBoard(ull board) {
    cout << stringBitBoard(board) << endl;
}

void RookBitboard::generateAllMoves(bool inclusive) {


    for(ull i = 0; i < 8; i++) {
        for (ull j = 0; j < 8; j++) {

            ull firstUp = 0, firstDown = 0, firstLeft = 0, firstRight = 0;
            for(ull k = 0; k < i; k++) {
                firstUp <<= 1;
                firstUp++;
            }


            for(ull k = i + 1; k < 8ull; k++) {
                firstDown <<= 1;
                firstDown++;
            }
            firstDown <<= i;

            for(ull k = 0; k < j; k++) {
                firstLeft <<= 1;
                firstLeft++;
            }
            firstLeft <<= 7ull;

            for(ull k = j + 1ull; k < 8ull; k++) {
                firstRight <<= 1;
                firstRight++;
            }

            firstRight <<= 7ull + j;

            ull upMask = 0;
            for(ull k = 0; k < i; k++) {
                upMask |= (1ull << ((7ull - k) * 8ull + (7ull - j)));
            }

            ull downMask = 0;
            for(ull k = i + 1; k < 8ull; k++) {
                downMask |= (1ull << ((7ull - k) * 8ull + (7ull - j)));
            }

            ull leftMask = firstLeft << (8ull * (7ull - i) + j);
            ull rightMask = firstRight << (8ull * (7ull - i));
            positionMoves[i][j] = upMask | downMask | leftMask | rightMask;

            for(ll incrementor = 0; incrementor < (1ull << 14ull); incrementor++) {
                ull mask = 0;
                auto legalMoves = new vector<pair<short, short>>();

                ull upIntersection = (firstUp & incrementor);
                ull downIntersection = (firstDown & incrementor) >> i;
                ull leftIntersection = (firstLeft & incrementor) >> 7ull;
                ull rightIntersection = (firstRight & incrementor) >> (7ull + j);

                ull upRotator = 0;
                for(ull k = 0; k < i; k++) {
                    if((upIntersection & (1ull << k)) == 0) continue;
                    upRotator |= (1ull << ((7ull - k) * 8ull + (7ull - j)));
                }

                ull downRotator = 0;
                for(ull k = i + 1; k < 8ull; k++) {
                    if((downIntersection & (1ull << (7ull - k))) == 0) continue;
                    downRotator |= (1ull << ((7ull - k) * 8ull + (7ull - j)));
                }

                ull leftRotator = leftIntersection << (8ull * (7ull - i) + j);
                ull rightRotator = rightIntersection << (8ull * (7ull - i));

                mask |= upRotator | downRotator | leftRotator | rightRotator;

                ull chord = (7ull - i) * 8ull + (7ull - j);
                ull legalMovesBitboard = 0;
                for(ull k = 0; k < i; k++) {
                    //up
                    chord += 8ull;
                    if((mask & (1ull << chord)) > 0 && !inclusive) break;
                    legalMoves->push_back({7 - chord / 8, 7 - chord % 8});
                    legalMovesBitboard |= (1ull << chord);
                    if((mask & (1ull << chord)) > 0 && inclusive) break;
                }

                chord = (7ull - i) * 8ull + (7ull - j);
                for(ull k = i + 1; k < 8ull; k++) {
                    //down
                    chord -= 8ull;
                    if((mask & (1ull << chord)) > 0 && !inclusive) break;
                    legalMoves->push_back({7 - chord / 8, 7 - chord % 8});
                    legalMovesBitboard |= (1ull << chord);
                    if((mask & (1ull << chord)) > 0 && inclusive) break;
                }

                chord = (7ull - i) * 8ull + (7ull - j);
                for(ull k = 0; k < j; k++) {
                    //left
                    chord++;
                    if((mask & (1ull << chord)) > 0 && !inclusive) break;
                    legalMoves->push_back({7 - chord / 8, 7 - chord % 8});
                    legalMovesBitboard |= (1ull << chord);
                    if((mask & (1ull << chord)) > 0 && inclusive) break;
                }

                chord = (7ull - i) * 8ull + (7ull - j);
                for(ull k = j + 1ull; k < 8ull; k++) {
                    //right
                    chord--;
                    if((mask & (1ull << chord)) > 0 && !inclusive) break;
                    legalMoves->push_back({7 - chord / 8, 7 - chord % 8});
                    legalMovesBitboard |= (1ull << chord);
                    if((mask & (1ull << chord)) > 0 && inclusive) break;
                }

                KeyStore element = {mask, (short) i, (short) j};
                auto value = new LegalMovesStore{legalMovesBitboard, new vector<vector<pair<short, short>>*>{legalMoves}};

                if(!inclusive) {
                    moves[element] = value;
                } else {
                    movesInclusive[element] = value;
                }
            }
        }
    }
}

void BishopBitboard::generateAllMoves(bool inclusive) {


    for (ull i = 0; i < 8; i++) {
        for (ull j = 0; j < 8; j++) {

            ull firstUpLeft = 0, firstUpRight = 0, firstDownLeft = 0, firstDownRight = 0;
            ull maskUpLeft = 0, maskUpRight = 0, maskDownLeft = 0, maskDownRight = 0;
            ll newI = i, newJ = j;
            ull k = 0;
            while(true) {

                newI--;
                newJ--;
                if(newI < 0 || newJ < 0) break;
                maskUpLeft |= (1ull << ((7ull - i) * 8ull + (7ull - j)));

                firstUpLeft <<= 1ull;
                firstUpLeft++;
                k++;
            }


            newI = i, newJ = j;
            ull shiftLater = k;
            while(true) {

                newI--;
                newJ++;
                if(newI < 0 || newJ > 7) break;
                maskUpRight |= (1ull << ((7ull - i) * 8ull + (7ull - j)));

                firstUpRight <<= 1;
                firstUpRight++;
                k++;
            }
            firstUpRight <<= shiftLater;

            newI = i, newJ = j;
            ull shiftLater2 = k;
            while(true) {

                newI++;
                newJ--;
                if(newI > 7 || newJ < 0) break;
                maskDownLeft |= (1ull << ((7ull - i) * 8ull + (7ull - j)));

                firstDownLeft <<= 1ull;
                firstDownLeft++;
                k++;
            }
            firstDownLeft <<= shiftLater2;

            newI = i, newJ = j;
            ull shiftLater3 = k;
            while(true) {

                newI++;
                newJ++;
                if(newI > 7 || newJ > 7) break;
                maskDownRight |= (1ull << ((7ull - i) * 8ull + (7ull - j)));

                firstDownRight <<= 1ull;
                firstDownRight++;
                k++;
            }
            firstDownRight <<= shiftLater3;
            positionMoves[i][j] = maskDownLeft | maskUpLeft | maskDownRight | maskUpRight;

            for (ll incrementor = 0; incrementor < (1ull << k); incrementor++) {
                ull mask = 0;
                auto legalMoves = new vector<pair<short, short>>();

                ull upLeftIntersection = (firstUpLeft & incrementor);
                ull upRightIntersection = (firstUpRight & incrementor) >> shiftLater;
                ull downLeftIntersection = (firstDownLeft & incrementor) >> shiftLater2;
                ull downRightIntersection = (firstDownRight & incrementor) >> shiftLater3;

                ull upLeftRotator = 0;
                newI = i, newJ = j;
                ull newK = 0;
                while(true) {
                    newI--;
                    newJ--;
                    if(newI < 0 || newJ < 0) break;
                    if((upLeftIntersection & (1ull << (newK))) > 0)
                        upLeftRotator |= (1ull << (8ull * (7ull - newI) + (7ull - newJ)));
                    newK++;
                }

                ull upRightRotator = 0;
                newI = i, newJ = j;
                while(true) {
                    newI--;
                    newJ++;
                    if(newI < 0 || newJ > 7) break;
                    if((upRightIntersection & (1ull << (newK))) > 0)
                        upRightRotator |= (1ull << (8ull * (7ull - newI) + (7ull - newJ)));
                    newK++;
                }

                ull downLeftRotator = 0;
                newI = i, newJ = j;
                while(true) {
                    newI++;
                    newJ--;
                    if(newI > 7 || newJ < 0) break;
                    if((downLeftIntersection & (1ull << (newK))) > 0)
                        downLeftRotator |= (1ull << (8ull * (7ull - newI) + (7ull - newJ)));
                    newK++;
                }

                ull downRightRotator = 0;
                newI = i, newJ = j;
                while(true) {
                    newI++;
                    newJ++;
                    if(newI > 7 || newJ > 7) break;
                    if((downRightIntersection & (1ull << (newK))) > 0)
                        downRightRotator |= (1ull << (8ull * (7ull - newI) + (7ull - newJ)));
                    newK++;
                }

                mask |= upLeftRotator | upRightRotator | downLeftRotator | downRightRotator;

                ull chord = (7ull - i) * 8ull + (7ull - j);
                newI = i, newJ = j;
                ull legalMovesBitboard = 0;
                while(true) {
                    //up left

                    newI--;
                    newJ--;
                    if(newI < 0 || newJ < 0) break;

                    chord += 8ull + 1ull;
                    if((mask & (1ull << chord)) > 0 && !inclusive) break;
                    legalMoves->push_back({7 - chord / 8, 7 - chord % 8});
                    legalMovesBitboard |= (1ull << chord);
                    if((mask & (1ull << chord)) > 0 && inclusive) break;
                }

                chord = (7ull - i) * 8ull + (7ull - j);
                newI = i, newJ = j;
                while(true) {
                    //up right

                    newI--;
                    newJ++;
                    if(newI < 0 || newJ > 7) break;

                    chord += 8ull - 1ull;
                    if((mask & (1ull << chord)) > 0 && !inclusive) break;
                    legalMoves->push_back({7 - chord / 8, 7 - chord % 8});
                    legalMovesBitboard |= (1ull << chord);
                    if((mask & (1ull << chord)) > 0 && inclusive) break;
                }

                chord = (7ull - i) * 8ull + (7ull - j);
                newI = i, newJ = j;
                while(true) {
                    //down left

                    newI++;
                    newJ--;
                    if(newI > 7 || newJ < 0) break;

                    chord -= 8ull - 1ull;
                    if((mask & (1ull << chord)) > 0 && !inclusive) break;
                    legalMoves->push_back({7 - chord / 8, 7 - chord % 8});
                    legalMovesBitboard |= (1ull << chord);
                    if((mask & (1ull << chord)) > 0 && inclusive) break;
                }

                chord = (7ull - i) * 8ull + (7ull - j);
                newI = i, newJ = j;
                while(true) {
                    //down right

                    newI++;
                    newJ++;
                    if(newI > 7 || newJ > 7) break;

                    chord -= 8ull + 1ull;
                    if((mask & (1ull << chord)) > 0 && !inclusive) break;
                    legalMoves->push_back({7 - chord / 8, 7 - chord % 8});
                    legalMovesBitboard |= (1ull << chord);
                    if((mask & (1ull << chord)) > 0 && inclusive) break;
                }

                positionMoves[i][j] = mask;
                KeyStore element = {mask, (short) i, (short) j};
                auto value = new LegalMovesStore{legalMovesBitboard, new vector<vector<pair<short, short>>*>{legalMoves}};

                if(!inclusive) {
                    moves[element] = value;
                } else {
                    movesInclusive[element] = value;
                }
            }
        }
    }
}

void KnightBitboard::generateAllMoves(bool inclusive) {

    vector<pair<ll, ll>> knightMoves = {{1, 2}, {1, -2}, {-1, 2}, {-1, -2}, {2, 1}, {2, -1}, {-2, 1}, {-2, -1}};
    for(ull i = 0; i < 8ull; i++) {
        for(ull j = 0; j < 8ull; j++) {

            ull mask = 0;
            auto legalMoves = new vector<pair<short, short>>();
            for(ull k = 0; k < knightMoves.size(); k++) {
                ull newI = i + knightMoves[k].first, newJ = j + knightMoves[k].second;
                mask |= ((1ull << ((7ull - newI) * 8ull + (7ull - newJ))));
            }
            positionMoves[i][j] = mask;

            for (ull incrementor = 0; incrementor < (1ull << knightMoves.size()); incrementor++) {

                ull legalMovesBitboard = 0;
                ull legalMovesMask = 0;
                for (ull k = 0; k < knightMoves.size(); k++) {
                    ll newI = i + knightMoves[k].first, newJ = j + knightMoves[k].second;
                    if(newI < 0 || newI >= 8 || newJ < 0 || newJ >= 8) continue;
                    if (((1ull << k) & incrementor) > 0) {
                        legalMovesMask |= ((1ull << ((7ull - newI) * 8ull + (7ull - newJ))));

                        if(inclusive) {
                            legalMovesBitboard |= ((1ull << ((7ull - newI) * 8ull + (7ull - newJ))));
                            legalMoves->push_back({newI, newJ});
                        }

                        continue;
                    }

                    if(!inclusive) {
                        legalMovesBitboard |= ((1ull << ((7ull - newI) * 8ull + (7ull - newJ))));
                        legalMoves->push_back({newI, newJ});
                    }
                }

                KeyStore element = {legalMovesMask, (short) i, (short) j};
                auto value = new LegalMovesStore{legalMovesBitboard, new vector<vector<pair<short, short>>*>{legalMoves}};
                moves[element] = value;
            }

        }
    }
}

void KingBitboard::generateAllMoves(bool inclusive) {

    auto legalMoves = new vector<pair<short, short>>();

    vector<pair<ll, ll>> kingMoves = {{1, 1}, {1, -1}, {-1, 1}, {-1, -1}, {1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    for(ull i = 0; i < 8ull; i++) {
        for(ull j = 0; j < 8ull; j++) {

            ull mask = 0;
            for(ull k = 0; k < kingMoves.size(); k++) {
                ll newI = i + kingMoves[k].first, newJ = j + kingMoves[k].second;
                if(newI < 0 || newI >= 8 || newJ < 0 || newJ >= 8) continue;
                mask |= ((1ull << ((7ull - newI) * 8ull + (7ull - newJ))));
            }
            positionMoves[i][j] = mask;

            ull legalMovesBitboard = 0;
            ull legalMovesMask = 0;
            for(ull incrementor = 0; incrementor < (1ull << kingMoves.size()); incrementor++) {
                for(ull k = 0; k < kingMoves.size(); k++) {
                    ll newI = i + kingMoves[k].first, newJ = j + kingMoves[k].second;
                    if(newI < 0 || newI >= 8 || newJ < 0 || newJ >= 8) continue;
                    if (((1ull << k) & incrementor) > 0) {
                        legalMovesMask |= ((1ull << ((7ull - newI) * 8ull + (7ull - newJ))));
                        if(inclusive) {
                            legalMoves->push_back({newI, newJ});
                            legalMovesBitboard |= ((1ull << ((7ull - newI) * 8ull + (7ull - newJ))));
                        }
                        continue;
                    }

                    if(!inclusive) {
                        legalMoves->push_back({newI, newJ});
                        legalMovesBitboard |= ((1ull << ((7ull - newI) * 8ull + (7ull - newJ))));
                    }
                }
            }
            KeyStore element = {legalMovesMask, (short) i, (short) j};
            auto value = new LegalMovesStore{legalMovesBitboard, new vector<vector<pair<short, short>>*>{legalMoves}};
            movesInclusive[element] = value;
        }
    }
}

void addPawnValues(vector<pair<ll, ll>> &limit, ull i, ull j, Bitboard* board) {
    ull legalMovesMask = 0;
    ull legalMovesBitboard = 0;
    auto legalMoves = new vector<pair<short, short>>();


    for (ull incrementor = 0; incrementor < (1ull << limit.size()); incrementor++) {

        bool blockedFirstMove = false;
        for (ull k = 0; k < limit.size(); k++) {
            if (((1ull << k) & incrementor) > 0) {
                legalMovesMask |= ((1ull << ((7ull - limit[k].first) * 8ull + (7ull - limit[k].second))));

                if(j != limit[k].second) {
                    legalMoves->push_back(limit[k]);
                } else {
                    blockedFirstMove = true;
                }
            } else if(j == limit[k].second && !blockedFirstMove) {
                legalMoves->push_back(limit[k]);
            }
        }

        KeyStore element = {legalMovesMask, (short) i, (short) j};
        auto value = new LegalMovesStore{legalMovesBitboard, new vector<vector<pair<short, short>>*>{legalMoves}};
        board->setBoard(element, value);
    }
}

void WhitePawnBitboard::generateAllMoves(bool inclusive) {

    for(ull i = 0; i < 8ull; i++) {
        for(ull j = 0; j < 8ull; j++) {

            ull mask = 0;

            vector<pair<ll, ll>> limit;
            if(i < 7ull) {

                if(!inclusive) {
                    mask |= ((1ull << ((7ull - (i + 1)) * 8ull + (7ull - j))));
                    limit.push_back({i + 1, j});
                    if (i == 2ull) {
                        mask |= ((1ull << ((7ull - (i + 2)) * 8ull + (7ull - j))));
                        limit.push_back({i + 2, j});
                    }
                }

                if (inclusive) {

                    if(j < 7ull) {
                        mask |= ((1ull << ((7ull - (i + 1)) * 8ull + (7ull - (j + 1)))));
                        limit.push_back({i + 1, j + 1});
                    }

                    if(j > 0ll) {
                        mask |= ((1ull << ((7ull - (i + 1)) * 8ull + (7ull - (j - 1)))));
                        limit.push_back({i + 1, j - 1});
                    }
                }
            }

            positionMoves[i][j] = mask;
            addPawnValues(limit, i, j, this);
        }
    }
}

//TODO: implement en passant and promotions

void BlackPawnBitboard::generateAllMoves(bool inclusive) {
    for(ull i = 0; i < 8ull; i++) {
        for(ull j = 0; j < 8ull; j++) {

            ull mask = 0;
            ull maskInc = 0;
            vector<pair<ll, ll>> limit;
            if(i > 0ull) {

                if(!inclusive) {
                    mask |= ((1ull << ((7ull - (i - 1)) * 8ull + (7ull - j))));
                    limit.push_back({i - 1, j});
                    if (i == 5ull) {
                        mask |= ((1ull << ((7ull - (i - 2)) * 8ull + (7ull - j))));
                        limit.push_back({i - 2, j});
                    }
                }

                if (inclusive) {

                    if(j < 7ull) {
                        maskInc |= ((1ull << ((7ull - (i - 1)) * 8ull + (7ull - (j + 1)))));
                        limit.push_back({i - 1, j + 1});
                    }

                    if(j > 0ll) {
                        maskInc |= ((1ull << ((7ull - (i - 1)) * 8ull + (7ull - (j - 1)))));
                        limit.push_back({i - 1, j - 1});
                    }
                }
            }

            positionMoves[i][j] = mask;
            positionMovesIncl[i][j] = maskInc;
            addPawnValues(limit, i, j, this);
        }
    }
}

void QueenBitboard::generateAllMoves(BishopBitboard bishop, RookBitboard rook) {

    for(ull i = 0; i < 8ull; i++) {
        for(ull j = 0; j < 8ull; j++) {
            positionMoves[i][j] |= bishop.positionMoves[i][j] | rook.positionMoves[i][j];
        }
    }
}

//TODO: change move generation logic so that inclusive is only at the intersection
//TODO: change move generation so that each axis is independent (performance improvement)
//Later this will get added to same team & other team exclusive moves


pair<vector<LegalMovesStore*>, vector<LegalMovesStore*>> QueenBitboard::getMove(BishopBitboard bishop, RookBitboard rook, short x, short y, ull sameTeamMask, ull otherTeamMask) {
    return {bishop.getMove(x, y, sameTeamMask, otherTeamMask), rook.getMove(x, y, sameTeamMask, otherTeamMask)};
}

vector<LegalMovesStore*> BishopBitboard::getMove(short x, short y, ull sameTeamMask, ull otherTeamMask) {
    return {movesInclusive[{positionMoves[y][x] & otherTeamMask, x, y}],
            moves[{positionMoves[y][x] & sameTeamMask, x, y}]};
}

vector<LegalMovesStore*> RookBitboard::getMove(short x, short y, ull sameTeamMask, ull otherTeamMask) {
    return {movesInclusive[{positionMoves[y][x] & otherTeamMask, x, y}],
            moves[{positionMoves[y][x] & sameTeamMask, x, y}]};
}

vector<LegalMovesStore*> KnightBitboard::getMove(short x, short y, ull sameTeamMask, ull otherTeamMask) {
    return {movesInclusive[{positionMoves[y][x] & otherTeamMask, x, y}],
            moves[{positionMoves[y][x] & sameTeamMask, x, y}]};
}

vector<LegalMovesStore*> KingBitboard::getMove(short x, short y, ull sameTeamMask, ull otherTeamMask) {
    return {movesInclusive[{positionMoves[y][x] & otherTeamMask, x, y}],
            moves[{positionMoves[y][x] & sameTeamMask, x, y}]};
}

//We'll implement promotion in a higher level function
vector<LegalMovesStore*> WhitePawnBitboard::getMove(short x, short y, ull sameTeamMask, ull otherTeamMask, ull otherPawns) {
    vector<LegalMovesStore*>  returnValue = {movesInclusive[{positionMovesIncl[y][x] & otherTeamMask, x, y}],
            moves[{positionMoves[y][x] & (sameTeamMask | otherTeamMask), x, y}]};
    if(y == 5) {
        returnValue.push_back(movesInclusive[{positionMovesIncl[y - 1][x] & otherTeamMask, x, (short) (y - 1)}]); // en passant
    }
}

vector<LegalMovesStore*> BlackPawnBitboard::getMove(short x, short y, ull sameTeamMask, ull otherTeamMask, ull otherPawns) {
    vector<LegalMovesStore*> returnValue = {movesInclusive[{positionMovesIncl[y][x] & otherTeamMask, x, y}],
            moves[{positionMoves[y][x] & (sameTeamMask | otherTeamMask), x, y}]};
    if(y == 3) {
        returnValue.push_back(movesInclusive[{positionMovesIncl[y + 1][x] & otherTeamMask, x, (short) (y + 1)}]);
    }
}