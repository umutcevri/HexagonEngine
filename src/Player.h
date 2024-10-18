#pragma once
#include "World.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Player_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

glm::vec2 hexagonVertices[6] = {
    {0.5f * glm::sqrt(3), 0.5f},
    {0, 1.f},
    {-0.5f * glm::sqrt(3), 0.5f},
    {-0.5f * glm::sqrt(3), -0.5f},
    {0, -1.f},
    {0.5f * glm::sqrt(3), -0.5f}
};

const float MOVEMENT_SPEED = 50.f;
const float colliderRadius = 0.4f;
const float colliderHeight = 1.8f;

const float gravity = -9.81f;



class Player
{
    glm::vec3 Position;
    glm::vec3 AttemptedPosition;
    glm::vec3 Front;
    glm::vec3 Right;

    World world;

    float MovementSpeed;

public:
    float deltaTime;

    Player(glm::vec3 position = glm::vec3(0))
    {
        Position = position;
        AttemptedPosition = Position;
        MovementSpeed = MOVEMENT_SPEED;
    }

    void ProcessKeyboard(Player_Movement direction)
    {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            AttemptedPosition += Front * velocity;
        if (direction == BACKWARD)
            AttemptedPosition -= Front * velocity;
        if (direction == LEFT)
            AttemptedPosition -= Right * velocity;
        if (direction == RIGHT)
            AttemptedPosition += Right * velocity;
    }

    void Move(std::vector<Object> &blocks)
    {
        CheckForCollisions(blocks);
    }

    glm::vec3 GetPosition()
    {
        return Position;
    }

    void updateFront(glm::vec3 front)
    {
        Front = front;
        //Front = glm::normalize(glm::vec3(front.x, 0, front.z));
    }

    void updateRight(glm::vec3 right)
    {
        Right = right;
        //Right = glm::normalize(glm::vec3(right.x, 0, right.z));
    }

    void CheckForCollisions(std::vector<Object> &blocks)
    {
        float a = AttemptedPosition.x / (0.5f * glm::sqrt(3.f));
        float b = AttemptedPosition.z / 1.5f;

        int x = glm::round(a);
        int y = glm::round(b);
        
        
        glm::vec3 targetBlock(x * 0.5f * glm::sqrt(3.f), 0, y * 1.5f);
        auto it = std::find_if(blocks.begin(), blocks.end(), [targetBlock](const Object& element) {
            return element.blockCoords == targetBlock;
            });

        if (it != blocks.end()) {
            //std::cout << "found" << std::endl;
            it->isSolidColor = true;
            it->color = glm::vec3(1, 0, 1);
        }
        
        
        

        /*
        for (auto& block : blocks)
        {
            block.isSolidColor = true;
            block.color = glm::vec3(1, 0, 1);
        }
        */
        

        
        

        /*
        std::cout << x << " " << y << std::endl;
        std::cout << x + 2 << " " << y << std::endl;
        std::cout << x - 2 << " " << y << std::endl;
        std::cout << x + 1 << " " << y + 1 << std::endl;
        std::cout << x + 1 << " " << y - 1 << std::endl;
        std::cout << x - 1 << " " << y + 1 << std::endl;
        std::cout << x - 1 << " " << y - 1 << std::endl;
        */

        Position = AttemptedPosition;
        //std::cout << Position.x << " " << Position.y << " " << Position.z << std::endl;
    }

    bool CheckForHexagonCollisions(int x, int y)
    {
        glm::vec2 hexagonPos(x * (0.5f * glm::sqrt(3.f)), y * 1.5f);
        for (int i = 0; i < 6; i++)
        {
            glm::vec2 vA = hexagonPos + hexagonVertices[i];
            glm::vec2 vB = hexagonPos + hexagonVertices[(i + 1) % 6];


        }
    }

};
