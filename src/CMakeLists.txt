set(DIR ${CMAKE_CURRENT_LIST_DIR})
target_sources(RiichiRoyale
    PRIVATE
        "${DIR}/python.cpp"
        "${DIR}/walls.cpp"
)

if(PACKAGE_TOOLS)
    target_sources(mahjongShuffle
        PRIVATE
            "${DIR}/walls.cpp"
    )
endif()