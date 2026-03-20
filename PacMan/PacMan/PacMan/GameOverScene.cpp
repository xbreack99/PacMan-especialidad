#include "GameOverScene.h"
#include "../FrameworkCore/SceneManager.h"
#include <string>

GameOverScene::GameOverScene(SceneManager& sceneManager, sf::RenderWindow& window)
	: IScene(sceneManager, window)
	, mResultText(mFont)
	, mScoreText(mFont)
	, mRestartText(mFont)
	, mMenuText(mFont)
{
}

void GameOverScene::OnEnter()
{
	mFont.openFromFile("ARCADE_N.ttf");
	RebuildTexts();
	mSelectedIndex = 0;
	UpdateSelection();
}

void GameOverScene::OnExit()
{
}

void GameOverScene::HandleEvent(const sf::Event& event)
{
    if (const auto* keyEvent = event.getIf<sf::Event::KeyPressed>())
    {
        switch (keyEvent->code)
        {
        case sf::Keyboard::Key::Up:
        case sf::Keyboard::Key::W:
            mSelectedIndex = (mSelectedIndex - 1 + 2) % 2;
            UpdateSelection();
            break;

        case sf::Keyboard::Key::Down:
        case sf::Keyboard::Key::S:
            mSelectedIndex = (mSelectedIndex + 1) % 2;
            UpdateSelection();
            break;

        case sf::Keyboard::Key::Enter:
        case sf::Keyboard::Key::Space:
            if (mSelectedIndex == 0)
                mSceneManager.ChangeScene(SCENE_TYPE::GAME);
            else
                mSceneManager.ChangeScene(SCENE_TYPE::MENU);
            break;

        default: break;
        }
    }
}

void GameOverScene::Update(float deltaTime)
{
    mBlinkTimer += deltaTime;
    if (mBlinkTimer >= 0.5f)
    {
        mBlinkTimer = 0.f;
        mShowCursor = !mShowCursor;
        UpdateSelection();
    }
}

void GameOverScene::Render()
{
    mWindow.clear(sf::Color::Black);
    mWindow.draw(mResultText);
    mWindow.draw(mScoreText);
    mWindow.draw(mRestartText);
    mWindow.draw(mMenuText);
    mWindow.display();
}

void GameOverScene::RebuildTexts()
{
    const sf::Vector2u winSize = mWindow.getSize();
    const float cx = static_cast<float>(winSize.x) / 2.f;
    const float cy = static_cast<float>(winSize.y) / 2.f;

    mResultText.setFont(mFont);
    mResultText.setString(mPlayerWon ? "YOU WIN!" : "GAME OVER");
    mResultText.setCharacterSize(56);
    mResultText.setFillColor(mPlayerWon ? sf::Color::Yellow : sf::Color::Red);
    mResultText.setOrigin(mResultText.getLocalBounds().size / 2.f);
    mResultText.setPosition({ cx, cy - 120.f });

    mScoreText.setFont(mFont);
    mScoreText.setString("SCORE: " + std::to_string(mFinalScore));
    mScoreText.setCharacterSize(32);
    mScoreText.setFillColor(sf::Color::White);
    mScoreText.setOrigin(mScoreText.getLocalBounds().size / 2.f);
    mScoreText.setPosition({ cx, cy - 40.f });

    mRestartText.setFont(mFont);
    mRestartText.setString("PLAY AGAIN");
    mRestartText.setCharacterSize(28);
    mRestartText.setOrigin(mRestartText.getLocalBounds().size / 2.f);
    mRestartText.setPosition({ cx, cy + 40.f });

    mMenuText.setFont(mFont);
    mMenuText.setString("MAIN MENU");
    mMenuText.setCharacterSize(28);
    mMenuText.setOrigin(mMenuText.getLocalBounds().size / 2.f);
    mMenuText.setPosition({ cx, cy + 90.f });
}

void GameOverScene::UpdateSelection()
{
    auto highlight = [&](sf::Text& text, bool selected)
        {
            if (selected)
            {
                text.setFillColor(mShowCursor ? sf::Color::Yellow : sf::Color(180, 180, 0));
            }
            else
            {
                text.setFillColor(sf::Color::White);
            }
        };
    highlight(mRestartText, mSelectedIndex == 0);
    highlight(mMenuText, mSelectedIndex == 1);
}
