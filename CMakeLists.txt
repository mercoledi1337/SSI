cmake_minimum_required(VERSION 4.0)
project(SSI)

set(CMAKE_CXX_STANDARD 20)

add_executable(SSI main.cpp
        gnuplot_iostream.h)

find_package(Boost REQUIRED COMPONENTS iostreams)
find_package(Boost REQUIRED COMPONENTS iostreams filesystem)
target_link_libraries(SSI PRIVATE Boost::iostreams)
target_link_libraries(SSI PRIVATE Boost::iostreams Boost::filesystem)