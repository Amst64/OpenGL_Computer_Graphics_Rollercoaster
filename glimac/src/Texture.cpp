#include "glimac/Texture.hpp"


namespace glimac {

    Texture::Texture(std::unique_ptr<glimac::Image>& image, std::string _type, GLenum textureUnit, GLenum format) : type{ _type }
    {
        glActiveTexture(textureUnit);
        glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_2D, ID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, image->getWidth(), image->getHeight(), 0, format, GL_FLOAT, image->getPixels());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Texture::assignTexUnit(glimac::Program& program, const char* uniform, GLuint textureUnit)
    {
        // Gets the location of the uniform
        GLuint uTexture = glGetUniformLocation(program.getGLId(), uniform);

        // Sets the value of the uniform
        glUniform1i(uTexture, textureUnit);
    }
}