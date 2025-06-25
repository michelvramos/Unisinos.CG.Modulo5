#pragma once
// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

class MyCamera
{
public:
	glm::mat4 m_viewMatrix;
	glm::mat4 m_projectionMatrix;
	glm::vec3 m_position;	
	glm::vec3 m_up;

	void AddYaw(float increment);
	void AddPitch(float increment);
	void AddRoll(float increment);

	MyCamera(float fovy, float aspect, float f_near, float f_far, glm::vec3 position, float yaw, float pitch, float roll, glm::vec3 up);
	void AddPosition(glm::vec3 position);	
	void ApplyTransform();
	void ResetCamera();

private:
	float m_yaw, // y
		m_pitch, // z
		m_roll,  // x
		m_original_yaw,
		m_original_pitch,
		m_original_roll;
	glm::vec3 m_originalPosition, 
		m_originalUp;

	float Clamp(float value, float min, float max);

};

