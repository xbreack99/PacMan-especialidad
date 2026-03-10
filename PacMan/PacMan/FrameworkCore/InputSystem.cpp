#include "InputSystem.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics.hpp>
#include "InputComponent.h"
#include "TransformComponent.h"

InputSystem::~InputSystem()
{
}

void InputSystem::Update(float deltaTime)
{
	for (auto node : mNodes)
	{
        auto transformComponent = node->GetComponent<TransformComponent>();
        auto inputComponent = node->GetComponent<InputComponent>();

        if (transformComponent && inputComponent)
        {
            float speed = 300.0f; // Speed in pixels per second

            if (inputComponent->isKeyPressed(sf::Keyboard::Key::Up))
            {
                transformComponent->position.y -= speed * deltaTime;
            }
            if (inputComponent->isKeyPressed(sf::Keyboard::Key::Down))
            {
                transformComponent->position.y += speed * deltaTime;
            }
            if (inputComponent->isKeyPressed(sf::Keyboard::Key::Left))
            {
                transformComponent->position.x -= speed * deltaTime;
            }
            if (inputComponent->isKeyPressed(sf::Keyboard::Key::Right))
            {
                transformComponent->position.x += speed * deltaTime;
            }
        }
	}
}
