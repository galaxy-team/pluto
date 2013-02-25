cmake_minimum_required(VERSION 2.8.5)
project(pluto)

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

add_executable(pluto
	src/main.cpp
	src/lib/libpluto.hpp
	src/lib/libpluto.cpp
)

#set(TEST "YES")
#IF (TEST)
#	ENABLE_TESTING()

#	add_executable(tests
#	)
#	target_link_libraries(test libsaturn)

#	ADD_TEST(tests ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tests "Unit tests")
#ENDIF(TEST)
