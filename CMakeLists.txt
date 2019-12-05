cmake_minimum_required (VERSION 3.9)
project (Fractals)
set (CMAKE_CXX_STANDARD 14)

option (GLFW_BUILD_EXAMPLES OFF)
option (GLFW_BUILD_TESTS OFF)
option (GLFW_BUILD_DOCS OFF)
option (GLFW_INSTALL OFF)

#GLFW
add_subdirectory(libs/glfw)
include_directories(libs/glfw/include)

#OpenGL Loader
include_directories(libs/glad/include)

#OpenGL Math Library
include_directories(libs/glm)

#Find Sources and Headers in src folder
file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS src/*.h)

#add those files to a list
source_group (TREE ${CMAKE_SOURCE_DIR}/src PREFIX "" FILES ${SOURCES} ${HEADERS})

#define executable
add_executable (Fractals ${SOURCES} ${HEADERS} libs/glad/src/glad.c)
target_link_libraries (Fractals glfw)