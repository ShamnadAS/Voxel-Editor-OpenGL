#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Engine
{
public:

    bool                    Keys[1024];
    bool                    IsMouseMoving;
    unsigned int            Width, Height;
    float                   MouseOffsetX;
    float                   MouseOffsetY;

    // constructor/destructor
    Engine(unsigned int width, unsigned int height);
    ~Engine();

    void Init();
    // game loop
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();
};