#pragma once

#include "EngineTypes.h"

class Circle
{
public:
    static std::vector<Vertex> vertices;
    static std::vector<uint32_t> indices;

	static void Initialize(float radius, int segmentCount)
	{
        Vertex v;
        v.position = glm::vec3(0, 0, 0);

        vertices.push_back(v);

        // Generate vertices around the circumference
        float angleStep = 2.0f * glm::pi<float>() / static_cast<float>(segmentCount);

        for (int i = 0; i < segmentCount; ++i) {
            float angle = i * angleStep;
            float x = radius * cos(angle);
            float y = radius * sin(angle);
            v.position = glm::vec3(x, 0, y);
            vertices.push_back(v);
        }

        for (int i = 1; i < segmentCount; ++i) {
            indices.push_back(0);
            indices.push_back(i);
            indices.push_back(i + 1);
        }
  
        indices.push_back(0);
        indices.push_back(segmentCount);
        indices.push_back(1);
	}
};
