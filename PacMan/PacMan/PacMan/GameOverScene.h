#pragma once

#include "../FrameworkCore/IScene.h"
#include <SFML/Graphics.hpp>

class GameOverScene : public IScene
{
public:
	GameOverScene(SceneManager& sceneManager, sf::RenderWindow& window);
	~GameOverScene() override = default;

	void OnEnter() override;
	void OnExit() override;
	void HandleEvent(const sf::Event& event) override;
	void Update(float deltaTime) override;
	void Render() override;

	void SetScore(int score) { mFinalScore = score; };
	void SetWon(bool won) { mPlayerWon = won; };

private:

	sf::Font mFont;
	sf::Text mResultText;
	sf::Text mScoreText;
	sf::Text mRestartText;
	sf::Text mMenuText;

	int mFinalScore = 0;
	bool mPlayerWon = false;
	int mSelectedIndex = 0;
	float mBlinkTimer = 0.f;
	bool mShowCursor = true;

	void RebuildTexts();
	void UpdateSelection();

};