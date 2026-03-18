
#include "PacMan.h"
#include "../FrameworkCore/Node.h"
#include "../FrameworkCore/IComponent.h"
#include "../FrameworkCore/GraphicsComponent.h"
#include "../FrameworkCore/TransformComponent.h"
#include "../FrameworkCore/ColliderComponent.h"
#include "../FrameworkCore/ColisionSystem.h"
#include "../FrameworkCore/InputComponent.h"
#include "../FrameworkCore/MovementComponent.h"

#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>

PacMan::PacMan()
{
	mPacMan = std::make_shared<Node>();

	mGraphicsComponent = new GraphicsComponent();
	mTransformComponent = new TransformComponent(2.f * TILE_SIZE + 2.f, 2.f * TILE_SIZE + 1.f);
	mColliderComponent = new ColliderComponent();
	mMovementComponent = new MovementComponent();

	mInputComponent = new InputComponent();
	mInputComponent->setKey(sf::Keyboard::Key::Up, false);
	mInputComponent->setKey(sf::Keyboard::Key::Down, false);
	mInputComponent->setKey(sf::Keyboard::Key::Left, false);
	mInputComponent->setKey(sf::Keyboard::Key::Right, false);

	mPacMan->AddComponent(mGraphicsComponent);
	mPacMan->AddComponent(mTransformComponent);
	mPacMan->AddComponent(mColliderComponent);
	mPacMan->AddComponent(mInputComponent);
	mPacMan->AddComponent(mMovementComponent);

	mGraphicsComponent->mShape.setRadius(8.f);
	mGraphicsComponent->mShape.setFillColor(sf::Color::Yellow);

	mColliderComponent->mBounds.size = sf::Vector2f(16.f, 16.f);
}

PacMan::~PacMan()
{
}

void PacMan::Update()
{

}

void PacMan::InputEvent(sf::Keyboard::Key key, bool isPressed)
{
	mInputComponent->setKey(key, isPressed);
}

sf::Vector2f PacMan::GetPosition() const
{
	return mTransformComponent->position;
}

sf::FloatRect PacMan::GetBounds() const
{
	return sf::FloatRect(mTransformComponent->position, mColliderComponent->mBounds.size);
}

