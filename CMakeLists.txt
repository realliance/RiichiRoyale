cmake_minimum_required(VERSION 3.12)
set(CMAKE_CXX_STANDARD 20)
include(FetchContent)
project(RiichiRoyale)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
set(BUILD_SHARED_LIBS TRUE)
option(PACKAGE_TESTS "Build the tests" OFF)
option(PACKAGE_TOOLS "Build the tools" OFF)
option(PY_BIND "Build with pybind" ON)

add_subdirectory(${RiichiRoyale_SOURCE_DIR}/libs)

find_package(Python3 COMPONENTS Interpreter Development)

if(PACKAGE_TESTS)
  enable_testing()
  include(GoogleTest)
  add_subdirectory(${RiichiRoyale_SOURCE_DIR}/cxxtests)
endif()

pybind11_add_module(RiichiRoyale)
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  target_compile_options(RiichiRoyale PRIVATE -pedantic -Wall -Wmost -Wextra -fstack-protector -fsanitize=address)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options(RiichiRoyale PRIVATE -pedantic -Wall -Wextra -fstack-protector -fsanitize=address)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_options(RiichiRoyale PRIVATE "/MT")
endif()

add_subdirectory(${RiichiRoyale_SOURCE_DIR}/include)
add_subdirectory(${RiichiRoyale_SOURCE_DIR}/aicontrollers)
add_subdirectory(${RiichiRoyale_SOURCE_DIR}/src)

if(PACKAGE_TESTS)
  get_target_property(_sources RiichiRoyale SOURCES)
  add_library(TestRiichiRoyale ${_sources})
  target_compile_definitions(TestRiichiRoyale PUBLIC -DNO_PYBIND)
  target_include_directories(TestRiichiRoyale PUBLIC "${RiichiRoyale_SOURCE_DIR}/include")
  target_include_directories(TestRiichiRoyale PUBLIC "${RiichiRoyale_SOURCE_DIR}/aicontrollers")
endif()

if(PACKAGE_TOOLS)
  add_subdirectory(${RiichiRoyale_SOURCE_DIR}/tools)
endif()

if(PY_BIND)
  target_sources(RiichiRoyale PRIVATE "src/python.cpp")
else()
  target_compile_definitions(RiichiRoyale PUBLIC -DNO_PYBIND)
endif()

target_include_directories(RiichiRoyale PUBLIC "${RiichiRoyale_SOURCE_DIR}/include")
target_include_directories(RiichiRoyale PUBLIC "${RiichiRoyale_SOURCE_DIR}/aicontrollers")

add_custom_target(
  check
  COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
  VERBATIM
)

if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
  add_custom_command(TARGET RiichiRoyale POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:RiichiRoyale> ${CMAKE_CURRENT_SOURCE_DIR}/libmahjong/libmahjong.so
  )
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
  add_custom_command(TARGET RiichiRoyale POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:RiichiRoyale> ${CMAKE_CURRENT_SOURCE_DIR}/libmahjong/libmahjong.pyd
  )
endif()

find_program(iwyu_path NAMES include-what-you-use iwyu)
if(iwyu_path)
  set_property(TARGET RiichiRoyale PROPERTY CXX_INCLUDE_WHAT_YOU_USE ${iwyu_path})
endif()
