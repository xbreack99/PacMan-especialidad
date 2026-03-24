#pragma once
#include "../FrameworkCore/Utilitys.h"
#include <array>
#include <string>
#include <SFML/System/Vector2.hpp>

class MapLoader
{
public:
	using MapGrid = std::array<std::array<Cell, MAP_HEIGHT>, MAP_WIDTH>;

	static bool LoadFromText(const std::string& txtPath, MapGrid& outGrid, sf::Vector2i& outPlayerStart);

	static bool LoadFromBinary(const std::string& binPath, MapGrid& outGrid, sf::Vector2i& outPlayerStart);

	static bool SaveBinary(const std::string& binPath, const MapGrid& grid);

private:
	static Cell CharToCell(char c);
};