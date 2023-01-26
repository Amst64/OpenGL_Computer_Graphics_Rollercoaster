#include <glm/gtx/string_cast.hpp>
namespace glimac
{
    class Skybox {
    private:
        GLuint VAO, VBO;
        std::vector<glm::vec3> vertices;

        void setupMesh()
        {
            build();
            
            /*std::cout << vertices.size() << std::endl;
            for(int i = 0; i < vertices.size(); i++)
            {
                std::cout << glm::to_string(vertices[i]) << std::endl;
            }*/

            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        void build()
        {

            // positions          
            vertices.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));
            vertices.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
            vertices.push_back(glm::vec3(1.0f, -1.0f, -1.0f));
            vertices.push_back(glm::vec3(1.0f, -1.0f, -1.0f));
            vertices.push_back(glm::vec3(1.0f, 1.0f, -1.0f));
            vertices.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));

            vertices.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));
            vertices.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
            vertices.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));
            vertices.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));
            vertices.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));
            vertices.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));

            vertices.push_back(glm::vec3(1.0f, -1.0f, -1.0f));
            vertices.push_back(glm::vec3(1.0f, -1.0f, 1.0f));
            vertices.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
            vertices.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
            vertices.push_back(glm::vec3(1.0f, 1.0f, -1.0f));
            vertices.push_back(glm::vec3(1.0f, -1.0f, -1.0f));

            vertices.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));
            vertices.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));
            vertices.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
            vertices.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
            vertices.push_back(glm::vec3(1.0f, -1.0f, 1.0f));
            vertices.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));

            vertices.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));
            vertices.push_back(glm::vec3(1.0f, 1.0f, -1.0f));
            vertices.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
            vertices.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
            vertices.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));
            vertices.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));

            vertices.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
            vertices.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));
            vertices.push_back(glm::vec3(1.0f, -1.0f, -1.0f));
            vertices.push_back(glm::vec3(1.0f, -1.0f, -1.0f));
            vertices.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));
            vertices.push_back(glm::vec3(1.0f, -1.0f, 1.0f));

        }

    public:
        
        Skybox()
        {
            setupMesh();
        }

        void Draw(glimac::Program& program, glm::mat4 view, glm::mat4 projection, unsigned int cubemapTexture)
        {
            glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
            program.use();
            glm::mat4 view1 = glm::mat4(glm::mat3(view)); // remove translation from the view matrix

            glUniformMatrix4fv(glGetUniformLocation(program.getGLId(), "uView"), 1, GL_FALSE, glm::value_ptr(view1));
            glUniformMatrix4fv(glGetUniformLocation(program.getGLId(), "uProjection"), 1, GL_FALSE, glm::value_ptr(projection));

            // skybox cube
            glBindVertexArray(VAO);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
            glDrawArrays(GL_TRIANGLES, 0, (GLsizei) vertices.size());
            glBindVertexArray(0);
            glDepthFunc(GL_LESS); // set depth function back to default
        }

        
    };
}