#ifndef BOIDS_H
#define BOIDS_H

#include <iostream>
#include <vector>

#include <GL/glew.h>
#include<GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "Boid.h"


class Boids {

    private:
        GLFWwindow *window;

        // ImGui states
        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        int iNumBoids = 100;
        std::vector<Boid> vFlock;

    public:
        Boids();
        ~Boids();

        bool Construct();
        void Start();
        void Update(float fDeltaTime);
        void Render();

        void InitializeBoids();
        void UpdateBoids();
        glm::vec2 Rule1(Boid b, int iCurBoidPos);
        glm::vec2 Rule2(Boid b, int iCurBoidPos);
        glm::vec2 Rule3(Boid b, int iCurBoidPos);

};

#endif
