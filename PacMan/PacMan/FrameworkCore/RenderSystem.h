#pragma once
#include "ISystem.h"
#include <SFML/Graphics.hpp>

class RenderSystem : public ISystem
{
public:
	virtual ~RenderSystem() override;

	virtual void Update(float deltaTime) override;

	void Render(sf::RenderWindow& window);
};