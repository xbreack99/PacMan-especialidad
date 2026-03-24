#pragma once
#include "../FrameworkCore/ISystem.h"
#include "GhostAIComponent.h"
#include "SFML/Graphics.hpp"

class Node;
class TileMapComponent;
class Ghost;

class GhostAISystem : public ISystem
{
public:
	GhostAISystem() = default;
	~GhostAISystem() override = default;

	void Update(float deltaTime) override;

	void GhostUpdate(GhostAIComponent* ghost, float deltaTime);

	void SetPacman(Node* pacmanNode) { mPacManNode = pacmanNode; }

	void SetTileMap(TileMapComponent* tilemap) { mTileMap = tilemap; }

	void TriggerFrightened(float duration);

	void ReturnToHouse(GhostAIComponent* ghost);

	void MoveGhost(GhostAIComponent* ghost, Node* node, float deltaTime);

private:
	
	// move per Ghost
	sf::Vector2i CalculateTargetTile(GhostAIComponent* ghost) const;
	
	sf::Vector2i CalculateBlinkyTarget() const;
	sf::Vector2i CalculatePinkyTarget() const;

	sf::Vector2i CalculateInkyTarget(GhostAIComponent* ghost) const;
	sf::Vector2i CalculateClydeTarget(GhostAIComponent* ghost) const;

	sf::Vector2i ChooseNextDirection(GhostAIComponent* ghost, const sf::Vector2i& target) const;

	bool CanMove(const sf::Vector2i& tile) const;

	sf::Vector2i WorldToTile(const sf::Vector2f& position) const;
	sf::Vector2f TileToWorld(const sf::Vector2i& tile) const;
	sf::Vector2i GetPacManTile() const;
	sf::Vector2i GetPacManDirection() const;

	static int TileDistance(const sf::Vector2i& a, const sf::Vector2i& b);

	static sf::Vector2i Opposite(const sf::Vector2i& direction)
	{
		return { -direction.x, -direction.y};
	}

	Node* mPacManNode = nullptr;
	Node* mBlinkyNode = nullptr;
	TileMapComponent* mTileMap = nullptr;
	
	static constexpr int CLYDE_FLEE_DISTANCE = 34;

};