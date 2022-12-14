#include "Boids.h"


unsigned int SCR_WIDTH = 2000;
unsigned int SCR_HEIGHT = 2000;

//unsigned int SCR_WIDTH = 1920;
//unsigned int SCR_HEIGHT = 1080;

float RATIO = (float)SCR_WIDTH / (float)SCR_HEIGHT;

float fDeltaTime = 0.0f;
float fLastFrame = 0.0f;
float fCurrentFrame = 0.0f;

//glm::vec3 vCameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
//glm::vec3 vFront = glm::vec3(0.0f, 0.0f, -1.0f);
//glm::vec3 vUp = glm::vec3(0.0f, 1.0f, 0.0f);

Cam_Dir direction;

bool firstMouse = true;
bool bCursor = false;
bool bRun = false;
bool bScatter = false;
AppState eState = NORMAL;

glm::vec3 vDebugBoidPos = glm::vec3(0.0f, 0.0f, 8.0f);

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
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
    InitBoids();
    InitDebug();

    cBg = std::make_unique<Quad>(glm::vec3(0.0f, 0.0f, 0.0f));
    cBg->SetColor(glm::vec3(0.2f, 0.2f, 0.5f));
    cBg->SetScale(glm::vec3(400.0f, 400.0f, 0.0f));


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

    double x;
    double y;
    glfwGetCursorPos(window, &x, &y);
    m_vCursorPos = glm::vec3(x, y, 0.0f);
    //m_vCursorPos.x = math_util::remap((float)x, 0.0f, SCR_WIDTH, -RATIO, RATIO);
    //m_vCursorPos.y = math_util::remap((float)y, 0.0f, SCR_HEIGHT, 1.0f, -1.0f);


    processInput(window);

    /***********************************************/
    /*        Set Model / View / Projection        */
    /***********************************************/
    glm::mat4 mvp;
    glm::mat4 model = glm::mat4(1.0f);

    const float ratio = (float)SCR_WIDTH / (float)SCR_HEIGHT;

    glm::vec3 vCameraPos_ortho = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 vCameraPos_persp = glm::vec3(0.0f, 0.0f, -100.0f);
    glm::vec3 vLookDir = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 vTargetPos = vCameraPos_persp + vLookDir;
    //glm::vec3 vTargetPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 vUp = glm::vec3(0.0f, 1.0f, 0.0f);

    /******************************/
    /*        Orthographic        */
    /******************************/
    //glm::mat4 projection = glm::lookAt(vCameraPos_ortho, vTargetPos, glm::vec3(0.0f, 1.0f, 0.0f));
    //glm::mat4 projection = glm::ortho(0.0f, SCR_WIDTH, SCR_HEIGHT, 0.0f, -1.0f, 0.0f);
    // TODO - Which of the above projection matices do I use???  And what do I use for the view matrix??
    //glm::mat4 view = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 1000.0f);

    /*****************************/
    /*        Perspective        */
    /*****************************/
    glm::mat4 projection = glm::perspective(glm::radians<float>(90.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(vCameraPos_persp, vTargetPos, vUp);

    shader.SetMat4("projection", projection);
    shader.SetMat4("view", view);
    shader.Use();

    switch(eState)
    {
        case NORMAL:
            if (bRun)
                UpdateBoids();
            break;

        case DEBUG:
            if (vFlock.size() > 0)
                ClearBoids();
            UpdateDebug();
            break;
    }
}


void Boids::Render()
{
    glClearColor(0.05f, 0.05f, 0.25f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    cBg->Draw(shader);

    RenderUI();

    switch(eState)
    {
        case NORMAL:
            RenderBoids();
            break;

        case DEBUG:
            RenderDebugBoid();
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
}


void Boids::RenderBoids()
{
    cControlBoid->Draw(shader);

    for (auto &b : vFlock)
    {
        b.Draw(shader);
    }
}


void Boids::RenderDebugBoid()
{
    cControlBoid->Draw(shader);
    cCursorBoid->Draw(shader);
}


void Boids::RenderUI()
{
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
            ImGui::Checkbox("Debug", &bDebugScreen);
            ImGui::Checkbox("Boid Control Panel", &boid_control_panel);

            //ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
            //ImGui::ColorEdit3("clear color", (float*)&clear_color);

            //if (ImGui::Button("Button"))
            //    counter++;

            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

            ImGui::End();
        }

        // Debug window
        if (bDebugScreen)
        {
            ImGui::Begin("Debug", &bDebugScreen);
            ImGui::Text("Debug info here");
            ImGui::Text(sDebugInfo.c_str());
            ImGui::Text("fDeltaTime: %f", fDeltaTime);
            ImGui::Text("App State: %s", GetState().c_str());
            ImGui::Text("Cursor Position: %s", glm::to_string(m_vCursorPos).c_str());
            ImGui::Text("Cursor World Position: %s", glm::to_string(m_vCursorWorldPos).c_str());
            ImGui::Text("ray_eye: %s", glm::to_string(ray_eye).c_str());
            ImGui::Text("Boid #0 Position: %s", glm::to_string(cControlBoid->vPos).c_str());
            ImGui::Text("Boid #0 Velocity: %s", glm::to_string(vFlock[0].vVel).c_str());

            if (ImGui::Button("Close"))
                bDebugScreen = false;
            ImGui::End();
        }

        // Boid Configs
        if (boid_control_panel)
        {

            ImGui::Begin("Boid Control Panel");

            static int item_current = (int)eState;
            const char* items[] = { "Normal", "Debug" };
            ImGui::Combo("One liner test", &item_current, &SetState, items, 2);

            ImGui::Checkbox("Rule 1", &bRule1);
            ImGui::Checkbox("Rule 2", &bRule2);
            ImGui::Checkbox("Rule 3", &bRule3);
            ImGui::Checkbox("Bound Position", &bBoundPos);
            ImGui::Checkbox("Tend To Place", &bTendToPlace);
            ImGui::Checkbox("Strong Wind", &bStrongWind);

            ImGui::DragInt("Rule #1 Value", &fRule1_scalar);
            ImGui::DragInt("Rule #2 Value", &fRule2_scalar);
            ImGui::DragInt("Rule #3 Value", &fRule3_scalar);
            
            if (ImGui::Button("Reset Simulation"))
            {
                ClearBoids();
                InitBoids();
            }

            if (ImGui::Button("Close"))
                boid_control_panel = false;

            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    }
}


void Boids::InitBoids()
{
    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
    cControlBoid = std::make_unique<Boid> (pos, glm::vec3(0.0f));
    cControlBoid->SetColor(glm::vec3(1.0f, 0.0f, 0.5f));

    for (int i = 0; i < iNumBoids; i++)
    {
        glm::vec3 pos = math_util::remap(glm::vec3(i * 1.0f, i * 1.0f, 0.0f), 0.0f, (float)iNumBoids, -1.0f, 1.0f);
        Boid b(pos, glm::vec3(0.0f, 0.0f, 0.0f));
        b.SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
        vFlock.push_back(b);
    }
}


void Boids::InitDebug()
{
    glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f);
    cControlBoid = std::make_unique<Boid> (pos, glm::vec3(0.0f));
    cControlBoid->SetColor(glm::vec3(1.0f, 0.0f, 0.5f));

    cCursorBoid = std::make_unique<Boid> (pos, glm::vec3(0.0f));
    cCursorBoid->SetColor(glm::vec3(0.0f, 0.5f, 1.0f));
}


void Boids::ClearBoids()
{
    vFlock.clear();
}


/************************************************/
/************************************************/
/*                                              */
/*        Move all boids to new position        */
/*                                              */
/************************************************/
/************************************************/
void Boids::UpdateBoids()
{
    cControlBoid->vPos = vDebugBoidPos;

    glm::vec3 v1 = glm::vec3(0.0f);
    glm::vec3 v2 = glm::vec3(0.0f);
    glm::vec3 v3 = glm::vec3(0.0f);
    glm::vec3 v4 = glm::vec3(0.0f);
    glm::vec3 v5 = glm::vec3(0.0f);
    glm::vec3 v6 = glm::vec3(0.0f);
    glm::vec3 v7 = glm::vec3(0.0f);

    for (int i = 0; i < vFlock.size(); i++)
    {
        if (!bScatter & bRule1)
            v1 = Rule1(i);

        if (bRule1)
            v2 = Rule2(i);

        if (!bRule1)
            v3 = Rule3(i);

        if (bBoundPos)
            v4 = BoundPos(vFlock[i]);

        if (bTendToPlace)
            v5 = TendToPlace(vFlock[i], cControlBoid->vPos);
        else
            v7 = TendAwayFromPlace(vFlock[i], m_vCursorPos);

        if (bStrongWind)
            v6 = StrongWind();

        m_vBoundPos = v4;

        vFlock[i].vVel = vFlock[i].vVel + v1 + v2 + v3 + v4 + v5 + v7;
        LimitVel(vFlock[i]);
        vFlock[i].vPos = (vFlock[i].vPos + vFlock[i].vVel);
    }
}


void Boids::UpdateDebug()
{
    cControlBoid->vPos = vDebugBoidPos;
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

    return (vPCenterOfMass - vFlock[iBoidIndex].vPos) / (float)fRule1_scalar;
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
    float fMinDis = 1.0f;
    glm::vec3 c(0.0f, 0.0f, 0.0f);

    for (int i = 0; i < vFlock.size(); i++)
    {
        if (i == iBoidIndex)
            continue;

        if (glm::length(vFlock[i].vPos - vFlock[iBoidIndex].vPos) < fMinDis)
        {
            c = c - (vFlock[i].vPos - vFlock[iBoidIndex].vPos) * (float)fRule2_scalar;
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

    return (vPVel - vFlock[iBoidIndex].vVel) / (float)fRule3_scalar;
}


glm::vec3 Boids::BoundPos(Boid b)
{
    glm::vec3 v = glm::vec3(0.0f, 0.0f, 0.0f);

    glm::vec3 vMin = glm::vec3( -100.0f, -100.0f, 0.0f );
    glm::vec3 vMax = glm::vec3(  100.0f,  100.0f, 0.0f );

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
    glm::vec3 vLim = glm::vec3(0.5f, 0.5f, 0.0f);

    b.vVel = glm::clamp(b.vVel, -vLim, vLim);
}


glm::vec3 Boids::TendToPlace(Boid b, glm::vec3 vPlace)
{
    return (vPlace - b.vPos) / 100.0f;
}


glm::vec3 Boids::TendAwayFromPlace(Boid b, glm::vec3 vPlace)
{
    float m = 1.0f;

    return (TendToPlace(b, vPlace) * -m);
}


glm::vec3 Boids::StrongWind()
{
    return glm::vec3(0.01f, -0.001f, 0.0f);
}



/************************************/
/************************************/
/*                                  */
/*        Callback Functions        */
/*                                  */
/************************************/
/************************************/
bool SetState(void* data, int n, const char** out_str)
{
    //std::cout << ((const char**)data)[n] << std::endl;
    eState = (AppState)n;
    *out_str = ((const char**)data)[n];

    return true;
}


std::string GetState()
{
    std::string states[] = { "Normal", "Debug" };

    return states[eState];
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
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

    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
        bScatter = true;
    else
        bScatter = false;

    float cameraSpeed = static_cast<float>(2.5f * fDeltaTime);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        vDebugBoidPos.y += 1.0f;
        //vCameraPos += cameraSpeed * vFront;
        direction = FORWARD;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        vDebugBoidPos.y -= 1.0f;
        //vCameraPos -= cameraSpeed * vFront;
        direction = BACKWARD;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        vDebugBoidPos.x += 1.0f;
        //vCameraPos -= glm::normalize(glm::cross(vFront, vUp)) * cameraSpeed;
        direction = LEFT;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        vDebugBoidPos.x -= 1.0f;
        //vCameraPos += glm::normalize(glm::cross(vFront, vUp)) * cameraSpeed;
        direction = RIGHT;
    }
}
