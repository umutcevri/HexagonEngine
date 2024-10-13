#pragma once

#include "EngineTypes.h"
#include "SimplexNoise.h"
#include "HexagonalBlock.h"


const int CHUNK_SIZE = 8;
const int RENDER_DISTANCE = 10;
const int MAX_HEIGHT = 15;

const float FREQUENCY = 0.01f;

struct ChunkQueue
{
	std::deque<std::function<void()>> chunks;

	void push_function(std::function<void()>&& function) {
		chunks.push_back(function);
	}

	void pop_function()
	{
		if (!chunks.empty())
		{
			chunks.front()();
			chunks.pop_front();
		}	
	}

	void clear()
	{
		chunks.clear();
	}
};

class World
{

	std::vector<glm::vec2> chunks;
	ChunkQueue AddChunkQueue;
	ChunkQueue RemoveChunkQueue;

	int prevNearestChunkX = std::numeric_limits<int>::max();
	int prevNearestChunkY = std::numeric_limits<int>::max();

public:

	void UpdateChunks(glm::vec3 playerPos, std::vector<Object> &blocks)
	{
		int nearestChunkX = glm::round(playerPos.x / (CHUNK_SIZE * 2.f * glm::sqrt(3.f)));
		int nearestChunkY = glm::round(playerPos.z / (CHUNK_SIZE * 2.f * 1.5f));

		if (nearestChunkX == prevNearestChunkX && nearestChunkY == prevNearestChunkY)
		{
			AddChunkQueue.pop_function();
			RemoveChunkQueue.pop_function();
			return;
		}

		AddChunkQueue.clear();
		RemoveChunkQueue.clear();

		prevNearestChunkX = nearestChunkX;
		prevNearestChunkY = nearestChunkY;

		for (int i = 0; i < chunks.size(); i++)
		{
			if (chunks[i].x > (nearestChunkX + RENDER_DISTANCE) || chunks[i].x < (nearestChunkX - RENDER_DISTANCE) || chunks[i].y >(nearestChunkY + RENDER_DISTANCE) || chunks[i].y < (nearestChunkY - RENDER_DISTANCE))
			{
				glm::vec2 removeChunk = chunks[i];

				RemoveChunkQueue.push_function([this, removeChunk, &blocks]() {
					chunks.erase(std::remove(chunks.begin(), chunks.end(), removeChunk), chunks.end());
					RemoveChunk(removeChunk.x, removeChunk.y, blocks);
					});
			}
		}

		RemoveChunkQueue.pop_function();

		for (int x = -RENDER_DISTANCE; x <= RENDER_DISTANCE; x++)
		{
			for (int y = -RENDER_DISTANCE; y <= RENDER_DISTANCE; y++)
			{
				glm::vec2 chunkID(nearestChunkX + x, nearestChunkY + y);

				if (containsChunk(chunks, chunkID))
					continue;
			
				AddChunkQueue.push_function([this, chunkID, &blocks]() {
					chunks.push_back(chunkID);
					AddChunk(chunkID.x, chunkID.y, blocks);
				});								
			}
		}

		AddChunkQueue.pop_function();
	}

	void AddChunk(int chunkX, int chunkY, std::vector<Object> &blocks)
	{
		//std::cout << chunkX << " " << chunkY << std::endl;
		glm::vec3 chunkCenter = glm::vec3(chunkX * (CHUNK_SIZE * 2.f * glm::sqrt(3.f)), 0, chunkY * (CHUNK_SIZE * 2.f * 1.5f));

		int a = 0;

		for (int x = -(CHUNK_SIZE) * 2; x < (CHUNK_SIZE) * 2; x++)
		{
			for (int y = -(CHUNK_SIZE); y < (CHUNK_SIZE); y++)
			{
				if ((x + y) % 2 == 0)
				{
					a++;
					glm::vec3 position(x * 0.5f * glm::sqrt(3.f), 0, y * 1.5f);

					position += chunkCenter;

					int maxHeight = CalculateHeight(glm::vec2(position.x, position.z));

					for (int h = 0; h <= maxHeight; h++)
					{
						position.y = h;

						if (h == maxHeight || checkAround(position))
						{
							glm::mat4 modelMatrix = HexagonalBlock::translate(position);

							Object block;
							block.chunkID = glm::vec2(chunkX, chunkY);
							block.renderMatrix = modelMatrix;
							block.blockCoords = glm::vec3(position.x, h, position.z);
							blocks.push_back(block);
						}	
					}
				}
							
			}
		}
		//std::cout << a << std::endl;
	}

	void RemoveChunk(int chunkX, int chunkY, std::vector<Object>& blocks)
	{
		glm::vec2 targetChunkID = glm::vec2(chunkX, chunkY);

		blocks.erase(
			std::remove_if(
				blocks.begin(),
				blocks.end(),
				[&targetChunkID](const Object& block) {
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

	void removeChunksFromArray(std::vector<glm::vec2> &array)
	{
		for (auto &chunk : array)
		{
			chunks.erase(std::remove(chunks.begin(), chunks.end(), chunk), chunks.end());
		}
	}

	bool checkAround(glm::vec3 coords)
	{
		if (CalculateHeight(glm::vec2(coords.x + glm::sqrt(3.0f), coords.z)) < coords.y)
		{
			return true;
		}
		if (CalculateHeight(glm::vec2(coords.x - glm::sqrt(3.0f), coords.z)) < coords.y)
		{
			return true;
		}
		if (CalculateHeight(glm::vec2(coords.x + (0.5f * glm::sqrt(3.0f)), coords.z + 1.5f)) < coords.y)
		{
			return true;
		}
		if (CalculateHeight(glm::vec2(coords.x - (0.5f * glm::sqrt(3.0f)), coords.z + 1.5f)) < coords.y)
		{
			return true;
		}
		if (CalculateHeight(glm::vec2(coords.x + (0.5f * glm::sqrt(3.0f)), coords.z - 1.5f)) < coords.y)
		{
			return true;
		}
		if (CalculateHeight(glm::vec2(coords.x - (0.5f * glm::sqrt(3.0f)), coords.z - 1.5f)) < coords.y)
		{
			return true;
		}

		return false;
	}
};
