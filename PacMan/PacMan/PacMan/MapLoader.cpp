#include "MapLoader.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

bool MapLoader::LoadFromText(const std::string& txtPath, MapGrid& outGrid, sf::Vector2i& outPlayerStart)
{
	std::ifstream file(txtPath);
	if (!file.is_open())
	{
		std::cerr << "[MapLoader] Cannot open: " << txtPath << "\n";
		return false;
	}

	// Collect non-comment, non-blank lines
	std::vector<std::string> rows;
	std::string line;
	while (std::getline(file, line))
	{
		// Skip // comments
		if (line.size() >= 2 && line[0] == '/' && line[1] == '/') continue;
		if (line.empty()) continue;
		rows.push_back(line);
	}

	if (static_cast<int>(rows.size()) != MAP_HEIGHT)
	{
		std::cerr << "[MapLoader] " << txtPath
			<< " has " << rows.size() << " rows, expected "
			<< MAP_HEIGHT << "\n";
		return false;
	}

	outPlayerStart = { 0, 0 };
	outGrid = {};

	for (int y = 0; y < MAP_HEIGHT; ++y)
	{
		const std::string& row = rows[y];
		for (int x = 0; x < MAP_WIDTH; ++x)
		{
			const char c = (x < static_cast<int>(row.size())) ? row[x] : ' ';
			if (c == 'P')
				outPlayerStart = { x * TILE_SIZE, y * TILE_SIZE };
			outGrid[x][y] = CharToCell(c);
		}
	}

	return true;
}

bool MapLoader::LoadFromBinary(const std::string& binPath, MapGrid& outGrid, sf::Vector2i& outPlayerStart)
{
	std::ifstream file(binPath, std::ios::binary);
	if (!file.is_open())
	{
		return false;
	}
	uint8_t width{}, height{};

	file.read(reinterpret_cast<char*>(&width), 1);
	file.read(reinterpret_cast<char*>(&height), 1);

	if (width != MAP_WIDTH || height != MAP_HEIGHT)
	{
		return false;
	}

	outPlayerStart = { 0, 0 };
	outGrid = {};

	for (int x = 0; x < MAP_WIDTH; ++x)
	{
		for (int y = 0; y < MAP_HEIGHT; ++y)
		{
			uint8_t val{};
			file.read(reinterpret_cast<char*>(&val), 1);
			outGrid[x][y] = static_cast<Cell>(val);
		}
	}

	return file.good();
}

bool MapLoader::SaveBinary(const std::string& binPath, const MapGrid& grid)
{
	std::ofstream file(binPath, std::ios::binary);
	if (!file.is_open())
	{
		std::cerr << "[MapLoader] Cannot write: " << binPath << "\n";
		return false;
	}

	const uint8_t w = static_cast<uint8_t>(MAP_WIDTH);
	const uint8_t h = static_cast<uint8_t>(MAP_HEIGHT);
	file.write(reinterpret_cast<const char*>(&w), 1);
	file.write(reinterpret_cast<const char*>(&h), 1);

	for (int x = 0; x < MAP_WIDTH; ++x)
		for (int y = 0; y < MAP_HEIGHT; ++y)
		{
			const uint8_t val = static_cast<uint8_t>(grid[x][y]);
			file.write(reinterpret_cast<const char*>(&val), 1);
		}

	std::cout << "[MapLoader] Saved binary: " << binPath << "\n";
	return true;
}

Cell MapLoader::CharToCell(char c)
{
	switch (c)
	{
	case '#': return Cell::Wall;
	case '.': return Cell::Pellet;
	case 'o': return Cell::Energizer;
	case '=': return Cell::Door;
	default: return Cell::Empty;
	}
}
