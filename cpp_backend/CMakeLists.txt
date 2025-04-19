cmake_minimum_required(VERSION 3.29)
project(CliniQuery)

set(CMAKE_CXX_STANDARD 20)

# Add Crow (adjust the path to where crow_all.h is located)
# include_directories(${CMAKE_SOURCE_DIR}/cpp_backend/external/crow/include)
include_directories(external/crow/include)

# Add nlohmann/json (if using a local copy)
include_directories(${CMAKE_SOURCE_DIR}/include/json)

# Add asio
include_directories(external/asio/include)

# Add executable and source files
add_executable(CliniQuery
    src/main.cpp
    src/trieName.cpp
    src/trieSymp.cpp
    src/bplus.cpp
)

find_package(Threads REQUIRED)
target_link_libraries(CliniQuery PRIVATE ws2_32 Threads::Threads mswsock)

# Link pthread (required for Crow on Linux/Unix)
if(UNIX)
    target_link_libraries(CliniQuery pthread)
endif()
