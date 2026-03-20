#pragma once

#include "../FrameworkCore/IScene.h"
#include "../FrameworkCore/TileCollisionSystem.h"
#include "GhostAISystem.h"
#include <memory>
#include <SFML/Graphics.hpp>
#include <vector>
#include "PacmanCollisionSystem.h"
#include "../FrameworkCore/RenderSystem.h"
#include "../FrameworkCore/InputSystem.h"

class Node;
class TileMap;
class PacMan;

class GameScene : public IScene
{
public:

	GameScene(SceneManager& sceneManager, sf::RenderWindow& window);
	~GameScene() override;

	void OnEnter() override;
	void OnExit() override;
	void HandleEvent(const sf::Event& event) override;
	void Update(float deltaTime) override;
	void Render() override;

private: 

	void BuildMap();
	void BuildPacman();
	void BuildGhost(GhostType type);
	void ResetEntities();

	void CheckPacmanPickups();
	void KillPacman();
	void CheckWin();
	void UpdateHUD();

	std::vector<Node*> mEntities;
	PacMan* mPacmanNode = nullptr;
	Node* mBlinkyNode = nullptr;
	TileMap* mTileMapNode = nullptr;

	std::unique_ptr<GhostAISystem> mGhostAISystem;
	std::unique_ptr<TileCollisionSystem> mTileCollisionSystem;
	std::unique_ptr<PacmanCollisionSystem> mCollisionSystem;
	std::unique_ptr<RenderSystem> mRenderSystem;
	std::unique_ptr<InputSystem> mInputSystem;


	sf::Font mFont;
	sf::Text mScoreText;
	sf::Text mLivesText;

	int mScore = 0;
	int mLives = 3;
	int mPelletsLeft = 0;
	bool mGameActive = false;
	bool mPacmanDead = false;
	float mDeathTimer = 0.f;

	static constexpr float DEATH_DELAY = 2.f;
	static constexpr float FRIGHTENED_DUR = 8.f;
	static constexpr int PELLET_SCORE = 10;
	static constexpr int   ENERGIZER_SCORE = 50;
	static constexpr int   GHOST_SCORE = 200;
};