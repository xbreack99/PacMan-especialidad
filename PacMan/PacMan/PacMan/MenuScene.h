#pragma once

#include "../FrameworkCore/IScene.h"
#include <SFML/Graphics.hpp>

class MenuScene : public IScene
{
public:
	MenuScene(SceneManager& sceneManager, sf::RenderWindow& window);
	
	~MenuScene() override = default;

	void OnEnter() override;

	void OnExit() override;

	void HandleEvent(const sf::Event& event) override;

	void Update(float deltaTime) override;

	void Render() override;

private:
	sf::Font mFont;
	sf::Text mTitle;
	sf::Text mStartText;
	sf::Text mQuitText;

	int mSelectedIndex = 0;
	float mBlinkTimer = 0.f;
	bool mShowCursor = true;

	void UpdateSelection();
};