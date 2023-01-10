#pragma once
#include <vector>
#include "common.hpp"

namespace glimac {

    class Cube {
        void build();

    public:
        Cube() :
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