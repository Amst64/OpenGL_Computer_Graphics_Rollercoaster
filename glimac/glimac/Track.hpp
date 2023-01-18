#pragma once
#include <vector>
#include <glimac/Spline.hpp>
#include <glimac/common.hpp>

namespace glimac {
	class Track {
	private:
		std::vector<glimac::Spline> splines;
        std::vector<glimac::ShapeVertex> vertices;
        std::vector<uint32_t> indices;

	public:
		Track(std::vector<glimac::Spline> _splines)
			: splines{_splines}
		{
			calculateVertices();
		}

        void calculateVertices();

        std::vector<glimac::ShapeVertex> getTrackVertices() const
		{
            return vertices;
		}

		std::vector<uint32_t> getTrackIndices() const
		{
			return indices;
		}
	};
}