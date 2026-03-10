#include "RenderSystem.h"

#include "GraphicsComponent.h"
#include "TransformComponent.h"

RenderSystem::~RenderSystem()
{

}

void RenderSystem::Update(float deltaTime)
{

}

void RenderSystem::Render(sf::RenderWindow& window)
{
    for (auto& entity : mNodes)
    {
        auto spriteComponent = entity->GetComponent<GraphicsComponent>();
        auto transformComponent = entity->GetComponent<TransformComponent>();
        if (spriteComponent && transformComponent)
        {
            spriteComponent->mShape.setPosition(transformComponent->position);
            window.draw(spriteComponent->mShape);
        }
    }
}
