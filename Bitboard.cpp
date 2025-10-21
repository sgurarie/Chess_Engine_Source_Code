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

            ull firstLeft = 0, firstRight = 0;

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
            positionMoves[i][j][0] = upMask;
            positionMoves[i][j][1] = downMask;
            positionMoves[i][j][2] = leftMask;
            positionMoves[i][j][3] = rightMask;

            ull chord = (7ull - i) * 8ull + (7ull - j);
            for(ll incrementor = 0; incrementor < (1ull << (i + 1)); incrementor++) {

                ull mask = 0;
                for(ull k = 0; k < i; k++) {
                    if((incrementor & (1ull << k)) == 0) continue;
                    mask |= (1ull << ((7ull - k) * 8ull + (7ull - j)));
                }

                KeyStore element = {mask, (short) j, (short) i, 0};
                ull legalMovesBitboard = 0;
                for (ull k = 0; k < i; k++) {
                    //up
                    chord += 8ull;
                    if ((mask & (1ull << chord)) > 0 && !inclusive) break;

                    if (!inclusive) {
                        legalMovesBitboard |= (1ull << chord);
                        legalMoves->push_back({legalMovesBitboard, (short) (7 - chord / 8), (short) (7 - chord % 8)});
                    }

                    if ((mask & (1ull << chord)) > 0 && inclusive) {
                        legalMovesBitboard |= (1ull << chord);
                        legalMoves->push_back({legalMovesBitboard, (short) (7 - chord / 8), (short) (7 - chord % 8)});
                        break;
                    }
                }

                if(inclusive) {
                    movesInclusive[element] = legalMoves;
                } else {
                    moves[element] = legalMoves;
                }
            }

            chord = (7ull - i) * 8ull + (7ull - j);
            for(ll incrementor = 0; incrementor < (1ull << (7ull - i)); incrementor++) {

                ull mask = 0;
                for(ull k = i + 1; k < 8ull; k++) {
                    if((incrementor & (1ull << (7ull - k))) == 0) continue;
                    mask |= (1ull << ((7ull - k) * 8ull + (7ull - j)));
                }

                KeyStore element = {mask, (short) j, (short) i, 1};
                ull legalMovesBitboard = 0;
                for (ull k = i + 1; k < 8ull; k++) {
                    //down
                    chord -= 8ull;
                    if ((mask & (1ull << chord)) > 0 && !inclusive) break;

                    if (!inclusive) {
                        ull legalMovesBitboard = (1ull << chord);
                        legalMoves->push_back({legalMovesBitboard, (short) (7 - chord / 8), (short) (7 - chord % 8)});
                    }

                    if ((mask & (1ull << chord)) > 0 && inclusive) {
                        ull legalMovesBitboard = (1ull << chord);
                        legalMoves->push_back({legalMovesBitboard, (short) (7 - chord / 8), (short) (7 - chord % 8)});
                        break;
                    }
                }

                if(inclusive) {
                    movesInclusive[element] = legalMoves;
                } else {
                    moves[element] = legalMoves;
                }
            }

            chord = (7ull - i) * 8ull + (7ull - j);
            for(ll incrementor = 0; incrementor < (1ull << (j + 1)); incrementor++) {
                ull legalMovesBitboard = 0;
                ull leftIntersection = (firstLeft & incrementor) >> 7ull;
                ull mask = leftIntersection << (8ull * (7ull - i) + j);

                KeyStore element = {mask, (short) j, (short) i, 2};
                for (ull k = 0; k < j; k++) {
                    //left
                    chord++;
                    if ((mask & (1ull << chord)) > 0 && !inclusive) break;

                    if (!inclusive) {
                        ull legalMovesBitboard = (1ull << chord);
                        legalMoves->push_back({legalMovesBitboard, (short) (7 - chord / 8), (short) (7 - chord % 8)});
                    }

                    if ((mask & (1ull << chord)) > 0 && inclusive) {
                        ull legalMovesBitboard = (1ull << chord);
                        legalMoves->push_back({legalMovesBitboard, (short) (7 - chord / 8), (short) (7 - chord % 8)});
                        break;
                    }
                }

                if(inclusive) {
                    movesInclusive[element] = legalMoves;
                } else {
                    moves[element] = legalMoves;
                }
            }

            chord = (7ull - i) * 8ull + (7ull - j);
            for(ll incrementor = 0; incrementor < (1ull << (7ull - j)); incrementor++) {

                ull legalMovesBitboard = 0;
                ull rightIntersection = (firstRight & incrementor) >> (7ull + j);
                ull mask = rightIntersection << (8ull * (7ull - i));
                KeyStore element = {mask, (short) j, (short) i, 3};

                for (ull k = j + 1ull; k < 8ull; k++) {
                    //right
                    chord--;
                    if ((mask & (1ull << chord)) > 0 && !inclusive) break;

                    if (!inclusive) {
                        ull legalMovesBitboard = (1ull << chord);
                        legalMoves->push_back({legalMovesBitboard, (short) (7 - chord / 8), (short) (7 - chord % 8)});
                    }

                    if ((mask & (1ull << chord)) > 0 && inclusive) {
                        ull legalMovesBitboard = (1ull << chord);
                        legalMoves->push_back({legalMovesBitboard, (short) (7 - chord / 8), (short) (7 - chord % 8)});
                        break;
                    }
                }

                if(inclusive) {
                    movesInclusive[element] = legalMoves;
                } else {
                    moves[element] = legalMoves;
                }
            }
        }
    }
}

void BishopBitboard::generateAllMoves(bool inclusive) {


    for (ull i = 0; i < 8; i++) {
        for (ull j = 0; j < 8; j++) {

            ull maskUpLeft = 0, maskUpRight = 0, maskDownLeft = 0, maskDownRight = 0;
            ll newI = i, newJ = j;
            ull k = 0;
            while(true) {

                newI--;
                newJ--;
                if(newI < 0 || newJ < 0) break;
                maskUpLeft |= (1ull << ((7ull - newI) * 8ull + (7ull - newJ)));
                k++;
            }


            newI = i, newJ = j;
            while(true) {

                newI--;
                newJ++;
                if(newI < 0 || newJ > 7) break;
                maskUpRight |= (1ull << ((7ull - newI) * 8ull + (7ull - newJ)));
                k++;
            }

            newI = i, newJ = j;
            while(true) {

                newI++;
                newJ--;
                if(newI > 7 || newJ < 0) break;
                maskDownLeft |= (1ull << ((7ull - newI) * 8ull + (7ull - newJ)));
                k++;
            }

            newI = i, newJ = j;
            while(true) {

                newI++;
                newJ++;
                if(newI > 7 || newJ > 7) break;
                maskDownRight |= (1ull << ((7ull - newI) * 8ull + (7ull - newJ)));
                k++;
            }

            positionMoves[i][j][0] = maskDownLeft;
            positionMoves[i][j][1] = maskUpLeft;
            positionMoves[i][j][2] = maskDownRight;
            positionMoves[i][j][3] = maskUpRight;


            for (ll incrementor = 0; incrementor < (1ull << k); incrementor++) {

                ll newK = 0;
                newI = i, newJ = j;
                ull chord = (7ull - i) * 8ull + (7ull - j);
                ull legalMovesBitboard = 0;
                ull mask = 0;

                while (true) {
                    newI--;
                    newJ--;
                    if (newI < 0 || newJ < 0) break;
                    if ((incrementor & (1ull << (newK))) > 0)
                        mask |= (1ull << (8ull * (7ull - newI) + (7ull - newJ)));
                    newK++;
                }

                while(true) {
                    //up left

                    newI--;
                    newJ--;
                    if(newI < 0 || newJ < 0) break;

                    chord += 8ull + 1ull;
                    if((mask & (1ull << chord)) > 0 && !inclusive) break;

                    if(!inclusive) {
                        ull legalMovesBitboard = (1ull << chord);
                        legalMoves->push_back({legalMovesBitboard, (short) (7 - chord / 8), (short) (7 - chord % 8)});
                    }

                    if((mask & (1ull << chord)) > 0 && inclusive) {
                        ull legalMovesBitboard = (1ull << chord);
                        legalMoves->push_back({legalMovesBitboard, (short) (7 - chord / 8), (short) (7 - chord % 8)});
                        break;
                    }
                }

                KeyStore element = {mask, (short) j, (short) i, 0};
                if(inclusive) {
                    movesInclusive[element] = legalMoves;
                } else {
                    moves[element] = legalMoves;
                }
            }

            for (ll incrementor = 0; incrementor < (1ull << k); incrementor++) {

                ll newK = 0;
                newI = i, newJ = j;
                ull chord = (7ull - i) * 8ull + (7ull - j);
                ull legalMovesBitboard = 0;
                ull mask = 0;

                while (true) {
                    newI--;
                    newJ++;
                    if (newI < 0 || newJ > 7) break;
                    if ((incrementor & (1ull << (newK))) > 0)
                        mask |= (1ull << (8ull * (7ull - newI) + (7ull - newJ)));
                    newK++;
                }

                newI = i, newJ = j;
                while(true) {
                    //up right

                    newI--;
                    newJ++;
                    if(newI < 0 || newJ > 7) break;

                    chord += 8ull - 1ull;
                    if((mask & (1ull << chord)) > 0 && !inclusive) break;

                    if(!inclusive) {
                        ull legalMovesBitboard = (1ull << chord);
                        legalMoves->push_back({legalMovesBitboard, (short) (7 - chord / 8), (short) (7 - chord % 8)});
                    }

                    if((mask & (1ull << chord)) > 0 && inclusive) {
                        ull legalMovesBitboard = (1ull << chord);
                        legalMoves->push_back({legalMovesBitboard, (short) (7 - chord / 8), (short) (7 - chord % 8)});
                        break;
                    }
                }

                KeyStore element = {mask, (short) j, (short) i, 1};
                if(inclusive) {
                    movesInclusive[element] = legalMoves;
                } else {
                    moves[element] = legalMoves;
                }
            }

            for (ll incrementor = 0; incrementor < (1ull << k); incrementor++) {

                ll newK = 0;
                newI = i, newJ = j;
                ull chord = (7ull - i) * 8ull + (7ull - j);
                ull legalMovesBitboard = 0;
                ull mask = 0;

                while (true) {
                    newI++;
                    newJ--;
                    if (newI > 7 || newJ < 0) break;
                    if ((incrementor & (1ull << (newK))) > 0)
                        mask |= (1ull << (8ull * (7ull - newI) + (7ull - newJ)));
                    newK++;
                }

                newI = i, newJ = j;
                while(true) {
                    //down left

                    newI++;
                    newJ--;
                    if(newI > 7 || newJ < 0) break;

                    chord -= 8ull - 1ull;
                    if((mask & (1ull << chord)) > 0 && !inclusive) break;

                    if(!inclusive) {
                        ull legalMovesBitboard = (1ull << chord);
                        legalMoves->push_back({legalMovesBitboard, (short) (7 - chord / 8), (short) (7 - chord % 8)});
                    }
                    if((mask & (1ull << chord)) > 0 && inclusive) {
                        ull legalMovesBitboard = (1ull << chord);
                        legalMoves->push_back({legalMovesBitboard, (short) (7 - chord / 8), (short) (7 - chord % 8)});
                        break;
                    }
                }

                KeyStore element = {mask, (short) j, (short) i, 2};
                if(inclusive) {
                    movesInclusive[element] = legalMoves;
                } else {
                    moves[element] = legalMoves;
                }
            }

            for (ll incrementor = 0; incrementor < (1ull << k); incrementor++) {

                ll newK = 0;
                newI = i, newJ = j;
                ull chord = (7ull - i) * 8ull + (7ull - j);
                ull mask = 0;
                ull legalMovesBitboard = 0;

                while (true) {
                    newI++;
                    newJ++;
                    if (newI > 7 || newJ > 7) break;
                    if ((incrementor & (1ull << (newK))) > 0)
                        mask |= (1ull << (8ull * (7ull - newI) + (7ull - newJ)));
                }

                newI = i;
                newJ = j;
                while (true) {
                    //down right

                    newI++;
                    newJ++;
                    if (newI > 7 || newJ > 7) break;

                    chord -= 8ull + 1ull;
                    if ((mask & (1ull << chord)) > 0 && !inclusive) break;

                    if (!inclusive) {
                        legalMovesBitboard |= (1ull << chord);
                        legalMoves->push_back({legalMovesBitboard, (short) (7 - chord / 8), (short) (7 - chord % 8)});
                    }

                    if ((mask & (1ull << chord)) > 0 && inclusive) {
                        legalMovesBitboard |= (1ull << chord);
                        legalMoves->push_back({legalMovesBitboard, (short) (7 - chord / 8), (short) (7 - chord % 8)});
                        break;
                    }
                }

                KeyStore element = {mask, (short) j, (short) i, 3};
                if (inclusive) {
                    movesInclusive[element] = legalMoves;
                } else {
                    moves[element] = legalMoves;
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
            auto legalMoves = new vector<LegalMovesStore>();
            for(ull k = 0; k < knightMoves.size(); k++) {
                ull newI = i + knightMoves[k].first, newJ = j + knightMoves[k].second;
                mask |= ((1ull << ((7ull - newI) * 8ull + (7ull - newJ))));
            }
            positionMoves[i][j] = mask;

            for (ull incrementor = 0; incrementor < (1ull << knightMoves.size()); incrementor++) {

                ull legalMovesMask = 0;
                ull legalMovesBitboard = 0;
                for (ull k = 0; k < knightMoves.size(); k++) {
                    short newI = i + knightMoves[k].first, newJ = j + knightMoves[k].second;
                    if(newI < 0 || newI >= 8 || newJ < 0 || newJ >= 8) continue;
                    if (((1ull << k) & incrementor) > 0) {
                        legalMovesMask |= ((1ull << ((7ull - newI) * 8ull + (7ull - newJ))));

                        if(inclusive) {
                            ull legalMovesBitboard = ((1ull << ((7ull - newI) * 8ull + (7ull - newJ))));
                            legalMoves->push_back({legalMovesBitboard, newI, newJ});
                        }

                        continue;
                    }

                    if(!inclusive) {
                        ull legalMovesBitboard = ((1ull << ((7ull - newI) * 8ull + (7ull - newJ))));
                        legalMoves->push_back({legalMovesBitboard, newI, newJ});
                    }
                }

                KeyStore element = {legalMovesMask, (short) j, (short) i, 0};
                if(inclusive) {
                    movesInclusive[element] = legalMoves;
                } else {
                    moves[element] = legalMoves;
                }
            }

        }
    }
}

void KingBitboard::generateAllMoves(bool inclusive) {

    auto legalMoves = new vector<LegalMovesStore>();

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


            ull legalMovesMask = 0;
            for(ull incrementor = 0; incrementor < (1ull << kingMoves.size()); incrementor++) {
                for(ull k = 0; k < kingMoves.size(); k++) {
                    ll newI = i + kingMoves[k].first, newJ = j + kingMoves[k].second;
                    if(newI < 0 || newI >= 8 || newJ < 0 || newJ >= 8) continue;
                    if (((1ull << k) & incrementor) > 0) {
                        legalMovesMask |= ((1ull << ((7ull - newI) * 8ull + (7ull - newJ))));
                        if(inclusive) {
                            ull legalMovesBitboard = ((1ull << ((7ull - newI) * 8ull + (7ull - newJ))));
                            legalMoves->push_back({legalMovesBitboard, (short) newI, (short) newJ});
                        }
                        continue;
                    }

                    if(!inclusive) {
                        ull legalMovesBitboard = ((1ull << ((7ull - newI) * 8ull + (7ull - newJ))));
                        legalMoves->push_back({legalMovesBitboard, (short) newI, (short) newJ});
                    }
                }
            }
            KeyStore element = {legalMovesMask, (short) j, (short) i, 0};
            if(inclusive) {
                movesInclusive[element] = legalMoves;
            } else {
                moves[element] = legalMoves;
            }
        }
    }
}

void addPawnValues(vector<pair<short, short>> &limit, ull i, ull j, Bitboard* board) {
    ull legalMovesMask = 0;

    auto legalMoves = new vector<LegalMovesStore>();


    for (ull incrementor = 0; incrementor < (1ull << limit.size()); incrementor++) {

        bool blockedFirstMove = false;
        for (ull k = 0; k < limit.size(); k++) {
            if (((1ull << k) & incrementor) > 0) {
                ull legalMovesBitboard = ((1ull << ((7ull - limit[k].first) * 8ull + (7ull - limit[k].second))));
                legalMovesMask |= legalMovesBitboard;

                if(j != limit[k].second) {
                    legalMoves->push_back({legalMovesBitboard, limit[k].first, limit[k].second});
                } else {
                    blockedFirstMove = true;
                }
            } else if(j == limit[k].second && !blockedFirstMove) {
                ull legalMovesBitboard = ((1ull << ((7ull - limit[k].first) * 8ull + (7ull - limit[k].second))));
                legalMoves->push_back({legalMovesBitboard, limit[k].first, limit[k].second});
            }
        }

        KeyStore element = {legalMovesMask, (short) j, (short) i, 0};
        board->setBoard(element, legalMoves);
    }
}

void WhitePawnBitboard::generateAllMoves(bool inclusive) {

    for(ull i = 0; i < 8ull; i++) {
        for(ull j = 0; j < 8ull; j++) {

            ull mask = 0, maskInc = 0;

            vector<pair<short, short>> limit;
            if(i < 7ull) {

                if(!inclusive) {
                    mask |= ((1ull << ((7ull - (i + 1)) * 8ull + (7ull - j))));
                    limit.push_back({i + 1, j});
                    if (i == 1ull) {
                        mask |= ((1ull << ((7ull - (i + 2)) * 8ull + (7ull - j))));
                        limit.push_back({i + 2, j});
                    }
                }

                if (inclusive) {

                    if(j < 7ull) {
                        maskInc |= ((1ull << ((7ull - (i + 1)) * 8ull + (7ull - (j + 1)))));
                        limit.push_back({i + 1, j + 1});
                    }

                    if(j > 0ll) {
                        maskInc |= ((1ull << ((7ull - (i + 1)) * 8ull + (7ull - (j - 1)))));
                        limit.push_back({i + 1, j - 1});
                    }
                }
            }

            positionMoves[i][j] = mask;
            positionMovesIncl[i][j] = maskInc;
            addPawnValues(limit, i, j, this);
        }
    }
}

//TODO: implement promotions

void BlackPawnBitboard::generateAllMoves(bool inclusive) {
    for(ull i = 0; i < 8ull; i++) {
        for(ull j = 0; j < 8ull; j++) {

            ull mask = 0;
            ull maskInc = 0;
            vector<pair<short, short>> limit;
            if(i > 0ull) {

                if(!inclusive) {
                    mask |= ((1ull << ((7ull - (i - 1)) * 8ull + (7ull - j))));
                    limit.push_back({i - 1, j});
                    if (i == 6ull) {
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

//TODO: change move generation so that each axis is independent (performance improvement) and later for pinning
//Later this will get added to same team & other team exclusive moves


pair<vector<vector<LegalMovesStore>*>, vector<vector<LegalMovesStore>*>> QueenBitboard::getMove(BishopBitboard &bishop, RookBitboard &rook, short x, short y, ull sameTeamMask, ull otherTeamMask) {
    return {bishop.getMove(x, y, sameTeamMask, otherTeamMask), rook.getMove(x, y, sameTeamMask, otherTeamMask)};
}

vector<vector<LegalMovesStore>*> BishopBitboard::getMove(short x, short y, ull sameTeamMask, ull otherTeamMask) {
    vector<vector<LegalMovesStore>*> array(8);

    for(ll i = 0; i < 4; i++) {
        array[2 * i] = moves[{positionMoves[y][x][i] & sameTeamMask, x, y}];
        array[2 * i + 1] = movesInclusive[{positionMoves[y][x][i] & otherTeamMask, x, y}];
    }
    return array;
}

vector<vector<LegalMovesStore>*> RookBitboard::getMove(short x, short y, ull sameTeamMask, ull otherTeamMask) {
    vector<vector<LegalMovesStore>*> array(8);

    for(ll i = 0; i < 4; i++) {
        array[2 * i] = moves[{positionMoves[y][x][i] & sameTeamMask, x, y}];
        array[2 * i + 1] = movesInclusive[{positionMoves[y][x][i] & otherTeamMask, x, y}];
    }
    return array;
}

pair<vector<LegalMovesStore>*, vector<LegalMovesStore>*> KnightBitboard::getMove(short x, short y, ull sameTeamMask, ull otherTeamMask) {
    return {movesInclusive[{positionMoves[y][x] & otherTeamMask, x, y}],
            moves[{positionMoves[y][x] & sameTeamMask, x, y}]};
}

pair<vector<LegalMovesStore>*, vector<LegalMovesStore>*> KingBitboard::getMove(short x, short y, ull sameTeamMask, ull otherTeamMask) {
    return {movesInclusive[{positionMoves[y][x] & otherTeamMask, x, y}],
            moves[{positionMoves[y][x] & sameTeamMask, x, y}]};
}

//We'll implement promotion in a higher level function
vector<vector<LegalMovesStore>*> WhitePawnBitboard::getMove(short x, short y, ull sameTeamMask, ull otherTeamMask, ull otherPawns) {
    vector<vector<LegalMovesStore>*> returnValue = {movesInclusive[{positionMovesIncl[y][x] & otherTeamMask, x, y}],
            moves[{positionMoves[y][x] & (sameTeamMask | otherTeamMask), x, y}]};
    if(y == 4) {
        returnValue.push_back(movesInclusive[{positionMovesIncl[y - 1][x] & otherPawns, x, (short) (y - 1)}]); // en passant
    }

    return returnValue;
}

vector<vector<LegalMovesStore>*> BlackPawnBitboard::getMove(short x, short y, ull sameTeamMask, ull otherTeamMask, ull otherPawns) {
    vector<vector<LegalMovesStore>*> returnValue = {movesInclusive[{positionMovesIncl[y][x] & otherTeamMask, x, y}],
                                                    moves[{positionMoves[y][x] & (sameTeamMask | otherTeamMask), x, y}]};
    if(y == 3) {
        returnValue.push_back(movesInclusive[{positionMovesIncl[y + 1][x] & otherPawns, x, (short) (y + 1)}]);
    }

    return returnValue;
}