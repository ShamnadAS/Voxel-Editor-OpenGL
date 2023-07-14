#pragma once

#include<glad/glad.h>
#include<utility/shader.h>
#include<math/Matrices.h>

class CubeRenderer
{
    public:
    void Draw(Vector3 position, Vector3 color);
    CubeRenderer(Shader &shader);
    ~CubeRenderer();
    void Init();
    void SetShader(Shader shader);

    private:
    Shader shader;
    unsigned int VAO;
};
