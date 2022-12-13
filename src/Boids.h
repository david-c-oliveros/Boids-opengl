#ifndef BOIDS_H
#define BOIDS_H

#include <iostream>
#include <vector>

#include <GL/glew.h>
#include<GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "Boid.h"
#include "Shader.h"


/************************************/
/*        Callback Functions        */
/************************************/
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xPosIn, double yPosIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow* window);


class Boids {

    private:
        float debug_fRotAngle;
        GLFWwindow *window;
        Shader shader;

        // ImGui states
        bool bUI = true;
        bool show_demo_window = true;
        bool show_another_window = false;
        bool debug = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        std::string sDebugInfo;

        int iNumBoids = 1;
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
        glm::vec3 Rule1(Boid b, int iCurBoidPos);
        glm::vec3 Rule2(Boid b, int iCurBoidPos);
        glm::vec3 Rule3(Boid b, int iCurBoidPos);

};

#endif
