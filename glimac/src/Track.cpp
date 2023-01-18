#include <glimac/Track.hpp>
namespace glimac {
	void Track::calculateVertices()
	{
        std::vector<glm::vec3> splinePosition;
        std::vector<float> splineRatio;
        std::vector<glm::vec3> tangents;
        std::vector<glm::vec3> normals;

        float iteration = 0.01f;

        //get spline position, tangent, normal for each ratio
        for (int j = 0; j < splines.size(); j++)
        {
            for (float i = 0.0f; i <= 1.0f; i += iteration)
            {
                splinePosition.push_back(splines[j].getPosition(i));
                tangents.push_back(splines[j].GetCurveTangent(i));
                normals.push_back(splines[j].GetCurveNormal(i));
                splineRatio.push_back(i);
            }
        }

        //create a cube without the front and back face
        int index = 0;
        for (int i = 0; i < splinePosition.size() - 1; i++)
        {
            glm::vec3 tangent1 = tangents[i];
            glm::vec3 normal1 = normals[i];
            glm::vec3 directionRight1 = glm::cross(tangent1, normal1);

            glm::vec3 tangent2 = tangents[i + 1];
            glm::vec3 normal2 = normals[i + 1];
            glm::vec3 directionRight2 = glm::cross(tangent2, normal2);

            //up
            vertices.push_back(glimac::ShapeVertex(splinePosition[i] - 0.5f * directionRight1 + 0.5f * normal1, glm::vec3(0, 0, -1), glm::vec2(0, splineRatio[i])));
            vertices.push_back(glimac::ShapeVertex(splinePosition[i] + 0.5f * directionRight1 + 0.5f * normal1, glm::vec3(0, 0, -1), glm::vec2(1, splineRatio[i])));
            vertices.push_back(glimac::ShapeVertex(splinePosition[i + 1] + 0.5f * directionRight2 + 0.5f * normal2, glm::vec3(0, 0, -1), glm::vec2(1, splineRatio[i])));
            vertices.push_back(glimac::ShapeVertex(splinePosition[i + 1] - 0.5f * directionRight2 + 0.5f * normal2, glm::vec3(0, 0, -1), glm::vec2(0, splineRatio[i])));

            indices.push_back(index);
            indices.push_back(index + 1);
            indices.push_back(index + 2);
            indices.push_back(index);
            indices.push_back(index + 2);
            indices.push_back(index + 3);

            index += 4;
            //down
            vertices.push_back(glimac::ShapeVertex(splinePosition[i] - 0.5f * directionRight1 - 0.5f * normal1, glm::vec3(0, 0, -1), glm::vec2(0, splineRatio[i])));
            vertices.push_back(glimac::ShapeVertex(splinePosition[i] + 0.5f * directionRight1 - 0.5f * normal1, glm::vec3(0, 0, -1), glm::vec2(1, splineRatio[i])));
            vertices.push_back(glimac::ShapeVertex(splinePosition[i + 1] + 0.5f * directionRight2 - 0.5f * normal2, glm::vec3(0, 0, -1), glm::vec2(1, splineRatio[i])));
            vertices.push_back(glimac::ShapeVertex(splinePosition[i + 1] - 0.5f * directionRight2 - 0.5f * normal2, glm::vec3(0, 0, -1), glm::vec2(0, splineRatio[i])));

            indices.push_back(index);
            indices.push_back(index + 1);
            indices.push_back(index + 2);
            indices.push_back(index);
            indices.push_back(index + 2);
            indices.push_back(index + 3);

            index += 4;

            //right
            vertices.push_back(glimac::ShapeVertex(splinePosition[i] + (0.5f * directionRight1) + 0.5f * normal1, glm::vec3(0, 0, -1), glm::vec2(0, splineRatio[i])));
            vertices.push_back(glimac::ShapeVertex(splinePosition[i] + (0.5f * directionRight1) - 0.5f * normal1, glm::vec3(0, 0, -1), glm::vec2(1, splineRatio[i])));
            vertices.push_back(glimac::ShapeVertex(splinePosition[i + 1] + (0.5f * directionRight2) - 0.5f * normal2, glm::vec3(0, 0, -1), glm::vec2(1, splineRatio[i])));
            vertices.push_back(glimac::ShapeVertex(splinePosition[i + 1] + (0.5f * directionRight2) + 0.5f * normal2, glm::vec3(0, 0, -1), glm::vec2(0, splineRatio[i])));

            indices.push_back(index);
            indices.push_back(index + 1);
            indices.push_back(index + 2);
            indices.push_back(index);
            indices.push_back(index + 2);
            indices.push_back(index + 3);

            index += 4;

            //left
            vertices.push_back(glimac::ShapeVertex(splinePosition[i] - (0.5f * directionRight1) + 0.5f * normal1, glm::vec3(0, 0, -1), glm::vec2(0, splineRatio[i])));
            vertices.push_back(glimac::ShapeVertex(splinePosition[i] - (0.5f * directionRight1) - 0.5f * normal1, glm::vec3(0, 0, -1), glm::vec2(1, splineRatio[i])));
            vertices.push_back(glimac::ShapeVertex(splinePosition[i + 1] - (0.5f * directionRight2) - 0.5f * normal2, glm::vec3(0, 0, -1), glm::vec2(1, splineRatio[i])));
            vertices.push_back(glimac::ShapeVertex(splinePosition[i + 1] - (0.5f * directionRight2) + 0.5f * normal2, glm::vec3(0, 0, -1), glm::vec2(0, splineRatio[i])));

            indices.push_back(index);
            indices.push_back(index + 1);
            indices.push_back(index + 2);
            indices.push_back(index);
            indices.push_back(index + 2);
            indices.push_back(index + 3);

            index += 4;
        }
	}
}