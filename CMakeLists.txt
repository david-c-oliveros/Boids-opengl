cmake_minimum_required(VERSION 3.23.3)

project(Boids)

add_executable(${PROJECT_NAME} src/Boids.cpp)

add_subdirectory(external/glfw)

target_include_directories(${PROJECT_NAME}
    PUBLIC external/glfw/include)

target_link_directories(${PROJECT_NAME}
    PRIVATE external/glfw/src)

find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(GLEW REQUIRED)


add_dependencies(Boids OpenGL::OpenGL)
include_directories(${GLEW_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES} GLEW OpenGL::OpenGL)
