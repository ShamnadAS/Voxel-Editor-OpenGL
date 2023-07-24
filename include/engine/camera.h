#pragma once

#include <glad/glad.h>
#include <math/Vectors.h>
#include <math/Matrices.h>
#include <vector>
#include <math.h>

// Default camera values
const float SPEED = 0.5f;
const float SENSITIVITY = 0.05f;
const float FOV = 45.0f;

class Camera
{
private:
    Matrix4 RotationMatrix;

public:
    Vector3 Position;

    float MovementSpeed;
    float MouseSensitivity;
    float Fov;

    // constructor with vectors
    Camera()
    : MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Fov(FOV)
    {
        Position = Vector3(0.0f, 0.0f, 0.0f);
        RotationMatrix = Matrix4().identity();
    }

    Matrix4 GetRotationMatrix()
    {
        return RotationMatrix;
    }
    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void CameraOrbitMotion(Vector2 curMousePos, Vector2 prevMousePos)
    {
        Vector3 pStart;
        pStart.x = prevMousePos.x;
        pStart.y = prevMousePos.y;
        float r = ( pStart.x * pStart.x ) + ( pStart.y * pStart.y );
        pStart.z = r <= 1? abs(sqrt(1 - r)) : 0;
        
        Vector3 pCurrent;
        pCurrent.x = curMousePos.x;
        pCurrent.y = curMousePos.y;

        r = ( pCurrent.x * pCurrent.x ) + ( pCurrent.y * pCurrent.y );
        pCurrent.z = r <= 1? abs(sqrt(1 - r)) : 0;

        pStart.normalize();
        pCurrent.normalize();

        Vector3 axis = pCurrent.cross(pStart);
        axis.normalize();
        float angle = acos(std::min(pStart.dot(pCurrent), 1.0f));
        angle *= RAD2DEG;
        RotationMatrix.rotate(angle, axis);
    }
};