#include "MyCamera.h"

void MyCamera::AddYaw(float increment)
{
	m_yaw += increment;
}

void MyCamera::AddPitch(float increment)
{
	m_pitch = Clamp(m_pitch + increment, -89.0f, 89.0f);
}

void MyCamera::AddRoll(float increment)
{
	m_roll += increment;
}

MyCamera::MyCamera(float fovy, float aspect, float f_near, float f_far, glm::vec3 position, float yaw, float pitch, float roll, glm::vec3 up)
{
	m_original_yaw = m_yaw = yaw;
	m_original_pitch= m_pitch = Clamp(pitch,-89.0f, 89.0f);
	m_original_roll= m_roll = roll;

	// Matriz de projeção (perspectiva)
	m_projectionMatrix = glm::perspective(glm::radians(fovy),aspect, f_near, f_far);
	m_originalPosition = m_position = position;	
	m_originalUp= m_up = up;

	ApplyTransform();
	
}

void MyCamera::AddPosition(glm::vec3 position)
{
	m_position += position;
}

void MyCamera::ApplyTransform()
{
	glm::vec3 front;
	front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	front.y = sin(glm::radians(m_pitch));
	front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	front = glm::normalize(front);

	// Vetor "right" e "up" com roll:
	glm::vec3 worldUp(0.0f, 1.0f, 0.0f);
	glm::vec3 right = glm::normalize(glm::cross(front, worldUp));
	glm::vec3 m_up = glm::normalize(glm::cross(right, front));
	m_up = glm::rotate(m_up, glm::radians(m_roll), front);

	m_viewMatrix = glm::lookAt(
		m_position, //position
		m_position + front,  //look at
		m_up  //up direction
	);
}

void MyCamera::ResetCamera()
{
	m_yaw = m_original_yaw;
	m_pitch = m_original_pitch;
	m_roll = m_original_roll;
	m_position = m_originalPosition;
	m_up = m_originalUp;
}

float MyCamera::Clamp(float value, float min, float max)
{
	return value > max ? max : value < min ? min : value;
}
