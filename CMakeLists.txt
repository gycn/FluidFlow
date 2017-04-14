cmake_minimum_required(VERSION 2.8)
project(FluidFlow)
if(APPLE)

  # OSX Framework dependencies
  if(NOT BUILD_LIBCGL)
    include_directories( "/System/Library/Frameworks" )
    find_library (COCOA_LIBRARIES Cocoa)
    find_library (IOKIT_LIBRARIES IOkit)
    find_library (COREVIDEO_LIBRARIES CoreVideo)
  endif()

  # Clang configuration
  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")

    set(CLANG_CXX_FLAGS "-std=c++11 -m64")

    if(BUILD_DEBUG)
      set(CMAKE_BUILD_TYPE Debug)
      set(CLANG_CXX_FLAGS "${CLANG_CXX_FLAGS} -g")
    else(BUILD_DEBUG)
      set(CLANG_CXX_FLAGS "${CLANG_CXX_FLAGS} -O3")
      set(CLANG_CXX_FLAGS "${CLANG_CXX_FLAGS} -funroll-loops")
      set(CLANG_CXX_FLAGS "${CLANG_CXX_FLAGS} -Wno-narrowing")
      set(CLANG_CXX_FLAGS "${CLANG_CXX_FLAGS} -Wno-deprecated-register")
    endif(BUILD_DEBUG)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CLANG_CXX_FLAGS}")

  endif()

  # GCC configuration
  if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")

    set(GCC_CXX_FLAGS "-std=gnu++11 -m64")

    if(BUILD_DEBUG)
      set(CMAKE_BUILD_TYPE Debug)
      set(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -g")
    else(BUILD_DEBUG)
      set(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -O3")
    endif(BUILD_DEBUG)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_CXX_FLAGS}")

  endif()

endif(APPLE)

#GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(path/to/glfw)
target_link_libraries(FluidFlow glfw)

#project includes
include_directories(include)

#project sources
file(GLOB SOURCES "src/*.cpp")
add_executable(fluidflow ${SOURCES})
