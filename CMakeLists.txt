cmake_minimum_required(VERSION 2.8)
project(FluidFlow)

#project includes
include_directories(inc)

#project sources
file(GLOB SOURCES "src/*.cpp")
add_executable(fluidflow ${SOURCES})
