#pragma once
#include<glimac/glm.hpp>

namespace glimac {
	class Spline {
	private:
		glm::vec3 startPoint;
		glm::vec3 startPivotPoint;
		glm::vec3 endPivotPoint;
		glm::vec3 endPoint;

		bool isLoop;
		
		float lerp(float a, float b, float t)
		{
			if(t == 0.0f)
			{
				return a;
			}
			if(t == 1.0f)
			{
				return b;
			}
			return a + t * (b - a);
		}

	public:
		Spline(glm::vec3 _startPoint, glm::vec3 _startPivotPoint, glm::vec3 _endPivotPoint, glm::vec3 _endPoint, bool _isLoop)
			:startPoint{_startPoint}, startPivotPoint{_startPivotPoint}, endPivotPoint{ _endPivotPoint }, endPoint{_endPoint}, isLoop{_isLoop}
		{
		}

		glm::vec3 getPosition(float t)
		{
			glm::vec3 p0;
			p0.x = lerp(startPoint.x, startPivotPoint.x, t);
			p0.y = lerp(startPoint.y, startPivotPoint.y, t);
			p0.z = lerp(startPoint.z, startPivotPoint.z, t);

			glm::vec3 p1;
			p1.x = lerp(startPivotPoint.x, endPivotPoint.x, t);
			p1.y = lerp(startPivotPoint.y, endPivotPoint.y, t);
			p1.z = lerp(startPivotPoint.z, endPivotPoint.z, t);

			glm::vec3 p2;
			p2.x = lerp(endPivotPoint.x, endPoint.x, t);
			p2.y = lerp(endPivotPoint.y, endPoint.y, t);
			p2.z = lerp(endPivotPoint.z, endPoint.z, t);

			glm::vec3 p3;
			p3.x = lerp(p0.x, p1.x, t);
			p3.y = lerp(p0.y, p1.y, t);
			p3.z = lerp(p0.z, p1.z, t);

			glm::vec3 p4;
			p4.x = lerp(p1.x, p2.x, t);
			p4.y = lerp(p1.y, p2.y, t);
			p4.z = lerp(p1.z, p2.z, t);

			glm::vec3 p5;
			p5.x = lerp(p3.x, p4.x, t);
			p5.y = lerp(p3.y, p4.y, t);
			p5.z = lerp(p3.z, p4.z, t);

			return p5;
		}

		std::vector<glm::vec3> getPoints() const
		{
			std::vector<glm::vec3> points;
			points.push_back(startPoint);
			points.push_back(startPivotPoint);
			points.push_back(endPivotPoint);
			points.push_back(endPoint);

			return points;
		}

		glm::vec3 GetCurveTangent(float ratio)
		{

			//derivée de l'équation
			glm::vec3 tangent = startPoint * (-3 * ratio * ratio + 6 * ratio - 3) +
				startPivotPoint * (9 * ratio * ratio - 12 * ratio + 3) +
				endPivotPoint * (-9 * ratio * ratio + 6 * ratio) +
				endPoint * (3 * ratio * ratio);



			return glm::normalize(tangent);
		}

		glm::vec3 GetCurveNormal(float ratio)
		{

			//tangent of cubic bezier curve at point "ratio"
			glm::vec3 tangent = startPoint * (-3 * ratio * ratio + 6 * ratio - 3) +
				startPivotPoint * (9 * ratio * ratio - 12 * ratio + 3) +
				endPivotPoint * (-9 * ratio * ratio + 6 * ratio) +
				endPoint * (3 * ratio * ratio);

			glm::vec3 normal;

			if(tangent.y == 0 && tangent.z == 0)
			{
				
				normal = glm::normalize(glm::cross(tangent, glm::vec3(0, 1, 0)));
			}else
			{
				if(!isLoop)
				{
					if (tangent.z > 0)
					{
						normal = glm::normalize(glm::cross(tangent, glm::vec3(1, 0, 0)));
					}
					else {
						normal = glm::normalize(glm::cross(tangent, glm::vec3(-1, 0, 0)));
					}
				}
				else
				{
					normal = glm::normalize(glm::cross(tangent, glm::vec3(-1, 0, 0)));
				}
			}
			if(!isLoop && normal.x == 0 && normal.y == 0)
			{
				return glm::vec3(0, 1, 0);
			}

			return normal;
		}
		
	};
}