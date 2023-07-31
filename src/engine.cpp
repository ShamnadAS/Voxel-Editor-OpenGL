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
unsigned lastCubeIndex = 0;
unsigned gridCount = 15;

void Debug();   

Engine::Engine(unsigned int width, unsigned int height)
    : Keys(), Buttons(), Width(width), Height(height), IsMouseMoving(false), IsMouseScrolling(false),ActiveTool(0),
    ActiveColor(Vector3(1.0f, 1.0f, 1.0f))
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
   ResourceManager::LoadShader("shaders/cubeShaderLit.vs", "shaders/cubeShaderLit.fs", nullptr, "cubeShaderLit");


   ActiveShader = ResourceManager::GetShader("cubeShaderLit");
   Shader gridShader = ResourceManager::GetShader("gridShader");

   Mygrid = new Grid(gridShader, gridCount, gridCount, Vector3(0.5f, 0.5f, 0.5f));

   MyCamera = new Camera();
   float targetX = Mygrid->cellSize * (float)Mygrid->column / 2.0f;
   float targetZ = Mygrid->cellSize * (float)Mygrid->row / 2.0f;
   MyCamera->Target = Vector3(targetX, 0.0f, targetZ);  

   Renderer = new CubeRenderer(ActiveShader);
   Debug();
}

Vector3 lightDirection(-2.0f, -1.0f, -1.5f);
Matrix4 projection;

void Engine::Update(float dt)
{
    Matrix4 view = MyCamera->GetViewMatrix();
 
    projection = Matrix4().perspective(MyCamera->Fov, (float)Width/(float)Height, FAR, NEAR);
    ResourceManager::GetShader("cubeShader").Use().SetMatrix4("projection", projection);
    ResourceManager::GetShader("cubeShader").Use().SetMatrix4("view", view);

    ResourceManager::GetShader("cubeShaderLit").Use().SetMatrix4("projection", projection);
    ResourceManager::GetShader("cubeShaderLit").Use().SetMatrix4("view", view);
    ResourceManager::GetShader("cubeShaderLit").Use().SetVector3f("dirLight.direction", lightDirection);
    ResourceManager::GetShader("cubeShaderLit").Use().SetVector3f("dirLight.ambient", Vector3(0.5f, 0.5f, 0.5f));
    ResourceManager::GetShader("cubeShaderLit").Use().SetVector3f("dirLight.diffuse", Vector3(0.8f, 0.8f, 0.8f));
    ResourceManager::GetShader("cubeShaderLit").Use().SetVector3f("viewPos", MyCamera->Position);

    ResourceManager::GetShader("gridShader").Use().SetMatrix4("projection", projection);
    ResourceManager::GetShader("gridShader").Use().SetMatrix4("view", view);
    ResourceManager::GetShader("debugShader").Use().SetMatrix4("projection", projection);
    ResourceManager::GetShader("debugShader").Use().SetMatrix4("view", view);
}

Vector3 startPos(0.0f, 0.0f, 0.0f);
unsigned k;
std::vector<Cube> rectCubes; 

void Engine::ProcessInput(float dt)
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
    
    if(Buttons[GLFW_MOUSE_BUTTON_LEFT] && IsMouseInViewPort)
    {
        Vector2 scrMousePos(MousePosX, MousePosY); //screen space coordinates
        float t = 1000.0f;

        switch (ActiveTool)
        {
        case 0:
        {
            Vector3 position(0.0f, 0.0f, 0.0f);
            //Vector3 color(1.0f, 1.0f, 1.0f);
            bool cubePlaced = false;
           
            for(int i = 0; i < lastCubeIndex; i++)
            {
                //position = EngineManager().RayCastHit(*MyCamera, Width, Height, 0.1f, scrMousePos, cube);
                std::tuple<Vector3, float> rayhit = EngineManager().RayCastHit(*MyCamera, projection, MyCamera->GetViewMatrix(), scrMousePos, cubes[i], Width, Height);

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
                    Cube cube1(position, ActiveColor);
                    cubes.push_back(cube1); 
                    cubePlaced = true;
                    
                }
            }

            Vector3 hitPos = EngineManager().RayCastHit(0, *MyCamera, projection, MyCamera->GetViewMatrix(), scrMousePos, Width, Height);

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
                    Cube cube(position, ActiveColor);
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
                std::tuple<Vector3, float> rayhit = EngineManager().RayCastHit(*MyCamera, projection, MyCamera->GetViewMatrix(), scrMousePos, cubes[i], Width, Height);

                if(get<0>(rayhit) != Vector3(0.0f, 0.0f, 0.0f) && t > std::get<1>(rayhit))
                {
                    t = std::get<1>(rayhit);
                    index = i;
                }  
            }
            
            if(index != -1 && cubes[index].RenderCube)
            {
                cubes[index].RenderCube = false;
                //cubes.erase(cubes.begin() + index);
            }
            break;
        }
           
        case 2:
        {
            int index = -1;
            for (unsigned i = 0; i < cubes.size(); i++)
            {
                std::tuple<Vector3, float> rayhit = EngineManager().RayCastHit(*MyCamera, projection, MyCamera->GetViewMatrix(), scrMousePos, cubes[i], Width, Height);

                if(get<0>(rayhit) != Vector3(0.0f, 0.0f, 0.0f) && t > std::get<1>(rayhit))
                {
                    t = std::get<1>(rayhit);
                    index = i;
                }  
            }
            
            if(index != -1)
            {
                cubes[index].Color = ActiveColor;
            }
            break;
        }

        case 3:
        {
            float intPart;
            rectCubes.clear();

            if(startPos == Vector3(0.0f, 0.0f, 0.0f))
            {
                for(int i = 0; i < lastCubeIndex; i++)
                {
                    //position = EngineManager().RayCastHit(*MyCamera, Width, Height, 0.1f, scrMousePos, cube);
                    std::tuple<Vector3, float> rayhit = EngineManager().RayCastHit(*MyCamera, projection, MyCamera->GetViewMatrix(), scrMousePos, cubes[i], Width, Height);

                    if(get<0>(rayhit) != Vector3(0.0f, 0.0f, 0.0f) && t > std::get<1>(rayhit))
                    {
                        startPos = std::get<0>(rayhit);
                        t = std::get<1>(rayhit); 
                    }
                }

                if(startPos == Vector3(0.0f, 0.0f, 0.0f))
                {
                    startPos = EngineManager().RayCastHit(0, *MyCamera, projection, MyCamera->GetViewMatrix(), scrMousePos, Width, Height);
                
                    modf(startPos.x, &intPart);
                    startPos.x = intPart + 0.5f;
                    modf(startPos.z, &intPart);
                    startPos.z = intPart + 0.5f;
                    startPos.y = 0.5f;
                    k = 0;
                }
                else
                {
                    k = startPos.y - 0.5f;
                }
            }

            Vector3 currentPos = EngineManager().RayCastHit(k, *MyCamera, projection, MyCamera->GetViewMatrix(), scrMousePos, Width, Height);

            modf(currentPos.x, &intPart);
            currentPos.x = intPart + 0.5f;
            modf(currentPos.z, &intPart);
            currentPos.z = intPart + 0.5f;
            currentPos.y = currentPos.y + 0.5f;
            std::cout << currentPos << std::endl;
            
            Vector3 diff = currentPos - startPos;

            for (unsigned i = 0; i <= abs(diff.x); i++)
            {   
                for (unsigned j = 0; j <= abs(diff.z); j++)
                {
                    int x = diff.x < 0 ? -i : i;
                    int z = diff.z < 0 ? -j : j;

                    Vector3 position(startPos + Vector3(x, 0.0f, z));
                    Cube cube(position, ActiveColor);
                    rectCubes.push_back(cube);
                }
            }

            break;
        }
            
        default:
            break;
        }
    }
    else
    {
        if(rectCubes.size() > 0)
        {
            startPos = Vector3(0.0f, 0.0f, 0.0f);
            cubes.insert(cubes.end(), rectCubes.begin(), rectCubes.end());
            rectCubes.clear();
        }

        for (int i = 0; i < cubes.size(); i++)
        {
            if(!cubes[i].RenderCube)
            {
                cubes.erase(cubes.begin() + i);
            }
        }

        lastCubeIndex = cubes.size();
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

    Renderer->SetShader(ActiveShader);

    for (auto & cube : cubes) 
    {
        if(cube.RenderCube)
            cube.Draw(*Renderer);
    }

    for (auto & cube : rectCubes)
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
