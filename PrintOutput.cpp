//
// Created by Sam Gurarie on 3/28/24.
//
#include "PrintOutput.h"
typedef long long ll;
#include <fstream>
#include <sstream>
#include <mach/mach.h>

thread_local RootChessMoveTree rootChessMoveTree;

void printChar(ll i, ll j) {

    if(board[i][j] == nullptr) {
        if(i % 2 == 0 ^ j % 2 == 0) {
            cout << "■";
        } else {
            cout << "□";
        }
    } else {
        cout << board[i][j]->printBoard();
    }
}

void printBoard () {

    if(playerWhite) {
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

void clearMemory() {

    rootChessMoveTree.clearMemory();
    for(ll i = 0; i < 8; i++) {
        for(ll j = 0; j < 8; j++) {
            if(board[i][j] != nullptr) {
                delete board[i][j];
                board[i][j] = nullptr;
            }
             //technically still a memory leak but im too lazy to deal with it
             //cuz in function AI.makeMove() we don't delete initially configured pieces
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
    input_stream.close();
    string file_content = buffer.str();
    playerWhite = file_content[0] == 'w';
    playerTurn = playerWhite;
    ll i = 2;
    while(i < file_content.size()) {

        bool isWhite = file_content[i] == 'w';
        char pieceT = file_content[i + 1];
        ll x = file_content[i + 2] - '0';
        ll y = file_content[i + 3] - '0';
        bool firstMove = file_content[i + 4] == 't';
        bool advancedTwo = file_content[i + 5] == 't';
        ChessPiece *newPiece;
        if(pieceT == 'r') {
            Rook *newPieceIn = new Rook;
            newPieceIn->setup(x, y, isWhite, firstMove, advancedTwo);
            newPiece = newPieceIn;
        } else if(pieceT == 'n') {
            Knight *newPieceIn = new Knight;
            newPieceIn->setup(x, y, isWhite, firstMove, advancedTwo);
            newPiece = newPieceIn;
        } else if(pieceT == 'b') {
            Bishop *newPieceIn = new Bishop;
            newPieceIn->setup(x, y, isWhite, firstMove, advancedTwo);
            newPiece = newPieceIn;
        } else if(pieceT == 'q') {
            Queen *newPieceIn = new Queen;
            newPieceIn->setup(x, y, isWhite, firstMove, advancedTwo);
            newPiece = newPieceIn;
        } else if(pieceT == 'k') {
            King *newPieceIn = new King;
            newPieceIn->setup(x, y, isWhite, firstMove, advancedTwo);
            newPiece = newPieceIn;
            if(isWhite) whiteKing = *newPieceIn;
            else blackKing = *newPieceIn;
        } else {
            Pawn *newPieceIn = new Pawn;
            newPieceIn->setup(x, y, isWhite, firstMove, advancedTwo);
            newPiece = newPieceIn;
        }

        if(isWhite ^ playerWhite) {
            computerPieces.insert(newPiece);
        } else {
            playerPieces.insert(newPiece);
        }

        board[y][x] = newPiece->deepCopy();
        i += 7;
    }

}

void outPiece(ChessPiece *p, ofstream *writer) {


    (*writer) << p->printFileTypeChar();
    (*writer) << p->x << p->y << (p->firstMove ? "t" : "f") << (p->advancedTwo ? "t" : "f") << "\n";
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

        outPiece(*i, &writer);
    }

    for(auto i = playerPieces.begin(); i != playerPieces.end(); i++) {

        if(playerWhite) {
            writer << "w";
        } else {
            writer << "b";
        }

        outPiece(*i, &writer);
    }
}

bool notInRange(int num) {
    return num < 0 || num > 7;
}

void loadFileAndClearMemory(string name, AI &ai, bool firstTime) {
    loadFile(name);
    ai.assignPieceList();
    rootChessMoveTree.tree->evaluation = ai.findEvaluation();
    if(!playerWhite && firstTime) {
        playerTurn = false;
        ai.lookAhead(0, *rootChessMoveTree.tree);
        ChessMove* result = rootChessMoveTree.computerMove();
        ai.makeMove(*result);
        printBoard();
        cout << "\nResult of computer move. Evaluation: " << currentEvaluation << "\n\n";
    } else {
        printBoard();
    }

    if(!firstTime) {
        playerTurn = true;
    }
}

void playerMove() {

    AI ai;
    loadFileAndClearMemory("StartingPositionBlack.txt", ai, true);

    while(true) {
        cout << "Input Move: ";
        string move;
        while(move.empty()) { getline(cin, move); }
        if(move[0] == 's') {
            string name = move.substr(2, move.size() - 2);
            saveFile(name);
            continue;
        } else if(move[0] == 'l') {
            string name = move.substr(2, move.size() - 2);
            loadFileAndClearMemory(name, ai, false);
            continue;
        } else if(move[0] == 'u') {
            loadFileAndClearMemory("LastMove.txt", ai, false);
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

        if(notInRange(x1) || notInRange(y1) || notInRange(x2) || notInRange(y2) || board[y1][x1] == nullptr || board[y1][x1]->isWhite != playerWhite) {
            cout << "Invalid input format\n";
            continue;
        }

        ChessPiece::PieceType promotion = ChessPiece::PieceType::NONE;
        if(y2 == endPoint[playerWhite ? 0 : 1] && board[y1][x1]->pieceType == ChessPiece::PieceType::PAWN) {
            while(true) {
                cout << "Promote to (queen, rook, bishop, or knight)? ";
                string answer; cin >> answer;
                if (answer[0] == 'q') {
                    promotion = ChessPiece::PieceType::QUEEN;
                    break;
                } else if (answer[0] == 'r') {
                    promotion = ChessPiece::PieceType::ROOK;
                    break;
                } else if (answer[0] == 'b') {
                    promotion = ChessPiece::PieceType::BISHOP;
                    break;
                } else if (answer[0] == 'k') {
                    promotion = ChessPiece::PieceType::KNIGHT;
                    break;
                } else {
                    cout << "Invalid input" << endl;
                }
            }
        }

        ai.lookAhead(0, *rootChessMoveTree.tree);
        ChessMove* result = rootChessMoveTree.createMoveFromChords(x1, y1, x2, y2, promotion);
        if(!rootChessMoveTree.playerMove(*result)) {
            cout << "Not a legal move\n";
            continue;
        } else {
            ai.makeMove(*result);
        }


        ai.lookAhead(0, *rootChessMoveTree.tree);
        if(rootChessMoveTree.noNextMoves()) {
            cout << R"(You won! You're very smart! 🔥 😎)";
            return;
        }

        printBoard();
        cout << "\nResult of your move. Evaluation: " << currentEvaluation << "\n\n";

        result = rootChessMoveTree.computerMove();
        ai.makeMove(*result);
        ai.lookAhead(0, *rootChessMoveTree.tree);
        if(rootChessMoveTree.noNextMoves()) {
            cout << R"(Get gud. You got shat on. 🧌 💩)";
            return;
        }

        printBoard();
        cout << "\nResult of computer move. Evaluation: " << currentEvaluation << "\n\n";
    }

}

ll getMemoryUsage() {
    task_basic_info info;
    mach_msg_type_number_t infoCount = TASK_BASIC_INFO_COUNT;
    if (task_info(mach_task_self(), TASK_BASIC_INFO, (task_info_t)&info, &infoCount) != KERN_SUCCESS) {
        return 0;
    }
    return info.resident_size; // bytes
}
