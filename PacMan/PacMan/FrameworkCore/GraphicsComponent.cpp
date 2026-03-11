#include "GraphicsComponent.h"

#include <SFML/Graphics.hpp>

GraphicsComponent::GraphicsComponent()
{
	mShape.setFillColor(sf::Color::Red);
	mShape.setRadius(8.f);
}

GraphicsComponent::~GraphicsComponent()
{

}

ComponentType GraphicsComponent::GetType()
{
	return ComponentType::GRAPHICS;
}