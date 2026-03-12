#include "ColliderComponent.h"
#include <SFML/Graphics.hpp>

ColliderComponent::ColliderComponent()
{
	mBounds = sf::FloatRect(sf::Vector2f(0, 0), sf::Vector2f(14, 14));
}

ColliderComponent::ColliderComponent(float x, float y, float width, float height)
{
	mBounds = sf::FloatRect(sf::Vector2f(x, y), sf::Vector2f(width, height));
}

ColliderComponent::~ColliderComponent()
{

}

ComponentType ColliderComponent::GetType()
{
	return ComponentType::COLLIDER;
}

sf::FloatRect ColliderComponent::GetBounds() const
{
	return mBounds;
}
