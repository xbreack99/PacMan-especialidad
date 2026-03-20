#include <iostream>
#include <SFML/Graphics.hpp>
#include <memory>

#include "../FrameworkCore/SceneManager.h"
#include "MenuScene.h"
#include "PauseScene.h"
#include "GameScene.h"
#include "GameOverScene.h"


using namespace std;

int main()
{
    //sf::RenderWindow window(sf::VideoMode({ 16 * 21 * 2, (16 + 16 * 21) * 2 }), "Pac-Man", sf::Style::Close);
    //window.setView(sf::View(sf::FloatRect(sf::Vector2f(0, -20),sf::Vector2f(16 * 21, 16 + 16 * 21) )));

    const unsigned int WIN_W = MAP_WIDTH * TILE_SIZE;        // 448
    const unsigned int WIN_H = MAP_HEIGHT * TILE_SIZE + 48u;  // 544

    sf::RenderWindow window(
        sf::VideoMode({ WIN_W, WIN_H }),
        "PAC-MAN",
        sf::Style::Titlebar | sf::Style::Close
    );

    window.setFramerateLimit(60);
    SceneManager sceneManager(window);

    sceneManager.RegisterScene(SCENE_TYPE::MENU, std::make_unique<MenuScene>(sceneManager, window));
    sceneManager.RegisterScene(SCENE_TYPE::GAME,
        std::make_unique<GameScene>(sceneManager, window));

    sceneManager.RegisterScene(SCENE_TYPE::PAUSE,
        std::make_unique<PauseScene>(sceneManager, window));

    sceneManager.RegisterScene(SCENE_TYPE::GAMEOVER,
        std::make_unique<GameOverScene>(sceneManager, window));

    sceneManager.ChangeScene(SCENE_TYPE::MENU);
    sf::Clock clock;

    while (window.isOpen() && sceneManager.IsRunning())
    {
        const float deltaTime = std::min(clock.restart().asSeconds(), 0.05f);

        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                sceneManager.Quit();
            }
            sceneManager.HandleEvent(*event);
        }

        sceneManager.Update(deltaTime);

        sceneManager.Render();
    }

    return 0;

}
