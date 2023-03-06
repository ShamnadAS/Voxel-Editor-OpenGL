#pragma once

#include <math/Vectors.h>
#include <utility/shader.h>
#include <engine/engineManager.h>

class Grid
{
    public:
    int row;
    int column;
    float cellSize = 1.0f;
    Vector3 color;
    Grid(Shader &shader, int row, int column, Vector3 color);
    ~Grid();
    void Draw();
    void Init();
    Vector3 RayCastHit(Camera &camera, float scrWidth, float scrHeight, float n, Vector2 &scrMousePos);

    private:
    unsigned int rowVAO;
    unsigned int columnVAO;
    Shader shader;
};