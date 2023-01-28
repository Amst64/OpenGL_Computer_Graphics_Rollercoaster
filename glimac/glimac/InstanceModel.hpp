#pragma once
#include "common.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "FilePath.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <src/stb_image.h>

namespace glimac
{
    class InstanceModel
    {
    public:

        int amount;
        std::vector<glm::mat4> ModelMatrices;
        std::vector<glimac::Mesh> meshes;

        InstanceModel(const FilePath& filepath, int _amount):amount{_amount}
        {
            loadOBJ(filepath.c_str());
        }

        void setBuffer()
        {
            // vertex buffer object
            unsigned int buffer;
            glGenBuffers(1, &buffer);
            glBindBuffer(GL_ARRAY_BUFFER, buffer);
            glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), ModelMatrices.data(), GL_STATIC_DRAW);

            for (unsigned int i = 0; i < meshes.size(); i++)
            {
                unsigned int VAO = meshes[i].VAO;
                glBindVertexArray(VAO);
                // vertex attributes
                std::size_t vec4Size = sizeof(glm::vec4);
                glEnableVertexAttribArray(3);
                glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (const GLvoid*)0);
                glEnableVertexAttribArray(4);
                glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(1 * vec4Size));
                glEnableVertexAttribArray(5);
                glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(2 * vec4Size));
                glEnableVertexAttribArray(6);
                glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (GLvoid*)(3 * vec4Size));

                glVertexAttribDivisor(3, 1);
                glVertexAttribDivisor(4, 1);
                glVertexAttribDivisor(5, 1);
                glVertexAttribDivisor(6, 1);

                glBindVertexArray(0);
            }
        }

        void SetMatrix(glimac::Program& program, glm::mat4 ViewMatrix, glm::mat4 ProjMatrix, glm::mat4 NormalMatrix, std::vector<glm::vec3> lightsPosition)
        {
            glm::mat4 nothing = glm::mat4(0);
            for (unsigned int i = 0; i < meshes.size(); i++)
                meshes[i].SetMatrix(program, nothing, ViewMatrix, ProjMatrix, NormalMatrix, lightsPosition);
        }

        void Draw(glimac::Program& program, float shininess)
        {
            GLuint programID = program.getGLId();

            glUniform1f(glGetUniformLocation(programID, "uMaterial.shininess"), shininess);
            for(unsigned int j = 0; j < meshes.size(); j++)
            {
                unsigned int diffuseNr = 1;
                unsigned int specularNr = 1;
                for (unsigned int i = 0; i < meshes[j].modelTextures.size(); i++)
                {
                    glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
                    // retrieve texture number (the N in diffuse_textureN)
                    std::string number;
                    std::string name = meshes[j].modelTextures[i].type;
                    if (name == "texture_diffuse")
                        number = std::to_string(diffuseNr++);
                    else if (name == "texture_specular")
                        number = std::to_string(specularNr++);
                    meshes[j].modelTextures[i].assignTexUnit(program, ("uMaterial." + name + number).c_str(), i);
                    glBindTexture(GL_TEXTURE_2D, meshes[j].modelTextures[i].ID);
                }
                glActiveTexture(GL_TEXTURE0);
            }
            
            for (unsigned int i = 0; i < meshes.size(); i++)
            {
                glBindVertexArray(meshes[i].VAO);
                glDrawElementsInstanced(
                    GL_TRIANGLES, meshes[i].indices.size(), GL_UNSIGNED_INT, 0, amount);
            }
        }

    private:
        // model data
        std::string directory;
        std::vector<ModelTexture> textures_loaded;

        void loadOBJ(std::string path)
        {
            Assimp::Importer import;
            const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

            if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
            {
                std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
                return;
            }

            directory = path.substr(0, path.find_last_of("\\"));
            if (directory == path)
            {
                directory = path.substr(0, path.find_last_of("/"));
            }
            std::cout << "directory : " << directory << std::endl;
            processNode(scene->mRootNode, scene);
        }

        void processNode(aiNode* node, const aiScene* scene)
        {
            // process all the node's meshes (if any)
            for (unsigned int i = 0; i < node->mNumMeshes; i++)
            {
                aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
                meshes.push_back(processMesh(mesh, scene));
            }
            // then do the same for each of its children
            for (unsigned int i = 0; i < node->mNumChildren; i++)
            {
                processNode(node->mChildren[i], scene);
            }
        }

        glimac::Mesh processMesh(aiMesh* mesh, const aiScene* scene)
        {
            std::vector<glimac::ShapeVertex> vertices;
            std::vector<unsigned int> indices;
            std::vector<glimac::ModelTexture> textures;

            for (unsigned int i = 0; i < mesh->mNumVertices; i++)
            {
                glimac::ShapeVertex vertex;
                // process vertex positions, normals and texture coordinates
                glm::vec3 vector;
                vector.x = mesh->mVertices[i].x;
                vector.y = mesh->mVertices[i].y;
                vector.z = mesh->mVertices[i].z;
                vertex.position = vector;

                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.normal = vector;

                if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
                {
                    glm::vec2 vec;
                    vec.x = mesh->mTextureCoords[0][i].x;
                    vec.y = mesh->mTextureCoords[0][i].y;
                    vertex.texCoords = vec;
                }
                else
                    vertex.texCoords = glm::vec2(0.0f, 0.0f);

                vertices.push_back(vertex);
            }

            // process indices
            for (unsigned int i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];
                for (unsigned int j = 0; j < face.mNumIndices; j++)
                    indices.push_back(face.mIndices[j]);
            }

            if (mesh->mMaterialIndex != 0)
            {
                aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
                std::vector<glimac::ModelTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
                textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
                std::vector<glimac::ModelTexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
                textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
            }

            return glimac::Mesh(vertices, indices, textures);
        }

        std::vector<glimac::ModelTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
        {
            std::vector<ModelTexture> textures;
            for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
            {
                aiString str;
                mat->GetTexture(type, i, &str);
                bool skip = false;

                for (unsigned int j = 0; j < textures_loaded.size(); j++)
                {
                    if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
                    {
                        textures.push_back(textures_loaded[j]);
                        skip = true;
                        break;
                    }
                }
                if (!skip)
                {   // if texture hasn't been loaded already, load it
                    glimac::ModelTexture texture;
                    texture.ID = TextureFromFile(str.C_Str(), directory, type);
                    texture.type = typeName;
                    texture.path = str.C_Str();
                    textures.push_back(texture);
                    textures_loaded.push_back(texture); // add to loaded textures
                }
            }
            return textures;
        }

        unsigned int TextureFromFile(const char* path, const std::string& directory, aiTextureType type, bool gamma = false)
        {
            gamma = !gamma; //delete this line if using gamma
            std::string filename = std::string(path);
            filename = directory + '/' + filename;
            if (type == aiTextureType_DIFFUSE)
            {
                glActiveTexture(GL_TEXTURE0);
            }
            else if (type == aiTextureType_SPECULAR)
            {
                glActiveTexture(GL_TEXTURE1);
            }
            unsigned int textureID;
            glGenTextures(1, &textureID);

            int width, height, nrComponents;
            unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
            if (data)
            {
                GLenum format;
                if (nrComponents == 1)
                    format = GL_RED;
                else if (nrComponents == 3)
                    format = GL_RGB;
                else if (nrComponents == 4)
                    format = GL_RGBA;

                glBindTexture(GL_TEXTURE_2D, textureID);
                glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                stbi_image_free(data);
            }
            else
            {
                std::cout << "Texture failed to load at path: " << path << std::endl;
                stbi_image_free(data);
            }

            return textureID;
        }

    };
}
