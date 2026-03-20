#pragma once

#include "../FrameworkCore/IScene.h"
#include <SFML/Graphics.hpp>

class PauseScene : public IScene
{
public:
	PauseScene(SceneManager& sceneManager, sf::RenderWindow& window);
	~PauseScene() override = default;

	// Inherited via IScene
	void OnEnter() override;
	void OnExit() override;
	void HandleEvent(const sf::Event& event) override;
	void Update(float deltaTime) override;
	void Render() override;

private:

	sf::Font mFont;
	sf::Text mPauseText;
	sf::Text mResumeText;
	sf::Text mMenuText;
	sf::RectangleShape mOverlay;

	int mSelectedIndex = 0;
	float mBlinkTimer = 0.f;
	bool mShowCursor = true;

	void UpdateSelection();
};