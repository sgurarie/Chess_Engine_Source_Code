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

ReturnValueDoMove MoveGenerator::doMove(BitboardMove &move) {

    ReturnValueDoMove returnValue;

    auto it = board.chessPieces.find({move.x2, move.y2, move.piece, !move.isWhite});
    if(it == board.chessPieces.end()) returnValue.captured.piece = PieceType::NONE;
    else {
        returnValue.captured = *it;
        board.chessPieces.erase(it);
    }

    BasicChessPiece original = *board.chessPieces.find({move.x1, move.y1, move.piece, move.isWhite});
    board.chessPieces.erase(original);
    original.y = move.y2;
    original.x = move.x2;
    board.chessPieces.insert(original);

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



void MoveGenerator::undoMove(BitboardMove &move, ReturnValueDoMove &returnValue) {
    board.boardStates[move.isWhite][(int) move.piece] ^= move.endBoard;
    if(returnValue.captured.piece == PieceType::NONE)
        board.boardStates[move.isWhite][(int) move.piece] ^= move.startBoard;
    else
        board.boardStates[move.isWhite][(int) returnValue.captured.piece] ^= 1ull << ((7ull - returnValue.captured.y) * 8 + (7ull - returnValue.captured.x));

    board.chessPieces.insert(returnValue.captured);
    board.chessPieces.insert({move.x1, move.y1, move.piece, move.isWhite});

    board.advancedTwoPawnMove[move.isWhite][move.x1] = returnValue.wasAdvancedTwo;
    board.firstRookKingMove[move.isWhite] = returnValue.wasFirstMove;

    board.pinnedPieces[move.isWhite] = returnValue.pinnedPieces;
    board.attackPattern[move.isWhite] = returnValue.attackPattern;
    board.generalAttackPattern[move.isWhite] = returnValue.generalAttackPattern;
}

ll maxDepth = 1;
void MoveGenerator::recursiveMoveFunction(ll depth, bool whiteTurn) {

    if(depth > maxDepth) return;

    for(auto it = board.chessPieces.begin(); it != board.chessPieces.end(); it++) {
        if(it->isWhite != whiteTurn) continue;
        ll firstRef = -1, nextLevel = -1;
        switch (it->piece) {
            case PieceType::ROOK: {
                auto moves = rook.getMove(it->x, it->y, board.totalBoard[whiteTurn], board.totalBoard[!whiteTurn]);
                for(ll i = 0; i < moves.size(); i++) {
                    for(ll j = 0; j < moves[i]->size(); j++) {

                        ll nextSpot;
                        if(i == 0 && j == 0) firstRef = arena.currentFreeIndex();

                        if(i < moves.size() - 1 || j < moves.size() - 1) nextSpot = arena.nextFreeIndex();
                        else {
                            nextSpot = -1;
                            if(depth < maxDepth) nextLevel = arena.nextFreeIndex();
                        }

                        BitboardMove parameter = {it->x, it->y, (*moves[i])[j].x, (*moves[i])[j].y, board.boardStates[whiteTurn][(int) PieceType::ROOK], (*moves[i])[j].legalMovesBitboard, PieceType::ROOK, whiteTurn, nextSpot, -1, 0};
                        ReturnValueDoMove returnValue = doMove(parameter);
                        //eval
                        recursiveMoveFunction(depth + 1, !whiteTurn);
                        undoMove(parameter, returnValue);
                    }
                }
                break;
            }
            case PieceType::KNIGHT: {
                auto moves = knight.getMove(it->x, it->y, board.totalBoard[whiteTurn], board.totalBoard[!whiteTurn]);
                for(ll i = 0; i < moves.first->size(); i++) {

                    ll nextSpot;
                    if(i == 0) firstRef = arena.currentFreeIndex();

                    if(i < moves.first->size() - 1) nextSpot = arena.nextFreeIndex();
                    else {
                        nextSpot = -1;
                        if(depth < maxDepth) nextLevel = arena.nextFreeIndex();
                    }

                    BitboardMove parameter = {it->x, it->y, (*moves.first)[i].x, (*moves.first)[i].y, board.boardStates[whiteTurn][(int) PieceType::KNIGHT], (*moves.first)[i].legalMovesBitboard, PieceType::KNIGHT, whiteTurn, nextSpot, -1, 0};
                    ReturnValueDoMove returnValue = doMove(parameter);
                    //eval
                    recursiveMoveFunction(depth + 1, !whiteTurn);
                    undoMove(parameter, returnValue);
                }

                for(ll i = 0; i < moves.second->size(); i++) {

                    ll nextSpot;
                    if(i == 0) firstRef = arena.currentFreeIndex();

                    if(i < moves.second->size() - 1) nextSpot = arena.nextFreeIndex();
                    else {
                        nextSpot = -1;
                        if(depth < maxDepth) nextLevel = arena.nextFreeIndex();
                    }

                    BitboardMove parameter = {it->x, it->y, (*moves.second)[i].x, (*moves.second)[i].y, board.boardStates[whiteTurn][(int) PieceType::KNIGHT], (*moves.second)[i].legalMovesBitboard, PieceType::KNIGHT, whiteTurn, nextSpot, -1, 0};
                    ReturnValueDoMove returnValue = doMove(parameter);
                    //eval
                    recursiveMoveFunction(depth + 1, !whiteTurn);
                    undoMove(parameter, returnValue);
                }
                break;
            }
            case PieceType::BISHOP: {
                auto moves = bishop.getMove(it->x, it->y, board.totalBoard[whiteTurn], board.totalBoard[!whiteTurn]);

                for(ll i = 0; i < moves.size(); i++) {
                    for(ll j = 0; j < moves[i]->size(); j++) {

                        ll nextSpot;
                        if(i == 0 && j == 0) firstRef = arena.currentFreeIndex();

                        if(i < moves.size() - 1 || j < moves.size() - 1) nextSpot = arena.nextFreeIndex();
                        else {
                            nextSpot = -1;
                            if(depth < maxDepth) nextLevel = arena.nextFreeIndex();
                        }

                        BitboardMove parameter = {it->x, it->y, (*moves[i])[j].x, (*moves[i])[j].y, board.boardStates[whiteTurn][(int) PieceType::BISHOP], (*moves[i])[j].legalMovesBitboard, PieceType::BISHOP, whiteTurn, nextSpot, -1, 0};
                        ReturnValueDoMove returnValue = doMove(parameter);
                        //eval
                        recursiveMoveFunction(depth + 1, !whiteTurn);
                        undoMove(parameter, returnValue);
                    }
                }
                break;
            }
            case PieceType::QUEEN: {
                auto moves = queen.getMove(bishop, rook, it->x, it->y, board.totalBoard[whiteTurn], board.totalBoard[!whiteTurn]);
                for(ll i = 0; i < moves.first.size(); i++) {
                    for(ll j = 0; j < moves.first[i]->size(); j++) {

                        ll nextSpot;
                        if(i == 0 && j == 0) firstRef = arena.currentFreeIndex();

                        if(i < moves.first.size() - 1 || j < moves.first.size() - 1) nextSpot = arena.nextFreeIndex();
                        else {
                            nextSpot = -1;
                            if(depth < maxDepth) nextLevel = arena.nextFreeIndex();
                        }

                        BitboardMove parameter = {it->x, it->y, (*moves.first[i])[j].x, (*moves.first[i])[j].y, board.boardStates[whiteTurn][(int) PieceType::QUEEN], (*moves.first[i])[j].legalMovesBitboard, PieceType::QUEEN, whiteTurn, nextSpot, -1, 0};
                        ReturnValueDoMove returnValue = doMove(parameter);
                        //eval
                        recursiveMoveFunction(depth + 1, !whiteTurn);
                        undoMove(parameter, returnValue);
                    }
                }

                for(ll i = 0; i < moves.second.size(); i++) {
                    for(ll j = 0; j < moves.second[i]->size(); j++) {

                        ll nextSpot;
                        if(i == 0 && j == 0) firstRef = arena.currentFreeIndex();

                        if(i < moves.second.size() - 1 || j < moves.second.size() - 1) nextSpot = arena.nextFreeIndex();
                        else {
                            nextSpot = -1;
                            if(depth < maxDepth) nextLevel = arena.nextFreeIndex();
                        }

                        BitboardMove parameter = {it->x, it->y, (*moves.second[i])[j].x, (*moves.second[i])[j].y, board.boardStates[whiteTurn][(int) PieceType::QUEEN], (*moves.second[i])[j].legalMovesBitboard, PieceType::QUEEN, whiteTurn, nextSpot, -1, 0};
                        ReturnValueDoMove returnValue = doMove(parameter);
                        //eval
                        recursiveMoveFunction(depth + 1, !whiteTurn);
                        undoMove(parameter, returnValue);
                    }
                }
                break;
            }
            case PieceType::KING: {
                auto moves = king.getMove(it->x, it->y, board.totalBoard[whiteTurn], board.totalBoard[!whiteTurn]);

                for(ll i = 0; i < moves.first->size(); i++) {

                    ll nextSpot;
                    if(i == 0) firstRef = arena.currentFreeIndex();

                    if(i < moves.first->size() - 1) nextSpot = arena.nextFreeIndex();
                    else {
                        nextSpot = -1;
                        if(depth < maxDepth) nextLevel = arena.nextFreeIndex();
                    }

                    BitboardMove parameter = {it->x, it->y, (*moves.first)[i].x, (*moves.first)[i].y, board.boardStates[whiteTurn][(int) PieceType::KING], (*moves.first)[i].legalMovesBitboard, PieceType::KING, whiteTurn, nextSpot, -1, 0};
                    ReturnValueDoMove returnValue = doMove(parameter);
                    //eval
                    recursiveMoveFunction(depth + 1, !whiteTurn);
                    undoMove(parameter, returnValue);
                }

                for(ll i = 0; i < moves.second->size(); i++) {

                    ll nextSpot;
                    if(i == 0) firstRef = arena.currentFreeIndex();

                    if(i < moves.second->size() - 1) nextSpot = arena.nextFreeIndex();
                    else {
                        nextSpot = -1;
                        if(depth < maxDepth) nextLevel = arena.nextFreeIndex();
                    }

                    BitboardMove parameter = {it->x, it->y, (*moves.second)[i].x, (*moves.second)[i].y, board.boardStates[whiteTurn][(int) PieceType::KING], (*moves.second)[i].legalMovesBitboard, PieceType::KING, whiteTurn, nextSpot, -1, 0};
                    ReturnValueDoMove returnValue = doMove(parameter);
                    //eval
                    recursiveMoveFunction(depth + 1, !whiteTurn);
                    undoMove(parameter, returnValue);
                }
                break;
            }
            default: { //Pawn

                vector<vector<LegalMovesStore>*> moves;
                if(it->isWhite) {
                     moves = whitePawn.getMove(it->x, it->y, board.totalBoard[whiteTurn], board.totalBoard[!whiteTurn], board.boardStates[!whiteTurn][(int) PieceType::PAWN]);
                } else {
                    moves = blackPawn.getMove(it->x, it->y, board.totalBoard[whiteTurn], board.totalBoard[!whiteTurn], board.boardStates[!whiteTurn][(int) PieceType::PAWN]);
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

                        BitboardMove parameter = {it->x, it->y, (*moves[i])[j].x, (*moves[i])[j].y, board.boardStates[whiteTurn][(int) PieceType::PAWN], (*moves[i])[j].legalMovesBitboard, PieceType::PAWN, whiteTurn, nextSpot, -1, 0};
                        ReturnValueDoMove returnValue = doMove(parameter);
                        //eval
                        recursiveMoveFunction(depth + 1, !whiteTurn);
                        undoMove(parameter, returnValue);
                    }
                }
                break;
            }
        }

        while(firstRef > -1) {
            BitboardMove moved = arena.getMove(firstRef);
            moved.nextMoveLevel = nextLevel;
            firstRef = moved.currentLevel;
        }
    }
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
        for (ll i = 0; i < 8; i++) {
            cout << i + 1 << " | ";
            for (ll j = 0; j < 8; j++) {
                printChar(i, j);
                cout << " ";
            }
            cout << endl;
        }
    } else {
        for (ll i = 7; i >= 0; i--) {
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
    ll i = 2;
    while(i < file_content.size()) {

        bool isWhite = file_content[i] == 'w';
        char pieceT = file_content[i + 1];
        ll x = file_content[i + 2] - '0';
        ll y = file_content[i + 3] - '0';
        bool firstMove = file_content[i + 4] == 't';
        bool advancedTwo = file_content[i + 5] == 't';

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
    }
}

Arena::Arena() {
    moveStorage = new vector<BitboardMove>(size);

    head = new LinkedListNode;
    LinkedListNode *current = head;
    for(ll i = 0; i < size; i++) {
        current->freedMemory = i;
        if(i < size - 1) {
            current->nextSpot = new LinkedListNode;
            current = current->nextSpot;
        } else
            current->nextSpot = nullptr;

    }

    tail = current;
}

void Arena::addMove(BitboardMove &move) {
    (*moveStorage)[head->freedMemory] = move;
    LinkedListNode* nextRef = head->nextSpot;
    if(nextRef == nullptr) {
        cout << "Error, ran out of storage allocated for moves";
        abort();
    }
    delete head;
    head = nextRef;
}

void Arena::freeMoveChildren(ll index) {

    while((*moveStorage)[index].nextMoveLevel > -1) freeMoveChildren((*moveStorage)[index].nextMoveLevel);
    while((*moveStorage)[index].currentLevel > -1) freeMoveChildren((*moveStorage)[index].currentLevel);
    tail->nextSpot = new LinkedListNode;
    tail = tail->nextSpot;
    tail->freedMemory = index;
}

ll Arena::nextFreeIndex() {
    return head->nextSpot->freedMemory;
}

ll Arena::currentFreeIndex() {
    return head->freedMemory;
}

BitboardMove& Arena::getMove(ll index) {
    return (*moveStorage)[index];
}

