#pragma once
#include "Texture.hpp"


namespace glimac {
    class Mesh2 {
    private:
        GLuint VBO, EBO;

        void setupMesh()
        {
            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glimac::Vertex), vertices.data(), GL_STATIC_DRAW);
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
            const GLuint VERTEX_ATTRIB_TANGENT = 3;
            const GLuint VERTEX_ATTRIB_IDS = 4;
            const GLuint VERTEX_ATTRIB_WEIGHT = 5;

            glEnableVertexAttribArray(VERTEX_ATTRIB_POS);
            glEnableVertexAttribArray(VERTEX_ATTRIB_NORMAL);
            glEnableVertexAttribArray(VERTEX_ATTRIB_UV);
            glEnableVertexAttribArray(VERTEX_ATTRIB_TANGENT);
            glEnableVertexAttribArray(VERTEX_ATTRIB_IDS);
            glEnableVertexAttribArray(VERTEX_ATTRIB_WEIGHT);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glVertexAttribPointer(VERTEX_ATTRIB_POS, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::Vertex), (GLvoid*)offsetof(glimac::Vertex, Position));
            glVertexAttribPointer(VERTEX_ATTRIB_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::Vertex), (GLvoid*)offsetof(glimac::Vertex, Normal));
            glVertexAttribPointer(VERTEX_ATTRIB_UV, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::Vertex), (GLvoid*)offsetof(glimac::Vertex, TexCoords));
            glVertexAttribPointer(VERTEX_ATTRIB_TANGENT, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::Vertex), (GLvoid*)offsetof(glimac::Vertex, Tangent));
            glVertexAttribPointer(VERTEX_ATTRIB_IDS, 4, GL_FLOAT, GL_FALSE, sizeof(glimac::Vertex), (GLvoid*)offsetof(glimac::Vertex, m_BoneIDs));
            glVertexAttribPointer(VERTEX_ATTRIB_WEIGHT, 4, GL_FLOAT, GL_FALSE, sizeof(glimac::Vertex), (GLvoid*)offsetof(glimac::Vertex, m_Weights));
            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }

    public:
        GLuint VAO;
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        std::vector<glimac::Texture> textures;
        std::vector<glimac::ModelTexture> modelTextures;

        Mesh2(std::vector<glimac::Vertex> _vertices, std::vector<uint32_t> _indices, std::vector<glimac::Texture> _textures) : vertices{ _vertices },
            indices{ _indices }, textures{ _textures }
        {
            setupMesh();
        }

        Mesh2(std::vector<glimac::Vertex> _vertices, std::vector<uint32_t> _indices, std::vector<glimac::ModelTexture> _modelTextures) : vertices{ _vertices },
            indices{ _indices }, modelTextures{ _modelTextures }
        {
            setupMesh();
        }

        void SetMatrix(glimac::Program& program, glm::mat4 ModelMatrix, glm::mat4 ViewMatrix, glm::mat4 ProjMatrix, glm::mat4 NormalMatrix, std::vector<glm::vec3> lightsPosition)
        {
            program.use();
            GLuint programID = program.getGLId();

            glUniformMatrix4fv(glGetUniformLocation(programID, "uNormalMatrix"), 1, GL_FALSE, glm::value_ptr(NormalMatrix));
            glUniformMatrix4fv(glGetUniformLocation(programID, "uModelMatrix"), 1, GL_FALSE, glm::value_ptr(ModelMatrix));
            glUniformMatrix4fv(glGetUniformLocation(programID, "uViewMatrix"), 1, GL_FALSE, glm::value_ptr(ViewMatrix));
            glUniformMatrix4fv(glGetUniformLocation(programID, "uProjMatrix"), 1, GL_FALSE, glm::value_ptr(ProjMatrix));

            glm::vec3 lightPos = glm::vec3(-10, 10, -20); //The approximative position of the sun in the scene
            glUniform3fv(glGetUniformLocation(programID, "lightPos"), 1, glm::value_ptr(lightPos));
            glm::vec3 lightDir = ViewMatrix * glm::vec4(0.5f, -1.0f, 1.0f, 0.0f);
            //glm::vec3 lightDir = ViewMatrix * glm::vec4(-0.2f, -1.0f, -0.3f, 0.0f);

            for (GLuint i = 0; i < lightsPosition.size(); i++)
            {
                lightPos = ViewMatrix * glm::vec4(lightsPosition[i], 1);
                std::string number = std::to_string(i);


                glUniform3fv(glGetUniformLocation(programID, ("uPointLights[" + number + "].position").c_str()), 1, glm::value_ptr(lightPos));
                glUniform3f(glGetUniformLocation(programID, ("uPointLights[" + number + "].color").c_str()), 0.337f, 0.251f, 0.353f);
            }
            glUniform3fv(glGetUniformLocation(programID, "uDirLight.direction"), 1, glm::value_ptr(lightDir));
        }

        void Draw(glimac::Program& program, float shininess, glm::vec3 viewPos, bool isImportedModel = false, bool isSpecular = true, bool emissive = false)
        {
            GLuint programID = program.getGLId();

            glUniform1f(glGetUniformLocation(programID, "uMaterial.shininess"), shininess);
            glUniform1i(glGetUniformLocation(programID, "isSpecular"), isSpecular);
            glUniform1i(glGetUniformLocation(programID, "isEmissive"), emissive);
            glUniform3fv(glGetUniformLocation(programID, "uViewPos"), 1, glm::value_ptr(viewPos));

            unsigned int diffuseNr = 1;
            unsigned int specularNr = 1;
            unsigned int normalNr = 1;
            unsigned int emissiveNr = 1;

            if (isImportedModel)
            {

                for (unsigned int i = 0; i < modelTextures.size(); i++)
                {
                    glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
                    // retrieve texture number (the N in diffuse_textureN)
                    std::string number;
                    std::string name = modelTextures[i].type;
                    if (name == "texture_diffuse")
                        number = std::to_string(diffuseNr++);
                    else if (name == "texture_specular")
                        number = std::to_string(specularNr++); // transfer unsigned int to string
                    else if (name == "texture_normal")
                        number = std::to_string(normalNr++); // transfer unsigned int to string
                    else if (name == "texture_emissive")
                        number = std::to_string(emissiveNr++); // transfer unsigned int to string

                    modelTextures[i].assignTexUnit(program, ("uMaterial." + name + number).c_str(), i);
                    glBindTexture(GL_TEXTURE_2D, modelTextures[i].ID);
                }
                glActiveTexture(GL_TEXTURE0);

                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);
            }
            else
            {
                for (unsigned int i = 0; i < textures.size(); i++)
                {
                    glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
                    // retrieve texture number (the N in diffuse_textureN)
                    std::string number;
                    std::string name = textures[i].type;
                    if (name == "texture_diffuse")
                        number = std::to_string(diffuseNr++);
                    else if (name == "texture_specular")
                        number = std::to_string(specularNr++);
                    textures[i].assignTexUnit(program, ("uMaterial." + name + number).c_str(), i);
                    glBindTexture(GL_TEXTURE_2D, textures[i].ID);
                }
                glActiveTexture(GL_TEXTURE0);

                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);
            }
        }
    };
}
