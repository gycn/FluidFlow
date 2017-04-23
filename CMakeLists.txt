cmake_minimum_required(VERSION 2.8)
project(FluidFlow)

#project includes
include_directories(inc)

include_directories(glm)
#project sources
file(GLOB SOURCES "src/*.cpp" "src/*.c")
add_executable(fluidflow ${SOURCES})

#GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)
target_link_libraries(fluidflow glfw)

