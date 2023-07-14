#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math/Vectors.h>
#include <engine/cubeRenderer.h>

class Engine
{
public:

    bool                    Keys[1024];
    bool                    Buttons[3];
    bool                    IsMouseMoving;
    bool                    IsMouseScrolling;
    unsigned int            Width, Height;
    float                   MouseOffsetX;
    float                   MouseOffsetY;
    float                   MouseScroll;
    float                   MousePosX;
    float                   MousePosY;

    unsigned int            ActiveTool;
    Vector3                 ActiveColor;

    // constructor/destructor
    Engine(unsigned int width, unsigned int height);
    ~Engine();

    void Init();
    // game loop
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();
};