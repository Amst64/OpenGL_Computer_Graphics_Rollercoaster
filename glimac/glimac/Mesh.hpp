#pragma once
#include "Texture.hpp"


namespace glimac {
	class Mesh {
	private:
		GLuint VAO, VBO, EBO;

		void setupMesh()
		{
            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glimac::ShapeVertex), vertices.data(), GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glGenBuffers(1, &EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);

            const GLuint VERTEX_ATTRIB_POS = 0;
            const GLuint VERTEX_ATTRIB_NORMAL = 1;
            const GLuint VERTEX_ATTRIB_UV = 2;

            glEnableVertexAttribArray(VERTEX_ATTRIB_POS);
            glEnableVertexAttribArray(VERTEX_ATTRIB_NORMAL);
            glEnableVertexAttribArray(VERTEX_ATTRIB_UV);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glVertexAttribPointer(VERTEX_ATTRIB_POS, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (GLvoid*)offsetof(glimac::ShapeVertex, position));
            glVertexAttribPointer(VERTEX_ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (GLvoid*)offsetof(glimac::ShapeVertex, normal));
            glVertexAttribPointer(VERTEX_ATTRIB_UV, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (GLvoid*)offsetof(glimac::ShapeVertex, texCoords));
            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

	public:
		std::vector<glimac::ShapeVertex> vertices;
        std::vector<uint32_t> indices;
        std::vector<glimac::Texture> textures;

		Mesh(std::vector<glimac::ShapeVertex> _vertices, std::vector<uint32_t> _indices, std::vector<glimac::Texture> _textures) : vertices{ _vertices }, 
            indices{_indices}, textures{_textures}
        {
            setupMesh();
        }

		void Draw(glimac::Program& program, glm::mat4 MVPMatrix, glm::mat4 NormalMatrix)
        {
            program.use();
            GLuint programID = program.getGLId();

            glUniformMatrix4fv(glGetUniformLocation(programID, "uNormalMatrix"), 1, GL_FALSE, glm::value_ptr(NormalMatrix));
            glUniformMatrix4fv(glGetUniformLocation(programID, "uMVPMatrix"), 1, GL_FALSE, glm::value_ptr(MVPMatrix));

            for (unsigned int i = 0; i < textures.size(); i++)
            {
                glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
                glBindTexture(GL_TEXTURE_2D, textures[i].ID);
                textures[i].assignTexUnit(program, "uTexture", i);
                glBindTexture(GL_TEXTURE_2D, textures[i].ID);
            }
            glActiveTexture(GL_TEXTURE0);

            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }
	};
}
