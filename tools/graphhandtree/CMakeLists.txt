find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)
link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})

set(DIR ${CMAKE_CURRENT_LIST_DIR})

add_executable(graphHandTree "")
target_compile_definitions(graphHandTree PUBLIC -DNO_PYBIND)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  target_compile_options(graphHandTree PRIVATE -Wall -Wmost -Wextra -Werror)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options(graphHandTree PRIVATE -Wall -Wextra)
endif()


target_sources(graphHandTree
    PRIVATE
    "${DIR}/main.cpp"
    "${RiichiRoyale_SOURCE_DIR}/src/handnode.cpp"
    "${RiichiRoyale_SOURCE_DIR}/src/handtree.cpp"
    "${RiichiRoyale_SOURCE_DIR}/src/piecetype.cpp"
    "${RiichiRoyale_SOURCE_DIR}/src/possiblesets.cpp"
)
target_link_libraries(graphHandTree ${GTKMM_LIBRARIES})
target_include_directories(graphHandTree PRIVATE "${RiichiRoyale_SOURCE_DIR}/include")

add_custom_command(TARGET graphHandTree POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:graphHandTree> ${CMAKE_CURRENT_SOURCE_DIR}/graphHandTree.tool
)
