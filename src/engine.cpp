#include <engine/engine.h>
#include <utility/resource_manager.h>
#include <math/Matrices.h>
#include <math/Vectors.h>
#include <engine/cubeRenderer.h>
#include <engine/camera.h>
#include <engine/grid.h>

CubeRenderer *Renderer;
Grid *Mygrid;
Camera *MyCamera;

Engine::Engine(unsigned int width, unsigned int height)
    : Keys(), Buttons(), Width(width), Height(height), IsMouseMoving(false), IsMouseScrolling(false)
{
}

Engine::~Engine()
{
}

void Engine::Init()
{
   ResourceManager::LoadShader("shaders/cubeShader.vs", "shaders/cubeShader.fs", nullptr, "cubeShader");

   MyCamera = new Camera();
   MyCamera->Position = Vector3(0.0f, 0.0f, 5.0f);

   Shader shader = ResourceManager::GetShader("cubeShader");
   Renderer = new CubeRenderer(shader);
   Renderer->Init();
   Mygrid = new Grid(shader, 25, 25, Vector3(0.5f, 0.5f, 0.5f));
   Mygrid->Init();

}

void Engine::Update(float dt)
{
    Vector3 target = Vector3(0.0f, 0.0f, -1.0f);
    Vector3 position = Vector3(0.0f, 0.0f, 2.0f);
    Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
    Matrix4 view = MyCamera->GetViewMatrix();

    Matrix4 projection = Matrix4().perspective(MyCamera->Zoom, (float)Width/(float)Height, 1000.0f, 0.1f);
    ResourceManager::GetShader("cubeShader").Use().SetMatrix4("projection", projection);
    ResourceManager::GetShader("cubeShader").Use().SetMatrix4("view", view);
}

void Engine::ProcessInput(float dt)
{
    if(IsMouseMoving)
    {   
        if(Buttons[GLFW_MOUSE_BUTTON_MIDDLE] && Keys[GLFW_KEY_LEFT_SHIFT])
        {
            MyCamera->CameraPan(MouseOffsetX, MouseOffsetY, dt);
        }
        else if(Buttons[GLFW_MOUSE_BUTTON_MIDDLE])
        {
            MyCamera->ProcessMouseMovement(MouseOffsetX, MouseOffsetY);
            std::cout << "mouseOffsetX: " +  std::to_string(MouseOffsetX) << std::endl;
            //std::cout << "mouseOffsetY: " + std::to_string(MouseOffsetY) << std::endl;
        }
    }

    if(IsMouseScrolling)
    {
        MyCamera->ProcessMouseScroll(MouseScroll);
    }
}

void Engine::Render()
{
    Renderer->Draw();
    Mygrid->Draw();
}
