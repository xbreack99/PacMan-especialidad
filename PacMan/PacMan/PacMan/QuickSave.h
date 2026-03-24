#pragma once
#include "../FrameworkCore/Utilitys.h"
#include "GhostAIComponent.h"
#include <array>
#include <string>
#include <SFML/System/Vector2.hpp>

struct GhostSave
{
    float        posX;        // pixel position
    float        posY;
    int32_t      dirX;        // current direction tile units
    int32_t      dirY;
    int32_t      tileX;       // current tile
    int32_t      tileY;
    uint8_t      state;        // GhostState
    uint8_t      ghostType;   // GhostType
    float        frightenedTimer;
    int32_t      phaseIndex;
    float        phaseTimer;
    uint8_t      _pad[2];     // alignment padding
};

struct SaveState
{
    static constexpr int      NUM_GHOSTS = 4;
    using MapGrid = std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>;

    int32_t   score = 0;
    int32_t   lives = 3;
    int32_t   pelletsLeft = 0;

    float     pacmanPosX = 0.f;
    float     pacmanPosY = 0.f;
    float     pacmanVelX = 0.f;
    float     pacmanVelY = 0.f;

    GhostSave ghosts[NUM_GHOSTS] = {};

    MapGrid   mapGrid = {};
};

class GameScene;

class QuickSave
{
public:
    static bool Save(const std::string& path, const SaveState& state);

    static bool Load(const std::string& path,SaveState& outState);

    static void CaptureFrame(SaveState& outState, GameScene& scene);

    static void ChargeState(const SaveState& state, GameScene& scene);
};