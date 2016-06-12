cmake_minimum_required(VERSION 2.8)

project(learn-opengl)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(deps/glfw)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++11")
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

include_directories(include/
                    deps/glad/include/
                    deps/glfw/include/
                    deps/glm/
                    deps/stb/)

file(GLOB DEPS deps/glad/src/glad.c)
file(GLOB SOURCES src/*.cpp
                  src/*.c)
file(GLOB HEADERS include/*.hpp
                  include/*.h)
file(GLOB SHADERS shaders/*.comp
                  shaders/*.frag
                  shaders/*.geom
                  shaders/*.vert)
file(GLOB CONFIGS CMakeLists.txt
                          README.md
                         .gitignore
                         .gitmodules)

source_group("Headers" FILES ${HEADERS})
source_group("Shaders" FILES ${SHADERS})
source_group("Sources" FILES ${SOURCES})
source_group("Vendors" FILES ${})

add_definitions(-DGLFW_INCLUDE_NONE -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")

# Getting Started - Hello Window
add_executable(hello-window src/01-getting-started/01-hello-window.cpp ${SOURCES} ${HEADERS} ${SHADERS} ${CONFIGS} ${DEPS})
target_link_libraries(hello-window glfw ${GLFW_LIBRARIES} ${GLAD_LIBRARIES})

# Getting Started - Hello Triangle
add_executable(hello-triangle src/01-getting-started/02-hello-triangle.cpp ${SOURCES} ${HEADERS} ${SHADERS} ${CONFIGS} ${DEPS})
target_link_libraries(hello-triangle glfw ${GLFW_LIBRARIES} ${GLAD_LIBRARIES})
