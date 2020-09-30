#include "pieces.h"

enum Decisions{
    Discard,
    Riichi,
    ConcealedKan,
    Decline,
    Chi,
    Pon,
    Ron,
    Kan,
    Tsumo
};

struct Decision {
    Piece piece;
    Decisions decision = Discard;
};