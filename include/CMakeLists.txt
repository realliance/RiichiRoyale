set(DIR ${CMAKE_CURRENT_LIST_DIR})
target_sources(RiichiRoyale
    PRIVATE
    "${DIR}/event.h"
    "${DIR}/gamestate.h"
    "${DIR}/hand.h"
    "${DIR}/handnode.h"
    "${DIR}/handtree.h"
    "${DIR}/mahjongai.h"
    "${DIR}/manager.h"
    "${DIR}/meld.h"
    "${DIR}/piecetype.h"
    "${DIR}/possiblesets.h"
    "${DIR}/statefunction.h"
    "${DIR}/walls.h"
    "${DIR}/winds.h"
)
