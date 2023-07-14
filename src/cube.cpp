#include <engine/cube.h>

Cube::Cube(Vector3 position, Vector3 color)
:Position(position), Color(color), RenderCube(true)
{
    
}

Cube::~Cube()
{
    
}

void Cube::Draw(CubeRenderer &renderer)
{
    renderer.Draw(this->Position, this->Color); 
}