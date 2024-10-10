#pragma once

#include "EngineTypes.h"
#include "SimplexNoise.h"
#include "HexagonalBlock.h"


const int CHUNK_SIZE = 16;
const int RENDER_DISTANCE = 5;
const int MAX_HEIGHT = 15;

const float FREQUENCY = 0.03f;

class World
{
	HexagonalBlock blockData;

	std::vector<glm::vec2> chunks;

	int prevNearestChunkX = std::numeric_limits<int>::max();
	int prevNearestChunkY = std::numeric_limits<int>::max();

public:

	void UpdateChunks(glm::vec3 playerPos, std::vector<Block> &blocks)
	{
		int nearestChunkX = glm::round(playerPos.x / (CHUNK_SIZE * glm::sqrt(3.f)));
		int nearestChunkY = glm::round(playerPos.z / (CHUNK_SIZE * 1.5f));

		if (nearestChunkX == prevNearestChunkX && nearestChunkY == prevNearestChunkY)
		{
			return;
		}

		prevNearestChunkX = nearestChunkX;
		prevNearestChunkY = nearestChunkY;


		std::vector<int> chunksIDsToRemove;

		for (int i = 0; i < chunks.size(); i++)
		{
			if ((chunks[i].x > (nearestChunkX + RENDER_DISTANCE) || chunks[i].x < (nearestChunkX - RENDER_DISTANCE)) && (chunks[i].y > (nearestChunkY + RENDER_DISTANCE) || chunks[i].y < (nearestChunkY - RENDER_DISTANCE)))
			{
				std::cout << "ahash" << std::endl;
				chunksIDsToRemove.push_back(i);
				//chunks.erase(std::remove(chunks.begin(), chunks.end(), chunks[i]), chunks.end());
				//RemoveChunk(chunks[i].x, chunks[i].y, blocks);
			}
		}

		//removeChunksFromIDArray(chunksIDsToRemove);

		for (int x = -RENDER_DISTANCE; x <= RENDER_DISTANCE; x++)
		{
			for (int y = -RENDER_DISTANCE; y <= RENDER_DISTANCE; y++)
			{
				glm::vec2 chunkID(nearestChunkX + x, nearestChunkY + y);

				if (!containsChunk(chunks, chunkID))
				{
					chunks.push_back(chunkID);
					AddChunk(nearestChunkX + x, nearestChunkY + y, blocks);
				}				
				
			}
		}
	}

	void AddChunk(int chunkX, int chunkY, std::vector<Block> &blocks)
	{
		//std::cout << chunkX << " " << chunkY << std::endl;
		glm::vec3 chunkCenter = glm::vec3(chunkX * ((CHUNK_SIZE) * glm::sqrt(3.f)), 0, chunkY * ((CHUNK_SIZE) * 1.5f));

		int a = 0;

		for (int x = -(CHUNK_SIZE) * 2; x < (CHUNK_SIZE) * 2; x++)
		{
			for (int y = -(CHUNK_SIZE); y < (CHUNK_SIZE); y++)
			{
				if ((x + y) % 2 == 0)
				{
					a++;
					glm::vec3 position(x * 0.5 * glm::sqrt(3.f), 0, y * 1.5f);

					position += chunkCenter;

					int maxHeight = CalculateHeight(glm::vec2(position.x, position.z));

					for (int h = maxHeight; h <= maxHeight; h++)
					{
						position.y = h;

						glm::mat4 modelMatrix = blockData.translate(position);

						Block block;
						block.chunkID = glm::vec2(chunkX, chunkY);
						block.renderMatrix = modelMatrix;
						blocks.push_back(block);
					}
				}
							
			}
		}
		//std::cout << a << std::endl;
	}

	void RemoveChunk(int chunkX, int chunkY, std::vector<Block>& blocks)
	{
		glm::vec2 targetChunkID = glm::vec2(chunkX, chunkY);

		blocks.erase(
			std::remove_if(
				blocks.begin(),
				blocks.end(),
				[&targetChunkID](const Block& block) {
					return block.chunkID == targetChunkID;
				}),
			blocks.end()
		);
	}

	static int CalculateHeight(glm::vec2 position)
	{
		float noiseValue = SimplexNoise::noise(position.x * FREQUENCY, position.y * FREQUENCY);

		int height = static_cast<int>((noiseValue + 1.0f) * 0.5f * MAX_HEIGHT);

		return height;
	}

	bool containsChunk(const std::vector<glm::vec2>& array, const glm::vec2& target) {
		return std::find(array.begin(), array.end(), target) != array.end();
	}

	void removeChunksFromIDArray(std::vector<int> &array)
	{
		for (auto& index : array)
		{
			chunks.erase(chunks.begin() + index);
		}
	}
};
