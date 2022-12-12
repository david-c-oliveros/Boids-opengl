#include "Boids.h"


unsigned int SCR_WIDTH = 1920;
unsigned int SCR_HEIGHT = 1080;

float fDeltaTime = 0.0f;
float fLastFrame = 0.0f;
float fCurrentFrame = 0.0f;


Boids::Boids()
{
}


Boids::~Boids()
{
}


bool Boids::Construct()
{
    const char* glsl_version = "#version 330";
    /**********************************************************/
    /**********************************************************/
    /*               Initialize GLFW and GLEW                 */
    /**********************************************************/
    /**********************************************************/
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW\n";
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Boids", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "\nERROR: Failed to initialize GLEW\n";
        return false;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    return true;
}


void Boids::Start()
{
    InitializeBoids();

    // Main Loop
    while (!glfwWindowShouldClose(window))
    {
        fDeltaTime = fCurrentFrame - fLastFrame;
        fLastFrame = fCurrentFrame;

        Update(fDeltaTime);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}


void Boids::Update(float fDeltaTime)
{
    UpdateBoids();
    Render();
}


void Boids::Render()
{
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    // Show a simple window
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");

        ImGui::Text("Here is some text");
        ImGui::Checkbox("Demo Window", &show_demo_window);
        ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
        ImGui::ColorEdit3("clear color", (float*)&clear_color);

        if (ImGui::Button("Button"))
            counter++;

        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        ImGui::End();
    }

    // Show another simple window
    if (show_another_window)
    {
        ImGui::Begin("Another Window", &show_another_window);
        ImGui::Text("Hello from another window");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
}


void  Boids::InitializeBoids()
{
    for (int i = 0; i < iNumBoids; i++)
    {
        vFlock.push_back(Boid(glm::vec2(SCR_WIDTH / 2, SCR_HEIGHT / 2), glm::vec2(0.0f, 0.0f )));
    }
}


void Boids::UpdateBoids()
{
    glm::vec2 v1, v2, v3;
    for (int i = 0; i < vFlock.size(); i++)
    {
        v1 = Rule1(vFlock[i], i);
        v2 = Rule2(vFlock[i], i);
        v3 = Rule3(vFlock[i], i);

        vFlock[i].vVel = vFlock[i].vVel + v1 + v2 + v3;
        vFlock[i].vPos = vFlock[i].vPos + vFlock[i].vVel;
    }

}


glm::vec2 Boids::Rule1(Boid b, int iCurBoidPos)
{
    glm::vec2 vPCenterOfMass = glm::vec2(0.0f, 0.0f);

    for (int i = 0; i < vFlock.size(); i++)
    {
        if (i == iCurBoidPos)
            continue;

        vPCenterOfMass += vFlock[i].vPos;
    }

    vPCenterOfMass = vPCenterOfMass / (vFlock.size() - 1.0f);

    return vPCenterOfMass / 100.0f;
}

glm::vec2 Boids::Rule2(Boid b, int iCurBoidPos)
{
    glm::vec2 c(0.0f, 0.0f);

    for (int i = 0; i < vFlock.size(); i++)
    {
        if (i == iCurBoidPos)
            continue;

        if (glm::length(vFlock[i].vPos - b.vPos) < 100.0f)  // TODO Fix for vector
        {
            c = c - (vFlock[i].vPos - b.vPos);
        }
    }

    return c;
}


glm::vec2 Boids::Rule3(Boid b, int iCurBoidPos)
{
    glm::vec2 vPVel;

    for (int i = 0; i < vFlock.size(); i++)
    {
        if (i == iCurBoidPos)
            continue;
        vPVel = vPVel + vFlock[i].vVel;
    }

    vPVel = vPVel / (vFlock.size() - 1.0f);

    return (vPVel - b.vVel) / 8.0f;
}
