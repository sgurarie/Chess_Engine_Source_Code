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
    vector<vector<pair<KeyStore, ull>>> array;

public:
    CustomHashmap(ull newSize) {
        size = newSize;
        array.resize(size);
    }

    CustomHashmap() {
        array.resize(size);
    }

    ull& operator[](KeyStore element) {
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
        array[hashValue].emplace_back(element, 0);
        return array[hashValue][array[hashValue].size() - 1].second;
    }

};

class CustomHashmapToList {

    ull size = 1'000;
    vector<vector<pair<ull, vector<LegalMovesStore>*>>> array;

public:
    CustomHashmapToList(ull newSize) {
        size = newSize;
        array.resize(size);
    }

    CustomHashmapToList() {
        array.resize(size);
    }

    vector<LegalMovesStore>*& operator[](ull element) {
        ull hashValue = element;

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

class CustomHashmapKeyStoreToList {

    ull size = 1'000;
    vector<vector<pair<KeyStore, vector<LegalMovesStore>*>>> array;

public:
    CustomHashmapKeyStoreToList(ull newSize) {
        size = newSize;
        array.resize(size);
    }

    CustomHashmapKeyStoreToList() {
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

public:
    static string stringBitBoard(ull board);
    static void printBitBoard(ull board);
    virtual void generateAllMoves(bool inclusive) = 0;


};

class LineBitboard : public Bitboard {
protected:
    CustomHashmap moves;
    CustomHashmap movesInclusive;
    CustomHashmapToList hashmapToList;

    void setBoard(KeyStore element, ull value, vector<LegalMovesStore>* list, bool inclusive) {
        if(inclusive) {
            movesInclusive[element] = value;
        } else {
            moves[element] = value;
            hashmapToList[moves[element]] = list;
        }
    }
};

class DirectAttackBitboard : public Bitboard {
protected:
    CustomHashmapKeyStoreToList hashmapToList;
    CustomHashmapKeyStoreToList hashmapToListInc;
public:
    void setBoard(KeyStore element, vector<LegalMovesStore>* list, bool inclusive) {
        if(inclusive) {
            hashmapToListInc[element] = list;
        } else {
            hashmapToList[element] = list;
        }
    }
};

class RookBitboard : public LineBitboard {

public:

    ull positionMoves[8][8][4];
    void generateAllMoves(bool inclusive) override;
    vector<vector<LegalMovesStore>*> getMove(short x, short y, ull sameTeamMask, ull otherTeamMask);
};

class BishopBitboard : public LineBitboard {
public:

    ull positionMoves[8][8][4];
    void generateAllMoves(bool inclusive) override;
    vector<vector<LegalMovesStore>*> getMove(short x, short y, ull sameTeamMask, ull otherTeamMask);
};

class QueenBitboard {
public:
    pair<vector<vector<LegalMovesStore>*>, vector<vector<LegalMovesStore>*>>  getMove(BishopBitboard &bishop, RookBitboard &rook, short x, short y, ull sameTeamMask, ull otherTeamMask);
};


class KnightBitboard : public DirectAttackBitboard {
public:
    ull positionMoves[8][8];
    void generateAllMoves(bool inclusive) override;
    pair<vector<LegalMovesStore>*, vector<LegalMovesStore>*> getMove(short x, short y, ull sameTeamMask, ull otherTeamMask);
};

class KingBitboard : public DirectAttackBitboard {
public:
    ull positionMoves[8][8];
    void generateAllMoves(bool inclusive) override;
    pair<vector<LegalMovesStore>*, vector<LegalMovesStore>*> getMove(short x, short y, ull sameTeamMask, ull otherTeamMask);
};

class PawnBitboard : public DirectAttackBitboard {

public:
    ull positionMoves[8][8];
    ull positionMovesIncl[8][8];

    virtual vector<vector<LegalMovesStore>*> getMove(short x, short y, ull sameTeamMask, ull otherTeamMask, ull otherPawns) = 0;

};

class WhitePawnBitboard : public PawnBitboard {
public:
    void generateAllMoves(bool inclusive) override;
    vector<vector<LegalMovesStore>*> getMove(short x, short y, ull sameTeamMask, ull otherTeamMask, ull otherPawns) override;
};

class BlackPawnBitboard : public PawnBitboard {
public:
    void generateAllMoves(bool inclusive) override;
    vector<vector<LegalMovesStore>*> getMove(short x, short y, ull sameTeamMask, ull otherTeamMask, ull otherPawns) override;
};

//TODO: Rigorously test legal move generation
//TODO: Implement reading and writing board states to memory (? necessary)
//TODO: begin move storage logic and create an arena to store future moves


#endif //CTEMPLATE_BITBOARD_H
