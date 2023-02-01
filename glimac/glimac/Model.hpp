#pragma once
#include "common.hpp"
#include "Mesh2.hpp"
#include "Texture.hpp"
#include "FilePath.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <src/stb_image.h>

namespace glimac
{
	class Model
	{
    public:

        std::vector<glimac::Mesh2> meshes;

        Model(const FilePath& filepath)
        {
            loadOBJ(filepath.c_str());
        }

        void SetMatrix(glimac::Program& program, glm::mat4 ModelMatrix, glm::mat4 ViewMatrix, glm::mat4 ProjMatrix, glm::mat4 NormalMatrix, std::vector<glm::vec3> lightsPosition)
        {
            for (unsigned int i = 0; i < meshes.size(); i++)
                meshes[i].SetMatrix(program, ModelMatrix, ViewMatrix, ProjMatrix, NormalMatrix, lightsPosition);
        }

        void Draw(glimac::Program& program, float shininess, glm::vec3 viewPos, bool isSpecular = true, bool emissive = false)
        {
            for (unsigned int i = 0; i < meshes.size(); i++)
                meshes[i].Draw(program, shininess, viewPos, true, isSpecular, emissive);
        }

    private:
        // model data
        
        std::string directory;
        std::vector<ModelTexture> textures_loaded;

		void loadOBJ(std::string path)
		{
            Assimp::Importer import;
            const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

            if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
            {
                std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
                return;
            }

            directory = path.substr(0, path.find_last_of("\\"));
            if(directory == path)
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

        glimac::Mesh2 processMesh(aiMesh* mesh, const aiScene* scene)
        {
            std::vector<glimac::Vertex> vertices;
            std::vector<unsigned int> indices;
            std::vector<glimac::ModelTexture> textures;

            for (unsigned int i = 0; i < mesh->mNumVertices; i++)
            {
                glimac::Vertex vertex;
                // process vertex positions, normals and texture coordinates
                glm::vec3 vector;
                vector.x = mesh->mVertices[i].x;
                vector.y = mesh->mVertices[i].y;
                vector.z = mesh->mVertices[i].z;
                vertex.Position = vector;

                // normals
                if (mesh->HasNormals())
                {
                    vector.x = mesh->mNormals[i].x;
                    vector.y = mesh->mNormals[i].y;
                    vector.z = mesh->mNormals[i].z;
                    vertex.Normal = vector;
                }

                if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
                {
                    glm::vec2 vec;
                    vec.x = mesh->mTextureCoords[0][i].x;
                    vec.y = mesh->mTextureCoords[0][i].y;
                    vertex.TexCoords = vec;

                    // tangent
                    vector.x = mesh->mTangents[i].x;
                    vector.y = mesh->mTangents[i].y;
                    vector.z = mesh->mTangents[i].z;
                    vertex.Tangent = vector;
                }
                else
                    vertex.TexCoords = glm::vec2(0.0f, 0.0f);

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
                // 3. normal maps
                std::vector<glimac::ModelTexture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
                textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
                // 4. emissive maps
                std::vector<ModelTexture> heightMaps = loadMaterialTextures(material, aiTextureType_EMISSIVE, "texture_emissive");
                textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
            }

            return glimac::Mesh2(vertices, indices, textures);
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

            unsigned int textureID;
            glGenTextures(1, &textureID);

            int width, height, nrComponents;
            unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
            if (data)
            {
                glBindTexture(GL_TEXTURE_2D, textureID);
                if(type == aiTextureType_HEIGHT)
                {
                    if (nrComponents == 1)
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
                    else if (nrComponents == 3)
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                    else if (nrComponents == 4)
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                }
                else
                {
                    if (nrComponents == 1)
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
                    else if (nrComponents == 3)
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                    else if (nrComponents == 4)
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                }

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
