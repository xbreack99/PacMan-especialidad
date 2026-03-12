#pragma once

#include "IComponent.h"

#include <SFML/Graphics.hpp>

class MovementComponent : public IComponent
{
public:
	
	MovementComponent();
	
	~MovementComponent();
	
	ComponentType GetType() override;

	sf::Vector2f velocity;
	float speed = 100.f;
};