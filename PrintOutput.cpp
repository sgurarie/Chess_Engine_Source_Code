//
// Created by Sam Gurarie on 3/28/24.
//
#include "PrintOutput.h"
#include "ChessPiece.h"


void printChar(ll i, ll j) {
    if(board[i][j] == nullptr) {
        if(i % 2 == 0 ^ j % 2 == 0) {
            wcout << (wchar_t) u'■';
        } else {
            wcout << (wchar_t) u'□';
        }
    } else {
        wcout << (wchar_t) board[i][j]->character;
    }
}

void printBoard () {

    if(playerWhite) {
        for (ll i = 0; i < 8; i++) {
            for (ll j = 0; j < 8; j++) {
                printChar(i, j);
            }
        }
    } else {
        for (ll i = 7; i >= 0; i--) {
            for (ll j = 0; j < 8; j++) {
                printChar(i, j);
            }
        }
    }
}

void clearMemory() {
    for(ll i = 0; i < 8; i++) {
        for(ll j = 0; j < 8; j++) {
            board[i][j] = nullptr;
        }
    }

    computerPieces.clear();
    playerPieces.clear();
}

void loadFile(string const& file_name) {

    clearMemory();
    ifstream input_stream(file_name, ios::binary);
    if(!input_stream) {
        cerr << "Can't open file " + file_name << endl;
        exit(1);
    }

    stringstream buffer;
    buffer << input_stream.rdbuf();
    string file_content = buffer.str();

    playerWhite = file_content[0] == 'w';
    ll i = 2;
    while(i < file_content.size()) {

        bool isWhite = file_content[i] == 'w';
        char pieceT = file_content[i + 1];
        ll x = file_content[i + 2] - '0';
        ll y = file_content[i + 3] - '0';
        ChessPiece newPiece;
        if(pieceT == 'r') {
            Rook newPieceIn;
            newPieceIn.setup(x, y, isWhite);
            newPiece = (ChessPiece) newPieceIn;
        } else if(pieceT == 'n') {
            Knight newPieceIn;
            newPieceIn.setup(x, y, isWhite);
            newPiece = (ChessPiece) newPieceIn;
        } else if(pieceT == 'b') {
            Bishop newPieceIn;
            newPieceIn.setup(x, y, isWhite);
            newPiece = (ChessPiece) newPieceIn;
        } else if(pieceT == 'q') {
            Queen newPieceIn;
            newPieceIn.setup(x, y, isWhite);
            newPiece = (ChessPiece) newPieceIn;
        } else if(pieceT == 'k') {
            King newPieceIn;
            newPieceIn.setup(x, y, isWhite);
            newPiece = (ChessPiece) newPieceIn;
        } else {
            Pawn newPieceIn;
            newPieceIn.setup(x, y, isWhite);
            newPiece = (ChessPiece) newPieceIn;
        }

        const ChessPiece* ref;
        if(isWhite ^ playerWhite) {
            computerPieces.insert(newPiece);
            ref = &*computerPieces.cbegin();
        } else {
            playerPieces.insert(newPiece);
            ref = &*playerPieces.cbegin();
        }

        board[y][x] = ref;
        i += 5;
    }

    printBoard();
    input_stream.close();
}

void outPiece(const ChessPiece *p, ofstream *writer) {


    (*writer) << p->printFileTypeChar();
    (*writer) << p->x << p->y << "\n";
}

void saveFile(string const& file_name) {

    ofstream writer(file_name);
    if(playerWhite) {
        writer << "w\n";
    } else {
        writer << "b\n";
    }

    for(auto i = computerPieces.begin(); i != computerPieces.end(); i++) {

        if(playerWhite) {
            writer << "b";
        } else {
            writer << "w";
        }

        outPiece(&*i, &writer);
    }

    for(auto i = playerPieces.begin(); i != playerPieces.end(); i++) {

        if(playerWhite) {
            writer << "w";
        } else {
            writer << "b";
        }

        outPiece(&*i, &writer);
    }
}

void playerMove() {

}
