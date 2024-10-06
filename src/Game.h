#pragma once

#include "Engine.h"
#include "HexagonalBlock.h"
#include "SimplexNoise.h"

const int CHUNK_SIZE = 500;
const int MAX_HEIGHT = 15;

const float frequency = 0.01f;

class HexagonGame
{
	HexagonEngine Engine;

	HexagonalBlock block;
	SimplexNoise noise;

	bool bQuit = false;

public:

	void Run()
	{
		Engine.init();
		Engine.create_mesh(block.indices, block.vertices);

		UpdateObjectMatrices();

		while (!bQuit)
		{
			Update();
		}

		Engine.cleanup();
	}

private:
	void Update()
	{
		Engine.render();
		bQuit = Engine.bQuit;
	}

	void UpdateObjectMatrices()
	{
		Engine.modelMatrices.clear();

		//glm::vec3 center = Engine->get_camera()->Position;
		glm::vec3 center = glm::vec3(0);

		glm::mat4 modelMatrix = block.inverseMatrix;

		glm::vec3 position = glm::vec3(0, 0, 0);

		float noiseValue = noise.noise(position.x * frequency, position.z * frequency);

		int intHeight = static_cast<int>((noiseValue + 1.0f) * 0.5f * MAX_HEIGHT * 2);

		float height = intHeight / 2.0f;
		std::cout << height << std::endl;

		position.y = height;

		modelMatrix = glm::translate(modelMatrix, center + position);
		modelMatrix = modelMatrix * block.defaultMatrix;

		Engine.modelMatrices.push_back(modelMatrix);


		for (int i = 1; i <= CHUNK_SIZE; i++)
		{
			std::vector<glm::vec3> corners;
			float currentDegree = 0.f;
			for (int a = 0; a < 6; a++)
			{
				glm::vec3 position = glm::vec3(i * (0.5f * glm::sqrt(3.f)) * glm::cos(glm::radians(currentDegree)), 0, i * (0.5f * glm::sqrt(3.f)) * glm::sin(glm::radians(currentDegree)));

				corners.push_back(position);

				float noiseValue = noise.noise(position.x * frequency, position.z * frequency);

				int intHeight = static_cast<int>((noiseValue + 1.0f) * 0.5f * MAX_HEIGHT * 2);

				float height = intHeight / 2.0f;
				std::cout << height << std::endl;

				position.y = height;

				glm::mat4 modelMatrix = block.inverseMatrix;
				modelMatrix = glm::translate(modelMatrix, center + position);
				modelMatrix = modelMatrix * block.defaultMatrix;

				Engine.modelMatrices.push_back(modelMatrix);

				currentDegree += 60.f;
			}

			for (int a = 0; a < 6; a++)
			{
				glm::vec3 pos1 = corners[a];
				glm::vec3 pos2 = corners[glm::mod(static_cast<float>(a + 1), 6.f)];

				for (int b = 1; b < i; b++)
				{
					float alpha = static_cast<float>(b) / static_cast<float>(i);
					glm::vec3 position = pos1 * (1 - alpha) + pos2 * alpha;

					float noiseValue = noise.noise(position.x * frequency, position.z * frequency);

					int intHeight = static_cast<int>((noiseValue + 1.0f) * 0.5f * MAX_HEIGHT * 2);

					float height = intHeight / 2.0f;

					position.y = height;

					glm::mat4 modelMatrix = block.inverseMatrix;
					modelMatrix = glm::translate(modelMatrix, center + position);
					modelMatrix = modelMatrix * block.defaultMatrix;
					Engine.modelMatrices.push_back(modelMatrix);
				}
			}
		}
	}
};