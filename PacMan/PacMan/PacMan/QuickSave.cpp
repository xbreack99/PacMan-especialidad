#include "QuickSave.h"

#include "GameScene.h"
#include "../FrameworkCore/TransformComponent.h"
#include "../FrameworkCore/MovementComponent.h"
#include "../FrameworkCore/TileMapComponent.h"

#include <fstream>
#include <iostream>
#include <filesystem>

#include "TileMap.h"
#include "PacMan.h"
#include "Ghost.h"

bool QuickSave::Save(const std::string& path, const SaveState& state)
{
    std::ofstream file(path, std::ios::binary | std::ios::trunc);
    if (!file.is_open())
    {
        return false;
    }
    //Game save data
    file.write(reinterpret_cast<const char*>(&state.score), sizeof(int32_t));
    file.write(reinterpret_cast<const char*>(&state.lives), sizeof(int32_t));
    file.write(reinterpret_cast<const char*>(&state.pelletsLeft), sizeof(int32_t));

    // Pac-Man save data
    file.write(reinterpret_cast<const char*>(&state.pacmanPosX), sizeof(float));
    file.write(reinterpret_cast<const char*>(&state.pacmanPosY), sizeof(float));
    file.write(reinterpret_cast<const char*>(&state.pacmanVelX), sizeof(float));
    file.write(reinterpret_cast<const char*>(&state.pacmanVelY), sizeof(float));

    for (int i = 0; i < SaveState::NUM_GHOSTS; ++i)
    {
        file.write(reinterpret_cast<const char*>(&state.ghosts[i]), sizeof(GhostSave));
    }

    for (int x = 0; x < MAP_WIDTH; ++x)
    {
        for (int y = 0; y < MAP_HEIGHT; ++y)
        {
            const uint8_t val = static_cast<uint8_t>(state.mapGrid[x][y]);
            file.write(reinterpret_cast<const char*>(&val), 1);
        }
    }

    std::cout << "<SaveSystem> Saved: " << path;
    return true;
}

bool QuickSave::Load(const std::string& path,SaveState& outState)
{
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open())
    {
        return false;
    }

    file.read(reinterpret_cast<char*>(&outState.score), sizeof(int32_t));
    file.read(reinterpret_cast<char*>(&outState.lives), sizeof(int32_t));
    file.read(reinterpret_cast<char*>(&outState.pelletsLeft), sizeof(int32_t));

    file.read(reinterpret_cast<char*>(&outState.pacmanPosX), sizeof(float));
    file.read(reinterpret_cast<char*>(&outState.pacmanPosY), sizeof(float));
    file.read(reinterpret_cast<char*>(&outState.pacmanVelX), sizeof(float));
    file.read(reinterpret_cast<char*>(&outState.pacmanVelY), sizeof(float));


    for (int i = 0; i < SaveState::NUM_GHOSTS; ++i)
    {
        file.read(reinterpret_cast<char*>(&outState.ghosts[i]), sizeof(GhostSave));
    }

    for (int x = 0; x < MAP_WIDTH; ++x)
    {
        for (int y = 0; y < MAP_HEIGHT; ++y)
        {
            uint8_t val;
            file.read(reinterpret_cast<char*>(&val), 1);
            outState.mapGrid[x][y] = static_cast<Cell>(val);
        }
    }

	return true;
}

void QuickSave::CaptureFrame(SaveState& outState, GameScene& scene)
{
    outState.score = scene.mScore;
    outState.lives = scene.mLives;
    outState.pelletsLeft = scene.mPelletsLeft;

    if (scene.mPacmanNode)
    {
        auto* transform = scene.mPacmanNode->GetNode()->GetComponent<TransformComponent>();
        auto* movement = scene.mPacmanNode->GetNode()->GetComponent<MovementComponent>();
        
        outState.pacmanPosX = transform->GetPosition().x;
        outState.pacmanPosY = transform->GetPosition().y;
        
        outState.pacmanVelX = movement->velocity.x;
        outState.pacmanVelY = movement->velocity.y;
    }

    for (int i = 0; i < SaveState::NUM_GHOSTS; ++i)
    {
        Ghost* ghost = scene.mGhostNodes[i];
        auto* ai = ghost->GetNode()->GetComponent<GhostAIComponent>();
        auto* t = ghost->GetNode()->GetComponent<TransformComponent>();
        GhostSave& g = outState.ghosts[i];

        if (t)
        {
             g.posX = t->position.x;
             g.posY = t->position.y;
        }
        if (ai)
        {
            g.dirX = ai->mDirection.x;
            g.dirY = ai->mDirection.y;
            g.tileX = ai->mCurrentTile.x;
            g.tileY = ai->mCurrentTile.y;
            g.state = static_cast<uint8_t>(ai->mState);
            g.ghostType = static_cast<uint8_t>(ai->mGhostType);
            g.frightenedTimer = ai->mFrightenedTimer;
            g.phaseIndex = ai->mPhaseIndex;
            g.phaseTimer = ai->mPhaseTimer;
        }
    }

    if (scene.mTileMapNode)
    {
        auto* tilemapComponent = scene.mTileMapNode->GetNode()->GetComponent<TileMapComponent>();

        outState.mapGrid = tilemapComponent->mMap;
    }
}

void QuickSave::ChargeState(const SaveState& state, GameScene& scene)
{
    scene.mScore = state.score;
    scene.mLives = state.lives;
    scene.mPelletsLeft = state.pelletsLeft;

    if (scene.mPacmanNode)
    {
        auto* transform = scene.mPacmanNode->GetNode()->GetComponent<TransformComponent>();
        auto* movement = scene.mPacmanNode->GetNode()->GetComponent<MovementComponent>();
        if (transform)
        {
            transform->position.x = state.pacmanPosX;
            transform->position.y = state.pacmanPosY;
        }
        if (movement) 
        {
            movement->velocity.x = state.pacmanVelX;
            movement->velocity.y = state.pacmanVelY;
        }
    }

    for (int i = 0; i < static_cast<int>(scene.mGhostNodes.size()) &&
        i < SaveState::NUM_GHOSTS; ++i)
    {
        Ghost* ghost = scene.mGhostNodes[i];
        auto* ai = ghost->GetNode()->GetComponent<GhostAIComponent>();
        auto* t = ghost->GetNode()->GetComponent<TransformComponent>();
        const GhostSave& g = state.ghosts[i];

        if (t) { t->position.x = g.posX; t->position.y = g.posY; }
        if (ai)
        {
            ai->mDirection.x = g.dirX;
            ai->mDirection.y = g.dirY;
            ai->mCurrentTile.x = g.tileX;
            ai->mCurrentTile.y = g.tileY;
            ai->mState = static_cast<GhostState>(g.state);
            ai->mFrightenedTimer = g.frightenedTimer;
            ai->mPhaseIndex = g.phaseIndex;
            ai->mPhaseTimer = g.phaseTimer;
            ai->mPosition = { g.posX, g.posY };
        }
    }

    if (scene.mTileMapNode)
    {
        scene.mTileMapNode->mTileMapComponent->mMap = state.mapGrid;
        scene.mTileMapNode->BuildMesh();
    }
}
