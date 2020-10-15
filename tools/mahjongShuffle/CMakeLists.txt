find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)
link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})

set(DIR ${CMAKE_CURRENT_LIST_DIR})

add_executable(mahjongShuffle "")

target_sources(mahjongShuffle
    PRIVATE
    "${DIR}/main.cpp"
    "${DIR}/mainwindow.cpp"
    "${DIR}/mainwindow.h"
    "${DIR}/stdform.cpp"
    "${DIR}/stdform.h"
    "${RiichiRoyale_SOURCE_DIR}/src/piecetype.cpp"
    "${RiichiRoyale_SOURCE_DIR}/src/walls.cpp"
    "${RiichiRoyale_SOURCE_DIR}/src/possiblesets.cpp"
)
target_link_libraries(mahjongShuffle ${GTKMM_LIBRARIES})
target_include_directories(mahjongShuffle PRIVATE "${RiichiRoyale_SOURCE_DIR}/include")

add_custom_command(TARGET mahjongShuffle POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:mahjongShuffle> ${CMAKE_CURRENT_SOURCE_DIR}/mahjongShuffle.tool
)
