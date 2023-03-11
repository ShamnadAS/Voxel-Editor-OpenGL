#include<engine/engineManager.h>

Vector3 cubeNormal[] = 
{
    Vector3(1.0f, 0.0f, 0.0f),  //right
    Vector3(-1.0f, 0.0f, 0.0f), //left
    Vector3(0.0f, 1.0f, 0.0f),  //up
    Vector3(0.0f, -1.0f, 0.0f), //down
    Vector3(0.0f, 0.0f, 1.0f),  //forward
    Vector3(0.0f, 0.0f, -1.0f)  //backward
};

Vector3 EngineManager::CastRay(Camera &camera, unsigned int scrWidth, unsigned int scrHeight, float n, Vector2 &scrMousePos)
{
    float aspectRatio = (float)scrWidth / (float)scrHeight;
    float tangent = tanf(camera.Zoom * DEG2RAD / 2);
    float t = tangent * n;
    float r = aspectRatio * t;

    float xLength = ( 2.0f * r * scrMousePos.x / scrWidth ) - r;
    float yLength = t - ( 2.0f * t * scrMousePos.y / scrHeight );

    Vector3 a = camera.Right * xLength + camera.Up * yLength;
    Vector3 b = -n * camera.Forward; 
    Vector3 direction = a + b;

    return direction;
}   

//Hit position on the grid
Vector3 EngineManager::RayCastHit(Camera &camera, float scrWidth, float scrHeight, float n, Vector2 &scrMousePos)
{
    Vector3 direction = CastRay(camera, scrWidth, scrHeight, n, scrMousePos);
    float alpha = -camera.Position.y / direction.y;
    Vector3 hitPos;
    hitPos.x = camera.Position.x + (alpha) * direction.x;
    hitPos.y = 0.0f;
    hitPos.z = camera.Position.z + (alpha) * direction.z;

    return hitPos;
}
