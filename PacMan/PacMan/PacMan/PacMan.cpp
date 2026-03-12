// PacMan.cpp : Defines the entry point for the application.

#include "PacMan.h"
#include "../FrameworkCore/Node.h"
#include "../FrameworkCore/IComponent.h"
#include "../FrameworkCore/GraphicsComponent.h"
#include "../FrameworkCore/TransformComponent.h"
#include "../FrameworkCore/ColliderComponent.h"
#include "../FrameworkCore/ColisionSystem.h"
#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>

PacMan::PacMan()
{
	mPacMan = new Node();
	mGraphicsComponent = new GraphicsComponent();
	mTransformComponent = new TransformComponent();
	mColliderComponent = new ColliderComponent();

	mPacMan->AddComponent(mGraphicsComponent);
	mPacMan->AddComponent(mTransformComponent);
	mPacMan->AddComponent(mColliderComponent);

	mGraphicsComponent->mShape.setRadius(8.f);
	mGraphicsComponent->mShape.setFillColor(sf::Color::Yellow);

	mColliderComponent->mBounds.size = sf::Vector2f(16.f, 16.f);
}

PacMan::~PacMan()
{
}

