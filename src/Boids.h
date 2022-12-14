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

#include "math_util.h"


/************************************/
/*        Callback Functions        */
/************************************/
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processInput(GLFWwindow* window);


enum Cam_Dir
{
    NONE,
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};


class Boids {

    private:
        float debug_fRotAngle;

        GLFWwindow *window;
        
        GLuint vertex_array;
        GLuint program;
        GLint mvp_location;
        GLint vpos_location;
        GLint vcol_location;

        Shader shader;

        // ImGui states
        bool bUI = true;
        bool test_triangle = false;
        bool show_demo_window = false;
        bool show_another_window = false;
        bool debug = true;

        // Debug
        glm::vec3 m_vBoundPos = glm::vec3(0.0f, 0.0f, 0.0f);

        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        unsigned int VBO, VAO, EBO;
        std::string sDebugInfo;

        int iNumBoids = 100;
        std::vector<Boid> vFlock;

    public:
        Boids();
        ~Boids();

        bool Construct();
        void Start();
        void Update(float fDeltaTime);
        void Render();
        void RenderBoids();
        void RenderUI();

        void InitializeBoids();
        void UpdateBoids();
        void BuffersAndShaders();
        glm::vec3 Rule1(int iCurBoidPos);
        glm::vec3 Rule2(int iCurBoidPos);
        glm::vec3 Rule3(int iCurBoidPos);
        glm::vec3 BoundPos(Boid b);
        void LimitVel(Boid &b);

};

#endif
