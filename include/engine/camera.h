#pragma once

#include <glad/glad.h>
#include <math/Vectors.h>
#include <math/Matrices.h>
#include <vector>
#include <math.h>

// Default camera values
const float YAW =   0.0f;
const float PITCH = 0.0f;
const float SPEED = 0.5f;
const float SENSITIVITY = 0.01f;
const float ZOOM = 45.0f;

class Camera
{
public:
    // camera Attributes
    Vector3 Position;
    Vector3 Front;
    Vector3 Up;
    Vector3 Right;
    Vector3 WorldUp;
    // euler Angles
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // constructor with vectors
    Camera(Vector3 position = Vector3(0.0f, 5.0f, 5.0f), Vector3 up = Vector3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(Vector3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    Matrix4 GetViewMatrix()
    {
        Matrix4 lookAt;
        Vector3 target = Vector3(0.0f, 0.0f, 0.0f);
        return lookAt.CameraLookAt(Position, target, Up);
    }

    void CameraPan(float xoffset, float yoffset, float deltaTime)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;
        Vector3 offset = Vector3(-xoffset, -yoffset, 0.0f) * MovementSpeed;
        Position += offset;
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Pitch = xoffset;
        Yaw = yoffset;
        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }
    
private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        //calculate new front vector
        Matrix4 model;
        model.rotateX(Yaw);
        model.rotateY(Pitch);
        //model.rotateZ(Yaw);
        Position = model * Position;
        Vector3 front = Position;
        //recalculate right and up vectors
        Right = front.cross(WorldUp).normalize();
        Up = Right.cross(front).normalize();
    }
};