//
// Created by Sam Gurarie on 9/8/25.
//
#include<iostream>
#include<vector>


#ifndef CTEMPLATE_BITBOARD_H
#define CTEMPLATE_BITBOARD_H

using namespace std;
typedef long long ll;
typedef unsigned long long ull;


struct LegalMovesStore {
    ull legalMovesBitboard;
    short x, y;
};
//
class KeyStore {
public:
    ull elementMask;
    short x, y;
    short dir;

    bool operator==(KeyStore &other) const {
        return other.elementMask == this->elementMask && other.x == this->x && other.y == this->y && this->dir == other.dir;
    }
};


class CustomHashmap {

    ull size = 1'000;
    vector<vector<pair<KeyStore, vector<LegalMovesStore>*>>> array;

public:
    CustomHashmap(ull newSize) {
        size = newSize;
        array.resize(size);
    }

    CustomHashmap() {
        array.resize(size);
    }

    vector<LegalMovesStore>*& operator[](KeyStore element) {
        ull hashValue = element.elementMask;
        hashValue ^= (static_cast<unsigned long long>(element.x) & 0x7ull)
                     | ((static_cast<unsigned long long>(element.y) & 0x7ull) << 3)
                     |  ((static_cast<unsigned long long>(element.dir) & 0x7ull) << 6); //mix in position

        hashValue += 0x9e3779b97f4a7c15ull;                 // add golden ratio constant
        hashValue = (hashValue ^ (hashValue >> 30)) * 0xbf58476d1ce4e5b9ull;
        hashValue = (hashValue ^ (hashValue >> 27)) * 0x94d049bb133111ebull;
        hashValue = hashValue ^ (hashValue >> 31);
        hashValue = hashValue % size;
        for(auto & i : array[hashValue]) {
            if (i.first == element) {
                return i.second;
            }
        }
        array[hashValue].emplace_back(element, new vector<LegalMovesStore>);
        return array[hashValue][array[hashValue].size() - 1].second;
    }

};

class Bitboard {
protected:
    CustomHashmap moves;
    CustomHashmap movesInclusive;
public:
    static string stringBitBoard(ull board);
    static void printBitBoard(ull board);
    virtual void generateAllMoves(bool inclusive) = 0;
    void setBoard(KeyStore element, vector<LegalMovesStore>* value) {
        moves[element] = value;
    }
};

class RookBitboard : public Bitboard {

public:
    ull positionMoves[8][8][4];
    void generateAllMoves(bool inclusive) override;
    vector<vector<LegalMovesStore>*> getMove(short x, short y, ull sameTeamMask, ull otherTeamMask);
};

class BishopBitboard : public Bitboard {
public:
    ull positionMoves[8][8][4];
    void generateAllMoves(bool inclusive) override;
    vector<vector<LegalMovesStore>*> getMove(short x, short y, ull sameTeamMask, ull otherTeamMask);
};

class QueenBitboard {
public:
    pair<vector<vector<LegalMovesStore>*>, vector<vector<LegalMovesStore>*>>  getMove(BishopBitboard bishop, RookBitboard rook, short x, short y, ull sameTeamMask, ull otherTeamMask);
};


class KnightBitboard : public Bitboard {
public:
    ull positionMoves[8][8];
    void generateAllMoves(bool inclusive) override;
    pair<vector<LegalMovesStore>*, vector<LegalMovesStore>*> getMove(short x, short y, ull sameTeamMask, ull otherTeamMask);
};

class KingBitboard : public Bitboard {
public:
    ull positionMoves[8][8];
    void generateAllMoves(bool inclusive) override;
    pair<vector<LegalMovesStore>*, vector<LegalMovesStore>*> getMove(short x, short y, ull sameTeamMask, ull otherTeamMask);
};

class WhitePawnBitboard : public Bitboard {
public:
    ull positionMoves[8][8];
    ull positionMovesIncl[8][8];
    void generateAllMoves(bool inclusive) override;
    vector<vector<LegalMovesStore>*> getMove(short x, short y, ull sameTeamMask, ull otherTeamMask, ull otherPawns);
};

class BlackPawnBitboard : public Bitboard {
public:
    ull positionMoves[8][8];
    ull positionMovesIncl[8][8];
    void generateAllMoves(bool inclusive) override;
    vector<vector<LegalMovesStore>*> getMove(short x, short y, ull sameTeamMask, ull otherTeamMask, ull otherPawns);
};

//TODO: Rigorously test legal move generation
//TODO: Implement reading and writing board states to memory (? necessary)
//TODO: begin move storage logic and create an arena to store future moves


#endif //CTEMPLATE_BITBOARD_H
