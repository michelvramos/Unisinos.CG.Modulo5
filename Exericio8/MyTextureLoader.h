#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include "stb_image.h"

class MyTextureLoader
{
	public:
		static GLuint LoadTexture(const char* texturePath);
};

