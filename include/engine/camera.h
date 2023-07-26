#pragma once

#include <glad/glad.h>
#include <math/Vectors.h>
#include <math/Matrices.h>
#include <vector>
#include <math.h>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Default camera valuess
const float SPEED = 0.5f;
const float SENSITIVITY = 0.05f;
const float FOV = 45.0f;

class Camera
{
private:
    Matrix4 RotationMatrix;
    Matrix4 CenterTranslation;
    Matrix4 Translation;

public:
    Vector3 Position;
    Vector3 Centre;

    float MovementSpeed;
    float MouseSensitivity;
    float Fov;

    // constructor with vectors
    Camera(Vector3 position, Vector3 centre)
    : MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Fov(FOV), Position(position), Centre(Centre)
    {
        RotationMatrix = Matrix4().identity();
        CenterTranslation.translate(-centre);
        Translation.translate(-position);
    }

    Matrix4 GetRotationMatrix()
    {
        return Translation * RotationMatrix * CenterTranslation;
    }
    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void CameraOrbitMotion(Vector2 curMousePos, Vector2 prevMousePos)
    {
        Vector3 pStart;
        pStart.x = prevMousePos.x;
        pStart.y = prevMousePos.y;
        float r = pow(pStart.x, 2.0f) + pow(pStart.y, 2.0f);
        pStart.z = r <= 1? abs(sqrt(1 - r)) : 0;
                                                
        Vector3 pCurrent;
        pCurrent.x = curMousePos.x;
        pCurrent.y = curMousePos.y;

        r = pow(pCurrent.x, 2.0f) + pow(pCurrent.y, 2.0f);
        pCurrent.z = r <= 1? abs(sqrt(1 - r)) : 0;

        Vector3 axis = pStart.cross(pCurrent);
        axis.normalize();
        pStart.normalize();
        pCurrent.normalize();
        float angle = acos(std::min(pStart.dot(pCurrent), 1.0f)) * RAD2DEG;
        Matrix4 currentRotation;
        currentRotation.rotate(angle, axis);
        RotationMatrix = currentRotation * RotationMatrix;
    }
};