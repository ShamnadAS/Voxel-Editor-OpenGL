#pragma once

#include <math/Vectors.h>
#include <vector>
#include "camera.h"

class EngineManager
{
public:
    Vector3 CastRay(Camera &camera, unsigned int scrWidth, unsigned int scrHeight, float n, Vector2 &scrMousePos);
};