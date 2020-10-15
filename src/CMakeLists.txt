set(DIR ${CMAKE_CURRENT_LIST_DIR})
target_sources(RiichiRoyale
    PRIVATE
        "${DIR}/hand.cpp"
        "${DIR}/handnode.cpp"
        "${DIR}/hands.cpp"
        "${DIR}/handtree.cpp"
        "${DIR}/manager.cpp"
        "${DIR}/piecetype.cpp"
        "${DIR}/possiblesets.cpp"
        "${DIR}/walls.cpp"
)
