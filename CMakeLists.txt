cmake_minimum_required(VERSION 2.8.5)
project(pluto)

#set(CMAKE_VERBOSE_MAKEFILE ON)
set(VERBOSE, ON)
set(DEBUG, ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_FLAGS "--std=c++11 ${CMAKE_CXX_FLAGS}")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/lib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/asteroid)

add_library(libpluto
	${CMAKE_CURRENT_SOURCE_DIR}/src/lib/libpluto.cpp
)

set_target_properties(libpluto PROPERTIES PREFIX "")


set(OPTIONPARSER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third-party/optparse/)
include_directories(${OPTIONPARSER_DIR})
if (TARGET optionparser)
    message(STATUS "OptionParser already built as part of another part of Galaxy")
else()
    # setup OptionParser
    add_library(optionparser
        ${OPTIONPARSER_DIR}/OptionParser.cpp
    )
endif()

add_executable(pluto
    src/main.cpp
)

target_link_libraries(pluto libpluto optionparser)

add_executable(test_pluto
    ${CMAKE_CURRENT_SOURCE_DIR}/src/test/tests.cpp
)
target_link_libraries(test_pluto libpluto)

