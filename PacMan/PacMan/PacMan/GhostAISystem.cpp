#include "GhostAISystem.h"
#include "../FrameworkCore/Node.h"
#include "../FrameworkCore/TransformComponent.h"
#include "../FrameworkCore/TileMapComponent.h"
#include "../FrameworkCore/Utilitys.h"
#include "../FrameworkCore/MovementComponent.h"


void GhostAISystem::Update(float deltaTime)
{
	for (auto node : mNodes)
	{
		auto ghost = node->GetComponent<GhostAIComponent>();
		
		if (!ghost) continue;

		GhostUpdate(ghost, deltaTime);
		
		if (ghost->mState == GhostState::Frightened)
		{
			ghost->mFrightenedTimer -= deltaTime;

			ghost->isFrightened = (ghost->mFrightenedTimer <= GhostAIComponent::FRIGHTENED_THERSHOLD);

			if (ghost->mFrightenedTimer <= 0.f)
			{
				ghost->mState = (ghost->mPhaseIndex % 2 == 0) ? GhostState::Scatter : GhostState::Chase;
				ghost->isFrightened = false;
			}
		}
		switch (ghost->mState)
		{
			case GhostState::Chase: ghost->mTargetTile = CalculateTargetTile(ghost);
				break;
			case GhostState::Scatter: ghost->mTargetTile = ghost->mScatterTargetTile;
				break;
			case GhostState::Eaten: ghost->mTargetTile = ghost->mSpawnTile;
				break;
				//create random movement
			case GhostState::Frightened: ghost->mTargetTile = ghost->mSpawnTile;
				break;
		}

		MoveGhost(ghost, node.get(), deltaTime);
	}
}

void GhostAISystem::GhostUpdate(GhostAIComponent* ghost, float deltaTime)
{
	if (ghost->mState == GhostState::Frightened || ghost->mState == GhostState::Eaten)
	{
		return;
	}

	if (ghost->mPhaseIndex >= 4) return;

	ghost->mPhaseTimer += deltaTime;

	const float duration = GhostAIComponent::sPhaseDurations[ghost->mPhaseIndex];
	if (ghost->mPhaseTimer >= duration)
	{
		ghost->mPhaseTimer = 0.f;
		++ghost->mPhaseIndex;
		ghost->mDirection = Opposite(ghost->mDirection);
		ghost->mState = (ghost->mPhaseIndex % 2 == 0) ? GhostState::Scatter : GhostState::Chase;
	}
}

void GhostAISystem::TriggerFrightened(float duration)
{
	for (auto node : mNodes)
	{
		auto* ghost = node->GetComponent<GhostAIComponent>();

		if (!ghost || ghost->mState == GhostState::Eaten) continue;

		ghost->mState = GhostState::Frightened;
		ghost->mFrightenedTimer = duration;
		ghost->isFrightened = false; 
		ghost->mDirection = Opposite(ghost->mDirection);
	}
}

void GhostAISystem::ReturnToHouse(GhostAIComponent* ghost)
{
	ghost->mState = GhostState::Eaten;
	ghost->mTargetTile = ghost->mSpawnTile;
}

void GhostAISystem::MoveGhost(GhostAIComponent* ghost, Node* node, float deltaTime)
{
	auto* tranform = node->GetComponent<TransformComponent>();
	auto* movement = node->GetComponent<MovementComponent>();

	if (!tranform || !movement) return;

	const float speed = (ghost->mState == GhostState::Frightened) ? ghost->mFrightenedSpeed
		: (ghost->mState == GhostState::Eaten) ? ghost->mEatenSpeed
		: ghost->mSpeed;

	if (!ghost->isMoving)
	{
		ghost->mDirection = ChooseNextDirection(ghost, ghost->mTargetTile);
		ghost->isMoving = true;
	}

	const sf::Vector2i nextTile = ghost->mCurrentTile + ghost->mDirection;
	const sf::Vector2f nextCenter = TileToWorld(nextTile) + sf::Vector2f(TILE_SIZE * 0.5f, TILE_SIZE * 0.5f);

	const sf::Vector2f delta = nextCenter - ghost->mPosition;
	const float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);
	const float step = speed * deltaTime;

	if (step >= distance)
	{
		ghost->mPosition = nextCenter;
		ghost->mCurrentTile = nextTile;
		ghost->isMoving = false;

		if (ghost->mState == GhostState::Eaten && ghost->mCurrentTile == ghost->mSpawnTile)
		{
			ghost->mState = GhostState::Scatter;
			ghost->mPhaseIndex = 0;
			ghost->mPhaseTimer = 0.f;
		}
	}
	else
	{
		ghost->mPosition += (delta / distance) * step;
	}

	tranform->position = ghost->mPosition;
	movement->velocity = sf::Vector2f(
		static_cast<float>(ghost->mDirection.x),
		static_cast<float>(ghost->mDirection.y)) * speed;

}

sf::Vector2i GhostAISystem::CalculateTargetTile(GhostAIComponent* ghost) const
{
	switch (ghost->mGhostType)
	{
	case GhostType::Blinky: return CalculateBlinkyTarget();
		break;
	case GhostType::Pinky: return CalculatePinkyTarget();
		break;
	case GhostType::Inky: return CalculateInkyTarget(ghost);
		break;
	case GhostType::Clyde: return CalculateClydeTarget(ghost);
		break;
	}

	return GetPacManTile();
}

sf::Vector2i GhostAISystem::CalculateBlinkyTarget() const
{
	return GetPacManTile();
}

sf::Vector2i GhostAISystem::CalculatePinkyTarget() const
{
	const sf::Vector2i pacman = GetPacManTile();
	const sf::Vector2i dir = GetPacManDirection();

	return { pacman.x + dir.x * 4, pacman.y + dir.y * 4 };
}

sf::Vector2i GhostAISystem::CalculateInkyTarget(GhostAIComponent* ghost) const
{
	if (!mBlinkyNode) return GetPacManTile();

	const sf::Vector2i pacman = GetPacManTile();
	const sf::Vector2i dir = GetPacManDirection();
	const sf::Vector2i pivot = { pacman.x + dir.x * 2, pacman.y + dir.y * 2 };

	auto* transform = mBlinkyNode->GetComponent<TransformComponent>();
	
	if (!transform) return pivot;

	const sf::Vector2i blinkyTile = WorldToTile(transform->position);

	return { 2 * pivot.x - blinkyTile.x, 2 * pivot.y - blinkyTile.y };

}

sf::Vector2i GhostAISystem::CalculateClydeTarget(GhostAIComponent* ghost) const
{
	const sf::Vector2i pacman = GetPacManTile();
	const int dist = TileDistance(ghost->mCurrentTile, pacman);
	return (dist > CLYDE_FLEE_DISTANCE) ? pacman : ghost->mScatterTargetTile;
}

sf::Vector2i GhostAISystem::ChooseNextDirection(GhostAIComponent* ghost, const sf::Vector2i& target) const
{
	static const std::array<sf::Vector2i, 4> DIRS = { {
			{  0, -1 }, // Up     (checked first, classic priority)
			{ -1,  0 }, // Left
			{  1,  0 }, // Right
			{  0,  1 }, // Down
		} };

	if (ghost->mState == GhostState::Frightened)
	{
		std::array<sf::Vector2i, 4> legal;
		int count = 0;
		for (const auto& d : DIRS)
		{
			if (d == Opposite(ghost->mDirection)) continue;
			if (CanMove(ghost->mCurrentTile + d)) legal[count++] = d;
		}
		if (count == 0) return Opposite(ghost->mDirection);
		return legal[rand() % count];
	}

	sf::Vector2i best = ghost->mDirection;
	int          bestDistSq = std::numeric_limits<int>::max();

	for (const auto& d : DIRS)
	{
		if (d == Opposite(ghost->mDirection)) continue;
		const sf::Vector2i next = ghost->mCurrentTile + d;
		if (!CanMove(next)) continue;

		const int dist = TileDistance(next, target);
		if (dist < bestDistSq)
		{
			bestDistSq = dist;
			best = d;
		}
	}
	return best;
}

bool GhostAISystem::CanMove(const sf::Vector2i& tile) const
{
	if (!mTileMap) return true;
	return !mTileMap->IsBlockingCell(tile.x, tile.y);
}

sf::Vector2i GhostAISystem::WorldToTile(const sf::Vector2f& position) const
{
	return { static_cast<int>(position.x / TILE_SIZE), static_cast<int>(position.y / TILE_SIZE) };
}

sf::Vector2f GhostAISystem::TileToWorld(const sf::Vector2i& tile) const
{
	return { static_cast<float>(tile.x * TILE_SIZE), static_cast<float>(tile.y * TILE_SIZE) };
}

sf::Vector2i GhostAISystem::GetPacManTile() const
{
	if (!mPacManNode) return { 0, 0 };
	auto* transfom = mPacManNode->GetComponent<TransformComponent>();
	return transfom ? WorldToTile(transfom->position) : sf::Vector2i{ 0,0 };
}

sf::Vector2i GhostAISystem::GetPacManDirection() const
{
	if (!mPacManNode) return { 0,0 };
	auto* move = mPacManNode->GetComponent<MovementComponent>();
	if (!move) return { 0,0 };
	if (std::abs(move->velocity.x) > std::abs(move->velocity.y))
	{
		return { (move->velocity.x > 0.0f) ? 1 : -1, 0 };
	}
	if (move->velocity.y != 0.0f)
	{
		return { 0, (move->velocity.y > 0.f) ? 1 : -1 };
	}

	return { 0,0 };
}

int GhostAISystem::TileDistance(const sf::Vector2i& a, const sf::Vector2i& b)
{
	const int dx = a.x - b.x;
	const int dy = a.y - b.y;
	return  dx * dx + dy * dy;
}
