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

void MoveGenerator::recursiveMoveFunction(ll depth, bool whiteTurn) {
    for(auto it = board.chessPieces.begin(); it != board.chessPieces.end(); it++) {
        if(it->isWhite != whiteTurn) continue;
        switch (it->piece) {
            case PieceType::ROOK: {
                auto moves = rook.getMove(it->x, it->y, board.totalBoard[whiteTurn], board.totalBoard[!whiteTurn]);
                break;
            }
            case PieceType::KNIGHT: {
                auto moves = knight.getMove(it->x, it->y, board.totalBoard[whiteTurn], board.totalBoard[!whiteTurn]);
                break;
            }
            case PieceType::BISHOP: {
                auto moves = bishop.getMove(it->x, it->y, board.totalBoard[whiteTurn], board.totalBoard[!whiteTurn]);
                break;
            }
            case PieceType::QUEEN: {
                auto moves = queen.getMove(bishop, rook, it->x, it->y, board.totalBoard[whiteTurn], board.totalBoard[!whiteTurn]);
                break;
            }
            case PieceType::KING: {
                auto moves = king.getMove(it->x, it->y, board.totalBoard[whiteTurn], board.totalBoard[!whiteTurn]);
                break;
            }
            default: { //Pawn

                vector<vector<LegalMovesStore>*> moves;
                if(it->isWhite) {
                     moves = whitePawn.getMove(it->x, it->y, board.totalBoard[whiteTurn], board.totalBoard[!whiteTurn], board.boardStates[!whiteTurn][(int) PieceType::PAWN]);
                } else {
                    moves = blackPawn.getMove(it->x, it->y, board.totalBoard[whiteTurn], board.totalBoard[!whiteTurn], board.boardStates[!whiteTurn][(int) PieceType::PAWN]);
                }
                break;
            }
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

Arena::Arena() {
    moveStorage = new vector<BitboardMove>(size);

    LinkedListNode *current = &head;
    for(ll i = 0; i < size; i++) {
        current->freedMemory = i;
        if(i < size - 1) {
            current->nextSpot = new LinkedListNode;
            current = current->nextSpot;
        } else
            current->nextSpot = nullptr;

    }
}