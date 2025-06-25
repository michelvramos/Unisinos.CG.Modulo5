#include "My3DObject.h"

My3DObject::My3DObject(std::string objFile, std::string mtlFile)
{
	SimpleOBJLoader::Load(objFile, m_mesh);
	SimpleOBJLoader::LoadMTL(mtlFile, m_material);
	translationOffsetAccum = glm::vec3(0.0f);
	scaleFactorAccumulated = glm::vec3(1.0f);
	rotationAngleAccum = glm::vec3(0.0f);
	transform = glm::mat4(1.0f);
	startPosition = glm::vec3(0);

	shaderId = 0;
	colorTextureId = 0;
	aoTextureId = 0;
}

void My3DObject::SetStartPosition(glm::vec3 position)
{
	startPosition = position;
	Translate(position);
}

void My3DObject::SetScale(glm::vec3 scale)
{
	scaleFactorAccumulated *= scale;
}

void My3DObject::SetTranslation(glm::vec3 translation)
{
	translationOffsetAccum += translation;
}

void My3DObject::SetRotation(glm::vec3 increment)
{
	rotationAngleAccum.x += increment.x;
	rotationAngleAccum.y += increment.y;
	rotationAngleAccum.z += increment.z;
}

void My3DObject::Transform()
{
	//scaleFactorAccumulated *= scale;
	glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), scaleFactorAccumulated);

	// rotação acumulada
	glm::mat4 rotationMatrix = glm::mat4(1.0f);
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotationAngleAccum.x), glm::vec3(1, 0, 0));
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotationAngleAccum.y), glm::vec3(0, 1, 0));
	rotationMatrix = glm::rotate(rotationMatrix, glm::radians(rotationAngleAccum.z), glm::vec3(0, 0, 1));
	
	// transformações locais de rotação e escala
	glm::mat4 localTransform = rotationMatrix * scaleMatrix;

	// Aplicar transformações
	transform = glm::translate(glm::mat4(1.0f), translationOffsetAccum + startPosition) * localTransform;
}

void My3DObject::Translate(glm::vec3 toPosition)
{
	transform = glm::translate(transform, toPosition);
}

const glm::mat4 My3DObject::GetTransform() const
{
	return transform;
}

const Mesh& My3DObject::GetMesh() const
{
	return m_mesh;
}

const MyMaterial& My3DObject::GetMaterial() const
{
	return m_material;
}

void My3DObject::Draw(GLint uniLocationID, GLint colorTextureUniLocation, GLint ambientOcclusionUniLocation, GLint ka, GLint kd, GLint ks, GLint ns)
{
	glUniform3fv(ka, 1, glm::value_ptr(m_material.ka));
	glUniform3fv(kd, 1, glm::value_ptr(m_material.kd));
	glUniform3fv(ks, 1, glm::value_ptr(m_material.ks));
	glUniform1fv(ns, 1, &m_material.ns);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorTextureId);
	glUniform1i(colorTextureUniLocation, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, aoTextureId);
	glUniform1i(ambientOcclusionUniLocation, 1);

	glUniformMatrix4fv(uniLocationID, 1, FALSE, glm::value_ptr(transform));

	glBindVertexArray(m_mesh.VAO);
	glDrawArrays(GL_TRIANGLES, 0, m_mesh.vertexCount);

	glBindVertexArray(0);
}

void My3DObject::DeallocateOpenGL() const
{
	glDeleteVertexArrays(1, &m_mesh.VAO);
	glDeleteTextures(1, &colorTextureId);
	glDeleteTextures(1, &aoTextureId);
}
