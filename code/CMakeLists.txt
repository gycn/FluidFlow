cmake_minimum_required(VERSION 3.7)
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

#OpenCL
find_package(OpenCL REQUIRED)
target_link_libraries(fluidflow OpenCL::OpenCL)

#OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})
target_link_libraries(fluidflow ${OPENGL_LIBRARIES})

#OpenCV
find_package( OpenCV REQUIRED )
target_link_libraries( fluidflow ${OpenCV_LIBS} )
