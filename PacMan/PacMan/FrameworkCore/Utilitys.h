#pragma once

constexpr unsigned char MAP_HEIGHT = 21;
constexpr unsigned char MAP_WIDTH = 21;
constexpr unsigned char TILE_SIZE = 16;

enum class Cell
{
	Door,
	Empty,
	Energizer,
	Pellet,
	Wall,
};