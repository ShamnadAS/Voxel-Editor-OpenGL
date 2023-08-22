#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <engine/engine.h>
#include <utility/resource_manager.h>
#include <engine/UI.h>
#include <iostream>
#include <utility/frame_buffer.h>
#include <engine/engineManager.h>

//pointers
UI *myUI;

// GLFW function declarations
void framebuffers_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

// The Width of the screen
const unsigned int SCREEN_WIDTH = 1366;
// The height of the screen
const unsigned int SCREEN_HEIGHT = 786; 
//Mouse Input
float lastX = 400, lastY = 300;
bool firstMouse = true;

Engine VoxelEngine(SCREEN_WIDTH, SCREEN_HEIGHT);
FrameBuffer *Fbo;

int main(int argc, char *argv[])
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    //glfwWindowHint(GLFW_RESIZABLE, false);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "VoxelEngine", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //input callbacks
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    glfwSetFramebufferSizeCallback(window, framebuffers_size_callback); //check online

    // OpenGL configuration
    // --------------------
    //glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE);

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // initialize game
    // ---------------
    VoxelEngine.Init(); 
    Fbo = new FrameBuffer();
    Fbo->Init(VoxelEngine.Width, VoxelEngine.Height);

    // deltaTime variables
    // -------------------
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Setup Dear ImGui context
    myUI = new UI(VoxelEngine);
    myUI->OnCreate("#version 460", window);

    // Our state
    bool show_demo_window = false;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    while (!glfwWindowShouldClose(window))
    {
        // calculate delta time
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();
       
        // Start the Dear ImGui frame
        myUI->OnBegin();
        if(show_demo_window)
            ImGui::ShowDemoWindow();
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
        myUI->ToolBar();
        myUI->ColorPalette();
        myUI->ColorSelector();
        myUI->MenuBar();
        myUI->Debug(window);
        myUI->ViewPort(Fbo->InterTextureId);
        myUI->ControlBar();
        
        //update the viewport dimension
        glViewport(0, 0, VoxelEngine.ViewportSize.x, VoxelEngine.ViewportSize.y);
        VoxelEngine.Width = VoxelEngine.ViewportSize.x;
        VoxelEngine.Height = VoxelEngine.ViewportSize.y;

        // manage user input
        // -----------------
        VoxelEngine.ProcessInput(deltaTime);
        VoxelEngine.IsMouseMoving = false;
        VoxelEngine.IsMouseScrolling = false;
        //VoxelEngine.Buttons[0] = false;
        //VoxelEngine.Buttons[1] = false;

        // update game state
        // -----------------
        VoxelEngine.Update(deltaTime);

        // render
        // ------
        Fbo->Bind();
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        Fbo->UpdateDimensions(VoxelEngine.Width, VoxelEngine.Height);
        VoxelEngine.Render();
        Fbo->ResolveTexture(VoxelEngine.Width, VoxelEngine.Height);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        myUI->Render();

        // Get the position of the window
        int xPos, yPos;
        glfwGetWindowPos(window, &xPos, &yPos);

        glfwSwapBuffers(window);
    }

    // delete all resources as loaded using the resource manager
    // ---------------------------------------------------------
    ResourceManager::Clear();

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // when a user presses the escape key, we set the WindowShouldClose property to true, closing the application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
            VoxelEngine.Keys[key] = true;
        else if (action == GLFW_RELEASE)
            VoxelEngine.Keys[key] = false;
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    VoxelEngine.IsMouseMoving = true;

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	VoxelEngine.MouseOffsetX = xpos - lastX;
	VoxelEngine.MouseOffsetY = ypos - lastY;

    Vector2 mousePos = Vector2(xpos, ypos);
    Vector2 mousePosRelativeToViewport = EngineManager().MousePosRelativeToViewPort(mousePos, myUI, window);

    VoxelEngine.MousePosX = mousePosRelativeToViewport.x;
    VoxelEngine.MousePosY = mousePosRelativeToViewport.y;
	lastX = xpos;
	lastY = ypos;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button >= 0 && button <= 2)
    {
        if(action == GLFW_PRESS)
        {
            VoxelEngine.Buttons[button] = true;
            if(button == 2)
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        else if(action == GLFW_RELEASE)
        {
            VoxelEngine.Buttons[button] = false;
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    VoxelEngine.IsMouseScrolling = true;
    VoxelEngine.MouseScroll = static_cast<float>(yoffset);
}

void framebuffers_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
