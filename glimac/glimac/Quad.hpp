#pragma once
#include <vector>
#include "common.hpp"

namespace glimac {

    class Quad {
        void build()
        {
            m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(0, 1, 0), glm::vec2(0.0f, 1.0f)));
            m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(0.5f, 0.0f, 0.5f), glm::vec3(0, 1, 0), glm::vec2(1.0f, 0.0f)));
            m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(0.5f, 0.0f, -0.5f), glm::vec3(0, 1, 0), glm::vec2(1.0f, 1.0f)));
            m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(-0.5f, 0.0f, 0.5f), glm::vec3(0, 1, 0), glm::vec2(0.0f, 0.0f)));

            m_Indices.push_back(0);
            m_Indices.push_back(1);
            m_Indices.push_back(2);

            m_Indices.push_back(0);
            m_Indices.push_back(3);
            m_Indices.push_back(1);

            m_nVertexCount = (GLsizei)m_Vertices.size();
        }

    public:
        Quad() :
            m_nVertexCount(0) {
            build();
        }

        const std::vector<ShapeVertex> getVertices() const {
            return m_Vertices;
        }

        const std::vector<uint32_t> getIndices() const {
            return m_Indices;
        }

        GLsizei getVertexCount() const {
            return m_nVertexCount;
        }

    private:
        std::vector<ShapeVertex> m_Vertices;
        std::vector<uint32_t> m_Indices;
        GLsizei m_nVertexCount;
    };

}