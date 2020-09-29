FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.10.0
)

FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
  FetchContent_Populate(googletest)
  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()

FetchContent_Declare(
  pybind11
  GIT_REPOSITORY https://github.com/pybind/pybind11.git
  GIT_TAG        d159a563383d10c821ba7b2a71905d1207db6de4
)

FetchContent_GetProperties(pybind11)
if(NOT pybind11_POPULATED)
  FetchContent_Populate(pybind11)

  add_subdirectory(${pybind11_SOURCE_DIR} ${pybind11_BINARY_DIR})
endif()

FetchContent_Declare(
  riichimahjongtiles
  GIT_REPOSITORY https://github.com/FluffyStuff/riichi-mahjong-tiles.git
  GIT_TAG        9e2eba21ca69d6cd5cdd9a309d5754ac9517c481
)

FetchContent_GetProperties(riichimahjongtiles)
if(NOT riichimahjongtiles_POPULATED)
  FetchContent_Populate(riichimahjongtiles)
endif()

FetchContent_Declare(
  sourcesanspro
  GIT_REPOSITORY https://github.com/adobe-fonts/source-sans-pro.git
  GIT_TAG        5b44992cc72686a04adef7e81507060100c6ca77
)

FetchContent_GetProperties(sourcesanspro)
if(NOT sourcesanspro_POPULATED)
  FetchContent_Populate(sourcesanspro)
endif()
