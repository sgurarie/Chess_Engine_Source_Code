//
// Created by Sam Gurarie on 4/5/24.
//

#include "ChessMove.h"
#include "ChessPiece.h"

void ChessMove::makeMove() {

    board[m1.y][m1.x] = nullptr;
    board[m2.y][m2.x] = &m2;

    if(captured.pieceType != ChessPiece::PieceType::NONE) {
        if (captured.isWhite == playerWhite) {
            playerPieces.erase(m1);
        } else {
            computerPieces.erase(m1);
        }
    }

    if(m1.isWhite == playerWhite) {
        playerPieces.erase(m1);
        playerPieces.insert(m2);
    } else {
        computerPieces.erase(m1);
        computerPieces.insert(m2);
    }

    if(castled) {

        if(m2.x == 2) {
            ChessPiece &rook = *board[m2.y][0];

            if(rook.isWhite == playerWhite) {
                playerPieces.erase(rook);
                rook.x = 2;
                playerPieces.insert(rook);
            } else {
                computerPieces.erase(rook);
                rook.x = 2;
                computerPieces.insert(rook);
            }

            board[m2.y][0] = nullptr;
            board[m2.y][rook.x] = &rook;
        } else {
            ChessPiece &rook = *board[m2.y][7];

            if(rook.isWhite == playerWhite) {
                playerPieces.erase(rook);
                rook.x = 5;
                playerPieces.insert(rook);
            } else {
                computerPieces.erase(rook);
                rook.x = 5;
                computerPieces.insert(rook);
            }

            board[m2.y][7] = nullptr;
            board[m2.y][rook.x] = &rook;
        }
    }
}

void ChessMove::undoMove() {

    board[m2.y][m2.x] = nullptr;
    board[m1.y][m1.x] = &m1;

    if(captured.pieceType != ChessPiece::PieceType::NONE) {
        board[captured.y][captured.x] = &captured;
        if(captured.isWhite == playerWhite) {
            playerPieces.insert(captured);
        } else {
            computerPieces.insert(captured);
        }
    }

    if(m1.isWhite == playerWhite) {
        playerPieces.erase(m2);
        playerPieces.insert(m1);
    } else {
        computerPieces.erase(m2);
        computerPieces.insert(m1);
    }

    if(castled) {

        if(m2.x == 2) {
            ChessPiece &rook = *board[m2.y][2];
            if(rook.isWhite == playerWhite) {
                playerPieces.erase(rook);
                rook.x = 0;
                playerPieces.insert(rook);
            } else {
                computerPieces.erase(rook);
                rook.x = 0;
                computerPieces.insert(rook);
            }
            board[m2.y][2] = nullptr;
            board[m2.y][rook.x] = &rook;
        } else {
            ChessPiece &rook = *board[m2.y][5];
            if(rook.isWhite == playerWhite) {
                playerPieces.erase(rook);
                rook.x = 7;
                playerPieces.insert(rook);
            } else {
                computerPieces.erase(rook);
                rook.x = 7;
                computerPieces.insert(rook);
            }
            board[m2.y][5] = nullptr;
            board[m2.y][rook.x] = &rook;
        }
    }


}
