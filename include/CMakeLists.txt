set(DIR ${CMAKE_CURRENT_LIST_DIR})
target_sources(RiichiRoyale
    PRIVATE
    "${DIR}/enum.h"
    "${DIR}/event.h"
    "${DIR}/gamestate.h"
    "${DIR}/hand.h"
    "${DIR}/handnode.h"
    "${DIR}/handtree.h"
    "${DIR}/mahjongai.h"
    "${DIR}/manager.h"
    "${DIR}/piecetype.h"
    "${DIR}/possiblesets.h"
    "${DIR}/roundstate.h"
    "${DIR}/walls.h"
)

target_include_directories(RiichiRoyale PRIVATE "${Dev}/include")
