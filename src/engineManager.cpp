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

Vector3 EngineManager::CastRay(Matrix4 projection, Matrix4 view, Vector2 &scrMousePos, unsigned scrWidth, unsigned scrHeight)
{
    Vector3 ndc;
    ndc.x = (2 * scrMousePos.x/ scrWidth) - 1.0f;
    ndc.y = 1.0f - (2 * scrMousePos.y / scrHeight);
    ndc.z = 1.0f;

    Vector4 rayClip = Vector4(ndc.x, ndc.y, -1.0f, 1.0f);
    Vector4 rayEye = projection.invert() * rayClip;
    rayEye.z = -1.0f;
    rayEye.w = 0.0f;

    Vector4 rayWorld = view.invert() * rayEye;
    Vector3 ray = Vector3(rayWorld.x, rayWorld.y, rayWorld.z).normalize();

    return ray;
}   

//Hit position on the grid
Vector3 EngineManager::RayCastHit(unsigned k, Camera &camera, Matrix4 projection, Matrix4 view, Vector2 &scrMousePos, unsigned scrWidth, unsigned scrHeight)
{
    Vector3 value(0.0f, 0.0f, 0.0f);
    Vector3 direction = CastRay(projection, view, scrMousePos, scrWidth, scrHeight);
    float t = ( k - camera.Position.y ) / direction.y;
    if( t > 0)
    {
        value = camera.Position + ( t * direction );
    }
    return value;
}
 
tuple<Vector3,float> EngineManager::RayCastHit(Camera &camera, Matrix4 projection, Matrix4 view, Vector2 &scrMousePos, Cube &cube, float scrWidth, float scrHeight)
{
    Vector3 value(0.0f, 0.0f, 0.0f);
    unsigned normIndex = -1;    
    float tValue = 0.0f;
    float rayLenght = 1000.0f;

    for (unsigned i = 0; i < 6; i++)
    {
        Vector3 direction = CastRay(projection, view, scrMousePos, scrWidth, scrHeight);
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

Vector2 EngineManager::MousePosRelativeToViewPort(Vector2 &mousePos, UI* ui, GLFWwindow* window)
{
    Vector2 viewportPos = ui->ViewPortPos;
    int xpos, ypos;
    glfwGetWindowPos(window, &xpos, &ypos);
    Vector2 windowPos = Vector2(xpos, ypos);
    Vector2 offset = viewportPos - windowPos;
    Vector2 relativePos = mousePos - offset;

    return relativePos;
}