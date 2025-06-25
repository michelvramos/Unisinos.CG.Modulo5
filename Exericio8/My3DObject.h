#pragma once
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "SimpleOBJLoader.h"
#include "Mesh.h"
#include "MyMaterial.h"

class My3DObject
{
public:
	GLuint shaderId;
	GLuint colorTextureId;
	GLuint aoTextureId;

	My3DObject(std::string objFile, std::string mtlFile);
	void SetStartPosition(glm::vec3 position);
	void SetScale(glm::vec3 scale);
	void SetTranslation(glm::vec3 translation);
	void SetRotation(glm::vec3 increment);
	void Transform();
	void Translate(glm::vec3 toPosition);
	const glm::mat4 GetTransform() const;
	const Mesh& GetMesh() const;
	const MyMaterial& GetMaterial() const;
	void Draw(GLint uniLocationID, GLint colorTextureUniLocation, GLint ambientOcclusionUniLocation, GLint ka, GLint kd, GLint ks, GLint ns);
	void DeallocateOpenGL() const;

private:
	Mesh m_mesh;
	MyMaterial m_material;

	glm::mat4 transform;
	glm::vec3 startPosition;

	glm::vec3 translationOffsetAccum;
	glm::vec3 scaleFactorAccumulated;
	glm::vec3 rotationAngleAccum;
};

