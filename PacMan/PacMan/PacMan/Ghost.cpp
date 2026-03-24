#include "Ghost.h"
#include "../FrameworkCore/Node.h"
#include "../FrameworkCore/TransformComponent.h"
#include "../FrameworkCore/MovementComponent.h"
#include "../FrameworkCore/ColliderComponent.h"
#include "../FrameworkCore/TileMapComponent.h"
#include "../FrameworkCore/GraphicsComponent.h"

Ghost::Ghost()
{
    Initialize(GhostType::Blinky);
}

Ghost::Ghost(GhostType type)
{
    mGhost = std::make_shared<Node>();

    mGhostAIComponent = new GhostAIComponent(type);
    mTransformComponent = new TransformComponent(mGhostAIComponent->mPosition.x, mGhostAIComponent->mPosition.y);
    mMovementComponent = new MovementComponent();
    mGraphicsComponent = new GraphicsComponent();
    mMovementComponent->speed = mGhostAIComponent->mSpeed;
    mColiderComponent = new ColliderComponent(1.f, 1.f, 14.f, 14.f);
    mColiderComponent->isTrigger = true;

    mGraphicsComponent->mShape.setRadius(8.f);
    switch (type)
    {
    case GhostType::Blinky:
        mGraphicsComponent->mShape.setFillColor(sf::Color::Red);
        break;
    case GhostType::Pinky:
        mGraphicsComponent->mShape.setFillColor(sf::Color::Magenta);
        break;
    case GhostType::Inky:
        mGraphicsComponent->mShape.setFillColor(sf::Color::Blue);
        break;
    case GhostType::Clyde:
        mGraphicsComponent->mShape.setFillColor(sf::Color::Green);
        break;
    }

    mGhost->AddComponent(mGhostAIComponent);
    mGhost->AddComponent(mTransformComponent);
    mGhost->AddComponent(mGraphicsComponent);
    mGhost->AddComponent(mMovementComponent);
    mGhost->AddComponent(mColiderComponent);
}

Ghost::~Ghost()
{

}

void Ghost::Initialize(GhostType type)
{
    mGhost = std::make_shared<Node>();

    mGhostAIComponent = new GhostAIComponent(type);
    mTransformComponent = new TransformComponent(mGhostAIComponent->mPosition.x, mGhostAIComponent->mPosition.y);
    mMovementComponent = new MovementComponent();
    mGraphicsComponent = new GraphicsComponent();
    mMovementComponent->speed = mGhostAIComponent->mSpeed;
    mColiderComponent = new ColliderComponent(1.f, 1.f, 14.f, 14.f);
    mColiderComponent->isTrigger = true;

    mGraphicsComponent->mShape.setRadius(8.f);
    switch (type)
    {
    case GhostType::Blinky:
        mGraphicsComponent->mShape.setFillColor(sf::Color::Red);
        break;
    case GhostType::Pinky:
        mGraphicsComponent->mShape.setFillColor(sf::Color::Magenta);
        break;
    case GhostType::Inky:
        mGraphicsComponent->mShape.setFillColor(sf::Color::Blue);
        break;
    case GhostType::Clyde:
        mGraphicsComponent->mShape.setFillColor(sf::Color::Green);
        break;
    }

    mGhost->AddComponent(mGhostAIComponent);
    mGhost->AddComponent(mTransformComponent);
    mGhost->AddComponent(mGraphicsComponent);
    mGhost->AddComponent(mMovementComponent);
    mGhost->AddComponent(mColiderComponent);

}