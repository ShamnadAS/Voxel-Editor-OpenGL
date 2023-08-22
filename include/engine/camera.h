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
const float FOV = 45.0f;

class Camera
{
public:
    Vector3 Position;
    float Fov;

    float m_Pitch = 0.0f, m_Yaw = 0.0f;
    glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
    float m_Distance = 20.0f;
    glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };
    glm::mat4 m_ViewMatrix;
    float m_ViewportWidth = 1644, m_ViewportHeight = 800;
    glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };

    // constructor with vectors
    Camera(glm::vec3 focalPoint = glm::vec3(0, 0, 0), float yaw = 0.0f, float pitch = 0.0f) 
    : Fov(FOV)
    {
        m_Yaw = yaw;
        m_Pitch = pitch;
        m_FocalPoint = focalPoint;
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

    float RotationSpeed() const
	{
		return 0.8f;
	}

    std::pair<float, float> PanSpeed() const
	{
		float x = std::min(m_ViewportWidth / 1000.0f, 2.4f); // max = 2.4f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(m_ViewportHeight / 1000.0f, 2.4f); // max = 2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;
		return { xFactor, yFactor};
	}

    float ZoomSpeed() const
	{
		float distance = m_Distance * 0.2f;
		distance = std::max(distance, 0.0f);
		float speed = distance * distance * 0.05f;
		speed = std::min(speed, 100.0f); // max speed = 100
		return speed;
	}

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void MouseRotation(glm::vec2 &delta)
    {
        float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
		m_Yaw += yawSign * delta.x * RotationSpeed();
		m_Pitch += delta.y * RotationSpeed();

        UpdateView();
    }

    void MousePan(glm::vec2 &delta)
	{
        auto [xSpeed, ySpeed] = PanSpeed();
		m_FocalPoint += -GetRightDirection() * delta.x * xSpeed * m_Distance;
		m_FocalPoint += GetUpDirection() * delta.y * ySpeed * m_Distance;
        UpdateView();
	}

    void MouseZoom(float delta)
	{
		m_Distance -= delta * ZoomSpeed();
		if (m_Distance < 1.0f)
		{
			m_FocalPoint += GetForwardDirection();
			m_Distance = 1.0f;
		}
        UpdateView();
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

    void UpdateViewportSize(float width, float height)
    {
        m_ViewportWidth = width;
        m_ViewportHeight = height;
    }

    glm::quat GetOrientation() const
	{
		return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
	}

    glm::vec3 CalculatePosition() const
	{
		return m_FocalPoint - GetForwardDirection() * m_Distance;
	}

    glm::vec3 GetForwardDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

    glm::vec3 GetUpDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

    glm::vec3 GetRightDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}
};