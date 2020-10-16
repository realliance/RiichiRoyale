set(DIR ${CMAKE_CURRENT_LIST_DIR})

add_subdirectory(${DIR}/mahjongShuffle)
add_subdirectory(${DIR}/graphHandTree)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  target_compile_options(mahjongShuffle PRIVATE -Wall -Wmost -Wextra -Werror)
  target_compile_options(graphHandTree PRIVATE -Wall -Wmost -Wextra -Werror)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options(mahjongShuffle PRIVATE -Wall -Wextra)
  target_compile_options(graphHandTree PRIVATE -Wall -Wextra)
endif()
