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
    static Vector3 CastRay(Matrix4 projection, Matrix4 view, Vector2 &scrMousePos, unsigned scrWidth, unsigned scrHeight);
    static Vector3 RayCastHit(unsigned k, Camera &camera, Matrix4 projection, Matrix4 view, Vector2 &scrMousePos, unsigned scrWidth, unsigned scrHeight);
    static std::tuple<Vector3,float> RayCastHit(Camera &camera, Matrix4 projection, Matrix4 view, Vector2 &scrMousePos, Cube &cube, float scrWidth, float scrHeight);
    static Vector2 MousePosRelativeToViewPort(Vector2 &mousePos, UI* ui, GLFWwindow* window);
};

