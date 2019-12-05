cmake_minimum_required (VERSION 3.9)
project (Fractals)
set (CMAKE_CXX_STANDARD 14)

add_subdirectory(${CMAKE_SOURCE_DIR}/libs/glfw)

#Add OpenGL & GLAD
include_directories(${CMAKE_SOURCE_DIR}/libs/glfw/include)
include_directories(${CMAKE_SOURCE_DIR}/libs/glad/include)

#find sources and header files recursivly
file(GLOB_RECURSE MAIN_SOURCES ${CMAKE_SOURCE_DIR}/src/*.c*)
file(GLOB_RECURSE MAIN_HEADERS ${CMAKE_SOURCE_DIR}/src/*.h*)

file(COPY ${CMAKE_SOURCE_DIR}/src/shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

#Add Executable
add_executable(Fractals ${MAIN_SOURCES} ${MAIN_HEADERS} ${CMAKE_SOURCE_DIR}/libs/glad/src/glad.c)
target_link_libraries(Fractals glfw)