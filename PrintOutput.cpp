//
// Created by Sam Gurarie on 3/28/24.
//
#include "PrintOutput.h"
#include "ChessPiece.h"


void printBoard () {

    if(playerWhite) {
        for (ll i = 0; i < 8; i++) {
            for (ll j = 0; j < 8; j++) {
                cout << board[i][j]->character;
            }
        }
    } else {
        for (ll i = 7; i >= 0; i--) {
            for (ll j = 0; j < 8; j++) {
                cout << board[i][j]->character;
            }
        }
    }
}

void loadFile(bool playerWhite, string file_name) {

    ifstream input_stream(file_name, ios::binary);
    if(!input_stream) {
        cerr << "Can't open file " + file_name << endl;
        exit(1);
    }

    stringstream buffer;
    buffer << input_stream.rdbuf();
    string file_content = buffer.str();

    ll i = 0;
    while(i < file_content.size()) {

        char pieceT = file_content[i];
        bool isWhite = file_content[i + 1] == 'w';
        ll x = file_content[i + 2] - '0';
        ll y = file_content[i + 3] - '0';
        ChessPiece newPiece;
        if(pieceT == 'r') {
            Rook newPieceIn;
            newPieceIn.setup(x, y, isWhite);
            newPiece = (ChessPiece) newPieceIn;
        } else if(pieceT == 'n') {

        } else if(pieceT == 'b') {

        } else if(pieceT == 'q') {

        } else if(pieceT == 'k') {

        } else if(pieceT == 'p') {

        }

        i += 5;
    }


}
