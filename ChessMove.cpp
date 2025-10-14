#include "ChessMove.h"

bool ChessMove::operator==(const ChessMove &o) const {
    bool capturedTrue;
    if(this->captured != nullptr && o.captured != nullptr) {
        capturedTrue = *(this->captured) == *(o.captured);
    } else {
        capturedTrue = this->captured == nullptr && o.captured == nullptr;
    }

    return this->castled == o.castled && capturedTrue && *(this->m1) == *(o.m1) && *(this->m2) == *(o.m2);
}