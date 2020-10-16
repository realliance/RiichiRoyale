set(DIR ${CMAKE_CURRENT_LIST_DIR})
target_sources(RiichiRoyale
  PRIVATE
    "${DIR}/python.cpp"
    "${DIR}/event.cpp"
    "${DIR}/gamestate.cpp"
    "${DIR}/hand.cpp"
    "${DIR}/handnode.cpp"
    "${DIR}/hands.cpp"
    "${DIR}/handtree.cpp"
    "${DIR}/manager.cpp"
    "${DIR}/meld.cpp"
    "${DIR}/piecetype.cpp"
    "${DIR}/player.cpp"
    "${DIR}/possiblesets.cpp"
    "${DIR}/walls.cpp"
)
