#pragma once

namespace glimac {
	class Mesh {
	private:
		GLuint VAO, VBO;

		void setupMesh()
		{
            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glimac::ShapeVertex), vertices.data(), GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);

            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);

            const GLuint VERTEX_ATTRIB_POS = 0;
            const GLuint VERTEX_ATTRIB_NORMAL = 1;
            const GLuint VERTEX_ATTRIB_UV = 2;

            glEnableVertexAttribArray(VERTEX_ATTRIB_POS);
            glEnableVertexAttribArray(VERTEX_ATTRIB_NORMAL);
            glEnableVertexAttribArray(VERTEX_ATTRIB_UV);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glVertexAttribPointer(VERTEX_ATTRIB_POS, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (GLvoid*)offsetof(glimac::ShapeVertex, position));
            glVertexAttribPointer(VERTEX_ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (GLvoid*)offsetof(glimac::ShapeVertex, normal));
            glVertexAttribPointer(VERTEX_ATTRIB_UV, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (GLvoid*)offsetof(glimac::ShapeVertex, texCoords));
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
		}

	public:
		std::vector<glimac::ShapeVertex> vertices;

		Mesh(std::vector<glimac::ShapeVertex> _vertices) : vertices{ _vertices }
        {
            setupMesh();
        }

		void Draw(GLuint programID, glm::mat4 MVPMatrix, glm::mat4 NormalMatrix)
        {
            glUniformMatrix4fv(glGetUniformLocation(programID, "uNormalMatrix"), 1, GL_FALSE, glm::value_ptr(NormalMatrix));
            glUniformMatrix4fv(glGetUniformLocation(programID, "uMVPMatrix"), 1, GL_FALSE, glm::value_ptr(MVPMatrix));

            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertices.size());
            glBindVertexArray(0);
        }
	};
}
