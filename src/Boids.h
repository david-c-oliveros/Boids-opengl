#ifndef BOIDS_H
#define BOIDS_H

#include <iostream>
#include <GL/glew.h>
#include<GLFW/glfw3.h>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>


class Boids {

    private:
        GLFWwindow *window;

    public:
        Boids();
        ~Boids();

        bool Construct();
        void Start();
        void Update(float fDeltaTime);

};

#endif
