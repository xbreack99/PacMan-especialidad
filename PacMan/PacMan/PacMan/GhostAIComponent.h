#pragma once
#include "../FrameworkCore/IComponent.h"
#include "../FrameworkCore/Utilitys.h"
#include "SFML/Graphics.hpp"

enum class GhostState
{
	Chase,
	Scatter,
	Frightened,
	Eaten,
};

enum class GhostType
{
	Blinky,
	Pinky,
	Inky,
	Clyde
};

class GhostAIComponent : public IComponent
{
public:
	explicit GhostAIComponent(GhostType type);
	~GhostAIComponent() override = default;

	ComponentType GetType() override { return ComponentType::GHOST_AI; }

	GhostType mGhostType;

	GhostState mState = GhostState::Scatter;

	float mFrightenedTimer = 20.f;
	bool isFrightened = false;

	sf::Vector2i mTargetTile;
	sf::Vector2i mScatterTargetTile;
	sf::Vector2i mSpawnTile;
	sf::Vector2i mDirection;
	sf::Vector2i mCurrentTile;

	sf::Vector2f mPosition;
	bool isMoving = false;

	float mSpeed = 20.f;
	float mFrightenedSpeed = 20.f;
	float mEatenSpeed = 50.f;

	int mPhaseIndex = 0;
	float mPhaseTimer = 0.f;

	static constexpr float sPhaseDurations[4] = { 7.f, 20.f, 7.f, 20.f };
	static constexpr float  FRIGHTENED_THERSHOLD = 20.f;

};