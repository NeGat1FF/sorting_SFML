cmake_minimum_required(VERSION 3.22)
project(sorting_SFML)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR $"C:/SFML")
find_package(SFML 2.5.1 COMPONENTS audio graphics network system REQUIRED)

find_package(Boost 1.77.0 COMPONENTS program_options REQUIRED)

include_directories(${Boost_INCLUDE_DIR})

add_executable(sorting_SFML main.cpp)
target_link_libraries(sorting_SFML sfml-audio sfml-graphics sfml-network sfml-system sfml-window ${Boost_LIBRARIES})