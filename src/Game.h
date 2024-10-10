#pragma once

#include "Engine.h"
#include "Camera.h"
#include "Player.h"
#include "World.h"

class HexagonGame
{
	HexagonEngine Engine;
	Camera camera;
	Player player = Player(glm::vec3(0, 10, 0));
	World world;

	HexagonalBlock block;

	bool bQuit = false;

	

public:
	HexagonGame() {};

	void Run()
	{
		Engine.init();
		Engine.create_mesh(block.indices, block.vertices);

		world.UpdateChunks(player.GetPosition(), Engine.blocksVector);

		Engine.SetFOV(camera.FoV);

		while (!bQuit)
		{
			Update();
		}

		Engine.cleanup();
	}

private:
	void Update()
	{
		player.deltaTime = Engine.deltaTime;

		player.updateFront(camera.Front);
		player.updateRight(camera.Right);

		keyboardInput();
		mouseInput();

		player.Move();

		world.UpdateChunks(player.GetPosition(), Engine.blocksVector);

		Engine.SetViewMatrix(camera.GetViewMatrix());

		Engine.render();

		camera.updatePosition(player.GetPosition());

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
};