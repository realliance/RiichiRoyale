cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 20)
include(FetchContent)
project(RiichiRoyale)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
option(PACKAGE_TESTS "Build the tests" OFF)
option(PACKAGE_TOOLS "Build the tools" OFF)

add_subdirectory(${RiichiRoyale_SOURCE_DIR}/libs)

find_package (Python3 COMPONENTS Interpreter Development)

if(PACKAGE_TOOLS)
  add_subdirectory(${RiichiRoyale_SOURCE_DIR}/tools)
endif()

if(PACKAGE_TESTS)
  enable_testing()
  include(GoogleTest)
  add_subdirectory(${RiichiRoyale_SOURCE_DIR}/cxxtests)
endif()

pybind11_add_module(RiichiRoyale)
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  target_compile_options(RiichiRoyale PRIVATE -pedantic -Wall -Wmost -Wextra)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options(RiichiRoyale PRIVATE -Wall -Wextra)
endif()

add_subdirectory(${RiichiRoyale_SOURCE_DIR}/include)
add_subdirectory(${RiichiRoyale_SOURCE_DIR}/src)

target_include_directories(RiichiRoyale PUBLIC "${RiichiRoyale_SOURCE_DIR}/include")

add_custom_target(
  check
  COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
  VERBATIM
)

add_custom_command(TARGET RiichiRoyale POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:RiichiRoyale> ${CMAKE_CURRENT_SOURCE_DIR}/riichiroyale/libmahjong.so
)

find_program(iwyu_path NAMES include-what-you-use iwyu  REQUIRED)
set_property(TARGET RiichiRoyale PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
