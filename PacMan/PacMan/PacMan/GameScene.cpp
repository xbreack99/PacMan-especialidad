#include "GameScene.h"
#include "../FrameworkCore/SceneManager.h"
#include "../FrameworkCore/Node.h"
#include "../FrameworkCore/TransformComponent.h"
#include "../FrameworkCore/MovementComponent.h"
#include "../FrameworkCore/ColliderComponent.h"
#include "../FrameworkCore/TileMapComponent.h"
#include "../FrameworkCore/GraphicsComponent.h"
#include "GhostAIComponent.h"
#include "../FrameworkCore/Utilitys.h"
#include "TileMap.h"
#include "PacMan.h"
#include <memory.h>
#include <string>
#include "Ghost.h"
#include "QuickSave.h"
#include "GameOverScene.h"

GameScene::GameScene(SceneManager& sceneManager, sf::RenderWindow& window) :
	IScene(sceneManager, window)
	, mScoreText(mFont)
	, mLivesText(mFont)
    , mSaveText(mFont)
    , mEnergizerTimerText(mFont)
{
    mGhostAISystem = std::make_unique<GhostAISystem>();
    mTileCollisionSystem = std::make_unique<TileCollisionSystem>();
    mCollisionSystem = std::make_unique<PacmanCollisionSystem>();
    mRenderSystem = std::make_unique<RenderSystem>();
    mInputSystem = std::make_unique<InputSystem>();

    mCollisionSystem->SetGhostAISystem(mGhostAISystem.get());

    mCollisionSystem->OnPacmanKilled = [this]() { KillPacman();  };
    mCollisionSystem->OnGhostEaten = [this](Node*) {
		mScore += GHOST_SCORE;
		};

    mModManager.LoadMod("SpeedBoost.dll");
}

GameScene::~GameScene()
{
	for (Node* node : mEntities) delete node;
}

void GameScene::OnEnter()
{
    mFont.openFromFile("ARCADE_N.ttf");

    mScoreText.setFont(mFont);
    mScoreText.setCharacterSize(18);
    mScoreText.setFillColor(sf::Color::White);
    mScoreText.setPosition({ 8.f, 4.f });

    mLivesText.setFont(mFont);
    mLivesText.setCharacterSize(18);
    mLivesText.setFillColor(sf::Color::White);
    mLivesText.setPosition({ 8.f, 24.f });

    mEnergizerTimerText.setFont(mFont);
    mEnergizerTimerText.setCharacterSize(16);
    mEnergizerTimerText.setFillColor(sf::Color::Cyan);
    mEnergizerTimerText.setPosition({ 8.f, 46.f });


    mSaveText.setFont(mFont);
    mSaveText.setCharacterSize(18);
    mSaveText.setFillColor(sf::Color::Green);
    mSaveText.setString("");

    mScore = 0;
    mLives = 3;
    mGameActive = true;
    ResetEntities();
    UpdateHUD();
}

void GameScene::OnExit()
{

    mGhostAISystem->mNodes.clear();
    mGhostAISystem->SetPacman(nullptr);
    mGhostAISystem->SetTileMap(nullptr);
    mTileCollisionSystem->mNodes.clear();
    mTileCollisionSystem->SetTileMap(nullptr);
    mCollisionSystem->mNodes.clear();
    mRenderSystem->mNodes.clear();
    mInputSystem->mNodes.clear();

    mEntities.clear();
    mPacmanNode = nullptr;
    mGhostNodes.clear();
    mTileMapNode = nullptr;
}

void GameScene::ResetEntities()
{

    mGhostAISystem->mNodes.clear();
    mGhostAISystem->SetPacman(nullptr);
    mGhostAISystem->SetTileMap(nullptr);
    mTileCollisionSystem->mNodes.clear();
    mTileCollisionSystem->SetTileMap(nullptr);
    mCollisionSystem->mNodes.clear();
    mRenderSystem->mNodes.clear();
    mInputSystem->mNodes.clear();

    mEntities.clear();
    mPacmanNode = nullptr;
    mGhostNodes.clear();
    mTileMapNode = nullptr;

    BuildMap();
    BuildPacman();
    BuildGhost();


    mGhostAISystem->SetPacman(mPacmanNode->GetNode().get());
    mGhostAISystem->SetTileMap(mTileMapNode->GetNode()->GetComponent<TileMapComponent>());

    mPelletsLeft = 0;
    mPacmanDead = false;
    mDeathTimer = 0.f;
}


void GameScene::BuildMap()
{
    mTileMapNode = new TileMap("map2.txt");

    mEntities.push_back(mTileMapNode->GetNode().get());

    mTileCollisionSystem->SetTileMap(mTileMapNode->mTileMapComponent);
}

void GameScene::BuildPacman()
{
    mPacmanNode = new PacMan();
    mEntities.push_back(mPacmanNode->GetNode().get());
    auto startPosition = mTileMapNode->mPlayerStartposition;
    mPacmanNode->GetNode()->AddComponent(mTileMapNode->GetNode()->GetComponent<TileMapComponent>());
    mPacmanNode->SetPosition(startPosition.x, startPosition.y);

    mTileCollisionSystem->AddNode(mPacmanNode->GetNode());
    mCollisionSystem->AddNode(mPacmanNode->GetNode());
    mRenderSystem->AddNode(mPacmanNode->GetNode());
    mInputSystem->AddNode(mPacmanNode->GetNode());

    mCtx.pacmanMovement = mPacmanNode->GetNode()->GetComponent<MovementComponent>();
    mCtx.ghostAISystem = mGhostAISystem.get();
    mCtx.lives = &mLives;
    mCtx.score = &mScore;
}

void GameScene::BuildGhost()
{
    for (int i = 0; i < 4; ++i)
    {
        Ghost* ghost = new  Ghost(static_cast<GhostType>(i));
        mGhostNodes.push_back(ghost);
        mEntities.push_back(ghost->GetNode().get());
    
        mGhostAISystem->mNodes.push_back(ghost->GetNode());
        mRenderSystem->mNodes.push_back(ghost->GetNode());
        mCollisionSystem->mNodes.push_back(ghost->GetNode());
        mTileCollisionSystem->AddNode(ghost->GetNode());
    };

}

void GameScene::HandleEvent(const sf::Event& event)
{
    if (const auto* key = event.getIf<sf::Event::KeyPressed>())
    {
        if (key->code == sf::Keyboard::Key::Escape)
        {
            mSceneManager.PushScene(SCENE_TYPE::PAUSE);
        }
        switch (key->code)
        {
        case sf::Keyboard::Key::F1 : 
            mModManager.ToogleMod(0, mCtx); 
            break;
        case sf::Keyboard::Key::F5:
            SaveGame();
            break;
        case sf::Keyboard::Key::F9:
            LoadGame();
            break;
        default:
            break;
        }

        mPacmanNode->InputEvent(key->code, true);
    }
    else if (event.is<sf::Event::Closed>())
    {
        mWindow.close();
    }

    else if (const auto* keyReleased = event.getIf<sf::Event::KeyReleased>())
    {
        mPacmanNode->InputEvent(keyReleased->code, false);
    }
}

void GameScene::Update(float deltaTime)
{
    if (!mGameActive) return;

    if (mSaveTimer > 0.f)
        mSaveTimer -= deltaTime;

    if (mTimerEnergize > 0.f)
    {
        mTimerEnergize -= deltaTime;

        if(mTimerEnergize <= 0.f)
        {
            for (auto* ghost : mGhostNodes)
            {
                auto* ai = ghost->GetNode()->GetComponent<GhostAIComponent>();
                auto* graphics = ghost->GetNode()->GetComponent<GraphicsComponent>();
                switch (ai->mGhostType)
                {
                case GhostType::Blinky:
                    graphics->mShape.setFillColor(sf::Color::Red);
                    break;
                case GhostType::Pinky:
                    graphics->mShape.setFillColor(sf::Color::Magenta);
                    break;
                case GhostType::Inky:
                    graphics->mShape.setFillColor(sf::Color::Blue);
                    break;
                case GhostType::Clyde:
                    graphics->mShape.setFillColor(sf::Color::Green);
                    break;
                }
            }
        }
    }

    if (mPacmanDead)
    {
        mDeathTimer -= deltaTime;
        if (mDeathTimer <= 0.f)
        {
            if (mLives <= 0)
            {
                auto* goScene = dynamic_cast<GameOverScene*>(
                    mSceneManager.GetScene(SCENE_TYPE::GAMEOVER));
                if (goScene) { goScene->SetScore(mScore); goScene->SetWon(false); }
                mSceneManager.ChangeScene(SCENE_TYPE::GAMEOVER);
                return;
            }
            ResetEntities();
            UpdateHUD();
        }
        return;
    }

    if (auto* transform = mPacmanNode->GetNode()->GetComponent<TransformComponent>())
    {
        mTileMapNode->TryTunnel(transform->position);
    }

    mInputSystem->Update(deltaTime);
    mGhostAISystem->Update(deltaTime);
    mCollisionSystem->Update(deltaTime, mEntities);
    mRenderSystem->Update(deltaTime);
    mTileCollisionSystem->Update(deltaTime);
    mModManager.Update(deltaTime, mCtx);
    CheckPacmanPickups();
    CheckWin();
    UpdateHUD();
}

void GameScene::CheckPacmanPickups()
{
    if (!mPacmanNode) return;
    auto* transform = mPacmanNode->GetNode()->GetComponent<TransformComponent>();
    if (!transform) return;

    const sf::Vector2f center = transform->position + sf::Vector2f(7.f, 7.f);
    Cell consumed = Cell::Empty;

    if (mTileMapNode->TryConsumePickup(center, consumed))
    {
        switch (consumed)
        {
        case Cell::Pellet:
            mScore += PELLET_SCORE;
            --mPelletsLeft;
            break;
        case Cell::Energizer:
            mScore += ENERGIZER_SCORE;
            --mPelletsLeft;
            mGhostAISystem->TriggerFrightened(FRIGHTENED_DUR);
            mTimerEnergize = FRIGHTENED_DUR;
            break;
        default: break;
        }
    }
}

void GameScene::KillPacman()
{
    if (mPacmanDead) return;
    mPacmanDead = true;
    mDeathTimer = DEATH_DELAY;
    --mLives;
    if (auto* m = mPacmanNode->GetNode()->GetComponent<MovementComponent>())
        m->velocity = { 0.f, 0.f };
}

void GameScene::CheckWin()
{
    if (mPelletsLeft > 0) return;
}

void GameScene::UpdateHUD()
{
    mScoreText.setString("SCORE: " + std::to_string(mScore));
    mLivesText.setString("LIVES: " + std::to_string(mLives));

    const std::string timerStr = GetEnergizerTime();
    mEnergizerTimerText.setString(timerStr.empty() ? "" : "POWER: " + timerStr);
}

void GameScene::SaveGame()
{
    SaveState state;
    QuickSave::CaptureFrame(state, *this);
    if (QuickSave::Save("save.bin", state))
    {
        mSaveText.setString("GAME SAVED");
        mSaveTimer = SAVE_NOTICE_DUR;
    }
}

void GameScene::LoadGame()
{
    SaveState state;
    QuickSave::Load("save.bin",state);
    ResetEntities();
    QuickSave::ChargeState(state, *this);
    UpdateHUD();

    mSaveText.setString("GAME lOADED");
    mSaveTimer = SAVE_NOTICE_DUR;

}

std::string GameScene::GetEnergizerTime() const
{
    if (!IsActiveEnergize()) return "";
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1) << mTimerEnergize;
    return oss.str();
}

void GameScene::Render()
{
    mWindow.clear(sf::Color::Black);

    if (mTileMapNode)
    {
        mTileMapNode->Draw(mWindow);
    }
    
    if (IsActiveEnergize())
    {
        for (auto* ghost : mGhostNodes)
        {
            auto* ai = ghost->GetNode()->GetComponent<GhostAIComponent>();
            auto* graphics = ghost->GetNode()->GetComponent<GraphicsComponent>();
            if (ai && ai->mState == GhostState::Frightened)
            {
                graphics->mShape.setFillColor(sf::Color::White);
            }
        }
    }
    mRenderSystem->Render(mWindow);
    mWindow.draw(mScoreText);
    mWindow.draw(mLivesText);
    if (IsActiveEnergize())
        mWindow.draw(mEnergizerTimerText);
    
    if (mSaveTimer > 0.f)
    {
        const sf::Vector2u sz = mWindow.getSize();
        mSaveText.setOrigin(
            mSaveText.getLocalBounds().size / 2.f);
        mSaveText.setPosition({
            static_cast<float>(sz.x) / 2.f,
            static_cast<float>(sz.y) * 0.45f });
        mWindow.draw(mSaveText);
    }

    mWindow.display();
}