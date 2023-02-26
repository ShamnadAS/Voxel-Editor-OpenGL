#pragma once

#include<glad/glad.h>
#include<utility/shader.h>

class CubeRenderer
{
    public:
    void Draw();
    CubeRenderer(Shader &shader);
    ~CubeRenderer();
    void Init();

    private:
    unsigned int VAO;
    Shader shader;
};
