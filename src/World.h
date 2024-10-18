#pragma once

#include "EngineTypes.h"
#include "SimplexNoise.h"
#include "HexagonalBlock.h"


const int CHUNK_SIZE = 8;
const int RENDER_DISTANCE = 32;
const int MAX_HEIGHT = 30;

const float FREQUENCY = 0.01f;

struct Vec2Comparator {
	bool operator()(const glm::vec2& lhs, const glm::vec2& rhs) const {
		if (lhs.x != rhs.x)
			return lhs.x < rhs.x;
		return lhs.y < rhs.y;
	}
};

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
	ChunkQueue AddChunkQueue;
	ChunkQueue RemoveChunkQueue;

	int prevNearestChunkX = std::numeric_limits<int>::max();
	int prevNearestChunkY = std::numeric_limits<int>::max();

	std::map<glm::vec2, std::vector<int>, Vec2Comparator> chunks;
	std::vector<int> emptyIndexes;

public:

	ObjectBufferData* objectBuffer;
	int chunkInstanceCount = 0;

	void UpdateChunks(glm::vec3 playerPos)
	{
		int nearestChunkX = glm::round(playerPos.x / (CHUNK_SIZE * 2.f * glm::sqrt(3.f)));
		int nearestChunkY = glm::round(playerPos.z / (CHUNK_SIZE * 2.f * 1.5f));

		if (nearestChunkX == prevNearestChunkX && nearestChunkY == prevNearestChunkY)
		{
			AddChunkQueue.pop_function();
			return;
		}

		AddChunkQueue.clear();

		prevNearestChunkX = nearestChunkX;
		prevNearestChunkY = nearestChunkY;
	
		std::vector<glm::vec2> chunksToRemove;
		for (auto& chunk : chunks)
		{
			glm::vec2 chunkCoord = chunk.first;

			if (chunkCoord.x > (nearestChunkX + RENDER_DISTANCE) || chunkCoord.x < (nearestChunkX - RENDER_DISTANCE) || chunkCoord.y > (nearestChunkY + RENDER_DISTANCE) || chunkCoord.y < (nearestChunkY - RENDER_DISTANCE))
			{
				chunksToRemove.push_back(chunkCoord);
				emptyIndexes.insert(emptyIndexes.begin(), chunk.second.begin(), chunk.second.end());
			}
		}

		for (glm::vec2 key : chunksToRemove) {
			chunks.erase(key);
		}

		for (int x = -RENDER_DISTANCE; x <= RENDER_DISTANCE; x++)
		{
			for (int y = -RENDER_DISTANCE; y <= RENDER_DISTANCE; y++)
			{

				glm::vec2 chunkID(nearestChunkX + x, nearestChunkY + y);

				if (chunks.find(chunkID) != chunks.end())
					continue;

				AddChunkQueue.push_function([this, chunkID]() {
					chunks.emplace(chunkID, AddChunk(chunkID.x, chunkID.y));
					});
				
			}
		}

		AddChunkQueue.pop_function();
	}

	std::vector<int> AddChunk(int chunkX, int chunkY)
	{
		std::vector<int> bufferIndexes;

		glm::vec3 chunkCenter = glm::vec3(chunkX * (CHUNK_SIZE * 2.f * glm::sqrt(3.f)), 0, chunkY * (CHUNK_SIZE * 2.f * 1.5f));

		for (int x = -(CHUNK_SIZE) * 2; x < (CHUNK_SIZE) * 2; x++)
		{
			for (int y = -(CHUNK_SIZE); y < (CHUNK_SIZE); y++)
			{
				if ((x + y) % 2 == 0)
				{
					glm::vec3 position(x * 0.5f * glm::sqrt(3.f), 0, y * 1.5f);

					position += chunkCenter;

					int maxHeight = CalculateHeight(glm::vec2(position.x, position.z));

					for (int h = 0; h <= maxHeight; h++)
					{
						position.y = h;

						if (h == maxHeight || checkAround(position))
						{
							glm::mat4 modelMatrix = HexagonalBlock::translate(position);

							/*
							Object block;
							block.chunkID = glm::vec2(chunkX, chunkY);
							block.renderMatrix = modelMatrix;
							block.blockCoords = glm::vec3(position.x, h, position.z);
							blocks.push_back(block);
							*/

							ObjectBufferData bufferData;
							bufferData.renderMatrix = modelMatrix;
							bufferData.boundingSphereCenter = position + glm::vec3(0, 0.5f, 0);
							bufferData.boundingSphereRadius = 1.f;

							if (emptyIndexes.size() == 0)
							{
								objectBuffer[chunkInstanceCount] = bufferData;
								bufferIndexes.push_back(chunkInstanceCount);
								chunkInstanceCount++;
							}
							else
							{
								objectBuffer[emptyIndexes.back()] = bufferData;
								bufferIndexes.push_back(emptyIndexes.back());
								emptyIndexes.pop_back();
							}

						}	
					}
				}
							
			}
		}
		return bufferIndexes;
	}

	static int CalculateHeight(glm::vec2 position)
	{
		float noiseValue = SimplexNoise::noise(position.x * FREQUENCY, position.y * FREQUENCY);

		int height = static_cast<int>((noiseValue + 1.0f) * 0.5f * (MAX_HEIGHT - 1));

		return height;
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
