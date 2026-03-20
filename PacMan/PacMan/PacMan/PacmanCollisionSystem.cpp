#include "PacmanCollisionSystem.h"
#include "../FrameworkCore/Node.h"

PacmanCollisionSystem::PacmanCollisionSystem()
{

}

void PacmanCollisionSystem::OnCollision(Node* entityA, Node* entityB)
{
    Node* ghostNode = nullptr;
    Node* pacmanNode = nullptr;

    if (entityA->GetComponent<GhostAIComponent>())
    {
        ghostNode = entityA;
        pacmanNode = entityB;
    }
    else if (entityB->GetComponent<GhostAIComponent>())
    {
        ghostNode = entityB;
        pacmanNode = entityA;
    }

    
    if (!ghostNode || !pacmanNode) return;

    GhostAIComponent* ai = ghostNode->GetComponent<GhostAIComponent>();
    if (!ai) return;

    if (ai->mState == GhostState::Frightened)
    {
        if (mGhostAISystem) mGhostAISystem->ReturnToHouse(ai);

        if (OnGhostEaten) OnGhostEaten(ghostNode);
    }
    else if (ai->mState != GhostState::Eaten)
    {
        if (OnPacmanKilled) OnPacmanKilled();
    }
}

void PacmanCollisionSystem::Update(float deltaTime)
{
}
