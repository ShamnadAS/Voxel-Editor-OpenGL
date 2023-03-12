#include<engine/engineManager.h>
#include<tuple>
using namespace std;

Vector3 Normal[] = 
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
    Vector3 value(0.0f, 0.0f, 0.0f);
    Vector3 direction = CastRay(camera, scrWidth, scrHeight, n, scrMousePos);
    float t = -camera.Position.y / direction.y;
    if( t > 0)
    {
        value = camera.Position + ( t * direction );
    }
    
    return value;
}

tuple<Vector3,float> EngineManager::RayCastHit(Camera &camera, float scrWidth, float scrHeight, float n, Vector2 &scrMousePos, Cube &cube)
{
    Vector3 value(0.0f, 0.0f, 0.0f);
    unsigned normIndex = -1;    
    float tValue = 0.0f;
    float rayLenght = 1000.0f;

    for (unsigned i = 0; i < 6; i++)
    {
        Vector3 direction = CastRay(camera, scrWidth, scrHeight, n, scrMousePos);
        Vector3 face = cube.Position + ( Normal[i] * 0.5f );
        float t = (face -  camera.Position).dot(Normal[i]) / direction.dot(Normal[i]);

        if( t > 0)
        {
            Vector3 hitPos = camera.Position + ( t * direction);
            Vector3 hitPosFace = hitPos - face;
        
            if(abs(hitPosFace.x) < 0.5f && abs(hitPosFace.y) < 0.5f && abs(hitPosFace.z) < 0.5f && rayLenght > t)
            {
                rayLenght = t;
                normIndex = i;
                tValue = t;
            }
        }
    }

    if(normIndex != -1)
    {
        value = cube.Position + Normal[normIndex];
    }

    tuple<Vector3,float> result;
    result = make_tuple(value, tValue);
    return result;
}