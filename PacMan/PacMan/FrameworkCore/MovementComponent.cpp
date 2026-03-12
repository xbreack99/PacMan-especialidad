#include "MovementComponent.h"

MovementComponent::MovementComponent()
{
	speed = 100.f;
	velocity = sf::Vector2f(0.f, 0.f);
}

MovementComponent::~MovementComponent()
{
}

ComponentType MovementComponent::GetType()
{
	return ComponentType::MOVEMENT;
}
