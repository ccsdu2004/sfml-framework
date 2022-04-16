cmake_minimum_required(VERSION 3.5)

project(app)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb -pg")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

INCLUDE_DIRECTORIES(SFML-2.5.1/include)
INCLUDE_DIRECTORIES(thirdparty)
INCLUDE_DIRECTORIES(source)
INCLUDE_DIRECTORIES(framework)
INCLUDE_DIRECTORIES(./)

LINK_DIRECTORIES(SFML-2.5.1/lib)
link_directories(/usr/local/lib)

add_subdirectory(framework)
add_subdirectory(application)

message("finished ......................")
