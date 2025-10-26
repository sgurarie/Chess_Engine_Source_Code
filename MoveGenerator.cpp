//
// Created by Sam Gurarie on 10/19/25.
//

#include "MoveGenerator.h"


MoveGenerator::MoveGenerator() {

    bishop.generateAllMoves(true);
    bishop.generateAllMoves(false);

    rook.generateAllMoves(true);
    rook.generateAllMoves(false);

    knight.generateAllMoves(true);
    knight.generateAllMoves(false);

    king.generateAllMoves(true);
    king.generateAllMoves(false);

    whitePawn.generateAllMoves(true);
    whitePawn.generateAllMoves(false);

    blackPawn.generateAllMoves(true);
    blackPawn.generateAllMoves(false);
}

ReturnValueDoMove MoveGenerator::doMove(BitboardMove &move, ll i) {

    ReturnValueDoMove returnValue;

    BasicChessPiece& it = board.holder.chessPieces[move.isWhite][(int) move.piece][i];
    bool breakOut = false;
    for(ll k = 0; k < 6; k++) {
        for(ll j = 0; j < board.holder.limitCap[!move.isWhite][k]; j++) {
            if(board.holder.chessPieces[!move.isWhite][(int) move.piece][k].y == move.y2 && board.holder.chessPieces[!move.isWhite][(int) move.piece][k].x == move.x2) {
                returnValue.captured = board.holder.chessPieces[!move.isWhite][(int) move.piece][k];
                for(ll o = j + 1; o < board.holder.limitCap[!move.isWhite][k]; o++) {
                    board.holder.chessPieces[!move.isWhite][(int) move.piece][o - 1] =  board.holder.chessPieces[!move.isWhite][(int) move.piece][o];
                }
                breakOut = true;
                break;
            }
        }

        if(breakOut) break;
    }

    BasicChessPiece& original = board.holder.chessPieces[move.isWhite][(int) move.piece][i];
    original.y = move.y2;
    original.x = move.x2;
    returnValue.piece = move.piece;
    original.piece = move.piece;

    returnValue.generalAttackPattern = board.generalAttackPattern[move.isWhite];
    returnValue.attackPattern = board.generalAttackPattern[move.isWhite];
    returnValue.pinnedPieces = board.generalAttackPattern[move.isWhite];


    board.boardStates[move.isWhite][(int) move.piece] ^= move.startBoard;
    board.boardStates[move.isWhite][(int) move.piece] ^= move.endBoard;

    board.totalBoard[move.isWhite] ^= move.startBoard;
    board.totalBoard[move.isWhite] ^= move.endBoard;


    if(move.piece == PieceType::PAWN) {
        if((move.isWhite && move.y2 == 3 && move.y1 == 1) || (!move.isWhite && move.y2 == 4 && move.y1 == 6)) {
            board.advancedTwoPawnMove[move.isWhite][move.x1] = true;
        }

        if((move.isWhite && move.y1 == 3) || (!move.isWhite && move.y1 == 4)) {
            if(board.advancedTwoPawnMove[move.isWhite][move.x1]) returnValue.wasAdvancedTwo = true;
            board.advancedTwoPawnMove[move.isWhite][move.x1] = false;
        }
    }

    if(move.piece == PieceType::ROOK && ((move.isWhite && move.y1 == 0 ) || (!move.isWhite && move.y1 == 7) && (move.x1 == 0 || move.x1 == 7))
    || (move.piece == PieceType::KING && move.x1 == 4 && ((move.isWhite && move.y1 == 0) || (!move.isWhite && move.y1 == 7)))) {
        if(board.firstRookKingMove[move.isWhite]) returnValue.wasFirstMove = true;
        board.firstRookKingMove[move.isWhite] = false;
    }

    /*board.pinnedPieces;
    board.attackPattern;
    board.generalAttackPattern;*/

    return returnValue;
}



void MoveGenerator::undoMove(BitboardMove &move, ReturnValueDoMove &returnValue, ll i) {
    board.boardStates[move.isWhite][(int) move.piece] ^= move.endBoard;
    if(returnValue.captured.piece == PieceType::NONE)
        board.boardStates[move.isWhite][(int) move.piece] ^= move.startBoard;
    else
        board.boardStates[move.isWhite][(int) returnValue.captured.piece] ^= 1ull << ((7ull - returnValue.captured.y) * 8 + (7ull - returnValue.captured.x));

    board.holder.chessPieces[returnValue.captured.isWhite][(int) returnValue.captured.piece][board.holder.limitCap[returnValue.captured.isWhite][(int) returnValue.captured.piece]] =  returnValue.captured;
    board.holder.limitCap[returnValue.captured.isWhite][(int) returnValue.captured.piece]++;
    BasicChessPiece &it = board.holder.chessPieces[move.isWhite][(int) move.piece][i];
    it.piece = returnValue.piece;
    it.x = move.x1;
    it.y = move.y1;

    board.advancedTwoPawnMove[move.isWhite][move.x1] = returnValue.wasAdvancedTwo;
    board.firstRookKingMove[move.isWhite] = returnValue.wasFirstMove;

    board.pinnedPieces[move.isWhite] = returnValue.pinnedPieces;
    board.attackPattern[move.isWhite] = returnValue.attackPattern;
    board.generalAttackPattern[move.isWhite] = returnValue.generalAttackPattern;
}

ll maxDepth = 1;
void MoveGenerator::recursiveMoveFunction(ll depth, bool whiteTurn) {

    if (depth > maxDepth) return;

    for (ll it = 0; it < board.holder.limitCap[whiteTurn][(int) PieceType::ROOK]; it++) {
        ll firstRef = -1, nextLevel = -1;
        short x = board.holder.chessPieces[whiteTurn][(int) PieceType::ROOK][it].x, y = board.holder.chessPieces[whiteTurn][(int) PieceType::ROOK][it].y;
        ull chessPieceBoard = (1ull << ((7ull - y) * 8ull + (7ull - x)));
        if ((board.pinnedPieces[whiteTurn] & chessPieceBoard) > 0) continue;
        auto moves = rook.getMove(x, y, board.totalBoard[whiteTurn], board.totalBoard[!whiteTurn]);
        for (ll i = 0; i < moves.size(); i++) {
            for (ll j = 0; j < moves[i]->size(); j++) {

                ll nextSpot;
                if (i == 0 && j == 0) firstRef = arena.currentFreeIndex();

                if (i < moves.size() - 1 || j < moves.size() - 1) nextSpot = arena.nextFreeIndex();
                else {
                    nextSpot = -1;
                    if (depth < maxDepth) nextLevel = arena.nextFreeIndex();
                }

                BitboardMove parameter = {x, y, (*moves[i])[j].x, (*moves[i])[j].y,
                                          board.boardStates[whiteTurn][(int) PieceType::ROOK],
                                          (*moves[i])[j].legalMovesBitboard, PieceType::ROOK, whiteTurn, nextSpot, -1,
                                          0};
                ReturnValueDoMove returnValue = doMove(parameter, it);
                parameter.evaluation = getEvaluation();
                arena.addMove(parameter);
                recursiveMoveFunction(depth + 1, !whiteTurn);
                undoMove(parameter, returnValue, it);
            }
        }


        while(firstRef > -1) {
            BitboardMove moved = arena.getMove(firstRef);
            moved.nextMoveLevel = nextLevel;
            firstRef = moved.currentLevel;
        }
    }

    for (ll it = 0; it < board.holder.limitCap[whiteTurn][(int) PieceType::KNIGHT]; it++) {
        ll firstRef = -1, nextLevel = -1;
        short x = board.holder.chessPieces[whiteTurn][(int) PieceType::KNIGHT][it].x, y = board.holder.chessPieces[whiteTurn][(int) PieceType::KNIGHT][it].y;
        ull chessPieceBoard = (1ull << ((7ull - y) * 8ull + (7ull - x)));
        if ((board.pinnedPieces[whiteTurn] & chessPieceBoard) > 0) continue;
        auto moves = knight.getMove(x, y, board.totalBoard[whiteTurn], board.totalBoard[!whiteTurn]);
        for (ll i = 0; i < moves.first->size(); i++) {

            ll nextSpot;
            if (i == 0) firstRef = arena.currentFreeIndex();

            if (i < moves.first->size() - 1) nextSpot = arena.nextFreeIndex();
            else {
                nextSpot = -1;
                if (depth < maxDepth) nextLevel = arena.nextFreeIndex();
            }

            BitboardMove parameter = {x, y, (*moves.first)[i].x, (*moves.first)[i].y,
                                      board.boardStates[whiteTurn][(int) PieceType::KNIGHT],
                                      (*moves.first)[i].legalMovesBitboard, PieceType::KNIGHT, whiteTurn, nextSpot, -1,
                                      0};
            ReturnValueDoMove returnValue = doMove(parameter, it);
            parameter.evaluation = getEvaluation();
            arena.addMove(parameter);
            recursiveMoveFunction(depth + 1, !whiteTurn);
            undoMove(parameter, returnValue, it);
        }

        for (ll i = 0; i < moves.second->size(); i++) {

            ll nextSpot;
            if (i == 0) firstRef = arena.currentFreeIndex();

            if (i < moves.second->size() - 1) nextSpot = arena.nextFreeIndex();
            else {
                nextSpot = -1;
                if (depth < maxDepth) nextLevel = arena.nextFreeIndex();
            }

            BitboardMove parameter = {x, y, (*moves.second)[i].x, (*moves.second)[i].y,
                                      board.boardStates[whiteTurn][(int) PieceType::KNIGHT],
                                      (*moves.second)[i].legalMovesBitboard, PieceType::KNIGHT, whiteTurn, nextSpot, -1,
                                      0};
            ReturnValueDoMove returnValue = doMove(parameter, it);
            parameter.evaluation = getEvaluation();
            arena.addMove(parameter);
            recursiveMoveFunction(depth + 1, !whiteTurn);
            undoMove(parameter, returnValue, it);
        }


        while(firstRef > -1) {
            BitboardMove moved = arena.getMove(firstRef);
            moved.nextMoveLevel = nextLevel;
            firstRef = moved.currentLevel;
        }
    }

    for (ll it = 0; it < board.holder.limitCap[whiteTurn][(int) PieceType::BISHOP]; it++) {
        ll firstRef = -1, nextLevel = -1;
        short x = board.holder.chessPieces[whiteTurn][(int) PieceType::BISHOP][it].x, y = board.holder.chessPieces[whiteTurn][(int) PieceType::BISHOP][it].y;
        ull chessPieceBoard = (1ull << ((7ull - y) * 8ull + (7ull - x)));
        if ((board.pinnedPieces[whiteTurn] & chessPieceBoard) > 0) continue;
        auto moves = bishop.getMove(x, y, board.totalBoard[whiteTurn], board.totalBoard[!whiteTurn]);

        for (ll i = 0; i < moves.size(); i++) {
            for (ll j = 0; j < moves[i]->size(); j++) {

                ll nextSpot;
                if (i == 0 && j == 0) firstRef = arena.currentFreeIndex();

                if (i < moves.size() - 1 || j < moves.size() - 1) nextSpot = arena.nextFreeIndex();
                else {
                    nextSpot = -1;
                    if (depth < maxDepth) nextLevel = arena.nextFreeIndex();
                }

                BitboardMove parameter = {x, y, (*moves[i])[j].x, (*moves[i])[j].y,
                                          board.boardStates[whiteTurn][(int) PieceType::BISHOP],
                                          (*moves[i])[j].legalMovesBitboard, PieceType::BISHOP, whiteTurn, nextSpot, -1,
                                          0};
                ReturnValueDoMove returnValue = doMove(parameter, it);
                parameter.evaluation = getEvaluation();
                arena.addMove(parameter);
                recursiveMoveFunction(depth + 1, !whiteTurn);
                undoMove(parameter, returnValue, it);
            }
        }


        while(firstRef > -1) {
            BitboardMove moved = arena.getMove(firstRef);
            moved.nextMoveLevel = nextLevel;
            firstRef = moved.currentLevel;
        }
    }
    for (ll it = 0; it < board.holder.limitCap[whiteTurn][(int) PieceType::QUEEN]; it++) {
        ll firstRef = -1, nextLevel = -1;
        short x = board.holder.chessPieces[whiteTurn][(int) PieceType::QUEEN][it].x, y = board.holder.chessPieces[whiteTurn][(int) PieceType::QUEEN][it].y;
        ull chessPieceBoard = (1ull << ((7ull - y) * 8ull + (7ull - x)));
        if ((board.pinnedPieces[whiteTurn] & chessPieceBoard) > 0) continue;
        auto moves = queen.getMove(bishop, rook, x, y, board.totalBoard[whiteTurn],
                                   board.totalBoard[!whiteTurn]);
        for (ll i = 0; i < moves.first.size(); i++) {
            for (ll j = 0; j < moves.first[i]->size(); j++) {

                ll nextSpot;
                if (i == 0 && j == 0) firstRef = arena.currentFreeIndex();

                if (i < moves.first.size() - 1 || j < moves.first.size() - 1) nextSpot = arena.nextFreeIndex();
                else {
                    nextSpot = -1;
                    if (depth < maxDepth) nextLevel = arena.nextFreeIndex();
                }

                BitboardMove parameter = {x, y, (*moves.first[i])[j].x, (*moves.first[i])[j].y,
                                          board.boardStates[whiteTurn][(int) PieceType::QUEEN],
                                          (*moves.first[i])[j].legalMovesBitboard, PieceType::QUEEN, whiteTurn,
                                          nextSpot, -1, 0};
                ReturnValueDoMove returnValue = doMove(parameter, it);
                parameter.evaluation = getEvaluation();
                arena.addMove(parameter);
                recursiveMoveFunction(depth + 1, !whiteTurn);
                undoMove(parameter, returnValue, it);
            }
        }

        for (ll i = 0; i < moves.second.size(); i++) {
            for (ll j = 0; j < moves.second[i]->size(); j++) {

                ll nextSpot;
                if (i == 0 && j == 0) firstRef = arena.currentFreeIndex();

                if (i < moves.second.size() - 1 || j < moves.second.size() - 1) nextSpot = arena.nextFreeIndex();
                else {
                    nextSpot = -1;
                    if (depth < maxDepth) nextLevel = arena.nextFreeIndex();
                }

                BitboardMove parameter = {x, y, (*moves.second[i])[j].x, (*moves.second[i])[j].y,
                                          board.boardStates[whiteTurn][(int) PieceType::QUEEN],
                                          (*moves.second[i])[j].legalMovesBitboard, PieceType::QUEEN, whiteTurn,
                                          nextSpot, -1, 0};
                ReturnValueDoMove returnValue = doMove(parameter, it);
                parameter.evaluation = getEvaluation();
                arena.addMove(parameter);
                recursiveMoveFunction(depth + 1, !whiteTurn);
                undoMove(parameter, returnValue, it);
            }
        }


        while(firstRef > -1) {
            BitboardMove moved = arena.getMove(firstRef);
            moved.nextMoveLevel = nextLevel;
            firstRef = moved.currentLevel;
        }
    }
    for (ll it = 0; it < board.holder.limitCap[whiteTurn][(int) PieceType::KING]; it++) {
        ll firstRef = -1, nextLevel = -1;
        short x = board.holder.chessPieces[whiteTurn][(int) PieceType::KING][it].x, y = board.holder.chessPieces[whiteTurn][(int) PieceType::KING][it].y;
        ull chessPieceBoard = (1ull << ((7ull - y) * 8ull + (7ull - x)));
        if ((board.pinnedPieces[whiteTurn] & chessPieceBoard) > 0) continue;
        auto moves = king.getMove(x, y, board.totalBoard[whiteTurn], board.totalBoard[!whiteTurn]);

        for (ll i = 0; i < moves.first->size(); i++) {

            ll nextSpot;
            if (i == 0) firstRef = arena.currentFreeIndex();

            if (i < moves.first->size() - 1) nextSpot = arena.nextFreeIndex();
            else {
                nextSpot = -1;
                if (depth < maxDepth) nextLevel = arena.nextFreeIndex();
            }

            BitboardMove parameter = {x, y, (*moves.first)[i].x, (*moves.first)[i].y,
                                      board.boardStates[whiteTurn][(int) PieceType::KING],
                                      (*moves.first)[i].legalMovesBitboard, PieceType::KING, whiteTurn, nextSpot, -1,
                                      0};
            ReturnValueDoMove returnValue = doMove(parameter, it);
            parameter.evaluation = getEvaluation();
            arena.addMove(parameter);
            recursiveMoveFunction(depth + 1, !whiteTurn);
            undoMove(parameter, returnValue, it);
        }

        for (ll i = 0; i < moves.second->size(); i++) {

            ll nextSpot;
            if (i == 0) firstRef = arena.currentFreeIndex();

            if (i < moves.second->size() - 1) nextSpot = arena.nextFreeIndex();
            else {
                nextSpot = -1;
                if (depth < maxDepth) nextLevel = arena.nextFreeIndex();
            }

            BitboardMove parameter = {x, y, (*moves.second)[i].x, (*moves.second)[i].y,
                                      board.boardStates[whiteTurn][(int) PieceType::KING],
                                      (*moves.second)[i].legalMovesBitboard, PieceType::KING, whiteTurn, nextSpot, -1,
                                      0};
            ReturnValueDoMove returnValue = doMove(parameter, it);
            parameter.evaluation = getEvaluation();
            arena.addMove(parameter);
            recursiveMoveFunction(depth + 1, !whiteTurn);
            undoMove(parameter, returnValue, it);
        }

        while(firstRef > -1) {
            BitboardMove moved = arena.getMove(firstRef);
            moved.nextMoveLevel = nextLevel;
            firstRef = moved.currentLevel;
        }
    }

    for (ll it = 0; it < board.holder.limitCap[whiteTurn][(int) PieceType::KING]; it++) {
        ll firstRef = -1, nextLevel = -1;
        short x = board.holder.chessPieces[whiteTurn][(int) PieceType::KING][it].x, y = board.holder.chessPieces[whiteTurn][(int) PieceType::KING][it].y;
        ull chessPieceBoard = (1ull << ((7ull - y) * 8ull + (7ull - x)));
        if ((board.pinnedPieces[whiteTurn] & chessPieceBoard) > 0) continue;

        vector<vector<LegalMovesStore>*> moves;
        if(whiteTurn) {
             moves = whitePawn.getMove(x, y, board.totalBoard[whiteTurn], board.totalBoard[!whiteTurn], board.boardStates[!whiteTurn][(int) PieceType::PAWN]);
        } else {
            moves = blackPawn.getMove(x, y, board.totalBoard[whiteTurn], board.totalBoard[!whiteTurn], board.boardStates[!whiteTurn][(int) PieceType::PAWN]);
        }

        for(ll i = 0; i < moves.size(); i++) {
            for(ll j = 0; j < moves[i]->size(); j++) {

                ll nextSpot;
                if(i == 0 && j == 0) firstRef = arena.currentFreeIndex();

                if(i < moves.size() - 1 || j < moves.size() - 1) nextSpot = arena.nextFreeIndex();
                else {
                    nextSpot = -1;
                    if(depth < maxDepth) nextLevel = arena.nextFreeIndex();
                }

                BitboardMove parameter = {x, y, (*moves[i])[j].x, (*moves[i])[j].y, board.boardStates[whiteTurn][(int) PieceType::PAWN], (*moves[i])[j].legalMovesBitboard, PieceType::PAWN, whiteTurn, nextSpot, -1, 0};
                if((whiteTurn && y == 6) || (!whiteTurn && y == 1)) {
                    for(int k = (int) PieceType::ROOK; k < (int) PieceType::QUEEN; k++) {
                        parameter.piece = (PieceType) k;

                        ReturnValueDoMove returnValue = doMove(parameter, it);
                        parameter.evaluation = getEvaluation();
                        arena.addMove(parameter);
                        recursiveMoveFunction(depth + 1, !whiteTurn);
                        undoMove(parameter, returnValue, it);
                    }
                } else {
                    ReturnValueDoMove returnValue = doMove(parameter, it);
                    parameter.evaluation = getEvaluation();
                    arena.addMove(parameter);
                    recursiveMoveFunction(depth + 1, !whiteTurn);
                    undoMove(parameter, returnValue, it);
                }
            }
        }

        while(firstRef > -1) {
            BitboardMove moved = arena.getMove(firstRef);
            moved.nextMoveLevel = nextLevel;
            firstRef = moved.currentLevel;
        }
    }
}

ll pieceValueDebug[] = {5, 3, 3, 1, 9, 0, 0};
ll MoveGenerator::getEvaluation() {
    ll score = 0;
    for(ll i = 0; i < 6; i++) {
        for(ll j = 0; j < board.holder.limitCap[board.playerWhite][i]; j++) {
            short x = board.holder.chessPieces[board.playerWhite][i][j].x, y = board.holder.chessPieces[board.playerWhite][i][j].y;
            score -= pieceValueDebug[i];
            score -= min(min((ll) x, 7ll - ((ll) x)), min((ll) y, 7ll - ((ll) y)));
        }

        for(ll j = 0; j < board.holder.limitCap[!board.playerWhite][i]; j++) {
            short x = board.holder.chessPieces[board.playerWhite][i][j].x, y = board.holder.chessPieces[board.playerWhite][i][j].y;
            score += pieceValueDebug[i];
            score += min(min((ll) x, 7ll - ((ll) x)), min((ll) y, 7ll - ((ll) y)));
        }
    }

    return score;
}

ll Arena::currentFreeIndex() const {
    return startIndex;
}

ll Arena::nextFreeIndex() const {
    return startIndex + 1;
}

void MoveGenerator::printChar(short i, short j) {

    ull currentPos = 1ull << ((7ull - i) * 8ull + (7ull - j));
    if((currentPos & board.boardStates[1][(int) PieceType::ROOK]) > 0) {
        cout << "♖";
    } else if((currentPos & board.boardStates[0][(int) PieceType::ROOK]) > 0) {
        cout << "♜";
    } else if((currentPos & board.boardStates[1][(int) PieceType::KNIGHT]) > 0) {
        cout << "♘";
    } else if((currentPos & board.boardStates[0][(int) PieceType::KNIGHT]) > 0) {
        cout << "♞";
    } else if((currentPos & board.boardStates[1][(int) PieceType::BISHOP]) > 0) {
        cout << "♗";
    } else if((currentPos & board.boardStates[0][(int) PieceType::BISHOP]) > 0) {
        cout << "♝";
    } else if((currentPos & board.boardStates[1][(int) PieceType::QUEEN]) > 0) {
        cout << "♕";
    } else if((currentPos & board.boardStates[0][(int) PieceType::QUEEN]) > 0) {
        cout << "♛";
    } else if((currentPos & board.boardStates[1][(int) PieceType::PAWN]) > 0) {
        cout << "♙";
    } else if((currentPos & board.boardStates[0][(int) PieceType::PAWN]) > 0) {
        cout << "♟";
    }  else if((currentPos & board.boardStates[1][(int) PieceType::KING]) > 0) {
        cout << "♔";
    } else if((currentPos & board.boardStates[0][(int) PieceType::KING]) > 0) {
        cout << "♚";
    } else {
        if(i % 2 == 0 ^ j % 2 == 0) {
            cout << "■";
        } else {
            cout << "□";
        }
    }
}

void MoveGenerator::printBoard() {
    if(board.playerWhite) {
        for (ll i = 7; i >= 0; i--) {
            cout << i + 1 << " | ";
            for (ll j = 0; j < 8; j++) {
                printChar(i, j);
                cout << " ";
            }
            cout << endl;
        }
    } else {
        for (ll i = 0; i < 8; i++) {
            cout << i + 1 << " | ";
            for (ll j = 0; j < 8; j++) {
                printChar(i, j);
                cout << " ";
            }
            cout << endl;
        }
    }

    cout << "    ";
    for(ll i = 0; i <= 7; i++) {
        cout << (char) ('A' + i) << " ";
    }
    cout << endl;
}

void MoveGenerator::loadFile(string fileName) {
    ifstream inputStream(fileName, ios::binary);
    if(!inputStream) {
        cerr << "Can't open file " + fileName << endl;
        exit(1);
    }

    stringstream buffer;
    buffer << inputStream.rdbuf();
    inputStream.close();
    string file_content = buffer.str();
    board.playerWhite = file_content[0] == 'w';

    for(ll j = 0; j < 2; j++) {
        board.firstRookKingMove[j] = file_content[1 + j] == 't';
    }

    for(ll j = 0; j < 2; j++) {
        for (ll k = 0; k < 8; k++) {
            board.advancedTwoPawnMove[j][k] = file_content[3 + j * 8 + k] == 't';
        }
    }

    for(ll j = 0; j < 6; j++) {
        board.holder.limitCap[0][j] = 0;
        board.holder.limitCap[1][j] = 0;
    }

    for(ll i = 0; i < 2; i++) {
        board.totalBoard[i] = 0;
        for(ll j = 0; j < 6; j++) {
            board.boardStates[i][j] = 0;
        }
    }

    ll i = 20;
    while(i < file_content.size()) {

        bool isWhite = file_content[i] == 'w';
        char pieceT = file_content[i + 1];
        ll x = file_content[i + 2] - '0';
        ll y = file_content[i + 3] - '0';

        ull bitboardPosition = (1ull << ((7ull - y) * 8 + (7ull - x)));
        board.totalBoard[isWhite] |= bitboardPosition;

        PieceType pieceType;
        switch (pieceT) {
            case 'r': //rook
                pieceType = PieceType::ROOK;
                break;
            case 'n': //knight
                pieceType = PieceType::KNIGHT;
                break;
            case 'b': //bishop
                pieceType = PieceType::BISHOP;
                break;
            case 'q': //queen
                pieceType = PieceType::QUEEN;
                break;
            case 'k': //king
                pieceType = PieceType::KING;
                break;
            default: //pawn
                pieceType = PieceType::PAWN;
                break;
        }

        board.boardStates[isWhite][(int) pieceType] |= bitboardPosition;
        board.holder.chessPieces[isWhite][(int) pieceType][board.holder.limitCap[isWhite][(int) pieceType]] = {(short) x,(short) y, pieceType, isWhite};
        board.holder.limitCap[isWhite][(int) pieceType]++;

        i += 5;
    }

    printBoard();
}

void MoveGenerator::saveFile(string fileName) {
    ofstream writer(fileName);
    if(board.playerWhite) {
        writer << "w";
    } else {
        writer << "b";
    }

    for(ll j = 0; j < 2; j++) {
        if(board.firstRookKingMove[j]) {
            writer << "t";
        } else {
            writer << "f";
        }
    }

    for(ll j = 0; j < 2; j++) {
        for (ll k = 0; k < 8; k++) {
            if(board.advancedTwoPawnMove[j][k]) {
                writer << "t";
            } else {
                writer << "f";
            }
        }
    }

    writer << endl;

    for(ll i = 0; i < 6; i++) {
        for (ll j = 0; j < board.holder.limitCap[board.playerWhite][i]; j++) {
            printOut(writer, board.holder.chessPieces[board.playerWhite][i][j]);
        }

        for (ll j = 0; j < board.holder.limitCap[!board.playerWhite][i]; j++) {
            printOut(writer, board.holder.chessPieces[!board.playerWhite][i][j]);
        }
    }
}

void printOut(ofstream &writer, BasicChessPiece &i) {
    if (i.isWhite) {
        writer << "b";
    } else {
        writer << "w";
    }

    switch (i.piece) {
        case PieceType::ROOK: //rook
            writer << "r";
            break;
        case PieceType::KNIGHT: //knight
            writer << "n";
            break;
        case PieceType::BISHOP: //bishop
            writer << "b";
            break;
        case PieceType::QUEEN: //queen
            writer << "q";
            break;
        case PieceType::KING: //king
            writer << "k";
            break;
        default: //pawn
            writer << "p";
            break;
    }

    writer << i.x << i.y << endl;
}

ll getMemoryUsage() {
    task_basic_info info;
    mach_msg_type_number_t infoCount = TASK_BASIC_INFO_COUNT;
    if (task_info(mach_task_self(), TASK_BASIC_INFO, (task_info_t)&info, &infoCount) != KERN_SUCCESS) {
        return 0;
    }
    return info.resident_size; // bytes
}

bool notInRange(int num) {
    return num < 0 || num > 7;
}

BitboardMove MoveGenerator::makeMoveFromChords(int x1, int y1, int x2, int y2, PieceType promotion) {
    BitboardMove move;
    move.isWhite = board.playerWhite;
    move.x1 = x1;
    move.y1 = y1;
    move.x2 = x2;
    move.y2 = y2;
    move.piece = promotion;

    move.startBoard = ((1ull << ((7ull - y1) * 8 + (7ull - x1))));
    move.endBoard = ((1ull << ((7ull - y2) * 8 + (7ull - x2))));
    return move;
}

void MoveGenerator::playGame() {

    cout << "White or Black (w/b)? ";
    string ans; cin >> ans;
    if(ans[0] == 'b') {
        loadFile("StartingPositionBlack.txt");
    } else if(ans[0] == 'l'){
        string name = ans.substr(2, ans.size() - 2);
        loadFile(name);
    } else {
        loadFile("StartingPositionWhite.txt");
    }

    while(true) {
        cout << "Input Move (Ex: a2 a4): ";
        string move;
        while(move.empty()) { getline(cin, move); }
        if(move[0] == 's') {
            string name = move.substr(2, move.size() - 2);
            saveFile(name);
            continue;
        } else if(move[0] == 'l') {
            string name = move.substr(2, move.size() - 2);
            loadFile(name);
            continue;
        } else if(move[0] == 'u') {
            loadFile("LastMove.txt");
            continue;
        } else if(move[0] == 'm') {
            cout << "Memory used: " << getMemoryUsage() / 1000000.0 << " MB" << endl;
            continue;
        } else if(move[0] == 'q') {
            return;
        } else {
            saveFile("LastMove.txt");
        }

        if(move.size() != 5) {
            cout << "Invalid input format\n";
            continue;
        }

        int x1 = move[0] - 'a';
        int y1 = move[1] - '1';
        int x2 = move[3] - 'a';
        int y2 = move[4] - '1';

        if(notInRange(x1) || notInRange(y1) || notInRange(x2) || notInRange(y2)) {
            cout << "Invalid input format\n";
            continue;
        }

        PieceType pieceType = PieceType::NONE;
        if(y2 == (board.playerWhite ? 7 : 0) && (board.boardStates[board.playerWhite][(int) PieceType::PAWN] & (1ull << ((7ull - y1) * (8 + 7ull - x1)))) > 0) {
            while(true) {
                cout << "Promote to (queen, rook, bishop, or knight)? ";
                string answer; cin >> answer;
                if (answer[0] == 'q') {
                    pieceType = PieceType::QUEEN;
                    break;
                } else if (answer[0] == 'r') {
                    pieceType = PieceType::ROOK;
                    break;
                } else if (answer[0] == 'b') {
                    pieceType = PieceType::BISHOP;
                    break;
                } else if (answer[0] == 'k') {
                    pieceType = PieceType::KNIGHT;
                    break;
                } else {
                    cout << "Invalid input" << endl;
                }
            }
        } else {
            for(ll i = 0; i < 6; i++) {
                for(ll j = 0; j < board.holder.limitCap[board.playerWhite][i]; j++) {
                    if(board.holder.chessPieces[board.playerWhite][i][j].x == x1 && board.holder.chessPieces[board.playerWhite][i][j].y == y1) {
                        pieceType = board.holder.chessPieces[board.playerWhite][i][j].piece;
                    }
                }
            }
        }

        if(pieceType == PieceType::NONE) {
            cout << "Invalid input" << endl;
            continue;
        }

        BitboardMove playerMove = makeMoveFromChords(x1, y1, x2, y2, pieceType);
        recursiveMoveFunction(1, board.playerWhite);
        if(!arena.advanceMove(playerMove)) {
            cout << "Not a legal move\n";
            continue;
        }

        for(ll i = 0; i < 6; i++) {
            for(ll j = 0; j < board.holder.limitCap[board.playerWhite][i]; j++) {
                if(board.holder.chessPieces[board.playerWhite][i][j].x == playerMove.x1 && board.holder.chessPieces[board.playerWhite][i][j].y == playerMove.y1) {
                    doMove(playerMove, j);
                }
            }
        }

        printBoard();
        cout << "\nResult of your move. Evaluation: " << arena.getCurrentEvaluation() << "\n\n";

        if(arena.hasNoMove()) {
            if((board.attackPattern[!board.playerWhite] & board.boardStates[board.playerWhite][(int) PieceType::KING]) > 0) {
                cout << R"(You won! You're very smart! 🔥 😎)";
                return;
            } else {
                cout << R"(Stalemate. At least you didn't lose...)";
                return;
            }
        }

        recursiveMoveFunction(1, !board.playerWhite);
        BitboardMove optimalMove = arena.findOptimalMove();
        for(ll i = 0; i < 6; i++) {
            for(ll j = 0; j < board.holder.limitCap[!board.playerWhite][i]; j++) {
                if(board.holder.chessPieces[!board.playerWhite][i][j].x == optimalMove.x1 && board.holder.chessPieces[!board.playerWhite][i][j].y == optimalMove.y1) {
                    doMove(optimalMove, j);
                }
            }
        }

        if(arena.hasNoMove()) {

            if((board.attackPattern[board.playerWhite] & board.boardStates[!board.playerWhite][(int) PieceType::KING]) > 0) {
                cout << R"(GG 🧌)";
                return;
            } else {
                cout << R"(Stalemate. At least you didn't win...)";
                return;
            }
        }

        printBoard();
        cout << "\nResult of computer move. Evaluation: " << arena.getCurrentEvaluation() << "\n\n";
    }
}

Arena::Arena() {
    moveStorage = new vector<BitboardMove>(size);
}

void Arena::addMove(BitboardMove &move) {

    if(startIndex == size) {
        cout << "Error, ran out of storage allocated for moves";
        abort();
    }

    (*moveStorage)[startIndex] = move;
    startIndex++;
}

ll Arena::getCurrentEvaluation() {
    return 0; //tbd
    //return findOptimalMove().evaluation;
}

BitboardMove Arena::findOptimalMove() {
    ll index = 0;
    ll maxEval = LLONG_MIN, indexMaxEval = -1;
    while(index > -1) {

        if((*moveStorage)[index].evaluation > maxEval) {
            maxEval = (*moveStorage)[index].evaluation;
            indexMaxEval = index;
        }

        index = (*moveStorage)[index].currentLevel;
    }

    return (*moveStorage)[indexMaxEval];
}

bool Arena::advanceMove(BitboardMove &move) {
    ll index = startIndex;
    while(index > -1) {
        if((*moveStorage)[index] == move) {
            startIndex = 0;
            return true;
        }
        index = (*moveStorage)[index].currentLevel;
    }

    return false;
}

bool Arena::hasNoMove() {
    return (*moveStorage)[startIndex].nextMoveLevel == -1;
}

BitboardMove& Arena::getMove(ll index) {
    return (*moveStorage)[index];
}

