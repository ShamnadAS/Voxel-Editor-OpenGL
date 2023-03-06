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

unsigned int debugVAO;
std::vector<Matrix4> cubes;

void Debug();

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
   ResourceManager::LoadShader("shaders/gridShader.vs", "shaders/gridShader.fs", nullptr, "gridShader");
   ResourceManager::LoadShader("shaders/debugShader.vs", "shaders/debugShader.fs", nullptr, "debugShader");

   Shader cubeShader = ResourceManager::GetShader("cubeShader");
   Shader gridShader = ResourceManager::GetShader("gridShader");

   Mygrid = new Grid(gridShader, 26, 26, Vector3(0.5f, 0.5f, 0.5f));
   Mygrid->Init();

   MyCamera = new Camera();
   MyCamera->Position = Vector3(0.0f, 0.0f, 10.0f);
   float targetX = Mygrid->cellSize * (float)Mygrid->column / 2.0f;
   float targetZ = Mygrid->cellSize * (float)Mygrid->row / 2.0f;
   MyCamera->Target = Vector3(targetX, 0.0f, targetZ);

   Renderer = new CubeRenderer(cubeShader);
   Renderer->Init();

   Debug();
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
    ResourceManager::GetShader("gridShader").Use().SetMatrix4("projection", projection);
    ResourceManager::GetShader("gridShader").Use().SetMatrix4("view", view);
    ResourceManager::GetShader("debugShader").Use().SetMatrix4("projection", projection);
    ResourceManager::GetShader("debugShader").Use().SetMatrix4("view", view);
}

void Engine::ProcessInput(float dt)
{
    if(IsMouseMoving)
    {   
        if(Buttons[GLFW_MOUSE_BUTTON_MIDDLE] && Keys[GLFW_KEY_LEFT_SHIFT])
        {
            //MyCamera->CameraMovement(MouseOffsetX, MouseOffsetY, dt);
        }
        else if(Buttons[GLFW_MOUSE_BUTTON_MIDDLE])
        {
            MyCamera->CameraRotation(MouseOffsetX, MouseOffsetY);
            //std::cout << "mouseOffsetY: " + std::to_string(MouseOffsetY) << std::endl;
        }
    }
    
    if(Buttons[GLFW_MOUSE_BUTTON_LEFT])
    {
        Vector2 scrMousePos(MousePosX, MousePosY);
        Vector3 hitPos = Mygrid->RayCastHit(*MyCamera, Width, Height, 0.1f, scrMousePos);
        std::cout << "(" + std::to_string(hitPos.x) + "," + std::to_string(hitPos.y) + "," + 
        std::to_string(hitPos.z) + ")" << std::endl;
        Matrix4 model;
        float intPart;
        modf(hitPos.x, &intPart);
        hitPos.x = intPart + 0.5f;
        modf(hitPos.z, &intPart);
        hitPos.z = intPart + 0.5f;
        hitPos.y = 0.5f;
        model.translate(hitPos);
        cubes.push_back(model);
    }

    if(IsMouseScrolling)
    {
        MyCamera->CameraZoom(MouseScroll);
    }
}

void Engine::Render()
{
    Matrix4 model;
    model.translate(Vector3(-3.0f, 0.5f, 2.0f));
    Renderer->Draw(model);
    model.identity();
    model.translate(Vector3(5.0f, 0.5f, 5.0f));
    Renderer->Draw(model);
    Mygrid->Draw();

    for (auto & element : cubes) 
    {
        Renderer->Draw(element);
    }
   
    //Debug
    Shader debugShader = ResourceManager::GetShader("debugShader");
    debugShader.Use();
    Matrix4 debugModel;
    debugModel.scale(Mygrid->row / 2.0f);
    debugShader.SetMatrix4("model", debugModel);
    glBindVertexArray(debugVAO);
    debugShader.SetVector3f("color", Vector3(1.0f, 0.41f, 0.41f));
    glDrawArrays(GL_LINES, 0, 2);

    debugShader.SetVector3f("color", Vector3(0.30f, 0.59f, 1.0f));
    debugModel.rotateY(-90.0f);
    debugShader.SetMatrix4("model", debugModel);
    glDrawArrays(GL_LINES, 0, 2);
}

void Debug()
{
    float vertices[] =
    {
        0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
    };

    unsigned VBO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &debugVAO);
    glBindVertexArray(debugVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
