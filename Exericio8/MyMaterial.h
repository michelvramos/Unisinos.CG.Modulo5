#pragma once
#include <glm/glm.hpp>

struct MyMaterial
{
	//specular exponent
	GLfloat ns;

	//ambient color
	glm::vec3 ka;

	//difuse color
	glm::vec3 kd;
	
	//shine color
	glm::vec3 ks;

	//emissive color
	glm::vec3 ke;

	//optical density
	GLfloat ni;

	//Dissolve (transparency)
	GLfloat d;

	//Illumination mode
	GLint illum;


};