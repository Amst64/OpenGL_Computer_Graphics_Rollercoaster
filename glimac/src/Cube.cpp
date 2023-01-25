#include "glimac/Cube.hpp"
#include <iostream>

namespace glimac {
	void Cube::build()
	{
		//face1
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0, 0, -1), glm::vec2(0.0f, 0.0f)));
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0, 0, -1), glm::vec2(1.0f, 1.0f)));
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0, 0, -1), glm::vec2(1.0f, 0.0f)));
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0, 0, -1), glm::vec2(0.0f, 1.0f)));

		m_Indices.push_back(0);
		m_Indices.push_back(1);
		m_Indices.push_back(2);

		m_Indices.push_back(1);
		m_Indices.push_back(0);
		m_Indices.push_back(3);
		//

		//face2
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0, 0, 1), glm::vec2(0.0f, 0.0f)));
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0, 0, 1), glm::vec2(1.0f, 0.0f)));
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0, 0, 1), glm::vec2(1.0f, 1.0f)));
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0, 0, 1), glm::vec2(0.0f, 1.0f)));

		m_Indices.push_back(4);
		m_Indices.push_back(5);
		m_Indices.push_back(6);

		m_Indices.push_back(6);
		m_Indices.push_back(7);
		m_Indices.push_back(4);
		//

		//face3
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(-1, 0, 0), glm::vec2(1.0f, 0.0f)));
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(-1, 0,0), glm::vec2(1.0f, 1.0f)));
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1, 0, 0), glm::vec2(0.0f, 1.0f)));
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(-1, 0, 0), glm::vec2(0.0f, 0.0f)));

		m_Indices.push_back(8);
		m_Indices.push_back(9);
		m_Indices.push_back(10);

		m_Indices.push_back(10);
		m_Indices.push_back(11);
		m_Indices.push_back(8);
		//


		//Face4
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1, 0, 0), glm::vec2(1.0f, 0.0f)));
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1, 0, 0), glm::vec2(0.0f, 1.0f)));
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(1, 0, 0), glm::vec2(1.0f, 1.0f)));
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(1, 0, 0), glm::vec2(0.0f, 0.0f)));

		m_Indices.push_back(12);
		m_Indices.push_back(13);
		m_Indices.push_back(14);

		m_Indices.push_back(12);
		m_Indices.push_back(15);
		m_Indices.push_back(13);
		//


		//face5
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0, -1, 0), glm::vec2(0.0f, 1.0f)));
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0, -1, 0), glm::vec2(1.0f, 1.0f)));
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0, -1, 0), glm::vec2(1.0f, 0.0f)));
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0, -1, 0), glm::vec2(0.0f, 0.0f)));

		m_Indices.push_back(16);
		m_Indices.push_back(17);
		m_Indices.push_back(18);

		m_Indices.push_back(18);
		m_Indices.push_back(19);
		m_Indices.push_back(16);
		//

		//face6
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0, 1, 0), glm::vec2(0.0f, 1.0f)));
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0, 1, 0), glm::vec2(1.0f, 0.0f)));
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0, 1, 0), glm::vec2(1.0f, 1.0f)));
		m_Vertices.push_back(glimac::ShapeVertex(glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0, 1, 0), glm::vec2(0.0f, 0.0f)));
		

		m_Indices.push_back(20);
		m_Indices.push_back(21);
		m_Indices.push_back(22);

		m_Indices.push_back(20);
		m_Indices.push_back(23);
		m_Indices.push_back(21);
		//

		m_nVertexCount = m_Vertices.size();
	}
}
