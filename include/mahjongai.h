#include "pieces.h"

struct MahjongAi{
    Decision DiscardPiece() = 0;
    Decision TakeDiscard() = 0;
}