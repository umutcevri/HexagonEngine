#include "src/Engine.h"
#include "src/HexagonalBlock.h"
#include "SimplexNoise.h"

#define CHUNK_SIZE 3

int main(int argc, char* argv[])
{
	HexagonEngine Engine;

	Engine.init();

	HexagonalBlock block;

	std::vector<glm::mat4> modelMatrices;

	glm::mat4 modelMatrix = block.inverseMatrix;
	//modelMatrices.push_back(modelMatrix * block.defaultMatrix);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 0));
	modelMatrix = modelMatrix * block.defaultMatrix;
	modelMatrices.push_back(modelMatrix);

	
	for (int i = CHUNK_SIZE; i > 0; i--)
	{
		int numOfHexagons = i * 6;
		float degree = 360.f / numOfHexagons;
		float currentDegree = 0.f;
		for (int a = 0; a < numOfHexagons; a++)
		{
			modelMatrix = block.inverseMatrix;
			modelMatrix = glm::translate(modelMatrix, glm::vec3(i * (0.5f * glm::sqrt(3.f)) * glm::cos(glm::radians(currentDegree)), 0, i * (0.5f * glm::sqrt(3.f)) * glm::sin(glm::radians(currentDegree))));
			std::cout << i * (0.5f * glm::sqrt(3.f)) * glm::cos(currentDegree) << std::endl;
			modelMatrix = modelMatrix * block.defaultMatrix;
			modelMatrices.push_back(modelMatrix);

			currentDegree += degree;
			
		}
	}
	
	/*
	for (int i = 0; i < 1; i++)
	{
		for (int y = 0; y < 1; y++)
		{
			for (int z = 0; z < 1; z++)
			{
				glm::mat4 modelMatrix = glm::mat4(1.f);
				modelMatrix = glm::translate(modelMatrix, glm::vec3(i, y, z));
				modelMatrices.push_back(modelMatrix);
			}
		}	
	}
	*/
	/*
	glm::mat4 modelMatrix = glm::mat4(1.f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.5, 0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(-90.f), glm::vec3(0, 1, 0));
	modelMatrix = glm::rotate(modelMatrix, glm::radians(-90.f), glm::vec3(1, 0, 0));
	//modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.5, 0));

	glm::mat4 worldMatrix = glm::rotate(modelMatrix, glm::radians(90.f), glm::vec3(1, 0, 0));
	worldMatrix = glm::rotate(worldMatrix, glm::radians(90.f), glm::vec3(0, 1, 0));
	worldMatrix = glm::translate(worldMatrix, glm::vec3(0, -0.5, 0));

	worldMatrix = glm::translate(worldMatrix, glm::vec3(0, 0.5, 0));
	
	modelMatrix = worldMatrix * modelMatrix;

	modelMatrices.push_back(modelMatrix);
	*/


	Engine.create_mesh(block.indices, block.vertices, modelMatrices);

	Engine.run();

	Engine.cleanup();

	return 0;
}