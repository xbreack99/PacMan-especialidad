#include "ColisionSystem.h"
#include "Node.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"

ColisionSystem::ColisionSystem()
{
}

ColisionSystem::~ColisionSystem()
{
}

void ColisionSystem::Update(float deltaTime, std::vector<Node*>& entities)
{
    for (size_t i = 0; i < entities.size(); i++)
    {
        auto a = entities[i];

        if (!a->GetComponent<ColliderComponent>())
            continue;

        auto colliderA = a->GetComponent<ColliderComponent>();
        auto transformA = a->GetComponent<TransformComponent>();

        colliderA->mBounds.position.x = transformA->position.x;
        colliderA->mBounds.position.y = transformA->position.y;

        for (size_t j = i + 1; j < entities.size(); j++)
        {
            auto b = entities[j];

            if (!b->GetComponent<ColliderComponent>())
                continue;

            auto colliderB = b->GetComponent<ColliderComponent>();
            auto transformB = b->GetComponent<TransformComponent>();

            colliderB->mBounds.position.x = transformB->position.x;
            colliderB->mBounds.position.y = transformB->position.y;

            if (colliderA->mBounds.findIntersection(colliderB->mBounds))
            {
                OnCollision(a, b);
            }
        }
    }
}

void ColisionSystem::OnCollision(Node* entityA, Node* entityB)
{
	std::cout << "Collision detected between Entity " << entityA << " and Entity " << entityB << std::endl;
}
