#include <iostream>
#include <SFML/Graphics.hpp>
#include <memory>
#include "../FrameworkCore/Node.h"
#include "../FrameworkCore/TransformComponent.h"
#include "../FrameworkCore/GraphicsComponent.h"
#include "../FrameworkCore/InputComponent.h"
#include "../FrameworkCore/RenderSystem.h"
#include "../FrameworkCore/InputSystem.h"
#include "../FrameworkCore/TileCollisionSystem.h"
#include "../FrameworkCore/MovementComponent.h"
#include "../FrameworkCore/TileMapComponent.h"
#include "../FrameworkCore/ColliderComponent.h"
#include "TileMap.h"
#include "PacMan.h"
#include "SFML/System.hpp"
#include "SFML/Window.hpp"

using namespace std;

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 16 * 21 * 2, (16 + 16 * 21) * 2 }), "Pac-Man", sf::Style::Close);
    window.setView(sf::View(sf::FloatRect(sf::Vector2f(0, -20),sf::Vector2f(16 * 21, 16 + 16 * 21) )));

    PacMan player;

    RenderSystem renderSystem;
    InputSystem inputSystem;
    TileCollisionSystem tileCollisionSystem;
	
    renderSystem.AddNode(player.GetNode());
	inputSystem.AddNode(player.GetNode());

    const float timePerFrame = 1.0f / 60.0f;
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    std::array<std::string, 21> m_map = {

    " ################### ",
    " #........#........# ",
    " #o##.###.#.###.##o# ",
    " #.................# ",
    " #.##.#.#####.#.##.# ",
    " #....#...#...#....# ",
    " ####.### # ###.#### ",
    "    #.#   0   #.#    ",
    "#####.# ##=## #.#####",
    "     .  #123#  .     ",
    "#####.# ##### #.#####",
    "    #.#       #.#    ",
    " ####.# ##### #.#### ",
    " #........#........# ",
    " #.##.###.#.###.##.# ",
    " #o.#.....P.....#.o# ",
    " ##.#.#.#####.#.#.## ",
    " #....#...#...#....# ",
    " #.######.#.######.# ",
    " #.................# ",
    " ################### "
    };

	TileMap tileMap;
	auto gameMap = tileMap.ParseMapToGame(m_map);
	tileMap.Initialize(gameMap);

	player.GetNode()->AddComponent(tileMap.mTileMapComponent);
    player.GetNode()->AddComponent(new ColliderComponent());
	tileCollisionSystem.AddNode(player.GetNode());
    
 
    while (window.isOpen())
    {
        sf::Time deltaTime = clock.restart();
        timeSinceLastUpdate += deltaTime;

        while (const std::optional event = window.pollEvent())
        {
            // Close window: exit
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                player.InputEvent(keyPressed->code, true);
            }
            else if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
            {
                player.InputEvent(keyReleased->code, false);
            }
        }

        while (timeSinceLastUpdate.asSeconds() > timePerFrame)
        {
            timeSinceLastUpdate -= sf::seconds(timePerFrame);
            inputSystem.Update(timePerFrame);
			tileCollisionSystem.Update(timePerFrame);
        }

        Cell consumedCell;
        const sf::Vector2f pacmanCenter = player.GetPosition() + player.GetBounds().size * 0.5f;

        if (tileMap.TryConsumePickup(pacmanCenter, consumedCell))
        {
            if (consumedCell == Cell::Pellet)
            {
				player.AddScore(10);
            }
            else if (consumedCell == Cell::Energizer)
            {
				player.AddScore(50);
            }
        }

        window.clear();
		tileMap.Draw(window);
		renderSystem.Update(0.f);
		renderSystem.Render(window);
        window.display();
    }
}
