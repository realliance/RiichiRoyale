find_package(PkgConfig REQUIRED)

set(DIR ${CMAKE_CURRENT_LIST_DIR})

add_executable(testingZone "")
target_compile_definitions(testingZone PUBLIC -DNO_PYBIND)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  target_compile_options(testingZone PRIVATE -Wall -Wmost -Wextra)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options(testingZone PRIVATE -Wall -Wextra)
endif()

get_target_property(_sources RiichiRoyale SOURCES)
target_sources(testingZone
    PRIVATE
    "${DIR}/main.cpp"
    ${_sources}
)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(testingZone PRIVATE Threads::Threads)
target_compile_definitions(testingZone PUBLIC -DNO_PYBIND)
target_include_directories(testingZone PRIVATE "${RiichiRoyale_SOURCE_DIR}/include")
target_include_directories(testingZone PUBLIC "${RiichiRoyale_SOURCE_DIR}/aicontrollers")

add_custom_command(TARGET testingZone POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:testingZone> ${CMAKE_CURRENT_SOURCE_DIR}/testingZone.tool
)
