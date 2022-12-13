#include "Boids.h"


unsigned int SCR_WIDTH = 1920;
unsigned int SCR_HEIGHT = 1080;

float fDeltaTime = 0.0f;
float fLastFrame = 0.0f;
float fCurrentFrame = 0.0f;

glm::vec3 vCameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 vFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 vUp = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
bool bCursor = false;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
float fov = 45.0f;


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


    /********************************************/
    /********************************************/
    /*               GL Configs                 */
    /********************************************/
    /********************************************/
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);



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
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}


void Boids::Update(float fDeltaTime)
{
    processInput(window);

    debug_fRotAngle += 0.1f;
    //UpdateBoids();
    Render();
}


void Boids::Render()
{
    glClearColor(0.05f, 0.05f, 0.25f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glfwPollEvents();

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
        // Show another simple window
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);
            ImGui::Text("Hello from another window");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }

        if (debug)
        {
            ImGui::Begin("Debug", &debug);
            ImGui::Text("Debug info here");
            ImGui::Text(sDebugInfo.c_str());
            if (ImGui::Button("Close"))
                debug = false;
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    }


    /***********************************************/
    /*        Set Model / View / Projection        */
    /***********************************************/
    glm::mat4 projection = glm::perspective(glm::radians<float>(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 200.0f);
    glm::mat4 view = glm::lookAt(vCameraPos, vCameraPos + vFront, vUp);
    shader.SetMat4("projection", projection);
    shader.SetMat4("view", view);

    shader.Use();
    for(auto &b : vFlock)
    {
        sDebugInfo = glm::to_string(b.Draw(shader, debug_fRotAngle));
    }


    glfwSwapBuffers(window);
}


void  Boids::InitializeBoids()
{
    for (int i = 0; i < iNumBoids; i++)
    {
        Boid b(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
        b.SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
        vFlock.push_back(b);
    }
}


void Boids::UpdateBoids()
{
    glm::vec3 v1, v2, v3;
    for (int i = 0; i < vFlock.size(); i++)
    {
        v1 = Rule1(vFlock[i], i);
        v2 = Rule2(vFlock[i], i);
        v3 = Rule3(vFlock[i], i);

        vFlock[i].vVel = vFlock[i].vVel + v1 + v2 + v3;
        vFlock[i].vPos = vFlock[i].vPos + vFlock[i].vVel;
    }

}


glm::vec3 Boids::Rule1(Boid b, int iCurBoidPos)
{
    glm::vec3 vPCenterOfMass = glm::vec3(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < vFlock.size(); i++)
    {
        if (i == iCurBoidPos)
            continue;

        vPCenterOfMass += vFlock[i].vPos;
    }

    vPCenterOfMass = vPCenterOfMass / (vFlock.size() - 1.0f);

    return vPCenterOfMass / 100.0f;
}

glm::vec3 Boids::Rule2(Boid b, int iCurBoidPos)
{
    glm::vec3 c(0.0f, 0.0f, 0.0f);

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


glm::vec3 Boids::Rule3(Boid b, int iCurBoidPos)
{
    glm::vec3 vPVel;

    for (int i = 0; i < vFlock.size(); i++)
    {
        if (i == iCurBoidPos)
            continue;
        vPVel = vPVel + vFlock[i].vVel;
    }

    vPVel = vPVel / (vFlock.size() - 1.0f);

    return (vPVel - b.vVel) / 8.0f;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void mouse_callback(GLFWwindow* window, double xPosIn, double yPosIn)
{
    float xpos = static_cast<float>(xPosIn);
    float ypos = static_cast<float>(yPosIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    vFront = glm::normalize(front);
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
}


void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = static_cast<float>(2.5f * fDeltaTime);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        vCameraPos += cameraSpeed * vFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        vCameraPos -= cameraSpeed * vFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        vCameraPos -= glm::normalize(glm::cross(vFront, vUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        vCameraPos += glm::normalize(glm::cross(vFront, vUp)) * cameraSpeed;
}
