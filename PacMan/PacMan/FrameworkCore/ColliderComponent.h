#pragma once

#include "IComponent.h"
#include <SFML/Graphics.hpp>

class ColliderComponent : public IComponent
{
public:

	ColliderComponent();

	ColliderComponent(float x, float y, float width, float height);

	~ColliderComponent();

	// Inherited via IComponent
	ComponentType GetType() override;

	sf::FloatRect GetBounds() const;

public:

	bool isTrigger;

	sf::FloatRect mBounds;
};