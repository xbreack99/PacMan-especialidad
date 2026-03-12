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
#include "SFML/System.hpp"
#include "SFML/Window.hpp"
using namespace std;

int main()
{
    // Create the main window
    sf::RenderWindow window(sf::VideoMode({ 16 * 21 * 2, (16 + 16 * 21) * 2 }), "Pac-Man", sf::Style::Close);
    //Resizing the window.
    window.setView(sf::View(sf::FloatRect(sf::Vector2f(0, 0),sf::Vector2f(16 * 21, 16 + 16 * 21) )));
    shared_ptr<Node> Pacman = make_shared<Node>();

	Pacman->AddComponent(new TransformComponent(2.f * TILE_SIZE + 2.f, 2.f * TILE_SIZE + 1.f));
    Pacman->AddComponent(new GraphicsComponent());
	Pacman->AddComponent(new MovementComponent());
    RenderSystem renderSystem;

    InputSystem inputSystem;
    
    auto inputComponent = std::make_shared<InputComponent>();
    inputComponent->setKey(sf::Keyboard::Key::Up, false);
    inputComponent->setKey(sf::Keyboard::Key::Down, false);
    inputComponent->setKey(sf::Keyboard::Key::Left, false);
    inputComponent->setKey(sf::Keyboard::Key::Right, false);

    TileCollisionSystem tileCollisionSystem;

	Pacman->AddComponent(inputComponent.get());

	renderSystem.AddNode(Pacman);
	inputSystem.AddNode(Pacman);

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
	Pacman->AddComponent(tileMap.mTileMapComponent);
	Pacman->AddComponent(new ColliderComponent());
	tileCollisionSystem.AddNode(Pacman);
    // Start the game loop
    while (window.isOpen())
    {
        sf::Time deltaTime = clock.restart();
        timeSinceLastUpdate += deltaTime;
        // Process events
        while (const std::optional event = window.pollEvent())
        {
            // Close window: exit
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (inputComponent)
                {
                    inputComponent->setKey(keyPressed->code, true);
                }
            }
            else if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
            {
                if (inputComponent)
                {
                    inputComponent->setKey(keyReleased->code, false);
                }
            }
        }

        while (timeSinceLastUpdate.asSeconds() > timePerFrame)
        {
            timeSinceLastUpdate -= sf::seconds(timePerFrame);
            inputSystem.Update(timePerFrame);
			tileCollisionSystem.Update(timePerFrame);
        }

        window.clear();
		tileMap.Draw(window);
		renderSystem.Update(0.f);
		renderSystem.Render(window);
        window.display();
    }
}
