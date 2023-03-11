#pragma once

#include <math/Vectors.h>
#include "cubeRenderer.h"

class Cube
{
    public:
    Cube(Vector3 position, Vector3 color);
    ~Cube();
    Vector3 Position;
    Vector3 Color;
    void Draw(CubeRenderer &renderer);
};