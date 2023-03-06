#include<engine/engineManager.h>

Vector3 EngineManager::CastRay(Camera &camera, unsigned int scrWidth, unsigned int scrHeight, float n, Vector2 &scrMousePos)
{
    float aspectRatio = (float)scrWidth / (float)scrHeight;
    float tangent = tanf(camera.Zoom * DEG2RAD / 2);
    float t = tangent * n;
    float r = aspectRatio * t;

    float xLength = ( 2.0f * r * scrMousePos.x / scrWidth ) - r;
    float yLength = t - ( 2.0f * t * scrMousePos.y / scrHeight );

    Vector3 a = camera.Right * xLength + camera.Up * yLength;
    Vector3 b = n * camera.Front; //use updated camera forward vector instead
    Vector3 direction = a + b;

    return direction;
}   