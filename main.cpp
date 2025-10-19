#include "PrintOutput.h"
#include "Bitboard.h"


using namespace std;
typedef long long ll;

int main() {

    //playerMove();
//    RookBitboard rook;
//    rook.generateAllMoves();

    double memUsage = getMemoryUsage() / 1000000.0;
    BishopBitboard bishop;
    bishop.generateAllMoves(true);
    bishop.generateAllMoves(false);

    BishopBitboard rook;
    rook.generateAllMoves(true);
    rook.generateAllMoves(false);

    KnightBitboard knight;
    knight.generateAllMoves(true);
    knight.generateAllMoves(false);

    KingBitboard king;
    king.generateAllMoves(true);
    king.generateAllMoves(false);

    WhitePawnBitboard whitePawn;
    whitePawn.generateAllMoves(true);
    whitePawn.generateAllMoves(false);

    BlackPawnBitboard blackPawn;
    blackPawn.generateAllMoves(true);
    blackPawn.generateAllMoves(false);

    cout << getMemoryUsage() / 1000000.0 - memUsage;
}
