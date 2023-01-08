#include "glimac/Cube.hpp"
#include <iostream>

namespace glimac {
	void Cube::build()
	{

		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0, 0, -1), glm::vec2(0.0f, 0.0f)));
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0, 0, -1), glm::vec2(1.0f, 0.0f)));
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0, 0, -1), glm::vec2(1.0f, 1.0f)));
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0, 0, -1), glm::vec2(1.0f, 1.0f)));
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0, 0, -1), glm::vec2(0.0f, 1.0f)));
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0, 0, -1), glm::vec2(0.0f, 0.0f)));

		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0, 0, 1), glm::vec2(0.0f, 0.0f)));
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0, 0, 1), glm::vec2(1.0f, 0.0f)));
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0, 0, 1), glm::vec2(1.0f, 1.0f)));
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0, 0, 1), glm::vec2(1.0f, 1.0f)));
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0, 0, 1), glm::vec2(0.0f, 1.0f)));
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0, 0, 1), glm::vec2(0.0f, 0.0f)));

		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(-1, 0, 0), glm::vec2(1.0f, 0.0f)));
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(-1, 0,0), glm::vec2(1.0f, 1.0f)));
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1, 0, 0), glm::vec2(0.0f, 1.0f)));
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1, 0, 0), glm::vec2(0.0f, 1.0f)));
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(-1, 0, 0), glm::vec2(0.0f, 0.0f)));
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(-1, 0, 0), glm::vec2(1.0f, 0.0f)));

		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1, 0, 0), glm::vec2(1.0f, 0.0f)));
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(1, 0, 0), glm::vec2(1.0f, 1.0f)));
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1, 0, 0), glm::vec2(0.0f, 1.0f)));
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1, 0, 0), glm::vec2(0.0f, 1.0f)));
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(1, 0, 0), glm::vec2(0.0f, 0.0f)));
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1, 0, 0), glm::vec2(1.0f, 0.0f)));

		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0, -1, 0), glm::vec2(0.0f, 1.0f)));
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0, -1, 0), glm::vec2(1.0f, 1.0f)));
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0, -1, 0), glm::vec2(1.0f, 0.0f)));
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0, -1, 0), glm::vec2(1.0f, 0.0f)));
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0, -1, 0), glm::vec2(0.0f, 0.0f)));
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0, -1, 0), glm::vec2(0.0f, 1.0f)));

		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0, 1, 0), glm::vec2(0.0f, 1.0f)));
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0, 1, 0), glm::vec2(1.0f, 1.0f)));
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0, 1, 0), glm::vec2(1.0f, 0.0f)));
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0, 1, 0), glm::vec2(1.0f, 0.0f)));
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0, 1, 0), glm::vec2(0.0f, 0.0f)));
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0, 1, 0), glm::vec2(0.0f, 1.0f)));

		m_nVertexCount = m_Vertices.size();
	}
}
