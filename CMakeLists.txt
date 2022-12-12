cmake_minimum_required(VERSION 3.23.3)

#add_compile_options(-Wfatal-errors -w)

find_package(OpenGL REQUIRED COMPONENTS OpenGL)
find_package(GLEW REQUIRED)

project(Boids)

add_executable(${PROJECT_NAME} src/main.cpp
    src/Boids.cpp
    src/Boid.cpp)

add_subdirectory(external/glfw)
add_subdirectory(external/imgui)
add_subdirectory(external/glm)

target_include_directories(${PROJECT_NAME}
    PUBLIC external/glfw/include
    PUBLIC external/imgui
    PUBLIC external/glm)

target_link_directories(${PROJECT_NAME}
    PRIVATE external/glfw/src
    PRIVATE external/imgui
    PRIVATE external/glm)

include_directories(${GLEW_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES} GLEW OpenGL::OpenGL imgui glm::glm)
