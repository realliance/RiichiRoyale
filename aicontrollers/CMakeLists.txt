set(DIR ${CMAKE_CURRENT_LIST_DIR})
target_sources(RiichiRoyale
  PRIVATE
    "${DIR}/angrydiscardobot.cpp"
    "${DIR}/angrydiscardobot.h"
    "${DIR}/pythonaiinterface.cpp"
    "${DIR}/pythonaiinterface.h"
)

target_include_directories(RiichiRoyale PRIVATE "${Dev}/include")
