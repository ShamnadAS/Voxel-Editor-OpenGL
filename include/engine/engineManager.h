#pragma once

#include <math/Vectors.h>
#include <vector>
#include "camera.h"
#include "cube.h"
#include <engine/UI.h>
#include <GLFW/glfw3.h>

class EngineManager
{
public:
    static Vector3 CastRay(Camera &camera, unsigned int scrWidth, unsigned int scrHeight, float n, Vector2 &scrMousePos);
    static Vector3 RayCastHit(Camera &camera, float scrWidth, float scrHeight, float n, Vector2 &scrMousePos);
    static std::tuple<Vector3,float> RayCastHit(Camera &camera, float scrWidth, float scrHeight, float n, Vector2 &scrMousePos, Cube &cube);
    static Vector2 MousePosRelativeToViewPort(Vector2 &mousePos, UI* ui, GLFWwindow* window);
};

