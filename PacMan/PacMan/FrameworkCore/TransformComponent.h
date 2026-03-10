#pragma once

#include "IComponent.h"
#include <SFML/System/Vector2.hpp>

class TransformComponent: public IComponent
{
 public:
    
     TransformComponent();
     TransformComponent(float x, float y);
    
    ~TransformComponent();

    ComponentType GetType() override;

    void SetPosition(float x, float y);

	void Translate(const sf::Vector2f& offset);
	
    sf::Vector2f GetPosition() const;
    
public:
	sf::Vector2f position;
};