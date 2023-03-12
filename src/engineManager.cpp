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

    return direction.normalize();
}   

//Hit position on the grid
Vector3 EngineManager::RayCastHit(Camera &camera, float scrWidth, float scrHeight, float n, Vector2 &scrMousePos)
{
    Vector3 direction = CastRay(camera, scrWidth, scrHeight, n, scrMousePos);
    float t = -camera.Position.y / direction.y;
    Vector3 hitPos;
    hitPos = camera.Position + ( t * direction );
    return hitPos;
}

Vector3 EngineManager::RayCastHit(Camera &camera, float scrWidth, float scrHeight, float n, Vector2 &scrMousePos, Cube &cube)
{
    Vector3 direction = CastRay(camera, scrWidth, scrHeight, n, scrMousePos);
    Vector3 normal(0.0f, 1.0f, 0.0f);
    Vector3 face = cube.Position + ( normal * 0.5f );
    float t = (face -  camera.Position).dot(normal) / direction.dot(normal);
    Vector3 hitPos = camera.Position + ( t * direction);
    Vector3 hitPosFace = hitPos - face;
    
    Vector3 value(0.0f, 0.0f, 0.0f);
    if(abs(hitPosFace.x) < 0.5f && abs(hitPosFace.z) < 0.5f)
    {
        value = face + normal / 2.0f;
    }
    
    return value;
}