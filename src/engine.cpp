#include <engine/engine.h>
#include <utility/resource_manager.h>
#include <math/Matrices.h>
#include <math/Vectors.h>
#include <engine/cubeRenderer.h>
#include <engine/camera.h>
#include <engine/grid.h>
#include <engine/engineManager.h>
#include <engine/cube.h>
using namespace std;

CubeRenderer *Renderer;
Grid *Mygrid;
Camera *MyCamera;

unsigned int debugVAO;
std::vector<Cube> cubes;

void Debug();   

Engine::Engine(unsigned int width, unsigned int height)
    : Keys(), Buttons(), Width(width), Height(height), IsMouseMoving(false), IsMouseScrolling(false),ToolState(0)
{
}

Engine::~Engine()
{
    delete Renderer;
    delete Mygrid;
    delete MyCamera;
}

void Engine::Init()
{
   ResourceManager::LoadShader("shaders/cubeShader.vs", "shaders/cubeShader.fs", nullptr, "cubeShader");
   ResourceManager::LoadShader("shaders/gridShader.vs", "shaders/gridShader.fs", nullptr, "gridShader");
   ResourceManager::LoadShader("shaders/debugShader.vs", "shaders/debugShader.fs", nullptr, "debugShader");

   Shader cubeShader = ResourceManager::GetShader("cubeShader");
   Shader gridShader = ResourceManager::GetShader("gridShader");

   Mygrid = new Grid(gridShader, 10, 10, Vector3(0.5f, 0.5f, 0.5f));

   MyCamera = new Camera();
   MyCamera->Position = Vector3(-3.0f, 3.0f, -3.0f);
   float targetX = Mygrid->cellSize * (float)Mygrid->column / 2.0f;
   float targetZ = Mygrid->cellSize * (float)Mygrid->row / 2.0f;
   MyCamera->Target = Vector3(targetX, 0.0f, targetZ);

   Renderer = new CubeRenderer(cubeShader);

   //Debug();
}

void Engine::Update(float dt)
{
    Matrix4 view = MyCamera->GetViewMatrix();
 
    Matrix4 projection = Matrix4().perspective(MyCamera->Zoom, (float)Width/(float)Height, 1000.0f, 0.01f);
    ResourceManager::GetShader("cubeShader").Use().SetMatrix4("projection", projection);
    ResourceManager::GetShader("cubeShader").Use().SetMatrix4("view", view);
    ResourceManager::GetShader("gridShader").Use().SetMatrix4("projection", projection);
    ResourceManager::GetShader("gridShader").Use().SetMatrix4("view", view);
    ResourceManager::GetShader("debugShader").Use().SetMatrix4("projection", projection);
    ResourceManager::GetShader("debugShader").Use().SetMatrix4("view", view);
}

void Engine::ProcessInput(float dt, Vector3 &color)
{
    if(IsMouseMoving)
    {   
        if(Buttons[GLFW_MOUSE_BUTTON_MIDDLE] && Keys[GLFW_KEY_LEFT_SHIFT])
        {
            MyCamera->CameraPanning(MouseOffsetX, MouseOffsetY, dt);
        }
        else if(Buttons[GLFW_MOUSE_BUTTON_MIDDLE])
        {
            MyCamera->CameraRotation(MouseOffsetX, MouseOffsetY);
        }
    }
    
    if(Buttons[GLFW_MOUSE_BUTTON_LEFT])
    {
        Vector2 scrMousePos(MousePosX, MousePosY);
        float t = 1000.0f;

        switch (ToolState)
        {
        case 0:
        {
            Vector3 position(0.0f, 0.0f, 0.0f);
            //Vector3 color(1.0f, 1.0f, 1.0f);
            bool cubePlaced = false;

            for(auto &cube : cubes)
            {
                //position = EngineManager().RayCastHit(*MyCamera, Width, Height, 0.1f, scrMousePos, cube);
                std::tuple<Vector3, float> rayhit = EngineManager().RayCastHit(*MyCamera, Width, Height, 0.1f, scrMousePos, cube);

                if(get<0>(rayhit) != Vector3(0.0f, 0.0f, 0.0f) && t > std::get<1>(rayhit))
                {
                    position = std::get<0>(rayhit);
                    t = std::get<1>(rayhit);
                }
            }

            if(position !=  Vector3(0.0f, 0.0f, 0.0f))
            {
                for(auto &cube : cubes)
                {
                    if(position == cube.Position)
                    {
                        cubePlaced = true;
                        break;
                    }
                }

                if(!cubePlaced)
                {
                    Cube cube1(position, color);
                    cubes.push_back(cube1); 
                    cubePlaced = true;
                }
            }

            Vector3 hitPos = EngineManager().RayCastHit(*MyCamera, Width, Height, 0.1f, scrMousePos);

            if(hitPos.x > 0 && hitPos.x < Mygrid->column * Mygrid->cellSize 
            && hitPos.z > 0 && hitPos.z < Mygrid->row * Mygrid->cellSize)
            {
                float intPart;
                modf(hitPos.x, &intPart);
                position.x = intPart + 0.5f;
                modf(hitPos.z, &intPart);
                position.z = intPart + 0.5f;
                position.y = 0.5f;

                for(auto &cube : cubes)
                {
                    if(position == cube.Position)
                    {
                        cubePlaced = true;
                        break;
                    }
                }
                if(!cubePlaced)
                {
                    Cube cube(position, color);
                    cubes.push_back(cube);
                }
            }
            break;
        }

        case 1:
        {
            int index = -1;
            for (unsigned i = 0; i < cubes.size(); i++)
            {
                std::tuple<Vector3, float> rayhit = EngineManager().RayCastHit(*MyCamera, Width, Height, 0.1f, scrMousePos, cubes[i]);

                if(get<0>(rayhit) != Vector3(0.0f, 0.0f, 0.0f) && t > std::get<1>(rayhit))
                {
                    t = std::get<1>(rayhit);
                    index = i;
                }  
            }
            
            if(index != -1)
            {
                cubes.erase(cubes.begin() + index);
            }
            break;
        }
           
        case 2:
            //paint tool
            break;
        default:
            break;
        }
    }

    if(Buttons[GLFW_MOUSE_BUTTON_RIGHT])
    {
        //camera panning
    }

    if(IsMouseScrolling)
    {
        //MyCamera->CameraZoom(MouseScroll);
        MyCamera->CameraMoveAlongForwardAxis(MouseScroll);
    }
}

void Engine::Render()
{
    Mygrid->Draw();

    for (auto & cube : cubes) 
    {
        cube.Draw(*Renderer);
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
