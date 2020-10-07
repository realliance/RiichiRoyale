set(DIR ${CMAKE_CURRENT_LIST_DIR})
target_sources(RiichiRoyale
    PRIVATE
    "${DIR}/board.h"
    "${DIR}/enum.h"
    "${DIR}/event.h"
    "${DIR}/hand.h"
    "${DIR}/mahjongai.h"
    "${DIR}/manager.h"
    "${DIR}/pieces.h"
    "${DIR}/walls.h"
)
target_include_directories(RiichiRoyale PRIVATE "${Dev}/include")
