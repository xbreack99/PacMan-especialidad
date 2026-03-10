#pragma once

#include "IComponent.h"
#include <SFML/Graphics.hpp>

class GraphicsComponent : public IComponent
{
 public:
    
    GraphicsComponent();
    ~GraphicsComponent();

	ComponentType GetType() override;

public:

	sf::CircleShape mShape;
};