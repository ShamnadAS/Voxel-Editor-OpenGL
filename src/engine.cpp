#include <engine/engine.h>
#include <utility/resource_manager.h>
#include <math/Matrices.h>
#include <math/Vectors.h>
#include <engine/cubeRenderer.h>

CubeRenderer *Renderer;

Engine::Engine(unsigned int width, unsigned int height)
    : Keys(), Width(width), Height(height), IsMouseMoving(false)
{
}

Engine::~Engine()
{
}

void Engine::Init()
{
   ResourceManager::LoadShader("shaders/cubeShader.vs", "shaders/cubeShader.fs", nullptr, "cubeShader");

   Shader shader = ResourceManager::GetShader("cubeShader");
   Renderer = new CubeRenderer(shader);
   Renderer->Init();
}

void Engine::Update(float dt)
{
    Vector3 target = Vector3(0.0f, 0.0f, -1.0f);
    Vector3 position = Vector3(0.0f, 0.0f, 2.0f);
    Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
    Matrix4 view = Matrix4().CameraLookAt(position, target, up);

    Matrix4 projection = Matrix4().perspective(45.0f, (float)Width/(float)Height, 1000.0f, 0.1f);
    ResourceManager::GetShader("cubeShader").Use().SetMatrix4("projection", projection);
    ResourceManager::GetShader("cubeShader").Use().SetMatrix4("view", view);
}

void Engine::ProcessInput(float dt)
{
  
}

void Engine::Render()
{
    Renderer->Draw();
}
