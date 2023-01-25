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
        float width = 0.2f;
        float height = 0.2f;
        for (int i = 0; i < splinePosition.size() - 1; i++)
        {
            glm::vec3 tangent1 = tangents[i];
            glm::vec3 normal1 = normals[i];
            glm::vec3 directionLeft1 = glm::cross(tangent1, normal1);

            glm::vec3 tangent2 = tangents[i + 1];
            glm::vec3 normal2 = normals[i + 1];
            glm::vec3 directionLeft2 = glm::cross(tangent2, normal2);

            glm::vec3 offsetRight1 = 2 *width * directionLeft1;
            glm::vec3 offsetUp1 = 2 *height * normal1;

            glm::vec3 offsetRight2 = 2 * width * directionLeft2;
            glm::vec3 offsetUp2 = 2 * height * normal2;


            //down middle
            vertices.push_back(glimac::ShapeVertex(splinePosition[i] - width * directionLeft1 - height * normal1, -normal1, glm::vec2(0, splineRatio[i])));
            vertices.push_back(glimac::ShapeVertex(splinePosition[i + 1] + width * directionLeft2 - height * normal2, -normal2, glm::vec2(1, splineRatio[i])));
            vertices.push_back(glimac::ShapeVertex(splinePosition[i] + width * directionLeft1 - height * normal1, -normal1, glm::vec2(1, splineRatio[i])));
            vertices.push_back(glimac::ShapeVertex(splinePosition[i + 1] - width * directionLeft2 - height * normal2, -normal2, glm::vec2(0, splineRatio[i])));

            indices.push_back(index);
            indices.push_back(index + 1);
            indices.push_back(index + 2);
            indices.push_back(index);
            indices.push_back(index + 3);
            indices.push_back(index + 1);

            index += 4;

            //right middle
            vertices.push_back(glimac::ShapeVertex(splinePosition[i] + width * directionLeft1 + height * normal1, -directionLeft1, glm::vec2(0, splineRatio[i])));
            vertices.push_back(glimac::ShapeVertex(splinePosition[i] + width * directionLeft1 - height * normal1, -directionLeft1, glm::vec2(1, splineRatio[i])));
            vertices.push_back(glimac::ShapeVertex(splinePosition[i + 1] + width * directionLeft2 - height * normal2, -directionLeft2, glm::vec2(1, splineRatio[i])));
            vertices.push_back(glimac::ShapeVertex(splinePosition[i + 1] + width * directionLeft2 + height * normal2, -directionLeft2, glm::vec2(0, splineRatio[i])));

            indices.push_back(index);
            indices.push_back(index + 1);
            indices.push_back(index + 2);
            indices.push_back(index);
            indices.push_back(index + 2);
            indices.push_back(index + 3);

            index += 4;

            //left middle
            vertices.push_back(glimac::ShapeVertex(splinePosition[i] - width * directionLeft1 - height * normal1, directionLeft1, glm::vec2(1, splineRatio[i])));
            vertices.push_back(glimac::ShapeVertex(splinePosition[i] - width * directionLeft1 + height * normal1, directionLeft1, glm::vec2(0, splineRatio[i])));
            vertices.push_back(glimac::ShapeVertex(splinePosition[i + 1] - width * directionLeft2 - height * normal2, directionLeft2, glm::vec2(1, splineRatio[i])));
            vertices.push_back(glimac::ShapeVertex(splinePosition[i + 1] - width * directionLeft2 + height * normal2, directionLeft2, glm::vec2(0, splineRatio[i])));

            indices.push_back(index);
            indices.push_back(index + 1);
            indices.push_back(index + 2);
            indices.push_back(index + 1);
            indices.push_back(index + 3);
            indices.push_back(index + 2);

            index += 4;

            //down right
            vertices.push_back(glimac::ShapeVertex(splinePosition[i] - width * directionLeft1 - height * normal1 + offsetRight1 + offsetUp1, -normal1, glm::vec2(0, splineRatio[i])));
            vertices.push_back(glimac::ShapeVertex(splinePosition[i + 1] + width * directionLeft2 - height * normal2 + offsetRight2 + offsetUp2, -normal2, glm::vec2(1, splineRatio[i])));
            vertices.push_back(glimac::ShapeVertex(splinePosition[i] + width * directionLeft1 - height * normal1 + offsetRight1 + offsetUp1, -normal1, glm::vec2(1, splineRatio[i])));
            vertices.push_back(glimac::ShapeVertex(splinePosition[i + 1] - width * directionLeft2 - height * normal2 + offsetRight2 + offsetUp2, -normal2, glm::vec2(0, splineRatio[i])));

            indices.push_back(index);
            indices.push_back(index + 1);
            indices.push_back(index + 2);
            indices.push_back(index);
            indices.push_back(index + 3);
            indices.push_back(index + 1);

            index += 4;

            //right right
            vertices.push_back(glimac::ShapeVertex(splinePosition[i] + width * directionLeft1 + height * normal1 + offsetRight1 + offsetUp1, -directionLeft1, glm::vec2(0, splineRatio[i])));
            vertices.push_back(glimac::ShapeVertex(splinePosition[i] + width * directionLeft1 - height * normal1 + offsetRight1 + offsetUp1, -directionLeft1, glm::vec2(1, splineRatio[i])));
            vertices.push_back(glimac::ShapeVertex(splinePosition[i + 1] + width * directionLeft2 - height * normal2 + offsetRight2 + offsetUp2, -directionLeft2, glm::vec2(1, splineRatio[i])));
            vertices.push_back(glimac::ShapeVertex(splinePosition[i + 1] + width * directionLeft2 + height * normal2 + offsetRight2 + offsetUp2, -directionLeft2, glm::vec2(0, splineRatio[i])));

            indices.push_back(index);
            indices.push_back(index + 1);
            indices.push_back(index + 2);
            indices.push_back(index);
            indices.push_back(index + 2);
            indices.push_back(index + 3);

            index += 4;
            
            //top right
            vertices.push_back(glimac::ShapeVertex(splinePosition[i] - 0.3f*width * directionLeft1 - height * normal1 + offsetRight1 * 1.4f + offsetUp1*2.0f, normal1, glm::vec2(0, splineRatio[i])));
            vertices.push_back(glimac::ShapeVertex(splinePosition[i] + 0.3f*width * directionLeft1 - height * normal1 + offsetRight1 * 1.4f + offsetUp1*2.0f, normal1, glm::vec2(1, splineRatio[i])));
            vertices.push_back(glimac::ShapeVertex(splinePosition[i + 1] + 0.3f*width * directionLeft2 - height * normal2 + offsetRight2*1.4f + offsetUp2*2.0f, normal2, glm::vec2(1, splineRatio[i])));
            vertices.push_back(glimac::ShapeVertex(splinePosition[i + 1] - 0.3f*width * directionLeft2 - height * normal2 + offsetRight2*1.4f + offsetUp2*2.0f, normal2, glm::vec2(0, splineRatio[i])));

            indices.push_back(index);
            indices.push_back(index + 1);
            indices.push_back(index + 2);
            indices.push_back(index);
            indices.push_back(index + 2);
            indices.push_back(index + 3);

            index += 4;

            //right1 left
            vertices.push_back(glimac::ShapeVertex(splinePosition[i] + width * directionLeft1 - 0.7f * height * normal1 + offsetRight1 * 0.75f + offsetUp1 * 1.15f, -directionLeft1, glm::vec2(1, splineRatio[i])));
            vertices.push_back(glimac::ShapeVertex(splinePosition[i] + width * directionLeft1 + 0.7f*height * normal1 + offsetRight1 * 0.75f + offsetUp1*1.15f, -directionLeft1, glm::vec2(0, splineRatio[i])));
            vertices.push_back(glimac::ShapeVertex(splinePosition[i + 1] + width * directionLeft2 - 0.7f*height * normal2 + offsetRight2 * 0.75f + offsetUp2*1.15f, -directionLeft2, glm::vec2(1, splineRatio[i])));
            vertices.push_back(glimac::ShapeVertex(splinePosition[i + 1] + width * directionLeft2 + 0.7f*height * normal2 + offsetRight2 * 0.75f + offsetUp2*1.15f, -directionLeft2, glm::vec2(0, splineRatio[i])));

            indices.push_back(index);
            indices.push_back(index + 1);
            indices.push_back(index + 2);
            indices.push_back(index + 1);
            indices.push_back(index + 3);
            indices.push_back(index + 2);

            index += 4;

            //down left
            vertices.push_back(glimac::ShapeVertex(splinePosition[i] - width * directionLeft1 - height * normal1 - offsetRight1 + offsetUp1, -normal1, glm::vec2(0, splineRatio[i])));
            vertices.push_back(glimac::ShapeVertex(splinePosition[i + 1] + width * directionLeft2 - height * normal2 - offsetRight2 + offsetUp2, -normal2, glm::vec2(1, splineRatio[i])));
            vertices.push_back(glimac::ShapeVertex(splinePosition[i] + width * directionLeft1 - height * normal1 - offsetRight1 + offsetUp1, -normal1, glm::vec2(1, splineRatio[i])));
            vertices.push_back(glimac::ShapeVertex(splinePosition[i + 1] - width * directionLeft2 - height * normal2 - offsetRight2 + offsetUp2, -normal2, glm::vec2(0, splineRatio[i])));

            indices.push_back(index);
            indices.push_back(index + 1);
            indices.push_back(index + 2);
            indices.push_back(index);
            indices.push_back(index + 3);
            indices.push_back(index + 1);

            index += 4;

            //left left
            vertices.push_back(glimac::ShapeVertex(splinePosition[i] + width * directionLeft1 - height * normal1 - offsetRight1 * 2.0f + offsetUp1, directionLeft1, glm::vec2(1, splineRatio[i])));
            vertices.push_back(glimac::ShapeVertex(splinePosition[i] + width * directionLeft1 + height * normal1 - offsetRight1 * 2.0f + offsetUp1, directionLeft1, glm::vec2(0, splineRatio[i])));
            vertices.push_back(glimac::ShapeVertex(splinePosition[i + 1] + width * directionLeft2 - height * normal2 - offsetRight2 * 2.0f + offsetUp2, directionLeft2, glm::vec2(1, splineRatio[i])));
            vertices.push_back(glimac::ShapeVertex(splinePosition[i + 1] + width * directionLeft2 + height * normal2 - offsetRight2*2.0f + offsetUp2, directionLeft2, glm::vec2(0, splineRatio[i])));

            indices.push_back(index);
            indices.push_back(index + 1);
            indices.push_back(index + 2);

            indices.push_back(index + 1);
            indices.push_back(index + 3);
            indices.push_back(index + 2);

            index += 4;

            //top left
            vertices.push_back(glimac::ShapeVertex(splinePosition[i] - 0.3f * width * directionLeft1 - height * normal1 - offsetRight1 * 1.35f + offsetUp1 * 2.0f, normal1, glm::vec2(0, splineRatio[i])));
            vertices.push_back(glimac::ShapeVertex(splinePosition[i] + 0.3f * width * directionLeft1 - height * normal1 - offsetRight1 * 1.35f + offsetUp1 * 2.0f, normal1, glm::vec2(1, splineRatio[i])));
            vertices.push_back(glimac::ShapeVertex(splinePosition[i + 1] + 0.3f * width * directionLeft2 - height * normal2 - offsetRight2 * 1.35f + offsetUp2 * 2.0f, normal2, glm::vec2(1, splineRatio[i])));
            vertices.push_back(glimac::ShapeVertex(splinePosition[i + 1] - 0.3f * width * directionLeft2 - height * normal2 - offsetRight2 * 1.35f + offsetUp2 * 2.0f, normal2, glm::vec2(0, splineRatio[i])));

            indices.push_back(index);
            indices.push_back(index + 1);
            indices.push_back(index + 2);
            indices.push_back(index);
            indices.push_back(index + 2);
            indices.push_back(index + 3);

            index += 4;

            //right1 left
            vertices.push_back(glimac::ShapeVertex(splinePosition[i] + width * directionLeft1 + 0.7f * height * normal1 - offsetRight1 * 1.7f + offsetUp1 * 1.15f, -directionLeft1, glm::vec2(0, splineRatio[i])));
            vertices.push_back(glimac::ShapeVertex(splinePosition[i] + width * directionLeft1 - 0.7f * height * normal1 - offsetRight1 * 1.7f + offsetUp1 * 1.15f, -directionLeft1, glm::vec2(1, splineRatio[i])));
            vertices.push_back(glimac::ShapeVertex(splinePosition[i + 1] + width * directionLeft2 - 0.7f * height * normal2 - offsetRight2 * 1.7f + offsetUp2 * 1.15f, -directionLeft2, glm::vec2(1, splineRatio[i])));
            vertices.push_back(glimac::ShapeVertex(splinePosition[i + 1] + width * directionLeft2 + 0.7f * height * normal2 - offsetRight2 * 1.7f + offsetUp2 * 1.15f, -directionLeft2, glm::vec2(0, splineRatio[i])));

            indices.push_back(index);
            indices.push_back(index + 1);
            indices.push_back(index + 2);
            indices.push_back(index);
            indices.push_back(index + 2);
            indices.push_back(index + 3);

            index += 4;

            //top middle
            vertices.push_back(glimac::ShapeVertex(splinePosition[i] - 2.5f * width * directionLeft1 - height * normal1 + offsetUp1 * 1.3f, normal1, glm::vec2(0, splineRatio[i])));
            vertices.push_back(glimac::ShapeVertex(splinePosition[i] + 2.5f * width * directionLeft1 - height * normal1 + offsetUp1 * 1.3f, normal1, glm::vec2(1, splineRatio[i])));
            vertices.push_back(glimac::ShapeVertex(splinePosition[i + 1] + 2.5f * width * directionLeft2 - height * normal2 + offsetUp2 * 1.3f, normal1, glm::vec2(1, splineRatio[i])));
            vertices.push_back(glimac::ShapeVertex(splinePosition[i + 1] - 2.5f * width * directionLeft2 - height * normal2 + offsetUp2 * 1.3f, normal1, glm::vec2(0, splineRatio[i])));

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