cmake_minimum_required(VERSION 3.10)
project(Engine2D)
set(CMAKE_CXX_STANDARD 17)

# sources
set(SOURCES
    main.cpp
    engine.cpp
    testing.cpp
)

# headers
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# find packages
find_package(OpenGL REQUIRED)

# GLFW
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)

# GLEW
find_package(GLEW REQUIRED)

# executable
add_executable(${PROJECT_NAME} ${SOURCES})

# link libraries
target_link_libraries(${PROJECT_NAME}
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${GLFW_LIBRARIES}
)
