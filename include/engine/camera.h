#pragma once

#include <glad/glad.h>
#include <math/Vectors.h>
#include <math/Matrices.h>
#include <vector>
#include <math.h>

#include <glm/gtx/quaternion.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


// Default camera values
const float SPEED = 0.5f;
const float SENSITIVITY = 0.05f;
const float FOV = 45.0f;

class Camera
{
public:
    // camera Attributes
    Vector3 Position;
    Vector3 Forward;
    Vector3 Up;
    Vector3 Right;
    Vector3 WorldUp;
    Vector3 Target;
    // euler Angles
    float HorizontalAngle;
    float VerticalAngle;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Fov;

    float m_Pitch = 0.0f, m_Yaw = 0.0f;
    glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
    float m_Distance = 20.0f;
    glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };
    glm::mat4 m_ViewMatrix;

    // constructor with vectors
    Camera(Vector3 position = Vector3(0.0f, 0.0f, 5.0f), Vector3 up = Vector3(0.0f, 1.0f, 0.0f), Vector3 target = Vector3(0.0f, 0.0f, 0.0f)) 
    : Forward(Vector3(0.0f, 0.0f, 1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Fov(FOV)
    {
        Position = position;
        WorldUp = up;
        Target = target;
        UpdateView();
    }

    Matrix4 GetViewMatrix()
    {
        Matrix4 view;
        const float *pSource = (const float*)glm::value_ptr(m_ViewMatrix);
        for (int i = 0; i < 16; ++i)
        {
            view[i] = pSource[i];
        }
        
        return view;
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void CameraRotation(float xoffset, float yoffset)
    {
        xoffset*=0.003f;
        yoffset*=0.003f;
        glm::vec2 delta(xoffset, yoffset);
        float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
		m_Yaw += yawSign * delta.x * RotationSpeed();
		m_Pitch += delta.y * RotationSpeed();

        UpdateView();
    }

    glm::vec3 GetUpDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

    glm::quat GetOrientation() const
	{
		return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
	}

    float RotationSpeed() const
	{
		return 0.8f;
	}
    
	void UpdateView()
	{
		// m_Yaw = m_Pitch = 0.0f; // Lock the camera's rotation
		m_Position = CalculatePosition();
        Position = Vector3(m_Position.x, m_Position.y, m_Position.z);

		glm::quat orientation = GetOrientation();
		m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
		m_ViewMatrix = glm::inverse(m_ViewMatrix);
	}

    glm::vec3 CalculatePosition() const
	{
		return m_FocalPoint - GetForwardDirection() * m_Distance;
	}

    glm::vec3 GetForwardDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}
};