cmake_minimum_required(VERSION 3.23.3)

find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(GLEW REQUIRED)

project(Boids)

set(IMGUI_PATH ${CMAKE_SOURCE_DIR}/external/imgui)
add_executable(${PROJECT_NAME} src/main.cpp src/Boids.cpp ${IMGUI_SOURCES})

add_subdirectory(external/glfw)
add_subdirectory(external/imgui)

target_include_directories(${PROJECT_NAME}
    PUBLIC external/glfw/include
    PUBLIC external/imgui)

target_link_directories(${PROJECT_NAME}
    PRIVATE external/glfw/src
    PRIVATE external/imgui)

include_directories(${GLEW_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES} GLEW OpenGL::OpenGL imgui)
