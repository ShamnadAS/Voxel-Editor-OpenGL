#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <engine/engine.h>
#include <utility/resource_manager.h>

#include <iostream>

// GLFW function declarations
void framebuffers_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

// The Width of the screen
const unsigned int SCREEN_WIDTH = 800;
// The height of the screen
const unsigned int SCREEN_HEIGHT = 600;
//Mouse Input
float lastX = 400, lastY = 300;
bool firstMouse = true;

Engine VoxelEngine(SCREEN_WIDTH, SCREEN_HEIGHT);

int main(int argc, char *argv[])
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    glfwWindowHint(GLFW_RESIZABLE, false);

    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "VoxelEngine", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffers_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // OpenGL configuration
    // --------------------
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // initialize game
    // ---------------
    VoxelEngine.Init();

    // deltaTime variables
    // -------------------
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(window))
    {
        // calculate delta time
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();

        // manage user input
        // -----------------
        VoxelEngine.ProcessInput(deltaTime);
        VoxelEngine.IsMouseMoving = false;
        VoxelEngine.IsMouseScrolling = false;
        VoxelEngine.Buttons[0] = false;
        VoxelEngine.Buttons[1] = false;
        // update game state
        // -----------------
        VoxelEngine.Update(deltaTime);

        // render
        // ------
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        VoxelEngine.Render();

        glfwSwapBuffers(window);
    }

    // delete all resources as loaded using the resource manager
    // ---------------------------------------------------------
    ResourceManager::Clear();

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
	VoxelEngine.MouseOffsetY = lastY - ypos;  // reversed: y ranges bottom to top
    VoxelEngine.MousePosX = xpos;
    VoxelEngine.MousePosY = ypos;
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