set(DIR ${CMAKE_CURRENT_LIST_DIR})
target_sources(RiichiRoyale
    PRIVATE
        "${DIR}/walls.h"
        "${DIR}/pieces.h"
        "${DIR}/meld.h"
)
target_include_directories(RiichiRoyale PRIVATE "${RiichiRoyale_SOURCE_DIR}/include")

if(PACKAGE_TOOLS)
    target_sources(mahjongShuffle
        PRIVATE
            "${DIR}/walls.h"
    )
    target_include_directories(mahjongShuffle PRIVATE "${RiichiRoyale_SOURCE_DIR}/include")
endif()