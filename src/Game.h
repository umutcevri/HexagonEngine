#pragma once

#include "Engine.h"
#include "Camera.h"
#include "Player.h"
#include "World.h"
#include "Circle.h"

std::array<Vertex, 38> HexagonalBlock::vertices;
std::array<uint32_t, 72> HexagonalBlock::indices;
glm::mat4 HexagonalBlock::defaultMatrix = glm::mat4(1.f);
glm::mat4 HexagonalBlock::inverseMatrix = glm::mat4(1.f);

std::vector<Vertex> Circle::vertices;
std::vector<uint32_t> Circle::indices;

class HexagonGame
{
	float deltaTime;
	float lastFrame = 0;

	HexagonEngine Engine;
	Camera camera;
	Player player = Player(glm::vec3(0, 10, 0));
	World world;

	bool bQuit = false;

public:
	HexagonGame() {};

	void Run()
	{
		HexagonalBlock::Initialize();
		Circle::Initialize(1, 40);

		Engine.init();

		Engine.create_mesh(HexagonalBlock::indices, HexagonalBlock::vertices);
		//Engine.create_mesh(Circle::indices, Circle::vertices);

		Engine.renderObjectBufferDelete();

		Engine.SetFOV(camera.FoV);

		world.objectBuffer = Engine.getObjectBufferData();

		while (!bQuit)
		{
			Update();
		}

		Engine.cleanup();
	}

private:
	void Update()
	{
		float currentFrame = (float)SDL_GetTicks64() / 1000.0f;
		deltaTime = currentFrame - lastFrame;	
		lastFrame = currentFrame;

		player.deltaTime = deltaTime;

		player.updateFront(camera.Front);
		player.updateRight(camera.Right);

		keyboardInput();
		mouseInput();

		player.Move(Engine.renderObjects[0].instances);

		world.UpdateChunks(player.GetPosition());

		Engine.renderObjects[0].instanceCount = world.chunkInstanceCount;

		//std::cout << world.chunkInstanceCount << std::endl;

		glm::mat4 modelMatrix(1.f);
		modelMatrix = glm::translate(modelMatrix, player.GetPosition() + glm::vec3(0, 0.01f, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
		//Engine.renderObjects[1].instances[0].renderMatrix = modelMatrix;

		camera.updatePosition(player.GetPosition());

		Engine.SetViewMatrix(camera.GetViewMatrix());
		
		Engine.frustum = createFrustumFromCamera(camera, Engine._windowExtent.width / Engine._windowExtent.height, glm::radians(camera.FoV), 0.1f, 10000.f);

		Engine.render();

		bQuit = Engine.bQuit;
	}

	void keyboardInput()
	{
		const Uint8* keystate = SDL_GetKeyboardState(NULL);

		// adjust accordingly
		if (keystate[SDL_SCANCODE_W])
			player.ProcessKeyboard(FORWARD);
		if (keystate[SDL_SCANCODE_S])
			player.ProcessKeyboard(BACKWARD);
		if (keystate[SDL_SCANCODE_A])
			player.ProcessKeyboard(LEFT);
		if (keystate[SDL_SCANCODE_D])
			player.ProcessKeyboard(RIGHT);
		
	}

	void mouseInput()
	{
		int xPos, yPos;
		SDL_GetRelativeMouseState(&xPos, &yPos);

		camera.ProcessMouseMovement(xPos, -yPos);
	}

	Frustum createFrustumFromCamera(const Camera& cam, float aspect, float fovY,
		float zNear, float zFar)
	{
		Frustum frustum;
		const float halfVSide = zFar * tanf(fovY * 0.5f);
		const float halfHSide = zFar * tanf(fovY * 0.5f) * aspect * 2.0f;
		const glm::vec3 frontMultFar = zFar * cam.Front;

		frustum.nearFace = { cam.Position + zNear * cam.Front, cam.Front };
		frustum.farFace = { cam.Position + frontMultFar, -cam.Front };
		frustum.rightFace = { cam.Position,
								glm::cross(frontMultFar - cam.Right * halfHSide, cam.Up) };
		frustum.leftFace = { cam.Position,
								glm::cross(cam.Up,frontMultFar + cam.Right * halfHSide) };
		frustum.topFace = { cam.Position,
								glm::cross(cam.Right, frontMultFar - cam.Up * halfVSide) };
		frustum.bottomFace = { cam.Position,
								glm::cross(frontMultFar + cam.Up * halfVSide, cam.Right) };

		return frustum;
	}


};