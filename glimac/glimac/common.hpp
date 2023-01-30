#pragma once

#include <GLFW/glfw3.h>
#include "glm.hpp"
#define MAX_BONE_INFLUENCE 4

namespace glimac {

struct ShapeVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;

    ShapeVertex()
    {}
    ShapeVertex(glm::vec3 _position, glm::vec3 _normal, glm::vec2 _texCoords) : position{_position}, normal{_normal}, texCoords{_texCoords}
    {}
};

struct Vertex
{
    // position
    glm::vec3 Position;
    // normal
    glm::vec3 Normal;
    // texCoords
    glm::vec2 TexCoords;
    // tangent
    glm::vec3 Tangent;
    //bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    //weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};

}
