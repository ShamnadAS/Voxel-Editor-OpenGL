#pragma once

#include <math/Vectors.h>
#include <vector>
#include "camera.h"
#include "cube.h"

class EngineManager
{
public:
    static Vector3 CastRay(Camera &camera, unsigned int scrWidth, unsigned int scrHeight, float n, Vector2 &scrMousePos);
    static Vector3 RayCastHit(Camera &camera, float scrWidth, float scrHeight, float n, Vector2 &scrMousePos);
    static void SetValues(float width, float height, float n, Camera &camera);
};

