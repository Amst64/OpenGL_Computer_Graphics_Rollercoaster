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

        GLsizei getVertexCount() const {
            return m_nVertexCount;
        }

    private:
        std::vector<ShapeVertex> m_Vertices;
        GLsizei m_nVertexCount;
    };

}