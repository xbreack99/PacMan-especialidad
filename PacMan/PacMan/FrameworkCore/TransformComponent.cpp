#include "TransformComponent.h"

TransformComponent::TransformComponent() :position(0.f, 0.f)
{

}

TransformComponent::TransformComponent(float x, float y)
{
	position.x = x;
	position.y = y;

}

TransformComponent::~TransformComponent()
{
}

void TransformComponent::SetPosition(float x, float y)
{
	position.x = x;
	position.y = y;
}

void TransformComponent::Translate(const sf::Vector2f& offset)
{
	position += offset;
}

sf::Vector2f TransformComponent::GetPosition() const
{
	return position;
}

ComponentType TransformComponent::GetType()
{
	return ComponentType::TRANSFORM;
}
