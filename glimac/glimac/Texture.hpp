
#pragma once
#include <glad/glad.h>
#include <memory>
#include "glimac/Program.hpp"
#include <glimac/Image.hpp>
#include <string>

namespace glimac {

	struct Texture
	{
		GLuint ID;
		std::string type;

		Texture();
		Texture(std::unique_ptr<glimac::Image>& image, std::string _type, GLenum textureUnit, GLenum format);

		void assignTexUnit(glimac::Program& program, const char* uniform, GLuint textureUnit);

	};
}