
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
	mTransformComponent = new TransformComponent(13.f * TILE_SIZE, 23.f * TILE_SIZE);
	mColliderComponent = new ColliderComponent(1.f, 1.f, 14.f, 14.f);
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

	mTransformComponent->SetPosition(10, 10);
	mColliderComponent->mBounds.size = sf::Vector2f(14.f, 14.f);
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

void PacMan::SetPosition(int x, int y)
{
	mTransformComponent->SetPosition(x, y);
}

sf::FloatRect PacMan::GetBounds() const
{
	return sf::FloatRect(mTransformComponent->position, mColliderComponent->mBounds.size);
}

