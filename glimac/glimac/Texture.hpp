
#pragma once
#include <glad/glad.h>
#include <memory>
#include "glimac/Program.hpp"
#include <glimac/Image.hpp>
#include <string>

namespace glimac {

	struct ModelTexture
	{
		GLuint ID;
		std::string type;
		std::string path;  // we store the path of the texture to compare with other textures

		void assignTexUnit(glimac::Program& program, const char* uniform, GLuint textureUnit)
		{
			// Gets the location of the uniform
			GLuint uTexture = glGetUniformLocation(program.getGLId(), uniform);

			// Sets the value of the uniform
			glUniform1i(uTexture, textureUnit);
		}
	};

	struct Texture
	{
		GLuint ID;
		std::string type;

		Texture();
		Texture(std::unique_ptr<glimac::Image>& image, std::string _type, GLenum textureUnit, GLenum format);

		void assignTexUnit(glimac::Program& program, const char* uniform, GLuint textureUnit);

	};
}