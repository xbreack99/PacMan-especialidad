#include "TileCollisionSystem.h"
#include <SFML/Graphics.hpp>
#include "TransformComponent.h"
#include "MovementComponent.h"
#include "ColliderComponent.h"
#include "TileMapComponent.h"

TileCollisionSystem::TileCollisionSystem()
{
}

TileCollisionSystem::~TileCollisionSystem()
{
}

void TileCollisionSystem::Update(float deltaTime)
{
    for (auto node : mNodes)
    {
        auto transformComponent = node->GetComponent<TransformComponent>();
        auto movementComponent = node->GetComponent<MovementComponent>();
        auto colliderComponent = node->GetComponent<ColliderComponent>();
		auto tileMapComponent = node->GetComponent<TileMapComponent>();

        if (!transformComponent || !movementComponent || !colliderComponent || !tileMapComponent)
        {
            continue;
        }

        sf::Vector2f frameDisplacement = movementComponent->velocity * deltaTime;

        tileMapComponent->MoveAndCollide(
            transformComponent->position,
            colliderComponent->mBounds.size,
            frameDisplacement
        );
    }
}
