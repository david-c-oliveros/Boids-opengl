#include "Boids.h"


unsigned int SCR_WIDTH = 1920;
unsigned int SCR_HEIGHT = 1080;
float RATIO = (float)SCR_WIDTH / (float)SCR_HEIGHT;

float fDeltaTime = 0.0f;
float fLastFrame = 0.0f;
float fCurrentFrame = 0.0f;

glm::vec3 vCameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 vFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 vUp = glm::vec3(0.0f, 1.0f, 0.0f);

Cam_Dir direction;

bool firstMouse = true;
bool bCursor = false;
bool bRun = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
float fov = 45.0f;

glm::vec3 boidPos = glm::vec3(0.0f, 0.0f, 0.0f);

Boids::Boids()
{
}


Boids::~Boids()
{
}


bool Boids::Construct()
{
    std::cout << "Screen Ratio: " << RATIO << std::endl;
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

    if (glewInit() != GLEW_OK)
    {
        std::cout << "\nERROR: Failed to initialize GLEW\n";
        return false;
    }

    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);


    /********************************************/
    /********************************************/
    /*               GL Configs                 */
    /********************************************/
    /********************************************/
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);



    /*****************************************/
    /*****************************************/
    /*               Shaders                 */
    /*****************************************/
    /*****************************************/
    shader.Create("../../shaders/vSimple.shader", "../../shaders/fSimple.shader");
    shader.Use();

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
        Render();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}


void Boids::Update(float fDeltaTime)
{
    fCurrentFrame = static_cast<float>(glfwGetTime());
    direction = NONE;
    processInput(window);

    /***********************************************/
    /*        Set Model / View / Projection        */
    /***********************************************/
    glm::mat4 mvp;
    glm::mat4 model = glm::mat4(1.0f);

    const float ratio = (float)SCR_WIDTH / (float)SCR_HEIGHT;
    glm::mat4 projection = glm::ortho(-ratio, ratio, -1.0f, 1.0f, 1.0f, -1.0f);

    shader.SetMat4("projection", projection);
    shader.SetMat4("view", glm::mat4(1.0f));
    shader.Use();

    if (bRun)
        UpdateBoids();
}


void Boids::Render()
{
    RenderUI();
    RenderBoids();

    glfwSwapBuffers(window);
    glfwPollEvents();
}


void Boids::RenderBoids()
{
    for (auto &b : vFlock)
    {
        b.Draw(shader);
    }
}


void Boids::RenderUI()
{
    glClearColor(0.05f, 0.05f, 0.25f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    if (bCursor == false)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    else
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // Start the Dear ImGui frame
    if (bUI)
    {
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
            ImGui::Checkbox("Debug", &debug);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
            ImGui::ColorEdit3("clear color", (float*)&clear_color);

            if (ImGui::Button("Button"))
                counter++;

            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

            ImGui::End();
        }

        // Debug window
        if (debug)
        {
            ImGui::Begin("Debug", &debug);
            ImGui::Text("Debug info here");
            ImGui::Text(sDebugInfo.c_str());
            ImGui::Text("fDeltaTime: %d", fDeltaTime);
            ImGui::Text("Boid #0 Position: %s", glm::to_string(vFlock[0].vPos).c_str());
            ImGui::Text("Boid #0 Velocity: %s", glm::to_string(vFlock[0].vVel).c_str());
            ImGui::Text("Boid #0 BoundPos Value: %s", glm::to_string(m_vBoundPos).c_str());

            if (ImGui::Button("Close"))
                debug = false;
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    }
}


void Boids::InitializeBoids()
{
    //glm::vec3 pos = glm::vec3(1.0f, 0.0f, 0.0f);
    //Boid b(pos, glm::vec3(0.0f, 0.0f, 0.0f));
    //b.SetColor(glm::vec3(1.0f, 0.0f, 0.0f));
    //vFlock.push_back(b);

    for (int i = 0; i < iNumBoids; i++)
    {
        glm::vec3 pos = math_util::remap(glm::vec3(i * 1.0f, i * 1.0f, 0.0f), 0.0f, (float)iNumBoids, -1.0f, 1.0f);
        std::cout << "Boid #" << i << ": " << glm::to_string(pos) << std::endl;
        Boid b(pos, glm::vec3(0.0f, 0.0f, 0.0f));
        b.SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
        vFlock.push_back(b);
    }
    vFlock[0].SetColor(glm::vec3(1.0f, 0.0f, 0.0f));
}


void Boids::UpdateBoids()
{
    //vFlock[0].vPos = boidPos;
    glm::vec3 v1, v2, v3, v4;
    for (int i = 0; i < vFlock.size(); i++)
    {
        v1 = Rule1(i);
        v2 = Rule2(i);
        v3 = Rule3(i);
        v4 = BoundPos(vFlock[i]);
        m_vBoundPos = v4;

        vFlock[i].vVel = vFlock[i].vVel + v1 + v2 + v3 + v4;
        LimitVel(vFlock[i]);
        vFlock[i].vPos = (vFlock[i].vPos + vFlock[i].vVel);
    }
}


/**************************************************************************/
/**************************************************************************/
/*                                                                        */
/*        Rule 1: Boids try to fly towards center of mass of flock        */
/*                                                                        */
/**************************************************************************/
/**************************************************************************/
glm::vec3 Boids::Rule1(int iBoidIndex)
{
    glm::vec3 vPCenterOfMass = glm::vec3(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < vFlock.size(); i++)
    {
        if (i == iBoidIndex)
            continue;

        vPCenterOfMass += vFlock[i].vPos;
    }

    vPCenterOfMass = vPCenterOfMass / (vFlock.size() - 1.0f);

    return vPCenterOfMass / 1000.0f;
}


/**********************************************************************************/
/**********************************************************************************/
/*                                                                                */
/*        Rule 2: Boids try ot keep a small distance away from other boids        */
/*                                                                                */
/**********************************************************************************/
/**********************************************************************************/
glm::vec3 Boids::Rule2(int iBoidIndex)
{
    float fInfluence = 1.0f;
    glm::vec3 c(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < vFlock.size(); i++)
    {
        if (i == iBoidIndex)
            continue;

        if (glm::length(vFlock[i].vPos - vFlock[iBoidIndex].vPos) < 0.04f)
        {
            c = c - (vFlock[i].vPos - vFlock[iBoidIndex].vPos) * fInfluence;
        }
    }

    return c;
}


/****************************************************************************/
/****************************************************************************/
/*                                                                          */
/*        Rule 3: Boids try to match the velocity of the other boids        */
/*                                                                          */
/****************************************************************************/
/****************************************************************************/
glm::vec3 Boids::Rule3(int iBoidIndex)
{
    glm::vec3 vPVel = glm::vec3(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < vFlock.size(); i++)
    {
        if (i == iBoidIndex)
            continue;
        vPVel = vPVel + vFlock[i].vVel;
    }

    vPVel = vPVel / (vFlock.size() - 1.0f);

    return (vPVel - vFlock[iBoidIndex].vVel) / 64.0f;
}


glm::vec3 Boids::BoundPos(Boid b)
{
    glm::vec3 v = glm::vec3(0.0f, 0.0f, 0.0f);

    glm::vec3 vMin = glm::vec3(-RATIO + 0.5f, -0.5, 0.0f);
    glm::vec3 vMax = glm::vec3( RATIO - 0.5f,  0.5f, 0.0f);

    bool oob = false;

    if (b.vPos.x < vMin.x)
        v.x =  0.4;
    else if(b.vPos.x > vMax.x)
        v.x = -0.4;

    if (b.vPos.y < vMin.y)
        v.y =  0.4;
    else if(b.vPos.y > vMax.y)
        v.y = -0.4;

    return v;
}


void Boids::LimitVel(Boid &b)
{
    glm::vec3 vLim = glm::vec3(0.005f, 0.005f, 0.0f);

    b.vVel = glm::clamp(b.vVel, -vLim, vLim);
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_C && action == GLFW_PRESS)
        bCursor = !bCursor;

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        bRun = !bRun;
}


void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = static_cast<float>(2.5f * fDeltaTime);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        boidPos.y += 0.01f;
        vCameraPos += cameraSpeed * vFront;
        direction = FORWARD;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        boidPos.y -= 0.01f;
        vCameraPos -= cameraSpeed * vFront;
        direction = BACKWARD;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        boidPos.x -= 0.01f;
        vCameraPos -= glm::normalize(glm::cross(vFront, vUp)) * cameraSpeed;
        direction = LEFT;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        boidPos.x += 0.01f;
        vCameraPos += glm::normalize(glm::cross(vFront, vUp)) * cameraSpeed;
        direction = RIGHT;
    }
}
