#include "InputSystem.h"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Graphics.hpp>
#include "InputComponent.h"
#include "TransformComponent.h"
#include "MovementComponent.h"

InputSystem::~InputSystem()
{
}

void InputSystem::Update(float deltaTime)
{
	for (auto node : mNodes)
	{
        auto transformComponent = node->GetComponent<TransformComponent>();
        auto inputComponent = node->GetComponent<InputComponent>();
		auto movementComponent = node->GetComponent<MovementComponent>();

        sf::Vector2f direction(0.f, 0.f);

        if (transformComponent && inputComponent)
        {

            if (inputComponent->isKeyPressed(sf::Keyboard::Key::Up))
            {
                direction.y -= 1.f;
            }
            if (inputComponent->isKeyPressed(sf::Keyboard::Key::Down))
            {
                direction.y += 1.f;
            }
            if (inputComponent->isKeyPressed(sf::Keyboard::Key::Left))
            {
                direction.x -= 1.f;
            }
            if (inputComponent->isKeyPressed(sf::Keyboard::Key::Right))
            {
                direction.x += 1.f;
            }
        }

		movementComponent->velocity = direction * movementComponent->speed;
	}
}
